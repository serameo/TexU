#!./auitclsh

# set user
atr_settext 10008 1000
set user [atr_gettext 10008]
if {$user == "1000"} {
  puts "set user OK"
} else {
  puts "set user failed "
}

# set passwd 
atr_settext 10009 H1W0RLD
set passwd [atr_gettext 10009]
if {$passwd == "H1W0RLD"} {
  puts "set password OK"
} else {
  puts "set password failed"
}

atr_sendmsg -1 12 10001
set info [atr_gettext 10011]
if {$info == "Login OK"} {
  puts "Login successfully"
} else {
  puts "Login FAILED"
}

