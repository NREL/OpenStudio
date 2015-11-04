require 'openstudio'
require 'optparse'

options = Hash.new
optparse = OptionParser.new do|opts|

  opts.on('--inputPath INPUTPATH', String, "Path to EnergyPlus Idf or OpenStudio Model" ) do |inputPath|
    options[:inputPath] = inputPath
  end
  
  opts.on('--outputPath OUTPUTPATH', String, "Output file to write" ) do |outputPath|
    options[:outputPath] = outputPath
  end
  
  opts.on( '-o', '--objectType OBJECTYPE', String, "Type of Idd Object" ) do |objectType|
    options[:objectType] = objectType
  end
  
  opts.on( '-n', '--nameRegex NAMEREGEX', String, "Regular Expression for Name of Idf Object" ) do |nameRegex|
    options[:nameRegex] = nameRegex
  end
  
  opts.on( '-f', '--field FIELD', Integer, "Field of Idf Object" ) do |field|
    options[:fields] = Array.new if not options[:fields]
    options[:fields] << field
  end
  
  opts.on( '-v', '--value VALUE', String, "Value of Field" ) do |value|
    options[:values] = Array.new if not options[:values]
    options[:values] << value
  end
  
end

optparse.parse!

if not options[:objectType] or not options[:nameRegex] or not options[:fields] or not options[:values]
  puts optparse
  exit
end

if options[:fields].size != options[:values].size
  puts "Mismatched field/value pairs"
  puts "Fields: #{options[:fields].join(',')}"
  puts "Values: #{options[:values].join(',')}"
  exit
end

objectType = options[:objectType].to_IddObjectType
nameRegex = Regexp.new(options[:nameRegex])
fields = options[:fields]
values = options[:values]

input_path = nil
if options[:inputPath]
  input_path = OpenStudio::Path.new(options[:inputPath])
else
  input_path = OpenStudio::Path.new("in.osm")
end

puts "Loading model from '" + input_path.to_s + "'."
raise "There is no file at '" + input_path.to_s + "'." if not File.exist?(input_path.to_s)
oIdfFile = OpenStudio::Workspace::load(input_path)
raise "Unable to load model from '" + input_path.to_s + "'." if oIdfFile.empty?
idf_file = oIdfFile.get

idf_file.getObjectsByType(objectType).each do |object|
  
  objectName = object.name
  
  if not objectName.empty?
    if not nameRegex.match(objectName.get)
      next
    end
  end
  
  fields.each_index do |i|
    object.setString(fields[i], values[i])
  end
  
end

output_path = nil
if options[:outputPath]
  output_path = options[:outputPath]
else
  output_path = "out.osm"
end

puts "Saving model to '" + output_path.to_s + "'."
File.open(output_path, 'w') do |f| 
  f.puts idf_file.to_s
end
