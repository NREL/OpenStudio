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


class DaylightSim_Test < Test::Unit::TestCase
  
  def setup
    
  end

  def test_DaylightSim_genannual
    modelExample = OpenStudio::Model::exampleModel()
    modelPath = OpenStudio::Path.new("#{$OpenStudio_ResourcePath}radiance/test/ExampleModel.osm")
    FileUtils.mkdir_p(modelPath.parent_path.to_s)
    ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
    epwPath = OpenStudio::Path.new(ep_hash[:energyplus_weatherdata].to_s) / OpenStudio::Path.new("USA_CO_Golden-NREL.724666_TMY3.epw")
    epwFile = OpenStudio::EpwFile.new(epwPath)
    weatherFile = OpenStudio::Model::WeatherFile::setWeatherFile(modelExample, epwFile)
    assert((not weatherFile.empty?))
    modelExample.toIdfFile.save(modelPath, true)
    #run ModelToRad.rb
    command = "#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/radiance/ModelToRad.rb' '#{modelPath}'"
    puts command
    assert(system(command))
  
    #modelPath = OpenStudio::Path.new("#{$OpenStudio_ResourcePath}radiance/test/ExampleModel.osm")
    sqlPath = OpenStudio::Path.new("#{$OpenStudio_ResourcePath}radiance/test/ExampleModel/model/radiance/gen_eplus/ModelToRadPreProcess/ModelToIdf-0/ExpandObjects-0/EnergyPlus-0/eplusout.sql") 
    #generate daylight coefficients
    assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/radiance/DaylightSim.rb' '#{modelPath}' '#{sqlPath}' --dc"))
    #do annual sim
    assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/radiance/DaylightSim.rb' '#{modelPath}' '#{sqlPath}' --dcts --month 1 --day 21"))
  end
  
  def test_DaylightSim_help
    #print help message
    assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/radiance/DaylightSim.rb' --help"))
  end
  
  def teardown
    #FileUtils.rm_rf("#{$OpenStudio_ResourcePath}radiance/test/")
  end
  
end

