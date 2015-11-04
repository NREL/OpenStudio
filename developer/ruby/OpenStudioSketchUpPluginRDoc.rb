# Stages and then runs rdoc on OpenStudio.

# Inputs: 
#   ARGV[0] - path to binary directory containing config file

require ARGV[0] + 'config'
require 'pathname'
require 'fileutils'

include FileUtils

# check that called from command line directly
if not ($0 == __FILE__)
  puts "#{__FILE__} called from external script"
  exit
end

source_dir = "#{$OpenStudio_LibPath}openstudio/sketchup_plugin/"
name = "OpenStudio"
output_dir = "#{$OpenStudio_RDocifyPath}#{name}/"

puts "Staging files in #{output_dir}"

mkdir_p(output_dir) if not File.exists?(output_dir)

# empty output dir
Dir.glob("#{output_dir}*").each {|d| rm_rf(d)}
Dir.mkdir("#{output_dir}lib") 
Dir.mkdir("#{output_dir}sketchup") 

# copy only files we want
cp_r("#{source_dir}lib/.", "#{output_dir}lib/.")
cp_r("#{source_dir}sketchup/.", "#{output_dir}sketchup/.")
cp("#{source_dir}mainpage.rb", "#{output_dir}mainpage.rb")

# remove all svn
Dir.glob("./**/\.svn").each {|d| rm_rf(d)}

# call rdoc
Dir.chdir(output_dir)

puts "Calling rdoc from #{output_dir}"
system("#{$OpenStudio_RDOC} --main mainpage.rb")
