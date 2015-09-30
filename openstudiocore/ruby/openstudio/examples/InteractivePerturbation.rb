######################################################################
#  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

######################################################################
# == Synopsis 
#
#   Opens an idf or osm model, then interacts with the user to define 
#   a continuous variable perturbation study, and saves the resulting
#   models.
#
# == Usage
#
#   ruby ContinuousVariablePerturbation.rb ARGV[0] 
#
#   ARGV[0] - Path to .idf or .osm
#   ARGV[1] - (optional) Path to folder for models
#
# == Examples
#
#   ruby InteractivePerturbation.rb resources/analysis/models/ASHRAE140-Case640.idf resources/analysis/models/ASHRAE140-Case640Perturbations/
#
######################################################################

require 'openstudio'
require 'openstudio/model/ModelHelpers.rb'

# open the file, converting to Model format if necessary
model_path = OpenStudio::Path.new(ARGV[0])
ext = getFileExtension(model_path)
model = nil
if (ext == "idf") 
  model = OpenStudio::EnergyPlus::loadAndTranslateIdf(model_path)  
else
  model = OpenStudio::Model::Model::load(model_path)
end
raise "Unable to load " + model_path.to_s + "." if model.empty?
model = model.get

# get an IddObjectType from the user
puts "Enter in the IddObjectType you would like to run a perturbation study on, type 'list' " +
     "to see a list of available types, or type 'exit' to exit."
input = String.new
types = OpenStudio::IddObjectTypeSet.new
while input.empty?
  input = STDIN.gets; input.chomp!
  Process.exit if (input.casecmp("exit") == 0)
  if input.casecmp("list") == 0
    if types.empty?
      wsObjects = model.objects(true)    
      wsObjects.each{ |obj| types.insert(obj.iddObject().type()) }
    end  
    puts "The IddObjectTypes available in your Model are:"
    types.each{ |tp| puts "  " + tp.valueName() }
    input = String.new
    puts "Please enter a type from the list, or type 'exit' to exit."
  else
    begin
      type = input.to_IddObjectType
    rescue
      input = String.new
      puts "I did not understand your IddObjectType entry. Please try again, type 'list' " + 
           "to see a list of available types, or type 'exit' to exit."
    end
  end
end

# interactively identify a particular object, if necessary
wsObjects = model.getObjectsByType(type)
name = String.new
if wsObjects.size() > 1
  puts "Enter the name of a particular object of type '" + type.valueName() + "' that you would " +
       "like to perturb, type 'list' to see the names of the relevant objects, or type 'exit' to exit."
  found = false
  names = Array.new
  while not found
    input = STDIN.gets; input.chomp!
    Process.exit if (input.casecmp("exit") == 0)
    
    if input.casecmp("list") == 0
      if names.empty?
        wsObjects.each{ |obj|
          oName = obj.name()
          names.push(oName.get()) if oName
        }
      end
      puts "The names of the " + wsObjects.size().to_s + " objects in your model of type '" + 
           type.valueName() + "' are:"
      names.each{ |name| puts "  " + name }
      puts "Please enter a name from the list, or type 'exit' to exit."
    else
      name = input
      oo = model.getObjectByTypeAndName(type,name)
      if oo 
        object = oo.get()
        found = true
      else
        puts "I was not able to retrieve an object from your model based on the type '" + 
             type.valueName() + "' and name '" + input + "'. Please enter another name, type " +
             "'list' to see the names of the relevant objects, or type 'exit' to exit."
      end
    end
  end
else
  object = wsObjects[0]
  oName = object.name()
  (name = oName.get()) if oName
end

# interactively identify a particular field
indices = Array.new
names = Array.new
# determine ok indices
for i in 0..(object.numFields()-1)
  oIddField = object.iddObject().getField(i)
  if oIddField
    iddField = oIddField.get()
    if iddField.properties().type == "Real".to_IddFieldType
      indices.push(i)
      names.push(iddField.name())
    end
  end
end

index = -1
puts "Please enter the index of the field you would like to perturb, type 'list' to see the " +
     "indices and descriptors of perturbable fields, or type 'exit' to exit."
while index == -1
  input = STDIN.gets; input.chomp!
  Process.exit if (input.casecmp("exit") == 0)
  
  if input.casecmp("list") == 0
    puts "The following fields of the '" + type.valueName() + "' object named '" + name + 
         "' are perturbable: "
    for i in 0..(indices.size()-1)
      puts "  " + indices[i].to_s + " - " + names[i]
    end
    puts "Please enter the field you would like to perturb, or type 'exit' to exit."
  else
    begin
      index = Integer(input)
      ok = indices.include?(index)
      if not ok
        puts "Entered field index '" + index.to_s + "' is invalid or belongs to a field not of " + 
             "type Real. Please enter the index of the field you would like to perturb, type " +
             "'list' to see the indices and descriptors of perturbable fields, or type 'exit' to exit."       
        index = -1
      end
    rescue
      puts "Expected an integer, 'list', or 'exit', but got '" + input + "'. Please try again."
      index = -1
    end
  end
end

# get bounds
puts "Please enter the lower bound for your perturbation study, type 'current' to see the current " +
     "value of the variable and the IddField information, or type 'exit' to exit."
minFound = false
maxFound = false
iddField = object.iddObject().getField(index).get
value = object.getString(index).get
while not (minFound and maxFound)
  input = STDIN.gets; input.chomp!
  Process.exit if (input.casecmp("exit") == 0)
  
  if input.casecmp("current") == 0
    puts "The current value of the field is '" + value + "'. The IddField information is: "
    puts iddField.to_s
    if not minFound
      puts "Please enter the lower bound for your perturbation study, or type 'exit' to exit."
    else
      puts "Please enter the upper bound for your perturbation study, or type 'exit' to exit."
    end
  else
    begin
      num = Float(input)
      if not minFound
        minValue = num
        minFound = true
        puts "Please enter the upper bound for your perturbation study, type 'current' to see " +
             "the current value of the variable and the IddField information, or type 'exit' to " +
             "exit."
      else 
        maxValue = num
        maxFound = true
      end
    rescue
      puts "I did not understand your input, '" + input + "'. Please try again."      
    end      
  end
end
puts "Thank you. Your study will use the bounds [" + minValue.to_s + "," + maxValue.to_s + "]."

# get number of steps
nSteps = -1
puts "Please enter an integer number of steps for your perturbation study, or type 'exit' to exit." +
     "Note that the resulting study will consist of nSteps + 1 simulations with the variable set " + 
     "to the values [minValue:(maxValue - minValue)/nSteps:maxValue]."
while nSteps == -1
  input = STDIN.gets; input.chomp!
  Process.exit if (input.casecmp("exit") == 0)
  
  begin
    nSteps = Integer(input)
    nSteps = -1 if (nSteps <= 0)     
  rescue
    nSteps = -1
  end
  if (nSteps == -1)
    puts "I did not understand your input of '" + input + "'. Please enter an integer greater than 0."
  end
end

# create the Perturbation Models
accessor = OpenStudio::Analysis::FieldVariableAccessor.new(object.handle(),index,OpenStudio::Model::OptionalModel.new(model))
variable = OpenStudio::Analysis::ContinuousVariable.new(accessor,minValue,maxValue,nSteps)
models = variable.getPerturbations(model)

# save the Models
if ARGV.size() > 1
  study_path = OpenStudio::Path.new(ARGV[1])
else
  study_path = model_path.parent_path()
  study_path = study_path / OpenStudio::Path.new(model.getBuilding.name.get)
end
if not File.directory?(study_path.to_s)
  Dir.mkdir(study_path.to_s)
end
models.each{ |studyModel| 
  p = study_path / OpenStudio::Path.new(studyModel.getBuilding.name.get + "-" + 
      variable.getName(studyModel) + "-" + variable.getValue(studyModel).get.to_s + ".osm")
  studyModel.toIdfFile.save(p,true)
}

# run the Models


# view results

