/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "epJSONFixture.hpp"

#include "../epJSONTranslator.hpp"

#include "../../utilities/core/ApplicationPathHelpers.hpp"
#include "../../utilities/core/Compare.hpp"
#include "../../utilities/core/Filesystem.hpp"
#include "../../utilities/core/PathHelpers.hpp"
#include "../../utilities/idf/IdfFile.hpp"

#include <resources.hxx>
#include <OpenStudio.hxx>

#include <fmt/format.h>
#include <json/json.h>
#include <cstdlib>

using namespace openstudio;

void epJSONFixture::SetUp() {}

void epJSONFixture::TearDown() {}

void epJSONFixture::SetUpTestSuite() {
  // set up logging
  logFile = FileLogSink(toPath("./epJSONFixture.log"));
  logFile->setLogLevel(Debug);
  Logger::instance().standardOutLogger().disable();
}

void epJSONFixture::TearDownTestSuite() {
  logFile->disable();
}

void epJSONFixture::makeDoubles(Json::Value& value) {
  if (value.isNumeric()) {
    value = value.asDouble();
  } else {
    for (auto& child : value) {
      makeDoubles(child);
    }
  }
}

void epJSONFixture::compareJSONS(const Json::Value& lhs, const Json::Value& rhs) {
  auto doubledLhs = lhs;
  makeDoubles(doubledLhs);

  auto doubledRhs = rhs;
  makeDoubles(doubledRhs);

  if (doubledLhs != doubledRhs) {
    compareJSONS_detailed(doubledLhs, doubledRhs, "root");
    constexpr bool debug = false;
    if constexpr (debug) {
      {
        std::ofstream file_id;
        file_id.open("lhs.json");
        Json::StreamWriterBuilder builder;
        builder["commentStyle"] = "None";
        builder["indentation"] = "   ";
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        std::ofstream outputFileStream("/tmp/lhs.json");
        writer->write(doubledLhs, &outputFileStream);
      }
      {
        std::ofstream file_id;
        file_id.open("lhs.json");
        Json::StreamWriterBuilder builder;
        builder["commentStyle"] = "None";
        builder["indentation"] = "   ";
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        std::ofstream outputFileStream("/tmp/rhs.json");
        writer->write(doubledRhs, &outputFileStream);
      }
    }
  }
  ASSERT_TRUE(doubledLhs == doubledRhs);
}

void epJSONFixture::compareJSONS_detailed(Json::Value& lhs, Json::Value& rhs, const std::string& currentPath) {
  if (lhs.isNumeric()) {
    ASSERT_TRUE(rhs.isNumeric()) << "At " << currentPath << ", lhs is a numeric while rhs is not";
    ASSERT_EQ(lhs.asDouble(), rhs.asDouble()) << "At " << currentPath;
  } else if (lhs.isString()) {
    ASSERT_EQ(lhs, rhs) << "At " << currentPath;
  } else if (lhs.isArray()) {
    ASSERT_TRUE(rhs.isArray()) << "At " << currentPath << ", lhs is an arrary while rhs is not";
    ASSERT_EQ(lhs.size(), rhs.size()) << "At " << currentPath << ", size of array object differs";
    if (lhs != rhs) {
      for (int i = 0; i < static_cast<int>(lhs.size()); ++i) {
        compareJSONS_detailed(lhs[i], rhs[i], currentPath + "[" + std::to_string(i) + "]");
      }
      ASSERT_TRUE(false) << "Terminating due to array mismatch.";
    }
  } else {
    ASSERT_TRUE(lhs.isObject()) << "At " << currentPath << ", lhs is not an object? did I miss a type? [developer error]";
    ASSERT_TRUE(rhs.isObject()) << "At " << currentPath << ", lhs is an object but rhs is not.";
    if (lhs != rhs) {
      std::vector<std::string> lhsMemberNames = lhs.getMemberNames();
      std::vector<std::string> rhsMemberNames = rhs.getMemberNames();
      if (lhsMemberNames != rhsMemberNames) {
        for (auto& s : lhsMemberNames) {
          EXPECT_TRUE(rhs.isMember(s)) << "At " << currentPath << ", rhs is missing member named '" << s << "'";
        }
        for (auto& s : rhsMemberNames) {
          EXPECT_TRUE(lhs.isMember(s)) << "At " << currentPath << ", lhs is missing member named '" << s << "'";
        }
        ASSERT_TRUE(false) << "At " << currentPath << ", Terminating due to member name mismatch.";
      }

      for (auto& s : lhsMemberNames) {
        compareJSONS_detailed(lhs[s], rhs[s], currentPath + ">" + s);
      }
      ASSERT_TRUE(false) << "At " << currentPath << ", Terminating due to object mismatch.";
    }
  }
}

openstudio::path setupIdftoEPJSONTest(const openstudio::path& location) {
  const auto basename = openstudio::toPath(openstudio::filesystem::basename(location));
  const auto working_directory = openstudio::filesystem::complete(openstudio::toPath("epjson_tests") / basename);
  auto idf_path = working_directory / openstudio::toPath("eplus.idf");
  openstudio::filesystem::create_directories(working_directory);
  openstudio::filesystem::copy_file(location, idf_path, openstudio::filesystem::copy_options::overwrite_existing);
  return idf_path;
}

Json::Value translateIdfToEPJSONWithEP(const openstudio::path& location) {

  // In case for some reason the energyplus CLI cannot do the conversion, we do not want to pick up an old artifact
  const auto epJSONFile = openstudio::setFileExtension(location, "epJSON", true);
  if (openstudio::filesystem::exists(epJSONFile)) {
    openstudio::filesystem::remove(epJSONFile);
  }

  std::string cmd = fmt::format("{} --output-directory {} --convert-only {}", openstudio::toString(openstudio::getEnergyPlusExecutable().native()),
                                openstudio::toString(location.parent_path().native()), openstudio::toString(location.native()));

  [[maybe_unused]] auto result = std::system(cmd.c_str());
  if (!openstudio::filesystem::exists(epJSONFile)) {
    throw openstudio::Exception(fmt::format("Error during the E+ CLI call (return code {}), epJSON wasn't created at {}\ncmd = '{}'\n", result,
                                            openstudio::toString(epJSONFile), cmd));
  }

  auto root = openstudio::epJSON::loadJSON(epJSONFile);

  const auto outputLocation = location.parent_path() / openstudio::toPath("eplus-rewritten.epJSON");
  std::ofstream ofs(openstudio::toString(outputLocation), std::ofstream::trunc);
  ofs << root.toStyledString() << '\n';

  return root;
}

openstudio::path epJSONFixture::completeIDFPath(const openstudio::path& idf) {
  return openstudio::getEnergyPlusDirectory() / openstudio::toPath("ExampleFiles") / idf;
}

Json::Value translateIdfToEPJSONWithOS(const openstudio::path& location) {
  auto idf = openstudio::IdfFile::load(location);

  if (!idf) {
    return Json::Value{};
  }

  auto result = openstudio::epJSON::toJSON(*idf);

  const auto outputLocation = location.parent_path() / openstudio::toPath("os.epJSON");
  std::ofstream ofs(openstudio::toString(outputLocation), std::ofstream::trunc);
  ofs << result.toStyledString() << '\n';
  return result;
}

std::pair<Json::Value, Json::Value> epJSONFixture::doEPJSONTranslations(const std::string& idfname) {

  const auto setupIdf = setupIdftoEPJSONTest(epJSONFixture::completeIDFPath(openstudio::toString(idfname)));
  return {translateIdfToEPJSONWithEP(setupIdf), translateIdfToEPJSONWithOS(setupIdf)};
}

void epJSONFixture::compareEPJSONTranslations(const std::string& idfname) {
  const auto [epTranslation, osTranslation] = epJSONFixture::doEPJSONTranslations(idfname);
  ASSERT_TRUE(epTranslation) << "E+ translation failed for " << idfname;
  ASSERT_TRUE(osTranslation) << "OS translation failed for " << idfname;

  compareJSONS(epTranslation, osTranslation);
}

// static variables
boost::optional<openstudio::FileLogSink> epJSONFixture::logFile;
