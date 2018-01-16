/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include <resources.hxx>


#include "CoreFixture.hpp"
#include "../Path.hpp"
#include "../PathHelpers.hpp"
#include "../URLHelpers.hpp"

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

void logBeforeAndAfterPathInformation(const std::string& functionName,
                                      const path& before,const path& after) {
  std::stringstream ssb,ssa;

  printPathInformation(ssb,before);
  printPathInformation(ssa,after);

  LOG_FREE(Debug,"CoreFixture","Before " << functionName << ": " << ssb.str() << std::endl
           << "After " << functionName << ": " << ssa.str() << std::endl);
}

TEST_F(CoreFixture, Path_CompletePathToFile)
{
  path p = resourcesPath()/toPath("energyplus/5ZoneAirCooled/eplusout");
  path result = completePathToFile(p,path(),"sql");
  logBeforeAndAfterPathInformation("completePathToFile with ext=\"sql\"",p,result);
  path tmp = p.replace_extension(toPath("sql").string());
  EXPECT_TRUE(result == tmp);

  p = toPath("energyplus/5ZoneAirCooled/eplusout");
  path base = resourcesPath();
  result = completePathToFile(p,base,"sql");
  logBeforeAndAfterPathInformation("completePathToFile with base=resourcesPath() and ext=\"sql\"",p,result);
  EXPECT_TRUE(result == tmp);

  p = resourcesPath()/toPath("energyplus/5ZoneAirCooled.idf");
  result = completePathToFile(p,path(),"sql",true);
  logBeforeAndAfterPathInformation("completePathToFile with ext=\"sql\"",p,result);
  EXPECT_TRUE(result.empty());

  p = resourcesPath()/toPath("energyplus/5ZoneAirCooled");
  result = completePathToFile(p);
  logBeforeAndAfterPathInformation("completePathToFile",p,result);
  EXPECT_TRUE(result.empty());

}

TEST_F(CoreFixture, Path_toString)
{
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
}

TEST_F(CoreFixture, Path_RelativePathToFile)
{
  path relPath = toPath("energyplus/5ZoneAirCooled/eplusout.sql");
  path fullPath = resourcesPath() / relPath;
  EXPECT_EQ(toString(relPath),toString(relativePath(fullPath,resourcesPath())));

  EXPECT_EQ("eplusout.sql",toString(relativePath(relPath,toPath("energyplus/5ZoneAirCooled/"))));
}

TEST_F(CoreFixture, Path_SetFileExtension)
{
  // example usage for assigning proper file extension
  path p = resourcesPath()/toPath("energyplus/5ZoneAirCooled/in");
  path result = setFileExtension(p,"idf");
  EXPECT_TRUE(openstudio::filesystem::extension(p).empty());
  EXPECT_TRUE(toString(openstudio::filesystem::extension(result)) == std::string(".idf"));
  EXPECT_TRUE(openstudio::filesystem::exists(result));
  EXPECT_TRUE(openstudio::filesystem::is_regular_file(result));

  // passes out path as is if file extension already set
  p = resourcesPath()/toPath("energyplus/5ZoneAirCooled/in.idf");
  result = setFileExtension(p,"idf");
  EXPECT_TRUE(toString(openstudio::filesystem::extension(p)) == std::string(".idf"));
  EXPECT_TRUE(toString(openstudio::filesystem::extension(result)) == std::string(".idf"));

  // will not replace extension, but will log warning and alert user by returning empty path
  p = toPath("energyplus/5ZoneAirCooled/in.osm");
  result = setFileExtension(p,"idf",false);
  EXPECT_TRUE(result == p);

  // will replace extension if asked
  p = toPath("energyplus/5ZoneAirCooled/in.osm");
  result = setFileExtension(p,"idf",true,false);
  EXPECT_TRUE(toString(openstudio::filesystem::extension(result)) == std::string(".idf"));

  // setFileExtension does not care about existence
  p = toPath("fakeDir/fakeDirOrFile");
  result = setFileExtension(p,"jjj",true);
  EXPECT_TRUE(toString(openstudio::filesystem::extension(result)) == std::string(".jjj"));
}

TEST_F(CoreFixture, Path_MakeParentFolder)
{
  // path to directory
  // add one folder
  path p = resourcesPath()/toPath("energyplus/5ZoneAirCooled/MyTestFolder/");
  EXPECT_FALSE(openstudio::filesystem::is_directory(p));
  EXPECT_FALSE(openstudio::filesystem::exists(p));
  EXPECT_TRUE(makeParentFolder(p));
  EXPECT_TRUE(openstudio::filesystem::is_directory(p));
  EXPECT_TRUE(openstudio::filesystem::exists(p));
  EXPECT_EQ(static_cast<unsigned>(1),openstudio::filesystem::remove_all(p));

  // path to file
  // add parent folder
  p = resourcesPath()/toPath("energyplus/5ZoneAirCooled/MyTestFolder/in.idf");
  EXPECT_FALSE(openstudio::filesystem::is_directory(p.parent_path()));
  EXPECT_FALSE(openstudio::filesystem::exists(p.parent_path()));
  EXPECT_FALSE(openstudio::filesystem::is_regular_file(p));
  EXPECT_TRUE(makeParentFolder(p));
  EXPECT_TRUE(openstudio::filesystem::is_directory(p.parent_path()));
  EXPECT_TRUE(openstudio::filesystem::exists(p.parent_path()));
  EXPECT_FALSE(openstudio::filesystem::is_regular_file(p));
  EXPECT_EQ(static_cast<unsigned>(1),openstudio::filesystem::remove_all(p.parent_path()));

  // path to directory/directory
  // do not add any folders
  p = resourcesPath()/toPath("energyplus/5ZoneAirCooled/MyTestFolder1/MyTestFolder2/MyTestFolder3/");
  EXPECT_FALSE(openstudio::filesystem::is_directory(p));
  EXPECT_FALSE(openstudio::filesystem::exists(p));
  EXPECT_FALSE(makeParentFolder(p));
  EXPECT_FALSE(openstudio::filesystem::is_directory(p));
  EXPECT_FALSE(openstudio::filesystem::exists(p));

  // path to directory/directory
  // add folders recursively
  p = resourcesPath()/toPath("energyplus/5ZoneAirCooled/MyTestFolder1/MyTestFolder2/MyTestFolder3/");
  EXPECT_FALSE(openstudio::filesystem::is_directory(p));
  EXPECT_TRUE(makeParentFolder(p,path(),true));
  EXPECT_TRUE(openstudio::filesystem::is_directory(p));
  EXPECT_EQ(static_cast<unsigned>(3),openstudio::filesystem::remove_all(p.parent_path().parent_path().parent_path()));

  // path to directory/directory/file
  // use base
  // add folders recursively
  p = toPath("energyplus/5ZoneAirCooled/MyTestFolder1/MyTestFolder2/MyTestFolder3/in.idf");
  path base = resourcesPath();
  path tmp = base/p;
  EXPECT_FALSE(openstudio::filesystem::is_directory(tmp.parent_path()));
  EXPECT_FALSE(openstudio::filesystem::is_regular_file(tmp));
  EXPECT_TRUE(makeParentFolder(p,base,true));
  EXPECT_TRUE(openstudio::filesystem::is_directory(tmp.parent_path()));
  EXPECT_FALSE(openstudio::filesystem::is_regular_file(tmp));
  EXPECT_EQ(static_cast<unsigned>(3),openstudio::filesystem::remove_all(tmp.parent_path().parent_path().parent_path()));
}

TEST_F(CoreFixture, Path_WindowsPathOnUnix)
{
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
#ifdef _WINDOWS
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
#ifdef _WINDOWS
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

TEST_F(CoreFixture, OriginalPath_FromUrl)
{
  // mimics code in ToolBasedJob::acquireRequiredFiles
  // want to make sure we do not end up with "/E:/test/CloudTest/scripts/StandardReports/measure.rb"
  openstudio::Url url("file:///E:/test/CloudTest/scripts/StandardReports/measure.rb");
  openstudio::path file = openstudio::getOriginalPath(url);
  std::string str = openstudio::toString(file);
  EXPECT_EQ("E:/test/CloudTest/scripts/StandardReports/measure.rb", str);
}

TEST_F(CoreFixture, OriginalPath_FromUrl2)
{
  openstudio::Url url = openstudio::Url::fromLocalFile("E:/test/CloudTest/scripts/StandardReports/measure.rb");
  EXPECT_EQ("file:///E:/test/CloudTest/scripts/StandardReports/measure.rb", url.toString().toStdString());
  openstudio::path file = openstudio::getOriginalPath(url);
  std::string str = openstudio::toString(file);
  EXPECT_EQ("E:/test/CloudTest/scripts/StandardReports/measure.rb", str);
}

TEST_F(CoreFixture, WindowsDriveLetter)
{
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

TEST_F(CoreFixture, IsNetworkPath)
{
  openstudio::path path;

#ifdef _WINDOWS
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

  path = toPath("\\\\server\\folder");
  EXPECT_TRUE(path.is_absolute());
  EXPECT_TRUE(isNetworkPath(path));
  EXPECT_FALSE(isNetworkPathAvailable(path));

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
