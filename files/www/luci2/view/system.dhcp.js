L.ui.view.extend({
	execute: function() {
		var m = new L.cbi.Map('dhcp', {
			caption:     L.tr('DHCP Server'),
			description: L.tr('Configure DHCP Server'),
			collabsible: true
		});

		var s2 = m.section(L.cbi.NamedSection, 'lan', {
			caption:      L.tr('LAN'),
			readonly: false
		});

	 	s2.option(L.cbi.InputValue, 'start', {
                        caption:     L.tr('Start IP (last octec): '),
                        placeholder: '',
                        datatype:   'range(1, 254)' 
                });
	 	s2.option(L.cbi.InputValue, 'limit', {
                        caption:     L.tr('Maximum Number of Clients'),
                        placeholder: '',
                        datatype:   'range(1, 254)' 
                });
	 	s2.option(L.cbi.InputValue, 'leasetime', {
                        caption:     L.tr('Lease Time: 12h or 30m'),
                        placeholder: '',
                });

		return m.insertInto('#map');
	}
});
