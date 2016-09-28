########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

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

