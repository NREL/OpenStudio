
messages = []
Dir.glob( File.dirname(__FILE__) + "/../../openstudiocore/src/**/*.hpp") do |p|
  
  if /litesql/.match(p)
    next
  end
  
  file_messages = []
  File.open(p) do |f|
    while line = f.gets
      if /\&\&/.match(line)
        next
      elsif /=/.match(line)
        next
      elsif m = /\&[^\(]*\(.*\)[^\)]*;/.match(line)
        if m = /operator=/.match(line)
        elsif m = /operator+=/.match(line)
        elsif m = /operator-=/.match(line)
        elsif m = /operator*=/.match(line)
        elsif m = /operator\/=/.match(line)
        elsif m = /operator<</.match(line)
        elsif m = /std::ostream/.match(line)
        elsif m = /outputVariableNames/.match(line)
        else
          file_messages << line.strip
        end
      end
    end
    
    if !file_messages.empty?
      messages << p
      messages.concat(file_messages)
      messages << ""
    end
  end
end

puts messages.join("\n")
