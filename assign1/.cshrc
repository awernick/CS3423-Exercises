setenv OSTYPE `/bin/uname -s`

if($OSTYPE == "Linux") then
  setenv PATH ~/bin:/usr/local/turnin:${USRLOCAL}:${SYSTEM}:${FACBIN}:/home/maynard/bin:.
endif

if($?USER == 0 || $?prompt == 0) exit

# ENV 
setenv EDITOR '/usr/bin/vi'
setenv SUB_cs3423 'section1'

# Config
set cdpath=( ~/courses/cs ~ )
set prompt="`hostname`{`whoami`}\!: "
set ignoreeof
set history=100

# Aliases
alias ts 'eval `tset -s \@*`'
alias locate "locate -d /var/lib/mlocate/mlocate.db:/usr/local/mlocate/mlocate.db"
alias cs3423 "cd ~/courses/cs/3423"

