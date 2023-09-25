/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "SDDFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SimpleGlazing.hpp"
#include "../../model/MasslessOpaqueMaterial.hpp"
#include "../../model/Construction.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio::model;
using namespace openstudio::sdd;
using namespace openstudio;

TEST_F(SDDFixture, ForwardTranslator_exampleModel) {
  Model model = exampleModel();

  // change to constructions that can be translated to sdd
  SimpleGlazing simpleGlazing(model);
  simpleGlazing.setName("Test Glazing");
  simpleGlazing.setSolarHeatGainCoefficient(0.5);
  simpleGlazing.setUFactor(2);
  simpleGlazing.setVisibleTransmittance(0.7);

  MaterialVector windowLayers;
  windowLayers.push_back(simpleGlazing);

  Construction windowConstruction(model);
  windowConstruction.setLayers(windowLayers);

  MasslessOpaqueMaterial doorLayer(model);
  doorLayer.setName("Test Door");

  MaterialVector doorLayers;
  doorLayers.push_back(doorLayer);

  Construction doorConstruction(model);
  doorConstruction.setLayers(doorLayers);

  for (SubSurface subSurface : model.getConcreteModelObjects<SubSurface>()) {
    if ((subSurface.subSurfaceType() == "FixedWindow") || (subSurface.subSurfaceType() == "OperableWindow")) {
      subSurface.setConstruction(windowConstruction);
    } else if ((subSurface.subSurfaceType() == "Door") || (subSurface.subSurfaceType() == "OverheadDoor")) {
      subSurface.setConstruction(doorConstruction);
    }
  }

  // write out the input xml
  path p = resourcesPath() / openstudio::toPath("sdd/exampleModel.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToSDD(model, p);

  EXPECT_TRUE(test);

  // DLM: the forward translator writes out an input SDD, the reverse translator reads a sim SDD
  // these two are not compatible and round trip is not supported

  //ReverseTranslator reverseTranslator;
  //boost::optional<Model> model2 = reverseTranslator.loadModel(p);

  //ASSERT_TRUE(model2);

  //path p1 = resourcesPath() / openstudio::toPath("sdd/exampleModel.osm");
  //model.save(p1, true);

  //path p2 = resourcesPath() / openstudio::toPath("sdd/exampleModel2.osm");
  //model2->save(p2, true);
}
