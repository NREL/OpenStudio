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

######################################################################
# == Synopsis 
#
#   Tests the OpenStudio ruby bindings by using them to find and 
#   print a random IddObject.
#
# == Usage
#
#   ruby TestInsall.rb
#
# == Examples
#
#   ruby TestInsall.rb
#
######################################################################

require 'openstudio'

factory = OpenStudio::IddFileAndFactoryWrapper.new("WholeFactory".to_IddFileType)

# First types listed in IddObjectType are 'Catchall' and 'UserCustom', which are
# not available through the factory.
numberToIgnore = 2

# Get the enum values available for IddObjectType
valuesSet = OpenStudio::IddObjectType::getValues()
iddObjectTypeValues = valuesSet.to_a

# Randomly choose one of the types (excluding the first that are to be ignored)
n = rand(iddObjectTypeValues.size() - numberToIgnore)

# Check index and get type
index = n + numberToIgnore
raise "Bad index for iddObjectTypeValues." if (index >= iddObjectTypeValues.size())
objectType = OpenStudio::IddObjectType.new(iddObjectTypeValues[index])

# Get object
oo = factory.getObject(objectType)
raise "Cannot get IddObject of type " + objectType.to_s + " from IddFactory." if oo.empty?
object = oo.get

# Report result
puts "Random IddObject from the IddFactory:"
puts " "
puts object.to_s
puts "Test completed successfully."
