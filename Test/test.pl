#! /usr/bin/perl -w

$a = shift;
$b = shift;
$b = shift;
#push(@ARGV, $b);
#push(@ARGV, $b);
@c = <>;
@d = splice(@c, 1, 3, ("harrison"));

print "THIS IS B:\n$b";
print "THIS IS B:\n$b";
print "THIS IS C:\n@c";
print "THIS IS D:\n@d";
