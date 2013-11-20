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

# Set permissions on rails apps folders
sudo chmod 777 /var/www/rails/openstudio/public

# Force the generation of OpenStudio on the EBS mount and copy worker files
sudo rm -rf /mnt/openstudio
sudo mkdir -p /mnt/openstudio
sudo chmod -R 777 /mnt/openstudio
cp -rf /data/worker-nodes/* /mnt/openstudio/

# Unzip the worker files (including Mongoid models) and set permissions one last time
# Note that the 777 is redundant but needed until we actually deploy ACL
cd /mnt/openstudio/rails-models && unzip -o rails-models.zip
sudo chmod -R 777 /mnt/openstudio

# Force the generation of MongoDB dbpath on the EBS mount
sudo rm -rf /mnt/mongodb/data
sudo mkdir -p /mnt/mongodb/data
sudo chown mongodb:nogroup /mnt/mongodb/data
sudo service mongodb restart
sudo service delayed_job restart

# Add database indexes after it gets mounted for the first time
cd /var/www/rails/openstudio
sudo rake db:mongoid:create_indexes

#file flag the user_data has completed
cat /dev/null > /home/ubuntu/user_data_done


