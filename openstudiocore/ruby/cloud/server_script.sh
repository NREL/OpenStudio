#!/bin/sh

# AWS Server Bootstrap File
# This script is used to configure the AWS boxes

# Change Host File Entries
ENTRY="localhost localhost master"
FILE=/etc/hosts
if grep -q "$ENTRY" $FILE; then
  echo "entry already exists"
else
  sh -c "echo $ENTRY >> /etc/hosts"
fi

rm -rf /mnt
mkdir /mnt

# copy all the setup scripts to the appropriate home directory
# the scripts are called by the AWS connector for passwordless ssh config
cp /data/launch-instance/setup* /home/ubuntu/
chmod 775 /home/ubuntu/setup*
chown ubuntu:ubuntu /home/ubuntu/setup*

# stop the various services that use mongo
service delayed_job stop
service apache2 stop
service mongodb stop

# remove mongo db & add it back
mkdir -p /mnt/mongodb/data
chown mongodb:nogroup /mnt/mongodb/data
rm -rf /var/lib/mongodb

# restart mongo - old images has mongodb as the service. New ones use mongod
service mongodb start
service mongod start

# delay the continuation because mongo is a forked process and when it initializes
# it has to create the preallocated journal files (takes ~ 90 seconds on a slower system)
sleep 2m

# restart the rails application
service apache2 stop
service apache2 start

# Add in the database indexes after making the db directory
chmod 777 /var/www/rails/openstudio/public
su - ubuntu -c 'cd /var/www/rails/openstudio && bundle exec rake db:purge'
su - ubuntu -c 'cd /var/www/rails/openstudio && bundle exec rake db:mongoid:create_indexes'

## Worker Data Configuration -- On Vagrant this is a separate file

# Force the generation of various directories that are in the EBS mnt
rm -rf /mnt/openstudio
mkdir -p /mnt/openstudio
chown -R ubuntu:www-data /mnt/openstudio
chmod -R 775 /mnt/openstudio

# save application files into the right directory
cp -rf /data/worker-nodes/* /mnt/openstudio/

# install workflow dependencies - not yet needed for OpenStudio but still including
su - ubuntu -c 'cd /mnt/openstudio && rm -f'
rm -f /mnt/openstudio/Gemfile.lock
cd /mnt/openstudio && bundle update
su - ubuntu -c 'cd /mnt/openstudio && bundle update'

# copy over the models needed for mongo
cd /mnt/openstudio/rails-models && unzip -o rails-models.zip -d models
# Support the old  extraction as well - keep until we start using the new images
cd /mnt/openstudio/rails-models && unzip -o rails-models.zip

# rerun the permissions after unzipping the files
chown -R ubuntu:www-data /mnt/openstudio
find /mnt/openstudio -type d -print0 | xargs -0 chmod 775
find /mnt/openstudio -type f -print0 | xargs -0 chmod 664

## End Worker Data Configuration

# restart rserve
service Rserve restart

# restart delayed jobs
service delayed_job start

# Delay 1 minutes to make sure everything had tme to start.
# This is a hack, sorry.
sleep 1m

# -- Old settings that may still be needed on the old images --
# Set permissions on rails apps folders
#sudo chmod 777 /var/www/rails/openstudio/public
#sudo chmod -R 777 /mnt/openstudio
# -- End old settings 

#file flag the user_data has completed
cat /dev/null > /home/ubuntu/user_data_done


