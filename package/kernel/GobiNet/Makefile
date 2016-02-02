#
# Copyright (C) 2008-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=GobiNet
PKG_RELEASE:=S2.25N2.34

include $(INCLUDE_DIR)/package.mk

define KernelPackage/GobiNet
  SUBMENU:=Other modules
  TITLE:=GobiNet Driver
  DEPENDS:=+kmod-usb-net +kmod-usb-core
  FILES:=$(PKG_BUILD_DIR)/GobiNet.ko
  AUTOLOAD:=$(call AutoProbe,GobiNet)
  KCONFIG:=
endef

define KernelPackage/GobiNet/description
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

$(eval $(call KernelPackage,GobiNet))
