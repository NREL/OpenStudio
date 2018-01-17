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

######################################################################
# == Synopsis
#
#   Runs tests for all measures found in a directory.
#
# == Usage
#
#  ruby TestAllMeasuresInDir.rb ARGV[0]
#
#  ARGV[0] - Path to directory containing measures
#
# == Examples
#
#   ruby TestAllMeasuresInDir.rb 'C:\path\to\measures\' 
#
######################################################################

require 'openstudio'
require 'openstudio/ruleset/ShowRunnerOutput'

require 'minitest/autorun'

require 'set'

dir = ARGV[0].gsub("\\", "/")

if not dir or not File.directory?(dir)
  puts "Script requires argument which is path to directory containing measures"
  exit(false)
end

puts "Testing measures found in '#{dir}'"

$num_measures = 0
$failures = []
user_script_classes = Set.new

Dir.glob("#{dir}/*/") do |measure_dir|
  
  puts "Testing #{measure_dir}"
  $num_measures = $num_measures + 1
  
  if File.directory?("#{measure_dir}")
    measure = OpenStudio::BCLMeasure::load(OpenStudio::Path.new("#{measure_dir}"))
    if measure.empty?
      $failures <<  "Directory #{measure_dir} is not a measure"
      next
    end
    measure = measure.get
    
    # load the measure
    begin
      load "#{measure_dir}/measure.rb"
    rescue Exception => e
      $failures <<  "Could not load measure in #{measure_dir}"
      $failures <<  e
      $failures <<  e.backtrace.join("\n")
      next
    end
    
    user_script_class = nil
    ObjectSpace.each_object(OpenStudio::Ruleset::UserScript) do |us|
      if not user_script_classes.include?(us.class)
        user_script_class = us.class
        user_script_classes << user_script_class
      end
    end

    if user_script_class == nil
      $failures << "Did not find measure class in #{measure_dir}"
      next
    end
    
    args = nil
    if measure.measureType == "ModelMeasure".to_MeasureType
      args = user_script_class.new.arguments(OpenStudio::Model::Model.new)
    elsif measure.measureType == "EnergyPlusMeasure".to_MeasureType
      args = user_script_class.new.arguments(OpenStudio::Workspace.new)   
    elsif measure.measureType == "UtilityMeasure".to_MeasureType
      args = user_script_class.new.arguments()
    elsif measure.measureType == "ReportingMeasure".to_MeasureType
      args = user_script_class.new.arguments()      
    end
    
    if not args or args.class != OpenStudio::Ruleset::OSArgumentVector
      $failures << "Could not extract arguments for measure #{measure_dir}"
      next
    end
    
    # run all tests
    num_tests = 0
    Dir.glob("#{measure_dir}/tests/*_Test.rb").each do |test|
      num_tests = num_tests + 1
      puts "Running measure test #{test}"
      if not load(test)
        $failures << "Failed to load test #{test}"
      end
    end
    
    if num_tests == 0
      $failures << "No tests found for #{measure_dir}"
    end
    
  end
end

class TestAllMeasures_Test < MiniTest::Unit::TestCase
  # def setup
  # end

  # def teardown
  # end
  
  def test_AllMeasures
  
    assert($num_measures > 0)

    $failures.each do |failure|
      puts "#{failure}"
    end
    assert($failures.empty?)
    
  end
end
