#
#	'pepsis/src/rev' makefile
#
#	Copyright (C) 2008-2013	Roger March
#
#	See the file 'COPYING' for license information.
#
pepincdir = $(pkgincludedir)/rev
SUB_LOCAL = $(top_srcdir)/sub_local.pl
#
pkglib_LTLIBRARIES = libcnc-rev.la
#
pepinc_HEADERS =							\
	Rev.H
#
libcnc_rev_la_SOURCES =						\
	Change.C							\
	Rev.C
#
libcnc_rev_la_LDFLAGS =				\
	$(FRPATH) $(STATIC_LINK) $(VERSION_INFO)	\
	-L$(TOOLROOT)/lib64 -R$(TOOLROOT)/lib64		\
	-L$(TOOLROOT)/lib -R$(TOOLROOT)/lib
#
libcnc_rev_la_LIBADD =
#
#	Build number tagging.
#
PROJECTIONS =								\
	PACKAGE=$(PACKAGE)						\
	PROG_GIT=$(PROG_GIT)						\
	PROG_P4=$(PROG_P4)						\
	PROG_PERL=$(PROG_PERL)						\
	PROG_SED=$(PROG_SED)						\
	PROG_SH=$(PROG_SH)
#
#	Determine the change generator.
#
if	USE_P4
MAKE_REV = make_p4rev.TMPL.pl
else
if	USE_GIT
MAKE_REV = make_gitrev.TMPL.pl
else
MAKE_REV = make_norev.TMPL.pl
endif
endif
#
#	Constructed source.
#
Rev.C : Rev.TMPL.C $(SUB_LOCAL) Makefile.am
	$(PROG_PERL) $(SUB_LOCAL) < $< > $@ $(PROJECTIONS)
#
Rev.H : Rev.TMPL.H $(SUB_LOCAL) Makefile.am
	$(PROG_PERL) $(SUB_LOCAL) < $< > $@ $(PROJECTIONS)
#
make_rev.pl : $(MAKE_REV) $(SUB_LOCAL) Makefile.am
	$(PROG_PERL) $(SUB_LOCAL) < $< > $@ $(PROJECTIONS)
	chmod ugo+x $@
#
Change.C : make_rev.pl							\
	$(pkginclude_HEADERS)						\
	$(REV_FILES)
	$(top_builddir)/src/rev/$< $(top_srcdir) > $@
#
#	Create and fill in the "../po/POTFILES.in"
#
POTFILES = $(top_srcdir)/po/POTFILES.in
#
$(POTFILES) : POTDIRS Makefile
	@$(PROG_SED) -e "s|@|$(top_srcdir)|g" < $< > .tmp
	@$(PROG_SH) -f .tmp
	@$(PROG_RM) -f .tmp
#
BUILT_SOURCES =								\
	Change.C							\
	make_rev.pl							\
	Rev.C Rev.H							\
	$(POTFILES)
#
#  Yes, this is weird.  If $(POTFILES) is removed from BUILT_SOURCES,
#  config.status never gets created, and so the build never completes,
#  no matter how many {configure;make} pairs are run.
#
#  But if $(POTFILES) is cleaned, then po/Makefile complains that 
#  it doesn't exist during 'make clean'.
#
#  Other than ugly, this leaves $(top_srcdir)/po/POTFILES.in lying around,
#  even after 'make clean'.
#
CLEANFILES =								\
	Change.C							\
	make_rev.pl							\
	Rev.C Rev.H
