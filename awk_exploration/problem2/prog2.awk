/User-Name/ { 
  gsub(/"/,"", $3)
  print $3
}
