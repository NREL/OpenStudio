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

# Each user script is implemented within a class that derives from OpenStudio::Ruleset::UserScript
class SetupBCLKey < OpenStudio::Ruleset::UtilityUserScript

  # override name to return the name of your script
  def name
    return "Setup BCL Key"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments()
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    currentAuthKey = OpenStudio::LocalBCL::instance().prodAuthKey
    
    authKey = OpenStudio::Ruleset::OSArgument::makeStringArgument("authKey")
    authKey.setDisplayName("BCL AuthKey")
    authKey.setDefaultValue(currentAuthKey)
    result << authKey

    return result
  end

  # override run to implement the functionality of your script
  def run(runner, user_arguments)
    super(runner, user_arguments)
    
    if not runner.validateUserArguments(arguments,user_arguments)
      return false
    end
    
    authKey = runner.getStringArgumentValue("authKey",user_arguments)
    
    good_key = OpenStudio::LocalBCL::instance().setProdAuthKey(authKey)
    if not good_key 
      runner.registerError("BCL authentication failed, follow instructions at http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/#installation-instructions to get your auth key.")
      return false
    end
  end

end

# this call registers your script with the OpenStudio SketchUp plug-in
SetupBCLKey.new.registerWithApplication
