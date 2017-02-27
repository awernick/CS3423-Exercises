#! /usr/bin/perl -w

@processes = <>;
@sendmail = grep(/sendmail/,@processes);
@sendmail = grep(s/^\S+[ ]+([0-9]+).*sendmail: \S+ (.*@)*((\S+\.)+[^ .:]+).*/$3 $1/, @sendmail);
print @sendmail;

