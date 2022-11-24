#!./auitclsh

# set user
set usrid 10004
aui_settext $usrid "1001"
set user [aui_gettext $usrid]
if {$user == "1001"} {
  puts "set user OK"
} else {
  puts "set user failed "
}

# set passwd 
set pwdid 10005
aui_settext $pwdid "H1W0RLD"
set passwd [aui_gettext $pwdid]
puts $passwd
#if {$passwd == "H1W0RLD"} {
#  puts "set password OK"
#} else {
#  puts "set password failed"
#}

# send command (12) ID_LOGIN (10001)
set ID_LOGIN 10001
#aui_sendmsg -1 TEXU_WM_COMMAND $ID_LOGIN
aui_sendmsg $ID_LOGIN TEXU_BM_PUSH
set infoid 10006
set info [aui_gettext $infoid]
if {$info == "Invalid user or password"} {
  puts "PASSED - Test login failed"
} else {
  puts "FAILED - Test login failed"
}

