# to run this script, change directories to /build/src and call it

tmpout1 = "dumpbin.out"
tmpout2 = "undname.out"
buildType = "Debug"

symbols = Hash.new

excludes = [/boost-log/, /expat/, /gtest/, /libssh/, /litesql/, /qwt/, /sqlite/]

Dir.glob("**/#{buildType}/*.obj").each do |file|

  excludeThis = false
  excludes.each do |exclude| 
    if exclude.match(file)
      excludeThis = true
      break
    end
  end
  
  next if excludeThis

  puts file
  system("dumpbin/directives #{file} > #{tmpout1}")
  system("undname #{tmpout1} > #{tmpout2}")
  
  File.open(tmpout2, 'r') do |f|
    while line = f.gets
      if matchdata = /\/EXPORT:(.*)/.match(line)
        symbol = matchdata[1]
        count = symbols[symbol]
        if count
          symbols[symbol] = count + 1
        else
          symbols[symbol] = 1
        end
      end
    end
  end
end

symbols = symbols.to_a
symbols.sort! {|a,b| b[1] <=> a[1]}

File.open("results.out", 'w') do |f|
  symbols.each {|x| f.puts "#{x[1]}: #{x[0]}"}
end
