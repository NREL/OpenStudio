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

class TestCreateWarningMsgs < OpenStudio::Ruleset::ModelUserScript

  #override name to return the name of your script
  def name
    return "Test - Create Warning Messages"
  end
  
  #return a vector of arguments
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    num_warning_msgs = OpenStudio::Ruleset::OSArgument::makeIntegerArgument("num_warning_msgs", false)
    num_warning_msgs.setDisplayName("Number of Warning Messages to Create")
    num_warning_msgs.setDefaultValue(3)
    result << num_warning_msgs
    
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
    num_warning_msgs = runner.getIntegerArgumentValue("num_warning_msgs",user_arguments)
    
    #check the inputs to ensure they make sense
    if num_warning_msgs < 1 or num_warning_msgs > 100
      runner.registerError("You must specify a number between 1 and 100")
      return false
    end
    
    #do whatever the measure does
    num_warning_msgs.times do |i|
      runner.registerWarning("warning message number #{i+1} of #{num_warning_msgs}")
    end
     
    return true  
  end
  
end

TestCreateWarningMsgs.new.registerWithApplication
