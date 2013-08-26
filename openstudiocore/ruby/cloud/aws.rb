######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

######################################################################
# == Synopsis
#
#   Uses the aws-sdk gem to communicate with AWS
#
# == Usage
#
#  ruby aws.rb ARGV[0] ARGV[1] ARGV[2] ARGV[3]
#
#  ARGV[0] - Access Key
#  ARGV[1] - Secret Key
#  ARGV[2] - Service (e.g. "EC2" or "CloudWatch")
#  ARGV[3] - Command (e.g. "describe_availability_zones")
#
######################################################################

require 'aws'
require 'json'
#require 'active_support/core_ext/hash/conversions'

def error(code, msg)
  puts ({:error=>{:code=>code,:message=>msg}}.to_json)
  exit(1)
end

if ARGV.length < 4
  error(-1, 'Invalid number of args')
end

if ARGV[0].empty? && ARGV[1].empty?
  error(401, 'AuthFailure')
end

AWS.config(
  :access_key_id => ARGV[0],
  :region => 'us-east-1',
  :secret_access_key => ARGV[1],
  :ssl_verify_peer => false
)

if ARGV[2] == "CloudWatch"
  aws = AWS::CloudWatch.new
else
  aws = AWS::EC2.new
end
begin
  case ARGV[3]
  when 'describe_availability_zones'
    resp = aws.client.describe_availability_zones
    puts resp.data.to_json
  when 'launch_instance'
    aws.security_groups.create("sec-group-#{Time.now.to_i}")
  else
    error(-1, "Unknown command: #{ARGV[3]} (#{ARGV[2]})")
  end
  #puts \"Status: #{resp.http_response.status}\"
rescue Exception => e
  if e.message == 'getaddrinfo: No such host is known. '
    error(503, 'Offline')
  end
  #puts Hash.from_xml(e.http_response.body).to_json
  error(e.http_response.status, e.code)
end
