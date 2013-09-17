#!/bin/sh
mkdir /home/ubuntu/test

# Change Host
#echo SERVER_IP SERVER_DNS SERVER_HOSTNAME >> /etc/hosts
echo SERVER_IP SERVER_HOSTNAME >> /etc/hosts

# Replace Hostname 
#echo "worker1" > /etc/hostname
#sudo hostname worker
#hostname
#replace hostname in /etc/hosts

# Restart Server
sudo service networking restart
