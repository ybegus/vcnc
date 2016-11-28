#
#	'cnc_server' makefile
#
#	Copyright (C) 2015	IC Manage Inc
#
#	See the file 'COPYING' for license information.
#
ACLOCAL_AMFLAGS = -I m4
SUB_LOCAL = $(top_srcdir)/sub_local.pl
#
#  'doc' must follower 'vcnc-rest' becasue 'vcnc-rest' installs generate-md
SUBDIRS = src po vcnc-rest doc installer
#
PROJECTIONS =						\
	VERSION=$(VERSION)
#
#$(top_srcdir)/FRQU.doxyfile : FRQU.TMPL.doxyfile configure.ac Makefile
#	$(PROG_PERL) $(SUB_LOCAL) < $< > $@		\
#	$(PROJECTIONS)
##
#EXTRA_DIST = config.rpath m4/ChangeLog mkinstalldirs m4/Makefile.in
##

#
#  Install an API spec editor which may be useful for development but is not
#  needed for deployment
#
swagger-editor:
	if [ ! -d $(top_srcdir)/swagger-editor ]; then \
	  (cd $(top_srcdir); $(PROG_GIT) clone https://github.com/swagger-api/swagger-editor.git); \
	fi
#
check-gettext:
	@if test x$(USE_NLS) != "xyes" ; then echo "Missing gettext. Rerun configure and check for" \
	"'checking whether to use NLS... yes'!" ; exit 1 ; fi
#
update-po: check-gettext
	@find $(srcdir)/src/ -name "*.cpp" -print | sort > $(srcdir)/po/POTFILES.in.2 ; \
	if diff $(srcdir)/po/POTFILES.in $(srcdir)/po/POTFILES.in.2 >/dev/null 2>&1 ; then \
		rm -f $(srcdir)/po/POTFILES.in.2 ; \
	else \
		mv $(srcdir)/po/POTFILES.in.2 $(srcdir)/po/POTFILES.in ; \
	fi
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-po
#
update-gmo: check-gettext
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo
#
force-update-gmo: check-gettext
	touch po/*.po
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo
#
force-update-gmo-%: check-gettext
	@language=`echo $@ | sed s/force-update-gmo-//` ; \
	if test ! -f po/$$language.po ; then echo "file po/$$language.po does not exist" ; exit 1 ; fi ; \
	touch po/$$language.po ; \
	cd po && $(MAKE) $(AM_MAKEFLAGS) update-gmo
#
.PHONY: check-gettext update-po update-gmo force-update-gmo
#
clean-local:
	rm -rf $(top_srcdir)/swagger-editor

install-data-local:
	find $(top_srcdir)/vcnc-rest -name '*~' -exec rm '{}' ';'
	rsync -a $(top_srcdir)/vcnc-rest $(DESTDIR)$(prefix)/share/vcnc
