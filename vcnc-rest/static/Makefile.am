#
#	'cnc_server/static' makefile
#
#	Copyright (C) 2015-2017	IC Manage Inc
#
#	See the file 'COPYING' for license information.
#

#
#  Install the software which provides the interactive API documentation.
#
all:
	if [ ! -d $(top_srcdir)/static/swagger-ui ]; then \
	  (cd $(top_srcdir)/static; \
	  $(PROG_GIT) clone https://github.com/swagger-api/swagger-ui.git; \
	  rm -rf swagger-ui/.git) \
	fi

clean-local:
	rm -rf $(top_srcdir)/static/swagger-ui
