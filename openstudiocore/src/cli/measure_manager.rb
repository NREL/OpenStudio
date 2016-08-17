######################################################################
#  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

require 'json'
require 'openstudio'

class MeasureManager
  
  attr_reader :osms, :measures, :measure_info
  
  def initialize()
    @osms = {} # osm_path => {:checksum, :model, :workspace}
    @measures = {} # measure_dir => BCLMeasure
    @measure_info = {} # measure_dir => {osm_path => RubyUserScriptInfo}
    
    eval(OpenStudio::Ruleset::infoExtractorRubyFunction)
  end

  def print_message(message)
    puts message
  end
  
  def reset
    @osms = {}
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

    result = nil
    if !force_reload
      # load from cache
      temp = @osms[osm_path]
      if temp
        current_checksum = OpenStudio::checksum(OpenStudio::toPath(osm_path))
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
      
      missing_fields = false
      begin
        missing_fields = result.missingRequiredFields
      rescue
      end
      
      if file_updates || xml_updates || missing_fields
        print_message("Changes detected, updating '#{measure_dir}'")

        # clear cache before calling get_measure_info
        @measure_info[measure_dir] = {}
        
        # try to load the ruby measure
        info = get_measure_info(measure_dir, result, "", OpenStudio::Model::OptionalModel.new, OpenStudio::OptionalWorkspace.new)
        info.update(result)

        result.save
        @measures[measure_dir] = result
      end
    end
    
    return result
  end
  
  # returns OpenStudio::Ruleset::RubyUserScriptInfo 
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
        result = OpenStudio::Ruleset.getInfo(measure, model, workspace)
      rescue Exception => e  
        result = OpenStudio::Ruleset::RubyUserScriptInfo.new(e.message)
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
    
    return result
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
        
      elsif type == "Quantity".to_OSArgumentType
        arg[:units] = argument.units.get if argument.units.is_initialized
        arg[:default_value] = argument.defaultValueAsQuantity.value if argument.hasDefaultValue
        
      elsif type == "Integer".to_OSArgumentType
        arg[:units] = argument.units.get if argument.units.is_initialized
        arg[:default_value] = argument.defaultValueAsInteger if argument.hasDefaultValue
        
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
    
    return result
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
    
    return result
  end
  
end
