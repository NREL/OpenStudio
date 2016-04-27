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

# todo - make a copy of this that runs off of local directory.

# Each user script is implemented within a class that derives from OpenStudio::Ruleset::UserScript
class RunOpenStudioMeasure < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Run OpenStudio Local BCL Measure"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    #make choice argument for facade
    choices = OpenStudio::StringVector.new
    # Search for components
    library = OpenStudio::LocalBCL::instance()
    library.measures.each do |measure|
      next if not measure.measureType.value == 0 # next if not model measure
      choices << measure.displayName
    end
    local_bcl_measure = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("local_bcl_measure", choices,true)
    local_bcl_measure.setDisplayName("Choose a Measure From Your Local BCL Library to Run.")
    result << local_bcl_measure

    begin
      SKETCHUP_CONSOLE.show
    rescue => e
    end

    return result
  end
    
  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)      
    super(model, runner, user_arguments)

    # get argument
    local_bcl_measure = runner.getStringArgumentValue("local_bcl_measure",user_arguments)

    # todo - if I passed the measure vs. the name in argument I wouldn't have to find it again
    library = OpenStudio::LocalBCL::instance()
    library.measures.each do |measure|
      next if not measure.displayName == local_bcl_measure
      # put description and modeler description ton console
      puts "Running #{measure.displayName} - #{measure.taxonomyTag}"
      puts ""
      puts "Description: #{measure.description}"
      puts ""
      puts "Modeler Description: #{measure.modelerDescription}"
      puts ""
      puts "Measure Class: #{measure.className}"
      puts ""

      # path to measure for local command line run
      measure_dir = measure.directory.to_s
      #puts measure_dir
      require (measure_dir + "/measure.rb")

      # get class name
      # todo - find stable way to do this with all measures
      class_name = measure.className
      if not class_name == ''
        # create an instance of the measure
        measure = eval(class_name).new
      else

        # todo - try to guess class from name for now (rescue if cant't make instance)
        class_name = measure.name.split('_').map{|e| e.capitalize}.join
        # create an instance of the measure
        measure = eval(class_name).new

        # some measures I download from BCL seems to hit this (missin in measure.xml), try and find another approach
        #puts "can't identify class of #{local_bcl_measure}"
        #return false
      end

      # get arguments
      arguments = measure.arguments(model)
      argument_map = OpenStudio::Ruleset.convertOSArgumentVectorToMap(arguments)

      # populate argument with specified hash value if specified
      args_hash = {}
      arguments.each do |arg|
        temp_arg_var = arg.clone
        puts arg
        puts "arg type is #{arg.type.value}"

        arg_type = arg.type.value
        if arg_type == 0 # Bool
          arguments2 = OpenStudio::Ruleset::OSArgumentVector.new
          arg_val = OpenStudio::Ruleset::OSArgument::makeBoolArgument("arg_val")
          arg_val.setDisplayName(arg.name)
          arg_val.setDefaultValue(arg.defaultValueAsBool)
          arguments2 << arg_val
        elsif arg_type == 1 # Double
          arguments2 = OpenStudio::Ruleset::OSArgumentVector.new
          arg_val = OpenStudio::Ruleset::OSArgument::makeDoubleArgument("arg_val")
          arg_val.setDisplayName(arg.name)
          arg_val.setDefaultValue(arg.defaultValueAsDouble)
          arguments2 << arg_val
        elsif arg_type == 2 # Quantity (not used)

        elsif arg_type == 3 # Int
          arguments2 = OpenStudio::Ruleset::OSArgumentVector.new
          arg_val = OpenStudio::Ruleset::OSArgument::makeIntegerArgument("arg_val")
          arg_val.setDisplayName(arg.name)
          arg_val.setDefaultValue(arg.defaultValueAsInteger)
          arguments2 << arg_val
        elsif arg_type == 4 # String
          arguments2 = OpenStudio::Ruleset::OSArgumentVector.new
          arg_val = OpenStudio::Ruleset::OSArgument::makeStringArgument("arg_val")
          arg_val.setDisplayName(arg.name)
          arg_val.setDefaultValue(arg.defaultValueAsString)
          arguments2 << arg_val
        elsif arg_type == 5 # Choice
          arguments2 = OpenStudio::Ruleset::OSArgumentVector.new
          arg_val = OpenStudio::Ruleset::OSArgument::makeChoiceArgument("arg_val")
          arg_val.setDisplayName(arg.name)
          arg_val.setDefaultValue(arg.defaultValueAsString)
          arguments2 << arg_val
        elsif arg_type == 6 # Path (not used)

        end

        # todo - consolodate to a single input dialog with multiple arguments
        # prompt user
        input = runner.getUserInput(arguments2)
        puts input
        input_value = input["arg_val"]
        if not input_value
          # user canceled
          puts "user canceled"
          return false
        else

          if arg_type == 0 # Bool
            temp_arg_var.setValue(input_value.valueAsBool)
          elsif arg_type == 1 # Double
            temp_arg_var.setValue(input_value.valueAsDouble)
          elsif arg_type == 2 # Quantity (not used)

          elsif arg_type == 3 # Int
            temp_arg_var.setValue(input_value.valueAsInteger)
          elsif arg_type == 4 # String
            temp_arg_var.setValue(input_value.valueAsString)
          elsif arg_type == 5 # Choice
            temp_arg_var.setValue(input_value.valueAsString)
          elsif arg_type == 6 # Path (not used)

          end

        end

        argument_map[arg.name] = temp_arg_var
      end

      # run the measure
      begin

        # run the measure
        puts "running the measure"
        runner_child = OpenStudio::Ruleset::OSRunner.new
        measure.run(model, runner_child, argument_map)

        # put log messages to console
        require 'openstudio/ruleset/ShowRunnerOutput'
        result_child = runner_child.result
        show_output(result_child)

        # convert a return false in the measure to a return false and error here.
        if result_child.value.valueName == "Fail"
          runner.registerError("The measure was not successful")
          return false
        end

      rescue => e
        runner.registerError("Measure Failed with Error: #{e.backtrace.join("\n")}")
        return false
      end

      next # stop once found and ran a measure.
    end
    
    return true    
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
RunOpenStudioMeasure.new.registerWithApplication
