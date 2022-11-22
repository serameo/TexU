#!./auitclsh

# set user
aui_settext 10008 1000
set user [aui_gettext 10008]
if {$user == "1000"} {
  puts "set user OK"
} else {
  puts "set user failed "
}

# set passwd 
aui_settext 10009 H1W0RLD
set passwd [aui_gettext 10009]
if {$passwd == "H1W0RLD"} {
  puts "set password OK"
} else {
  puts "set password failed"
}

# send command (12) ID_LOGIN (10001)
aui_sendmsg -1 12 10001
set info [aui_gettext 10011]
if {$info == "Login OK"} {
  puts "Login successfully"
} else {
  puts "Login FAILED"
}

