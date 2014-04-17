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

require 'test/unit'

require 'fileutils'
require 'openstudio/energyplus/find_energyplus'


class DaylightCalculations_Test < Test::Unit::TestCase
  
  #def setup
  #  
  #end
  
  #def teardown
  #  #
  #end
  
  def test_DaylightCalculations
  
    co = OpenStudio::Runmanager::ConfigOptions.new(true)
    co.fastFindRadiance();
    loc = co.getTools().getAllByName("radee").tools()
    
    if (loc.empty?)
      puts "Radiance not found, test_DaylightCalculations cannot proceed"
      return
    end
    
    radianceLocation = loc[0].localBinPath.parent_path
  
    # set up model
    modelPath = OpenStudio::Path.new("#{$OpenStudio_ResourcePath}radiance/test/ExampleModel.osm")
    if (File.exists?(modelPath.parent_path.to_s))
      FileUtils.rm_rf(modelPath.parent_path.to_s)
    end
    FileUtils.mkdir_p(modelPath.parent_path.to_s)
    assert(File.exists?(modelPath.parent_path.to_s))
    
    modelExample = OpenStudio::Model::exampleModel()
    ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,0)
    epwPath = OpenStudio::Path.new(ep_hash[:energyplus_weatherdata].to_s) / OpenStudio::Path.new("USA_CO_Golden-NREL.724666_TMY3.epw")
    epwFile = OpenStudio::EpwFile.new(epwPath)
    weatherFile = OpenStudio::Model::WeatherFile::setWeatherFile(modelExample, epwFile)
    assert((not weatherFile.empty?))
    modelExample.toIdfFile.save(modelPath, true)
    
    # run DaylightCalculations
    command = "#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/radiance/DaylightCalculations.rb' '#{modelPath}' '#{radianceLocation}'"
    puts command
    assert(system(command))
  
  end

  
end

