########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'openstudio'

require 'minitest/autorun'

class Logger_Test < Minitest::Test

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


