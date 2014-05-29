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

TEST_F(SDDFixture, ForwardTranslator_exampleModel)
{
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

  for (SubSurface subSurface : model.getModelObjects<SubSurface>()){
    if ((subSurface.subSurfaceType() == "FixedWindow") || (subSurface.subSurfaceType() == "OperableWindow")){
      subSurface.setConstruction(windowConstruction);
    }else if ((subSurface.subSurfaceType() == "Door") || (subSurface.subSurfaceType() == "OverheadDoor")){
      subSurface.setConstruction(doorConstruction);
    }
  }

  path p = resourcesPath() / openstudio::toPath("sdd/exampleModel.xml");

  ForwardTranslator forwardTranslator;
  bool test = forwardTranslator.modelToSDD(model, p);

  EXPECT_TRUE(test);

  ReverseTranslator reverseTranslator;
  boost::optional<Model> model2 = reverseTranslator.loadModel(p);

  ASSERT_TRUE(model2);

  path p1 = resourcesPath() / openstudio::toPath("sdd/exampleModel.osm");
  model.save(p1, true);

  path p2 = resourcesPath() / openstudio::toPath("sdd/exampleModel2.osm");
  model2->save(p2, true);
}
