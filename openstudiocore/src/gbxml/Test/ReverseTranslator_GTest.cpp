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
#include "gbXMLFixture.hpp"

#include "../ReverseTranslator.hpp"
#include "../ForwardTranslator.hpp"

#include "../../energyplus/ForwardTranslator.hpp"

#include "../../model/Model.hpp"
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

#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/core/Optional.hpp"

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
