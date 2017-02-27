BEGIN {
  print "\t\t\t\tIncoming News Feed Summary"
  print
  print
  print "\t\t\t\taccepted\trejected\tcanceled"
}
{ 
  time = $1 " " $2 " " $3

  if(NR == 1) {
    start_time = time
  }

  feed = $5
  if($4 == "+") {
    feeds[feed]["accepted"]++
  } 
  else if($4 == "-") { 
    feeds[feed]["rejected"]++
  }
  else if($4 == "c") {
    feeds[feed]["canceled"]++
  }
}
END {
  for(feed in feeds) {

    if(feeds[feed]["canceled"] < 1) {
      feeds[feed]["canceled"] = 0
    }

    if(feeds[feed]["rejected"] < 1) {
      feeds[feed]["rejected"] = 0
    }

    if(feeds[feed]["accepted"] < 1) {
      feeds[feed]["accepted"] = 0
    }

    printf "%-20s\t\t%-4s\t\t%-4s\t\t%-4s\n", \
       feed, feeds[feed]["accepted"], feeds[feed]["rejected"], feeds[feed]["canceled"]
  }
  
  print ""
  print "Start Time =", start_time, "\t", "End Time =", time
}
