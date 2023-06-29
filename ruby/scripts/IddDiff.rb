########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

# call like: ruby IddDiff.rb /path/to/old_idd /path/to/new_idd

require 'openstudio'

old_idd = OpenStudio::IddFile::load(OpenStudio::Path.new(ARGV[0])).get
new_idd = OpenStudio::IddFile::load(OpenStudio::Path.new(ARGV[1])).get

old_object_hash = Hash.new
new_object_hash = Hash.new

old_idd.objects.each do |object|
  old_object_hash[object.name] = object
end

new_idd.objects.each do |object|
  new_object_hash[object.name] = object
end

puts

# things in old that are not in new
old_object_hash.each_key do |name|
  if new_object_hash[name].nil?
    puts "Object '#{name}' was in old Idd but not in new Idd"
  end
end

puts

# things in new that are not in old
new_object_hash.each_key do |name|
  if old_object_hash[name].nil?
    puts "Object '#{name}' is in new Idd but not in old Idd"
  end
end

puts

# things in both
old_object_hash.each_pair do |name, old_object|

  new_object = new_object_hash[name]

  next if not new_object

  if new_object != old_object
    puts "Object '#{name}' has changed"
  end

end
