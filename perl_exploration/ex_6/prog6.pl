#! /usr/bin/perl -w

$rex = shift;
$dir = shift;

open(FILES, "/usr/bin/find $dir -maxdepth 1 -type f -print | sort |") or die ("Could not execute find: $dir");

while($file = <FILES>) {
  chop $file;
  open(FILE, $file) or die("Could not open $file");

  while(<FILE>) {
    if(/$rex/) {
      chop;
      print "$file:$_\n";
    }
  }
}
