#!@PROG_PERL@
#
#	Copyright (C)	2013		Roger March
#
my $dir = $ARGV[0];
#
#	Ask GIT for our current hash.
#
$mxc = `@PROG_GIT@ log -n 1 --format="%H"`;
chop $mxc;
#
#	Put the value in the ::main scope.
#
print "//\n//\t\Change.C\n//\n";
print "#include <Rev.H>\n//\n";
#
print "const char *@PACKAGE@_release_change() { return (\"$mxc\"); }\n//\n";
#
exit(0);
