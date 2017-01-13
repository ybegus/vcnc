#!@PROG_PERL@
#
#	Copyright (C)	2013		Roger March
#
my $dir = $ARGV[0];
#
#	Create a stream to so we can process change numbers.
#
open(PS,"@PROG_P4@ files $dir/... |") || die "p4 stream failed";
my $l,my $mxc = 0;
while ($l = <PS>){
  my $chg;
  if (($chg) = ($l =~ /change (\d+) \(/)){
    $mxc = ($mxc < $chg) ? $chg : $mxc;
  }
}
close(PS);
#
#	Put the value in the ::main scope.
#
print "//\n//\t\Change.C\n//\n";
print "#include <Rev.H>\n//\n";
#
print "const char *@PACKAGE@_release_change() { return (\"$mxc\"); }\n//\n";
#
exit(0);
