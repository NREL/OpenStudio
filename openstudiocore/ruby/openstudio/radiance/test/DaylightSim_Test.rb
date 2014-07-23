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

require 'minitest/autorun'

require 'fileutils'
require 'openstudio/energyplus/find_energyplus'



class DaylightSim_Test < MiniTest::Unit::TestCase
  
  def setup
 
  end

  def test_DaylightSim_genannual
  
    #set up directories
    outdir = OpenStudio::tempDir() / OpenStudio::Path.new("DaylightSim_test_genannual")
    if File.exists?(outdir.to_s)
      FileUtils.rm_rf(outdir.to_s)
    end
    assert((not File.exists?(outdir.to_s)))
    
    FileUtils.mkdir_p(outdir.to_s)
    assert(File.exists?(outdir.to_s))
    
    #create example model, attach epw file
    modelExample = OpenStudio::Model::exampleModel()
    modelFile = outdir / OpenStudio::Path.new("in.osm") 
    ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
    epwPath = OpenStudio::Path.new(ep_hash[:energyplus_weatherdata].to_s) / OpenStudio::Path.new("USA_CO_Golden-NREL.724666_TMY3.epw")
    epwFile = OpenStudio::EpwFile.new(epwPath)
    weatherFile = OpenStudio::Model::WeatherFile::setWeatherFile(modelExample, epwFile)
    assert((not weatherFile.empty?))
    modelExample.save(modelFile, true)

    #setup path to Radiance binaries
    co = OpenStudio::Runmanager::ConfigOptions.new(true);
    co.fastFindRadiance();
    radiancePath = co.getTools().getLastByName("rad").localBinPath.parent_path
    
    Dir.chdir(outdir.to_s)
    	
	  #run DaylightCalculations.rb (executes end-to-end Radiance annual daylight simulation)
    assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/radiance/DaylightCalculations.rb' '#{modelFile}' '#{radiancePath}' "))
  
  end
  
  def teardown

  end
  
end

