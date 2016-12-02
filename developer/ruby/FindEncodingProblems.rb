require 'parallel'

messages = []
files = Dir.glob( File.dirname(__FILE__) + "/../../**/*.*") 
puts "examining #{files.size} files"

Parallel.each(files, in_threads: 8) do |p|
  
  if /build/.match(p) || /style/.match(p)
    next
  end
  puts "checking #{p}"
  
  output = `file #{p}`
  
  if /BOM/.match(output)
    messages << output
  end
end

puts messages.join("\n")
