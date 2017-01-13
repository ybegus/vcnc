/*
 *	Copyright (C) 2008-2013	Roger March
 *	Copyright (C) 2015	IC Manage Inc.
 *
 *	See the file 'COPYING' for license information.
 */
#if	!defined(@PACKAGE@_Rev_H)
#define	@PACKAGE@_Rev_H
///
///	\return	The release version number string.
///
const char *@PACKAGE@_version_number();
///
///	\return	Release build number ("current" if not relevant).
///
const char *@PACKAGE@_release_change();
///
///	\return	The major version number string.
///
const char *@PACKAGE@_version_major();
///
///	\return	The minor version number string.
///
const char *@PACKAGE@_version_minor();
///
///	\return	The package name string.
///
const char *@PACKAGE@_package();
//
#endif
