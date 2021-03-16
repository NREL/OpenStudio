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

  for (SubSurface subSurface : model.getModelObjects<SubSurface>()) {
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
