#!/usr/bin/perl -w

$file1 = shift(@ARGV);
$file2 = shift(@ARGV);

open(FILE1, $file1) or die("Could not open $file1");
open(FILE2, $file2) or die("Could not open $file2");

while(1) { 
  $line1 = <FILE1>;
  $line2 = <FILE2>;

  if(not defined $line1 and not defined $line2) {
    last;
  }
  
  if(defined $line1) { 
    print $line1;
  }
  if(defined $line2) {
    print $line2;
  }
}
