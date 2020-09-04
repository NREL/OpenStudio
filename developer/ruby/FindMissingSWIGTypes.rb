require 'json'

# Change this to match your settings
BUILD_DIRECTORY = '../../build'

GENERATED_SOURCE_DIRECTORY = File.join(BUILD_DIRECTORY, "csharp_wrapper/generated_sources")
subdirectory_names = Dir.entries(GENERATED_SOURCE_DIRECTORY).select {|entry| File.directory? File.join(GENERATED_SOURCE_DIRECTORY,entry) and !(entry =='.' || entry == '..') }

all_bad_swigs = {}
n_tot = 0

# Subprojects that aren't really concerning
ignore_subdirs = ['OpenStudioAirflow', 'OpenStudioISOModel', 'OpenStudioModelEditor']

subdirectory_names.each do |subdirectory_name|
  next if ignore_subdirs.include?(subdirectory_name)
  subdir_path = File.join(GENERATED_SOURCE_DIRECTORY, subdirectory_name)
  bad_swigs = Dir.glob(File.join(subdir_path, "*")).select{|e| /SWIGTYPE/.match(e)}.map{|e| File.basename(e)}
  n = bad_swigs.size
  n_tot += n
  if n == 0
    # puts "Ok for #{subdirectory_name}"
  else
    # puts "Not ok for #{subdirectory_name}: #{n} bad swigs found"
    all_bad_swigs[subdirectory_name] = bad_swigs
  end
end

if n_tot > 0
  puts "In total, #{n_tot} Bad swigs found"
  puts JSON.pretty_generate(all_bad_swigs)
end