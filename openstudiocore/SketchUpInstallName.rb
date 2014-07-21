a = []

`otool -L #{ARGV[0]}`.split("\n").each do |line|

  if lib = /^[\s]*([^\s]*libruby[^\s]+)/.match(line)
    `install_name_tool -change #{lib[1]} @executable_path/../Frameworks/Ruby.framework/Versions/2.0/libruby.2.0.0.dylib #{ARGV[0]}`
    a << lib[1]
  end
  
end

File.open("/Users/Dan/OpenStudio/build_10_9/OpenStudioCore-prefix/src/OpenStudioCore-build/yoyo.txt", "w") do |file|
  file.puts "yeah yeah"
  file.puts ARGV[0]
  file.puts a.join(',')
end