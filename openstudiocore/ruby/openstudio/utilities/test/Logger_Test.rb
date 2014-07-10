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

require 'minitest/autorun'

class Logger_Test < MiniTest::Unit::TestCase
  
  # def setup
  # end

  # def teardown
  # end
  
  def test_Logger
    
    assert(OpenStudio::Logger::instance())
    assert(OpenStudio::Logger::instance().standardOutLogger())
    
    # create a FileLogSink
    logFile = OpenStudio::FileLogSink.new(OpenStudio::Path.new("./log.log"))
    assert(logFile.logLevel.empty?)
    logFile.setLogLevel(OpenStudio::Debug)
    assert((not logFile.logLevel.empty?))
    assert_equal(OpenStudio::Debug, logFile.logLevel.get)
    
    # create a StringStreamLogSink
    logStream = OpenStudio::StringStreamLogSink.new()
    assert(logStream.logLevel.empty?)
    logStream.setLogLevel(OpenStudio::Warn)
    assert((not logStream.logLevel.empty?))
    assert_equal(OpenStudio::Warn, logStream.logLevel.get)

    # invoke methods to test the log sinks
    OpenStudio::logFree(OpenStudio::Debug, "Logger_Test.test_instance", "Debug")
    OpenStudio::logFree(OpenStudio::Warn, "Logger_Test.test_instance", "Warn")
    OpenStudio::logFree(OpenStudio::Error, "Logger_Test.test_instance", "Error")
    
    # check log sinks
    assert_equal(3, logFile.logMessages.size)
    assert_equal(2, logStream.logMessages.size)
    
  end

end


