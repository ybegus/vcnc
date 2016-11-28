#
#	'cnc_server/vcnc-rest' makefile
#
#	Copyright (C) 2015	IC Manage Inc
#
#	See the file 'COPYING' for license information.
#
SUBDIRS = addon static

#
#  On installation, place config/settings files into the 
#  target "/etc/" directory
#
#  .. this is premature; currently the node server only looks
#     in the directory containing app.js
#
#etcdir=$(datadir)/etc
#etc_DATA = velstor-vcnc.conf velstor-vcnc-settings.js

all:
	(cd $(top_srcdir)/vcnc-rest; \
	  NVM_DIR=$(TOOLROOT)/nvm; \
	  [ ! -d $(NVM_DIR) ] && (echo "Can't find NVM_DIR"; exit 1); \
	  . $(NVM_DIR)/nvm.sh;  \
	  npm install; \
	  npm run json-ize )

clean-local:
	echo TBD
