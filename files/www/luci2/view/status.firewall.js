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


		return 0;

	}
});
