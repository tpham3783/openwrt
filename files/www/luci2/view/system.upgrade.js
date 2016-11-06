L.ui.view.extend({
	title: L.tr('Flash operations'),

	cioUpgrade: L.rpc.declare({
		object: 'luci2.ciosystem',
		method: 'cio_upgrade',
		params: [ 'kep' ],
		expect: { '': { } }
	}),

	cioCleanUpgrade: L.rpc.declare({
		object: 'luci2.ciosystem',
		method: 'cio_clean_upgrade',
		params: [ 'keep' ],
		expect: { '': { } }
	}),

	sysUpgrade: L.rpc.declare({
		object: 'luci2.system',
		method: 'start_upgrade',
		params: [ 'keep' ],
		expect: { '': { } }
	}),

	testUpgrade: L.rpc.declare({
		object: 'luci2.system',
		method: 'upgrade_test',
		expect: { '': { } }
	}),

	startUpgrade: L.rpc.declare({
		object: 'luci2.system',
		method: 'upgrade_start',
		params: [ 'keep' ]
	}),

	cleanUpgrade: L.rpc.declare({
		object: 'luci2.system',
		method: 'upgrade_clean'
	}),

	restoreBackup: L.rpc.declare({
		object: 'luci2.system',
		method: 'backup_restore'
	}),

	cleanBackup: L.rpc.declare({
		object: 'luci2.system',
		method: 'backup_clean'
	}),

	getBackupConfig: L.rpc.declare({
		object: 'luci2.system',
		method: 'backup_config_get',
		expect: { config: '' }
	}),

	setBackupConfig: L.rpc.declare({
		object: 'luci2.system',
		method: 'backup_config_set',
		params: [ 'data' ]
	}),

	listBackup: L.rpc.declare({
		object: 'luci2.system',
		method: 'backup_list',
		expect: { files: [ ] }
	}),

	testReset: L.rpc.declare({
		object: 'luci2.system',
		method: 'reset_test',
		expect: { supported: false }
	}),

	startReset: L.rpc.declare({
		object: 'luci2.system',
		method: 'reset_start'
	}),

	startReboot: L.rpc.declare({
		object: 'luci2.system',
		method: 'reboot'
	}),

	handleFlashUpload: function() {
		var self = this;
		L.ui.upload(
			L.tr('Firmware upload'),
			L.tr('Select the sysupgrade image to flash and click "%s" to proceed.').format(L.tr('Ok')), {
				filename: '/tmp/firmware.bin',
				success: function(info) {
					self.handleFlashVerify(info);
				}
			}
		);
	},

	handleFlashVerify: function(info) {
		var self = this;
		self.testUpgrade().then(function(res) {
			if (res.code == 0)
			{
				L.ui.dialog(
					L.tr('Verify firmware'), [
						$('<p />').text(L.tr('The firmware image was uploaded completely. Please verify the checksum and file size below, then click "%s" to start the flash procedure.').format(L.tr('Ok'))),
						$('<ul />')
							.append($('<li />')
								.append($('<strong />').text(L.tr('Checksum') + ': '))
								.append(info.checksum))
							.append($('<li />')
								.append($('<strong />').text(L.tr('Size') + ': '))
								.append('%1024mB'.format(info.size))),
						$('<label />')
							.append($('<input />')
								.attr('type', 'checkbox')
								.prop('id', 'keepConfig')
								.prop('checked', true))
							.append(' ')
							.append(L.tr('Keep configuration when reflashing'))
					], {
						style: 'confirm',
						confirm: function() {
							if( $('#keepConfig').prop('checked') ){
								self.cioUpgrade(true);
                                                        }else{
								self.cioCleanUpgrade(true);
                                                        }

							self.handleUpgrading();
						}
					}
				);
			}
			else
			{
				L.ui.dialog(
					L.tr('Invalid image'), [
						$('<p />').text(L.tr('Firmware image verification failed, the "sysupgrade" command responded with the message below:')),
						$('<pre />')
							.addClass('alert-message')
							.text(res.stdout || res.stderr),
						$('<p />').text(L.tr('Image verification failed with code %d.').format(res.code))
					], {
						style: 'close',
						close: function() {
							self.cleanUpgrade().then(function() {
								L.ui.dialog(false);
							});
						}
					}
				);
			}
		});
	},
	progress: function(timeleft, timetotal, $element) {
		var self = this;
		$element.show();
		$element.focus();
		$(document.body).css( {"cursor": "wait"} );
		$("body").css( {"cursor": "wait"} );
		var progressAction = "";
		if(timeleft>595){
			progressAction = "Starting FW update!";	
		}else if(timeleft>590){
			progressAction = "Preparing system: Shutting down services.";	
		}else if(timeleft>586){
			progressAction = "Preparing system: Shutting down applications.";	
		}else if(timeleft>530){
			progressAction = "Writing Firmware Image";	
		}else if(timeleft>510){
			progressAction = "Preparing to reboot system";	
		}else if(timeleft>100){
			progressAction = "Formating filesytem";	
		}else{
			progressAction = "";
		}
    		var progressBarWidth = timeleft * 600 / timetotal;
    		$element
		.css('width', progressBarWidth+'px')
        	.html(progressAction);

		$('#progressText').html('Please wait, ' + timeleft + ' seconds left.');
		if(timeleft<=0){
			$(document.body).css( {"cursor": "default"} );
			$("body").css( {"cursor": "default"} );
			$('#progressText').html('<b>Firmware update completed!  You can now reload browser to the default webpage.</b>');
		}
    		if(timeleft > 0) {
        		setTimeout(function() {
            			self.progress(timeleft - 1, timetotal, $element);
        		}, 1000);
    		}
	},

	handleUpgrading: function() {
		 var self = this;
		 L.session.stopHeartbeat();
		 var x = L.ui.dialog(
                      L.tr('Upgrading firmware'), [
                      $('<p />').text(L.tr('Please wait while firmware is upgrading.  This process takes about 10 minutes, please wait...')),
			$('<div name="upgradeProgressBar" id="upgradeProgessBar" />')
                              .addClass('progress-bar')
                              .css('width', '80%')
                              .css('height', '15px'),
			$('<br>'),
	   		$('<div name="progressText" id="progressText" />')
                        ], {
                            style: 'wait',
			    wide: true,
                            //style: 'close',
                            close: function() {
					L.ui.dialog(true);
					//L.ui.dialog(false);
				}}
                            );

		self.progress(600, 600, $('#upgradeProgessBar'));

	},

	handleBackupUpload: function() {
		var self = this;
		L.ui.upload(
			L.tr('Backup restore'),
			L.tr('Select the backup archive to restore and click "%s" to proceed.').format(L.tr('Ok')), {
				filename: '/tmp/backup.tar.gz',
				success: function(info) {
					self.handleBackupVerify(info);
				}
			}
		);
	},

	handleBackupVerify: function(info) {
		var self = this;
		L.ui.dialog(
			L.tr('Backup restore'), [
				$('<p />').text(L.tr('The backup archive was uploaded completely. Please verify the checksum and file size below, then click "%s" to restore the archive.').format(L.tr('Ok'))),
				$('<ul />')
					.append($('<li />')
						.append($('<strong />').text(L.tr('Checksum') + ': '))
						.append(info.checksum))
					.append($('<li />')
						.append($('<strong />').text(L.tr('Size') + ': '))
						.append('%1024mB'.format(info.size)))
			], {
				style: 'confirm',
				confirm: function() {
					self.handleBackupRestore();
				}
			}
		);
	},

	handleBackupRestore: function() {
		var self = this;
		self.restoreBackup().then(function(res) {
			if (res.code == 0)
			{
				L.ui.dialog(
					L.tr('Backup restore'), [
						$('<p />').text(L.tr('The backup was successfully restored, it is advised to reboot the system now in order to apply all configuration changes.')),
						$('<input />')
							.addClass('cbi-button')
							.attr('type', 'button')
							.attr('value', L.tr('Reboot system'))
							.click(function() { alert('Reboot...');
												self.startReboot();
							 })
					], {
						style: 'close',
						close: function() {
							self.cleanBackup().then(function() {
								L.ui.dialog(false);
							});
						}
					}
				);
			}
			else
			{
				L.ui.dialog(
					L.tr('Backup restore'), [
						$('<p />').text(L.tr('Backup restoration failed, the "sysupgrade" command responded with the message below:')),
						$('<pre />')
							.addClass('alert-message')
							.text(res.stdout || res.stderr),
						$('<p />').text(L.tr('Backup restoration failed with code %d.').format(res.code))
					], {
						style: 'close',
						close: function() {
							self.cleanBackup().then(function() {
								L.ui.dialog(false);
							});
						}
					}
				);
			}
		});
	},

	handleBackupDownload: function() {
		var form = $('#btn_backup').parent();

		form.find('[name=sessionid]').val(L.globals.sid);
		form.submit();
	},

	handleReset: function() {
		var self = this;
		L.ui.dialog(L.tr('Really reset all changes?'), L.tr('This will reset the system to its initial configuration, all changes made since the initial flash will be lost!'), {
			style: 'confirm',
			confirm: function() {
				self.startReset().then(function() {
					L.ui.reconnect();
				});

				alert('Reset...');
			}
		});
	},

	execute: function() {
		var self = this;

		self.testReset().then(function(reset_avail) {
			if (!reset_avail) {
				$('#btn_reset').prop('disabled', true);
			}

			if (!self.options.acls.backup) {
				$('#btn_restore, #btn_save, textarea').prop('disabled', true);
			}
			else {
				$('#btn_backup').click(function() { self.handleBackupDownload(); });
				$('#btn_restore').click(function() { self.handleBackupUpload(); });
			}

			if (!self.options.acls.upgrade) {
				$('#btn_flash, #btn_reset').prop('disabled', true);
			}
			else {
				$('#btn_flash').click(function() { self.handleFlashUpload(); });
				$('#btn_reset').click(function() { self.handleReset(); });
			}

			return self.getBackupConfig();
		}).then(function(config) {
			$('textarea')
				.attr('rows', (config.match(/\n/g) || [ ]).length + 1)
				.val(config);

			$('#btn_save')
				.click(function() {
					var data = ($('textarea').val() || '').replace(/\r/g, '').replace(/\n?$/, '\n');
					L.ui.loading(true);
					self.setBackupConfig(data).then(function() {
						$('textarea')
							.attr('rows', (data.match(/\n/g) || [ ]).length + 1)
							.val(data);

						L.ui.loading(false);
					});
				});

			$('#btn_list')
				.click(function() {
					L.ui.loading(true);
					self.listBackup().then(function(list) {
						L.ui.loading(false);
						L.ui.dialog(
							L.tr('Backup file list'),
							$('<textarea />')
								.css('width', '100%')
								.attr('rows', list.length)
								.prop('readonly', true)
								.addClass('form-control')
								.val(list.join('\n')),
							{ style: 'close' }
						);
					});
				});
		});
	}
});
