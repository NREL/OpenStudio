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

#include <resources.hxx>

#include <boost/filesystem.hpp>
#include "CoreFixture.hpp"
#include "../Path.hpp"
#include "../PathHelpers.hpp"

using openstudio::path;
using openstudio::toPath;
using openstudio::toString;
using openstudio::completePathToFile;
using openstudio::setFileExtension;
using openstudio::makeParentFolder;
using openstudio::relativePath;
using openstudio::printPathInformation;

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
  EXPECT_TRUE(boost::filesystem::extension(p).empty());
  EXPECT_TRUE(toString(boost::filesystem::extension(result)) == std::string(".idf"));
  EXPECT_TRUE(boost::filesystem::exists(result));
  EXPECT_TRUE(boost::filesystem::is_regular_file(result));

  // passes out path as is if file extension already set
  p = resourcesPath()/toPath("energyplus/5ZoneAirCooled/in.idf");
  result = setFileExtension(p,"idf");
  EXPECT_TRUE(toString(boost::filesystem::extension(p)) == std::string(".idf"));
  EXPECT_TRUE(toString(boost::filesystem::extension(result)) == std::string(".idf"));

  // will not replace extension, but will log warning and alert user by returning empty path
  p = toPath("energyplus/5ZoneAirCooled/in.osm");
  result = setFileExtension(p,"idf",false);
  EXPECT_TRUE(result == p);

  // will replace extension if asked
  p = toPath("energyplus/5ZoneAirCooled/in.osm");
  result = setFileExtension(p,"idf",true,false);
  EXPECT_TRUE(toString(boost::filesystem::extension(result)) == std::string(".idf"));

  // setFileExtension does not care about existence
  p = toPath("fakeDir/fakeDirOrFile");
  result = setFileExtension(p,"jjj",true);
  EXPECT_TRUE(toString(boost::filesystem::extension(result)) == std::string(".jjj"));
}

TEST_F(CoreFixture, Path_MakeParentFolder)
{
  // path to directory
  // add one folder
  path p = resourcesPath()/toPath("energyplus/5ZoneAirCooled/MyTestFolder/");
  EXPECT_FALSE(boost::filesystem::is_directory(p));
  EXPECT_FALSE(boost::filesystem::exists(p));
  EXPECT_TRUE(makeParentFolder(p));
  EXPECT_TRUE(boost::filesystem::is_directory(p));
  EXPECT_TRUE(boost::filesystem::exists(p));
  EXPECT_EQ(static_cast<unsigned>(1),boost::filesystem::remove_all(p));

  // path to file
  // add parent folder
  p = resourcesPath()/toPath("energyplus/5ZoneAirCooled/MyTestFolder/in.idf");
  EXPECT_FALSE(boost::filesystem::is_directory(p.parent_path()));
  EXPECT_FALSE(boost::filesystem::exists(p.parent_path()));
  EXPECT_FALSE(boost::filesystem::is_regular_file(p));
  EXPECT_TRUE(makeParentFolder(p));
  EXPECT_TRUE(boost::filesystem::is_directory(p.parent_path()));
  EXPECT_TRUE(boost::filesystem::exists(p.parent_path()));
  EXPECT_FALSE(boost::filesystem::is_regular_file(p));
  EXPECT_EQ(static_cast<unsigned>(1),boost::filesystem::remove_all(p.parent_path()));

  // path to directory/directory
  // do not add any folders
  p = resourcesPath()/toPath("energyplus/5ZoneAirCooled/MyTestFolder1/MyTestFolder2/MyTestFolder3/");
  EXPECT_FALSE(boost::filesystem::is_directory(p));
  EXPECT_FALSE(boost::filesystem::exists(p));
  EXPECT_FALSE(makeParentFolder(p));
  EXPECT_FALSE(boost::filesystem::is_directory(p));
  EXPECT_FALSE(boost::filesystem::exists(p));

  // path to directory/directory
  // add folders recursively
  p = resourcesPath()/toPath("energyplus/5ZoneAirCooled/MyTestFolder1/MyTestFolder2/MyTestFolder3/");
  EXPECT_FALSE(boost::filesystem::is_directory(p));
  EXPECT_TRUE(makeParentFolder(p,path(),true));
  EXPECT_TRUE(boost::filesystem::is_directory(p));
  EXPECT_EQ(static_cast<unsigned>(3),boost::filesystem::remove_all(p.parent_path().parent_path().parent_path()));

  // path to directory/directory/file
  // use base
  // add folders recursively
  p = toPath("energyplus/5ZoneAirCooled/MyTestFolder1/MyTestFolder2/MyTestFolder3/in.idf");
  path base = resourcesPath();
  path tmp = base/p;
  EXPECT_FALSE(boost::filesystem::is_directory(tmp.parent_path()));
  EXPECT_FALSE(boost::filesystem::is_regular_file(tmp));
  EXPECT_TRUE(makeParentFolder(p,base,true));
  EXPECT_TRUE(boost::filesystem::is_directory(tmp.parent_path()));
  EXPECT_FALSE(boost::filesystem::is_regular_file(tmp));
  EXPECT_EQ(static_cast<unsigned>(3),boost::filesystem::remove_all(tmp.parent_path().parent_path().parent_path()));
}

