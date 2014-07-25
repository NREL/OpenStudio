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

#include "ModelFixture.hpp"

#include "../../utilities/idd/IddKey.hpp"
#include "../../utilities/core/Finder.hpp"

#include <resources.hxx>

using openstudio::Logger;
using openstudio::FileLogSink;
using openstudio::toPath;

void ModelFixture::SetUp() {}

void ModelFixture::TearDown() {}

void ModelFixture::SetUpTestCase() {
  // set up logging
  logFile = FileLogSink(toPath("./ModelTestFixture.log"));
  logFile->setLogLevel(Debug);
  Logger::instance().standardOutLogger().disable();

  // create resources folder for scratch files
  boost::filesystem::create_directory(resourcesPath()/toPath("model"));
}

void ModelFixture::TearDownTestCase()
{
  logFile->disable();
}

void ModelFixture::testBooleanIddField(const openstudio::IddField& iddField,
                                       const boost::optional<std::string>& defaultValue)
{
  EXPECT_TRUE(iddField.properties().type == openstudio::IddFieldType::ChoiceType);
  openstudio::IddKeyVector keys = iddField.keys();
  EXPECT_EQ(2u,keys.size());
  openstudio::NameFinder<openstudio::IddKey> finder("Yes");
  openstudio::IddKeyVector::const_iterator it = std::find_if(keys.begin(),keys.end(),finder);
  EXPECT_FALSE(it == keys.end());
  finder = openstudio::NameFinder<openstudio::IddKey>("No");
  it = std::find_if(keys.begin(),keys.end(),finder);
  EXPECT_FALSE(it == keys.end());
  if (defaultValue) {
    ASSERT_TRUE(iddField.properties().stringDefault);
    EXPECT_EQ(*defaultValue,iddField.properties().stringDefault.get());
  }
}

double ModelFixture::tol(1.0E-5);
boost::optional<openstudio::FileLogSink> ModelFixture::logFile;
