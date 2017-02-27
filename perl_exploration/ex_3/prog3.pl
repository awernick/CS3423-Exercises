#! /usr/bin/perl -w 

while(<>) {
  open(FILE, $_) or die("Could not open $_");
  while(<FILE>) {
    print;
  }
  close(FILE)
}
