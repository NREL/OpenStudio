/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
  openstudio::filesystem::copy_file(location, idf_path, openstudio::filesystem::copy_option::overwrite_if_exists);
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
