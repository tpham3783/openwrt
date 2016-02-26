#
# Copyright (C) 2015 Toan Pham, Exact Systems LLC. 
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=sierra_sdk
PKG_VERSION:=03.03.14


PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

PKG_MAINTAINER := Toan Pham <tpham3783@gmail.com>

include $(INCLUDE_DIR)/package.mk

define Package/sierra_sdk
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=
  #DEPENDS:=+libusb-1.0 +zlib +librt +libpthread
  TITLE:=Sierra Wireless SDK for GobiNET driver
  URL:=http://www.sierrawireless.com
endef


define Build/Prepare
	$(Build/Prepare/Default)
	cp -rf src/* $(PKG_BUILD_DIR)/
endef

define Package/sierra_sdk/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) ./files/autoreconnect.sh $(1)/usr/bin/
	chmod ugo+x $(1)/usr/bin/autoreconnect.sh
	if [ ! grep "autoreconnect" $(1)/etc/rc.local ]; then \
		echo "/usr/bin/autoreconnect.sh" > $(1)/etc/rc.local; \
		echo "exit 0" >> $(1)/etc/rc.local; \
	fi;
	$(INSTALL_DIR) $(1)/sdk
	$(INSTALL_DIR) /tmp/sdk
	-$(INSTALL_BIN) /tmp/sdk/{connect*,slq*} $(1)/sdk/
endef

$(eval $(call BuildPackage,sierra_sdk))
