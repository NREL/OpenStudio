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
require 'fileutils'

require 'test/unit'

class RunWorkItems_Test < Test::Unit::TestCase

  # def setup
  # end

  # def teardown
  # end

  def test_RunWorkItems
    # create folder for this test
    puts "Creating test folder."
    test_dir = OpenStudio::Path.new($OpenStudio_ResourcePath + "runmanager/RunWorkItems_Test")
    if File.exists? test_dir.to_s
      FileUtils.rm_rf(test_dir.to_s)
    end
    FileUtils.mkdir(test_dir.to_s)

    # create model 
    puts "Creating example model."
    model = OpenStudio::Model::exampleModel
    model_path = test_dir / OpenStudio::Path.new("example.osm")
    model.save(model_path,true)
    model_dir = test_dir / OpenStudio::Path.new("example")
    FileUtils.mkdir(model_dir.to_s)
    
    # copy weather file over to model_dir
    puts "Adding weather file and design days."
    ep_hash = OpenStudio::EnergyPlus::find_energyplus(8,1)
    weather_dir = OpenStudio::Path.new(ep_hash[:energyplus_weatherdata].to_s)
    weather_path = weather_dir / OpenStudio::Path.new("USA_CO_Golden-NREL.724666_TMY3.epw")
    ddy_path = weather_dir / OpenStudio::Path.new("USA_CO_Golden-NREL.724666_TMY3.ddy")
    files_dir = model_dir / OpenStudio::Path.new("files")
    FileUtils.mkdir(files_dir.to_s)
    FileUtils.cp(weather_path.to_s, 
                 (files_dir / OpenStudio::Path.new(weather_path.filename)).to_s)
    weather_path = files_dir / OpenStudio::Path.new(weather_path.filename)

    # add climate data to model
    epw_file = OpenStudio::EpwFile.new(weather_path)
    OpenStudio::Model::WeatherFile::setWeatherFile(model,epw_file)
    ddy_model = OpenStudio::EnergyPlus::loadAndTranslateIdf(ddy_path)
    raise "Unable to load ddy file." if ddy_model.empty?
    ddy_model = ddy_model.get
    model.insertObjects(ddy_model.objects)
    model.save(model_path,true)

    # create vector of work items
    puts "Creating vector of work items."
    work_items = OpenStudio::Runmanager::WorkItemVector.new
    scripts_dir = model_dir / OpenStudio::Path.new("scripts")
    FileUtils.mkdir(scripts_dir.to_s)

    # OpenStudio Measure 1: Rotate the Building
    # get and place a copy in scripts folder
    puts "Adding rotate building measure."
    rotate_bldg_measure = OpenStudio::getMeasure("a5be6c96-4ecc-47fa-8d32-f4216ebc2e7d")
    raise "Unable to retrieve rotate building measure from BCL." if rotate_bldg_measure.empty?
    rotate_bldg_measure = rotate_bldg_measure.get
    rotate_bldg_measure = rotate_bldg_measure.clone(scripts_dir / OpenStudio::Path.new(rotate_bldg_measure.name))
    raise "Unable to clone rotate building measure into scripts directory." if rotate_bldg_measure.empty?
    rotate_bldg_measure = rotate_bldg_measure.get
    
    # set arguments and add to work_items
    args = OpenStudio::Ruleset::getArguments(rotate_bldg_measure,model)
    arg_map = OpenStudio::Ruleset::convertOSArgumentVectorToMap(args)
    arg_map["relative_building_rotation"].setValue(45.0)    
    ruby_job = OpenStudio::Runmanager::RubyJobBuilder.new(rotate_bldg_measure,arg_map)
    ruby_job.setIncludeDir(OpenStudio::Path.new("#{$OpenStudio_Dir}"))
    work_items << ruby_job.toWorkItem

    # get measure definition for next four measure instances and place a copy in scripts folder
    puts "Adding window to wall ratio measure."
    wwr_measure = OpenStudio::getMeasure("f347ae80-48b4-4c40-bfd4-6c5139b38136")
    raise "Unable to retrieve window-to-wall ratio measure from BCL." if wwr_measure.empty?
    wwr_measure = wwr_measure.get
    wwr_measure = wwr_measure.clone(scripts_dir / OpenStudio::Path.new(wwr_measure.name))
    raise "Unable to clone window-to-wall ratio measure into scripts directory." if wwr_measure.empty?    
    wwr_measure = wwr_measure.get
    args = OpenStudio::Ruleset::getArguments(wwr_measure,model)
    arg_map = OpenStudio::Ruleset::convertOSArgumentVectorToMap(args)

    # OpenStudio Measure 2: Set the South WWR
    arg_map["wwr"].setValue(0.4)
    arg_map["sillHeight"].setValue(30.0)
    arg_map["facade"].setValue("South")
    ruby_job = OpenStudio::Runmanager::RubyJobBuilder.new(wwr_measure,arg_map)
    ruby_job.setIncludeDir(OpenStudio::Path.new("#{$OpenStudio_Dir}"))
    work_items << ruby_job.toWorkItem

    # OpenStudio Measure 3: Set the North WWR
    arg_map["wwr"].setValue(0.2)
    arg_map["sillHeight"].setValue(60.0)
    arg_map["facade"].setValue("North")
    ruby_job = OpenStudio::Runmanager::RubyJobBuilder.new(wwr_measure,arg_map)
    ruby_job.setIncludeDir(OpenStudio::Path.new("#{$OpenStudio_Dir}"))
    work_items << ruby_job.toWorkItem

    # OpenStudio Measure 4: Set the East WWR
    arg_map["wwr"].setValue(0.0)
    arg_map["sillHeight"].setValue(30.0)
    arg_map["facade"].setValue("East")
    ruby_job = OpenStudio::Runmanager::RubyJobBuilder.new(wwr_measure,arg_map)
    ruby_job.setIncludeDir(OpenStudio::Path.new("#{$OpenStudio_Dir}"))
    work_items << ruby_job.toWorkItem

    # OpenStudio Measure 5: Set the West WWR
    arg_map["wwr"].setValue(0.0)
    arg_map["sillHeight"].setValue(30.0)
    arg_map["facade"].setValue("West")
    ruby_job = OpenStudio::Runmanager::RubyJobBuilder.new(wwr_measure,arg_map)
    ruby_job.setIncludeDir(OpenStudio::Path.new("#{$OpenStudio_Dir}"))
    work_items << ruby_job.toWorkItem

    # ModelToIdf
    puts "Adding ModelToIdf step."
    work_items << OpenStudio::Runmanager::WorkItem.new("ModelToIdf".to_JobType)
    
    # EnergyPlus Measure 1: Xcel EDA Tariff Selection and Model Setup
    puts "Adding Xcel set-up measure."
    xcel_measure = OpenStudio::getMeasure("c5ab60e9-b8e5-4653-a502-5ed030f1335d")
    raise "Unable to retrieve Xcel EDA Setup measure from BCL." if xcel_measure.empty?
    xcel_measure = xcel_measure.get
    xcel_measure = xcel_measure.clone(scripts_dir / OpenStudio::Path.new(xcel_measure.name))
    raise "Unable to clone Xcel EDA Setup measure into scripts directory." if xcel_measure.empty?
    xcel_measure = xcel_measure.get
    args = OpenStudio::Ruleset::getArguments(xcel_measure,model)
    arg_map = OpenStudio::Ruleset::convertOSArgumentVectorToMap(args)
    ruby_job = OpenStudio::Runmanager::RubyJobBuilder.new(xcel_measure,arg_map)
    ruby_job.setIncludeDir(OpenStudio::Path.new("#{$OpenStudio_Dir}"))
    work_items << ruby_job.toWorkItem

    # EnergyPlus
    puts "Adding EnergyPlus and OpenStudioPostProcess steps."
    work_items << OpenStudio::Runmanager::WorkItem.new("EnergyPlus".to_JobType)

    # OpenStudioPostProcess
    work_items << OpenStudio::Runmanager::WorkItem.new("OpenStudioPostProcess".to_JobType)

    # Save WorkItems JSON
    json_path = model_dir / OpenStudio::Path.new("work_items.json")
    puts "Saving the work_items.json file to " + json_path.to_s + "."
    test = OpenStudio::Runmanager::saveJSON(work_items,json_path,true)
    raise "Unable to save WorkItems json file." if not test

    # Call RunWorkItems.rb
    puts "Calling RunWorkItems.rb."
    assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/examples/RunWorkItems.rb' '#{model_path.to_s}'"))    
    
  end

end

