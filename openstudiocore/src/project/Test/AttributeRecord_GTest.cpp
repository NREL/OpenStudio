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
#include "ProjectFixture.hpp"
#include "../ProjectDatabase.hpp"
#include "../AnalysisRecord.hpp"
#include "../AnalysisRecord_Impl.hpp"
#include "../AttributeRecord.hpp"
#include "../AttributeRecord_Impl.hpp"
#include "../FileReferenceRecord.hpp"
#include "../FileReferenceRecord_Impl.hpp"

#include "../../model/Component.hpp"
#include "../../model/Component_Impl.hpp"
#include "../../model/ComponentData.hpp"
#include "../../model/ComponentData_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/data/EndUses.hpp"
#include "../../utilities/core/FileReference.hpp"

using namespace openstudio;
using namespace openstudio::project;

// this is a monster test that is more for testing the database than attributes
TEST_F(ProjectFixture, AttributeRecord_LoadSave)
{
  int id;
  UUID handle;
  Attribute attribute("name", 1.0, std::string("m"));
  {
    ProjectDatabase database = getCleanDatabase("AttributeRecord_LoadSave");

    EXPECT_EQ(static_cast<unsigned>(0), AttributeRecord::getAttributeRecords(database).size());

    FileReferenceRecord model(FileReference(toPath("./in.osm")),database);

    AttributeRecord attributeRecord(attribute,model);
    id = attributeRecord.id();
    handle = attributeRecord.handle();
    EXPECT_EQ("name", attributeRecord.name());
    EXPECT_EQ("", attributeRecord.displayName());
    EXPECT_EQ("", attributeRecord.description());
    EXPECT_EQ(AttributeValueType::Double, attributeRecord.attributeValueType().value());
    EXPECT_EQ(1.0, attributeRecord.attributeValueAsDouble());
    ASSERT_TRUE(attributeRecord.attributeUnits());
    EXPECT_EQ("m", attributeRecord.attributeUnits().get());

    EXPECT_TRUE(database.includesRecord(attributeRecord));
    EXPECT_TRUE(database.isNewRecord(attributeRecord));
    EXPECT_FALSE(database.isDirtyRecord(attributeRecord));
    EXPECT_FALSE(database.isCleanRecord(attributeRecord));
    EXPECT_FALSE(database.isRemovedRecord(attributeRecord));
    EXPECT_EQ(static_cast<unsigned>(2), attributeRecord.useCount());
  }

  {
    // open existing database
    ProjectDatabase database = getExistingDatabase("AttributeRecord_LoadSave");

    // new objects were removed
    EXPECT_EQ(static_cast<unsigned>(0), AnalysisRecord::getAnalysisRecords(database).size());
    EXPECT_EQ(static_cast<unsigned>(0), FileReferenceRecord::getFileReferenceRecords(database).size());
    EXPECT_EQ(static_cast<unsigned>(0), AttributeRecord::getAttributeRecords(database).size());

    FileReferenceRecord model(FileReference(toPath("./in.osm")),database);

    // create AttributeRecord
    AttributeRecord attributeRecord(attribute, model);
    id = attributeRecord.id();
    handle = attributeRecord.handle();
    EXPECT_EQ("name", attributeRecord.name());
    EXPECT_EQ("", attributeRecord.displayName());
    EXPECT_EQ("", attributeRecord.description());
    EXPECT_EQ(AttributeValueType::Double, attributeRecord.attributeValueType().value());
    EXPECT_EQ(1.0, attributeRecord.attributeValueAsDouble());
    ASSERT_TRUE(attributeRecord.attributeUnits());
    EXPECT_EQ("m", attributeRecord.attributeUnits().get());

    EXPECT_TRUE(database.includesRecord(attributeRecord));
    EXPECT_TRUE(database.isNewRecord(attributeRecord));
    EXPECT_FALSE(database.isDirtyRecord(attributeRecord));
    EXPECT_FALSE(database.isCleanRecord(attributeRecord));
    EXPECT_FALSE(database.isRemovedRecord(attributeRecord));
    EXPECT_EQ(static_cast<unsigned>(2), attributeRecord.useCount());

    // save to database
    EXPECT_TRUE(database.save());

    EXPECT_TRUE(database.includesRecord(attributeRecord));
    EXPECT_FALSE(database.isNewRecord(attributeRecord));
    EXPECT_FALSE(database.isDirtyRecord(attributeRecord));
    EXPECT_TRUE(database.isCleanRecord(attributeRecord));
    EXPECT_FALSE(database.isRemovedRecord(attributeRecord));
    EXPECT_EQ(static_cast<unsigned>(2), attributeRecord.useCount());
  }

  {
    // open existing database
    ProjectDatabase database = getExistingDatabase("AttributeRecord_LoadSave");

    // saved objects were not removed
    ASSERT_EQ(static_cast<unsigned>(1), FileReferenceRecord::getFileReferenceRecords(database).size());
    ASSERT_EQ(static_cast<unsigned>(1), AttributeRecord::getAttributeRecords(database).size());

    std::vector<FileReferenceRecord> fileReferenceRecords = FileReferenceRecord::getFileReferenceRecords(database);
    FileReferenceRecord model = fileReferenceRecords.at(0);
    EXPECT_EQ(static_cast<unsigned>(1), model.attributeRecords().size());
    AttributeRecord attributeRecord = model.attributeRecords()[0];

    EXPECT_EQ(id, attributeRecord.id());
    EXPECT_EQ(handle, attributeRecord.handle());
    EXPECT_EQ("name", attributeRecord.name());
    EXPECT_EQ("", attributeRecord.displayName());
    EXPECT_EQ("", attributeRecord.description());
    EXPECT_EQ(AttributeValueType::Double, attributeRecord.attributeValueType().value());
    EXPECT_EQ(1.0, attributeRecord.attributeValueAsDouble());
    ASSERT_TRUE(attributeRecord.attributeUnits());
    EXPECT_EQ("m", attributeRecord.attributeUnits().get());

    EXPECT_TRUE(database.includesRecord(attributeRecord));
    EXPECT_FALSE(database.isNewRecord(attributeRecord));
    EXPECT_FALSE(database.isDirtyRecord(attributeRecord));
    EXPECT_TRUE(database.isCleanRecord(attributeRecord));
    EXPECT_FALSE(database.isRemovedRecord(attributeRecord));
    EXPECT_EQ(static_cast<unsigned>(2), attributeRecord.useCount());

    // remove record but don't save
    EXPECT_TRUE(database.removeRecord(attributeRecord));

    EXPECT_EQ(static_cast<unsigned>(1), FileReferenceRecord::getFileReferenceRecords(database).size());
    EXPECT_EQ(static_cast<unsigned>(0), AttributeRecord::getAttributeRecords(database).size());

    EXPECT_TRUE(database.includesRecord(attributeRecord));
    EXPECT_FALSE(database.isNewRecord(attributeRecord));
    EXPECT_FALSE(database.isDirtyRecord(attributeRecord));
    EXPECT_FALSE(database.isCleanRecord(attributeRecord));
    EXPECT_TRUE(database.isRemovedRecord(attributeRecord));
    EXPECT_EQ(static_cast<unsigned>(2), attributeRecord.useCount());
  }

  {
    // open existing database
    ProjectDatabase database = getExistingDatabase("AttributeRecord_LoadSave");

    // saved objects were not removed
    ASSERT_EQ(static_cast<unsigned>(1), FileReferenceRecord::getFileReferenceRecords(database).size());
    ASSERT_EQ(static_cast<unsigned>(1), AttributeRecord::getAttributeRecords(database).size());

    std::vector<FileReferenceRecord> fileReferenceRecords = FileReferenceRecord::getFileReferenceRecords(database);
    FileReferenceRecord model = fileReferenceRecords.at(0);
    EXPECT_EQ(static_cast<unsigned>(1), model.attributeRecords().size());
    AttributeRecord attributeRecord = model.attributeRecords()[0];

    EXPECT_EQ(id, attributeRecord.id());
    EXPECT_EQ(handle, attributeRecord.handle());
    EXPECT_EQ("name", attributeRecord.name());
    EXPECT_EQ("", attributeRecord.displayName());
    EXPECT_EQ("", attributeRecord.description());
    EXPECT_EQ(AttributeValueType::Double, attributeRecord.attributeValueType().value());
    EXPECT_EQ(1.0, attributeRecord.attributeValueAsDouble());
    ASSERT_TRUE(attributeRecord.attributeUnits());
    EXPECT_EQ("m", attributeRecord.attributeUnits().get());

    EXPECT_TRUE(database.includesRecord(attributeRecord));
    EXPECT_FALSE(database.isNewRecord(attributeRecord));
    EXPECT_FALSE(database.isDirtyRecord(attributeRecord));
    EXPECT_TRUE(database.isCleanRecord(attributeRecord));
    EXPECT_FALSE(database.isRemovedRecord(attributeRecord));
    EXPECT_EQ(static_cast<unsigned>(2), attributeRecord.useCount());

    // remove record then save save
    EXPECT_TRUE(database.removeRecord(attributeRecord));

    EXPECT_EQ(static_cast<unsigned>(1), FileReferenceRecord::getFileReferenceRecords(database).size());
    EXPECT_EQ(static_cast<unsigned>(0), AttributeRecord::getAttributeRecords(database).size());

    EXPECT_TRUE(database.includesRecord(attributeRecord));
    EXPECT_FALSE(database.isNewRecord(attributeRecord));
    EXPECT_FALSE(database.isDirtyRecord(attributeRecord));
    EXPECT_FALSE(database.isCleanRecord(attributeRecord));
    EXPECT_TRUE(database.isRemovedRecord(attributeRecord));
    EXPECT_EQ(static_cast<unsigned>(2), attributeRecord.useCount());

    EXPECT_TRUE(database.save());

    EXPECT_EQ(static_cast<unsigned>(1), FileReferenceRecord::getFileReferenceRecords(database).size());
    EXPECT_EQ(static_cast<unsigned>(0), AttributeRecord::getAttributeRecords(database).size());

    EXPECT_FALSE(database.includesRecord(attributeRecord));
    EXPECT_FALSE(database.isNewRecord(attributeRecord));
    EXPECT_FALSE(database.isDirtyRecord(attributeRecord));
    EXPECT_FALSE(database.isCleanRecord(attributeRecord));
    EXPECT_FALSE(database.isRemovedRecord(attributeRecord));
    EXPECT_EQ(static_cast<unsigned>(1), attributeRecord.useCount());
  }

  {
    // open existing database
    ProjectDatabase database = getExistingDatabase("AttributeRecord_LoadSave");

    // objects were removed
    EXPECT_EQ(static_cast<unsigned>(1), FileReferenceRecord::getFileReferenceRecords(database).size());
    EXPECT_EQ(static_cast<unsigned>(0), AttributeRecord::getAttributeRecords(database).size());
  }

}

TEST_F(ProjectFixture, AttributeRecord_AttributeTypes)
{
  ProjectDatabase database = getCleanDatabase("AttributeRecord_AttributeTypes");

  FileReferenceRecord model(FileReference(toPath("./in.osm")),database);

  EXPECT_EQ(static_cast<unsigned>(0), AttributeRecord::getAttributeRecords(database).size());
  EXPECT_EQ(static_cast<unsigned>(0), model.attributeRecords().size());

  // make attributes
  Attribute boolAttribute("bool", true);
  Attribute unsignedAttribute("unsigned", 2u);
  Attribute intAttribute("int", -1);
  Attribute doubleAttribute("double", 1.23, std::string("m"));
  Attribute stringAttribute("string", "hello");
  // because of tracking by UUID, have to make separate child attributes
  AttributeVector childAttributes;
  childAttributes.push_back(Attribute("bool", true));
  childAttributes.push_back(Attribute("bool", true));
  Attribute vectorAttribute("vector", childAttributes);

  // create AttributeRecords
  AttributeRecord boolAttributeRecord(boolAttribute, model);
  AttributeRecord unsignedAttributeRecord(unsignedAttribute, model);
  AttributeRecord intAttributeRecord(intAttribute, model);
  AttributeRecord doubleAttributeRecord(doubleAttribute, model);
  AttributeRecord stringAttributeRecord(stringAttribute, model);
  AttributeRecord vectorAttributeRecord(vectorAttribute, model);

  ASSERT_TRUE(model.getAttributeRecord("bool"));
  EXPECT_TRUE(boolAttribute == model.getAttributeRecord("bool").get().attribute());
  ASSERT_TRUE(model.getAttributeRecord("unsigned"));
  EXPECT_TRUE(unsignedAttribute == model.getAttributeRecord("unsigned").get().attribute());
  ASSERT_TRUE(model.getAttributeRecord("int"));
  EXPECT_TRUE(intAttribute == model.getAttributeRecord("int").get().attribute());
  ASSERT_TRUE(model.getAttributeRecord("double"));
  EXPECT_TRUE(doubleAttribute == model.getAttributeRecord("double").get().attribute());
  ASSERT_TRUE(model.getAttributeRecord("string"));
  EXPECT_TRUE(stringAttribute == model.getAttributeRecord("string").get().attribute());
  ASSERT_TRUE(model.getAttributeRecord("vector"));
  EXPECT_TRUE(vectorAttribute == model.getAttributeRecord("vector").get().attribute());

  EXPECT_EQ(static_cast<unsigned>(6), AttributeRecord::getAttributeRecords(database).size());
  EXPECT_EQ(static_cast<unsigned>(6), model.attributeRecords().size());

  database.save();
}

TEST_F(ProjectFixture, AttributeRecord_AnnualEndUses)
{
  ProjectDatabase database = getCleanDatabase("AttributeRecord_AnnualEndUses");

  EndUses endUses;
  endUses.addEndUse(25.0, EndUseFuelType::Gas, EndUseCategoryType::Heating);
  endUses.addEndUse(2.0, EndUseFuelType::Electricity, EndUseCategoryType::Cooling);
  endUses.addEndUse(1.0, EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights);
  endUses.addEndUse(10.0, EndUseFuelType::Electricity, EndUseCategoryType::Fans, "Fan Energy");
  endUses.addEndUse(10.0, EndUseFuelType::Water, EndUseCategoryType::Humidifier);

  FileReferenceRecord model(FileReference(toPath("./in.osm")),database);

  AttributeRecord attributeRecord(endUses.attribute(), model);
  ASSERT_TRUE(model.getAttributeRecord(EndUses::attributeName()));
  EXPECT_TRUE(endUses.attribute() == model.getAttributeRecord(EndUses::attributeName()).get().attribute());

  database.save();
}

TEST_F(ProjectFixture, AttributeRecord_ManyAnnualEndUses)
{
  ProjectDatabase database = getCleanDatabase("AttributeRecord_ManyAnnualEndUses");

  for (unsigned i = 0; i < 10; ++i){

    EndUses endUses;
    endUses.addEndUse(25.0, EndUseFuelType::Gas, EndUseCategoryType::Heating);
    endUses.addEndUse(2.0, EndUseFuelType::Electricity, EndUseCategoryType::Cooling);
    endUses.addEndUse(1.0, EndUseFuelType::Electricity, EndUseCategoryType::InteriorLights);
    endUses.addEndUse(10.0, EndUseFuelType::Electricity, EndUseCategoryType::Fans, "Fan Energy");
    endUses.addEndUse(10.0, EndUseFuelType::Water, EndUseCategoryType::Humidifier);

    std::stringstream ss;
    ss << "model " << i;

    FileReferenceRecord model(FileReference(toPath("./in.osm")),database);

    AttributeRecord attributeRecord(endUses.attribute(), model);
    ASSERT_TRUE(model.getAttributeRecord(EndUses::attributeName()));
    EXPECT_TRUE(endUses.attribute() == model.getAttributeRecord(EndUses::attributeName()).get().attribute());
  }

  database.save();
}

TEST_F(ProjectFixture, AttributeRecord_FromScratch_Database)
{
  // create database and add records
  ProjectDatabase database = getCleanDatabase("AttributeRecord_FromScratch_Database");

  // create a zone component
  openstudio::model::Model model = openstudio::model::exampleModel();
  openstudio::model::ThermalZoneVector zones = model.getModelObjects<openstudio::model::ThermalZone>();
  ASSERT_FALSE(zones.empty());
  openstudio::model::Component zoneComponent = zones[0].createComponent();
  openstudio::path componentPath = toPath("./in.osc");
  zoneComponent.save(componentPath,true);
  FileReference componentReference(componentPath);

  FileReferenceRecord component(componentReference,database);

  EXPECT_EQ(static_cast<unsigned>(0), component.attributeRecords().size());

  Attribute attribute1("occupancy type","office");
  Attribute attribute2("energy use","54.23","kBtu/ft^2");
  AttributeRecord record1(attribute1, component);
  AttributeRecord record2(attribute2, component);

  /* UUID r1VUuid = */ record1.uuidLast();
  /* UUID r2VUuid = */ record2.uuidLast();
  EXPECT_TRUE(record1.attributeValueType() == AttributeValueType::String);
  EXPECT_TRUE(record2.attributeValueType() == AttributeValueType::String);
  EXPECT_NO_THROW(record1.cast<AttributeRecord>());
  EXPECT_NO_THROW(record2.cast<AttributeRecord>());
  EXPECT_TRUE(record1.description().empty());
  EXPECT_TRUE(record2.description().empty());
  EXPECT_EQ("",record1.displayName());
  EXPECT_EQ("",record2.displayName());
  EXPECT_NE(std::numeric_limits<int>::min(), record1.id());
  EXPECT_NE(std::numeric_limits<int>::min(), record2.id());
  EXPECT_EQ("occupancy type",record1.name());
  EXPECT_EQ("energy use",record2.name());
  EXPECT_FALSE(record1.attributeUnits());
  ASSERT_TRUE(record2.attributeUnits());
  EXPECT_EQ("kBtu/ft^2",record2.attributeUnits().get());
  EXPECT_EQ("office",record1.attributeValueAsString());
  EXPECT_EQ("54.23",record2.attributeValueAsString());
}

