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
#  ARGV[4] - Optional json with parameters associated with command
#
######################################################################

require 'aws'
require 'json'
require 'net/http'
require 'net/scp'
require 'net/ssh'
require 'tempfile'
#require 'active_support/core_ext/hash/conversions'

def error(code, msg)
  puts ({:error => {:code => code, :message => msg}}.to_json)
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
@image_id = 'ami-d0f89fb9'
@master_instance_type = 't1.micro'
@slave_instance_type = 't1.micro'

if ARGV[2] == "CloudWatch"
  @aws = AWS::CloudWatch.new
else
  @aws = AWS::EC2.new
end

if ARGV.length == 5
  @params = JSON.parse(ARGV[4])
end

def create_struct(instance)
  instance_struct = Struct.new(:instance, :id, :ip, :dns)
  return instance_struct.new(instance, instance.instance_id, instance.ip_address, instance.dns_name)
end

def launch_master
  user_data = File.read(File.expand_path(File.dirname(__FILE__))+'/master_script.sh')
  @master = @aws.instances.create(:image_id => @image_id,
                                  :key_pair => @key_pair,
                                  :security_groups => @group,
                                  :user_data => user_data,
                                  :instance_type => @master_instance_type)
  sleep 5 while @master.status == :pending
  if @master.status != :running
    error(-1, "Master status: #{@master.status}")
  end

  @master = create_struct(@master)
end

def launch_slaves(num, master_ip)
  user_data = File.read(File.expand_path(File.dirname(__FILE__))+'/slave_script_template.sh')
  user_data.gsub!(/MASTER_IP/, master_ip)
  user_data.gsub!(/MASTER_HOSTNAME/, 'master_name')
  instances = []
  num.times do
    slave = @aws.instances.create(:image_id => @image_id,
                                  :key_pair => @key_pair,
                                  :security_groups => @group,
                                  :user_data => user_data,
                                  :instance_type => @slave_instance_type)
    instances.push(slave)
  end
  sleep 5 while instances.any? { |instance| instance.status == :pending }
  if instances.any? { |instance| instance.status != :running }
    error(-1, "Slave status: Not running")
  end
  instances.each { |instance| @slaves.push(create_struct(instance)) }
end

def upload_file(host, local_path, remote_path)
  begin
    Net::SCP.start(host, 'ubuntu', :key_data => [@private_key]) do |scp|
      scp.upload! local_path, remote_path
    end
  rescue SystemCallError, Timeout::Error => e
    # port 22 might not be available immediately after the instance finishes launching
    sleep 1
    retry
  rescue
    # unknown upload error, retry
    sleep 1
    retry
  end
end

def send_command(host, command)
  begin
    Net::SSH.start(host, 'ubuntu', :key_data => [@private_key]) do |ssh|
      ssh.exec(command)
    end
  rescue Net::SSH::HostKeyMismatch => e
    e.remember_host!
    # key mismatch, retry
    sleep 1
    retry
  rescue SystemCallError, Timeout::Error => e
    # port 22 might not be available immediately after the instance finishes launching
    sleep 1
    retry
  end
end

def shell_command(host, command)
  begin
    Net::SSH.start(host, 'ubuntu', :key_data => [@private_key]) do |ssh|
      channel = ssh.open_channel do |ch|
        ch.exec "#{command}" do |ch, success|
          error(-1, "could not execute #{command}") unless success

          # "on_data" is called when the process writes something to stdout
          #ch.on_data do |c, data|
          #  $stdout.print data
          #end

          # "on_extended_data" is called when the process writes something to stderr
          #ch.on_extended_data do |c, type, data|
          #  $stderr.print data
          #end

          #ch.on_close { puts "done!" }
        end
      end
    end
  rescue Net::SSH::HostKeyMismatch => e
    e.remember_host!
    # "key mismatch, retry"
    sleep 1
    retry
  rescue SystemCallError, Timeout::Error => e
    # port 22 might not be available immediately after the instance finishes launching
    sleep 1
    retry
  end
end

begin
  case ARGV[3]
    when 'describe_availability_zones'
      resp = @aws.client.describe_availability_zones
      puts resp.data.to_json
    when 'launch_master'
      @timestamp = Time.now.to_i
      @group = @aws.security_groups.create("sec-group-#{@timestamp}")
      # web traffic
      #@group.authorize_ingress(:tcp, 80)
      # allow ping
      #@group.allow_ping()
      # ftp traffic
      #@group.authorize_ingress(:tcp, 20..21)
      # ftp traffic
      @group.authorize_ingress(:tcp, 1..65535)
      # ssh access
      #@group.authorize_ingress(:tcp, 22, '0.0.0.0/0')
      # telnet
      #@group.authorize_ingress(:tcp, 23, '0.0.0.0/0')

      # generate a key pair
      @key_pair = @aws.key_pairs.create("key-pair-#{@timestamp}")
      # save key to file
      #File.open("ec2.pem", "w") do |f| f.write(@key_pair.private_key)
      #end

      launch_master()

      puts ({:timestamp => @timestamp,
             :private_key => @key_pair.private_key,
             :master => {
                 :id => @master.id,
                 :ip => @master.ip,
                 :dns => @master.dns
             }}.to_json)
    when 'launch_slaves'
      if ARGV.length < 5
        error(-1, 'Invalid number of args')
      end

      @slaves = []
      @timestamp = @params['timestamp']
      @group = @aws.security_groups.filter('group-name', "sec-group-#{@timestamp}").first
      @key_pair = @aws.key_pairs.filter('key-name', "key-pair-#{@timestamp}").first
      @private_key = File.read(@params['private_key'])
      @master = @aws.instances[@params['master_id']]
      error(-1, "Master node does not exist") unless @master.exists?
      @master = create_struct(@master)
      launch_slaves(@params['num'], @master.ip)
      #@slaves.push(create_struct(@aws.instances['i-7db7641f']))
      #@slaves.push(create_struct(@aws.instances['i-deaf08b4']))

      slave_ips = ''
      @slaves.each { |slave| slave_ips << "#{slave.ip}\n" }
      file = Tempfile.new('hosts_slave_file')
      file.write(slave_ips)
      file.close
      upload_file(@master.ip, file.path, '/home/ubuntu/hosts_slave_file.sh')
      file.unlink

      logins = ''
      logins << "#{@master.dns}|ubuntu|ubuntu\n"
      @slaves.each { |slave| logins << "#{slave.dns}|ubuntu|ubuntu\n" }
      file = Tempfile.new('logins')
      file.write(logins)
      file.close
      upload_file(@master.ip, file.path, 'ip_addresses')
      file.unlink

      prefix = File.expand_path(File.dirname(__FILE__))+'/'
      upload_files = [prefix + 'setup-ssh-keys.sh', prefix + 'setup-ssh-worker-nodes.sh', prefix + 'setup-ssh-worker-nodes.expect', prefix + 'start_rserve.sh', prefix + 'R_config.rb']
      upload_files.each do |file|
        upload_file(@master.ip, file, File.basename(file))
      end

      send_command(@master.ip, 'chmod 774 ~/setup-ssh-keys.sh; chmod 774 ~/setup-ssh-worker-nodes.expect; chmod 774 ~/setup-ssh-worker-nodes.sh; chmod 774 ~/start_rserve.sh; chmod 774 ~/R_config.rb; sh ~/setup-ssh-keys.sh; sh ~/setup-ssh-worker-nodes.sh; nohup ~/start_rserve.sh </dev/null >/dev/null 2>&1 &')
      shell_command(@master.ip, '/usr/local/rbenv/shims/ruby ~/R_config.rb')

      slave_json = []
      @slaves.each { |slave|
        slave_json.push({
                            :id => slave.id,
                            :ip => slave.ip,
                            :dns => slave.dns
                        })
      }
      puts ({:timestamp => @timestamp,
             :private_key => @key_pair.private_key,
             :slaves => slave_json}.to_json)

    when 'terminate_instance'
      if ARGV.length < 5
        error(-1, 'Invalid number of args')
      end
      @aws.security_groups.filter('group-name', "sec-group-#{@params['timestamp']}").each do |group|
        group.delete
      end
    else
      error(-1, "Unknown command: #{ARGV[3]} (#{ARGV[2]})")
  end
    #puts \"Status: #{resp.http_response.status}\"
rescue Exception => e
  if e.message == 'getaddrinfo: No such host is known. '
    error(503, 'Offline')
  end
  #puts Hash.from_xml(e.http_response.body).to_json
  puts e
  #error(e.http_response.status, e.code)
end
