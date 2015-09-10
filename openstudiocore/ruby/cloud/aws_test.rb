# This is a silly script to execute the various endpoints of the aws.rb script. It requires the ENV vars for AWS to be set

# list total number of instances
`ruby aws.rb.in $AWS_ACCESS_KEY_ID $AWS_SECRET_ACCESS_KEY us-east-1 EC2 total_instances "{\"openstudio_version\":\"1.7.1\"}"`

# list availability zones
`ruby aws.rb.in $AWS_ACCESS_KEY_ID $AWS_SECRET_ACCESS_KEY us-east-1 EC2 describe_availability_zones "{\"openstudio_version\":\"1.7.1\"}"`

# get the status of the running instances
`ruby aws.rb.in $AWS_ACCESS_KEY_ID $AWS_SECRET_ACCESS_KEY us-east-1 EC2 instance_status "{\"openstudio_version\":\"1.7.1\"}"`

# launch a new instance
`ruby aws.rb.in $AWS_ACCESS_KEY_ID $AWS_SECRET_ACCESS_KEY us-east-1 EC2 launch_server "{\"openstudio_version\":\"1.7.1\", \"instance_type\":\"m3.medium\"}"`

# No workers specified should error out
`ruby aws.rb.in $AWS_ACCESS_KEY_ID $AWS_SECRET_ACCESS_KEY us-east-1 EC2 launch_workers "{\"openstudio_version\":\"1.7.1\", \"instance_type\":\"m3.medium\"}"`

# No worker nodes, but shouldn't error out
`ruby aws.rb.in $AWS_ACCESS_KEY_ID $AWS_SECRET_ACCESS_KEY us-east-1 EC2 launch_workers "{\"openstudio_version\":\"1.7.1\", \"instance_type\":\"m3.medium\", \"num\": 0}"`

# terminate the cluster
`ruby aws.rb.in $AWS_ACCESS_KEY_ID $AWS_SECRET_ACCESS_KEY us-east-1 EC2 terminate_session "{\"openstudio_version\":\"1.7.1\"}"`

# check if the cluster has terminated and if so, do some cleanup
`ruby aws.rb.in $AWS_ACCESS_KEY_ID $AWS_SECRET_ACCESS_KEY us-east-1 EC2 termination_status "{\"openstudio_version\":\"1.7.1\"}"`

# session uptime
`ruby aws.rb.in $AWS_ACCESS_KEY_ID $AWS_SECRET_ACCESS_KEY us-east-1 EC2 session_uptime "{\"openstudio_version\":\"1.7.1\"}"`

# cloudwatch estimated charges
`ruby aws.rb.in $AWS_ACCESS_KEY_ID $AWS_SECRET_ACCESS_KEY us-east-1 EC2 estimated_charges "{\"openstudio_version\":\"1.7.1\"}"`
