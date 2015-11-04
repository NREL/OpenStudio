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

require 'openstudio'

require 'minitest/autorun'

require 'fileutils'

class ModelToRad_Test < MiniTest::Unit::TestCase
  
  #def setup
  
  #end

  def test_ModelToRad
    modelExample = OpenStudio::Model::exampleModel()
    modelPath = OpenStudio::Path.new("#{$OpenStudio_ResourcePath}radiance/test/ExampleModel.osm")
    FileUtils.mkdir_p(modelPath.parent_path.to_s)
    
    epwPath = OpenStudio::Path.new(OpenStudio::Path.new("#{$OpenStudio_ResourcePath}runmanager/USA_CO_Golden-NREL.724666_TMY3.epw"))
    epwFile = OpenStudio::EpwFile.new(epwPath)
    weatherFile = OpenStudio::Model::WeatherFile::setWeatherFile(modelExample, epwFile)
    assert((not weatherFile.empty?))
    
    modelExample.toIdfFile.save(modelPath, true)
    
    #run ModelToRad.rb
    command = "#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/radiance/ModelToRad.rb' '#{modelPath}' '--v'"
    puts command
    assert(system(command))
    #FileUtils.rm_rf("#{$OpenStudio_ResourcePath}radiance/test/")
  end
  
  def test_ModelToRad_Blinds
    modelExample = OpenStudio::Model::exampleModel()
    
    blind = OpenStudio::Model::Blind.new(modelExample)
    shadingControl = OpenStudio::Model::ShadingControl.new(blind)
    modelExample.getSubSurfaces.each do |subSurface|
      subSurface.setShadingControl(shadingControl)
    end
    
    modelPath = OpenStudio::Path.new("#{$OpenStudio_ResourcePath}radiance/test/ExampleModel_Blinds.osm")
    FileUtils.mkdir_p(modelPath.parent_path.to_s)
    
    epwPath = OpenStudio::Path.new(OpenStudio::Path.new("#{$OpenStudio_ResourcePath}runmanager/USA_CO_Golden-NREL.724666_TMY3.epw"))
    epwFile = OpenStudio::EpwFile.new(epwPath)
    weatherFile = OpenStudio::Model::WeatherFile::setWeatherFile(modelExample, epwFile)
    assert((not weatherFile.empty?))
    
    modelExample.toIdfFile.save(modelPath, true)
    #run ModelToRad.rb
    command = "#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/radiance/ModelToRad.rb' '#{modelPath}' '--v'"
    puts command
    assert(system(command))
    #FileUtils.rm_rf("#{$OpenStudio_ResourcePath}radiance/test/")
  end

  def test_ModelToRad_FrameAndDivider
    modelExample = OpenStudio::Model::exampleModel()
    
    frameAndDivider = OpenStudio::Model::WindowPropertyFrameAndDivider.new(modelExample)
    frameAndDivider.setOutsideRevealDepth(0.15)
    frameAndDivider.setInsideRevealDepth(0.15)
    frameAndDivider.setInsideSillDepth(0.30)
    modelExample.getSubSurfaces.each do |subSurface|
      subSurface.setWindowPropertyFrameAndDivider(frameAndDivider)
    end
    
    modelPath = OpenStudio::Path.new("#{$OpenStudio_ResourcePath}radiance/test/ExampleModel_FrameAndDivider.osm")
    FileUtils.mkdir_p(modelPath.parent_path.to_s)
    
    epwPath = OpenStudio::Path.new(OpenStudio::Path.new("#{$OpenStudio_ResourcePath}runmanager/USA_CO_Golden-NREL.724666_TMY3.epw"))
    epwFile = OpenStudio::EpwFile.new(epwPath)
    weatherFile = OpenStudio::Model::WeatherFile::setWeatherFile(modelExample, epwFile)
    assert((not weatherFile.empty?))
    
    modelExample.toIdfFile.save(modelPath, true)
    #run ModelToRad.rb
    command = "#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/radiance/ModelToRad.rb' '#{modelPath}' '--v'"
    puts command
    assert(system(command))
    #FileUtils.rm_rf("#{$OpenStudio_ResourcePath}radiance/test/")
  end
  
#  def teardown
#    FileUtils.rm_rf("#{$OpenStudio_ResourcePath}radiance/test/")
#  end
  
 
end

