#!/bin/sh

# Change Host File Entries
ENTRY="localhost localhost master"
FILE=/etc/hosts
if grep -q "$ENTRY" $FILE; then
  echo "entry already exists"
else
  sudo sh -c "echo $ENTRY >> /etc/hosts"
fi

# copy all the setup scripts to the appropriate home directory
cp /data/launch-instance/setup* /home/ubuntu/
chmod 775 /home/ubuntu/setup*
chown ubuntu:ubuntu /home/ubuntu/setup*

# Force the generation of various directories that are in the EBS mnt
sudo rm -rf /mnt/openstudio
sudo mkdir -p /mnt/openstudio
sudo chmod 777 /var/www/rails/openstudio/public

# save some files into the right directory
cp -rf /data/worker-nodes/* /mnt/openstudio/

# copy over the models needed for mongo
cd /mnt/openstudio/rails-models && unzip -o rails-models.zip

sudo chmod -R 777 /mnt/openstudio

#file flag the user_data has completed
cat /dev/null > /home/ubuntu/user_data_done
