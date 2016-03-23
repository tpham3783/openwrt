L.ui.view.extend({
	title: L.tr('Modem'),   /* Optional title */

	runAtcommand: L.rpc.declare({
		object: 'atCMD',
		method: 'cmd',
		params: [ 'data' ],
		expect: { '': { code: -1 } }
	}),

	runApn: L.rpc.declare({
		object: 'atCMD',
		method: 'apnChange',
		params: [ 'apn' ],
		expect: { '': { code: -1 } }
	}),

	getInfo: L.rpc.declare({
		object: 'uci',
		method: 'get',
		params: [ 'config' ],
		expect: { '': { code: -1 } }
	}),

	modemConnect: L.rpc.declare({
		object: 'modem',
		method: 'connect',
		params: [ 'keep' ],
		expect: { '': { code: -1 } }
	}),

	modemDisconnect: L.rpc.declare({
		object: 'modem',
		method: 'disconnect',
		params: [ 'keep' ],
		expect: { '': { code: -1 } }
	}),

	getAuto: L.rpc.declare({
		object: 'uci',
		method: 'get',
		params: [ 'config' ],
		expect: { '': { code: -1 } }
	}),

	setAuto: L.rpc.declare({
		object: 'modem',
		method: 'checkAuto',
		params: [ 'keep' ],
		expect: { '': { code: -1 } }
	}),

	setMode: L.rpc.declare({
		object: 'modem',
		method: 'changeMode',
		params: [ 'mode' ],
		expect: { '': { code: -1 } }
	}),

	renderContents: function() {
		var self = this;
		return $.when(
			self.getInfo('modem').then(function(result) {
				var sysinfoTable = new L.ui.grid({
					caption: L.tr('Information'),
					columns: [ {
						width:    4
					}, {
						width:    8,
						nowrap:   true
					} ]
				});

				sysinfoTable.rows([
					[ L.tr('Module'), result.values.cfg0203f7.module    ],
					[ L.tr('Status'), result.values.cfg0203f7.status   ],
					[ L.tr('Mode'), result.values.cfg0203f7.detectMode ],
					[ L.tr('RSSI'), result.values.cfg0203f7.rssi        ],
					[ L.tr('APN'), result.values.cfg0203f7.apn        ]
				]);

				sysinfoTable.insertInto('#modem_status_table');

				if (result.values.cfg0203f7.status === "Connected"){
					document.getElementById("connect").disabled = true;
					document.getElementById("disconnect").disabled = false;
				} 
				else
				{
					document.getElementById("connect").disabled = false;
					document.getElementById("disconnect").disabled = true;
				}

				// if (result.values.cfg0203f7.autoconnect === "1"){
				// 	document.getElementById("autoBox").checked = "checked";
				// } 
				// else
				// {
				// 	document.getElementById("autoBox").checked = "";
				// }

				// document.getElementById(result.values.cfg0203f7.mode).checked = "checked";

			})
		)
	},

	execute: function() {
		var self = this;

		L.network.load().then(function() {
			self.repeat(self.renderContents, 5000);
        });

		$('#run').click(function() {
			L.ui.loading(true);
			self['runAtcommand']($('#atcommand').val()).then(function(rv) {
				$('#at_output').empty().show();

				if (rv.stdout)
					$('#at_output').text(rv.stdout);

				if (rv.stderr)
					$('#at_output').append($('<span />').css('color', 'red').text(rv.stderr));

				L.ui.loading(false);
			});
		});

		$('#change').click(function() {
			L.ui.loading(true);
			self['runApn']($('#apn').val()).then(function(rv) {

				L.ui.loading(false);
			});
		});

		$('#autoBox').click(function() {
			L.ui.loading(true);
			self['setAuto'](true).then(function(rv) {

				L.ui.loading(false);
			});

		});

		$('#connect').click(function() {
			L.ui.loading(true);
			self['modemConnect'](true).then(function(rv) {
				document.getElementById("connect").disabled = true;

				L.ui.loading(false);
			});
		});

		$('#disconnect').click(function() {
			L.ui.loading(true);
			self['modemDisconnect'](true).then(function(rv) {
				document.getElementById("disconnect").disabled = true;

				L.ui.loading(false);
			});
		});

		$('#Auto').click(function() {
			L.ui.loading(true);
			self['setMode']("Auto").then(function(rv) {

				L.ui.loading(false);
			});
		});

		$('#LTE').click(function() {
			L.ui.loading(true);
			self['setMode']("LTE").then(function(rv) {

				L.ui.loading(false);
			});
		});

		$('#3G').click(function() {
			L.ui.loading(true);
			self['setMode']("3G").then(function(rv) {

				L.ui.loading(false);
			});
		});

		self.getInfo('modem').then(function(result) {

			if (result.values.cfg0203f7.autoconnect === "1"){
				document.getElementById("autoBox").checked = "checked";
			} 
			else {
				document.getElementById("autoBox").checked = "";
			}

			document.getElementById(result.values.cfg0203f7.mode).checked = "checked";

		});
	}
});