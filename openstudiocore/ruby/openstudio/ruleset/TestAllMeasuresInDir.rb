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
