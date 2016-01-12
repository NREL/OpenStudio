######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

require 'openstudio'
require 'openstudio/ruleset/ShowRunnerOutput'

require "#{File.dirname(__FILE__)}/../measure.rb"

require 'test/unit'

class SetWindowToWallRatioByFacade_Test < Test::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_SetWindowToWallRatioByFacade_fail
     
    # create an instance of the measure
    measure = SetWindowToWallRatioByFacade.new
    
    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # make an empty model
    model = OpenStudio::Model::Model.new
    
    # get arguments and test that they are what we are expecting
    arguments = measure.arguments(model)
    assert_equal(3, arguments.size)
    assert_equal("wwr", arguments[0].name)
    assert_equal("sillHeight", arguments[1].name)
    assert_equal("facade", arguments[2].name)

    # set argument values to bad values and run the measure
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    wwr = arguments[0].clone
    assert(wwr.setValue("20"))
    argument_map["wwr"] = wwr
    measure.run(model, runner, argument_map)
    result = runner.result
    assert(result.value.valueName == "Fail")
  end
  
  def test_SetWindowToWallRatioByFacade_with_model

    # create an instance of the measure
    measure = SetWindowToWallRatioByFacade.new
    
    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # load the test model
    translator = OpenStudio::OSVersion::VersionTranslator.new
    path = OpenStudio::Path.new(File.dirname(__FILE__) + "/EnvelopeAndLoadTestModel_01.osm")
    model = translator.loadModel(path)
    assert((not model.empty?))
    model = model.get
    
    # get arguments
    arguments = measure.arguments(model)    
    
    # set argument values to good values and run the measure on model with spaces
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    
    wwr = arguments[0].clone
    assert(wwr.setValue(0.4))
    argument_map["wwr"] = wwr
    
    sillHeight = arguments[1].clone
    assert(sillHeight.setValue(30.0))
    argument_map["sillHeight"] = sillHeight
    
    facade = arguments[2].clone
    assert(facade.setValue("South"))
    argument_map["facade"] = facade
    
    measure.run(model, runner, argument_map)    
    result = runner.result
    #show_output(result)
    assert(result.value.valueName == "Success")
    assert(result.warnings.size == 2)
    assert(result.info.size == 2)

    #save the model
    #output_file_path = OpenStudio::Path.new(File.dirname(__FILE__) + "/test.osm")
    #model.save(output_file_path,true)

  end

  def test_SetWindowToWallRatioByFacade_with_model_RotationTest

    # create an instance of the measure
    measure = SetWindowToWallRatioByFacade.new

    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new

    # load the test model
    translator = OpenStudio::OSVersion::VersionTranslator.new
    path = OpenStudio::Path.new(File.dirname(__FILE__) + "/EnvelopeAndLoadTestModel_02_RotatedSpaceAndBuilding.osm")
    model = translator.loadModel(path)
    assert((not model.empty?))
    model = model.get

    # get arguments
    arguments = measure.arguments(model)

    # set argument values to good values and run the measure on model with spaces
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new

    wwr = arguments[0].clone
    assert(wwr.setValue(0.4))
    argument_map["wwr"] = wwr

    sillHeight = arguments[1].clone
    assert(sillHeight.setValue(30.0))
    argument_map["sillHeight"] = sillHeight

    facade = arguments[2].clone
    assert(facade.setValue("South"))
    argument_map["facade"] = facade

    measure.run(model, runner, argument_map)
    result = runner.result
    show_output(result)
    assert(result.value.valueName == "Success")
    #assert(result.warnings.size == 2)
    #assert(result.info.size == 2)

    #save the model
    output_file_path = OpenStudio::Path.new(File.dirname(__FILE__) + "/test.osm")
    model.save(output_file_path,true)

  end

  def test_SetWindowToWallRatioByFacade_with_model_MinimalCost

    # create an instance of the measure
    measure = SetWindowToWallRatioByFacade.new
    
    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # load the test model
    translator = OpenStudio::OSVersion::VersionTranslator.new
    path = OpenStudio::Path.new(File.dirname(__FILE__) + "/EnvelopeAndLoadTestModel_01.osm")
    model = translator.loadModel(path)
    assert((not model.empty?))
    model = model.get
    
    # get arguments
    arguments = measure.arguments(model)    
    
    # set argument values to good values and run the measure on model with spaces
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    
    wwr = arguments[0].clone
    assert(wwr.setValue(0.4))
    argument_map["wwr"] = wwr
    
    sillHeight = arguments[1].clone
    assert(sillHeight.setValue(30.0))
    argument_map["sillHeight"] = sillHeight
    
    facade = arguments[2].clone
    assert(facade.setValue("South"))
    argument_map["facade"] = facade
    
    measure.run(model, runner, argument_map)    
    result = runner.result
    #show_output(result)
    assert(result.value.valueName == "Success")
    assert(result.warnings.size == 2)
    assert(result.info.size == 2)
    
  end 
  
  def test_SetWindowToWallRatioByFacade_with_model_NoCost

    # create an instance of the measure
    measure = SetWindowToWallRatioByFacade.new
    
    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new
    
    # load the test model
    translator = OpenStudio::OSVersion::VersionTranslator.new
    path = OpenStudio::Path.new(File.dirname(__FILE__) + "/EnvelopeAndLoadTestModel_01.osm")
    model = translator.loadModel(path)
    assert((not model.empty?))
    model = model.get
    
    # get arguments
    arguments = measure.arguments(model)    
    
    # set argument values to good values and run the measure on model with spaces
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new
    
    wwr = arguments[0].clone
    assert(wwr.setValue(0.4))
    argument_map["wwr"] = wwr
    
    sillHeight = arguments[1].clone
    assert(sillHeight.setValue(30.0))
    argument_map["sillHeight"] = sillHeight
    
    facade = arguments[2].clone
    assert(facade.setValue("South"))
    argument_map["facade"] = facade    
    
    measure.run(model, runner, argument_map)    
    result = runner.result
    #show_output(result)
    assert(result.value.valueName == "Success")
    assert(result.warnings.size == 2)
    assert(result.info.size == 2)
    
  end

  def test_SetWindowToWallRatioByFacade_ReverseTranslatedModel

    # create an instance of the measure
    measure = SetWindowToWallRatioByFacade.new

    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new

    # load the test model
    translator = OpenStudio::OSVersion::VersionTranslator.new
    path = OpenStudio::Path.new(File.dirname(__FILE__) + "/ReverseTranslatedModel.osm")
    model = translator.loadModel(path)
    assert((not model.empty?))
    model = model.get

    # get arguments
    arguments = measure.arguments(model)

    # set argument values to good values and run the measure on model with spaces
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new

    wwr = arguments[0].clone
    assert(wwr.setValue(0.4))
    argument_map["wwr"] = wwr

    sillHeight = arguments[1].clone
    assert(sillHeight.setValue(30.0))
    argument_map["sillHeight"] = sillHeight

    facade = arguments[2].clone
    assert(facade.setValue("East"))
    argument_map["facade"] = facade

    measure.run(model, runner, argument_map)
    result = runner.result
    #show_output(result)
    assert(result.value.valueName == "Success")
    assert(result.warnings.size == 1)
    assert(result.info.size == 0)

  end

  def test_SetWindowToWallRatioByFacade_EmptySpaceNoLoadsOrSurfaces

    # create an instance of the measure
    measure = SetWindowToWallRatioByFacade.new

    # create an instance of a runner
    runner = OpenStudio::Ruleset::OSRunner.new

    # make an empty model
    model = OpenStudio::Model::Model.new

    # add a space to the model without any geometry or loads, want to make sure measure works or fails gracefully
    new_space = OpenStudio::Model::Space.new(model)

    # get arguments
    arguments = measure.arguments(model)

    # set argument values to good values and run the measure on model with spaces
    argument_map = OpenStudio::Ruleset::OSArgumentMap.new

    wwr = arguments[0].clone
    assert(wwr.setValue(0.4))
    argument_map["wwr"] = wwr

    sillHeight = arguments[1].clone
    assert(sillHeight.setValue(30.0))
    argument_map["sillHeight"] = sillHeight

    facade = arguments[2].clone
    assert(facade.setValue("South"))
    argument_map["facade"] = facade

    measure.run(model, runner, argument_map)
    result = runner.result
    #show_output(result)
    assert(result.value.valueName == "NA")
    #assert(result.warnings.size == 0)
    #assert(result.info.size == 1)

  end

end


