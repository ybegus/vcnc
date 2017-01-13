/*
 *	Copyright (C) 2008-2013	Roger March
 *
 *	See the file 'COPYING' for license information.
 */
#include <config.h>
#include <string.h>
#include <algorithm>
//
#include <Rev.H>
//
const char *@PACKAGE@_version_number() { return (VERSION); }
//
const char *@PACKAGE@_version_major(){
  static char buf[8];
  const char *v = VERSION;
  unsigned s = ::strlen(v);
  const char *e = std::find(v,v + s,'.');
  std::copy(v,e,&buf[0]);
  buf[e - v] = '\0';
  return (buf);
}
//
const char *@PACKAGE@_version_minor(){
  static char buf[8];
  const char *v = VERSION;
  unsigned s = ::strlen(v);
  const char *b = std::find(v,v + s,'.') + 1;
  const char *e = std::find(b,v + s,'.');
  std::copy(b,e,&buf[0]);
  buf[e - v] = '\0';
  return (buf);
}
//
const char *@PACKAGE@_package(){
  return (PACKAGE);
}
