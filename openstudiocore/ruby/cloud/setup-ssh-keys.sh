#!/usr/bin/expect --

#
# passwordless ssh on the systems
#

set send_slow {1 .01}
log_user 1

system rm -f $env(HOME)/.ssh/id_dsa $env(HOME)/.ssh/id_dsa.pub
system rm -f $env(HOME)/.ssh/id_rsa $env(HOME)/.ssh/id_rsa.pub

puts "unlocking ubuntu ..."
if {[catch "spawn sudo passwd -u ubuntu"]} {
  puts "error"
}

puts "generating ssh key ..."
if {[catch "spawn /usr/bin/ssh-keygen -t rsa"]} {
  puts "error"
}

set timeout 30
expect {
  "Enter file in which to save the key" {
     send -s "\r"
     exp_continue
  }
  "Enter passphrase" {
     send -s "\r"
     exp_continue
  }
  "Enter same passphrase again" {
     send -s "\r"
     exp_continue
  }
  EOF {
     puts "got EOF"
  }
}

if {[catch "system cat $env(HOME)/.ssh/id_rsa.pub >> $env(HOME)/.ssh/authorized_keys" catch_result]} {
  puts "Error: $catch_result"
}

#
# create the config file
#

if {[catch "system \"cat << EOF > $env(HOME)/.ssh/config
StrictHostKeyChecking no
EOF\"" catch_result]} {
  puts "Error: $catch_result"

}

#
# Ensure that the permissions are correct on the ssh directories.
#
system chmod 700 $env(HOME)/.ssh
system chmod 600 $env(HOME)/.ssh/config
