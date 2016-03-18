######################################################################
#  Copyright (c) 2008-2012, Alliance for Sustainable Energy.  
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
require 'fileutils'

require 'minitest/autorun'

class PATMeasures_Test < MiniTest::Unit::TestCase

  def runPATMeasureTest(pat_measure_name, test_file_name, test_name)
    patApplicationMeasuresDir = OpenStudio::BCLMeasure::patApplicationMeasuresDir
    
    # copy measures to build directory and test there
    test_dir = (OpenStudio::Path.new($OpenStudio_ResourcePath) / 
                OpenStudio::Path.new("ruleset/PATMeasures/#{pat_measure_name}/#{test_name}")).to_s
    if File.exists?(test_dir)
      FileUtils.rm_rf(test_dir)
    end
    assert(File.exists?("#{patApplicationMeasuresDir}/#{pat_measure_name}"))
    FileUtils.mkdir_p(test_dir)
    FileUtils.cp_r("#{patApplicationMeasuresDir}/#{pat_measure_name}", test_dir)
    
    test_file = "#{test_dir}/#{pat_measure_name}/tests/#{test_file_name}"
    assert(File.exists?(test_file))
    
    test_found = false
    File.open(test_file, 'r') do |file|
      while line = file.gets
        if /^\s*def\s+#{test_name}/.match(line)
          test_found = true
        end
      end
    end
    assert(test_found)

    test_string = "#{$OpenStudio_RubyExe} -I '#{$OpenStudio_Dir}' '#{test_file}' --name=#{test_name}"
    puts test_string
    assert(system(test_string))
  end
  
  def test_CalibrationMeasure_test_CalibrationReports
    runPATMeasureTest("CalibrationReports", "CalibrationReports_Test.rb", "test_CalibrationReports")
  end
  
  def test_CalibrationMeasure_test_CalibrationReport_NoGas
    runPATMeasureTest("CalibrationReports", "CalibrationReports_Test.rb", "test_CalibrationReport_NoGas")
  end
  
  def test_CalibrationMeasure_test_CalibrationReports_NoDemand
    runPATMeasureTest("CalibrationReports", "CalibrationReports_Test.rb", "test_CalibrationReports_NoDemand")
  end

  def test_RadianceMeasure_test_number_of_arguments_and_argument_names
    runPATMeasureTest("RadianceMeasure", "radiance_measure_test.rb", "test_number_of_arguments_and_argument_names")
  end
  
  def test_RadianceMeasure_test_measure
    runPATMeasureTest("RadianceMeasure", "radiance_measure_test.rb", "test_measure")
  end
  
  def test_ReplaceModel_test_ReplaceModel
    runPATMeasureTest("ReplaceModel", "ReplaceModel_Test.rb", "test_ReplaceModel")
  end
  
  def test_ReplaceModel_test_ReplaceModel_AltMeasures
    runPATMeasureTest("ReplaceModel", "ReplaceModel_Test.rb", "test_ReplaceModel_AltMeasures")
  end
  
  def test_ReportRequest_test_number_of_arguments_and_argument_names
    runPATMeasureTest("ReportRequest", "report_request_test.rb", "test_number_of_arguments_and_argument_names")
  end
  
  def test_StandardReports_test_example_model
    runPATMeasureTest("StandardReports", "OpenStudioResults_Test.rb", "test_example_model")
  end
    
  def test_StandardReports_test_edge_model
    runPATMeasureTest("StandardReports", "OpenStudioResults_Test.rb", "test_edge_model")
  end
  
  def test_StandardReports_test_empty_model
    runPATMeasureTest("StandardReports", "OpenStudioResults_Test.rb", "test_empty_model")
  end
  
  def test_NewMeasures
    
    test_dir = (OpenStudio::Path.new($OpenStudio_ResourcePath) / 
                OpenStudio::Path.new("ruleset/NewMeasuresDir/")).to_s
    if File.exists?(test_dir)
      FileUtils.rm_rf(test_dir)
    end
    assert((not File.exists?(test_dir)))
    
    measure_type = "ModelMeasure"
    OpenStudio::BCLMeasure.new(measure_type, measure_type, OpenStudio::Path.new(test_dir + measure_type), "", measure_type.to_MeasureType, measure_type, measure_type)
    assert(File.exists?(test_dir + measure_type))
    
    measure_type = "EnergyPlusMeasure"
    OpenStudio::BCLMeasure.new(measure_type, measure_type, OpenStudio::Path.new(test_dir + measure_type), "", measure_type.to_MeasureType, measure_type, measure_type)
    assert(File.exists?(test_dir + measure_type))
    
    measure_type = "UtilityMeasure"
    OpenStudio::BCLMeasure.new(measure_type, measure_type, OpenStudio::Path.new(test_dir + measure_type), "", measure_type.to_MeasureType, measure_type, measure_type)
    assert(File.exists?(test_dir + measure_type))
    
    measure_type = "ReportingMeasure"
    OpenStudio::BCLMeasure.new(measure_type, measure_type, OpenStudio::Path.new(test_dir + measure_type), "", measure_type.to_MeasureType, measure_type, measure_type)
    assert(File.exists?(test_dir + measure_type))
    
    puts "#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/ruleset/TestAllMeasuresInDir.rb' '#{test_dir}'"
    assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/ruleset/TestAllMeasuresInDir.rb' '#{test_dir}'"))

  end
  
end

