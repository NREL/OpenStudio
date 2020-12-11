/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/Facility.hpp"
#include "../../model/Facility_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/MasslessOpaqueMaterial.hpp"
#include "../../model/MasslessOpaqueMaterial_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include "../../model/Model.hpp"
#include "utilities/core/Compare.hpp"

#include <resources.hxx>

#include <sstream>
#include <iostream>
#include <algorithm>
#include <pugixml.hpp>

using namespace openstudio::model;
using namespace openstudio::gbxml;
using namespace openstudio;

TEST_F(gbXMLFixture, ForwardTranslator_exampleModel) {
  Model model = exampleModel();

  path p = resourcesPath() / openstudio::toPath("gbxml/exampleModel.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  ReverseTranslator reverseTranslator;
  boost::optional<Model> model2 = reverseTranslator.loadModel(p);

  ASSERT_TRUE(model2);

  path p1 = resourcesPath() / openstudio::toPath("gbxml/exampleModel.osm");
  model.save(p1, true);

  path p2 = resourcesPath() / openstudio::toPath("gbxml/exampleModel2.osm");
  model2->save(p2, true);
}

TEST_F(gbXMLFixture, ForwardTranslator_AdiabaticSurface) {
  Model model = exampleModel();

  std::string surfname("Adiabatic_Surface");

  // Find a surface, make it adiabatic
  for (auto& surf : model.getModelObjects<Surface>()) {
    if (surf.outsideBoundaryCondition() == "Outdoors") {
      surf.setOutsideBoundaryCondition("Adiabatic");
      surf.setSunExposure("NoSun");
      surf.setWindExposure("NoWind");
      surf.setName(surfname);
      break;
    }
  }

  // Write out the XML
  path p = resourcesPath() / openstudio::toPath("gbxml/exampleModelAdiabatic.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  // Read the XML back in and check the surface
  ReverseTranslator reverseTranslator;
  boost::optional<Model> model2 = reverseTranslator.loadModel(p);

  ASSERT_TRUE(model2);

  auto osurf = model2->getModelObjectByName<Surface>(surfname);
  ASSERT_TRUE(osurf);
  EXPECT_EQ("Adiabatic", osurf->outsideBoundaryCondition());
  EXPECT_EQ("NoSun", osurf->sunExposure());
  EXPECT_EQ("NoWind", osurf->windExposure());
}

TEST_F(gbXMLFixture, ForwardTranslator_ConstructionLayers) {
  Model model;

  Construction construction(model);
  construction.setName("Construction1");

  MaterialVector layers;

  MasslessOpaqueMaterial material1(model);
  material1.setName("Material1");
  layers.push_back(material1);

  StandardOpaqueMaterial material2(model);
  material2.setName("Material2");
  layers.push_back(material2);

  MasslessOpaqueMaterial material3(model);
  material3.setName("Material3");
  layers.push_back(material3);

  StandardOpaqueMaterial material4(model);
  material4.setName("Material4");
  layers.push_back(material4);

  construction.setLayers(layers);

  Facility facility = model.getUniqueModelObject<Facility>();

  Building building = model.getUniqueModelObject<Building>();

  Space space(model);
  space.setName("Space1");

  Point3dVector points;
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 0, 1));

  //std::string surfname("Surface 1"); // DLM: note this will fail because "Surface 1" gets round tripped as "Surface_1"
  std::string surfname("Surface1");
  Surface surface(points, model);
  surface.setName(surfname);
  surface.setConstruction(construction);
  surface.setSpace(space);

  ThermalZone zone(model);
  space.setThermalZone(zone);

  // Write out the XML
  path p = resourcesPath() / openstudio::toPath("gbxml/ForwardTranslator_ConstructionLayers.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  // Read the XML back in and check the surface
  ReverseTranslator reverseTranslator;
  boost::optional<Model> model2 = reverseTranslator.loadModel(p);

  ASSERT_TRUE(model2);
  //std::cout << *model2 << std::endl;
  auto osurf = model2->getModelObjectByName<Surface>(surfname);
  ASSERT_TRUE(osurf);
  auto ocons = osurf->construction();
  ASSERT_TRUE(ocons);
  auto olayeredcons = ocons->optionalCast<Construction>();
  ASSERT_TRUE(olayeredcons);
  ASSERT_EQ(4u, olayeredcons->layers().size());
  EXPECT_TRUE(olayeredcons->layers()[0].optionalCast<MasslessOpaqueMaterial>());
  EXPECT_TRUE(olayeredcons->layers()[1].optionalCast<StandardOpaqueMaterial>());
  EXPECT_TRUE(olayeredcons->layers()[2].optionalCast<MasslessOpaqueMaterial>());
  EXPECT_TRUE(olayeredcons->layers()[3].optionalCast<StandardOpaqueMaterial>());
}

TEST_F(gbXMLFixture, ForwardTranslator_NoFacility) {
  // Test for #3314: gbXML translation does not roundtrip unless Facility object present

  Model model;

  Construction construction(model);
  construction.setName("Construction1");

  MaterialVector layers;

  MasslessOpaqueMaterial material1(model);
  material1.setName("Material1");
  layers.push_back(material1);

  StandardOpaqueMaterial material2(model);
  material2.setName("Material2");
  layers.push_back(material2);

  MasslessOpaqueMaterial material3(model);
  material3.setName("Material3");
  layers.push_back(material3);

  StandardOpaqueMaterial material4(model);
  material4.setName("Material4");
  material4.setRoughness("MediumSmooth");
  layers.push_back(material4);

  construction.setLayers(layers);

  // Not instantiating facility nor building on purpose
  // Facility facility = model.getUniqueModelObject<Facility>();
  // Building building = model.getUniqueModelObject<Building>();

  Space space(model);
  space.setName("Space1");

  Point3dVector points;
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 0, 1));

  //std::string surfname("Surface 1"); // DLM: note this will fail because "Surface 1" gets round tripped as "Surface_1"
  std::string surfname("Surface1");
  Surface surface(points, model);
  surface.setName(surfname);
  surface.setConstruction(construction);
  surface.setSpace(space);

  ThermalZone zone(model);
  zone.setName("Zone1");
  space.setThermalZone(zone);

  // save model for diffing
  bool debug = false;
  if (debug) {
    path modelPath = resourcesPath() / openstudio::toPath("gbxml/ForwardTranslator_NoFacility_original.osm");
    model.save(modelPath, true);
  }

  // Write out the XML
  path p = resourcesPath() / openstudio::toPath("gbxml/ForwardTranslator_NoFacility.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  // Read the XML back in and check surface/space/zone were all translated
  ReverseTranslator reverseTranslator;
  boost::optional<Model> model2 = reverseTranslator.loadModel(p);

  ASSERT_TRUE(model2);
  //std::cout << *model2 << std::endl;
  auto osurf = model2->getModelObjectByName<Surface>(surfname);
  ASSERT_TRUE(osurf);
  auto ospace = model2->getModelObjectByName<Space>(space.nameString());
  ASSERT_TRUE(ospace);
  auto ozone = model2->getModelObjectByName<ThermalZone>(zone.nameString());  // Dragostea Din Tei!
  ASSERT_TRUE(ozone);

  // This really tests a RT feature, but doesn't really matter. When diffing original & rountripped, I noticed a diff in Material:
  // the roundtripped model has Roughness missing
  auto omat = model2->getModelObjectByName<StandardOpaqueMaterial>("Material4");
  ASSERT_TRUE(omat);
  EXPECT_EQ("MediumSmooth", omat->roughness());

  if (debug) {
    path modelPath2 = resourcesPath() / openstudio::toPath("gbxml/ForwardTranslator_NoFacility_roundtripped.osm");
    model2->save(modelPath2, true);
  }
}

TEST_F(gbXMLFixture, ForwardTranslator_surfaceType_4001) {
  // Test for #4001 : surfaceType is written twice when SlabOnGrade
  Model model = exampleModel();

  // Write out the XML
  path p = resourcesPath() / openstudio::toPath("gbxml/exampleModel.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToGbXML(model, p);

  EXPECT_TRUE(test);

  ASSERT_TRUE(openstudio::filesystem::exists(p));

  openstudio::filesystem::ifstream file(p, std::ios_base::binary);
  ASSERT_TRUE(file.is_open());
  pugi::xml_document doc;
  auto load_result = doc.load(file);
  file.close();
  ASSERT_TRUE(load_result) << "'" << p << "' Failed to load:\n"
                           << "Error description: " << load_result.description() << "\n"
                           << "Error offset: " << load_result.offset;

  // Now go select the Surface_1, which currently ends up with two surfaceType attributes before fix for #4001
  pugi::xpath_node surfaceXPath = doc.select_node("//Surface[@id='Surface_1']");
  ASSERT_TRUE(surfaceXPath);
  pugi::xml_node surfaceNode = surfaceXPath.node();
  EXPECT_EQ(1u, std::count_if(surfaceNode.attributes_begin(), surfaceNode.attributes_end(),
                              [](const auto& att) { return openstudio::istringEqual(att.name(), "surfaceType"); }));
  std::string surfaceType = surfaceNode.attribute("surfaceType").value();
  std::string expectedSurfaceType("SlabOnGrade");
  EXPECT_EQ(expectedSurfaceType, surfaceType);
}

TEST_F(gbXMLFixture, ForwardTranslator_exampleModel_State) {
  // Test for #4135: translating a model twice should produce the same result
  Model model = exampleModel();

  ForwardTranslator forwardTranslator;
  std::string gbXML_str1 = forwardTranslator.modelToGbXMLString(model);
  EXPECT_FALSE(gbXML_str1.empty());

  std::string gbXML_str2 = forwardTranslator.modelToGbXMLString(model);
  EXPECT_FALSE(gbXML_str2.empty());

  EXPECT_EQ(gbXML_str1.length(), gbXML_str2.length());
  EXPECT_GT(gbXML_str1.length(), 50000);
}
