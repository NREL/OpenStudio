/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

  ASSERT_TRUE(openstudio::filesystem::exists(resourcesPath));
  ASSERT_FALSE(isEmptyDirectory(resourcesPath));

  QDir resourcesDir(toQString(resourcesPath));
  QStringList filters;
  filters << QString("*.osm");
  QFileInfoList files = resourcesDir.entryInfoList(filters, QDir::Files, QDir::NoSort);
  EXPECT_FALSE(files.empty());
  for (const QFileInfo& file : files) {
    openstudio::path path = toPath(file.absoluteFilePath());
    EXPECT_TRUE(openstudio::filesystem::exists(path));

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

  ASSERT_TRUE(openstudio::filesystem::exists(hvacPath));

  osversion::VersionTranslator vt;
  boost::optional<model::Model> model = vt.loadModel(hvacPath);
  ASSERT_TRUE(model);

  // uncomment this to save the version translated file to the original path
  // DO NOT leave this in the test execution when you commit!
  //model->save(hvacPath, true);
}
