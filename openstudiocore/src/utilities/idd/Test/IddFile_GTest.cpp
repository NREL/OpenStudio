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
#include <utilities/idd/Test/IddFixture.hpp>

#include <utilities/idd/IddFieldProperties.hpp>
#include <utilities/idd/IddKey.hpp>

#include <utilities/time/Time.hpp>

#include <utilities/core/String.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/Compare.hpp>

#include <resources.hxx>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/fstream.hpp>


#include <algorithm>

using namespace std;
using namespace boost;
using namespace openstudio;

// this will call all sub tests, probably should break out into their own file
void testIddFile(const IddFile& iddFile);

// test idd file
TEST_F(IddFixture, IddFile)
{
  // from factory
  SCOPED_TRACE("IddFile");
  testIddFile(epIddFile);

  // from file
  path iddPath = resourcesPath()/toPath("energyplus/ProposedEnergy+.idd");
  boost::filesystem::ifstream inFile(iddPath); ASSERT_TRUE(inFile);
  OptionalIddFile loadedIddFile = IddFile::load(inFile);
  ASSERT_TRUE(loadedIddFile); inFile.close();
  EXPECT_EQ("8.1.0.009",loadedIddFile->version());
  EXPECT_EQ(epIddFile.objects().size(),loadedIddFile->objects().size());
  if (epIddFile.objects().size() != loadedIddFile->objects().size()) {
    // get sets of IddObjectType
    IddObjectTypeSet epIddObjectTypes, loadedIddObjectTypes, diff;
    BOOST_FOREACH(const IddObject& iddObject,epIddFile.objects()) {
      EXPECT_TRUE(iddObject.type() != IddObjectType::UserCustom);
      epIddObjectTypes.insert(iddObject.type());
    }
    BOOST_FOREACH(const IddObject& iddObject,loadedIddFile->objects()) {
      if (iddObject.type() == IddObjectType::UserCustom) {
        try {
          IddObjectType iddObjectType(iddObject.name());
          loadedIddObjectTypes.insert(iddObjectType);
        }
        catch (...) {
          LOG(Debug,"Unable to convert IddObject name '" << iddObject.name() << "' "
              << "to IddObjectType.");
        }
      }
      else {
        loadedIddObjectTypes.insert(iddObject.type());
      }
    }
    std::set_difference(epIddObjectTypes.begin(),epIddObjectTypes.end(),
                        loadedIddObjectTypes.begin(),loadedIddObjectTypes.end(),
                        std::inserter(diff,diff.begin()));
    std::stringstream ss;
    BOOST_FOREACH(const IddObjectType& iddType,diff) {
      ss << "  " << iddType << std::endl;
    }
    diff.clear();
    LOG(Debug,"The following object types are in epIddFile, but are not in loadedIddFile: "
        << std::endl << ss.str());
    ss.str("");
    std::set_difference(loadedIddObjectTypes.begin(),loadedIddObjectTypes.end(),
                        epIddObjectTypes.begin(),epIddObjectTypes.end(),
                        std::inserter(diff,diff.begin()));
    BOOST_FOREACH(const IddObjectType& iddType,diff) {
      ss << "  " << iddType << std::endl;
    }
    LOG(Debug,"The following object types are in loadedIddFile, but are not in epIddFile: "
        << std::endl << ss.str());
  }

  testIddFile(*loadedIddFile);
}

// test single object, no fields
void IddFile_LeadInput(const IddFile& iddFile)
{
  string objectName("Lead Input");
  OptionalIddObject object = iddFile.getObject(objectName);
  ASSERT_TRUE(object);
  EXPECT_EQ(object->name(), objectName);
  EXPECT_TRUE(object->nonextensibleFields().empty() );
}

// test single object, one alpha field, no keys
void IddFile_Version(const IddFile& iddFile)
{
  string objectName("Version");
  OptionalIddObject object = iddFile.getObject(objectName);
  ASSERT_TRUE(object);
  EXPECT_EQ(object->name(), objectName);
  EXPECT_EQ(static_cast<unsigned int>(1),object->nonextensibleFields().size());

  string fieldName("Version Identifier");
  OptionalIddField field = object->getField(fieldName);
  ASSERT_TRUE(field);
  EXPECT_EQ(field->name(), fieldName);
  EXPECT_TRUE(field->keys().empty());
}

// test single object, 7 fields, 5 keys on field 3
void IddFile_Building(const IddFile& iddFile)
{
  string objectName("Building");
  OptionalIddObject object = iddFile.getObject(objectName);
  ASSERT_TRUE(object);
  EXPECT_EQ(object->name(), objectName);
  EXPECT_EQ(static_cast<unsigned int>(8),object->nonextensibleFields().size());
  EXPECT_TRUE(object->properties().unique);
  EXPECT_TRUE(object->properties().required);

  string fieldName("Terrain");
  OptionalIddField field = object->getField(fieldName);
  ASSERT_TRUE(field);
  EXPECT_EQ(field->name(), fieldName);
  EXPECT_EQ(static_cast<unsigned int>(5),field->keys().size());
  ASSERT_TRUE(field->properties().stringDefault);
  EXPECT_EQ(*(field->properties().stringDefault), "Suburbs");
  EXPECT_FALSE(field->properties().numericDefault);

  string keyName("Suburbs");
  OptionalIddKey key = field->getKey(keyName);
  EXPECT_TRUE(key);
  EXPECT_EQ(key->name(), keyName);
}

// case insensitive test
void IddFile_bUiLdInG(const IddFile& iddFile)
{
  string objectName("bUiLdInG");
  OptionalIddObject object = iddFile.getObject(objectName);
  ASSERT_TRUE(object);
  EXPECT_TRUE(iequals(object->name(), objectName));
  EXPECT_EQ(static_cast<unsigned int>(8),object->nonextensibleFields().size());
  EXPECT_TRUE(object->properties().unique);
  EXPECT_TRUE(object->properties().required);

  string fieldName("TeRrAiN");
  OptionalIddField field = object->getField(fieldName);
  ASSERT_TRUE(field);
  EXPECT_TRUE(iequals(field->name(), fieldName));
  EXPECT_EQ(field->keys().size(), static_cast<unsigned int>(5) );
  EXPECT_TRUE(field->properties().stringDefault);
  EXPECT_EQ(*(field->properties().stringDefault), "Suburbs");
  EXPECT_FALSE(field->properties().numericDefault);

  string keyName("SuBuRbS");
  OptionalIddKey key = field->getKey(keyName);
  ASSERT_TRUE(key);
  EXPECT_FALSE(equals(key->name(), keyName));
  EXPECT_TRUE(iequals(key->name(), keyName));
}

// extensible object test, BuildingSurface:Detailed
void IddFile_BuildingSurfaceDetailed(const IddFile& iddFile)
{
  string objectName("BuildingSurface:Detailed");
  OptionalIddObject object = iddFile.getObject(objectName);
  ASSERT_TRUE(object);
  EXPECT_TRUE(iequals(object->name(), objectName));
  ASSERT_TRUE(object->nonextensibleFields().size() > 0);
  EXPECT_EQ(object->nonextensibleFields().size(), static_cast<unsigned int>(10));
  EXPECT_FALSE(object->properties().format.empty());
  EXPECT_TRUE(iequals("vertices", object->properties().format));

  EXPECT_TRUE(object->properties().extensible);
  EXPECT_TRUE(object->properties().numExtensible);
  ASSERT_EQ(static_cast<unsigned int>(3), object->properties().numExtensible);

  EXPECT_FALSE(object->getField("Vertex 1 X-coordinate"));
  EXPECT_FALSE(object->getField("Vertex 1 X-coordinate"));

  ASSERT_TRUE(object->extensibleGroup().size() > 0);
  string fieldName("Vertex X-coordinate");
  EXPECT_TRUE(iequals(fieldName, object->extensibleGroup().front().name() ));
  OptionalIddField field = object->getField(fieldName);
  ASSERT_TRUE(field);
  EXPECT_TRUE(iequals(fieldName, field->name()));
  EXPECT_TRUE(field->properties().beginExtensible);

  fieldName = "Vertex Z-coordinate";
  EXPECT_TRUE(iequals(fieldName, object->extensibleGroup().back().name() ));
  field = object->getField(fieldName);
  ASSERT_TRUE(field);
  EXPECT_TRUE(iequals(fieldName, field->name()));
  EXPECT_FALSE(field->properties().beginExtensible);
}

// call all test functions, keep as last function
void testIddFile(const IddFile& iddFile)
{
  IddFile_LeadInput(iddFile);
  IddFile_Version(iddFile);
  IddFile_Building(iddFile);
  IddFile_bUiLdInG(iddFile);
  IddFile_BuildingSurfaceDetailed(iddFile);
}

TEST_F(IddFixture, IddFile_EpAllReferencesHaveNames)
{
  BOOST_FOREACH(const IddObject& object, epIddFile.objects()){
    if (!object.references().empty()){
      if (object.nonextensibleFields().size() == 0) {
        LOG(Debug,"IddObject " << object.name() << " has references, but no fields.");
      }
      if (object.nonextensibleFields().size() > 0) {
        // ETH@20100319 Would be nice if \references conformed to this convention, but not getting
        // fixed in EnergyPlus 5.0. Removing failed tests, but retaining log messages.
        // EXPECT_EQ("Name", object.fields().front().name());
        if (object.nonextensibleFields().front().name() != "Name") {
          LOG(Debug,"IddObject " << object.name() << " has a reference field, but its first field is "
              << object.nonextensibleFields().front().name() << ".");
        }
      }
    }
  }
}

TEST_F(IddFixture, IddFile_EpMinFields) {
  BOOST_FOREACH(const IddObject& object, epIddFile.objects()) {
    if (object.properties().minFields == 0) {
      IddFieldVector fields = object.nonextensibleFields();
      unsigned setTo = 0;
      for (unsigned i = 0, n = fields.size(); i < n; ++i) {
        if (fields[i].properties().required) { setTo = i; }
      }
      if (setTo > 0) {
          LOG(Debug,"IddObject " << object.name() << " has no min-fields property, but its last "
              << "required field is " << fields[setTo].name() << ".");
      }
      fields = object.extensibleGroup();
      BOOST_FOREACH(const IddField& field, fields) {
        if (field.properties().required) {
          LOG(Debug,"IddObject " << object.name() << " has a required extensible field.");
        }
        break;
      }
    }
  }
}

TEST_F(IddFixture, IddFile_EpGroups) {
  StringVector groups = epIddFile.groups();
  EXPECT_TRUE(groups.size() > 0);
  EXPECT_EQ("",groups[0]);
  std::stringstream ss;
  // uniqueness
  for (StringVector::const_iterator it = groups.begin(), itEnd = groups.end(); it != itEnd; ++it) {
    StringVector::const_iterator loc = std::find_if(it+1,itEnd,boost::bind(istringEqual,*it,_1));
    EXPECT_TRUE(loc == itEnd);
    if (loc != itEnd) {
      LOG(Debug,"The group name '" << *it << "' is repeated in epIddFile.");
    }
    ss << "  " << *it << std::endl;
  }
  // log groups
  LOG(Info,"The current EnergyPlus IddFile contains the following " << groups.size()
      << " object groups, including the first, unnamed group: " << std::endl << ss.str());
}

