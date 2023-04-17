########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

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

  def test_LogChannelFilter

    assert(OpenStudio::Logger::instance())
    assert(OpenStudio::Logger::instance().standardOutLogger())

    # create a StringStreamLogSink
    logStream = OpenStudio::StringStreamLogSink.new()
    assert(logStream.logLevel.empty?)
    logStream.setLogLevel(OpenStudio::Warn)
    assert((not logStream.logLevel.empty?))
    assert_equal(OpenStudio::Warn, logStream.logLevel.get)

    # set channel regex
    logStream.setChannelRegex("^model.*")

    # invoke methods to test the log sinks
    OpenStudio::logFree(OpenStudio::Warn, "utilities.test_instance", "Warn")
    OpenStudio::logFree(OpenStudio::Warn, "model.test_instance", "Warn")
    OpenStudio::logFree(OpenStudio::Warn, "gbxml.test_instance", "Warn")

    # check log sinks
    assert_equal(1, logStream.logMessages.size)

  end
end


