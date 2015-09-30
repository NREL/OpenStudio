######################################################################
#  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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