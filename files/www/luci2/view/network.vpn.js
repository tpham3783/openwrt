L.ui.view.extend({
	title: L.tr('IPSec VPN'),   /* Optional title */

	getAuto: L.rpc.declare({
		object: 'uci',
		method: 'get',
		params: [ 'config' ],
		expect: { '': { code: -1 } }
	}),

	restart: L.rpc.declare({
		object: 'luci2.vpn',
		method: 'restart',
		params: [ ],
		expect: { '': { code: -1 } }
	}),

	stop: L.rpc.declare({
		object: 'luci2.vpn',
		method: 'stop',
		params: [ ],
		expect: { '': { code: -1 } }
	}),

	start: L.rpc.declare({
		object: 'luci2.vpn',
		method: 'start',
		params: [ ],
		expect: { '': { code: -1 } }
	}),

	getStatus: L.rpc.declare({
		object: 'luci2.vpn',
		method: 'statusall',
		params: [ ],
		expect: { '': { code: -1 } }
	}),
    
    updateView: function() {
	var self = this;
	self.getStatus().then(function(rv){
		$('#field_ipsec_vpn_sonicwall_status').empty().append($('<pre>').css('color', 'black').css('font-size', '10px').text(rv.stdout));
	});
    },
    
	execute: function() {
		var self = this;
        
        	/* Default values before ubus callback */
        	self.modemName = "Unknown"
        	self.apn = "Unknown";

                var m = new L.cbi.Map('ipsec', {
                        readonly:    false
                });

		var s = m.section(L.cbi.TypedSection, 'vpn', {
                        caption:     L.tr('IPSec Client Definition'),
                        //description:     L.tr('Setup site-to-site VPN tunnel using IPSec IKEv2 Authentication'),
                        teasers:     [ 'vpn','leftsourceip'],
                        collabsible: true,
                        add_caption: L.tr('Add new LED defintion'),
                        remove_caption: L.tr('Remove LED definition'),
                        readonly:   false 
                });


		var p1 = s.option(L.cbi.CheckboxValue, 'enabled', {
                        caption:     L.tr('Enable VPN Tunnel - Auto start tunnel at boot'),
                        initial:     true,
                        enabled:     '1',
                        disabled:    '0'
                });

		s.option(L.cbi.InputValue, 'leftsourceip', {
                        caption:     L.tr('leftsourceip'),
                        xdatatype:    'ipaddr',
                        datatype:    'ip4addr',
                        placeholder: '192.168.45.1',
                        width:       2
                });

                s.option(L.cbi.InputValue, 'leftid', {
                        caption:     L.tr('leftid'),
                        placeholder: '',
                        //datatype:    'ip4addr'
                });

                s.option(L.cbi.InputValue, 'leftsubnet', {
                        caption:     L.tr('leftsubnet'),
                        placeholder: '192.168.45.0/24',
                        datatype:    'cidr4'
                });

                s.option(L.cbi.InputValue, 'right', {
                        caption:     L.tr('right'),
                        placeholder: '',
                        //datatype:    'ip4addr'
                });

                s.option(L.cbi.InputValue, 'rightid', {
                        caption:     L.tr('rightid'),
                        placeholder: '',
                        //datatype:    'ip4addr'
                });

                s.option(L.cbi.InputValue, 'rightsubnet', {
                        caption:     L.tr('rightsubnet'),
                        placeholder: '172.16.10.0/24',
			datatype: 'cidr4'
                });

                s.option(L.cbi.InputValue, 'ike', {
                        caption:     L.tr('ike (Initial Key Exchange algorithm) - ie) 3des-sha1-modp1024'),
                        placeholder: '3des-sha1-modp1024',
                });

                s.option(L.cbi.InputValue, 'esp', {
                        caption:     L.tr('esp (Encapsulating Security Payload) - ie) aes256-sha1-modp1024'),
                        placeholder: '3des-sha1-modp1024',
                });

                s.option(L.cbi.InputValue, 'psk', {
                        caption:     L.tr('pks (Pre-Shared Key)'),
                        placeholder: '',
                });

                (s.option(L.cbi.DummyValue, 'status', {
                        caption:     L.tr('Current Connection Status:'),
			readonly: true,
                })).ucivalue = function(sid)
		{
			var is_enabled = this.ownerMap.get('ipsec', sid, 'enabled'); 
			if ( is_enabled == '1' ){
				self.getStatus().then(function(rv){
					$('#field_ipsec_vpn_sonicwall_status').empty().append($('<pre>').css('color', 'black').text(rv.stdout));
				});
				return '';
			}else{
				return "VPN Connection is disabled, please enable to view status.";
			}
		};

                var restart_button = s.option(L.cbi.ButtonValue, 'control', {
                        caption:     L.tr(''),
			text:        L.tr('Restart VPN Connection'),
			readonly: true,
                        placeholder: '22',
                });
		
		restart_button.on('click', function(ev) {
			//self.restart();
			self.stop().then(function(){
				$('#field_ipsec_vpn_sonicwall_status').empty().text("VPN Tunnel stopped, getting ready to start the tunnel back up");
				self.start();
			});
                });

		var stop_button = s.option(L.cbi.ButtonValue, 'stop_control', {
                        caption:     L.tr(''),
                        text:        L.tr('Stop VPN Connection'),
                        readonly: true,
                });

                stop_button.on('click', function(ev) {
                        self.stop();
                });

                var start_button = s.option(L.cbi.ButtonValue, 'start_control', {
                        caption:     L.tr(''),
			text:        L.tr('Start VPN Connection'),
			readonly: true,
                });

		start_button.on('click', function(ev) {
			self.start();
                });

                m.insertInto('#tbl_vpn');

		// Schedule viewupdate
		L.network.load().then(function() {
			self.repeat(self.updateView, 5000);
		});
		

		return 0;
	}
});
