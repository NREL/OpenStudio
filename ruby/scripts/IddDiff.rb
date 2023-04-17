########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
