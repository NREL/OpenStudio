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
#   This folder contains examples using the OpenStudio ruby bindings.
#
#   If you just installed OpenStudio, run 'TestInstall.rb' to check 
#   that the bindings are working properly. Instructions:
#   1. Open a command prompt.
#   2. Navigate to this folder.
#   3. Type 'ruby TestInstall.rb'.
#   4. If successful, a random IddObject will be printed to your
#      screen, followed by the message 'Test completed successfully.'
#   5. If unsuccessful, the best case is an informative error message.
#
#   To run the more complete test suite:
#   1. Open a command prompt.
#   2. Navigate to the install Ruby folder
#   3. Type 'ruby -I. openstudio_test.rb'.
#
#   Some examples require arguments, check the synopsis at the top of
#   each example to see what arguments and options are supported.
#
######################################################################