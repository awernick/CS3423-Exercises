/Acct-Input-Packets/ { input += $3 }
/Acct-Output-Packets/ { output += $3 }
END {
  print "Total Input Bandwidth Usage  =", input, "packets"
  print "Total Output Bandwidth Usage =", output, "packets"
}
