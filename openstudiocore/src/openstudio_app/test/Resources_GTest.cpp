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

#include "OpenStudioAppFixture.hpp"

#include "../../osversion/VersionTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SpaceLoad.hpp"
#include "../../model/SpaceLoad_Impl.hpp"
#include "../../model/SpaceType.hpp"

#include "../../utilities/core/ApplicationPathHelpers.hpp"
#include "../../utilities/core/PathHelpers.hpp"

#include <QDir>
#include <QFileInfo>

using namespace openstudio;

TEST_F(OpenStudioAppFixture, Resources_Templates)
{
  openstudio::path resourcesPath = getApplicationSourceDirectory() / openstudio::toPath("src/openstudio_app/Resources");

  ASSERT_TRUE(boost::filesystem::exists(resourcesPath));
  ASSERT_FALSE(isEmptyDirectory(resourcesPath));

  QDir resourcesDir(toQString(resourcesPath));
  QStringList filters;
  filters << QString("*.osm");
  QFileInfoList files = resourcesDir.entryInfoList(filters, QDir::Files, QDir::NoSort);
  EXPECT_FALSE(files.empty());
  for (const QFileInfo& file : files) {
    openstudio::path path = toPath(file.absoluteFilePath());
    EXPECT_TRUE(boost::filesystem::exists(path));

    osversion::VersionTranslator vt;
    boost::optional<model::Model> model = vt.loadModel(path);
    ASSERT_TRUE(model);

    // check that each space load has a parent space type
    std::vector<model::SpaceLoad> spaceLoads;
    for (const model::SpaceLoad& spaceLoad : spaceLoads){
      EXPECT_TRUE(spaceLoad.spaceType());
    }

    // uncomment this to save the version translated file to the original path
    // DO NOT leave this in the test execution when you commit!
    //model->save(path, true);
  }
}

TEST_F(OpenStudioAppFixture, Resources_HVACLibrary)
{
  openstudio::path hvacPath = getApplicationSourceDirectory() / openstudio::toPath("src/openstudio_app/Resources/hvaclibrary/hvac_library.osm");

  ASSERT_TRUE(boost::filesystem::exists(hvacPath));

  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(hvacPath);
  ASSERT_TRUE(model);

  // uncomment this to save the version translated file to the original path
  // DO NOT leave this in the test execution when you commit!
  //model->save(hvacPath, true);
}
