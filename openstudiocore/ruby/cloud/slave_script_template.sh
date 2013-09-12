#!/bin/sh
mkdir /home/ubuntu/test

# Change Host
#echo MASTER_IP MASTER_DNS MASTER_HOSTNAME >> /etc/hosts
echo MASTER_IP MASTER_HOSTNAME >> /etc/hosts

# Replace Hostname 
#echo "slave1" > /etc/hostname
#sudo hostname slave
#hostname
#replace hostname in /etc/hosts

# Restart Server
sudo service networking restart
