BEGIN {
  for(i = 0; i < 4; i++) {
    cs2413[i] = 0
    cs2413d[i] = 0
    articles[i] = 0
    groups[i] = 0
    time[i] = 0
  }

  print "\t\t\tNews Reader Summary"
  print
  print
  print "\t\tlonestar\trunner\t\tringer\t\trings"
}

/lonestar.jpl.utsa.edu/    { idx = 0 }
/runner.jpl.utsa.edu/      { idx = 1 }
/ringer.cs.utsa.edu/      { idx = 2 }
/ring[0-9]+.cs.utsa.edu/  { idx = 3 }
{
  curr_time = $1 " " $2 " " $3
  if(NR == 1) {
    start_time = curr_time
  }

  if($7 == "group") {
    if($8 == "utsa.cs.2413") {
      cs2413[idx] += $9
    } 
    else if($8 == "utsa.cs.2413.d") {
      cs2413d[idx] += $9
    }
  }
  else if($7 == "exit") {
    articles[idx] += $9
    groups[idx] += $11
  }
  else if($7 == "times") {
    time[idx] += $13
  }
  idx = 6
}
END {
    printf "Articles:\t%-5s\t\t%-5s\t\t%-5s\t\t%-5s\n",\
       articles[0], articles[1], articles[2], articles[3]
    printf "Groups:\t\t%-5s\t\t%-5s\t\t%-5s\t\t%-5s\n",\
       groups[0], groups[1], groups[2], groups[3]
    printf "Cs2413:\t\t%-5s\t\t%-5s\t\t%-5s\t\t%-5s\n",\
       cs2413[0], cs2413[1], cs2413[2], cs2413[3]
    printf "Cs2413.d:\t%-5s\t\t%-5s\t\t%-5s\t\t%-5s\n",\
       cs2413d[0], cs2413d[1], cs2413d[2], cs2413d[3]
    printf "User Time:\t%-5s\t\t%-5s\t\t%-5s\t\t%-5s\n",\
       time[0], time[1], time[2], time[3]

    print ""
    print "Start Time =", start_time, "\t\t", "End Time =", curr_time
}

