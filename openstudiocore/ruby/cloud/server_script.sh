#!/bin/bash

# AWS Server Bootstrap File
# This script is used to configure the AWS boxes

# Unlock the user account
sudo passwd -u ubuntu

# Change Host File Entries
ENTRY="localhost localhost master"
FILE=/etc/hosts
if grep -q "$ENTRY" $FILE; then
  echo "entry already exists"
else
  sh -c "echo $ENTRY >> /etc/hosts"
fi

# copy all the setup scripts to the appropriate home directory
# the scripts are called by the AWS connector for passwordless ssh config
cp /data/launch-instance/setup* /home/ubuntu/
chmod 775 /home/ubuntu/setup*
chown ubuntu:ubuntu /home/ubuntu/setup*

# stop the various services that use mongo
service delayed_job stop
service apache2 stop
service mongodb stop
service mongod stop

# remove mongo db & add it back
mkdir -p /mnt/mongodb/data
chown mongodb:nogroup /mnt/mongodb/data
rm -rf /var/lib/mongodb

# restart mongo - old images has mongodb as the service. New ones use mongod
service mongodb start
service mongod start

# delay the continuation because mongo is a forked process and when it initializes
# it has to create the preallocated journal files (takes ~ 90 seconds on a slower system)
# Wait until mongo logs that it's ready (or timeout after 120s)
COUNTER=0
MONGOLOG=/var/log/mongo/mongod.log

# Clear out the log first
cat /dev/null > $MONGOLOG

grep -q 'waiting for connections on port' $MONGOLOG
while [[ $? -ne 0 && $COUNTER -lt 120 ]] ; do
    sleep 2
    let COUNTER+=2
    echo "Waiting for mongo to initialize... ($COUNTER seconds so far)"
    grep -q 'waiting for connections on port' $MONGOLOG
done

# Now we know mongo is ready and can continue with other commands
echo "Mongo is ready. Moving on..."

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

# install workflow dependencies
rm -f /mnt/openstudio/Gemfile.lock
su - ubuntu -c 'cd /mnt/openstudio && bundle'
# also install as root for now
cd /mnt/openstudio && /opt/rbenv/shims/bundle

# copy over the models needed for mongo
cd /mnt/openstudio/rails-models && unzip -o rails-models.zip -d models

# rerun the permissions after unzipping the files
chown -R ubuntu:www-data /mnt/openstudio
find /mnt/openstudio -type d -print0 | xargs -0 chmod 775
find /mnt/openstudio -type f -print0 | xargs -0 chmod 664

## End Worker Data Configuration

# restart rserve
service Rserve restart

# restart delayed jobs
service delayed_job start

#file flag the user_data has completed
cat /dev/null > /home/ubuntu/user_data_done
