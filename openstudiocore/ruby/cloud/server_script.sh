#!/bin/sh
mkdir /home/ubuntu/test

# Change Host
echo localhost localhost server >> /etc/hosts

# Replace Hostname
#echo "worker1" > /etc/hostname
#sudo hostname worker
#hostname
#replace hostname in /etc/hosts

# Restart Server
sudo service networking restart
