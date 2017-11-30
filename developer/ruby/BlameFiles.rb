#!/usr/bin/ruby1.8 -w
#
#This program goes through all the files in the SVN repository
#and performs a blame and saves all the "blamed" files to the
#newrootname as defined below.  

#note that the blame code only works if you have svn installed 
#(not just tortoise).  I've been running this

#note that the "NEW_DIR" must already be created to run this

require 'rubygems'
require 'pathname'

GLOBPATH = '../../../openstudio/**/*.*'
CLEAN_GLOBPATH = '../../../openstudio-blame/**/*.*'
CURRENT_DIR = './openstudio/'
NEW_DIR = 

EXCLUDE_DIRS = ["/boost-enum/", "/boost-log/", "/qwt/", "/gtest/", "/build/", "/squish/", "/expat/", "/orm/", "/sqlite/", "/dl/", "/libssh/"]
INCLUDE_FILES = [".txt", ".cpp", ".hpp", ".hxx", ".cxx", ".cs", ".rb"]

def forcepath(filename, skipcnt, rootname, newrootname)
  #skipcnt is the number of directorys to descend
  #before starting the forcepath
  
  #create the rootname if not nil
  #if newrootname != ""
  #  Dir.mkdir(newrootname) unless File.directory?(newrootname) 
  #end
  
  path = Pathname.new(File.dirname(filename))
  
  i = 0
  path.descend do |newp|
    i += 1
    if i > skipcnt
      #puts newp
  
      newp_s = newp.to_s
      newp_s.gsub!(rootname, newrootname)
       
      #puts "creating new directory #{newp_s}"
      Dir.mkdir(newp_s) unless File.directory?(newp_s)
    end
  end
end

def check_skipfile(filename, exclude_list)
  skipfile = false
  exclude_list.each do |exclude|
    if filename.to_s.include?(exclude)
      skipfile = true
      puts "skipping: #{filename}"
      break
    end
  end
  return skipfile
end

if (ARGV[0].nil?) or (ARGV[0] == "")
  puts "You must pass an argument"
  puts "  /list : lists the files that will be blamed"
  puts "  /run : run the blame on all the files in the repository"
  puts "  /break: break out the code for each subcontractor"
elsif ARGV[0].downcase == "/list"
  files = Pathname.glob(GLOBPATH)
  files.each do |file|
    if not check_skipfile(file, EXCLUDE_DIRS) 
      puts file
    end
  end
elsif ARGV[0].downcase == "/run" 
  files = Pathname.glob(GLOBPATH)
  
  files.each do |file|
    next if check_skipfile(file, EXCLUDE_DIRS)

    forcepath(file, 2, CURRENT_DIR, NEW_DIR) 
    
    if File.file?(file) then
      puts "processing: #{file}" 
      newfile = file.to_s.gsub!(CURRENT_DIR, NEW_DIR)
      call = "svn blame #{file} > #{newfile}"
      #puts call
      system(call)
    end
  end
elsif ARGV[0].downcase == "/break"
  users = Hash.new
  files = Pathname.glob(CLEAN_GLOBPATH)
 
  cnt = 0
  files.each do |file|
    cnt += 1
        
    if not INCLUDE_FILES.include?(File.extname(file))
      puts "skipping #{file} | with ext #{File.extname(file)}"
      next
    end
      
    puts "processing: #{file}"
    infile = File.open(file, "r")
    infile.each_line do |line|
        #puts line
        
        user = line[9..17].strip
        ucnt = users[user]
        ucnt = 0 if ucnt.nil?
        users[user] = ucnt + 1
        
        user_dir = "/openstudio-user-#{user}/"
        newfile = Pathname.new(file.to_s.gsub!('/openstudio-blame/', user_dir))
        puts "saving:     #{newfile}"
        forcepath(newfile, 3, "/openstudio-blame/", user_dir)
        if File.exists?(newfile) then
          File.open(newfile, "a") do |outfile|
            outfile << line[17..line.length]
            #outfile << line
          end
        else
          File.open(newfile, "w") do |outfile|
            outfile << line[17..line.length]
            #outfile << line
          end
        end
      end
    
    #break if cnt == 4
  
  end
  
  users.each_key do |key|
    puts "#{key}"
  
    #autozip up the
    Dir.chdir("/home/nlong/projects/")
    puts Dir.pwd
    zipfilename = "openstudio-#{key}.zip"
    File.delete(zipfilename) if File.exists?(zipfilename)
    File.delete("#{zipfilename}.rename") if File.exists?("#{zipfilename}.rename")
    puts zipfilename
    system("zip -r #{zipfilename} openstudio-user-#{key}")
    system("cp #{zipfilename} #{zipfilename}.rename")
  end
  
  puts "coding stats"
  users.each_key do |key|
    puts "#{key} : #{users[key]}"
  end

  
end



