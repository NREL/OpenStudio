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
#include "gbXMLFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"

#include <resources.hxx>

#include <sstream>

using namespace openstudio::model;
using namespace openstudio::gbxml;
using namespace openstudio;

TEST_F(gbXMLFixture, ForwardTranslator_exampleModel)
{
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
