/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <gtest/gtest.h>
#include "BCLFixture.hpp"
#include "../BCLMeasure.hpp"
#include "../BCLFileReference.hpp"
#include "../../core/ApplicationPathHelpers.hpp"
#include "../../core/PathHelpers.hpp"
#include "utilities/core/Filesystem.hpp"

#include <algorithm>

using namespace openstudio;

namespace fs = openstudio::filesystem;

TEST_F(BCLFixture, BCLMeasure) {
  openstudio::path dir = resourcesPath() / toPath("/utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade/");
  boost::optional<BCLMeasure> measure = BCLMeasure::load(dir);
  ASSERT_TRUE(measure);

  EXPECT_EQ("Set Window to Wall Ratio by Facade", measure->name());
  EXPECT_EQ("f347ae80-48b4-4c40-bfd4-6c5139b38136", measure->uid());
  EXPECT_EQ("a0e33012-0183-45a0-8898-11f93f55164e", measure->versionId());
  EXPECT_EQ("This measure will set the window to wall ratio for exterior surfaces with a specified orientation. If one or more windows exist on an "
            "affected wall, they will be removed and replaced with a single ribbon window. Doors will not be removed. If the requested ratio can't "
            "be achieved then the wall will remain untouched.",
            measure->description());
  EXPECT_EQ(
    "This measure identifies exterior surfaces of the proper orientation. Then it runs a method that removes existing windows and applies a new "
    "window with a specified window to wall ratio and sill height. The construction chosen for the new window is defaulted to what is assigned to "
    "the space, or inherited from a higher level object, such as the building. If the baseline model uses hard assigned constructions you may not "
    "get the expected results. The measure doesn't have any cost or lifecycle arguments, however if lifecycle objects exist for exterior wall and "
    "window constructions, then this measure will be able to calculate the economic impact of change in window to wall ratio.",
    measure->modelerDescription());

  EXPECT_EQ(MeasureType::ModelMeasure, measure->measureType().value());
  EXPECT_TRUE(measure->primaryRubyScriptPath());
  EXPECT_EQ("Envelope.Fenestration", measure->taxonomyTag());

  EXPECT_EQ(6u, measure->files().size());
  for (BCLFileReference file : measure->files()) {
    EXPECT_TRUE(exists(file.path()));
    EXPECT_FALSE(file.checkForUpdate());
  }

  openstudio::path dir2 = resourcesPath() / toPath("/utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade2/");
  if (openstudio::filesystem::exists(dir2)) {
    ASSERT_TRUE(removeDirectory(dir2));
  }
  // If this assertion fails, check that you don't have an Explorer window opened to the SetWindowToWallRatioByFacade2 directory
  ASSERT_FALSE(openstudio::filesystem::exists(dir2));

  boost::optional<BCLMeasure> measure2 = measure->clone(dir2);
  ASSERT_TRUE(measure2);
  EXPECT_FALSE(measure2->checkForUpdatesFiles());
  EXPECT_FALSE(measure2->checkForUpdatesXML());
  EXPECT_TRUE(*measure == *measure2);
  EXPECT_FALSE(measure->directory() == measure2->directory());
  EXPECT_TRUE(dir2 == measure2->directory());
  EXPECT_EQ(6u, measure2->files().size());

  measure2->setName("New Measure");  // this would normally be initiated by a change from the measure
  EXPECT_FALSE(measure2->checkForUpdatesFiles());
  EXPECT_FALSE(measure2->checkForUpdatesXML());  // name change does not trigger xml update
  EXPECT_FALSE(*measure == *measure2);
  measure2->save();
  EXPECT_FALSE(measure2->checkForUpdatesFiles());
  EXPECT_FALSE(measure2->checkForUpdatesXML());

  measure2 = BCLMeasure::load(dir2);
  ASSERT_TRUE(measure2);
  EXPECT_FALSE(measure2->checkForUpdatesFiles());
  EXPECT_FALSE(measure2->checkForUpdatesXML());
  EXPECT_FALSE(*measure == *measure2);
  EXPECT_EQ("New Measure", measure2->name());
  ASSERT_TRUE(measure2->primaryRubyScriptPath());
  EXPECT_EQ(6u, measure2->files().size());

  openstudio::filesystem::ofstream file(measure2->primaryRubyScriptPath().get());
  ASSERT_TRUE(file.is_open());
  file << "Hi";
  file.close();
  EXPECT_FALSE(measure2->checkForUpdatesXML());
  EXPECT_TRUE(measure2->checkForUpdatesFiles());

  measure2.reset();
  ASSERT_TRUE(exists(dir2));
  ASSERT_TRUE(removeDirectory(dir2));
  ASSERT_FALSE(exists(dir2));

  std::string className = BCLMeasure::makeClassName("Another Measure");
  EXPECT_EQ("AnotherMeasure", className);

  EXPECT_NO_THROW(measure2 = BCLMeasure("Another Measure", className, dir2, "Envelope.Fenestration", MeasureType::ReportingMeasure, "Description",
                                        "Modeler Description"));
  ASSERT_TRUE(measure2);
  ASSERT_TRUE(exists(dir2));
  EXPECT_EQ("another_measure", measure2->name());
  EXPECT_EQ("Another Measure", measure2->displayName());
  EXPECT_EQ("AnotherMeasure", measure2->className());
  EXPECT_TRUE(measure2->primaryRubyScriptPath());

  measure2 = BCLMeasure::load(dir2);
  ASSERT_TRUE(measure2);
  EXPECT_FALSE(measure2->checkForUpdatesFiles());
  EXPECT_TRUE(measure2->checkForUpdatesXML());
  ASSERT_TRUE(measure2->primaryRubyScriptPath());
}

TEST_F(BCLFixture, BCLMeasure_CTor) {

  openstudio::path dir = openstudio::filesystem::system_complete(getApplicationBuildDirectory() / toPath("Testing/TestMeasure/"));
  if (exists(dir)) {
    removeDirectory(dir);
  }
  ASSERT_FALSE(exists(dir));

  std::string className = BCLMeasure::makeClassName(" _ $ Test & Measure");
  EXPECT_EQ("ATestMeasure", className);

  std::string className2 = BCLMeasure::makeClassName("3 _ $ ");
  EXPECT_EQ("A3", className2);

  std::string className3 = BCLMeasure::makeClassName(" _ $ ");
  ASSERT_FALSE(className3.empty());
  EXPECT_EQ('A', className3[0]);

  try {
    BCLMeasure measure("Test Measure", className, dir, "Envelope.Fenestration", MeasureType::ModelMeasure, "Description", "Modeler Description");
  } catch (const std::exception& e) {
    LOG_FREE(Error, "BCLFixture", "exception during measure creation: " << e.what());
    ASSERT_TRUE(false);
  }
  ASSERT_TRUE(exists(dir));

  boost::optional<BCLMeasure> measure = BCLMeasure::load(dir);
  ASSERT_TRUE(measure);
}
/*
TEST_F(BCLFixture, PatApplicationMeasures)
{
  std::vector<BCLMeasure> patApplicationMeasures = BCLMeasure::patApplicationMeasures();
  ASSERT_EQ(5u, patApplicationMeasures.size());

  bool filesUpdated;
  bool xmlUpdated;

  BCLMeasure alternativeModelMeasure = BCLMeasure::alternativeModelMeasure();
  ASSERT_TRUE(alternativeModelMeasure.primaryRubyScriptPath());
  filesUpdated = alternativeModelMeasure.checkForUpdatesFiles();
  ASSERT_FALSE(filesUpdated); // DLM: comment out to update built in PAT measures
  xmlUpdated = alternativeModelMeasure.checkForUpdatesXML();
  ASSERT_FALSE(xmlUpdated); // DLM: comment out to update built in PAT measures
  if (filesUpdated || xmlUpdated){
    alternativeModelMeasure.save();
  }

  BCLMeasure reportRequestMeasure = BCLMeasure::reportRequestMeasure();
  ASSERT_TRUE(reportRequestMeasure.primaryRubyScriptPath());
  filesUpdated = reportRequestMeasure.checkForUpdatesFiles();
  ASSERT_FALSE(filesUpdated); // DLM: comment out to update built in PAT measures
  xmlUpdated = reportRequestMeasure.checkForUpdatesXML();
  ASSERT_FALSE(xmlUpdated); // DLM: comment out to update built in PAT measures
  if (filesUpdated || xmlUpdated){
    reportRequestMeasure.save();
  }

  BCLMeasure standardReportMeasure = BCLMeasure::standardReportMeasure();
  ASSERT_TRUE(standardReportMeasure.primaryRubyScriptPath());
  filesUpdated = standardReportMeasure.checkForUpdatesFiles();
  ASSERT_FALSE(filesUpdated); // DLM: comment out to update built in PAT measures
  xmlUpdated = standardReportMeasure.checkForUpdatesXML();
  ASSERT_FALSE(xmlUpdated); // DLM: comment out to update built in PAT measures
  if (filesUpdated || xmlUpdated){
    standardReportMeasure.save();
  }

  BCLMeasure calibrationReportMeasure = BCLMeasure::calibrationReportMeasure();
  ASSERT_TRUE(calibrationReportMeasure.primaryRubyScriptPath());
  filesUpdated = calibrationReportMeasure.checkForUpdatesFiles();
  ASSERT_FALSE(filesUpdated); // DLM: comment out to update built in PAT measures
  xmlUpdated = calibrationReportMeasure.checkForUpdatesXML();
  ASSERT_FALSE(xmlUpdated); // DLM: comment out to update built in PAT measures
  if (filesUpdated || xmlUpdated){
    calibrationReportMeasure.save();
  }

  BCLMeasure radianceMeasure = BCLMeasure::radianceMeasure();
  ASSERT_TRUE(radianceMeasure.primaryRubyScriptPath());
  filesUpdated = radianceMeasure.checkForUpdatesFiles();
  ASSERT_FALSE(filesUpdated); // DLM: comment out to update built in PAT measures
  xmlUpdated = radianceMeasure.checkForUpdatesXML();
  ASSERT_FALSE(xmlUpdated); // DLM: comment out to update built in PAT measures
  if (filesUpdated || xmlUpdated){
    radianceMeasure.save();
  }

}
*/

struct TestPath
{
  TestPath(fs::path t_path, bool t_allowed) : path(std::move(t_path)), allowed(t_allowed){};
  fs::path path;
  bool allowed;
};

std::vector<TestPath> generateTestMeasurePaths() {

  std::vector<TestPath> testPaths;

  std::vector<std::string> approvedRootFiles{
    "measure.rb", "README.md", "README.md.erb", "LICENSE.md",
    // ".gitkeep"      // assuming .gitkeep outside a subfolder makes zero sense...
    // "measure.xml"   // not included in itself!
  };

  std::vector<std::string> approved_folders{"docs", "resources", "tests"};

  std::vector<std::string> ignoredPaths{"root_file.txt", "tests/output/output.txt", "subfolder/subfolder.txt", ".git/index",
                                        ".hidden_folder/file.txt"};

  testPaths.reserve(approvedRootFiles.size() + 2 * approved_folders.size() + ignoredPaths.size());
  for (const auto& s : approvedRootFiles) {
    testPaths.emplace_back(fs::path{s}, true);
  }

  for (const auto& s : approved_folders) {
    fs::path file(s);
    file.replace_extension(".rb");
    testPaths.emplace_back(fs::path{s} / file, true);
    testPaths.emplace_back(fs::path(s) / "subfolder" / "subfolder_file.txt", true);
  }

  for (const auto& s : ignoredPaths) {
    testPaths.emplace_back(fs::path{s}, false);
  }

  return testPaths;
}

void createTestMeasureDirectory(const fs::path& srcDir, const std::vector<TestPath>& testPaths) {

  auto absolutePath = fs::weakly_canonical(srcDir);
  // fs::remove_all(srcDir);
  fs::create_directories(absolutePath);

  for (const auto& testPath : testPaths) {
    fs::path p = fs::weakly_canonical(absolutePath / testPath.path);
    if (!fs::exists(p)) {
      fs::create_directories(p.parent_path());
      std::ofstream(p.string()) << p.string();
    } else {
      std::cout << p << " already exists\n";
    }
  }
}

TEST_F(BCLFixture, 4156_TestRecursive) {

  openstudio::path testDir = openstudio::filesystem::system_complete(getApplicationBuildDirectory() / toPath("Testing"));
  openstudio::path srcDir = testDir / toPath("TestMeasureRecursive");
  openstudio::path destDir = testDir / toPath("TestMeasureRecursiveClone");

  for (const auto& dir : {srcDir, destDir}) {
    if (exists(dir)) {
      removeDirectory(dir);
    }
    ASSERT_FALSE(fs::exists(dir));
  }

  try {
    BCLMeasure measure("Test Recursive Measure", "TestRecursiveMeasure", srcDir, "Envelope.Fenestration", MeasureType::ModelMeasure, "Description",
                       "Modeler Description");
  } catch (const std::exception& e) {
    LOG_FREE(Error, "BCLFixture", "exception during measure creation: " << e.what());
    ASSERT_TRUE(false);
  }
  ASSERT_TRUE(exists(srcDir));

  boost::optional<BCLMeasure> measure = BCLMeasure::load(srcDir);
  ASSERT_TRUE(measure);
  size_t numFiles = measure->files().size();

  // │   └── ./docs/.gitkeep
  // ├── ./LICENSE.md
  // ├── ./measure.rb
  // ├── ./measure.xml
  // ├── ./README.md.erb
  // └── ./tests
  //     ├── ./tests/a_test_measure_test.rb
  //     └── ./tests/example_model.osm
  std::vector<fs::path> expectedInitialPaths = {
    "docs/.gitkeep", "LICENSE.md", "measure.rb", "README.md.erb", "tests/a_test_measure_test.rb", "tests/example_model.osm",
    // "measure.xml": it's not included in itself!
  };
  EXPECT_EQ(numFiles, expectedInitialPaths.size());

  std::vector<TestPath> testPaths = generateTestMeasurePaths();
  EXPECT_GT(testPaths.size(), 5);

  size_t addedFiles = std::count_if(testPaths.cbegin(), testPaths.cend(), [&expectedInitialPaths](const TestPath& t) {
    return t.allowed && (std::find(expectedInitialPaths.cbegin(), expectedInitialPaths.cend(), t.path) == expectedInitialPaths.cend());
  });

  // This will add a few files if not existing, including some that shouldn't be allowed
  createTestMeasureDirectory(srcDir, testPaths);

  // Trigger update of the BCLXML via scanning of the new files
  EXPECT_TRUE(measure->checkForUpdatesFiles());
  EXPECT_TRUE(measure->checkForUpdatesXML());
  auto files = measure->files();
  EXPECT_EQ(numFiles + 4, files.size());
  EXPECT_EQ(numFiles + addedFiles, files.size());

  auto checkError = [](const std::vector<fs::path>& paths, std::string_view headerEnd) {
    std::stringstream ss;
    if (!paths.empty()) {
      ss << "There are " << paths.size() << " " << headerEnd << ":\n";
      for (const auto& p : paths) {
        ss << "* " << p << '\n';
      }
    }
    EXPECT_TRUE(paths.empty()) << ss.str();
  };

  {
    std::vector<fs::path> missing;
    std::vector<fs::path> extra;
    for (const auto& testPath : testPaths) {
      const auto& p = testPath.path;
      if (std::find_if(files.cbegin(), files.cend(),
                       [&p, &srcDir](const BCLFileReference& fileRef) {
                         // fileRef.path() is an absolute path, while p is a relative one
                         return fileRef.path() == srcDir / p;
                       })
          == files.cend()) {
        // EXPECT_FALSE(testPath.allowed) << p << " doesn't exist in the XML when it should";
        if (testPath.allowed) {
          missing.push_back(p);
        }
      } else {
        // EXPECT_TRUE(testPath.allowed) << p << " shouldn't exist in the XML";
        if (!testPath.allowed) {
          extra.push_back(p);
        }
      }
    }
    checkError(missing, "missing files in the XML");
    checkError(extra, "extra files in the XML");
  }

  boost::optional<BCLMeasure> measure2 = measure->clone(destDir);
  ASSERT_TRUE(measure2);
  EXPECT_FALSE(measure2->checkForUpdatesFiles());
  EXPECT_FALSE(measure2->checkForUpdatesXML());

  // EXPECT_TRUE(copyDir(srcDir, destDir));
  files = measure2->files();
  EXPECT_EQ(numFiles + addedFiles, files.size());

  {
    std::vector<fs::path> missing;
    std::vector<fs::path> extra;
    for (const auto& testPath : testPaths) {
      fs::path p = fs::weakly_canonical(destDir / testPath.path);
      if (testPath.allowed) {
        // EXPECT_TRUE(fs::exists(p)) << p << " doesn't exist in the cloned Dir";
        if (!fs::exists(p)) {
          missing.push_back(p);
        }
      } else {
        // EXPECT_FALSE(fs::exists(p)) << p << " shouldn't exist in the cloned Dir";
        if (fs::exists(p)) {
          extra.push_back(p);
        }
      }
    }
    checkError(missing, "missing files in the cloned Dir");
    checkError(extra, "extra files in the cloned Dir");
  }
}

TEST_F(BCLFixture, TestIgnoredSubDirectoryLogic) {

  auto isInIgnoredSubDirectory = [](const fs::path& relativeFilePath, const fs::path& measureDir,
                                    const std::vector<fs::path>& ignoredSubFolders = {}) {
    fs::path srcItemPath = measureDir / relativeFilePath;
    auto parentPath = srcItemPath.parent_path();
    std::string filename = toString(relativeFilePath.filename());
    bool ignore = (filename.empty() || boost::starts_with(filename, "."));

    // This will check back up to the root (C:\ or /)... It's missing a condition `parentPath != srcDir`
    while (!ignore && !parentPath.empty()) {
      if (std::find_if(ignoredSubFolders.begin(), ignoredSubFolders.end(),
                       [&measureDir, &parentPath](const auto& subFolderPath) {
                         auto fullSubFolderPath = measureDir / subFolderPath;
                         return parentPath == fullSubFolderPath;
                       })
          != ignoredSubFolders.end()) {
        ignore = true;
        break;
      }
      parentPath = parentPath.parent_path();

      // I shouldn't go above the measure directory
      // fs::equivalent needs both path to exist, they don't here
      // EXPECT_FALSE(fs::equivalent(parentPath, measureDir / ".."));
      EXPECT_NE(parentPath, measureDir.parent_path().parent_path())
        << "For relativeFilePath = " << relativeFilePath << ", measureDir = " << measureDir;
    }

    return ignore;
  };

  {
    // relativeFilePath, measureDir, expectIgnored
    std::vector<std::tuple<fs::path, fs::path, bool>> tests = {{"tests/myfile.txt", "/usr/local/mymeasure", false},
                                                               {"tests/myfile.txt", "/usr/local/output/something/mymeasure", false},
                                                               {"tests/output/myfile.txt", "/usr/local/mymeasure", true},
                                                               {"docs/myfile.txt", "/usr/local/mymeasure", false}};

    std::vector<fs::path> ignoredSubFolders{"docs/", "tests/output"};

    for (auto& [relativeFilePath, measureDir, expectedIgnored] : tests) {
      EXPECT_EQ(expectedIgnored, isInIgnoredSubDirectory(relativeFilePath, measureDir, ignoredSubFolders));
    }
  }
}
