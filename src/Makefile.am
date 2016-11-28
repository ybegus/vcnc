#
#	'cnc_server/src' makefile
#
#	Copyright (C) 2015-2016	IC Manage Inc
#
#	See the file 'COPYING' for license information.
#

AM_CPPFLAGS =				\
	-I${top_srcdir}/src		\
	-I$(top_builddir)/src		\
	$(PEPSIS_CFLAGS)		\
	-I$(pepsis_dir)/bld/src		\
	-D_FILE_OFFSET_BITS=64		\
	-I$(TOOLROOT)/include		\
	-I$(TOOLROOT)/include/pepsis

SUBDIRS =						\
	rev

bin_PROGRAMS = vcnc_version
#
vcnc_version_SOURCES = 					\
	$(top_srcdir)/src/vcnc_version.cc
#
vcnc_version_LDFLAGS = 					\
	$(STATIC_LINK) $(FRPATH)			\
	-L$(TOOLROOT)/lib64 -R$(TOOLROOT)/lib64
#
vcnc_version_LDADD = 					\
	$(PEPSIS_LIBS)					\
	$(top_builddir)/src/rev/libcnc-rev.la
