require 'openstudio'
require 'set'

# load set from file, if present
set = Set.new
if File.exist?("UUIDSet.txt")
  f = File.new("UUIDSet.txt","r")
  while (line = f.gets)
    set.add(OpenStudio::toUUID(line))
  end
end

# add ARGV[0] more UUIDs
m = set.size
puts "m = " + m.to_s
n = ARGV[0].to_i
puts "n = " + n.to_s
(0...n).each do |i|
  set.add(OpenStudio::UUID::create())
end

# test size of set
raise "Duplicate UUIDs created." if not (set.size == m + n)

# write set to file
f = File.new("UUIDSet.txt","w")
set.each { |uuid|
  f.puts uuid.to_s
}
f.close

