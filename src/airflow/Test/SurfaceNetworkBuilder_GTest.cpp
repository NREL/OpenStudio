/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "AirflowFixture.hpp"

#include "../SurfaceNetworkBuilder.hpp"

#include "../../model/Model.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/Surface.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../osversion/VersionTranslator.hpp"
#include "../../utilities/idf/Handle.hpp"

#include "DemoModel.hpp"

#include <resources.hxx>

class SurfaceCounter : public openstudio::airflow::SurfaceNetworkBuilder
{
public:
  SurfaceCounter() : openstudio::airflow::SurfaceNetworkBuilder(nullptr)
  {
    clear();
  }

  virtual void clear() override
  {
    interiorSurface = 0;
    interiorSubSurface = 0;
    exteriorSurface = 0;
    exteriorSubSurface = 0;
  }

  int interiorSurface;
  int exteriorSurface;
  int interiorSubSurface;
  int exteriorSubSurface;

protected:
  virtual bool linkExteriorSurface(openstudio::model::ThermalZone zone, openstudio::model::Space space, openstudio::model::Surface surface) override
  {
    exteriorSurface++;
    return true;
  }
  virtual bool linkExteriorSubSurface(openstudio::model::ThermalZone zone, openstudio::model::Space space, openstudio::model::Surface surface, openstudio::model::SubSurface subSurface) override
  {
    exteriorSubSurface++;
    return true;
  }
  virtual bool linkInteriorSurface(openstudio::model::ThermalZone zone, openstudio::model::Space space, openstudio::model::Surface surface,
    openstudio::model::Surface adjacentSurface, openstudio::model::Space adjacentSpace, openstudio::model::ThermalZone adjacentZone) override
  {
    interiorSurface++;
    return true;
  }
  virtual bool linkInteriorSubSurface(openstudio::model::ThermalZone zone, openstudio::model::Space space, openstudio::model::Surface surface, openstudio::model::SubSurface subSurface,
    openstudio::model::SubSurface adjacentSubSurface, openstudio::model::Surface adjacentSurface, openstudio::model::Space adjacentSpace, openstudio::model::ThermalZone adjacentZone) override
  {
    interiorSubSurface++;
    return true;
  }

};

TEST_F(AirflowFixture, SurfaceNetworkBuilder_DemoModel_2012)
{
  openstudio::path modelPath = (resourcesPath() / openstudio::toPath("contam") / openstudio::toPath("CONTAMTemplate.osm"));
  openstudio::osversion::VersionTranslator vt;
  boost::optional<openstudio::model::Model> optionalModel = vt.loadModel(modelPath);
  ASSERT_TRUE(optionalModel);
  openstudio::model::Model model = optionalModel.get();

  boost::optional<openstudio::model::Model> demoModel = buildDemoModel2012(model);

  ASSERT_TRUE(demoModel);

  SurfaceCounter counter;
  counter.build(demoModel.get());

  EXPECT_EQ(5, counter.interiorSurface);
  EXPECT_EQ(12, counter.exteriorSurface);
  EXPECT_EQ(0, counter.interiorSubSurface);
  EXPECT_EQ(0, counter.exteriorSubSurface);

}

TEST_F(AirflowFixture, SurfaceNetworkBuilder_DemoModel_2014)
{
  openstudio::path modelPath = (resourcesPath() / openstudio::toPath("contam") / openstudio::toPath("CONTAMTemplate.osm"));
  openstudio::osversion::VersionTranslator vt;
  boost::optional<openstudio::model::Model> optionalModel = vt.loadModel(modelPath);
  ASSERT_TRUE(optionalModel);
  openstudio::model::Model model = optionalModel.get();

  boost::optional<openstudio::model::Model> demoModel = buildDemoModel2014(model);

  ASSERT_TRUE(demoModel);

  SurfaceCounter counter;
  EXPECT_TRUE(counter.build(demoModel.get()));

  EXPECT_EQ(5, counter.interiorSurface);
  EXPECT_EQ(12, counter.exteriorSurface);
  EXPECT_EQ(3, counter.interiorSubSurface);
  EXPECT_EQ(28, counter.exteriorSubSurface);
}