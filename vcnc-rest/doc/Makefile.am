#
#	'cnc_server/doc' makefile
#
#	Copyright (C) 2015	IC Manage Inc
#
#	See the file 'COPYING' for license information.
#
GENERATE_MD=$(top_srcdir)/node_modules/markdown-styles/bin/generate-md
MARKDOWN_SRCS = 		\
	api-overview.md		\
	index.md

MARKDOWN_HTML =			\
	api-overview.html	\
	index.html

$(MARKDOWN_HTML) : markdown-products-proxy
markdown-products-proxy : $(MARKDOWN_SRCS)
	[ -x $(GENERATE_MD) ] || { echo 'generate-md not found'; exit 1; }
	rm -rf $(top_srcdir)/vcnc-rest/doc-html
	mkdir -p $(top_srcdir)/vcnc-rest/doc-html
	$(GENERATE_MD) --input $(top_srcdir)/doc --output $(top_srcdir)/vcnc-rest/static/doc-html --layout mixu-page
	touch markdown-products-proxy

BUILT_SOURCES = $(MARKDOWN_HTML)
CLEANFILES = $(MARKDOWN_HTML) markdown-products-proxy

local-clean:
	rm -rf $(top_srcdir)/static/doc-html
