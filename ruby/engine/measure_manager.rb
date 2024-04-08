########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'json'
require 'erb'
require 'openstudio'

class MeasureInfoBinding
  def initialize(info, result_hash)
    @measure_info = info
    @measure_hash = result_hash
  end
  def measure_info
    @measure_info
  end
  def measure_hash
    @measure_hash
  end
  def error
    @measure_info.error.to_s
  end
  def measureType
    @measure_info.measureType.valueName.to_s
  end
  def className
    @measure_info.className.to_s
  end
  def name
    @measure_info.name.to_s
  end
  def description
    @measure_info.description.to_s
  end
  def taxonomy
    @measure_info.taxonomy.to_s
  end
  def modelerDescription
    @measure_info.modelerDescription.to_s
  end
  def arguments
    @measure_hash[:arguments]
  end
  def outputs
    @measure_hash[:outputs]
  end
  def get_binding
    result = binding()
    return result
  end
end

class MeasureManager

  attr_reader :osms, :measures, :measure_info

  def initialize(logger=nil)
    @logger = logger
    @osms = {} # osm_path => {:checksum, :model, :workspace}
    @idfs = {} # idf_path => {:checksum, :workspace}
    @measures = {} # measure_dir => BCLMeasure
    @measure_info = {} # measure_dir => {osm_path => RubyUserScriptInfo}

    eval(OpenStudio::Measure::infoExtractorRubyFunction)
  end

  def force_encoding(object, encoding = 'utf-8')
    if object.frozen?
      return object
    end

    type = object.class
    if type == Hash
      object.keys.each do |key|
        force_encoding(object[key], encoding)
      end
    elsif type == Array
      object.each do |val|
        force_encoding(val, encoding)
      end
    elsif type == String
      object.force_encoding(encoding)
    end

    return object
  end

  def print_message(message)
    if @logger
      @logger.debug(message)
    else
      puts message
    end
  end

  def reset
    @osms = {}
    @idfs = {}
    @measures = {}
    @measure_info = {}
  end

  # returns nil or [OpenStudio::Model::Model, OpenStudio::Workspace]
  # force_reload forces the model to be read from disk, should never be needed
  def get_model(osm_path, force_reload)

    # check if model exists on disk
    if !File.exist?(osm_path)
      print_message("Model '#{osm_path}' does not exist")
      @osms[osm_path] = nil
      @measure_info.each_value {|value| value[osm_path] = nil}
      force_reload = true
    end

    current_checksum = OpenStudio::checksum(OpenStudio::toPath(osm_path))

    result = nil
    if !force_reload
      # load from cache
      temp = @osms[osm_path]
      if temp
        last_checksum = temp[:checksum]
        if last_checksum && current_checksum == last_checksum
          model = temp[:model]
          workspace = temp[:workspace]
          if model && workspace
            result = [model, workspace]
            print_message("Using cached model '#{osm_path}'")
          end
        else
          print_message("Checksum of cached model does not match current checksum for '#{osm_path}'")
        end
      end
    end

    if !result
      # load from disk
      print_message("Attempting to load model '#{osm_path}'")
      vt = OpenStudio::OSVersion::VersionTranslator.new
      model = vt.loadModel(osm_path)

      if model.empty?
        print_message("Failed to load model '#{osm_path}'")
        @osms[osm_path] = nil
      else
        print_message("Successfully loaded model '#{osm_path}'")
        model = model.get
        ft = OpenStudio::EnergyPlus::ForwardTranslator.new
        workspace = ft.translateModel(model)
        @osms[osm_path] = {:checksum => current_checksum, :model => model, :workspace => workspace}
        result = [model, workspace]
      end

      @measure_info.each_value {|value| value[osm_path] = nil}
    end

    return result
  end

  # returns nil or OpenStudio::Workspace
  # force_reload forces the idf to be read from disk, should never be needed
  def get_idf(idf_path, force_reload)

    # check if model exists on disk
    if !File.exist?(idf_path)
      print_message("Idf '#{idf_path}' does not exist")
      @idfs[idf_path] = nil
      @measure_info.each_value {|value| value[idf_path] = nil}
      force_reload = true
    end

    current_checksum = OpenStudio::checksum(OpenStudio::toPath(idf_path))

    result = nil
    if !force_reload
      # load from cache
      temp = @osms[idf_path]
      if temp
        last_checksum = temp[:checksum]
        if last_checksum && current_checksum == last_checksum
          workspace = temp[:workspace]
          if workspace
            result = workspace
            print_message("Using cached workspace '#{idf_path}'")
          end
        else
          print_message("Checksum of cached workspace does not match current checksum for '#{idf_path}'")
        end
      end
    end

    if !result
      # load from disk
      print_message("Attempting to load idf '#{idf_path}'")
      workspace = OpenStudio::Workspace.load(idf_path, "EnergyPlus".to_IddFileType)

      if workspace.empty?
        print_message("Failed to load idf '#{idf_path}'")
        @idfs[idf_path] = nil
      else
        print_message("Successfully loaded idf '#{idf_path}'")
        workspace = workspace.get

        if !workspace.isValid('Draft'.to_StrictnessLevel)
          print_message("Workspace loaded from '#{idf_path}' is not valid")
          @idfs[idf_path] = nil
        else
          @idfs[idf_path] = {:checksum => current_checksum, :workspace => workspace}
          result = workspace
        end
      end

      @measure_info.each_value {|value| value[idf_path] = nil}
    end

    return result
  end

  # returns nil or OpenStudio::BCLMeasure from path
  # force_reload forces the measure.xml to be read from disk, should only be needed if user has edited the xml
  def get_measure(measure_dir, force_reload)

    # check if measure exists on disk
    if !File.exist?(measure_dir) || !File.exist?(File.join(measure_dir, 'measure.xml'))
      print_message("Measure '#{measure_dir}' does not exist")
      @measures[measure_dir] = nil
      @measure_info[measure_dir] = {}
      force_reload = true
    end

    result = nil
    if !force_reload
      # load from cache
      result = @measures[measure_dir]
      if result
        print_message("Using cached measure '#{measure_dir}'")
      end
    end

    if !result
      # load from disk
      print_message("Attempting to load measure '#{measure_dir}'")

      measure = OpenStudio::BCLMeasure.load(measure_dir)
      if measure.empty?
        print_message("Failed to load measure '#{measure_dir}'")
        @measures[measure_dir] = nil
      else
        print_message("Successfully loaded measure '#{measure_dir}'")
        result = measure.get
        @measures[measure_dir] = result
      end

      @measure_info[measure_dir] = {}
    end

    if result
      # see if there are updates, want to make sure to perform both checks so do outside of conditional
      file_updates = result.checkForUpdatesFiles # checks if any files have been updated
      xml_updates = result.checkForUpdatesXML # only checks if xml as loaded has been changed since last save

      readme_in_path = File.join(measure_dir, "README.md.erb")
      readme_out_path = File.join(measure_dir, "README.md")

      readme_out_of_date = false
      if File.exist?(readme_in_path) && !File.exist?(readme_out_path)
        readme_out_of_date = true
      end

      missing_fields = false
      begin
        missing_fields = result.missingRequiredFields
      rescue
      end

      if file_updates || xml_updates || missing_fields || readme_out_of_date
        print_message("Changes detected, updating '#{measure_dir}'")

        # clear cache before calling get_measure_info
        @measure_info[measure_dir] = {}

        # try to load the ruby measure
        info = get_measure_info(measure_dir, result, "", OpenStudio::Model::OptionalModel.new, OpenStudio::OptionalWorkspace.new)
        info.update(result)

        # update README.md.erb
        if File.exist?(readme_in_path)

          begin
            # delete README.md if it exists
            File.delete(readme_out_path) if File.exist?(readme_out_path)

            readme_in = nil
            File.open(readme_in_path, 'r') do |file|
              readme_in = file.read
            end

            result_hash = measure_hash(measure_dir, result, info)

            renderer = ERB.new(readme_in)
            result_binding = MeasureInfoBinding.new(info, result_hash)
            readme_out = renderer.result(result_binding.get_binding)

            # write README.me file
            File.open(readme_out_path, 'w') do |file|
              file << readme_out
              # make sure data is written to the disk one way or the other
              begin
                file.fsync
              rescue StandardError
                file.flush
              end
            end

            # update the files
            result.checkForUpdatesFiles

          rescue => e
            # update error in info
            info = OpenStudio::Measure::OSMeasureInfo.new(e.message)
            info.update(result)
          end

          # check for file updates
          file_updates = result.checkForUpdatesFiles
        end

        result.save
        @measures[measure_dir] = result
      end
    end

    return result
  end

  # returns OpenStudio::Measure::OSMeasureInfo
  def get_measure_info(measure_dir, measure, osm_path, model, workspace)

    result = nil

    # load from cache
    temp = @measure_info[measure_dir]
    if temp
      result = temp[osm_path]
      if result
        print_message("Using cached measure info for '#{measure_dir}', '#{osm_path}'")
      end
    end

    # try to load the ruby measure
    if !result

      # DLM: this is where we are executing user's arbitrary Ruby code
      # might need some timeouts or additional protection
      print_message("Loading measure info for '#{measure_dir}', '#{osm_path}'")
      begin
        result = OpenStudio::Measure.getInfo(measure, model, workspace)
      rescue Exception => e
        result = OpenStudio::Measure::OSMeasureInfo.new(e.message)
      end

      @measure_info[measure_dir] = {} if @measure_info[measure_dir].nil?
      @measure_info[measure_dir][osm_path] = result
    end

    return result
  end

  def get_arguments_from_measure(measure_dir, measure)
    result = []

    begin
      # this type was not wrapped with SWIG until OS 1.11.2
      measure.arguments.each do |argument|
        type = argument.type

        arg = {}
        arg[:name] = argument.name
        arg[:display_name] = argument.displayName
        arg[:description] = argument.description.to_s
        arg[:type] = argument.type
        arg[:required] = argument.required
        arg[:model_dependent] = argument.modelDependent

        case type
        when 'Boolean'
          if argument.defaultValue.is_initialized
            default_value = argument.defaultValue.get
            arg[:default_value] = (default_value.downcase == "true")
          end

        when 'Double'
          arg[:units] = argument.units.get if argument.units.is_initialized
          arg[:default_value] = argument.defaultValue.get.to_f if argument.defaultValue.is_initialized
          arg[:min_value] = argument.minValue.get.to_f if argument.minValue.is_initialized
          arg[:max_value] = argument.maxValue.get.to_f if argument.maxValue.is_initialized

        when 'Integer'
          arg[:units] = argument.units.get if argument.units.is_initialized
          arg[:default_value] = argument.defaultValue.get.to_i if argument.defaultValue.is_initialized
          arg[:min_value] = argument.minValue.get.to_i if argument.minValue.is_initialized
          arg[:max_value] = argument.maxValue.get.to_i if argument.maxValue.is_initialized

        when 'String'
          arg[:default_value] = argument.defaultValue.get if argument.defaultValue.is_initialized

        when 'Choice'
          arg[:default_value] = argument.defaultValue.get if argument.defaultValue.is_initialized
          arg[:choice_values] = []
          argument.choiceValues.each {|value| arg[:choice_values] << value}
          arg[:choice_display_names] = []
          argument.choiceDisplayNames.each {|value| arg[:choice_display_names] << value}

        when 'Path'
          arg[:default_value] = argument.defaultValue.get if argument.defaultValue.is_initialized
        end

        result << arg
      end
    rescue
      info = get_measure_info(measure_dir, measure, "", OpenStudio::Model::OptionalModel.new, OpenStudio::OptionalWorkspace.new)
      return get_arguments_from_measure_info(info)
    end

    return force_encoding(result, 'utf-8')
  end

  def get_arguments_from_measure_info(measure_info)
    result = []

    measure_info.arguments.each do |argument|
      type = argument.type

      arg = {}
      arg[:name] = argument.name
      arg[:display_name] = argument.displayName
      arg[:description] = argument.description.to_s
      arg[:type] = argument.type.valueName
      arg[:required] = argument.required
      arg[:model_dependent] = argument.modelDependent

      if type == "Boolean".to_OSArgumentType
        arg[:default_value] = argument.defaultValueAsBool if argument.hasDefaultValue

      elsif type == "Double".to_OSArgumentType
        arg[:units] = argument.units.get if argument.units.is_initialized
        arg[:default_value] = argument.defaultValueAsDouble if argument.hasDefaultValue
        if argument.hasDomain
          min, max = argument.domainAsDouble
          # I'm a bit wary of rounding issues... I think 1e308 instead of
          # Float::MAX (1.7976931348623157e+308) is fine for our applications...
          if min > -1e308 # -Float::MAX
            arg[:min_value] = min
          end
          if max < 1e308 # Float::MAX
            arg[:max_value] = max
          end
        end
      elsif type == "Quantity".to_OSArgumentType
        arg[:units] = argument.units.get if argument.units.is_initialized
        arg[:default_value] = argument.defaultValueAsQuantity.value if argument.hasDefaultValue

      elsif type == "Integer".to_OSArgumentType
        arg[:units] = argument.units.get if argument.units.is_initialized
        arg[:default_value] = argument.defaultValueAsInteger if argument.hasDefaultValue
        if argument.hasDomain
          min, max = argument.domainAsInteger
          if min != -2147483648
            arg[:min_value] = min
          end
          if max != 2147483648
            arg[:max_value] = max
          end
        end
      elsif type == "String".to_OSArgumentType
        arg[:default_value] = argument.defaultValueAsString if argument.hasDefaultValue

      elsif type == "Choice".to_OSArgumentType
        arg[:default_value] = argument.defaultValueAsString if argument.hasDefaultValue
        arg[:choice_values] = []
        argument.choiceValues.each {|value| arg[:choice_values] << value}
        arg[:choice_display_names] = []
        argument.choiceValueDisplayNames.each {|value| arg[:choice_display_names] << value}

      elsif type == "Path".to_OSArgumentType
        arg[:default_value] = argument.defaultValueAsPath.to_s if argument.hasDefaultValue

      end

      result << arg
    end

    return force_encoding(result, 'utf-8')
  end

  def measure_hash(measure_dir, measure, measure_info = nil)
    result = {}
    result[:measure_dir] = measure_dir
    result[:name] = measure.name
    result[:directory] = measure.directory.to_s
    if measure.error.is_initialized
      result[:error] = measure.error.get
    end
    result[:uid] = measure.uid
    result[:uuid] = measure.uuid.to_s
    result[:version_id] = measure.versionId
    result[:version_uuid] = measure.versionUUID.to_s
    version_modified = measure.versionModified
    if version_modified.is_initialized
      result[:version_modified] = version_modified.get.toISO8601
    else
      result[:version_modified] = nil
    end
    result[:xml_checksum] = measure.xmlChecksum
    result[:name] = measure.name
    result[:display_name] = measure.displayName
    result[:class_name] = measure.className
    result[:description] = measure.description
    result[:modeler_description] = measure.modelerDescription
    result[:tags] = []
    measure.tags.each {|tag| result[:tags] << tag}

    result[:outputs] = []
    begin
      # this is an OS 2.0 only method
      measure.outputs.each do |output|
        out = {}
        out[:name] = output.name
        out[:display_name] = output.displayName
        out[:short_name] = output.shortName.get if output.shortName.is_initialized
        out[:description] = output.description
        out[:type] = output.type
        out[:units] = output.units.get if output.units.is_initialized
        out[:model_dependent] = output.modelDependent
        result[:outputs] << out
      end
    rescue
    end

    attributes = []
    measure.attributes.each do |a|
      value_type = a.valueType
      if value_type == "Boolean".to_AttributeValueType
        attributes << {:name => a.name, :display_name => a.displayName(true).get, :value => a.valueAsBoolean}
      elsif value_type == "Double".to_AttributeValueType
        attributes << {:name => a.name, :display_name => a.displayName(true).get, :value => a.valueAsDouble}
      elsif value_type == "Integer".to_AttributeValueType
        attributes << {:name => a.name, :display_name => a.displayName(true).get, :value => a.valueAsInteger}
      elsif value_type == "Unsigned".to_AttributeValueType
        attributes << {:name => a.name, :display_name => a.displayName(true).get, :value => a.valueAsUnsigned}
      elsif value_type == "String".to_AttributeValueType
        attributes << {:name => a.name, :display_name => a.displayName(true).get, :value => a.valueAsString}
      end
    end
    result[:attributes] = attributes

    if measure_info
      result[:arguments] = get_arguments_from_measure_info(measure_info)
    else
      result[:arguments] = get_arguments_from_measure(measure_dir, measure)
    end

    return force_encoding(result, 'utf-8')
  end

end
