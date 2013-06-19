#!/usr/bin/ruby1.8 -w
#
#This program goes through all the files in the SVN repository
#and performs a blame and saves all the "blamed" files to the
#newrootname as defined below.  

#note that the blame code only works if you have svn installed 
#(not just tortoise).  I've been running this

require 'pathname'
require 'ftools'

GLOBPATH = '../../../zerokit/**/{*.hpp}'

EXCLUDE_DIRS = ["/boost-enum/", "/boost-log/", "/qwt/", "/gtest/"]

if ARGV[0] == ""
  puts "You must pass an argument"
  puts "  /list : lists the files that will be blamed"
  puts "  /run : run the routine to clean up the files"
elsif ARGV[0].downcase == "/list"
  files = Pathname.glob(GLOBPATH)
  files.each {|f| puts f}
elsif ARGV[0].downcase == "/run" 
  files = Pathname.glob(GLOBPATH)
  
  files.each do |file|
    skipfile = false
    EXCLUDE_DIRS.each do |exclude|
      if file.to_s.include?(exclude) 
        skipfile = true
        puts "skipping: #{file}"
        next
      end
    end 
  
    next if skipfile

    if File.file?(file) then
      puts "processing: #{file}" 
      
      text = ""
      #open the file, and replace the following 
      File.open(file, "r") do |op|
        text = op.read
        
        #find all the register loggers
        regex = /REGISTER_LOGGER\((.*)\)\s*$/

        text.gsub!(regex, 'REGISTER_LOGGER(\1);')  
        
      end
      
      # write file
      File.open(file, "w") do |op|
        op << text
      end

      
    end
  end
end
