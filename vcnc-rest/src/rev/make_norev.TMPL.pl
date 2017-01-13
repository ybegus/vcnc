#!@PROG_PERL@
#
#	Copyright (C)	2013		Roger March
#
my $dir = $ARGV[0];
#
#	Put the value in the ::main scope.
#
print "//\n//\t\Change.C\n//\n";
print "#include <Rev.H>\n//\n";
#
print "const char *@PACKAGE@_release_change() { return (\"none\"); }\n//\n";
#
exit(0);
