#
#	'cnc_server/vcnc-rest' makefile
#
#	Copyright (C) 2015	IC Manage Inc
#
#	See the file 'COPYING' for license information.
#
ACLOCAL_AMFLAGS = -I m4
SUB_LOCAL = $(top_srcdir)/sub_local.pl

SUBDIRS = src addon static . doc installer
#
PROJECTIONS = 			\
	VERSION=$(VERSION)

#
#  On installation, place config/settings files into the 
#  target "/etc/" directory
#
#  .. this is premature; currently the node server only looks
#     in the directory containing app.js
#
#etcdir=$(datadir)/etc
#etc_DATA = velstor-vcnc.conf velstor-vcnc-settings.js


all-local:
	(cd $(top_srcdir); \
	  NVM_DIR=$(TOOLROOT)/nvm; \
	  [ ! -d $(NVM_DIR) ] && (echo "Can't find NVM_DIR"; exit 1); \
	  . $(NVM_DIR)/nvm.sh;  \
	  npm install; \
	  npm run json-ize )

#
#  Install an API spec editor which may be useful for development but is not
#  needed for deployment
#
swagger-editor:
	if [ ! -d swagger-editor ]; then \
	  $(PROG_GIT) clone https://github.com/swagger-api/swagger-editor.git; \
	fi
#
#
clean-local:
	rm -rf swagger-editor

install-data-local:
	find $(top_srcdir) -name '*~' -exec rm '{}' ';'
	mkdir $(DESTDIR)$(prefix)/share/vcnc/vcnc-rest
	rsync -a $(top_srcdir) $(DESTDIR)$(prefix)/share/vcnc/vcnc-rest
