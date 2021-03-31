require 'openstudio'

module ModelFile

	# version-translates and loads a model
  def self.load_model(osm_path)
    model_path = OpenStudio::Path.new(osm_path)
    version_translator = OpenStudio::OSVersion::VersionTranslator.new
    if OpenStudio::exists(model_path)
      model = version_translator.loadModel(model_path)
      if model.empty?
        puts "Version translation of #{model_path} failed."
        exists
			else
				model = model.get
				return model
			end
		else 
			puts "Model could not be found at #{model_path}."
			exit
    end 
  end
  
  def self.load_model_NoTranslator(osm_path)
    model_path = OpenStudio::Path.new(osm_path)
    #version_translator = OpenStudio::OSVersion::VersionTranslator.new
    model1 = OpenStudio::Model::Model
    if OpenStudio::exists(model_path)
      model = model1.load(model_path)
      if model.empty?
        puts "Model Load of #{model_path} failed."
        exists
			else
				model = model.get
				return model
			end
		else 
			puts "Model could not be found at #{model_path}."
			exit
    end 
	end
  
  # returns a unique path by adding incrementing number on filename, 
	# for saving backups
	def self.unique_name(osm_path)
		count = 0
		unique_name = osm_path
		while File.exist?(unique_name)
			count +=1
			unique_name = "#{File.join(File.dirname(unique_name), File.basename(unique_name,".*"))}_#{count}#{File.extname(unique_name)}"
		end
		return unique_name
  end
  
end 


##################################################################################################################################
class OpenStudio::Model::Model
	# saves a model with the specified name to the specified directory
	def save_model(name,dir)
		osm_file = name #+ '.osm'
		osm_dir = dir
		osm_path = File.join(osm_dir, osm_file)
		puts "SAVING #{name} to #{osm_dir}"
		self.save(osm_path,true)
		return true
	end

end 

#####################################################################################################################################
class OpenStudio::Model::ModelObject
  # from Julien Marrec
  # https://unmethours.com/question/17616/get-thermal-zone-supply-terminal/
  # Extend ModelObject class to add a to_actual_object method
  # Casts a ModelObject into what it actually is (OS:Node for example...)
  def to_actual_object
    # get the name of the idd type of the ModelObject 
    obj_type = self.iddObjectType.valueName
    # remove prefix and underscores
    obj_type_name = obj_type.gsub('OS_','').gsub('_','')
    # create method string 
    method_name = "to_#{obj_type_name}"
    # if call method string on ModelObject and return the resulting object
    if self.respond_to?(method_name) # https://ruby-doc.org/core-2.6.2/Object.html#method-i-respond_to-3F
      actual_thing = self.method(method_name).call
      if !actual_thing.empty?
          return actual_thing.get
      end
    end
    return false
  end
  
end
