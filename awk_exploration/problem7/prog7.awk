/[0-9]+:[0-9]+:[0-9]+/ {
  disconnect = $4;
  FS = "\""
}
/User-Name/ {
  username = $2
  print username
}
/Acct-Session-Id/ {
  id = $2
  print id
  FS = " "
  users[id]["username"] = username
  users[id]["disconnect"] = disconnect
}
/Acct-Session-Time/ {
  users[id]["time"] = $3;
}
/Ascend-First-Dest/ {
  users[id]["dest"] = $3;
}
/Framed-Address/ {
  users[id]["address"] = $3;
  id = ""
}

END {
  for(id in users) {
    print "User:\t\t\t", users[id]["username"]
    print "Session Id:\t\t", id
    print "Framed Address:\t\t", users[id]["address"]
    print "Disconnect Time:\t", users[id]["disconnect"]
    print "Session Time:\t\t", users[id]["time"]
    print "First Address:\t\t", users[id]["dest"]
    print
  }
}
