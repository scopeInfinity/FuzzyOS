#!/usr/bin/expect -f
set timeout 10

set port [lindex $argv 0]
set cmd [lindex $argv 1]

spawn telnet 127.0.0.1 $port

expect "(qemu) "

send -- "$cmd\r"

expect {
    "(qemu) " {}
    "closed" {}
}