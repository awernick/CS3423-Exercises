BEGIN { x = 0 }
/Acct-Session-Time/ { x += $3 }
END { print "Total User Session Time =", x }
