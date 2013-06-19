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

#include <simxml/ReverseTranslator.hpp>
#include <simxml/ForwardTranslator.hpp>

#include <model/Model.hpp>

#include <resources.hxx>

TEST_F(SimXMLFixture, ReverseTranslator)
{
  openstudio::path inputPath = resourcesPath() / openstudio::toPath("simxml/ClassLevel_Test-3.xml");

  boost::optional<openstudio::model::Model> model = openstudio::simxml::ReverseTranslator::SimXmlToModel(inputPath);
  ASSERT_TRUE(model);
  model->save(resourcesPath() / openstudio::toPath("simxml/ClassLevel_Test-3.osm"), true);
  
  openstudio::path outputPath = resourcesPath() / openstudio::toPath("simxml/ClassLevel_Test-3-out.xml");
  ::simxml::SimXmlDoc simXmlDoc = openstudio::simxml::ForwardTranslator::modelToSimXml(*model);
  simXmlDoc.write(openstudio::toString(outputPath));
}
