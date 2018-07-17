include $(TOPDIR)/rules.mk

PKG_NAME:=client_main
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/client_main
	SECTION:=CSI
	CATEGORY:=CSI
	TITLE:=CSI Receive Data
endef

define Package/client_main/description
	CSI Receive Data utility
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS)
endef

define Package/client_main/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/client_main $(1)/bin/
endef

$(eval $(call BuildPackage,client_main))

