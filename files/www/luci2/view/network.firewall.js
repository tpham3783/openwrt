L.ui.view.extend({
	title: L.tr('Firewall'),

	getIP4FirewallRules: L.rpc.declare({
                object: 'luci2.ciosystem',
                method: 'show_firewall',
        }),
	resetCounts: L.rpc.declare({
                object: 'luci2.ciosystem',
                method: 'reset_counts',
        }),
	restartFirewall: L.rpc.declare({
                object: 'luci2.ciosystem',
                method: 'restart_firewall',
        }),
	getUserFirewall: L.rpc.declare({
                object: 'luci2.ciosystem',
                method: 'get_user_firewall',
        }),
	setUserFirewall: L.rpc.declare({
                object: 'luci2.ciosystem',
                method: 'set_user_firewall',
		params: [ 'file' ]
        }),



	opkg: {
		updateLists: L.rpc.declare({
			object: 'luci2.opkg',
			method: 'update',
			expect: { '': { } }
		}),

		_allPackages: L.rpc.declare({
			object: 'luci2.opkg',
			method: 'list',
			params: [ 'offset', 'limit', 'pattern' ],
			expect: { '': { } }
		}),

		_installedPackages: L.rpc.declare({
			object: 'luci2.opkg',
			method: 'list_installed',
			params: [ 'offset', 'limit', 'pattern' ],
			expect: { '': { } }
		}),

		_findPackages: L.rpc.declare({
			object: 'luci2.opkg',
			method: 'find',
			params: [ 'offset', 'limit', 'pattern' ],
			expect: { '': { } }
		}),

		_fetchPackages: function(action, offset, limit, pattern)
		{
			var packages = [ ];

			return action(offset, limit, pattern).then(function(list) {
				if (!list.total || !list.packages)
					return { length: 0, total: 0 };

				packages.push.apply(packages, list.packages);
				packages.total = list.total;

				if (limit <= 0)
					limit = list.total;

				if (packages.length >= limit)
					return packages;

				L.rpc.batch();

				for (var i = offset + packages.length; i < limit; i += 100)
					action(i, (Math.min(i + 100, limit) % 100) || 100, pattern);

				return L.rpc.flush();
			}).then(function(lists) {
				for (var i = 0; i < lists.length; i++)
				{
					if (!lists[i].total || !lists[i].packages)
						continue;

					packages.push.apply(packages, lists[i].packages);
					packages.total = lists[i].total;
				}

				return packages;
			});
		},

		listPackages: function(offset, limit, pattern)
		{
			return this._fetchPackages(this._allPackages, offset, limit, pattern);
		},

		installedPackages: function(offset, limit, pattern)
		{
			return this._fetchPackages(this._installedPackages, offset, limit, pattern);
		},

		findPackages: function(offset, limit, pattern)
		{
			return this._fetchPackages(this._findPackages, offset, limit, pattern);
		},

		installPackage: L.rpc.declare({
			object: 'luci2.opkg',
			method: 'install',
			params: [ 'package' ],
			expect: { '': { } }
		}),

		removePackage: L.rpc.declare({
			object: 'luci2.opkg',
			method: 'remove',
			params: [ 'package' ],
			expect: { '': { } }
		}),

		getConfig: L.rpc.declare({
			object: 'luci2.opkg',
			method: 'config_get',
			expect: { config: '' }
		}),

		setConfig: L.rpc.declare({
			object: 'luci2.opkg',
			method: 'config_set',
			params: [ 'data' ]
		}),

		isInstalled: function(pkg)
		{
			return this._installedPackages(0, 1, pkg).then(function(list) {
				return (!isNaN(list.total) && list.total > 0);
			});
		}
	},

	updateDiskSpace: function()
	{
		return L.system.getDiskInfo().then(function(info) {
			$('#package_space').empty().append(
				new L.ui.progress({
					value:  info.root.used / 1024,
					max:    info.root.total / 1024,
					format: '%d ' + L.tr('kB') + ' / %d ' + L.tr('kB') + ' ' + L.trc('Used disk space', 'used') + ' (%d%%)'
				}).render());
		});
	},

	installRemovePackage: function(pkgname, installed)
	{
		var self = this;

		var dspname   = pkgname.replace(/^.+\//, '');
		var action    = installed ? self.opkg.removePackage : self.opkg.installPackage;
		var title     = (installed ? L.tr('Removing package "%s" …') : L.tr('Installing package "%s" …')).format(dspname);
		var confirm   = (installed ? L.tr('Really remove package "%h" ?') : L.tr('Really install package "%h" ?')).format(dspname);

		L.ui.dialog(title, confirm, {
			style:   'confirm',
			confirm: function() {
				L.ui.dialog(title, L.tr('Waiting for package manager …'), { style: 'wait' });

				action.call(self.opkg, pkgname).then(function(res) {
					self.fetchInstalledList().then(function() { return self.fetchPackageList(); }).then(function() {
						var output = [ ];

						if (res.stdout)
							output.push($('<pre />').text(res.stdout));

						if (res.stderr)
							output.push($('<pre />').addClass('alert-message').text(res.stderr));

						output.push(res.code ? L.tr('Package manager failed with status %d.').format(res.code) : L.tr('Package manager finished successfully.'));

						L.ui.dialog(title, output, { style: 'close' });

						if (name)
							$('#package_url').val('');
					});
				});
			}
		});
	},

	fetchInstalledList: function()
	{
		var self = this;
		return self.opkg.installedPackages(0, 0, '*').then(function(list) {
			self.installedList = { };
			for (var i = 0; i < list.length; i++)
				self.installedList[list[i][0]] = true;
		});
	},

	fetchPackageList: function(offset, interactive)
	{
		if (interactive)
			L.ui.loading(true);

		if (typeof(offset) == 'undefined')
			offset = parseInt($('#package_filter').attr('offset')) || 0;

		var self = this;

		var pattern = $('#package_filter').val() || '';
		var action;

		if (pattern.length)
		{
			action = $('#package_which').prop('checked') ? self.opkg.installedPackages : self.opkg.findPackages;
			pattern = '*' + pattern + '*';

			$('#package_filter').next().attr('src', L.globals.resource + '/icons/cbi/remove.gif');
		}
		else
		{
			action = $('#package_which').prop('checked') ? self.opkg.installedPackages : self.opkg.listPackages;
			pattern = '*';

			$('#package_filter').next().attr('src', L.globals.resource + '/icons/cbi/find.gif');
		}

		$('#package_filter').attr('offset', offset);

		var install_disabled = $('#package_install').attr('disabled');

		return action.call(self.opkg, offset, 100, pattern).then(function(list) {
/*
			var packageTable = new L.ui.table({
				placeholder: L.tr('No matching packages found.'),
				columns: [ {
					caption: L.trc('Package table header', 'Package'),
					key:     0
				}, {
					caption: L.trc('Package table header', 'Version'),
					key:     1,
					format:  function(v) {
						return (v.length > 15 ? v.substring(0, 14) + '…' : v);
					}
				}, {
					caption: L.trc('Package table header', 'Description'),
					key:     2
				}, {
					caption: L.trc('Package table header', 'Installation Status'),
					key:     0,
					width:   '120px',
					format: function(v, n) {
						var inst = self.installedList[list[n][0]];
						return L.ui.button(inst ? L.trc('Package state', 'Installed') : L.trc('Package state', 'Not installed'), inst ? 'success' : 'danger')
							.css('width', '100%')
							.attr('disabled', install_disabled)
							.attr('pkgname', list[n][0])
							.attr('installed', inst)
							.click(function() {
								self.installRemovePackage(this.getAttribute('pkgname'), this.getAttribute('installed') == 'true');
							});
					}
				} ]
			});

			packageTable.rows(list);
			packageTable.insertInto('#package_table');
*/

			var packageTable = new L.ui.table({
                                placeholder: L.tr('No matching packages found.'),
                                columns: [ {
                                        caption: L.trc('Package table header', 'Rule#'),
                                        key:     0
                                }, {
                                        caption: L.trc('Package table header', 'Pkts.'),
                                        key:     1,
                                        xformat:  function(v) {
                                                return (v.length > 15 ? v.substring(0, 14) + '...' : v);
                                        }
                                }, {
                                        caption: L.trc('Package table header', 'Traffic'),
                                        key:     2
                                }, {
                                        caption: L.trc('Package table header', 'Target'),
                                        key:     3
                                }, {
                                        caption: L.trc('Package table header', 'Prot'),
                                        key:     4
                                }, {
                                        caption: L.trc('Package table header', 'Flags'),
                                        key:     5
                                }, {
                                        caption: L.trc('Package table header', 'In'),
                                        key:     6
                                }, {
                                        caption: L.trc('Package table header', 'Out'),
                                        key:     7
                                }, {
                                        caption: L.trc('Package table header', 'Source'),
                                        key:     8
                                }, {
                                        caption: L.trc('Package table header', 'Destination'),
                                        key:     9 
                                }, {
                                        caption: L.trc('Package table header', 'Options'),
                                        key:     10 
                                }, {
                                        caption: L.trc('Package table header', 'Installation Status'),
                                        key:     0,
                                        width:   '120px'
                                } ]
                        });

                        packageTable.rows(list);
                       // packageTable.insertInto('#ipv4_table');
                        packageTable.insertInto('#package_table');


			if (offset > 0)
				$('#package_prev')
					.attr('offset', offset - 100)
					.attr('disabled', false)
					.text('« %d - %d'.format(offset - 100 + 1, offset));
			else
				$('#package_prev')
					.attr('disabled', true)
					.text('« %d - %d'.format(1, Math.min(100, list.total)));

			if ((offset + 100) < list.total)
				$('#package_next')
					.attr('offset', offset + 100)
					.attr('disabled', false)
					.text('%d - %d »'.format(offset + 100 + 1, Math.min(offset + 200, list.total)));
			else
				$('#package_next')
					.attr('disabled', true)
					.text('%d - %d »'.format(list.total - (list.total % 100) + 1, list.total));

			if (interactive)
				L.ui.loading(false);
		}).then(self.updateDiskSpace);
	},

	updateFirewallView: function(){
		var self = this;
		this.getIP4FirewallRules().then(function(log) {
                        var ta = document.getElementById('ipv4_rules');
                        var lines = log.ipv4.replace(/\n+$/, '').split(/\n/);

                        ta.rows = lines.length;
                        ta.value = lines.join("\n");

                        var ta1 = document.getElementById('ipv6_rules');
                        var lines1 = log.ipv6.replace(/\n+$/, '').split(/\n/);

                        ta1.rows = lines1.length;
                        ta1.value = lines1.join("\n");
                });
	},

	execute: function()
	{
		var self = this;
// pham
		self.updateFirewallView();

		
		/* event binding for interactive buttons */
		$('#btn_reset_counters').click(function() {
			self.resetCounts().then(function(){
				self.updateFirewallView();
			});
		});

		$('#btn_restart_firewall').click(function() {
			self.restartFirewall().then(function(){
				self.updateFirewallView();
			});
		});
		/* ip6 buttons */
		$('#btn_reset_counters_ip6').click(function() {
			self.resetCounts().then(function(){
				self.updateFirewallView();
			});
		});

		$('#btn_restart_firewall_ip6').click(function() {
			self.restartFirewall().then(function(){
				self.updateFirewallView();
			});
		});

		config='bug of shit';

		self.getUserFirewall().then(function(obj) {
			config = obj.file;
                        var ta = document.getElementById('ta_custom_rules');
			var lines1 = config.replace(/\n+$/, '').split(/\n/);
//			var lines1 = obj.file.split(/\n/);
			console.log(lines1);
			ta.rows = lines1.length+10; 
			ta.value = lines1.join("\n");



			$('#btn_save')
                                .click(function() {
		                        var ta = document.getElementById('ta_custom_rules');
					var data = ta.value.replace(/\n/g,"_EOL_"); //config.replace(/\n+$/, '').split(/\n/);
					console.log(data);
					
				//	var data = ($('#ta_custom_rules').val() || '').replace(/\r/g, '').replace(/\n?$/, 'EOL');
//					var data = ($('#ta_custom_rules').val() || '').replace(/\r/g, '').replace(/\n?$/, '___\n');
                                        L.ui.loading(true);
                                        self.setUserFirewall(data).then(function() {
/*
                                                $('#ta_custom_rules')
                                                        .attr('rows', (data.match(/\n/g) || [ ]).length + 10)
                                                        .val(data);
*/

                                                L.ui.loading(false);
                           	        });
                        	});
		});


		/* port forward, specfication @ https://wiki.openwrt.org/doc/uci/firewall#forwardingportsdestinationnatdnat */
		var m = new L.cbi.Map('firewall', {
                        xreadonly:    !this.options.acls.hostnames, 
                        readonly:    false 
                });

                var s = m.section(L.cbi.TableSection, 'redirect', {
			caption:     L.tr('Port forward from WAN to LAN'),
                        anonymous:   true,
                        addremove:   true,
                        add_caption: L.tr('Add new port'),
                        remove_caption: L.tr('Remove')
                });

		/* Always redirect from WAN, let's not make the user choose this option */
		s.add = function() {
			var sid = this.ownerMap.add('firewall', 'redirect');
                        this.ownerMap.set('firewall', sid, 'src', 'wan');
		};



                s.option(L.cbi.InputValue, 'name', {
                        caption:     L.tr('Description'),
			width:	     3,
                        xdatatype:    'hostname'
                });


                s.option(L.cbi.InputValue, 'src_dport', {
                        caption:     L.tr('Port'),
                        datatype:    'range(0,65535)',
			width:	     1
                });

                s.option(L.cbi.InputValue, 'dest_ip', {
                        caption:     L.tr('Dest. IP address'),
                        xdatatype:    'ipaddr',
			datatype:    'ip4addr',
			placeholder: '192.168.45.1',
                        width:       2
                });

                s.option(L.cbi.InputValue, 'dest_port', {
                        caption:     L.tr('Dest. Port#'),
			placeholder: '22',
			width:	     1,
                        datatype:    'range(0,65535)' 
                });

                m.insertInto('#tbl_port_forward');

		return 0;


		//});
	}
});
