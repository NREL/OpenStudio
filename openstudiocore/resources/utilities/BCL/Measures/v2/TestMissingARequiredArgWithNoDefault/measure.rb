######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

class TestMissingARequiredArgWithNoDefault < OpenStudio::Ruleset::ModelUserScript

  #override name to return the name of your script
  def name
    return "Test - Missing a Required Argument with No Default"
  end
  
  #return a vector of arguments
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    required_argument = OpenStudio::Ruleset::OSArgument::makeIntegerArgument("required_argument", true)
    required_argument.setDisplayName("Required Argument")
    result << required_argument
    
    return result
  end

  #actually run the measure
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments)
    
    #make sure that all the required arguments have either a user input or a default filled in
    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end

    #get the user inputs out of user_arguments so we can use them
    required_argument = runner.getIntegerArgumentValue("required_argument",user_arguments)
    
    #check the inputs to ensure they make sense
 
    #do whatever the measure does

    return true
  end
  
end

TestMissingARequiredArgWithNoDefault.new.registerWithApplication
