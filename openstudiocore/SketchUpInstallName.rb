`otool -L #{ARGV[0]}`.split("\n").each do |line|

  if lib = /^[\s]*([^\s]*libruby[^\s]+)/.match(line)
    `install_name_tool -change #{lib[1]} @rpath/libruby.2.0.0.dylib #{ARGV[0]}`
    `install_name_tool -add_rpath @executable_path/../Frameworks/Ruby.framework/Versions/2.0 #{ARGV[0]}` # SketchUp
    `install_name_tool -add_rpath @executable_path #{ARGV[0]}` # OpenStudio and PAT
    `install_name_tool -add_rpath @executable_path/../lib #{ARGV[0]}` # custom ruby
    #`install_name_tool -add_rpath @loader_path #{ARGV[0]}` we do not want ruby bindings to load their own libruby
    `install_name_tool -add_rpath /System/Library/Frameworks/Ruby.framework/Versions/2.0/usr/lib/ #{ARGV[0]}` # fall back to system ruby
    `install_name_tool -add_rpath /usr/lib #{ARGV[0]}` # fall back to system ruby
  end
  
end
