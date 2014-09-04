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
require 'fileutils'

require 'minitest/autorun'


class CreateScheduleLibrary_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_CreateScheduleLibrary
    logFile = OpenStudio::FileLogSink.new(OpenStudio::Path.new("CreateScheduleLibrary_Test.log"))
    logFile.setLogLevel(-3)
    OpenStudio::Logger.instance.standardOutLogger.disable

    dirStr = $OpenStudio_ResourcePath + "model/example/"
    FileUtils.mkdir_p dirStr
    osmFile = OpenStudio::Path.new(dirStr + "in.osm") 
    model = OpenStudio::Model::exampleModel
    model.save(osmFile,true)
    puts "#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/examples/CreateScheduleLibrary.rb' '#{osmFile}' ./scheduleLibrary.osp"
    assert(system("#{$OpenStudio_RubyExe} -I'#{$OpenStudio_Dir}' '#{$OpenStudio_LibPath}openstudio/examples/CreateScheduleLibrary.rb' '#{osmFile}' ./scheduleLibrary.osp"))
  end

end


