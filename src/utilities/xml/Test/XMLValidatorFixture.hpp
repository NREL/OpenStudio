/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_XML_TEST_XMLVALIDATORFIXTURE_HPP
#define UTILITIES_XML_TEST_XMLVALIDATORFIXTURE_HPP

#include <gtest/gtest.h>

#include "../XMLValidator.hpp"
#include "../../core/Logger.hpp"
#include "../../core/FileLogSink.hpp"

#include <boost/optional.hpp>
#include <resources.hxx>

class XMLValidatorFixture : public ::testing::Test
{
 public:
  // initialize static members
  static void SetUpTestSuite();

  // tear down static members
  static void TearDownTestSuite();

 protected:
  // initialize for each test
  virtual void SetUp() override;

  // tear down after for each test
  virtual void TearDown() override;

  // set up logging
  REGISTER_LOGGER("XMLValidatorFixture");

 public:
  static openstudio::path schemaPath;
  static boost::optional<openstudio::FileLogSink> logFile;
};

// filename, isValid, number of e
struct GbXMLTestInfo
{
  std::string filename{};
  int n_warnings = 0;
  int n_errors = 0;
};

class GbXMLValidatorParametrizedFixture
  : public XMLValidatorFixture
  , public ::testing::WithParamInterface<std::tuple<std::string, int, int>>
{
};

#endif  // UTILITIES_XML_TEST_VALIDATORFIXTURE_HPP
