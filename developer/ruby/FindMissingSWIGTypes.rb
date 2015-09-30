Dir.glob("../../build/csharp_wrapper/generated_sources/**/*.cs").each do |p|
  puts p if /SWIGTYPE/.match(p)
end