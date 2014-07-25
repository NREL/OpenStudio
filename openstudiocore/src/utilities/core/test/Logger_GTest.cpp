/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <gtest/gtest.h>

#include "../Logger.hpp"
#include "../FileLogSink.hpp"
#include "../StringStreamLogSink.hpp"

#include <sstream>

using openstudio::toPath;
using openstudio::Logger;
using openstudio::FileLogSink;
using openstudio::StringStreamLogSink;
using openstudio::LogMessage;

namespace
{

  class Hello{
    public:
      Hello(){}
      void logDebug(){LOG(Debug, "Hello Debug");}
      void logError(){LOG(Error, "Hello Error");}
      REGISTER_LOGGER("hello.channel");
  };

  class Goodbye{
    public:
      Goodbye(){}
      void logDebug(){LOG(Debug, "Goodbye Debug");}
      void logError(){LOG(Error, "Goodbye Error");}
      REGISTER_LOGGER("goodbye.channel");
  };

  void freeLogging()
  {
    // this works
    //openstudio::logFree(Debug, "free.channel", "Free Debug");
    //openstudio::logFree(Error, "free.channel", "Free Error");

    // this does not work
    //BOOST_LOG_SEV(openstudio::Logger::instance().loggerFromChannel("free.channel"), Debug) << "Free Debug";
    //BOOST_LOG_SEV(openstudio::Logger::instance().loggerFromChannel("free.channel"), Error) << "Free Error";

    // this does not work
    //LOG_FREE(Debug, std::string("free.channel"), std::string("Free Debug"));
    //LOG_FREE(Error, std::string("free.channel"), std::string("Free Error"));

    // this does not work
    //LOG_FREE(Debug, "free.channel", std::string("Free Debug"));
    //LOG_FREE(Error, "free.channel", std::string("Free Error"));

    // this does not work
    LOG_FREE(Debug, "free.channel", "Free Debug");
    LOG_FREE(Error, "free.channel", "Free Error");
  }

  void classLogging()
  {
    Hello h;
    h.logDebug();
    h.logError();

    Goodbye g;
    g.logDebug();
    g.logError();
  }


  TEST(LoggerTest, LogLevel_Formatting)
  {
    std::stringstream ss;

    ss << Debug;
    EXPECT_EQ("-2", ss.str());

    // reset stream
    ss.str("");

    ss << Error;
    EXPECT_EQ("1", ss.str());
  }

  TEST(LoggerTest, std_out)
  {
    openstudio::Logger::instance().standardOutLogger().disable();
    openstudio::Logger::instance().standardOutLogger().disable();
    openstudio::Logger::instance().standardOutLogger().enable();
    openstudio::Logger::instance().standardOutLogger().enable();
    openstudio::Logger::instance().standardOutLogger().disable();
    openstudio::Logger::instance().standardOutLogger().disable();
  }

  TEST(LoggerTest, std_err)
  {
    openstudio::Logger::instance().standardErrLogger().disable();
    openstudio::Logger::instance().standardErrLogger().disable();
    openstudio::Logger::instance().standardErrLogger().enable();
    openstudio::Logger::instance().standardErrLogger().enable();
    openstudio::Logger::instance().standardErrLogger().disable();
    openstudio::Logger::instance().standardErrLogger().disable();
  }

  TEST(LoggerTest, free_function)
  {
    openstudio::Logger::instance().standardOutLogger().disable();

    StringStreamLogSink sink;
    EXPECT_FALSE(sink.logLevel());
    EXPECT_FALSE(sink.channelRegex());
    EXPECT_TRUE(sink.logMessages().empty());

    freeLogging();
    ASSERT_EQ(2u, sink.logMessages().size());
    EXPECT_EQ(Debug, sink.logMessages()[0].logLevel());
    EXPECT_EQ("free.channel", sink.logMessages()[0].logChannel());
    EXPECT_EQ("Free Debug", sink.logMessages()[0].logMessage());
    EXPECT_EQ(Error, sink.logMessages()[1].logLevel());
    EXPECT_EQ("free.channel", sink.logMessages()[1].logChannel());
    EXPECT_EQ("Free Error", sink.logMessages()[1].logMessage());

    // reset sink
    sink.resetStringStream();
    sink.setLogLevel(Debug);
    ASSERT_TRUE(sink.logLevel());
    EXPECT_EQ(Debug, sink.logLevel().get());
    EXPECT_FALSE(sink.channelRegex());
    EXPECT_TRUE(sink.logMessages().empty());

    freeLogging();
    ASSERT_EQ(2u, sink.logMessages().size());
    EXPECT_EQ(Debug, sink.logMessages()[0].logLevel());
    EXPECT_EQ("free.channel", sink.logMessages()[0].logChannel());
    EXPECT_EQ("Free Debug", sink.logMessages()[0].logMessage());
    EXPECT_EQ(Error, sink.logMessages()[1].logLevel());
    EXPECT_EQ("free.channel", sink.logMessages()[1].logChannel());
    EXPECT_EQ("Free Error", sink.logMessages()[1].logMessage());

    // reset sink
    sink.resetStringStream();
    sink.setLogLevel(Error);
    ASSERT_TRUE(sink.logLevel());
    EXPECT_EQ(Error, sink.logLevel().get());
    EXPECT_FALSE(sink.channelRegex());
    EXPECT_TRUE(sink.logMessages().empty());

    freeLogging();
    ASSERT_EQ(1u, sink.logMessages().size());
    EXPECT_EQ(Error, sink.logMessages()[0].logLevel());
    EXPECT_EQ("free.channel", sink.logMessages()[0].logChannel());
    EXPECT_EQ("Free Error", sink.logMessages()[0].logMessage());

    // reset sink
    sink.resetStringStream();
    sink.setLogLevel(Debug);
    ASSERT_TRUE(sink.logLevel());
    EXPECT_EQ(Debug, sink.logLevel().get());
    EXPECT_FALSE(sink.channelRegex());
    EXPECT_TRUE(sink.logMessages().empty());

    // new sink
    StringStreamLogSink sink2;
    sink2.disable();
    EXPECT_FALSE(sink2.logLevel());
    EXPECT_FALSE(sink2.channelRegex());
    EXPECT_TRUE(sink2.logMessages().empty());

    freeLogging();
    EXPECT_TRUE(sink2.logMessages().empty());

    ASSERT_EQ(2u, sink.logMessages().size());
    EXPECT_EQ(Debug, sink.logMessages()[0].logLevel());
    EXPECT_EQ("free.channel", sink.logMessages()[0].logChannel());
    EXPECT_EQ("Free Debug", sink.logMessages()[0].logMessage());
    EXPECT_EQ(Error, sink.logMessages()[1].logLevel());
    EXPECT_EQ("free.channel", sink.logMessages()[1].logChannel());
    EXPECT_EQ("Free Error", sink.logMessages()[1].logMessage());
  }

  TEST(LoggerTest, class_logger)
  {
    openstudio::Logger::instance().standardOutLogger().disable();

    StringStreamLogSink sink;
    EXPECT_FALSE(sink.logLevel());
    EXPECT_FALSE(sink.channelRegex());
    EXPECT_TRUE(sink.logMessages().empty());

    classLogging();
    ASSERT_EQ(4u, sink.logMessages().size());
    EXPECT_EQ(Debug, sink.logMessages()[0].logLevel());
    EXPECT_EQ("hello.channel", sink.logMessages()[0].logChannel());
    EXPECT_EQ("Hello Debug", sink.logMessages()[0].logMessage());
    EXPECT_EQ(Error, sink.logMessages()[1].logLevel());
    EXPECT_EQ("hello.channel", sink.logMessages()[1].logChannel());
    EXPECT_EQ("Hello Error", sink.logMessages()[1].logMessage());
    EXPECT_EQ(Debug, sink.logMessages()[2].logLevel());
    EXPECT_EQ("goodbye.channel", sink.logMessages()[2].logChannel());
    EXPECT_EQ("Goodbye Debug", sink.logMessages()[2].logMessage());
    EXPECT_EQ(Error, sink.logMessages()[3].logLevel());
    EXPECT_EQ("goodbye.channel", sink.logMessages()[3].logChannel());
    EXPECT_EQ("Goodbye Error", sink.logMessages()[3].logMessage());

    // reset sink
    sink.resetStringStream();
    sink.setChannelRegex(boost::regex("hello\\..*"));
    EXPECT_FALSE(sink.logLevel());
    ASSERT_TRUE(sink.channelRegex());
    EXPECT_EQ(boost::regex("hello\\..*"), sink.channelRegex().get());
    EXPECT_TRUE(sink.logMessages().empty());

    classLogging();
    ASSERT_EQ(2u, sink.logMessages().size());
    EXPECT_EQ(Debug, sink.logMessages()[0].logLevel());
    EXPECT_EQ("hello.channel", sink.logMessages()[0].logChannel());
    EXPECT_EQ("Hello Debug", sink.logMessages()[0].logMessage());
    EXPECT_EQ(Error, sink.logMessages()[1].logLevel());
    EXPECT_EQ("hello.channel", sink.logMessages()[1].logChannel());
    EXPECT_EQ("Hello Error", sink.logMessages()[1].logMessage());

    // reset sink
    sink.resetStringStream();
    sink.setLogLevel(Error);
    sink.setChannelRegex(boost::regex("hello\\..*"));
    ASSERT_TRUE(sink.logLevel());
    EXPECT_EQ(Error, sink.logLevel().get());
    ASSERT_TRUE(sink.channelRegex());
    EXPECT_EQ(boost::regex("hello\\..*"), sink.channelRegex().get());
    EXPECT_TRUE(sink.logMessages().empty());

    classLogging();
    ASSERT_EQ(1u, sink.logMessages().size());
    EXPECT_EQ(Error, sink.logMessages()[0].logLevel());
    EXPECT_EQ("hello.channel", sink.logMessages()[0].logChannel());
    EXPECT_EQ("Hello Error", sink.logMessages()[0].logMessage());
  }

  TEST(LoggerTest, file_logger)
  {
    openstudio::Logger::instance().standardOutLogger().disable();

    openstudio::path path = toPath("./file_logger.log");
    boost::filesystem::remove(path);
    ASSERT_FALSE(boost::filesystem::exists(path));

    {
      FileLogSink sink(path);
      sink.setLogLevel(Error);
      sink.setChannelRegex(boost::regex("hello\\..*"));
      ASSERT_TRUE(boost::filesystem::exists(path));

      freeLogging();
      classLogging();

#ifdef BOOST_WINDOWS
      // You can delete an open file on Unix platforms, so this aspect of the test
      // is only valid on Windows
      EXPECT_THROW(boost::filesystem::remove(path), std::exception);
#endif

      std::vector<LogMessage> logMessages = sink.logMessages();
      ASSERT_EQ(1u, logMessages.size());
      EXPECT_EQ(Error, sink.logMessages()[0].logLevel());
      EXPECT_EQ("hello.channel", sink.logMessages()[0].logChannel());
      EXPECT_EQ("Hello Error", sink.logMessages()[0].logMessage());
    }

#ifdef BOOST_WINDOWS
    // You can delete an open file on Unix platforms, so this aspect of the test
    // is only valid on Windows
    EXPECT_THROW(boost::filesystem::remove(path), std::exception);
#endif

  }

  TEST(LoggerTest, file_logger_disable)
  {
    openstudio::Logger::instance().standardOutLogger().disable();

    openstudio::path path = toPath("./file_logger_disable.log");
    boost::filesystem::remove(path);
    ASSERT_FALSE(boost::filesystem::exists(path));

    {
      FileLogSink sink(path);
      sink.setLogLevel(Error);
      sink.setChannelRegex(boost::regex("hello\\..*"));
      ASSERT_TRUE(boost::filesystem::exists(path));

      freeLogging();
      classLogging();

#ifdef BOOST_WINDOWS
      // You can delete an open file on Unix platforms, so this aspect of the test
      // is only valid on Windows
      EXPECT_THROW(boost::filesystem::remove(path), std::exception);
#endif


      sink.disable();

#ifdef BOOST_WINDOWS
      // You can delete an open file on Unix platforms, so this aspect of the test
      // is only valid on Windows
      EXPECT_THROW(boost::filesystem::remove(path), std::exception);
#endif


      std::vector<LogMessage> logMessages = sink.logMessages();
      ASSERT_EQ(1u, logMessages.size());
      EXPECT_EQ(Error, sink.logMessages()[0].logLevel());
      EXPECT_EQ("hello.channel", sink.logMessages()[0].logChannel());
      EXPECT_EQ("Hello Error", sink.logMessages()[0].logMessage());
    }

    EXPECT_NO_THROW(boost::filesystem::remove(path));
  }
}
