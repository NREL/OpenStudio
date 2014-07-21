`otool -L #{ARGV[0]}`.split("\n").each do |line|

  if lib = /[\s]*(libruby[^\s]+)/.match(line)
    `install_name_tool -change #{lib[1]} @executable_path/../Frameworks/Ruby.framework/Versions/2.0/libruby.2.0.0.dylib #{ARGV[0]}`
  end
  
end