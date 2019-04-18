src_dir = File.join(File.dirname(__FILE__), "../../openstudiocore/src/model/")

module_depends = {}
module_depends['ModelCore.i'] = nil
module_depends['ModelSimulation.i'] = 'ModelCore.i'
module_depends['ModelResources.i'] = 'ModelSimulation.i'
module_depends['ModelGeometry.i'] = 'ModelResources.i'
module_depends['ModelHVAC.i'] = 'ModelGeometry.i'
module_depends['ModelZoneHVAC.i'] = 'ModelHVAC.i'
module_depends['ModelAvailabilityManager.i'] = 'ModelHVAC.i'
module_depends['ModelPlantEquipmentOperationScheme.i'] = 'ModelHVAC.i'
module_depends['ModelStraightComponent.i'] = 'ModelZoneHVAC.i'
module_depends['ModelAirflow.i'] = 'ModelHVAC.i'
module_depends['ModelRefrigeration.i'] = 'ModelZoneHVAC.i'
module_depends['ModelGenerators.i'] = 'ModelHVAC.i'

module_depends_array = {}
module_depends.each do |key, value|
  depends_array = []
  while value 
    depends_array << value
    value = module_depends[value]
  end
  module_depends_array[key] = depends_array
end

# find all the modules and classes in those modules
modules = []
class_names = []
class_to_module = {}
Dir.glob(File.join(src_dir, "*.i")).each do |file_name|
  modules << file_name
  File.readlines(file_name).each do |line|
    md = /SWIG_MODELOBJECT\((.*), [01]\)/.match(line)
    if md.nil?
      md = /SWIG_UNIQUEMODELOBJECT\((.*), [01]\)/.match(line)
    end
    if md
      if !/_name/.match(md[1])
        class_names << md[1]
        class_to_module[md[1]] = File.basename(file_name)
      end
    end
  end
end

dot_in = ["digraph{"]
report_out = ""

n = class_names.size
class_names.each_index do |i|
  class_name = class_names[i]
  module_name = class_to_module[class_name]
  depends_array = module_depends_array[module_name]
  
  puts "#{i} out of #{n}, #{class_name}, #{module_name}"

  display_module_name = module_name.gsub('.i','') # names can't have '.' in graphviz
  display_class_name = class_name.gsub('Node', 'mNode') # Node is key word in graphviz
  
  file_name = File.join(src_dir, "#{class_name}.hpp")
  if !File.exist?(file_name)
    puts "no #{class_name}"
  else 
  
    #dot_in += "  #{display_class_name} -> #{display_module_name};\n"
    
    File.readlines(file_name).each do |line|
 
      line = line.gsub(/\/\/.*$/,'').gsub(/\/\*.*$/,'').gsub(/\*.*$/,'')
    
      class_names.each do |other_class_name|
      
        other_module_name = class_to_module[other_class_name]

        display_other_module_name = other_module_name.gsub('.i','') # names can't have '.' in graphviz
        display_other_class_name = other_class_name.gsub('Node', 'mNode') # Node is key word in graphviz
        
        next if class_name == other_class_name
        next if class_to_module[class_name] == class_to_module[other_class_name]
        next if depends_array.index(other_module_name)
        
        if /\b#{other_class_name}\b/.match(line)
          dot_in << "  #{display_module_name} -> #{display_other_module_name};"
          report_out += "#{class_name} in module #{module_name} points to #{other_class_name} in module #{other_module_name}:\n"
          report_out += "#{line}\n\n"
        end
      end
    end
  end
  
  #if i > 5
  #  break
  #end
end

dot_in << ["}"]

puts report_out

File.open('dot.in', 'w') do |file|
  file << dot_in.uniq.join("\n")
end

cmd = "dot -O -x -Kneato -Tpng ./dot.in"
puts cmd
system(cmd)

