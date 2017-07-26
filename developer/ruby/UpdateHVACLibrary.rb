# make sure that correct version of openstudio is loaded before running this, run from cli like:
#
# openstudio.exe UpdateHVACLibrary.rb

require 'openstudio'

path = File.join(File.dirname(__FILE__), '../../openstudiocore/**/*.osm')

Dir.glob(path) do |model_path|

  if /sketchup_plugin\/resources\/templates/.match(model_path)
    # do this 
  elsif /openstudio_app\/Resources/.match(model_path)
    # do this 
  else
    next
  end
    
  model_path = OpenStudio::Path.new(model_path)
  #model_path = OpenStudio::Path.new('hvac_library.osm')

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
end
