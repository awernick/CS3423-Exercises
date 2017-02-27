#! /usr/bin/perl -w

$rex = shift @ARGV;

while($file = shift @ARGV) {
  open(FILE, $file) or die("Could not open $file");

  while($line = <FILE>) {
    if($line =~ /$rex/) {
      print "$file:$line"
    }
  }
}
