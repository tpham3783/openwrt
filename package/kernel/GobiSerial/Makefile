#
# Copyright (C) 2008-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=GobiSerial
PKG_RELEASE:=S2.25N2.34

include $(INCLUDE_DIR)/package.mk

define KernelPackage/GobiSerial
  SUBMENU:=Other modules
  TITLE:=GobiSerial Driver
  DEPENDS:=+kmod-usb-serial
  FILES:=$(PKG_BUILD_DIR)/GobiSerial.ko
  AUTOLOAD:=$(call AutoProbe,GobiSerial)
  KCONFIG:=
endef

define KernelPackage/GobiSerial/description
  Kernel module for Sierra Wireless MC7350.
endef


MAKE_OPTS:= \
	ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)" \
	SUBDIRS="$(PKG_BUILD_DIR)" \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
	$(EXTRA_KCONFIG)

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		$(MAKE_OPTS) \
		modules
endef

$(eval $(call KernelPackage,GobiSerial))
