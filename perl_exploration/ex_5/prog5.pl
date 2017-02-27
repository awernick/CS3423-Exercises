#! /usr/bin/perl -w

$dir_file = shift @ARGV;
open(DIR, $dir_file) or die("Could not open $dir_file");

while($dir = <DIR>) {
  chomp $dir;
  $cmd = "/usr/bin/find $dir -name core |\n";
  open(CORES, $cmd) or die("Could not execute find:$dir"); 

  while($core = <CORES>) {
    print "rm $core";
  }
}
