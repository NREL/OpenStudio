
concrete_model_objects = []
File.open( File.dirname(__FILE__) + "/../../openstudiocore/src/model/ConcreteModelObjects.hpp") do |f|
  while line = f.gets
    if m = /^#include\s+\"(.*)\.hpp\"/.match(line)
      if !/Impl$/.match(m[1])
        concrete_model_objects << m[1].to_s
      end
    end
  end
end

translator_methods = []
headers = false
if headers
  # work on hpp
  File.open( File.dirname(__FILE__) + "/../../openstudiocore/src/energyplus/ForwardTranslator.hpp") do |f|
    while line = f.gets
      if m = /translate(.*)\s*\(/i.match(line)
        translator_methods << m[1].to_s
      end
    end
  end
else
  # work on cpp
  File.open( File.dirname(__FILE__) + "/../../openstudiocore/src/energyplus/ForwardTranslator.cpp") do |f|
    while line = f.gets
      if m = /case.*OS_(\S*)\s*:/i.match(line)
        name = m[1].to_s.gsub('_','')
        translator_methods << name.to_s
        #puts  name.to_s
      end
    end
  end
end

concrete_model_objects.each do |concrete_model_object|
  #puts concrete_model_object
  if translator_methods.index(concrete_model_object)
    #puts "Found translator for '#{concrete_model_object}'"
  else
    puts "#{concrete_model_object}"
  end
end
