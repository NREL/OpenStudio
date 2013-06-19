/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <simxml/Test/SimXMLFixture.hpp>

#include <simxml/ForwardTranslator.hpp>
#include <simxml/ReverseTranslator.hpp>

#include <model/Model.hpp>

#include <resources.hxx>

TEST_F(SimXMLFixture, ForwardTranslator)
{
  openstudio::path modelPath = resourcesPath() / openstudio::toPath("simxml/ExampleModel.osm");
  openstudio::model::Model model = openstudio::model::exampleModel();
  model.save(modelPath, true);

  openstudio::path simXmlPath = resourcesPath() / openstudio::toPath("simxml/ExampleModel.xml");
  ::simxml::SimXmlDoc simXmlDoc = openstudio::simxml::ForwardTranslator::modelToSimXml(model);
  simXmlDoc.write(openstudio::toString(simXmlPath));

  openstudio::path model2Path = resourcesPath() / openstudio::toPath("simxml/ExampleModel2.osm");
  boost::optional<openstudio::model::Model> model2 = openstudio::simxml::ReverseTranslator::SimXmlToModel(simXmlPath);
  ASSERT_TRUE(model2);
  model2->save(model2Path, true);
}
