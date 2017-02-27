/User-Name/ {
  user = $3
}
/Acct-Status-Type/ {
  if($3 == "Stop") {
    users[user]["sessions"]++;
  }
}
/Acct-Session-Time/ {
  users[user]["time"] += $3;
}
/Acct-Input-Packets/ {
  users[user]["input"] += $3;
}
/Acct-Output-Packets/ {
  users[user]["output"] += $3;
  user = ""
}

END {
  for(u in users) {
    print "User:\t\t\t", u
    print "Number of Sessions:\t", users[u]["sessions"]
    print "Total Connect Time:\t", users[u]["time"]
    print "Input Bandwidth Usage:\t", users[u]["input"]
    print "Output Bandwidth Usage:\t", users[u]["output"]
    print
  }
}
