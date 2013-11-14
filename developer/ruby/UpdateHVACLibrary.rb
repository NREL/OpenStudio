require 'openstudio'

# Specify the .osm to open, assuming you run the script from the directory of hvac_library.osm
model_path = OpenStudio::Path.new('hvac_library.osm')

# Load the model, version translating if necessary
if OpenStudio::exists(model_path)
  versionTranslator = OpenStudio::OSVersion::VersionTranslator.new 
  model = versionTranslator.loadModel(model_path)
  if model.empty?
    puts "Version translation failed for #{model_path}"
    exit
  else
    model = model.get
  end
else
  puts "The model couldn't be found"
  exit
end

# Save updated model
model.save(model_path,true)

