#
#	'cnc_server/addon' makefile
#
#	Copyright (C) 2015	IC Manage Inc
#
#	See the file 'COPYING' for license information.
#

#
#  Install the software which provides the interactive API documentation.
#
all:
	(cd $(top_srcdir)/vcnc-rest/addon; \
	  npm install; \
	  $(PROG_NODE_GYP) configure; \
	  $(PROG_NODE_GYP) build)

clean-local:
	echo TBD
