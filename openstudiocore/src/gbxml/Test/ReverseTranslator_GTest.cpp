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
#include "gbXMLFixture.hpp"

#include "../ReverseTranslator.hpp"
#include "../ForwardTranslator.hpp"

#include "../../energyplus/ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/ModelMerger.hpp"
#include "../../model/Facility.hpp"
#include "../../model/Facility_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/core/Optional.hpp"

#include <utilities/idd/OS_Surface_FieldEnums.hxx>
#include <utilities/idd/OS_SubSurface_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(gbXMLFixture, ReverseTranslator_ZNETH)
{
  //openstudio::Logger::instance().standardOutLogger().enable();
  //openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/ZNETH.xml");
  openstudio::path outputPath = resourcesPath() / openstudio::toPath("gbxml/ZNETH2.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  model->save(resourcesPath() / openstudio::toPath("gbxml/ZNETH.osm"), true);

  // add test to see that surfaces that reference two spaces get "surface" boundary condition
  // e.g. surface named "su-76" should have "Surface" string for OutsideBoundaryCondition
  //OptionalSurface osurf = model->getModelObjectByName<Surface>("su-76"); // su-76 is the id
  OptionalSurface osurf = model->getModelObjectByName<Surface>("B-101-201-I-F-76"); // B-101-201-I-F-76 is the name
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Surface",osurf->outsideBoundaryCondition());

  openstudio::energyplus::ForwardTranslator energyPlusTranslator;
  openstudio::Workspace workspace = energyPlusTranslator.translateModel(*model);

  EXPECT_TRUE(workspace.numObjects() > 0);

  workspace.save(resourcesPath() / openstudio::toPath("gbxml/ZNETH.idf"), true);

  openstudio::gbxml::ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(*model, outputPath);
  EXPECT_TRUE(test);
}

TEST_F(gbXMLFixture, ReverseTranslator_Constructions)
{

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/TestCube.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  model->save(resourcesPath() / openstudio::toPath("gbxml/TestCube.osm"), true);

  auto osurf = model->getModelObjectByName<Surface>("T-1-5-I-F-6 Reversed");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Surface", osurf->outsideBoundaryCondition());

  auto oconstruct = osurf->construction();
  ASSERT_TRUE(oconstruct);
  EXPECT_EQ("Floor: Floor 1", oconstruct->name().get());

  int count = 0;
  for (auto &srf : model->getModelObjects<Surface>()) {
    if (srf.outsideBoundaryCondition() != "Surface") {
      continue;
    }
    if (srf.name().get().find("Reversed") == std::string::npos) {
      auto other_name = srf.name().get() + " Reversed";
      auto other_surf = model->getModelObjectByName<Surface>(other_name);
      ASSERT_TRUE(other_surf);

      // the construction will be assigned to one of these surfaces, the other surface will have an empty construction
      // the reversed construction will be created and assigned at translation time
      auto srfConstructionName = srf.getString(OS_SurfaceFields::ConstructionName, false, true);
      auto other_srfConstructionName = other_surf->getString(OS_SurfaceFields::ConstructionName, false, true);
      if (srfConstructionName) {
        EXPECT_FALSE(other_srfConstructionName);
        auto oc = srf.construction();
        ASSERT_TRUE(oc);
        EXPECT_EQ(oc->name().get(), srfConstructionName.get());
      } else {
        EXPECT_FALSE(srfConstructionName);
        auto oc = other_surf->construction();
        ASSERT_TRUE(oc);
        EXPECT_EQ(oc->name().get(), other_srfConstructionName.get());
      }
      ++count;
    }
  }
  EXPECT_EQ(20, count);

}

TEST_F(gbXMLFixture, ReverseTranslator_SubSurfaceConstructions)
{

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/seb.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  model->save(resourcesPath() / openstudio::toPath("gbxml/seb.osm"), true);

  auto osurf = model->getModelObjectByName<SubSurface>("Sub_Surface_8");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Outdoors", osurf->outsideBoundaryCondition());

  auto oconstruct = osurf->construction();
  ASSERT_TRUE(oconstruct);
  EXPECT_EQ("3\'0\" x 3\'0\" Double pane  Alum Construction", oconstruct->name().get());
  auto ofield = osurf->getString(OS_SubSurfaceFields::ConstructionName);
  ASSERT_TRUE(ofield);
  EXPECT_EQ("3\'0\" x 3\'0\" Double pane  Alum Construction", ofield.get());

  int count = 0;
  for (auto &srf : model->getModelObjects<SubSurface>()) {
    auto oc = srf.construction();
    ASSERT_TRUE(oc);
    ofield = srf.getString(OS_SubSurfaceFields::ConstructionName);
    ASSERT_TRUE(ofield);
    EXPECT_EQ(oc->name().get(), ofield.get());
    ++count;
  }
  EXPECT_EQ(8, count);

}

TEST_F(gbXMLFixture, ReverseTranslator_UndergroundWalls)
{

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/TestCube.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  // Check all the surfaces that are supposed to be underground
  OptionalSurface osurf = model->getModelObjectByName<Surface>("S-3-U-W-12");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  osurf = model->getModelObjectByName<Surface>("S-2-U-W-8");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  osurf = model->getModelObjectByName<Surface>("E-2-U-W-7");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  osurf = model->getModelObjectByName<Surface>("E-1-U-W-1");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  osurf = model->getModelObjectByName<Surface>("N-1-U-W-2");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  osurf = model->getModelObjectByName<Surface>("N-4-U-W-18");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  osurf = model->getModelObjectByName<Surface>("W-4-U-W-17");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  osurf = model->getModelObjectByName<Surface>("W-3-U-W-13");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Ground", osurf->outsideBoundaryCondition());

  // Count the underground surfaces
  int count = 0;
  for (auto &surf : model->getModelObjects<Surface>()) {
    if (surf.outsideBoundaryCondition() == "Ground") {
      count += 1;
    }
  }
  EXPECT_EQ(12, count); // 4 slabs + 8 walls
}

TEST_F(gbXMLFixture, ReverseTranslator_FloorSurfaces)
{

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/TestCube.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  // Check all the surfaces that are supposed to be floors and ceilings
  OptionalSurface osurf = model->getModelObjectByName<Surface>("T-1-5-I-F-6");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Floor", osurf->surfaceType());
  auto space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("5 Space", space->name().get());

  osurf = model->getModelObjectByName<Surface>("T-1-5-I-F-6 Reversed");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("RoofCeiling", osurf->surfaceType());
  space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("1 Space", space->name().get());

  osurf = model->getModelObjectByName<Surface>("T-2-6-I-F-11");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Floor", osurf->surfaceType());
  space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("6 Space", space->name().get());

  osurf = model->getModelObjectByName<Surface>("T-2-6-I-F-11 Reversed");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("RoofCeiling", osurf->surfaceType());
  space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("2 Space", space->name().get());

  osurf = model->getModelObjectByName<Surface>("T-3-7-I-F-16");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Floor", osurf->surfaceType());
  space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("7 Space", space->name().get());

  osurf = model->getModelObjectByName<Surface>("T-3-7-I-F-16 Reversed");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("RoofCeiling", osurf->surfaceType());
  space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("3 Space", space->name().get());

  osurf = model->getModelObjectByName<Surface>("T-4-8-I-F-20");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Floor", osurf->surfaceType());
  space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("8 Space", space->name().get());

  osurf = model->getModelObjectByName<Surface>("T-4-8-I-F-20 Reversed");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("RoofCeiling", osurf->surfaceType());
  space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("4 Space", space->name().get());

  osurf = model->getModelObjectByName<Surface>("T-5-9-I-F-25");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Floor", osurf->surfaceType());
  space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("9 Space", space->name().get());

  osurf = model->getModelObjectByName<Surface>("T-5-9-I-F-25 Reversed");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("RoofCeiling", osurf->surfaceType());
  space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("5 Space", space->name().get());

  osurf = model->getModelObjectByName<Surface>("T-6-10-I-F-29");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Floor", osurf->surfaceType());
  space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("10 Space", space->name().get());

  osurf = model->getModelObjectByName<Surface>("T-6-10-I-F-29 Reversed");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("RoofCeiling", osurf->surfaceType());
  space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("6 Space", space->name().get());

  osurf = model->getModelObjectByName<Surface>("T-7-11-I-F-33");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Floor", osurf->surfaceType());
  space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("11 Space", space->name().get());

  osurf = model->getModelObjectByName<Surface>("T-7-11-I-F-33 Reversed");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("RoofCeiling", osurf->surfaceType());
  space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("7 Space", space->name().get());

  osurf = model->getModelObjectByName<Surface>("T-8-12-I-F-36");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Floor", osurf->surfaceType());
  space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("12 Space", space->name().get());

  osurf = model->getModelObjectByName<Surface>("T-8-12-I-F-36 Reversed");
  ASSERT_TRUE(osurf);
  EXPECT_EQ("RoofCeiling", osurf->surfaceType());
  space = osurf->space();
  ASSERT_TRUE(space);
  EXPECT_EQ("8 Space", space->name().get());

}

TEST_F(gbXMLFixture, ReverseTranslator_AlternateUnits)
{
  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/TestCubeAlternateUnits.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  auto surfs = model->getModelObjects<Surface>();

  OptionalSurface osurf = model->getModelObjectByName<Surface>("T-1-5-I-F-6");
  ASSERT_TRUE(osurf);
  auto points = osurf->vertices();
  ASSERT_EQ(4, points.size());
  EXPECT_EQ(0.9144, points[1].y());

  //auto omat = model->getModelObjectByName<StandardOpaqueMaterial>("Concrete: 100 [mm]");
  //ASSERT_TRUE(omat);
  //EXPECT_NEAR(0.07407407, omat->thermalResistance(), 1.0e-8);
  //EXPECT_NEAR(1570.0, omat->density(), 1.0e-8);
  //EXPECT_NEAR(1.35, omat->conductivity(), 1.0e-8);
  //EXPECT_NEAR(0.1, omat->thickness(), 1.0e-8);
  //EXPECT_NEAR(840.0, omat->specificHeat(), 1.0e-8);
  //omat = model->getModelObjectByName<StandardOpaqueMaterial>("RockWool: 50 [mm]");
  //ASSERT_TRUE(omat);
  //EXPECT_NEAR(1.470588, omat->thermalResistance(), 1.0e-8);
  //EXPECT_NEAR(200.0, omat->density(), 1.0e-8);
  //EXPECT_NEAR(0.034, omat->conductivity(), 1.0e-8);
  //EXPECT_NEAR(0.05, omat->thickness(), 1.0e-8);
  //EXPECT_NEAR(710.0, omat->specificHeat(), 1.0e-8);
  //omat = model->getModelObjectByName<StandardOpaqueMaterial>("Concrete: 150 [mm]");
  //ASSERT_TRUE(omat);
  //EXPECT_NEAR(0.1111111, omat->thermalResistance(), 1.0e-8);
  //EXPECT_NEAR(1570.0, omat->density(), 1.0e-8);
  //EXPECT_NEAR(1.35, omat->conductivity(), 1.0e-8);
  //EXPECT_NEAR(0.15, omat->thickness(), 1.0e-8);
  //EXPECT_NEAR(840.0, omat->specificHeat(), 1.0e-8);

}

TEST_F(gbXMLFixture, ReverseTranslator_HandleMapping)
{
  //openstudio::Logger::instance().standardOutLogger().enable();
  //openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/ZNETH.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model1 = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model1);

  unsigned numSpaces = model1->getConcreteModelObjects<model::Space>().size();
  unsigned numSurfaces = model1->getConcreteModelObjects<model::Surface>().size();
  unsigned numPlanarSurfaces = model1->getModelObjects<model::PlanarSurface>().size();
  EXPECT_NE(0, numSpaces);
  EXPECT_NE(0, numSurfaces);
  EXPECT_NE(0, numPlanarSurfaces);

  boost::optional<openstudio::model::Model> model2 = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model2);
  EXPECT_EQ(numSpaces, model2->getConcreteModelObjects<model::Space>().size());
  EXPECT_EQ(numSurfaces, model2->getConcreteModelObjects<model::Surface>().size());
  EXPECT_EQ(numPlanarSurfaces, model2->getModelObjects<model::PlanarSurface>().size());

  for (const auto& object : model2->objects()) {
    EXPECT_FALSE(model1->getObject(object.handle()));
  }

  model::Model model1Clone1 = model1->clone(false).cast<model::Model>();
  model::Model model1Clone2 = model1->clone(false).cast<model::Model>();

  for (const auto& object : model1Clone1.objects()) {
    EXPECT_FALSE(model1->getObject(object.handle()));
    EXPECT_FALSE(model2->getObject(object.handle()));
    EXPECT_FALSE(model1Clone2.getObject(object.handle()));
  }

  model::ModelMerger mm;

  // no mapping merge, objects not in mapping are removed
  mm.mergeModels(model1Clone1, *model2, std::map<UUID, UUID>());
  EXPECT_EQ(numSpaces, model2->getConcreteModelObjects<model::Space>().size());
  EXPECT_EQ(numSurfaces, model2->getConcreteModelObjects<model::Surface>().size());
  EXPECT_EQ(numPlanarSurfaces, model2->getModelObjects<model::PlanarSurface>().size());
  EXPECT_EQ(numSpaces, model1Clone1.getConcreteModelObjects<model::Space>().size());
  EXPECT_EQ(numSurfaces, model1Clone1.getConcreteModelObjects<model::Surface>().size());
  EXPECT_EQ(numPlanarSurfaces, model1Clone1.getModelObjects<model::PlanarSurface>().size());

  // merge with mapping
  const auto mapping = mm.suggestHandleMapping(model1Clone2, *model2);
  for (const auto& space : model1Clone2.getConcreteModelObjects<model::Space>()) {
    const auto it = mapping.find(space.handle());
    // spaces are mapped
    ASSERT_TRUE(it != mapping.end());
    EXPECT_TRUE(model2->getObject(it->second));
  }
  for (const auto& planarSurface : model1Clone2.getModelObjects<model::PlanarSurface>()) {
    const auto it = mapping.find(planarSurface.handle());
    // surfaces are not mapped
    EXPECT_TRUE(it == mapping.end());
  }
  mm.mergeModels(model1Clone2, *model2, mapping);
  EXPECT_EQ(numSpaces, model2->getConcreteModelObjects<model::Space>().size());
  EXPECT_EQ(numSurfaces, model2->getConcreteModelObjects<model::Surface>().size());
  EXPECT_EQ(numPlanarSurfaces, model2->getModelObjects<model::PlanarSurface>().size());
  EXPECT_EQ(numSpaces, model1Clone2.getConcreteModelObjects<model::Space>().size());
  EXPECT_EQ(numSurfaces, model1Clone2.getConcreteModelObjects<model::Surface>().size());
  EXPECT_EQ(numPlanarSurfaces, model1Clone2.getModelObjects<model::PlanarSurface>().size());
}

TEST_F(gbXMLFixture, ReverseTranslator_SimpleBox_Vasari)
{
  //openstudio::Logger::instance().standardOutLogger().enable();
  //openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/simpleBox_vasari.xml");
  openstudio::path outputPath = resourcesPath() / openstudio::toPath("gbxml/simpleBox_vasari2.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  model->save(resourcesPath() / openstudio::toPath("gbxml/simpleBox_vasari.osm"), true);

  openstudio::energyplus::ForwardTranslator energyPlusTranslator;
  openstudio::Workspace workspace = energyPlusTranslator.translateModel(*model);

  EXPECT_TRUE(workspace.numObjects() > 0);

  workspace.save(resourcesPath() / openstudio::toPath("gbxml/simpleBox_vasari.idf"), true);

  openstudio::gbxml::ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(*model, outputPath);
  EXPECT_TRUE(test);
}

TEST_F(gbXMLFixture, ReverseTranslator_TwoStoryOffice_Trane)
{
  //openstudio::Logger::instance().standardOutLogger().enable();
  //openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);

  openstudio::path inputPath = resourcesPath() / openstudio::toPath("gbxml/TwoStoryOffice_Trane.xml");
  openstudio::path outputPath = resourcesPath() / openstudio::toPath("gbxml/TwoStoryOffice_Trane2.xml");

  openstudio::gbxml::ReverseTranslator reverseTranslator;
  boost::optional<openstudio::model::Model> model = reverseTranslator.loadModel(inputPath);
  ASSERT_TRUE(model);

  model->save(resourcesPath() / openstudio::toPath("gbxml/TwoStoryOffice_Trane.osm"), true);

  openstudio::energyplus::ForwardTranslator energyPlusTranslator;
  openstudio::Workspace workspace = energyPlusTranslator.translateModel(*model);

  EXPECT_TRUE(workspace.numObjects() > 0);

  workspace.save(resourcesPath() / openstudio::toPath("gbxml/TwoStoryOffice_Trane.idf"), true);

  openstudio::gbxml::ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(*model, outputPath);
  EXPECT_TRUE(test);
}
