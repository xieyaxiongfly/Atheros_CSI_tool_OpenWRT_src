include $(TOPDIR)/rules.mk

PKG_NAME:=lorcon
PKG_VERSION:=2017git
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/xieyaxiongfly/lorcon.git
PKG_SOURCE_VERSION:=3c2c1d398f6caaca0f524d397003212932b55ec3
#PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
PKG_FIXUP:=autoreconf
#PKG_FIXUP:=autoreconf libtool

include $(INCLUDE_DIR)/package.mk

PKG_BUILD_DEPENDS:=+libpcap +libnl +wireless-tools +libnfnetlink

define Package/lorcon
  SECTION:=lib
  CATEGORY:=Libraries
  TITLE:=Lorcon (2017)
  URL:=https://www.kismetwireless.net/
  DEPENDS:=+libpcap +wireless-tools +libnfnetlink +libnl
endef

define Package/lorcon/description
 LORCON Tools
endef

#define Build/Configure
#  $(call Build/Configure/Default,--with-linux-headers=$(LINUX_DIR))
#endef

define Build/Configure
	(cd $(PKG_BUILD_DIR); mv ../$(PKG_NAME) .; cd $(PKG_NAME); ls -l aclocal.m4; rm aclocal.m4; aclocal; autoconf; autoreconf -ivf; rm -rf config.cache; \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(strip $(TARGET_CFLAGS))" \
		CPPFLAGS="-I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/include" \
		LDFLAGS="-L$(STAGING_DIR)/usr/lib -L$(STAGING_DIR)/lib" \
		./configure \
			--target=$(GNU_TARGET_NAME) \
			--host=$(GNU_TARGET_NAME) \
			--build=$(GNU_HOST_NAME) \
			--prefix=/usr \
	);
endef

#user to say 'all install'
define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/ \
		LD="$(TARGET_CXX)" \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		all install
	$(MAKE) -C $(PKG_BUILD_DIR)/ \
		LD="$(TARGET_CXX)" \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		tools
endef

define Package/lorcon/install
	# Copy the libraries
	mkdir -p $(STAGING_DIR)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/lorcon2 $(STAGING_DIR)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/liborcon*.so $(STAGING_DIR)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/liborcon*.so $(1)/usr/lib/

	# Copy mcs_sweep2
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_BUILD_DIR)/tools/mcs_sweep2 $(1)/usr/bin/
	$(CP) $(PKG_BUILD_DIR)/tools/injector $(1)/usr/bin/
endef

$(eval $(call BuildPackage,lorcon))

