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
#include <standardsinterface/test/StandardsInterfaceFixture.hpp>

#include <standardsinterface/OpenStudioStandardsInterface.hpp>

#include <model/Model_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/Site.hpp>
#include <model/Site_Impl.hpp>
#include <model/ClimateZones.hpp>

#include <utilities/core/Containers.hpp>

#include <boost/foreach.hpp>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::standardsinterface;

TEST_F(StandardsInterfaceFixture,OpenStudioStandardsInterface_GetObjects) {
  OpenStudioStandardsInterface workingModel(exampleModel(false).clone().cast<Model>());

  // get building objects
  ModelObjectVectorResult objectsResult = workingModel.getObjects("building");
  ASSERT_TRUE(objectsResult);
  ModelObjectVector buildings = *objectsResult;
  ASSERT_EQ(static_cast<unsigned>(1),buildings.size());

  // get zone objects
  objectsResult = workingModel.getObjects("zone");
  ASSERT_TRUE(objectsResult);
  ModelObjectVector zones = *objectsResult;
  ASSERT_TRUE(zones.size() > 0);

  // get zones through building
  objectsResult = workingModel.getObjects("building",buildings[0],"ZoNe");
  ASSERT_TRUE(objectsResult);
  ModelObjectVector temp = *objectsResult;
  EXPECT_EQ(zones.size(),temp.size());

  // get roofs and ceilings
  BOOST_FOREACH(const ModelObject& zone,zones) {
    ModelObjectVectorResult roofResult = workingModel.getObjects("zone",zone,"roof");
    ModelObjectVectorResult ceilingResult = workingModel.getObjects("zone",zone,"ceiling");
    ASSERT_TRUE(roofResult && ceilingResult);
    ModelObjectVector roofSurfaces = *roofResult;
    ModelObjectVector ceilingSurfaces = *ceilingResult;
    bool hasRoof = (!roofSurfaces.empty());
    bool hasCeiling = (!ceilingSurfaces.empty());
    EXPECT_TRUE(hasRoof || hasCeiling);
  }

  // get wall insulation
  objectsResult = workingModel.getObjects("wall");
  ASSERT_TRUE(objectsResult);
  ModelObjectVector walls = *objectsResult;
  bool foundInsulation(false);
  BOOST_FOREACH(const ModelObject& wall, walls) {
    objectsResult = workingModel.getObjects("Wall",wall,"ConstructionAssembly");
    if (objectsResult) {
      temp = *objectsResult;
      ASSERT_EQ(1u,temp.size());
      ModelObject construction = temp[0];
      objectsResult = workingModel.getObjects("ConstructionAssembly",construction,"Insulation");
      if (objectsResult) {
        temp = *objectsResult;
        if (!temp.empty()) {
          foundInsulation = true;
          break;
        }
      }
    }
  }
  EXPECT_TRUE(foundInsulation);
}

TEST_F(StandardsInterfaceFixture,OpenStudioStandardsInterface_SupportedObjectTypes) {
  OpenStudioStandardsInterface workingModel(exampleModel(false).clone().cast<Model>());

  ObjectTypeVector supportedObjectTypes = OpenStudioStandardsInterface::supportedObjectTypes();
  BOOST_FOREACH(const ObjectType& objectType,supportedObjectTypes) {
    LOG(Debug,"Looking for objects of type '" << objectType.valueName() << "'.");
    ModelObjectVectorResult objectsResult = workingModel.getObjects(objectType.valueName());
    if (objectType != ObjectType::Insulation) {
      ASSERT_TRUE(objectsResult);
      ModelObjectVector objects = *objectsResult;
      if (objectType == ObjectType::Building) {
        EXPECT_EQ(1u,objects.size());
      }
      else if (objectType != ObjectType::Ceiling) {
        EXPECT_FALSE(objects.empty());
      }
    }
  }
}

TEST_F(StandardsInterfaceFixture,OpenStudioStandardsInterface_SupportedBooleanAttributes) {
  OpenStudioStandardsInterface workingModel(exampleModel(false).clone().cast<Model>());

  ObjectTypeVector supportedObjectTypes = OpenStudioStandardsInterface::supportedObjectTypes();
  BOOST_FOREACH(const ObjectType& objectType,supportedObjectTypes) {
    BooleanAttributeVector supportedAttributes =
        OpenStudioStandardsInterface::supportedBooleanAttributes(objectType);
    EXPECT_TRUE(supportedAttributes.empty());
  }
}

TEST_F(StandardsInterfaceFixture,OpenStudioStandardsInterface_SupportedDoubleAttributes) {
  OpenStudioStandardsInterface workingModel(exampleModel(true).clone().cast<Model>());
  EXPECT_TRUE(workingModel.model().sqlFile()); // cloning now pulls SqlFile along.

  ObjectTypeVector supportedObjectTypes = OpenStudioStandardsInterface::supportedObjectTypes();
  BOOST_FOREACH(const ObjectType& objectType,supportedObjectTypes) {
    // get objects
    ModelObjectVector objects;
    ModelObjectVectorResult objectsResult = workingModel.getObjects(objectType.valueName());
    // exceptional case
    if (objectType == ObjectType::Insulation) {
      objectsResult = workingModel.getObjects(ObjectType(ObjectType::ConstructionAssembly).valueName());
      ASSERT_TRUE(objectsResult);
      ModelObjectVector temp = *objectsResult;
      BOOST_FOREACH(const ModelObject& construction,temp) {
        objectsResult = workingModel.getObjects(ObjectType(ObjectType::ConstructionAssembly).valueName(),
                                                construction,
                                                objectType.valueName());
        if (objectsResult) {
          ModelObjectVector insulations = *objectsResult;
          objects.insert(objects.end(),insulations.begin(),insulations.end());
        }
      }
      EXPECT_FALSE(objects.empty());
    }
    else {
      ASSERT_TRUE(objectsResult);
      objects = *objectsResult;
    }
    // get attributes
    DoubleAttributeVector supportedAttributes =
        OpenStudioStandardsInterface::supportedDoubleAttributes(objectType);
    BOOST_FOREACH(const DoubleAttribute& attribute,supportedAttributes) {
      LOG(Debug,"Investigating double attribute '" << attribute.valueName() << "' for object type '"
          << objectType.valueName() << ".");
      BOOST_FOREACH(const ModelObject& object,objects) {
        // just check getter for now
        DoubleResult dr = workingModel.doubleAttribute(objectType.valueName(),object,attribute.valueName());
        // expect result unless non-envelope surface
        OptionalSurface oSurface = object.optionalCast<Surface>();
        if (!oSurface || oSurface->isPartOfEnvelope()) {
          EXPECT_TRUE(dr);
          if (!dr) {
            LOG(Debug,"Object of type '" << objectType.valueName() << "' was unable to return a "
                << "value for double attribute '" << attribute.valueName() << "'.");
          }
        }
        if (!dr) { break; }
        double value = *dr;
        LOG(Debug,"Object of type '" << objectType.valueName() << "' has double attribute '"
            << attribute.valueName() << "' with value " << value << ".");
      }
    }
  }
}

TEST_F(StandardsInterfaceFixture,OpenStudioStandardsInterface_SupportedEnumAttributes) {
  OpenStudioStandardsInterface workingModel(exampleModel(false).clone().cast<Model>());
  workingModel.model().getUniqueModelObject<Site>().setActiveClimateZoneInstitution(ClimateZones::cecInstitutionName());

  ObjectTypeVector supportedObjectTypes = OpenStudioStandardsInterface::supportedObjectTypes();
  BOOST_FOREACH(const ObjectType& objectType,supportedObjectTypes) {
    if (objectType == ObjectType::Insulation) {
      continue;
    }
    // get objects
    ModelObjectVectorResult objectsResult = workingModel.getObjects(objectType.valueName());
    ASSERT_TRUE(objectsResult);
    ModelObjectVector objects = *objectsResult;
    // get attributes
    EnumAttributeVector supportedAttributes =
        OpenStudioStandardsInterface::supportedEnumAttributes(objectType);
    BOOST_FOREACH(const EnumAttribute& attribute,supportedAttributes) {
      LOG(Debug,"Investigating enum attribute '" << attribute.valueName() << "' for object type '"
          << objectType.valueName() << ".");
      StringVector allowedValues = DataDictionary::enumValues(attribute.valueName());
      ASSERT_FALSE(allowedValues.empty());
      BOOST_FOREACH(ModelObject& object,objects) {
        if (attribute == EnumAttribute::PartitionThermalType) {
          // just get
          StringResult sr = workingModel.enumAttribute(objectType.valueName(),object,attribute.valueName());
          EXPECT_TRUE(sr);
          if (!sr) {
            LOG(Debug,"Object of type '" << objectType.valueName() << "' was unable to return a "
                << "value for enum attribute '" << attribute.valueName() << "'.");
            break;
          }
          std::string value = *sr;
          LOG(Debug,"Object of type '" << objectType.valueName() << "' has enum attribute '"
              << attribute.valueName() << "' with value " << value << ".");
        }
        else {
          // set, then get
          VoidResult ok = workingModel.setAttribute(objectType.valueName(),
                                                    object,
                                                    attribute.valueName(),
                                                    allowedValues[0]);
          EXPECT_TRUE(ok);
          if (!ok) {
            LOG(Debug,"Object of type '" << objectType.valueName() << "' was unable to set a "
                << "value for enum attribute '" << attribute.valueName() << "'.");
            break;
          }
          StringResult sr = workingModel.enumAttribute(objectType.valueName(),object,attribute.valueName());
          EXPECT_TRUE(sr);
          if (!sr) {
            LOG(Debug,"Object of type '" << objectType.valueName() << "' was unable to return a "
                << "value for enum attribute '" << attribute.valueName() << "'.");
            break;
          }
          std::string value = *sr;
          EXPECT_EQ(allowedValues[0],value);
          LOG(Debug,"Object of type '" << objectType.valueName() << "' has enum attribute '"
              << attribute.valueName() << "' with value " << value << ".");
        }
      }
    }
  }
}

TEST_F(StandardsInterfaceFixture,OpenStudioStandardsInterface_SupportedIntegerAttributes) {
  OpenStudioStandardsInterface workingModel(exampleModel(false).clone().cast<Model>());

  ObjectTypeVector supportedObjectTypes = OpenStudioStandardsInterface::supportedObjectTypes();
  BOOST_FOREACH(const ObjectType& objectType,supportedObjectTypes) {
    if (objectType == ObjectType::Insulation) {
      continue;
    }
    // get objects
    ModelObjectVectorResult objectsResult = workingModel.getObjects(objectType.valueName());
    ASSERT_TRUE(objectsResult);
    ModelObjectVector objects = *objectsResult;
    // get attributes
    IntegerAttributeVector supportedAttributes =
        OpenStudioStandardsInterface::supportedIntegerAttributes(objectType);
    BOOST_FOREACH(const IntegerAttribute& attribute,supportedAttributes) {
      LOG(Debug,"Investigating integer attribute '" << attribute.valueName() << "' for object type '"
          << objectType.valueName() << ".");
      BOOST_FOREACH(ModelObject& object,objects) {
        // set, then get
        VoidResult ok = workingModel.setAttribute(objectType.valueName(),
                                                  object,
                                                  attribute.valueName(),
                                                  1);
        ASSERT_TRUE(ok);
        IntegerResult ir = workingModel.integerAttribute(objectType.valueName(),object,attribute.valueName());
        EXPECT_TRUE(ir);
        if (!ir) { break; }
        int value = *ir;
        EXPECT_EQ(1,value);
        LOG(Debug,"Object of type '" << objectType.valueName() << "' has integer attribute '"
            << attribute.valueName() << "' with value " << value << ".");
      }
    }
  }
}
