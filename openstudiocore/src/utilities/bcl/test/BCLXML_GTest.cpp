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
#include "BCLFixture.hpp"
#include <resources.hxx>

#include "../BCLXML.hpp"

using namespace openstudio;

TEST_F(BCLFixture, BCLXML)
{
  openstudio::path path = resourcesPath() / toPath("/utilities/BCL/Measures/SetWindowToWallRatioByFacade/measure.xml");
  boost::optional<BCLXML> bclXML = BCLXML::load(path);
  ASSERT_TRUE(bclXML);
}

TEST_F(BCLFixture, BCLXML_New)
{
  BCLXML bclXML(BCLXMLType::MeasureXML);
  EXPECT_FALSE(bclXML.uid().empty());
  EXPECT_FALSE(bclXML.versionId().empty());
  EXPECT_TRUE(bclXML.name().empty());
  EXPECT_TRUE(bclXML.description().empty());
  EXPECT_TRUE(bclXML.modelerDescription().empty());
  EXPECT_TRUE(bclXML.path().empty());
  EXPECT_TRUE(bclXML.directory().empty());
  EXPECT_TRUE(bclXML.attributes().empty());
  EXPECT_TRUE(bclXML.tags().empty());
  EXPECT_TRUE(bclXML.files().empty());

  std::string versionId = bclXML.versionId();
  bclXML.setName("Name");
  EXPECT_EQ("Name", bclXML.name());
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  bclXML.setDescription("Description");
  EXPECT_EQ("Description", bclXML.description());
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  bclXML.setModelerDescription("ModelerDescription");
  EXPECT_EQ("ModelerDescription", bclXML.modelerDescription());
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  bclXML.addTag("tag");
  ASSERT_EQ(1u, bclXML.tags().size());
  EXPECT_EQ("tag", bclXML.tags()[0]);
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  bclXML.addTag("tag");
  ASSERT_EQ(1u, bclXML.tags().size());
  EXPECT_EQ("tag", bclXML.tags()[0]);
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  Attribute attr("attribute", 1.0);
  bclXML.addAttribute(attr);
  ASSERT_EQ(1u, bclXML.attributes().size());
  EXPECT_EQ("attribute", bclXML.attributes()[0].name());
  EXPECT_TRUE(bclXML.getAttribute("attribute"));
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  attr = Attribute("attribute", 2.0);
  bclXML.addAttribute(attr);
  ASSERT_EQ(1u, bclXML.attributes().size());
  EXPECT_EQ("attribute", bclXML.attributes()[0].name());
  EXPECT_TRUE(bclXML.getAttribute("attribute"));
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  EXPECT_FALSE(bclXML.save());
  EXPECT_EQ(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  openstudio::path path = toPath("./measure.xml");
  EXPECT_TRUE(bclXML.saveAs(path));
  EXPECT_NE(versionId, bclXML.versionId());

  versionId = bclXML.versionId();
  EXPECT_TRUE(bclXML.save());
  EXPECT_EQ(versionId, bclXML.versionId());

  boost::optional<BCLXML> copy = BCLXML::load(path);
  ASSERT_TRUE(copy);
}
