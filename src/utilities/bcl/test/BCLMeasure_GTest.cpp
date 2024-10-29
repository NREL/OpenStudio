/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "BCLFixture.hpp"
#include "../BCLMeasure.hpp"
#include "../BCLFileReference.hpp"
#include "../BCLXML.hpp"
#include "../../core/ApplicationPathHelpers.hpp"
#include "../../core/PathHelpers.hpp"
#include "../../core/Filesystem.hpp"

#include <pugixml.hpp>

#include <algorithm>
#include <sstream>

using namespace openstudio;
namespace fs = openstudio::filesystem;

struct TestFile
{
  std::string fileName;
  std::string usageType;
  openstudio::path relativePath;
};

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
  EXPECT_EQ(MeasureLanguage::Ruby, measure->measureLanguage().value());
  EXPECT_TRUE(measure->primaryRubyScriptPath());
  EXPECT_EQ("Envelope.Fenestration", measure->taxonomyTag());

  EXPECT_EQ(6u, measure->files().size());

  std::vector<TestFile> expectedFiles{
    {"measure.rb", "script", openstudio::toPath("measure.rb")},
    {"SetWindowToWallRatioByFacade_Test.rb", "test", openstudio::toPath("tests") / openstudio::toPath("SetWindowToWallRatioByFacade_Test.rb")},
    {"EnvelopeAndLoadTestModel_01.osm", "test", openstudio::toPath("tests") / openstudio::toPath("EnvelopeAndLoadTestModel_01.osm")},
    {"EnvelopeAndLoadTestModel_02_Rotated.osm", "test", openstudio::toPath("tests") / openstudio::toPath("EnvelopeAndLoadTestModel_02_Rotated.osm")},
    {"ReverseTranslatedModel.osm", "test", openstudio::toPath("tests") / openstudio::toPath("ReverseTranslatedModel.osm")},
    {"test.osm", "test", openstudio::toPath("tests") / openstudio::toPath("test.osm")},
  };

  for (BCLFileReference& file : measure->files()) {
    EXPECT_TRUE(exists(file.path()));
    EXPECT_FALSE(file.checkForUpdate());
    auto it =
      std::find_if(expectedFiles.cbegin(), expectedFiles.cend(), [&file](const auto& testFile) { return file.fileName() == testFile.fileName; });
    ASSERT_NE(it, expectedFiles.cend()) << "Can't find " << file.fileName();
    EXPECT_EQ(file.usageType(), it->usageType);
    EXPECT_EQ(file.relativePath(), it->relativePath);
  }

  // Test Clone
  {
    openstudio::path dir2 = resourcesPath() / toPath("/utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade2/");
    openstudio::path dir2clean = dir2;
    dir2clean.remove_trailing_separator();

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

    // Trailing separators differ...
    EXPECT_NE(dir2, measure2->directory());
    EXPECT_EQ(dir2clean, measure2->directory());
    EXPECT_TRUE(openstudio::filesystem::is_directory(dir2));
    EXPECT_TRUE(openstudio::filesystem::is_directory(dir2clean));

    EXPECT_EQ(6u, measure2->files().size()) << [&measure2]() {
      std::stringstream ss;
      for (const auto& f : measure2->files()) {
        ss << "filename=" << f.fileName() << ", path=" << f.path() << '\n';
      }
      return ss.str();
    }();

    for (BCLFileReference& file : measure2->files()) {
      EXPECT_TRUE(exists(file.path()));
      EXPECT_FALSE(file.checkForUpdate());
      auto it =
        std::find_if(expectedFiles.cbegin(), expectedFiles.cend(), [&file](const auto& testFile) { return file.fileName() == testFile.fileName; });
      ASSERT_NE(it, expectedFiles.cend());
      EXPECT_EQ(file.usageType(), it->usageType);
      EXPECT_EQ(file.relativePath(), it->relativePath);
    }

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
    EXPECT_EQ(6u, measure2->files().size()) << [&measure2]() {
      std::stringstream ss;
      for (const auto& f : measure2->files()) {
        ss << "filename=" << f.fileName() << ", path=" << f.path() << '\n';
      }
      return ss.str();
    }();

    for (BCLFileReference& file : measure2->files()) {
      EXPECT_TRUE(exists(file.path()));
      EXPECT_FALSE(file.checkForUpdate());
      auto it =
        std::find_if(expectedFiles.cbegin(), expectedFiles.cend(), [&file](const auto& testFile) { return file.fileName() == testFile.fileName; });
      ASSERT_NE(it, expectedFiles.cend());
      EXPECT_EQ(file.usageType(), it->usageType);
      EXPECT_EQ(file.relativePath(), it->relativePath);
    }

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
  }
}

TEST_F(BCLFixture, BCLMeasure_NewReportingMeasure) {

  openstudio::path dir = resourcesPath() / toPath("/utilities/BCL/Measures/AnotherReportingMeasure/");

  // In case cleanup didn't happen in the previous run
  if (openstudio::filesystem::exists(dir)) {
    ASSERT_TRUE(removeDirectory(dir));
  }

  boost::optional<BCLMeasure> measure;

  std::string className = BCLMeasure::makeClassName("Another Measure");
  EXPECT_EQ("AnotherMeasure", className);

  measure =
    BCLMeasure("Another Measure", className, dir, "Envelope.Fenestration", MeasureType::ReportingMeasure, "Description", "Modeler Description");
  ASSERT_TRUE(measure);
  ASSERT_TRUE(exists(dir));
  EXPECT_EQ("another_measure", measure->name());
  EXPECT_EQ("Another Measure", measure->displayName());
  EXPECT_EQ("AnotherMeasure", measure->className());
  EXPECT_EQ(MeasureType::ReportingMeasure, measure->measureType().value());
  EXPECT_TRUE(measure->primaryRubyScriptPath());
  EXPECT_EQ("Envelope.Fenestration", measure->taxonomyTag());

  EXPECT_EQ(8u, measure->files().size());

  std::vector<TestFile> expectedFiles{
    {"measure.rb", "script", openstudio::toPath("measure.rb")},
    {"LICENSE.md", "license", openstudio::toPath("LICENSE.md")},
    {"README.md.erb", "readmeerb", openstudio::toPath("README.md.erb")},
    {".gitkeep", "doc", openstudio::toPath("docs") / openstudio::toPath(".gitkeep")},
    {"another_measure_test.rb", "test", openstudio::toPath("tests") / openstudio::toPath("another_measure_test.rb")},
    {"example_model.osm", "test", openstudio::toPath("tests") / openstudio::toPath("example_model.osm")},
    {"USA_CO_Golden-NREL.724666_TMY3.epw", "test", openstudio::toPath("tests") / openstudio::toPath("USA_CO_Golden-NREL.724666_TMY3.epw")},
    {"report.html.in", "resource", openstudio::toPath("resources") / openstudio::toPath("report.html.in")},
  };

  for (BCLFileReference& file : measure->files()) {
    EXPECT_TRUE(exists(file.path()));
    EXPECT_FALSE(file.checkForUpdate());
    auto it =
      std::find_if(expectedFiles.cbegin(), expectedFiles.cend(), [&file](const auto& testFile) { return file.fileName() == testFile.fileName; });
    ASSERT_NE(it, expectedFiles.cend());
    EXPECT_EQ(file.usageType(), it->usageType);
    EXPECT_EQ(file.relativePath(), it->relativePath);
  }

  // Reload
  measure = BCLMeasure::load(dir);
  ASSERT_TRUE(measure);
  EXPECT_FALSE(measure->checkForUpdatesFiles());
  EXPECT_TRUE(measure->checkForUpdatesXML());  // Checksum is reset in Ctor (to 00000000) to trigger update even if nothing has changed
  ASSERT_TRUE(measure->primaryRubyScriptPath());

  for (BCLFileReference& file : measure->files()) {
    EXPECT_TRUE(exists(file.path()));
    EXPECT_FALSE(file.checkForUpdate());
    auto it =
      std::find_if(expectedFiles.cbegin(), expectedFiles.cend(), [&file](const auto& testFile) { return file.fileName() == testFile.fileName; });
    ASSERT_NE(it, expectedFiles.cend());
    EXPECT_EQ(file.usageType(), it->usageType);
    EXPECT_EQ(file.relativePath(), it->relativePath);
  }

  // Cleanup
  measure.reset();
  ASSERT_TRUE(exists(dir));
  ASSERT_TRUE(removeDirectory(dir));
  ASSERT_FALSE(exists(dir));
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

size_t createTestMeasureDirectory(const fs::path& srcDir, const std::vector<TestPath>& testPaths) {

  size_t added = 0;

  auto absolutePath = fs::weakly_canonical(srcDir);
  // fs::remove_all(srcDir);
  fs::create_directories(absolutePath);

  for (const auto& testPath : testPaths) {
    fs::path p = fs::weakly_canonical(absolutePath / testPath.path);
    if (!fs::exists(p)) {
      fs::create_directories(p.parent_path());
      std::ofstream(p.string()) << p.string();
      if (testPath.allowed) {
        ++added;
      }
    }
  }
  return added;
}

TEST_F(BCLFixture, isApprovedFile) {

  EXPECT_TRUE(BCLMeasure::isIgnoredFileName(".hello"));
  EXPECT_TRUE(BCLMeasure::isIgnoredFileName(""));
  EXPECT_TRUE(BCLMeasure::isIgnoredFileName("."));
  EXPECT_TRUE(BCLMeasure::isIgnoredFileName(".."));
  EXPECT_FALSE(BCLMeasure::isIgnoredFileName(".gitkeep"));
  EXPECT_FALSE(BCLMeasure::isIgnoredFileName("hello"));

  openstudio::path measureDir = "/usr/output/my_measure";

  std::vector<std::pair<openstudio::path, bool>> tests{
    {fs::path("measure.rb"), true},
    {fs::path("somethingelse.rb"), false},  // Non approved root file
    {fs::path("docs/hello.rb"), true},
    {fs::path("docs/subfolder/hello.rb"), true},
    {fs::path("tests/hello.rb"), true},
    {fs::path("tests/subfolder/hello.rb"), true},
    {fs::path("tests/output/hello.rb"), false},  // excluded subfolder
    {fs::path("resources/hello.rb"), true},
    {fs::path("resources/subfolder/hello.rb"), true},
    {fs::path("resources/output/hello.rb"), true},

    // Not an approved subfolder
    {fs::path("non_approved_subfolder/hello.rb"), false},
    {fs::path("non_approved_subfolder/subfolder/hello.rb"), false},
    {fs::path("non_approved_subfolder/output/hello.rb"), false},
  };

  for (const auto& [relativeFilePath, expectedAllowed] : tests) {
    openstudio::path absoluteFilePath = measureDir / relativeFilePath;
    EXPECT_EQ(expectedAllowed, BCLMeasure::isApprovedFile(absoluteFilePath, measureDir));
  }
}

TEST_F(BCLFixture, 4156_TestRecursive) {

  /***************************************************************************************************************************************************
   *                                               C R E A T E    A   T E M P L A T E    M E A S U R E                                               *
   **************************************************************************************************************************************************/

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
  auto files = measure->files();
  size_t numFiles = files.size();

  // .
  // ├── ./docs
  // │   └── ./docs/.gitkeep
  // ├── ./LICENSE.md
  // ├── ./measure.rb
  // ├── ./measure.xml
  // ├── ./README.md.erb
  // └── ./tests
  //     ├── ./tests/test_recursive_measure_test.rb
  //     └── ./tests/example_model.osm
  std::vector<fs::path> expectedInitialPaths = {
    "docs/.gitkeep", "LICENSE.md", "measure.rb", "README.md.erb", "tests/test_recursive_measure_test.rb", "tests/example_model.osm",
    // "measure.xml": it's not included in itself!
  };

  {

    EXPECT_EQ(numFiles, expectedInitialPaths.size());
    std::vector<fs::path> expectedInitialAbsolutePaths;
    expectedInitialAbsolutePaths.reserve(expectedInitialPaths.size());
    std::transform(expectedInitialPaths.cbegin(), expectedInitialPaths.cend(), std::back_inserter(expectedInitialAbsolutePaths),
                   [&srcDir](const auto& p) { return srcDir / p; });
    std::sort(expectedInitialAbsolutePaths.begin(), expectedInitialAbsolutePaths.end());

    std::vector<fs::path> initialPaths;
    initialPaths.reserve(files.size());
    std::transform(files.cbegin(), files.cend(), std::back_inserter(initialPaths), [](const auto& fileRef) { return fileRef.path(); });
    std::sort(initialPaths.begin(), initialPaths.end());

    EXPECT_TRUE(std::equal(initialPaths.begin(), initialPaths.end(), expectedInitialAbsolutePaths.begin(), expectedInitialAbsolutePaths.end()));
  }

  std::vector<TestPath> testPaths = generateTestMeasurePaths();
  EXPECT_EQ(15, testPaths.size());

  size_t addedFiles = std::count_if(testPaths.cbegin(), testPaths.cend(), [&expectedInitialPaths](const TestPath& t) {
    return t.allowed && (std::find(expectedInitialPaths.cbegin(), expectedInitialPaths.cend(), t.path) == expectedInitialPaths.cend());
  });

  /***************************************************************************************************************************************************
   *                                             A D D    E X T R A    F I L E S    I N    F O L D E R                                               *
   **************************************************************************************************************************************************/

  // This will add a few files if not existing, including some that shouldn't be allowed
  size_t added = createTestMeasureDirectory(srcDir, testPaths);

  EXPECT_EQ(addedFiles, added);

  // Trigger update of the BCLXML via scanning of the new files
  EXPECT_TRUE(measure->checkForUpdatesFiles());
  EXPECT_TRUE(measure->checkForUpdatesXML());
  files = measure->files();
  EXPECT_EQ(numFiles + addedFiles, files.size());

  // .
  // ├── ./docs
  //+│   ├── ./docs/docs.rb
  // │   ├── ./docs/.gitkeep
  // │   └── ./docs/subfolder
  //+│       └── ./docs/subfolder/subfolder_file.txt
  // ├── ./.git
  //~│   └── ./.git/index
  // ├── ./.hidden_folder
  //~│   └── ./.hidden_folder/file.txt
  // ├── ./LICENSE.md
  // ├── ./measure.rb
  // ├── ./measure.xml
  //+├── ./README.md
  // ├── ./README.md.erb
  // ├── ./resources
  //+│   ├── ./resources/resources.rb
  // │   └── ./resources/subfolder
  //+│       └── ./resources/subfolder/subfolder_file.txt
  //~├── ./root_file.txt
  // ├── ./subfolder
  //~│   └── ./subfolder/subfolder.txt
  // └── ./tests
  //     ├── ./tests/example_model.osm
  //     ├── ./tests/output
  //~    │   └── ./tests/output/output.txt
  //     ├── ./tests/subfolder
  //+    │   └── ./tests/subfolder/subfolder_file.txt
  //     ├── ./tests/test_recursive_measure_test.rb
  //+    └── ./tests/tests.rb
  //
  //  10 directories, 19 files:   12 have been added on disk (+/~), but only 7 are allowed (+ is allowed, ~ is disallowed)

  EXPECT_EQ(addedFiles, 7);

  std::vector<fs::path> newXMLPaths;
  newXMLPaths.reserve(files.size());
  std::transform(files.cbegin(), files.cend(), std::back_inserter(newXMLPaths), [](const auto& fileRef) { return fileRef.path(); });
  std::sort(newXMLPaths.begin(), newXMLPaths.end());

  std::vector<fs::path> expectedAfterNewFilesPaths = {
    "LICENSE.md",
    "README.md",
    "README.md.erb",
    "measure.rb",
    "docs/.gitkeep",
    "docs/docs.rb",
    "docs/subfolder/subfolder_file.txt",
    "resources/resources.rb",
    "resources/subfolder/subfolder_file.txt",
    "tests/example_model.osm",
    "tests/subfolder/subfolder_file.txt",
    "tests/test_recursive_measure_test.rb",
    "tests/tests.rb",
  };
  EXPECT_EQ(numFiles + addedFiles, expectedAfterNewFilesPaths.size());
  std::vector<fs::path> expectedAfterNewFilesAbsolutePaths;
  expectedAfterNewFilesAbsolutePaths.reserve(expectedAfterNewFilesPaths.size());
  std::transform(expectedAfterNewFilesPaths.cbegin(), expectedAfterNewFilesPaths.cend(), std::back_inserter(expectedAfterNewFilesAbsolutePaths),
                 [&srcDir](const auto& p) { return srcDir / p; });
  std::sort(expectedAfterNewFilesAbsolutePaths.begin(), expectedAfterNewFilesAbsolutePaths.end());

  EXPECT_TRUE(
    std::equal(newXMLPaths.begin(), newXMLPaths.end(), expectedAfterNewFilesAbsolutePaths.begin(), expectedAfterNewFilesAbsolutePaths.end()));

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

  /***************************************************************************************************************************************************
   *                                                                    C L O N E                                                                    *
   **************************************************************************************************************************************************/

  boost::optional<BCLMeasure> measure2 = measure->clone(destDir);
  ASSERT_TRUE(measure2);
  // I do expect this one to be true, since we do not copy the docs/ subdirectory during clone
  EXPECT_TRUE(measure2->checkForUpdatesFiles());
  // And I do expect this one to be true, because we copied the measure.xml without doing measure->save() first so it's outdated
  EXPECT_TRUE(measure2->checkForUpdatesXML());

  // EXPECT_TRUE(copyDir(srcDir, destDir));
  files = measure2->files();
  size_t nClonedFiles = std::count_if(expectedAfterNewFilesPaths.begin(), expectedAfterNewFilesPaths.end(),
                                      [](const openstudio::path& p) { return *(p.begin()) != "docs"; });
  EXPECT_EQ(nClonedFiles, numFiles + addedFiles - 3);
  EXPECT_EQ(nClonedFiles, files.size());

  {
    std::vector<fs::path> missing;
    std::vector<fs::path> extra;
    for (const auto& testPath : testPaths) {
      fs::path p = fs::weakly_canonical(destDir / testPath.path);
      if (testPath.allowed) {
        // EXPECT_TRUE(fs::exists(p)) << p << " doesn't exist in the cloned Dir";
        if (!fs::exists(p)) {
          if (*(testPath.path.begin()) != "docs") {
            missing.push_back(p);
          }
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

  // Save, so we can inspect the measure.xml
  measure->save();
  measure2->save();

  /**************************************************************************************************************************************************
  *                                                         P U G I    X M L    C H E C K                                                          *
  **************************************************************************************************************************************************/
  {
    pugi::xml_document bclXML;
    auto xmlPath = srcDir / toPath("measure.xml");
    openstudio::filesystem::ifstream file(xmlPath);
    ASSERT_TRUE(file.is_open());
    ASSERT_TRUE(bclXML.load(file));
    auto measureElement = bclXML.child("measure");
    auto xmlFilesElement = measureElement.child("files");
    std::vector<fs::path> xmlFilePaths;
    for (auto& fileElement : xmlFilesElement.children("file")) {
      if (!fileElement.first_child().empty()) {
        // Filename is a relative path, and if usageType is doc/resource/test it omits the subdirectory
        std::string fileName = fileElement.child("filename").text().as_string();
        std::string usageType = fileElement.child("usage_type").text().as_string();
        fs::path baseDir;
        if (usageType == "doc") {
          baseDir /= "docs";
        } else if (usageType == "resource") {
          baseDir /= "resources";
        } else if (usageType == "test") {
          baseDir /= "tests";
        }
        xmlFilePaths.emplace_back(srcDir / baseDir / toPath(fileName));
      }
    }

    std::sort(xmlFilePaths.begin(), xmlFilePaths.end());

    EXPECT_TRUE(
      std::equal(xmlFilePaths.begin(), xmlFilePaths.end(), expectedAfterNewFilesAbsolutePaths.begin(), expectedAfterNewFilesAbsolutePaths.end()));
  }
}

TEST_F(BCLFixture, 4156_TweakXML) {

  /**************************************************************************************************************************************************
  *                                      T E S T    W I T H    " M A N U A L L Y "    T W E A K E D    X M L                                       *
  **************************************************************************************************************************************************/

  openstudio::path testDir = openstudio::filesystem::system_complete(getApplicationBuildDirectory() / toPath("Testing"));
  openstudio::path srcDir = testDir / toPath("TestMeasureTweakXML");

  if (exists(srcDir)) {
    removeDirectory(srcDir);
  }
  ASSERT_FALSE(fs::exists(srcDir));

  try {
    BCLMeasure measure("Test Recursive Measure", "TestRecursiveMeasure", srcDir, "Envelope.Fenestration", MeasureType::ModelMeasure, "Description",
                       "Modeler Description");
  } catch (const std::exception& e) {
    LOG_FREE(Error, "BCLFixture", "exception during measure creation: " << e.what());
    ASSERT_TRUE(false);
  }
  ASSERT_TRUE(exists(srcDir));

  std::vector<fs::path> expectedInitialPaths = {
    "docs/.gitkeep", "LICENSE.md", "measure.rb", "README.md.erb", "tests/test_recursive_measure_test.rb", "tests/example_model.osm",
    // "measure.xml": it's not included in itself!
  };
  std::sort(expectedInitialPaths.begin(), expectedInitialPaths.end());

  /**************************************************************************************************************************************************
  *                                                         P U G I    X M L    C H E C K                                                          *
  **************************************************************************************************************************************************/
  {
    pugi::xml_document bclXML;
    auto xmlPath = srcDir / toPath("measure.xml");
    openstudio::filesystem::ifstream file(xmlPath);
    ASSERT_TRUE(file.is_open());
    ASSERT_TRUE(bclXML.load(file));
    auto measureElement = bclXML.child("measure");
    auto xmlFilesElement = measureElement.child("files");
    std::vector<fs::path> xmlRelativeFilePaths;
    for (auto& fileElement : xmlFilesElement.children("file")) {
      if (!fileElement.first_child().empty()) {
        // Filename is a relative path, and if usageType is doc/resource/test it omits the subdirectory. We check that
        std::string fileName = fileElement.child("filename").text().as_string();
        std::string usageType = fileElement.child("usage_type").text().as_string();
        fs::path baseDir;
        if (usageType == "doc") {
          baseDir /= "docs";
        } else if (usageType == "resource") {
          baseDir /= "resources";
        } else if (usageType == "test") {
          baseDir /= "tests";
        }
        xmlRelativeFilePaths.emplace_back(baseDir / toPath(fileName));
      }
    }

    std::sort(xmlRelativeFilePaths.begin(), xmlRelativeFilePaths.end());

    EXPECT_TRUE(std::equal(expectedInitialPaths.begin(), expectedInitialPaths.end(), xmlRelativeFilePaths.begin(), xmlRelativeFilePaths.end()));
  }

  // Load the BCLXML manually
  auto xmlPath = srcDir / toPath("measure.xml");
  BCLXML bclXML(xmlPath);
  size_t numFiles = bclXML.files().size();
  EXPECT_EQ(6, numFiles);

  size_t addedUntrackedFiles = 0;
  std::vector<fs::path> relativeFilePaths = {"root_file.rb", "untracked_subfolder/subfolder_file.rb", "tests/output/test_report.html"};

  for (const openstudio::path& relativeFilePath : relativeFilePaths) {
    auto absoluteFilePath = srcDir / relativeFilePath;
    fs::create_directories(absoluteFilePath.parent_path());
    std::ofstream(absoluteFilePath.string()) << absoluteFilePath.string();
    BCLFileReference fileRef(srcDir, relativeFilePath, true);
    fileRef.setUsageType("script");
    bclXML.addFile(fileRef);
    ++addedUntrackedFiles;
    EXPECT_EQ(numFiles + addedUntrackedFiles, bclXML.files().size());
  }

  // Save the XML
  bclXML.save();

  // Now load back the measure.
  boost::optional<BCLMeasure> measure = BCLMeasure::load(srcDir);
  ASSERT_TRUE(measure);
  EXPECT_EQ(numFiles + addedUntrackedFiles, measure->files().size());
  EXPECT_EQ(MeasureLanguage(MeasureLanguage::Ruby), measure->measureLanguage());

  // Call CheckUpdateFiles(), it should detect the files that aren't allowed
  EXPECT_TRUE(measure->checkForUpdatesFiles());
  EXPECT_TRUE(measure->checkForUpdatesXML());

  // number of files is back at original
  auto files = measure->files();
  EXPECT_EQ(numFiles, files.size());

  // We check that they are what we expect
  {

    EXPECT_EQ(numFiles, expectedInitialPaths.size());
    std::vector<fs::path> expectedInitialAbsolutePaths;
    expectedInitialAbsolutePaths.reserve(expectedInitialPaths.size());
    std::transform(expectedInitialPaths.cbegin(), expectedInitialPaths.cend(), std::back_inserter(expectedInitialAbsolutePaths),
                   [&srcDir](const auto& p) { return srcDir / p; });
    std::sort(expectedInitialAbsolutePaths.begin(), expectedInitialAbsolutePaths.end());

    std::vector<fs::path> initialPaths;
    initialPaths.reserve(files.size());
    std::transform(files.cbegin(), files.cend(), std::back_inserter(initialPaths), [](const auto& fileRef) { return fileRef.path(); });
    std::sort(initialPaths.begin(), initialPaths.end());

    EXPECT_TRUE(std::equal(initialPaths.begin(), initialPaths.end(), expectedInitialAbsolutePaths.begin(), expectedInitialAbsolutePaths.end()));
  }

  // We save the measure so we can inspect the XML
  measure->save();
}

TEST_F(BCLFixture, 4156_TestRecursive_OutdatedXML) {

  /***************************************************************************************************************************************************
   *                                               C R E A T E    A   T E M P L A T E    M E A S U R E                                               *
   **************************************************************************************************************************************************/

  openstudio::path testDir = openstudio::filesystem::system_complete(getApplicationBuildDirectory() / toPath("Testing"));
  openstudio::path srcDir = testDir / toPath("TestMeasureRecursiveOutdatedXML");
  openstudio::path destDir = testDir / toPath("TestMeasureRecursiveOutdatedXMLClone");

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
  auto files = measure->files();
  size_t numFiles = files.size();

  std::vector<TestPath> testPaths = generateTestMeasurePaths();
  EXPECT_EQ(15, testPaths.size());

  /***************************************************************************************************************************************************
   *                                             A D D    E X T R A    F I L E S    I N    F O L D E R                                               *
   **************************************************************************************************************************************************/

  // This will add a few files if not existing, including some that shouldn't be allowed
  size_t added = createTestMeasureDirectory(srcDir, testPaths);

  // DO NOT CALL CHECKFORUPDATESFILES: XML IS OUTDATED
  // EXPECT_TRUE(measure->checkForUpdatesFiles());
  // EXPECT_TRUE(measure->checkForUpdatesXML());

  EXPECT_EQ(added, 7);

  std::vector<fs::path> expectedAfterNewFilesPaths = {
    "LICENSE.md",
    "README.md",
    "README.md.erb",
    "measure.rb",
    "docs/.gitkeep",
    "docs/docs.rb",
    "docs/subfolder/subfolder_file.txt",
    "resources/resources.rb",
    "resources/subfolder/subfolder_file.txt",
    "tests/example_model.osm",
    "tests/subfolder/subfolder_file.txt",
    "tests/test_recursive_measure_test.rb",
    "tests/tests.rb",
  };
  EXPECT_EQ(numFiles + added, expectedAfterNewFilesPaths.size());

  /**************************************************************************************************************************************************
  *                                            C L O N E    W I T H    A N    O U T D A T E D    X M L                                             *
  **************************************************************************************************************************************************/
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

  boost::optional<BCLMeasure> measure2 = measure->clone(destDir);
  ASSERT_TRUE(measure2);
  // I do expect this one to be true, since we do not copy the docs/ subdirectory during clone
  EXPECT_TRUE(measure2->checkForUpdatesFiles());
  // And I do expect this one to be true, because we copied the measure.xml without doing measure->save() first so it's outdated
  EXPECT_TRUE(measure2->checkForUpdatesXML());

  // EXPECT_TRUE(copyDir(srcDir, destDir));
  files = measure2->files();
  size_t nClonedFiles = std::count_if(expectedAfterNewFilesPaths.begin(), expectedAfterNewFilesPaths.end(),
                                      [](const openstudio::path& p) { return *(p.begin()) != "docs"; });
  EXPECT_EQ(nClonedFiles, numFiles + added - 3);
  EXPECT_EQ(nClonedFiles, files.size());

  {
    std::vector<fs::path> missing;
    std::vector<fs::path> extra;
    for (const auto& testPath : testPaths) {
      fs::path p = fs::weakly_canonical(destDir / testPath.path);
      if (testPath.allowed) {
        // EXPECT_TRUE(fs::exists(p)) << p << " doesn't exist in the cloned Dir";
        if (!fs::exists(p)) {
          if (*(testPath.path.begin()) != "docs") {
            missing.push_back(p);
          }
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

  // Save, so we can inspect the measure.xml
  measure2->save();
}

TEST_F(BCLFixture, BCLMeasure_Ctor_PythonModelMeasure) {

  openstudio::path testDir = openstudio::filesystem::system_complete(getApplicationBuildDirectory() / toPath("Testing"));
  openstudio::path srcDir = testDir / toPath("TestPythonModelMeasure");
  openstudio::path destDir = testDir / toPath("TestPythonModelMeasureClone");

  for (const auto& dir : {srcDir, destDir}) {
    if (exists(dir)) {
      removeDirectory(dir);
    }
    ASSERT_FALSE(fs::exists(dir));
  }

  BCLMeasure{"My Python Measure",   "MyPythonMeasure",      srcDir, "Envelope.Fenestration", MeasureType::ModelMeasure, "Description",
             "Modeler Description", MeasureLanguage::Python};
  ASSERT_TRUE(exists(srcDir));

  boost::optional<BCLMeasure> measure = BCLMeasure::load(srcDir);
  ASSERT_TRUE(measure);
  EXPECT_EQ(MeasureType(MeasureType::ModelMeasure), measure->measureType());
  EXPECT_EQ(MeasureLanguage(MeasureLanguage::Python), measure->measureLanguage());
  auto files = measure->files();
  size_t numFiles = files.size();

  // .
  // ├── ./docs
  // │   └── ./docs/.gitkeep
  // ├── ./LICENSE.md
  // ├── ./measure.py
  // ├── ./measure.xml
  // └── ./tests
  //     ├── ./tests/test_my_python_measure.py
  //     └── ./tests/example_model.osm
  std::vector<fs::path> expectedInitialPaths = {
    "docs/.gitkeep", "LICENSE.md", "measure.py", "tests/test_my_python_measure.py", "tests/example_model.osm",
    // "measure.xml": it's not included in itself!
  };

  {

    EXPECT_EQ(numFiles, expectedInitialPaths.size());
    std::vector<fs::path> expectedInitialAbsolutePaths;
    expectedInitialAbsolutePaths.reserve(expectedInitialPaths.size());
    std::transform(expectedInitialPaths.cbegin(), expectedInitialPaths.cend(), std::back_inserter(expectedInitialAbsolutePaths),
                   [&srcDir](const auto& p) { return srcDir / p; });
    std::sort(expectedInitialAbsolutePaths.begin(), expectedInitialAbsolutePaths.end());

    std::vector<fs::path> initialPaths;
    initialPaths.reserve(files.size());
    std::transform(files.cbegin(), files.cend(), std::back_inserter(initialPaths), [](const auto& fileRef) { return fileRef.path(); });
    std::sort(initialPaths.begin(), initialPaths.end());

    EXPECT_TRUE(std::equal(initialPaths.begin(), initialPaths.end(), expectedInitialAbsolutePaths.begin(), expectedInitialAbsolutePaths.end()));
  }
}

TEST_F(BCLFixture, BCLMeasure_Ctor_PythonEnergyPlusMeasure) {

  openstudio::path testDir = openstudio::filesystem::system_complete(getApplicationBuildDirectory() / toPath("Testing"));
  openstudio::path srcDir = testDir / toPath("TestPythonEnergyPlusMeasure");
  openstudio::path destDir = testDir / toPath("TestPythonEnergyPlusMeasureClone");

  for (const auto& dir : {srcDir, destDir}) {
    if (exists(dir)) {
      removeDirectory(dir);
    }
    ASSERT_FALSE(fs::exists(dir));
  }

  BCLMeasure{"My Python Measure",   "MyPythonMeasure",      srcDir, "Envelope.Fenestration", MeasureType::EnergyPlusMeasure, "Description",
             "Modeler Description", MeasureLanguage::Python};
  ASSERT_TRUE(exists(srcDir));

  boost::optional<BCLMeasure> measure = BCLMeasure::load(srcDir);
  ASSERT_TRUE(measure);
  EXPECT_EQ(MeasureType(MeasureType::EnergyPlusMeasure), measure->measureType());
  EXPECT_EQ(MeasureLanguage(MeasureLanguage::Python), measure->measureLanguage());
  auto files = measure->files();
  size_t numFiles = files.size();

  // .
  // ├── ./docs
  // │   └── ./docs/.gitkeep
  // ├── ./LICENSE.md
  // ├── ./measure.py
  // ├── ./measure.xml
  // └── ./tests
  //     └── ./tests/test_my_python_measure.py
  std::vector<fs::path> expectedInitialPaths = {
    "docs/.gitkeep", "LICENSE.md", "measure.py", "tests/test_my_python_measure.py",
    // "measure.xml": it's not included in itself!
  };

  {

    EXPECT_EQ(numFiles, expectedInitialPaths.size());
    std::vector<fs::path> expectedInitialAbsolutePaths;
    expectedInitialAbsolutePaths.reserve(expectedInitialPaths.size());
    std::transform(expectedInitialPaths.cbegin(), expectedInitialPaths.cend(), std::back_inserter(expectedInitialAbsolutePaths),
                   [&srcDir](const auto& p) { return srcDir / p; });
    std::sort(expectedInitialAbsolutePaths.begin(), expectedInitialAbsolutePaths.end());

    std::vector<fs::path> initialPaths;
    initialPaths.reserve(files.size());
    std::transform(files.cbegin(), files.cend(), std::back_inserter(initialPaths), [](const auto& fileRef) { return fileRef.path(); });
    std::sort(initialPaths.begin(), initialPaths.end());

    EXPECT_TRUE(std::equal(initialPaths.begin(), initialPaths.end(), expectedInitialAbsolutePaths.begin(), expectedInitialAbsolutePaths.end()));
  }
}

TEST_F(BCLFixture, BCLMeasure_Ctor_PythonReportingMeasure) {

  openstudio::path testDir = openstudio::filesystem::system_complete(getApplicationBuildDirectory() / toPath("Testing"));
  openstudio::path srcDir = testDir / toPath("TestPythonReportingMeasure");
  openstudio::path destDir = testDir / toPath("TestPythonReportingMeasureClone");

  for (const auto& dir : {srcDir, destDir}) {
    if (exists(dir)) {
      removeDirectory(dir);
    }
    ASSERT_FALSE(fs::exists(dir));
  }

  BCLMeasure{"My Python Measure",   "MyPythonMeasure",      srcDir, "Envelope.Fenestration", MeasureType::ReportingMeasure, "Description",
             "Modeler Description", MeasureLanguage::Python};
  ASSERT_TRUE(exists(srcDir));

  boost::optional<BCLMeasure> measure = BCLMeasure::load(srcDir);
  ASSERT_TRUE(measure);
  EXPECT_EQ(MeasureType(MeasureType::ReportingMeasure), measure->measureType());
  EXPECT_EQ(MeasureLanguage(MeasureLanguage::Python), measure->measureLanguage());
  auto files = measure->files();
  size_t numFiles = files.size();

  // .
  // ├── ./docs
  // │   └── ./docs/.gitkeep
  // ├── ./LICENSE.md
  // ├── ./measure.py
  // ├── ./measure.xml
  // ├── resources
  // └── ./tests
  //     ├── ./tests/test_my_python_measure.py
  //     ├── ./tests/example_model.osm
  //     └── USA_CO_Golden-NREL.724666_TMY3.epw
  std::vector<fs::path> expectedInitialPaths = {
    "docs/.gitkeep",
    "LICENSE.md",
    "measure.py",
    "tests/test_my_python_measure.py",
    "tests/example_model.osm",
    "tests/USA_CO_Golden-NREL.724666_TMY3.epw",
    // "measure.xml": it's not included in itself!
  };

  {

    EXPECT_EQ(numFiles, expectedInitialPaths.size());
    std::vector<fs::path> expectedInitialAbsolutePaths;
    expectedInitialAbsolutePaths.reserve(expectedInitialPaths.size());
    std::transform(expectedInitialPaths.cbegin(), expectedInitialPaths.cend(), std::back_inserter(expectedInitialAbsolutePaths),
                   [&srcDir](const auto& p) { return srcDir / p; });
    std::sort(expectedInitialAbsolutePaths.begin(), expectedInitialAbsolutePaths.end());

    std::vector<fs::path> initialPaths;
    initialPaths.reserve(files.size());
    std::transform(files.cbegin(), files.cend(), std::back_inserter(initialPaths), [](const auto& fileRef) { return fileRef.path(); });
    std::sort(initialPaths.begin(), initialPaths.end());

    EXPECT_TRUE(std::equal(initialPaths.begin(), initialPaths.end(), expectedInitialAbsolutePaths.begin(), expectedInitialAbsolutePaths.end()));
  }
}

TEST_F(BCLFixture, BCLMeasure_CTor_throw_invalid_xml) {

  openstudio::path testDir = openstudio::filesystem::system_complete(getApplicationBuildDirectory() / toPath("Testing"));
  openstudio::path srcDir = testDir / toPath("BCLMeasure_CTor_throw_invalid_xml");
  openstudio::path xmlPath = srcDir / toPath("measure.xml");

  if (exists(srcDir)) {
    removeDirectory(srcDir);
  }
  ASSERT_FALSE(fs::exists(srcDir));

  fs::create_directories(srcDir);
  ASSERT_TRUE(fs::exists(srcDir));

  BCLXML bclXML(BCLXMLType::MeasureXML);
  bclXML.setName("Dumb Measure");
  bclXML.saveAs(xmlPath);

  // Ensure the bclXML is minimally viable (has name for eg)
  EXPECT_TRUE(BCLXML::load(xmlPath));

  // Missing required "Measure Type"
  EXPECT_ANY_THROW(BCLMeasure{srcDir});
  std::string msg = logFile->logMessages().back().logMessage();
  EXPECT_TRUE(msg.find("is missing the required attribute \"Measure Type\"") != std::string::npos) << msg;

  // Missing a measure.rb/.py
  bclXML.addAttribute(Attribute("Measure Type", MeasureType(MeasureType::ModelMeasure).valueName()));
  bclXML.saveAs(xmlPath);
  EXPECT_ANY_THROW(BCLMeasure{srcDir});
  msg = logFile->logMessages().back().logMessage();
  EXPECT_TRUE(msg.find("has neither measure.rb nor measure.py") != std::string::npos) << logFile->logMessages().back().logMessage();

  // Add a measure.rb, all good
  BCLFileReference rubyFileref(srcDir, "measure.rb", true);
  rubyFileref.setUsageType("script");
  bclXML.addFile(rubyFileref);
  bclXML.saveAs(xmlPath);
  EXPECT_NO_THROW(BCLMeasure{srcDir});

  // if MeasureLanguage is set, we enforce it matches
  bclXML.addAttribute(Attribute("Measure Language", MeasureLanguage(MeasureLanguage::Python).valueName()));
  bclXML.saveAs(xmlPath);
  EXPECT_ANY_THROW(BCLMeasure{srcDir});
  msg = logFile->logMessages().back().logMessage();
  EXPECT_TRUE(msg.find("has a measure.rb; but \"Measure Language\" is not 'Ruby', it's 'Python'") != std::string::npos)
    << logFile->logMessages().back().logMessage();

  bclXML.removeAttributes("Measure Language");
  bclXML.addAttribute(Attribute("Measure Language", MeasureLanguage(MeasureLanguage::Ruby).valueName()));
  bclXML.saveAs(xmlPath);
  EXPECT_NO_THROW(BCLMeasure{srcDir});

  // We can't have both a measure.rb and measure.py
  BCLFileReference pythonFileref(srcDir, "measure.py", true);
  pythonFileref.setUsageType("script");
  bclXML.addFile(pythonFileref);
  bclXML.saveAs(xmlPath);
  EXPECT_ANY_THROW(BCLMeasure{srcDir});
  msg = logFile->logMessages().back().logMessage();
  EXPECT_TRUE(msg.find("has both measure.rb and measure.py, and they cannot be used at the same time") != std::string::npos)
    << logFile->logMessages().back().logMessage();

  // Now I only have measure.py. Enforce Measure Language matches
  bclXML.removeFile(rubyFileref.path());
  bclXML.saveAs(xmlPath);
  EXPECT_ANY_THROW(BCLMeasure{srcDir});
  msg = logFile->logMessages().back().logMessage();
  EXPECT_TRUE(msg.find("has a measure.py; but \"Measure Language\" is not 'Python', it's 'Ruby'") != std::string::npos)
    << logFile->logMessages().back().logMessage();

  bclXML.removeAttributes("Measure Language");
  bclXML.addAttribute(Attribute("Measure Language", MeasureLanguage(MeasureLanguage::Python).valueName()));
  bclXML.saveAs(xmlPath);
  EXPECT_NO_THROW(BCLMeasure{srcDir});

  // Can't have multiple copies of MeasureLanguage
  bclXML.addAttribute(Attribute("Measure Language", MeasureLanguage(MeasureLanguage::Ruby).valueName()));
  bclXML.saveAs(xmlPath);
  EXPECT_ANY_THROW(BCLMeasure{srcDir});
  msg = logFile->logMessages().back().logMessage();
  EXPECT_TRUE(msg.find("has multiple copies of required attribute \"Measure Language\"") != std::string::npos)
    << logFile->logMessages().back().logMessage();

  bclXML.removeAttributes("Measure Language");
  bclXML.saveAs(xmlPath);
  EXPECT_NO_THROW(BCLMeasure{srcDir});

  // Can't have a wrong type
  bclXML.removeAttributes("Measure Type");
  bclXML.addAttribute(Attribute("Measure Type", 10.0));
  bclXML.saveAs(xmlPath);
  EXPECT_ANY_THROW(BCLMeasure{srcDir});
  msg = logFile->logMessages().back().logMessage();
  EXPECT_TRUE(msg.find("has wrong type for required attribute \"Measure Type\"") != std::string::npos) << logFile->logMessages().back().logMessage();
}
