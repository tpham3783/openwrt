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

		config='';

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
					
                                        L.ui.loading(true);
                                        self.setUserFirewall(data).then(function() {
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

	}
});
