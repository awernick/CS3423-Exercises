#! /usr/bin/perl -w

use File::Basename;

$newname = shift;

while($file = shift) {
  ($oldname, $path, $ext) = fileparse($file, '\..*');
  if(defined($ext)) {
    print "mv $path$oldname$ext $path$newname$ext\n";
  }
}
