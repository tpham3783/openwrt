L.ui.view.extend({
	shutdown: L.rpc.declare({
		object: 'luci2.ciosystem',
		method: 'shutdown'
	}),

	execute: function() {
		var self = this;

		$('#btnReboot').click(function() {
			L.system.performReboot();
			alert("Rebooting system, please wait a minute and reload the webpage");
                        L.ui.loading(true);
                });

		$('#btnShutdown').click(function() {
			alert("System is shutting down, you'll not able able to access it anymore.");
                        L.ui.loading(true);
			self.shutdown();
                });
	}
});
