require 'openstudio'
require 'optparse'

options = Hash.new
optparse = OptionParser.new do|opts|

  opts.on('--inputPath INPUTPATH', String, "Path to OpenStudio Model" ) do |inputPath|
    options[:inputPath] = inputPath
  end
  
  opts.on('--outputPath OUTPUTPATH', String, "Output file to write" ) do |outputPath|
    options[:outputPath] = outputPath
  end
    
end

optparse.parse!

input_path = nil
if options[:inputPath]
  input_path = OpenStudio::Path.new(options[:inputPath])
else
  input_path = OpenStudio::Path.new("in.osm")
end

puts "Loading model from '" + input_path.to_s + "'."
oModel = OpenStudio::Model::Model::load(input_path)
raise "Unable to load model from '" + input_path.to_s + "'." if oModel.empty?
model = oModel.get

translator = OpenStudio::EnergyPlus::ForwardTranslator.new()
workspace = translator.translateModel(model)

output_path = nil
if options[:outputPath]
  output_path = options[:outputPath]
else
  output_path = "out.idf"
end

puts "Saving idf to '" + output_path.to_s + "'."
File.open(output_path, 'w') do |f| 
  f.puts workspace.to_s
end
