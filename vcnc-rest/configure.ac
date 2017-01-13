#
#	'vCNC' configuration
#
#	Copyright (C) 2012-2016	IC Manage Inc
#
#	See the file 'COPYING' for license information.
#
AC_INIT(vCNC, 3.4.1, nick@icmanage.com)
AC_PREFIX_DEFAULT(/opt/vcnc)
#
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])
AM_SILENT_RULES([no])
#
#	Our major, minor and micro definitions.
#
version_major=`echo $VERSION | sed -e 's|\(.*\)\.\(.*\)\.\(.*\)|\1|'`
version_minor=`echo $VERSION | sed -e 's|\(.*\)\.\(.*\)\.\(.*\)|\2|'`
#
VERSION_INFO="-version-info $version_major:$version_minor:0"
AC_SUBST(VERSION_INFO)
#
#	Programing environment settings.
#
AC_GNU_SOURCE
AC_SYS_LARGEFILE
AC_LANG_CPLUSPLUS
AC_PROG_CXX
AC_PROG_CPP
AM_PROG_LIBTOOL
AM_PATH_PYTHON(2.6)
AX_PKG_SWIG
SWIG_ENABLE_CXX
SWIG_PYTHON
#AM_GNU_GETTEXT_VERSION([0.18.3])
#AM_GNU_GETTEXT([external])
#
#	Use static linking for programs.
#
AC_ARG_WITH(static_link,
  [  --with-static-link      link programs statically],
STATIC_LINK="-static-libtool-libs",
STATIC_LINK="")
AC_SUBST(STATIC_LINK)
#
#	Get our TOOLROOT variables from the prefix.
#
AC_ARG_WITH(toolroot,
  [  --with-toolroot=[[dir]]   define TOOLROOT directory [[PREFIX]]],
TOOLROOT="$withval",
if test "$prefix" = NONE; then
  optbase="$ac_default_prefix/TOOLROOT"
else
  optbase="$prefix/TOOLROOT"
fi
TOOLROOT="$optbase")
AC_SUBST(TOOLROOT)
#
#	Set the test run directory.
#
AC_ARG_WITH(testbase,
  [  --with-testbase=[[dir]]   test run directory [[/tmp/pepsis]]],
TESTBASE="$withval",
TESTBASE="/tmp/frq")
AC_SUBST(TESTBASE)
#
#	See where to find 'gettext.h'.
#
#AC_ARG_WITH(gettext,
#  [  --with-gettext=[[file]]   location of 'gettext.h' [[/usr/share/gettext/gettext.h]]],
#GETTEXT_H="$withval",
#GETTEXT_H="/usr/share/gettext/gettext.h")
#AC_SUBST(GETTEXT_H)
#
#	Allow Perforce versioning.
#
AC_ARG_ENABLE(p4rev,
  [  --disable-p4rev         do not use Perforce revision numbering],
  [],
  [enable_p4rev=yes])
#
#	See what kind of atomic library to use.
#
AC_ARG_ENABLE(tbb,
  [  --disable-tbb           do not use the TBB library],
  [],
  [enable_tbb=yes])
#
#  Set our version of node.  Sadly, this is necessary.  The
#  initial 'v' is also necessary.
#
NODE_VERSION=v6.6.0
#
#	Get program definitions.
#
AC_PATH_PROGS(PROG_CP,cp,,[$TOOLROOT/bin:$PATH])
AC_PATH_PROGS(PROG_DIFF,diff,,[$TOOLROOT/bin:$PATH])
AC_PATH_PROGS(PROG_GIT,git,,[$TOOLROOT/bin:$PATH])
AC_PATH_PROGS(PROG_GPERF,gperf,,[$TOOLROOT/bin:$PATH])
AC_PATH_PROGS(PROG_LN,ln,,[$TOOLROOT/bin:$PATH])
AC_PATH_PROGS(PROG_MKDIR,mkdir,,[$TOOLROOT/bin:$PATH])
AC_PATH_PROGS(PROG_MV,mv,,[$TOOLROOT/bin:$PATH])
AC_PATH_PROGS(PROG_P4,p4,,[$TOOLROOT/bin:$PATH])
AC_PATH_PROGS(PROG_PERL,perl perl5,,[$TOOLROOT/bin:$PATH])
AC_PATH_PROGS(PROG_PIDL,pidl,,[$TOOLROOT/bin:$PATH])
AC_PATH_PROGS(PROG_PKG_CONFIG,pkg-config,,[$TOOLROOT/bin:$PATH])
AC_PATH_PROGS(PROG_RM,rm,,[$TOOLROOT/bin:$PATH])
AC_PATH_PROGS(PROG_SED,sed,,[$TOOLROOT/bin:$PATH])
AC_PATH_PROGS(PROG_SH,bash sh,,[$TOOLROOT/bin:$PATH])
AC_PATH_PROGS(PROG_TAR,tar,,[$TOOLROOT/bin:$PATH])
AC_PATH_PROGS(PROG_NODE_GYP,node-gyp,,[$TOOLROOT/nvm/versions/node/$NODE_VERSION/lib/node_modules/npm/bin/node-gyp-bin:$PATH])
#
#  If node-gyp isn't there, might as well stop.
#
if test "x$PROG_NODE_GYP" = "x" ;then AC_MSG_ERROR([node-gyp not found]) ;fi
#
AM_CONDITIONAL(USE_P4,[test "x$PROG_P4" != "x"] && [test "$enable_p4rev" = "yes"])
AM_CONDITIONAL(USE_GIT,[test "x$PROG_GIT" != "x"] && [test "$enable_gitrev" = "yes"])
#
#	Update the PKG_CONFIG variable.
#
if test "x$PKG_CONFIG" = "x"; then
  PKG_CONFIG=$PROG_PKG_CONFIG
fi
#
#	Detect and use Pepsis.
#
PEPSIS_REV=6.5.16
#
PKG_CHECK_MODULES(PEPSIS,Pepsis >= $PEPSIS_REV)
AC_SUBST(PEPSIS_REV)
AC_SUBST(PEPSIS_CFLAGS)
AC_SUBST(PEPSIS_LIBS)
#
PKG_CHECK_MODULES(PEPSIS_PEER,Pepsis-peer >= $PEPSIS_REV)
AC_SUBST(PEPSIS_PEER_CFLAGS)
AC_SUBST(PEPSIS_PEER_LIBS)
#
#	Detect and use FRQU
#
FRQU_REV=4.0.0
#
#PKG_CHECK_MODULES(FRQU,FRQU >= $FRQU_REV)
#AC_SUBST(FRQU_REV)
#AC_SUBST(PEPSIS_CFLAGS)
#AC_SUBST(PEPSIS_LIBS)
#
#	Detect and use SSL.
#
PKG_CHECK_MODULES(SSL,openssl >= 0.9.8)
AC_SUBST(SSL_CFLAGS)
AC_SUBST(SSL_LIBS)
AM_CONDITIONAL([SSL],[test "x$SSL_LIBS" != "x"])
#
#	Detect and use log4cpp
#
PKG_CHECK_MODULES(LOG4CPP,log4cpp >= 1.1)
AC_SUBST(LOG4CPP_CFLAGS)
AC_SUBST(LOG4CPP_LIBS)
#
#	Type checks.
#
AC_C_BIGENDIAN
AC_HEADER_STDBOOL
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_STRUCT_ST_BLOCKS
AC_TYPE_UINT64_T
AC_CHECK_TYPES(wchar_t)
AC_CHECK_SIZEOF(char)
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)
#
#	Header checks.
#
AC_CHECK_HEADERS(sys/xattr.h)
AC_CHECK_HEADERS([fcntl.h libintl.h limits.h locale.h stdlib.h string.h unistd.h])
#
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_CHECK_MEMBERS([struct stat.st_rdev])
#
#	We want large files, are we sensitive to them?
#
#AX_SYS_LARGEFILE_SENSITIVE
#
#	Boost checks.
#
AX_BOOST_BASE([1.42])
#
#	Checks for library functions.
#
AC_FUNC_ALLOCA
AC_FUNC_CHOWN
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_STRERROR_R
AC_FUNC_STRTOD
AC_CHECK_FUNCS([fdatasync ftruncate mkdir mkfifo realpath rmdir setlocale socket strerror utime])
#
#	System information for runtime.
#
AC_DEFINE_UNQUOTED(CPU_TYPE,"$build_cpu",[Configured CPU type.])
AC_DEFINE_UNQUOTED(OS_TYPE,"$build_os",[Configured OS type.])
#
#	Definition for RPATH lebensraum.
#
rpath10="012345678910"
rpath50="/$rpath10/$rpath10/$rpath10/$rpath10/$rpath10"
RPATH="-Wl,-rpath -Wl,$rpath50$rpath50"
FRPATH="-R$rpath50$rpath50"
AC_SUBST(RPATH)
AC_SUBST(FRPATH)
#
#	Build the 'Makefiles'.
#
AC_OUTPUT([Makefile
	doc/Makefile
	installer/Makefile
	addon/Makefile
	static/Makefile
	src/Makefile
	src/rev/Makefile
	])
