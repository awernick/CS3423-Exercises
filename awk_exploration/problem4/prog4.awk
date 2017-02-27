BEGIN {
  a = 0; b = 0; c = 0; d = 0; e = 0;
}
/Ascend-Data-Rate/ {
  if($3 <= 14400) { a++ }
  if($3 > 14400 && $3 <= 19200) { b++ }
  if($3 > 19200 && $3 <= 28800) { c++ }
  if($3 > 28800 && $3 <= 33600) { d++ }
  if($3 > 33600) { e++ }
}
END {
  print "0-14400\t\t"a
  print "14401-19200\t"b
  print "19201-28800\t"c
  print "28801-33600\t"d
  print "above 33600\t"e
}
