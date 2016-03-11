# run from inside /developer/ruby
# add path to ruby bindings to ruby include
# ruby -I ../../build/ruby/Release MakeModelMainPage.rb > out.txt

require 'openstudio'

class ClassInfo
  attr_accessor :name, :abstract, :base_class, :idd_object_type, :children_src, :allowable_child_types_src, :parent_src
  
  def to_s
    result = ""
    if @abstract
      result = "abstract class #{@name} < #{@base_class}\n"
    else
      result = "concrete class #{@name} < #{@base_class} wraps #{@idd_object_type}\n"
    end
    
    if @parent_src
      result += @parent_src
    end
    
    if @children_src
      result += @children_src
    end
    
    if @allowable_child_types_src
      result += @allowable_child_types_src
    end

    return result
  end
end

def parse_impl(obj_name, impl_header)
  
  info = ClassInfo.new
  info.name = obj_name
  info.abstract = true

  File.open(impl_header, 'r') do |file|
    while line = file.gets
      if m = Regexp.new("#{obj_name}\_Impl\s*:\s*public\s*([^\s]*)\_Impl").match(line)
        info.base_class = m[1]
      end
      if /virtual\s+const\s+IddObjectType\&\s+iddObjectType\(\)\s+const/.match(line)
        info.abstract = false
      end
    end
  end
  
  return info
end


def parse_cpp(info, cpp_filename)

  File.open(cpp_filename, 'r') do |file|
    while line = file.gets
      if /_Impl::children\(\s*\)\s+const/.match(line)
        first_time = true
        curly_count = line.count('{')
        children_src = line
        while (first_time or curly_count > 0) and line = file.gets
          first_time = false
          curly_count += line.count('{')
          curly_count -= line.count('}')
          children_src += line
        end
        info.children_src = children_src
      elsif /_Impl::allowableChildTypes\(\s*\)\s+const/.match(line)
        first_time = true
        curly_count = line.count('{')
        allowable_child_types_src = line
        while (first_time or curly_count > 0) and line = file.gets
          first_time = false
          curly_count += line.count('{')
          curly_count -= line.count('}')
          allowable_child_types_src += line
        end
        info.allowable_child_types_src = allowable_child_types_src
      elsif /_Impl::parent\(\s*\)\s+const/.match(line)
        first_time = true
        curly_count = line.count('{')
        parent_src = line
        while (first_time or curly_count > 0) and line = file.gets
          first_time = false
          curly_count += line.count('{')
          curly_count -= line.count('}')
          parent_src += line
        end
        info.parent_src = parent_src
      end
      
    end
  end
  
  return info
end



model_dir = "../../openstudiocore/src/model/"

puts
puts "***************************************************"
puts "parsing mainpage"

# parse main page
main_page_objects = Hash.new
File.open(model_dir + "mainpage.hpp", 'r') do |file|
  while line = file.gets
    if /\\subsection modelobject_hierachy Parent-Child Hierarchy/.match(line)
      while line = file.gets
        if /\<ul\>/.match(line)
          break
        end
      end
      
      ul_count = 1
      while ul_count > 0 and line = file.gets
        if m = /\<li\>\s+([^\s]+)\s+\(([^,\s\)]+)/.match(line)
        
          info = ClassInfo.new
          info.abstract = false
          info.name = m[1]
          info.base_class = m[2]
        end
        
        ul_count += (line.split('<ul>').size - 1)
        ul_count -= (line.split('</ul>').size - 1)
        
        main_page_objects[info.name] = info
      end
      
    end
  end
end

puts
puts "***************************************************"
puts "parsing source files"

# parse files to find model objects
src_objects = Hash.new
Dir.glob(model_dir + "*.cpp").each do |cpp_filename|

  obj_name = File.basename(cpp_filename, '.cpp')
  
  next if obj_name == "ModelObject"
  
  impl_header = model_dir + obj_name + "_Impl.hpp"
  
  if not File.exists?(impl_header)
    puts "Can't find impl header for #{obj_name}"
    next
  end
  
  info = parse_impl(obj_name, impl_header)
  info = parse_cpp(info, cpp_filename)

  if not info.abstract
    begin
      idd_object_type = nil
      cmd = "idd_object_type = OpenStudio::Model::#{obj_name}::iddObjectType"
      #puts cmd
      eval(cmd, binding)
      info.idd_object_type = idd_object_type.valueName
    rescue => e
      puts "Could not determine iddObjectType for #{obj_name}. #{e}"
      info.idd_object_type = "Unknown"
    end
  end
  
  src_objects[obj_name] = info

end

puts
puts "***************************************************"
puts "comparing mainpage to source files"

# compare mainpage to src objects
main_page_objects.each_pair do |name, main_page_info|
  info = src_objects[name]
  if not info
    puts "#{name} is listed in the mainpage but there is no model object"
  else
    if main_page_info.base_class != info.base_class
      puts "mainpage lists #{name}'s base class as #{main_page_info.base_class} but it really is #{info.base_class}"
    end
  end
end

src_objects.each_pair do |name, info|
  main_page_info = main_page_objects[name]
  if not info.abstract and not main_page_info
    puts "#{name} is not listed in the mainpage"
  end
end

puts
puts "***************************************************"
puts "printing object information from source files"

def recursive_print(info, object_map)

  puts info.to_s
  
  if info.base_class != "ModelObject"
    base_info = object_map[info.base_class]
    if base_info
      recursive_print(base_info, object_map)
    else
      raise "Can't find info for base class #{info.base_class}"
    end
  end
  
end


src_objects.each_value do |info|

  if not info.abstract
    puts
    puts "***************************************************"
    recursive_print(info, src_objects)
  end

end