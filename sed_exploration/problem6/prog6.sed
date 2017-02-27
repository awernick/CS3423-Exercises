# Delete whatever doesn't match to verification
/verification/!d;

# Remove up to the ip address
s/[^:]*:[^:]*:[^:]*:[^:]*: //

# Remove hostname
s/: [^ ]* /\t/

# Remove the rest of the string
s/ .*$//
