#! /usr/bin/perl -w

while($dir = shift @ARGV) {
  @files = `find $dir -name core`;
  for $file (@files) {
    chomp $file;
    print "rm $file\n";
  }
}
