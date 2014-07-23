`otool -L #{ARGV[0]}`.split("\n").each do |line|

  if lib = /^[\s]*([^\s]*libruby[^\s]+)/.match(line)
    `install_name_tool -change #{lib[1]} @rpath/libruby.2.0.0.dylib #{ARGV[0]}`
    `install_name_tool -add_rpath @executable_path/../Frameworks/Ruby.framework/Versions/2.0 #{ARGV[0]}` # SketchUp
    `install_name_tool -add_rpath @executable_path #{ARGV[0]}` 
    `install_name_tool -add_rpath @loader_path #{ARGV[0]}`
    `install_name_tool -add_rpath /usr/lib #{ARGV[0]}`
  end
  
end
