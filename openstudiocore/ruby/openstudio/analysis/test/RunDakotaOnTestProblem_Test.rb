######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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


class SimpleDakotaDemonstration_Test < Test::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_RunDakotaOnTestProblem_Driver
    outputDir = OpenStudio::Path.new("./RunDakotaOnTestProblem/Driver")
    OpenStudio::create_directory(outputDir.parent_path)
    outputDir = outputDir.to_s
    problemFile = "#{$OpenStudio_LibPath}openstudio/analysis/problems/Rosenbrock.rb"  
    initFile = "#{$OpenStudio_LibPath}openstudio/analysis/initializers/DaceLHS.rb"
    assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/analysis/RunDakotaOnTestProblem.rb' -o '#{outputDir}' -p '#{problemFile}' -i '#{initFile}' -d"))    
  end
  
  def test_RunDakotaOnTestProblem_NoDriver
    outputDir = OpenStudio::Path.new("./RunDakotaOnTestProblem/NoDriver")
    OpenStudio::create_directory(outputDir.parent_path)
    outputDir = outputDir.to_s    
    problemFile = "#{$OpenStudio_LibPath}openstudio/analysis/problems/Rosenbrock.rb" 
    initFile = "#{$OpenStudio_LibPath}openstudio/analysis/initializers/DaceOAS.rb"
    assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/analysis/RunDakotaOnTestProblem.rb' -o '#{outputDir}' -p '#{problemFile}' -i '#{initFile}'"))    
  end  
  
  def test_RunDakotaOnTestProblem_MainEffectsModel
    outputDir = OpenStudio::Path.new("./RunDakotaOnTestProblem/MainEffectsModel")
    OpenStudio::create_directory(outputDir.parent_path)
    outputDir = outputDir.to_s    
    problemFile = "#{$OpenStudio_LibPath}openstudio/analysis/problems/TestFunction10D.rb"
    
    # create samples
    initFile = "#{$OpenStudio_LibPath}openstudio/analysis/initializers/DaceLHS.rb"
    assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/analysis/RunDakotaOnTestProblem.rb' -o '#{outputDir}' -p '#{problemFile}' -i '#{initFile}' -d"))
  
    # create and evaluate meta-model
    initFile = "#{$OpenStudio_LibPath}openstudio/analysis/initializers/MainEffectsModel.rb"
    restartFile = OpenStudio::Path.new(outputDir)/OpenStudio::Path.new("dakota1.rst")
    restartFile = restartFile.to_s
    assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/analysis/RunDakotaOnTestProblem.rb' -o '#{outputDir}' -p '#{problemFile}' -i '#{initFile}' -r '#{restartFile}' -d"))
  
  end

end


