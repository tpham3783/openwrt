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
    /* On demand method */
    getStatus: L.rpc.declare({
		object: 'modem',
		method: 'getStatus',
		params: [ 'keep' ],
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
			self.getStatus('modem').then(function(result) {
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
					[ L.tr('Module'), self.modemName   ],
					[ L.tr('Status'), result.status   ],
					[ L.tr('Mode'), result.mode ],
					[ L.tr('RSSI'), result.rssi        ],
					[ L.tr('APN'), self.apn        ]
				]);
				sysinfoTable.insertInto('#modem_status_table');

				if (result.status === "Connected"){
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
        
        /* Default values before ubus callback */
        self.modemName = "Unknown"
        self.apn = "Unknown";
        
		L.network.load().then(function() {
            /* Schedule page reload */
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

        /* Retrieve Static Config on pageload */
        self.getInfo("modem").then(function(r) {
            /* Update the static APNs & modem name */
            self.modemName = r.values.cfg0203f7.module;
            self.apn = r.values.cfg0203f7.apn;

            /* Update the auto connect checkbox */
            if (r.values.cfg0203f7.autoconnect === "1") {
                document.getElementById("autoBox").checked = "checked";
            } else {
                document.getElementById("autoBox").checked = "";
            }

            /* Select Prefered Network Indicator */
            document.getElementById(r.values.cfg0203f7.mode).checked = "checked";
        });

	}
});
