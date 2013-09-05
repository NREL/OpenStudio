#!/bin/sh
mkdir /home/ubuntu/test

# Change Host
echo localhost localhost master >> /etc/hosts

# Replace Hostname
#echo "slave1" > /etc/hostname
#sudo hostname slave
#hostname
#replace hostname in /etc/hosts

# Restart Server
sudo service networking restart
