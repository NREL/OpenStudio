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

require 'test/unit'

class PATMeasures_Test < Test::Unit::TestCase

  def test_PATMeasures
    patApplicationMeasuresDir = OpenStudio::BCLMeasure::patApplicationMeasuresDir

    puts "#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/ruleset/TestAllMeasuresInDir.rb' '#{patApplicationMeasuresDir}'"
    assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/ruleset/TestAllMeasuresInDir.rb' '#{patApplicationMeasuresDir}'"))

  end
  
  def test_NewMeasures
    
    test_dir = "./NewMeasuresDir/"
    if File.exists?(test_dir)
      FileUtils.rm_rf(test_dir)
    end
    assert((not File.exists?(test_dir)))
    
    measure_type = "ModelMeasure"
    OpenStudio::BCLMeasure.new(measure_type, measure_type, OpenStudio::Path.new(test_dir + measure_type), "", measure_type.to_MeasureType, false)
    assert(File.exists?(test_dir + measure_type))
    
    measure_type = "EnergyPlusMeasure"
    OpenStudio::BCLMeasure.new(measure_type, measure_type, OpenStudio::Path.new(test_dir + measure_type), "", measure_type.to_MeasureType, false)
    assert(File.exists?(test_dir + measure_type))
    
    measure_type = "UtilityMeasure"
    OpenStudio::BCLMeasure.new(measure_type, measure_type, OpenStudio::Path.new(test_dir + measure_type), "", measure_type.to_MeasureType, false)
    assert(File.exists?(test_dir + measure_type))
    
    measure_type = "ReportingMeasure"
    OpenStudio::BCLMeasure.new(measure_type, measure_type, OpenStudio::Path.new(test_dir + measure_type), "", measure_type.to_MeasureType, false)
    assert(File.exists?(test_dir + measure_type))
    
    puts "#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/ruleset/TestAllMeasuresInDir.rb' '#{test_dir}'"
    assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/ruleset/TestAllMeasuresInDir.rb' '#{test_dir}'"))

  end
end

