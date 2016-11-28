#
#	$Id: //depot/repo/Pepsis/pepsis/.sub_local.pl#1 $
#
#	Copyright (C) 1998-2004	Roger March
#
#	See the file 'COPYING' for license information.
#
################################################################################
#
#	  This is the local parameter substitution script. Command line
#	arguments of:
#
#		X=Y
#
#	Cause triggers of the form, '@X@' in the input file to be converted
#	to 'Y' in the output stream.
#
use IO::File;
use IO::Handle;
use strict;
#
%::Subval = {};
#
#	Extract and define the substitution patterns
#
while (@ARGV){
  my $arg = shift(@ARGV);
  my $nam;
  my $val;
  if (($nam,$val) = ($arg =~ /^(\w+)\=(.*)/)){
    $::Subval{$nam} = $val;
  }
}
#
#	Process data from the passde stream to the standard output.
#
sub suck_data {
  my $fil = shift;
  #
  #	Go through all files.
  #
  my $lin;
  while ($lin = $fil->getline()){
    #
    #	Iterate on top of variable substitutions.
    #
    my $pat;
    while (($pat) = ($lin =~ /\@(\w+)\@/)){
      #
      #	See if we have a substitution pattern, otherwise use what we have.
      #
      my $val;
      if (exists $::Subval{$pat}){
	$val = $::Subval{$pat};
      } else {
	$val = $pat;
      }
      #
      #	Do the pattern substitution.
      #
      $lin =~ s/\@$pat\@/$val/g;
    }
    #
    #	If the current line is an inclusion pattern we have to recurse
    #	into a new file.
    #
    my $fnm;
    if (($fnm) = ($lin =~ /\@include\s+(\S+)\@/)){
      my $inf = new IO::File("$fnm",'r') || die "Can not open $fnm";
      suck_data($inf);
      next;
    }
    print $lin;
  }
}
#
#	Start processing from the standard input.
#
my $stdin = new IO::Handle;
$stdin->fdopen(fileno(STDIN),'r') || die "Can not redirect STDIN";
suck_data($stdin);
0;
