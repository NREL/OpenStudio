/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include <resources.hxx>

#include "CoreFixture.hpp"
#include "../Path.hpp"
#include "../PathHelpers.hpp"
#include "../Filesystem.hpp"

#include <clocale>

using openstudio::path;
using openstudio::toPath;
using openstudio::toString;
using openstudio::completePathToFile;
using openstudio::setFileExtension;
using openstudio::makeParentFolder;
using openstudio::relativePath;
using openstudio::printPathInformation;
using openstudio::windowsDriveLetter;
using openstudio::isNetworkPath;
using openstudio::isNetworkPathAvailable;

void logBeforeAndAfterPathInformation(const std::string& functionName, const path& before, const path& after) {
  std::stringstream ssb;
  std::stringstream ssa;

  printPathInformation(ssb, before);
  printPathInformation(ssa, after);

  LOG_FREE(Debug, "CoreFixture", "Before " << functionName << ": " << ssb.str() << '\n' << "After " << functionName << ": " << ssa.str() << '\n');
}

TEST_F(CoreFixture, Path_CompletePathToFile) {
  path p = resourcesPath() / toPath("energyplus/5ZoneAirCooled/eplusout");
  path result = completePathToFile(p, path(), "sql");
  logBeforeAndAfterPathInformation("completePathToFile with ext=\"sql\"", p, result);
  path tmp = p.replace_extension(toPath("sql").string());
  EXPECT_TRUE(result == tmp);

  p = toPath("energyplus/5ZoneAirCooled/eplusout");
  path base = resourcesPath();
  result = completePathToFile(p, base, "sql");
  logBeforeAndAfterPathInformation("completePathToFile with base=resourcesPath() and ext=\"sql\"", p, result);
  EXPECT_TRUE(result == tmp);

  p = resourcesPath() / toPath("energyplus/5ZoneAirCooled.idf");
  result = completePathToFile(p, path(), "sql", true);
  logBeforeAndAfterPathInformation("completePathToFile with ext=\"sql\"", p, result);
  EXPECT_TRUE(result.empty());

  p = resourcesPath() / toPath("energyplus/5ZoneAirCooled");
  result = completePathToFile(p);
  logBeforeAndAfterPathInformation("completePathToFile", p, result);
  EXPECT_TRUE(result.empty());
}

TEST_F(CoreFixture, Path_toString) {
#ifdef _WINDOWS
  EXPECT_EQ("energyplus/5ZoneAirCooled/eplusout.sql", toString(toPath("energyplus/5ZoneAirCooled/eplusout.sql")));
  EXPECT_EQ("energyplus/5ZoneAirCooled/eplusout.sql", toString(toPath("energyplus\\5ZoneAirCooled\\eplusout.sql")));
  EXPECT_EQ("/energyplus/5ZoneAirCooled/eplusout.sql", toString(toPath("/energyplus/5ZoneAirCooled/eplusout.sql")));
  EXPECT_EQ("/energyplus/5ZoneAirCooled/eplusout.sql", toString(toPath("/energyplus\\5ZoneAirCooled\\eplusout.sql")));
  EXPECT_EQ("energyplus/5ZoneAirCooled/", toString(toPath("energyplus/5ZoneAirCooled/")));
  EXPECT_EQ("energyplus/5ZoneAirCooled/", toString(toPath("energyplus\\5ZoneAirCooled\\")));
  EXPECT_EQ("/energyplus/5ZoneAirCooled/", toString(toPath("/energyplus/5ZoneAirCooled/")));
  EXPECT_EQ("/energyplus/5ZoneAirCooled/", toString(toPath("\\energyplus\\5ZoneAirCooled\\")));
  EXPECT_EQ("energyplus/5ZoneAirCooled", toString(toPath("energyplus/5ZoneAirCooled")));
  EXPECT_EQ("energyplus/5ZoneAirCooled", toString(toPath("energyplus\\5ZoneAirCooled")));
  EXPECT_EQ("/energyplus/5ZoneAirCooled", toString(toPath("/energyplus/5ZoneAirCooled")));
  EXPECT_EQ("/energyplus/5ZoneAirCooled", toString(toPath("\\energyplus\\5ZoneAirCooled")));
#else
  EXPECT_EQ("energyplus/5ZoneAirCooled/eplusout.sql", toString(toPath("energyplus/5ZoneAirCooled/eplusout.sql")));
  EXPECT_EQ("energyplus\\5ZoneAirCooled\\eplusout.sql", toString(toPath("energyplus\\5ZoneAirCooled\\eplusout.sql")));
  EXPECT_EQ("/energyplus/5ZoneAirCooled/eplusout.sql", toString(toPath("/energyplus/5ZoneAirCooled/eplusout.sql")));
  EXPECT_EQ("/energyplus\\5ZoneAirCooled\\eplusout.sql", toString(toPath("/energyplus\\5ZoneAirCooled\\eplusout.sql")));
  EXPECT_EQ("energyplus/5ZoneAirCooled/", toString(toPath("energyplus/5ZoneAirCooled/")));
  EXPECT_EQ("energyplus\\5ZoneAirCooled\\", toString(toPath("energyplus\\5ZoneAirCooled\\")));
  EXPECT_EQ("/energyplus/5ZoneAirCooled/", toString(toPath("/energyplus/5ZoneAirCooled/")));
  EXPECT_EQ("\\energyplus\\5ZoneAirCooled\\", toString(toPath("\\energyplus\\5ZoneAirCooled\\")));
  EXPECT_EQ("energyplus/5ZoneAirCooled", toString(toPath("energyplus/5ZoneAirCooled")));
  EXPECT_EQ("energyplus\\5ZoneAirCooled", toString(toPath("energyplus\\5ZoneAirCooled")));
  EXPECT_EQ("/energyplus/5ZoneAirCooled", toString(toPath("/energyplus/5ZoneAirCooled")));
  EXPECT_EQ("\\energyplus\\5ZoneAirCooled", toString(toPath("\\energyplus\\5ZoneAirCooled")));
#endif
}

TEST_F(CoreFixture, Path_RelativePathToFile) {
  path relPath = toPath("energyplus/5ZoneAirCooled/eplusout.sql");
  path fullPath = resourcesPath() / relPath;
  EXPECT_EQ(toString(relPath), toString(relativePath(fullPath, resourcesPath())));

  EXPECT_EQ("eplusout.sql", toString(relativePath(relPath, toPath("energyplus/5ZoneAirCooled/"))));
}

TEST_F(CoreFixture, Path_SetFileExtension) {
  // example usage for assigning proper file extension
  path p = resourcesPath() / toPath("energyplus/5ZoneAirCooled/in");
  path result = setFileExtension(p, "idf");
  EXPECT_TRUE(openstudio::filesystem::extension(p).empty());
  EXPECT_TRUE(toString(openstudio::filesystem::extension(result)) == std::string(".idf"));
  EXPECT_TRUE(openstudio::filesystem::exists(result));
  EXPECT_TRUE(openstudio::filesystem::is_regular_file(result));

  // passes out path as is if file extension already set
  p = resourcesPath() / toPath("energyplus/5ZoneAirCooled/in.idf");
  result = setFileExtension(p, "idf");
  EXPECT_TRUE(toString(openstudio::filesystem::extension(p)) == std::string(".idf"));
  EXPECT_TRUE(toString(openstudio::filesystem::extension(result)) == std::string(".idf"));

  // will not replace extension, but will log warning and alert user by returning empty path
  p = toPath("energyplus/5ZoneAirCooled/in.osm");
  result = setFileExtension(p, "idf", false);
  EXPECT_TRUE(result == p);

  // will replace extension if asked
  p = toPath("energyplus/5ZoneAirCooled/in.osm");
  result = setFileExtension(p, "idf", true, false);
  EXPECT_TRUE(toString(openstudio::filesystem::extension(result)) == std::string(".idf"));

  // setFileExtension does not care about existence
  p = toPath("fakeDir/fakeDirOrFile");
  result = setFileExtension(p, "jjj", true);
  EXPECT_TRUE(toString(openstudio::filesystem::extension(result)) == std::string(".jjj"));
}

TEST_F(CoreFixture, Path_MakeParentFolder) {
  // path to directory
  // add one folder
  path p = resourcesPath() / toPath("energyplus/5ZoneAirCooled/MyTestFolder/");
  EXPECT_FALSE(openstudio::filesystem::is_directory(p));
  EXPECT_FALSE(openstudio::filesystem::exists(p));
  EXPECT_TRUE(makeParentFolder(p));
  EXPECT_TRUE(openstudio::filesystem::is_directory(p));
  EXPECT_TRUE(openstudio::filesystem::exists(p));
  EXPECT_EQ(static_cast<unsigned>(1), openstudio::filesystem::remove_all(p));

  // path to file
  // add parent folder
  p = resourcesPath() / toPath("energyplus/5ZoneAirCooled/MyTestFolder/in.idf");
  EXPECT_FALSE(openstudio::filesystem::is_directory(p.parent_path()));
  EXPECT_FALSE(openstudio::filesystem::exists(p.parent_path()));
  EXPECT_FALSE(openstudio::filesystem::is_regular_file(p));
  EXPECT_TRUE(makeParentFolder(p));
  EXPECT_TRUE(openstudio::filesystem::is_directory(p.parent_path()));
  EXPECT_TRUE(openstudio::filesystem::exists(p.parent_path()));
  EXPECT_FALSE(openstudio::filesystem::is_regular_file(p));
  EXPECT_EQ(static_cast<unsigned>(1), openstudio::filesystem::remove_all(p.parent_path()));

  // path to directory/directory
  // do not add any folders
  p = resourcesPath() / toPath("energyplus/5ZoneAirCooled/MyTestFolder1/MyTestFolder2/MyTestFolder3/");
  EXPECT_FALSE(openstudio::filesystem::is_directory(p));
  EXPECT_FALSE(openstudio::filesystem::exists(p));
  EXPECT_FALSE(makeParentFolder(p));
  EXPECT_FALSE(openstudio::filesystem::is_directory(p));
  EXPECT_FALSE(openstudio::filesystem::exists(p));

  // path to directory/directory
  // add folders recursively
  p = resourcesPath() / toPath("energyplus/5ZoneAirCooled/MyTestFolder1/MyTestFolder2/MyTestFolder3/");
  EXPECT_FALSE(openstudio::filesystem::is_directory(p));
  EXPECT_TRUE(makeParentFolder(p, path(), true));
  EXPECT_TRUE(openstudio::filesystem::is_directory(p));
  EXPECT_EQ(static_cast<unsigned>(3), openstudio::filesystem::remove_all(p.parent_path().parent_path().parent_path()));

  // path to directory/directory/file
  // use base
  // add folders recursively
  p = toPath("energyplus/5ZoneAirCooled/MyTestFolder1/MyTestFolder2/MyTestFolder3/in.idf");
  path base = resourcesPath();
  path tmp = base / p;
  EXPECT_FALSE(openstudio::filesystem::is_directory(tmp.parent_path()));
  EXPECT_FALSE(openstudio::filesystem::is_regular_file(tmp));
  EXPECT_TRUE(makeParentFolder(p, base, true));
  EXPECT_TRUE(openstudio::filesystem::is_directory(tmp.parent_path()));
  EXPECT_FALSE(openstudio::filesystem::is_regular_file(tmp));
  EXPECT_EQ(static_cast<unsigned>(3), openstudio::filesystem::remove_all(tmp.parent_path().parent_path().parent_path()));
}

TEST_F(CoreFixture, Path_WindowsPathOnUnix) {
  // want to make sure we do not end up with "/E:/test/CloudTest/scripts/StandardReports/measure.rb"
  openstudio::path file = openstudio::toPath("E:/test/CloudTest/scripts/StandardReports/measure.rb");
  EXPECT_FALSE(file.empty());
  EXPECT_EQ("E:/test/CloudTest/scripts/StandardReports/measure.rb", openstudio::toString(file));
  EXPECT_EQ("measure.rb", openstudio::toString(file.filename()));
  EXPECT_TRUE(file.has_parent_path());
  EXPECT_EQ("E:/test/CloudTest/scripts/StandardReports", openstudio::toString(file.parent_path()));
  EXPECT_TRUE(file.has_stem());
  EXPECT_EQ("measure", openstudio::toString(file.stem()));
  EXPECT_TRUE(file.has_extension());
  EXPECT_EQ(".rb", openstudio::toString(file.extension()));
#if (defined(_WIN32) || defined(_WIN64))
  EXPECT_TRUE(file.has_root_name());
  EXPECT_EQ("E:", openstudio::toString(file.root_name()));
  EXPECT_TRUE(file.has_root_directory());
  EXPECT_EQ("/", openstudio::toString(file.root_directory()));
  EXPECT_TRUE(file.has_root_path());
  EXPECT_EQ("E:/", openstudio::toString(file.root_path()));
  EXPECT_TRUE(file.has_relative_path());
  EXPECT_EQ("test/CloudTest/scripts/StandardReports/measure.rb", openstudio::toString(file.relative_path()));
  EXPECT_TRUE(file.is_absolute());
  EXPECT_FALSE(file.is_relative());
#else
  EXPECT_FALSE(file.has_root_name());
  EXPECT_EQ("", openstudio::toString(file.root_name()));
  EXPECT_FALSE(file.has_root_directory());
  EXPECT_EQ("", openstudio::toString(file.root_directory()));
  EXPECT_FALSE(file.has_root_path());
  EXPECT_EQ("", openstudio::toString(file.root_path()));
  EXPECT_TRUE(file.has_relative_path());
  EXPECT_EQ("E:/test/CloudTest/scripts/StandardReports/measure.rb", openstudio::toString(file.relative_path()));
  EXPECT_FALSE(file.is_absolute());
  EXPECT_TRUE(file.is_relative());
#endif

  file = openstudio::toPath("E:\\test\\CloudTest\\scripts\\StandardReports\\measure.rb");
  EXPECT_FALSE(file.empty());
  EXPECT_TRUE(file.has_extension());
  EXPECT_EQ(".rb", openstudio::toString(file.extension()));
#if (defined(_WIN32) || defined(_WIN64))
  EXPECT_EQ("E:/test/CloudTest/scripts/StandardReports/measure.rb", openstudio::toString(file));
  EXPECT_EQ("measure.rb", openstudio::toString(file.filename()));
  EXPECT_TRUE(file.has_root_name());
  EXPECT_EQ("E:", openstudio::toString(file.root_name()));
  EXPECT_TRUE(file.has_root_directory());
  EXPECT_EQ("/", openstudio::toString(file.root_directory()));
  EXPECT_TRUE(file.has_root_path());
  EXPECT_EQ("E:/", openstudio::toString(file.root_path()));
  EXPECT_TRUE(file.has_relative_path());
  EXPECT_EQ("test/CloudTest/scripts/StandardReports/measure.rb", openstudio::toString(file.relative_path()));
  EXPECT_TRUE(file.has_parent_path());
  EXPECT_EQ("E:/test/CloudTest/scripts/StandardReports", openstudio::toString(file.parent_path()));
  EXPECT_TRUE(file.has_stem());
  EXPECT_EQ("measure", openstudio::toString(file.stem()));
  EXPECT_TRUE(file.is_absolute());
  EXPECT_FALSE(file.is_relative());
#else
  EXPECT_EQ("E:\\test\\CloudTest\\scripts\\StandardReports\\measure.rb", openstudio::toString(file));
  EXPECT_EQ("E:\\test\\CloudTest\\scripts\\StandardReports\\measure.rb", openstudio::toString(file.filename()));
  EXPECT_FALSE(file.has_root_name());
  EXPECT_EQ("", openstudio::toString(file.root_name()));
  EXPECT_FALSE(file.has_root_directory());
  EXPECT_EQ("", openstudio::toString(file.root_directory()));
  EXPECT_FALSE(file.has_root_path());
  EXPECT_EQ("", openstudio::toString(file.root_path()));
  EXPECT_TRUE(file.has_relative_path());
  EXPECT_EQ("E:\\test\\CloudTest\\scripts\\StandardReports\\measure.rb", openstudio::toString(file.relative_path()));
  EXPECT_FALSE(file.has_parent_path());
  EXPECT_EQ("", openstudio::toString(file.parent_path()));
  EXPECT_TRUE(file.has_stem());
  EXPECT_EQ("E:\\test\\CloudTest\\scripts\\StandardReports\\measure", openstudio::toString(file.stem()));
  EXPECT_FALSE(file.is_absolute());
  EXPECT_TRUE(file.is_relative());
#endif
}

TEST_F(CoreFixture, WindowsDriveLetter) {
  boost::optional<std::string> test;

  test = windowsDriveLetter(toPath("C:"));
  ASSERT_TRUE(test);
  EXPECT_EQ("C", *test);

  test = windowsDriveLetter(toPath("C:\\"));
  ASSERT_TRUE(test);
  EXPECT_EQ("C", *test);

  test = windowsDriveLetter(toPath("C:\\test\\file"));
  ASSERT_TRUE(test);
  EXPECT_EQ("C", *test);

  test = windowsDriveLetter(toPath("C:/"));
  ASSERT_TRUE(test);
  EXPECT_EQ("C", *test);

  test = windowsDriveLetter(toPath("C:/test/file"));
  ASSERT_TRUE(test);
  EXPECT_EQ("C", *test);

  test = windowsDriveLetter(toPath("/"));
  EXPECT_FALSE(test);

  test = windowsDriveLetter(toPath("/test/file"));
  EXPECT_FALSE(test);

  test = windowsDriveLetter(toPath("./"));
  EXPECT_FALSE(test);

  test = windowsDriveLetter(toPath("./test/file"));
  EXPECT_FALSE(test);
}

TEST_F(CoreFixture, IsNetworkPath) {
  openstudio::path path;

#if (defined(_WIN32) || defined(_WIN64))
  path = toPath("C:/");
#else
  path = toPath("/");
#endif
  EXPECT_TRUE(path.is_absolute());
  EXPECT_FALSE(isNetworkPath(path));
  EXPECT_FALSE(isNetworkPathAvailable(path));

  path = toPath("./test");
  EXPECT_FALSE(path.is_absolute());
  EXPECT_FALSE(isNetworkPath(path));
  EXPECT_FALSE(isNetworkPathAvailable(path));

#ifdef _WINDOWS
  path = toPath("\\\\server\\folder");
  EXPECT_TRUE(path.is_absolute());
  EXPECT_TRUE(isNetworkPath(path));
  EXPECT_FALSE(isNetworkPathAvailable(path));
#endif

  // DLM: below you can use for manual testing, don't check in
  // on windows you can type 'net use' to see network drives and their status
  // to add a test drive: net use x: \\server\folder
  // to remove a test drive: net use x: /Delete
  /*
  path = "X:/";
  EXPECT_TRUE(path.is_absolute());
  EXPECT_FALSE(isNetworkPath(path));
  EXPECT_FALSE(isNetworkPathAvailable(path));

  path = "U:/";
  EXPECT_TRUE(path.is_absolute());
  EXPECT_TRUE(isNetworkPath(path));
  EXPECT_TRUE(isNetworkPathAvailable(path));

  path = "Y:/";
  EXPECT_TRUE(path.is_absolute());
  EXPECT_TRUE(isNetworkPath(path));
  EXPECT_TRUE(isNetworkPathAvailable(path));

  path = "Z:/";
  EXPECT_TRUE(path.is_absolute());
  EXPECT_FALSE(isNetworkPath(path));
  EXPECT_FALSE(isNetworkPathAvailable(path));
  */
}

TEST_F(CoreFixture, Path_Conversions) {
  std::string s;
  std::wstring w;
  path p;

  s = std::string("basic_path");
  w = std::wstring(L"basic_path");
  p = path(s);
  EXPECT_EQ(s, p.string());
  EXPECT_EQ(s, toString(p));
  EXPECT_EQ(w, p.wstring());

  p = path(w);
  EXPECT_EQ(s, p.string());
  EXPECT_EQ(s, toString(p));

  p = path(w);
  EXPECT_EQ(s, p.string());
  EXPECT_EQ(s, toString(p));

  // http://utf8everywhere.org/

  // from http://www.nubaria.com/en/blog/?p=289

  // Chinese characters for "zhongwen" ("Chinese language").
  const char kChineseSampleText[] = {-28, -72, -83, -26, -106, -121, 0};

  // Arabic "al-arabiyya" ("Arabic").
  const char kArabicSampleText[] = {-40, -89, -39, -124, -40, -71, -40, -79, -40, -88, -39, -118, -40, -87, 0};

  // Spanish word "canon" with an "n" with "~" on top and an "o" with an acute accent.
  const char kSpanishSampleText[] = {99, 97, -61, -79, -61, -77, 110, 0};

  std::string t;

  t = std::string(kChineseSampleText);
  p = toPath(t);
  EXPECT_EQ(t, toString(p));

  t = std::string(kArabicSampleText);
  p = toPath(t);
  EXPECT_EQ(t, toString(p));

  t = std::string(kSpanishSampleText);
  p = toPath(t);
  EXPECT_EQ(t, toString(p));
}

TEST_F(CoreFixture, LastLevelDirectoryWithDot) {
  // We want to make sure that we don't end up with "A measure with 90" (test for #3249)
  openstudio::path measure_directory = openstudio::toPath("C:/users/user name/OpenStudio/Measures/A measure with 90.1 dots");
  openstudio::path lastLevelDir = openstudio::getLastLevelDirectoryName(measure_directory);
  EXPECT_EQ("A measure with 90.1 dots", toString(lastLevelDir));
  EXPECT_EQ(measure_directory, measure_directory.parent_path() / lastLevelDir);
}

TEST_F(CoreFixture, Path_WithSpecialChars) {

// TODO: we should perhaps include Boost.Locale (which requires compilation...)
// or even better use ICU...

// Note JM 2019-05-22: MSVC 2013 doesn't support unicode string literals...
// https://docs.microsoft.com/en-us/previous-versions/hh567368(v=vs.140)

// "AfolderwithspécialCHar#%ù/test.osm"
#if defined(_MSC_VER)
  // On Windows, assume Windows-1252
  std::string windows_1252_str("Afolderwithsp"
                               "\xe9"
                               "cialCHar#%"
                               "\xf9"
                               "/test.osm");
  openstudio::path p_native(windows_1252_str);
#else
  // We're going to rely on boost::filesystem::path passing a wide string with unicode code points, as it will correctly interpret this as unicode
  std::wstring unicode_w_str(L"Afolderwithsp\u00E9cialCHar\u0023\u0025\u00F9/test.osm");
  openstudio::path p_native(unicode_w_str);
#endif

  // é = \xc3\xa9
  // # = \x23 (but classic)
  // % = \x25 (but classic)
  // ù = \xc3\xb9
  // / = \x2f (but classic)
  // By classic I mean part of the Latin Classic table
  // é and ù are part of the Latin-1 Supplement
  // Here I have breaking between normal chars and the escape codes otherwise it tries to read whatever normal char is after an \x until the next \x
  std::string c("Afolderwithsp"
                "\xc3\xa9"
                "cialCHar"
                "\x23\x25\xc3\xb9\x2f"
                "test.osm");
  // Perhaps ill-named now (because before I was just using L"Afolderwithsp\u00E9cialCHar\u0023\u0025\u00F9/test.osm")
  // But we are comparing this and the p_native so it should be ok
  openstudio::path pwide;
  EXPECT_NO_THROW(pwide = toPath(c));
  EXPECT_EQ(pwide, p_native);

  // Make the folder, and touch the test file
  openstudio::path outfolder = resourcesPath() / toPath("..") / toPath("Testing");
  openstudio::path pwide_full = outfolder / p_native;
  makeParentFolder(pwide_full, path(), true);
  boost::filesystem::ofstream outfile(pwide_full);
  outfile.close();
  // Ensure that worked
  EXPECT_TRUE(openstudio::filesystem::exists(pwide_full));

  // Alright, now we try with a regular string with special chars
  std::string s("AfolderwithspécialCHar#%ù/test.osm");
#if defined(_MSC_VER)
  EXPECT_NE(s, windows_1252_str);
#else
  // s = "Afolderwithsp\xC3\xA9" "cialCHar#%\xC3\xB9/test.osm"
  // the string representation of unicode_w_str is "Afolderwithsp\xE9" "cialCHar#%\xF9/test.osm"
  EXPECT_NE(s, std::string(unicode_w_str.begin(), unicode_w_str.end()));
#endif
  openstudio::path p;
  EXPECT_NO_THROW(p = toPath(s));
  openstudio::path pfull = outfolder / p;

  // The real test is that the internal paths should be the same
  EXPECT_TRUE(openstudio::filesystem::exists(pfull));
  EXPECT_EQ(p, pwide);

  // Test some strings
  std::string converted_s = toString(p);
  std::string converted_swide = toString(pwide);
  EXPECT_EQ(converted_s, converted_swide);

  // This doesn't work
  //EXPECT_EQ(s, converted_s);
  //EXPECT_EQ(s, converted_swide);
  //EXPECT_EQ(s.length(), converted_s.length());
  //for (size_t i = 0; i < s.length(); ++i) {
  //  EXPECT_EQ(s[i], converted_s[i]) << "s[i]=" << s[i] << ", converted_s[i]=" << converted_s[i];
  //}

  // Alright, now we try with a regular string that is encoded as Windows-1252
  // "Afolderwithsp\xe9cialCHar#%\xf9/test.osm"
#if defined(_MSC_VER)
#  pragma warning(push)
// MSVC 2013 appears to raise this warning
#  pragma warning(disable : 4309)
// MSVC 2017/19 would issue this one instead
#  pragma warning(disable : 4838)

  // Commented out because we don't want to run that on Unix
  //#else
  //  #pragma GCC diagnostic push
  //  #pragma GCC diagnostic ignored "-Wnarrowing"
  //#endif

  const char windows_1252_chr[] = {65, 102, 111, 108, 100, 101, 114, 119, 105, 116, 104, 115, 112, 233, 99,  105, 97, 108,
                                   67, 72,  97,  114, 35,  37,  249, 47,  116, 101, 115, 116, 46,  111, 115, 109, 0};
  // std::string s3("Afolderwithsp\xe9cialCHar#%\xf9/test.osm");
  std::string s2(windows_1252_chr);
  // This really ends up the same as the windows_1252_str string defined above
  EXPECT_EQ(s2, windows_1252_str);
  // But it isn't indeed the same as the string we used to create the folder and file
  EXPECT_NE(s2, c);

  openstudio::path p2;
  EXPECT_NO_THROW(p2 = toPath(s2));
  openstudio::path pfull2 = outfolder / p2;

  // The real test is that the internal paths should be the same
  ASSERT_NO_THROW(openstudio::filesystem::exists(pfull2));
  EXPECT_TRUE(openstudio::filesystem::exists(pfull2));
  EXPECT_EQ(p2, pwide);

  // Test some strings
  std::string converted_s2 = toString(p2);
  EXPECT_EQ(converted_s2, converted_swide);

  // Clean up after yourself!
  boost::filesystem::remove_all(pwide_full);

  // #if defined(_MSC_VER)
#  pragma warning(pop)
// #else
//  #pragma GCC diagnostic pop
#endif
}
