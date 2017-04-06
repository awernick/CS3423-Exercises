BEGIN {
  baidu = 0;
  ms = 0;
  msother = 0;
  all = 0;
}
/Baidu/ {
  baidu += $5;
}
/bingbot/ {
  if(/40\.77\.167\.39/) {
    ms += $5;
  } else {
    msother += $5;
  }
}
/[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+/{
  all += $5;
}
END {
  print "Baidu:", baidu;
  print "ms:", ms;
  print "msother:", msother;
  print "all:", all;
}
