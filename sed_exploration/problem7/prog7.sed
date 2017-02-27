# Delete all non-sshd
/sshd\[.*\]/!d;

# Delete everything that does not contain invalid
/Invalid/!d;

# Substitue all the way to the error message
s/.*\]: //

# Sub up to the username
s/[^ ]* [^ ]* //

# Get only the ip address
s/ [^ ]*// 
