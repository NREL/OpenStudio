/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../ApplicationPathHelpers.hpp"
#include "../PathHelpers.hpp"
#include "../Logger.hpp"
#include <stdlib.h>
#include <thread>
#include <future>
#include <OpenStudio.hxx>

// TODO: GTEST 1.9 should have a GTEST_SKIP macro we could use
#define SKIP(TEST_NAME)                                                                                                                              \
  do {                                                                                                                                               \
    std::cout << "[  SKIPPED ] " << #TEST_NAME << ": symlink tests can only be run with administrator rights on windows (elevated cmd.exe)" << '\n'; \
    return;                                                                                                                                          \
  } while (0)

#if defined(_WIN32)
#  include <Windows.h>

// A wrapper to implement setenv on Windows like on Unix, using _putenv internally
int setenv(const char* name, const char* value, int overwrite) {
  if (!overwrite) {
    size_t envsize = 0;
    int errcode = getenv_s(&envsize, NULL, 0, name);
    if (errcode || envsize) return errcode;
  }
  return _putenv_s(name, value);
}

void unsetenv(const char* name) {
  _putenv_s(name, "");
}

BOOL IsElevated() {
  BOOL fRet = FALSE;
  HANDLE hToken = NULL;
  if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
    TOKEN_ELEVATION Elevation;
    DWORD cbSize = sizeof(TOKEN_ELEVATION);
    if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
      fRet = Elevation.TokenIsElevated;
    }
  }
  if (hToken) {
    CloseHandle(hToken);
  }
  return fRet;
}
#else
bool IsElevated() {
  return true;
}
#endif

using namespace openstudio;

TEST(ApplicationPathHelpers, Strings) {
  path applicationPath = getApplicationPath();
  path applicationDir = getApplicationDirectory();
  path openstudioModule = getOpenStudioModule();
  path openstudioModuleDirectory = getOpenStudioModuleDirectory();
  path openstudioCLI = getOpenStudioCLI();
  path energyplusDirectory = getEnergyPlusDirectory();
  path energyplusExecutable = getEnergyPlusExecutable();

  EXPECT_TRUE(exists(applicationPath));
  EXPECT_TRUE(exists(applicationDir));
  EXPECT_TRUE(exists(openstudioModule));
  EXPECT_TRUE(exists(openstudioModuleDirectory));
  EXPECT_TRUE(exists(openstudioCLI));
  EXPECT_TRUE(exists(energyplusDirectory));
  EXPECT_TRUE(exists(energyplusExecutable));
}

// This is really in PathHelpers but used by ApplicationPathHelpers
TEST(ApplicationPathHelpers, findInSystemPath) {

  openstudio::path dummy_dir = getApplicationBuildDirectory() / toPath("Testing/Temporary");
  openstudio::filesystem::create_directories(dummy_dir);
  openstudio::path dummy_file_path = dummy_dir / "dummy_file";

  boost::filesystem::ofstream outfile(dummy_file_path);
  outfile.close();

  // This should fail
  openstudio::path absolute_path_to_dummy = findInSystemPath(dummy_file_path.filename());
  EXPECT_FALSE(exists(absolute_path_to_dummy));

  // Append the containing dir to the PATH
  auto* current_path = std::getenv("PATH");
  std::string new_path = toString(dummy_dir) + openstudio::pathDelimiter() + current_path;
  setenv("PATH", new_path.c_str(), 1);
  setenv("PathWhenSymlinkInPath_Setup", "true", 1);

  // Locate the file with only its name
  absolute_path_to_dummy = findInSystemPath(dummy_file_path.filename());
  EXPECT_TRUE(exists(absolute_path_to_dummy)) << "Tried to find " << dummy_file_path.filename() << "in path: " << current_path;
  EXPECT_EQ(dummy_file_path, absolute_path_to_dummy);

  // Put it back
  setenv("PATH", current_path, 1);
  boost::filesystem::remove(dummy_file_path);
}

// From PathHelpers too. Ensure that completeAndNormalize keeps resolving symlinks until found
TEST(ApplicationPathHelpers, completeAndNormalizeMultipleSymlinks) {

  // cppcheck-suppress knownConditionTrueFalse
  if (!IsElevated()) {
    SKIP(completeAndNormalizeMultipleSymlinks);
  }

  std::vector<openstudio::path> toClean;
  openstudio::path path_dir = getApplicationBuildDirectory() / toPath("Testing/Temporary");
  openstudio::path path_subdir = path_dir / toPath("SubDir");

  openstudio::filesystem::create_directories(path_subdir);

  openstudio::path ori_path = path_dir / "completeAndNormalize_oriFile";
  toClean.push_back(ori_path);

  // 'touch'
  boost::filesystem::ofstream outfile(ori_path);
  outfile.close();

  openstudio::path prev_path = ori_path;

  // First one, same directory, absolute
  openstudio::path symlink_path = path_dir / toPath("1.symlink_absolute_same_dir");
  openstudio::path pointsTo = prev_path;
  boost::filesystem::create_symlink(pointsTo, symlink_path);
  prev_path = symlink_path;
  toClean.push_back(symlink_path);

  // Subdir, absolute
  symlink_path = path_subdir / toPath("2.symlink_absolute_subdir");
  pointsTo = prev_path;
  boost::filesystem::create_symlink(pointsTo, symlink_path);
  prev_path = symlink_path;
  toClean.push_back(symlink_path);

  // Same subdir, relative
  symlink_path = path_subdir / toPath("3.symlink_relative_same_subdir");
  pointsTo = prev_path.filename();
  boost::filesystem::create_symlink(pointsTo, symlink_path);
  prev_path = symlink_path;
  toClean.push_back(symlink_path);

  // Parent dir, relative
  symlink_path = path_dir / toPath("4.symlink_relative_pardir");
  // Just for fun, adding an uncessary "dir/../dir/" portion
  pointsTo = path_subdir.filename() / toPath("..") / path_subdir.filename() / prev_path.filename();
  boost::filesystem::create_symlink(pointsTo, symlink_path);
  prev_path = symlink_path;
  toClean.push_back(symlink_path);

  // Subdir, relative
  symlink_path = path_subdir / toPath("5.symlink_relative_pardir");
  pointsTo = toPath("..") / prev_path.filename();
  boost::filesystem::create_symlink(pointsTo, symlink_path);
  // prev_path = symlink_path;
  toClean.push_back(symlink_path);

  openstudio::path foundPath = completeAndNormalize(symlink_path);
  EXPECT_TRUE(exists(foundPath));
  EXPECT_EQ(toString(ori_path), toString(foundPath));

  for (const auto& p : toClean) {
    boost::filesystem::remove(p);
  }
}

// Want to make sure what getOpenStudioModule actually returns
TEST(ApplicationPathHelpers, Simple_test_forThisModule) {
  path openstudioModulePath = getOpenStudioModule();
  EXPECT_TRUE(exists(openstudioModulePath));
  // The expected path is the utilities one, but resolved for symlinks (we don't want to hardcode the version eg openstudio_utilities_tests-2.8.0)
#if defined(_WIN32)
#  if defined(NINJA)
  openstudio::path expectedOpenstudioModulePath = getApplicationBuildDirectory() / toPath("Products/openstudiolib.dll");
#  elif _DEBUG
  openstudio::path expectedOpenstudioModulePath = getApplicationBuildDirectory() / toPath("Products/Debug/openstudiolib.dll");
#  else
  openstudio::path expectedOpenstudioModulePath = getApplicationBuildDirectory() / toPath("Products/Release/openstudiolib.dll");
#  endif
#elif __APPLE__
  openstudio::path expectedOpenstudioModulePath = getApplicationBuildDirectory() / toPath("Products/libopenstudiolib.dylib");
#else
  openstudio::path expectedOpenstudioModulePath = getApplicationBuildDirectory() / toPath("Products/libopenstudiolib.so");
#endif
  expectedOpenstudioModulePath = completeAndNormalize(expectedOpenstudioModulePath);
  EXPECT_EQ(toString(expectedOpenstudioModulePath), toString(openstudioModulePath));
}
