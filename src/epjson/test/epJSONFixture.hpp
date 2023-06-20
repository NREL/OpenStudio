/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef EPJSON_TEST_EPJSONFIXTURE_HPP
#define EPJSON_TEST_EPJSONFIXTURE_HPP

#include <gtest/gtest.h>

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/FileLogSink.hpp"

#include <boost/optional.hpp>

namespace Json {
class Value;
}

class epJSONFixture : public ::testing::Test
{
 public:
  void compareEPJSONTranslations(const std::string& idfname);
  static openstudio::path completeIDFPath(const openstudio::path& idf);
  static void makeDoubles(Json::Value& value);

 protected:
  /// initialize for each test
  virtual void SetUp() override;

  /// tear down after each test
  virtual void TearDown() override;

  /// initialize static members
  static void SetUpTestSuite();

  /// tear down static members
  static void TearDownTestSuite();

  REGISTER_LOGGER("epJSONFixture");

  // static variables
  static boost::optional<openstudio::FileLogSink> logFile;

 private:
  void compareJSONS(const Json::Value& lhs, const Json::Value& rhs);

  void compareJSONS_detailed(Json::Value& lhs, Json::Value& rhs, const std::string& currentPath);

  static std::pair<Json::Value, Json::Value> doEPJSONTranslations(const std::string& idfname);
};

#endif  // EPJSON_TEST_EPJSONFIXTURE_HPP
