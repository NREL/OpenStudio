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

require 'openstudio'
require 'openstudio/energyplus/find_energyplus'

require 'test/unit'

class BasicRun_Test < Test::Unit::TestCase

  # def setup
  # end

  # def teardown
  # end
  
  def test_BasicRun
    idfPathAndFilename = OpenStudio::Path.new($OpenStudio_ResourcePath + "resultsviewer/SmallOffice/in.idf")  
    # find EnergyPlus
    ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
    weatherDir = OpenStudio::Path.new(ep_hash[:energyplus_weatherdata].to_s)
    weatherPathAndFilename = weatherDir / OpenStudio::Path.new("USA_IL_Chicago-OHare.Intl.AP.725300_TMY3.epw")
    outputDirectory = OpenStudio::Path.new
    launchResultsViewer = false
    assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/examples/BasicRun.rb' '#{idfPathAndFilename}' '#{weatherPathAndFilename}' '#{outputDirectory}' '#{launchResultsViewer}'"))
  end

end
