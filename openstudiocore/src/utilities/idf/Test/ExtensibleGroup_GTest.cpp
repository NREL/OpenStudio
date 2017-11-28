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
#include "IdfFixture.hpp"
#include "../IdfObject.hpp"
#include "../WorkspaceObject.hpp"
#include "../WorkspaceObject_Impl.hpp"
#include "../WorkspaceExtensibleGroup.hpp"
#include "../Workspace.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/BuildingSurface_Detailed_FieldEnums.hxx>
#include <utilities/idd/Construction_FieldEnums.hxx>

#include <resources.hxx>

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(IdfFixture,ExtensibleGroup_ExtensibleObjects) {
  LOG(Info,"The following types of objects in IdfFixture::epIdfFile are extensible: ");
  IddObjectTypeVector iddTypes;
  for (const IdfObject& object : epIdfFile.objects()) {
    if (object.iddObject().properties().extensible) {
      if (std::find(iddTypes.begin(),iddTypes.end(),object.iddObject().type()) == iddTypes.end()) {
        iddTypes.push_back(object.iddObject().type());
        LOG(Info,"  " << iddTypes.back().valueDescription());
      }
    }
  }
}

TEST_F(IdfFixture,IdfExtensibleGroup_Getters) {
  LOG(Debug,"IdfFixture.ExtensibleGroup_Getters");

  Workspace ws(epIdfFile);

  LOG(Debug,"Getting all BuildingSurface:Detailed objects.");
  IddObjectType objType(IddObjectType::BuildingSurface_Detailed);
  WorkspaceObjectVector bldgSurfaces = ws.getObjectsByType(objType);
  ASSERT_TRUE(bldgSurfaces.size() > 0);

  LOG(Debug,"Looping through all BuildingSurface:Detailed objects.");
  for (const WorkspaceObject& surface : bldgSurfaces) {
    unsigned nVertices = 0;
    IdfExtensibleGroup vertex = surface.getExtensibleGroup(nVertices);
    while (!vertex.empty()) {
      ++nVertices;
      vertex = surface.getExtensibleGroup(nVertices);
    }
    OptionalString surfaceName = surface.name();
    ASSERT_TRUE(surfaceName);
    EXPECT_EQ(static_cast<unsigned>(4),nVertices);
  }
  LOG(Debug,"Testing contents of first surface, first vertex.");
  WorkspaceObject surface = bldgSurfaces[0];
  IdfExtensibleGroup firstVertex = surface.getExtensibleGroup(0);
  ASSERT_FALSE(firstVertex.empty());
  ASSERT_TRUE(firstVertex.isValid(0));
  ASSERT_TRUE(firstVertex.isValid(1));
  ASSERT_TRUE(firstVertex.isValid(2));
  OptionalString surfaceName = surface.name();
  std::stringstream msg;
  msg << "Surface " << *surfaceName << "'s first vertex is (";
  OptionalDouble c = firstVertex.getDouble(0);
  ASSERT_TRUE(c); msg << *c << ",";
  c = firstVertex.getDouble(1);
  ASSERT_TRUE(c); msg << *c << ",";
  c = firstVertex.getDouble(2);
  ASSERT_TRUE(c); msg << *c << ").";
  LOG(Debug,msg.str());

  LOG(Debug,"Test Complete.");
}

TEST_F(IdfFixture,IdfExtensibleGroup_Setters) {

  Workspace ws(epIdfFile);

  IddObjectType objType(IddObjectType::PlantEquipmentOperation_HeatingLoad);
  WorkspaceObjectVector objs = ws.getObjectsByType(objType);
  ASSERT_TRUE(objs.size() > 0);
  WorkspaceObject obj = objs[0];

  IdfExtensibleGroup eg = obj.getExtensibleGroup(0);
  ASSERT_FALSE(eg.empty());
  ASSERT_EQ(static_cast<unsigned>(3),eg.numFields());

  // setFieldComment
  OptionalString os = eg.fieldComment(0);
  if (os) { EXPECT_NE("my comment",*os); }
  EXPECT_TRUE(eg.setFieldComment(0,"my comment"));
  os = eg.fieldComment(0);
  ASSERT_TRUE(os);
  EXPECT_EQ("! my comment",*os);
  os = objs[0].getExtensibleGroup(0).fieldComment(0);
  if (os) { EXPECT_NE("my comment",*os); }

  // setString

  // setDouble
  OptionalDouble od = eg.getDouble(1,true);
  if (od) { EXPECT_NE(32.161,*od); }
  EXPECT_TRUE(eg.setDouble(1,32.161));
  od = eg.getDouble(1);
  ASSERT_TRUE(od);
  EXPECT_DOUBLE_EQ(32.161,*od);

  objType = IddObjectType(IddObjectType::ZoneHVAC_EquipmentList);
  objs = ws.getObjectsByType(objType);
  ASSERT_TRUE(objs.size() > 0);

  obj = objs[0];
  eg = obj.getExtensibleGroup(0);
  ASSERT_FALSE(eg.empty());
  ASSERT_EQ(static_cast<unsigned>(4),eg.numFields());

  // setUnsigned
  OptionalUnsigned ou = eg.getUnsigned(2,true); 
  ASSERT_TRUE(ou);
  EXPECT_EQ(static_cast<unsigned>(1),*ou);
  EXPECT_TRUE(eg.setUnsigned(2,5)); 
  ou = eg.getUnsigned(2);
  ASSERT_TRUE(ou);
  EXPECT_EQ(static_cast<unsigned>(5),*ou);

  // setInt  
  OptionalInt oi = eg.getInt(3,true);
  ASSERT_TRUE(oi); 
  EXPECT_EQ(1,*oi);
  EXPECT_TRUE(eg.setInt(3,4));
  oi = eg.getInt(3);
  EXPECT_EQ(4,*oi);

  // clone
  IdfExtensibleGroup egc = eg.pushClone();
  ASSERT_FALSE(egc.empty());
  EXPECT_TRUE(eg.groupIndex() < egc.groupIndex());
  StringVector egFields = eg.fields();
  StringVector egcFields = egc.fields();
  EXPECT_TRUE(egFields == egcFields);

}

TEST_F(IdfFixture,IdfExtensibleGroup_Queries) {
  
  Workspace ws(epIdfFile);

  IddObjectType objType(IddObjectType::AvailabilityManagerAssignmentList);
  WorkspaceObjectVector objs = ws.getObjectsByType(objType);
  ASSERT_TRUE(objs.size() > 0);
  
  // empty
  IdfExtensibleGroup eg = objs[0].getExtensibleGroup(1);
  EXPECT_TRUE(eg.empty());
  eg = objs[0].getExtensibleGroup(0);
  EXPECT_FALSE(eg.empty());

  // groupIndex
  unsigned i = eg.groupIndex();
  EXPECT_EQ(static_cast<unsigned>(0),i);
  eg = objs[0].getExtensibleGroup(1);
  EXPECT_THROW(eg.groupIndex(),std::exception);

  // numFields
  EXPECT_EQ(static_cast<unsigned>(0),eg.numFields());
  eg = objs[0].getExtensibleGroup(0);
  EXPECT_EQ(static_cast<unsigned>(2),eg.numFields());

  // isValid
  EXPECT_TRUE(eg.isValid(0));
  EXPECT_TRUE(eg.isValid(1));
  EXPECT_FALSE(eg.isValid(2));
  EXPECT_FALSE(eg.isValid(10));

  // objectListFields
  UnsignedVector olFields = eg.objectListFields();
  ASSERT_EQ(static_cast<unsigned>(1),olFields.size());
  EXPECT_EQ(static_cast<unsigned>(1),olFields[0]);

  // requiredFields
  UnsignedVector reqFields = eg.requiredFields();
  ASSERT_EQ(static_cast<unsigned>(2),reqFields.size());
  EXPECT_EQ(static_cast<unsigned>(0),reqFields[0]);
  EXPECT_EQ(static_cast<unsigned>(1),reqFields[1]);

  // ==
  IdfExtensibleGroup copy = objs[0].getExtensibleGroup(0);
  EXPECT_TRUE(eg == copy);

  // !=
  copy = objs[0].getExtensibleGroup(1);
  EXPECT_TRUE(eg != copy);
}

TEST_F(IdfFixture,WorkspaceExtensibleGroup_Getters) {
  Workspace ws(epIdfFile);

  // extensible group with no targets
  IddObjectType objType(IddObjectType::BuildingSurface_Detailed);
  WorkspaceObjectVector bldgSurfaces = ws.getObjectsByType(objType);
  ASSERT_FALSE(bldgSurfaces.empty());
  IdfExtensibleGroup group = bldgSurfaces[0].getExtensibleGroup(0);
  ASSERT_FALSE(group.empty());
  WorkspaceExtensibleGroup vertex = group.cast<WorkspaceExtensibleGroup>();

  // extensible group with targets
  objType = IddObjectType(IddObjectType::Construction);
  WorkspaceObjectVector constructions = ws.getObjectsByType(objType);
  ASSERT_FALSE(constructions.empty());
  group = constructions[0].getExtensibleGroup(0);
  ASSERT_FALSE(group.empty());
  WorkspaceExtensibleGroup layer = group.cast<WorkspaceExtensibleGroup>();

  // getTarget
  OptionalWorkspaceObject oTarget;
  oTarget = vertex.getTarget(BuildingSurface_DetailedExtensibleFields::VertexXcoordinate);
  EXPECT_FALSE(oTarget);
  oTarget = layer.getTarget(ConstructionExtensibleFields::Layer);
  ASSERT_TRUE(oTarget);
  WorkspaceObject material = *oTarget;

  // getTargets
  WorkspaceObjectVector targets;
  targets = vertex.getTargets();
  EXPECT_TRUE(targets.empty());
  targets = layer.getTargets();
  ASSERT_EQ(static_cast<unsigned>(1),targets.size());
  EXPECT_TRUE(targets[0] == material);

  // getSourceFieldIndices
  UnsignedVector indices = vertex.getSourceFieldIndices(material.handle());
  EXPECT_TRUE(indices.empty());
  indices = layer.getSourceFieldIndices(material.handle());
  ASSERT_EQ(static_cast<unsigned>(1),indices.size());
  EXPECT_EQ(static_cast<unsigned>(ConstructionExtensibleFields::Layer),indices[0]);
}

TEST_F(IdfFixture,WorkspaceExtensibleGroup_Setters) {
  Workspace ws(epIdfFile);

  // extensible group with no targets
  IddObjectType objType(IddObjectType::BuildingSurface_Detailed);
  WorkspaceObjectVector bldgSurfaces = ws.getObjectsByType(objType);
  ASSERT_FALSE(bldgSurfaces.empty());
  IdfExtensibleGroup group = bldgSurfaces[0].getExtensibleGroup(0);
  ASSERT_FALSE(group.empty());
  WorkspaceExtensibleGroup vertex = group.cast<WorkspaceExtensibleGroup>();

  // extensible group with targets
  objType = IddObjectType(IddObjectType::Construction);
  WorkspaceObjectVector constructions = ws.getObjectsByType(objType);
  ASSERT_FALSE(constructions.empty());
  group = constructions[0].getExtensibleGroup(0);
  ASSERT_FALSE(group.empty());
  WorkspaceExtensibleGroup layer = group.cast<WorkspaceExtensibleGroup>();

  // setPointer
  bool ok;
  ok = vertex.setPointer(BuildingSurface_DetailedExtensibleFields::VertexYcoordinate,
                         constructions[0].handle());
  EXPECT_FALSE(ok);
  objType = IddObjectType(IddObjectType::Material);
  WorkspaceObjectVector materials = ws.getObjectsByType(objType);
  for (WorkspaceObject& material : materials) {
    ok = layer.setPointer(ConstructionExtensibleFields::Layer,material.handle());
    EXPECT_TRUE(ok);
    OptionalWorkspaceObject oRetrievedMaterial = layer.getTarget(ConstructionExtensibleFields::Layer);
    ASSERT_TRUE(oRetrievedMaterial);
    EXPECT_TRUE(*oRetrievedMaterial == material);
  }
}

TEST_F(IdfFixture,WorkspaceExtensibleGroup_Queries) {
  Workspace ws(epIdfFile);

  // extensible group with no targets
  IddObjectType objType(IddObjectType::BuildingSurface_Detailed);
  WorkspaceObjectVector bldgSurfaces = ws.getObjectsByType(objType);
  ASSERT_FALSE(bldgSurfaces.empty());
  IdfExtensibleGroup group = bldgSurfaces[0].getExtensibleGroup(0);
  ASSERT_FALSE(group.empty());
  WorkspaceExtensibleGroup vertex = group.cast<WorkspaceExtensibleGroup>();

  // extensible group with targets
  objType = IddObjectType(IddObjectType::Construction);
  WorkspaceObjectVector constructions = ws.getObjectsByType(objType);
  ASSERT_FALSE(constructions.empty());
  group = constructions[0].getExtensibleGroup(0);
  ASSERT_FALSE(group.empty());
  WorkspaceExtensibleGroup layer = group.cast<WorkspaceExtensibleGroup>();

  // isSource/canBeSource
  EXPECT_FALSE(vertex.isSource());
  EXPECT_FALSE(vertex.canBeSource());
  EXPECT_TRUE(layer.isSource());
  EXPECT_TRUE(layer.canBeSource());
  layer.setString(ConstructionExtensibleFields::Layer,"");
  EXPECT_FALSE(layer.isSource());
  EXPECT_TRUE(layer.canBeSource());
}
TEST_F(IdfFixture,ExtensibleGroup_Insert) {
  // IdfObject
  IdfObject idfConstruction(IddObjectType::Construction);

  IdfExtensibleGroup eg = idfConstruction.insertExtensibleGroup(1);
  EXPECT_TRUE(eg.empty());
  eg = idfConstruction.insertExtensibleGroup(3);
  EXPECT_TRUE(eg.empty());
  StringVector values(1,"Insulation");
  eg = idfConstruction.insertExtensibleGroup(0,values);
  ASSERT_FALSE(eg.empty());
  values[0] = "Gypsum";
  eg = idfConstruction.insertExtensibleGroup(1,values);
  ASSERT_FALSE(eg.empty());
  ASSERT_EQ(static_cast<unsigned>(3),idfConstruction.numFields());
  EXPECT_EQ("Insulation",idfConstruction.getString(1).get());
  EXPECT_EQ("Gypsum",idfConstruction.getString(2).get());
  values[0] = "Brick";
  eg = idfConstruction.insertExtensibleGroup(0,values);
  ASSERT_FALSE(eg.empty());
  EXPECT_EQ("Brick",eg.getString(0).get());
  ASSERT_EQ(static_cast<unsigned>(4),idfConstruction.numFields());
  EXPECT_EQ("Brick",idfConstruction.getString(1).get());
  EXPECT_EQ("Insulation",idfConstruction.getString(2).get());
  EXPECT_EQ("Gypsum",idfConstruction.getString(3).get());
  values[0] = "More Insulation";
  eg = idfConstruction.insertExtensibleGroup(2,values);
  ASSERT_FALSE(eg.empty());
  ASSERT_EQ(static_cast<unsigned>(5),idfConstruction.numFields());
  EXPECT_EQ("Insulation",idfConstruction.getString(2).get());
  EXPECT_EQ("More Insulation",idfConstruction.getString(3).get());
  EXPECT_EQ("Gypsum",idfConstruction.getString(4).get());

  // WorkspaceObject
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus); // test that bad data inserts fail, nulls ok

  // add some objects
  IdfObjectVector objects;
  objects.push_back(IdfObject(IddObjectType::Construction));
  objects.push_back(IdfObject(IddObjectType::Material));
  objects[1].setName("Brick");
  objects.push_back(IdfObject(IddObjectType::Material));
  objects[2].setName("Insulation");
  objects.push_back(IdfObject(IddObjectType::Material));
  objects[3].setName("Gypsum");
  WorkspaceObjectVector temp = ws.addObjects(objects);
  ASSERT_EQ(static_cast<unsigned>(4),temp.size());
  ASSERT_TRUE(temp[0].iddObject().type() == IddObjectType::Construction);
  ASSERT_TRUE(temp[1].name().get() == "Brick");
  ASSERT_TRUE(temp[2].name().get() == "Insulation");
  ASSERT_TRUE(temp[3].name().get() == "Gypsum");
  WorkspaceObject wsConstruction = temp[0];

  // wsConstruction has no materials
  values[0] = "Brick";
  eg = wsConstruction.insertExtensibleGroup(0,values);
  ASSERT_FALSE(eg.empty()); // Brick
  values[0] = "More Insulation"; // not in workspace
  eg = wsConstruction.insertExtensibleGroup(1,values);
  ASSERT_TRUE(eg.empty()); // Brick
  values[0] = "Gypsum";
  eg = wsConstruction.insertExtensibleGroup(1,values);
  ASSERT_FALSE(eg.empty()); // Brick, Gypsum
  values[0] = "Insulation";
  eg = wsConstruction.insertExtensibleGroup(1,values);
  ASSERT_FALSE(eg.empty()); // Brick, Insulation, Gypsum
  ASSERT_EQ(static_cast<unsigned>(4),wsConstruction.numFields());
  EXPECT_EQ("Brick",wsConstruction.getString(1).get());
  EXPECT_EQ("Insulation",wsConstruction.getString(2).get());
  EXPECT_EQ("Gypsum",wsConstruction.getString(3).get());
  values[0] = "More Insulation"; // not in workspace
  eg = wsConstruction.insertExtensibleGroup(1,values);
  ASSERT_TRUE(eg.empty()); // Brick, Insulation, Gypsum
  ASSERT_EQ(static_cast<unsigned>(4),wsConstruction.numFields());
  EXPECT_EQ("Brick",wsConstruction.getString(1).get());
  EXPECT_EQ("Insulation",wsConstruction.getString(2).get());
  EXPECT_EQ("Gypsum",wsConstruction.getString(3).get());
  OptionalWorkspaceObject oTarget = wsConstruction.getTarget(1);
  ASSERT_TRUE(oTarget); EXPECT_TRUE(*oTarget == temp[1]); // Brick
  oTarget = wsConstruction.getTarget(2);
  ASSERT_TRUE(oTarget); EXPECT_TRUE(*oTarget == temp[2]); // Insulation
  oTarget = wsConstruction.getTarget(3);
  ASSERT_TRUE(oTarget); EXPECT_TRUE(*oTarget == temp[3]); // Gypsum
  EXPECT_EQ(static_cast<unsigned>(1),temp[1].numSources()); // wsConstruction points to Brick
  EXPECT_EQ(static_cast<unsigned>(1),temp[2].numSources()); // ... Insulation
  EXPECT_EQ(static_cast<unsigned>(1),temp[3].numSources()); // ... Gypsum

  eg = wsConstruction.insertExtensibleGroup(1);
  ASSERT_FALSE(eg.empty()); // Brick, "", Insulation, Gypsum
  ASSERT_EQ(static_cast<unsigned>(5),wsConstruction.numFields());
  EXPECT_EQ("Brick",wsConstruction.getString(1).get());
  EXPECT_EQ("",wsConstruction.getString(2).get());
  EXPECT_EQ("Insulation",wsConstruction.getString(3).get());
  EXPECT_EQ(static_cast<unsigned>(1),temp[1].numSources());
  EXPECT_EQ(static_cast<unsigned>(1),temp[2].numSources());
  EXPECT_EQ(static_cast<unsigned>(1),temp[3].numSources());
}

TEST_F(IdfFixture,ExtensibleGroup_Erase) {
  // IdfObject
  IdfObject idfConstruction(IddObjectType::Construction);  
  StringVector values(1,"Brick");
  IdfExtensibleGroup eg = idfConstruction.pushExtensibleGroup(values);
  ASSERT_FALSE(eg.empty());
  values[0] = "Insulation";
  eg = idfConstruction.pushExtensibleGroup(values);
  ASSERT_FALSE(eg.empty());
  values[0] = "More Insulation";
  eg = idfConstruction.pushExtensibleGroup(values);
  ASSERT_FALSE(eg.empty());
  values[0] = "Gypsum";
  eg = idfConstruction.pushExtensibleGroup(values);
  ASSERT_FALSE(eg.empty());

  values = idfConstruction.eraseExtensibleGroup(idfConstruction.numExtensibleGroups());
  ASSERT_TRUE(values.empty());
  values = idfConstruction.eraseExtensibleGroup(1);
  ASSERT_EQ(static_cast<unsigned>(1),values.size());
  EXPECT_EQ("Insulation",values[0]);
  ASSERT_EQ(static_cast<unsigned>(4),idfConstruction.numFields());
  EXPECT_EQ("Brick",idfConstruction.getString(1).get());
  EXPECT_EQ("More Insulation",idfConstruction.getString(2).get());
  EXPECT_EQ("Gypsum",idfConstruction.getString(3).get());

  values = idfConstruction.eraseExtensibleGroup(0);
  ASSERT_EQ(static_cast<unsigned>(1),values.size());
  EXPECT_EQ("Brick",values[0]);
  ASSERT_EQ(static_cast<unsigned>(3),idfConstruction.numFields());
  EXPECT_EQ("More Insulation",idfConstruction.getString(1).get());
  EXPECT_EQ("Gypsum",idfConstruction.getString(2).get());

  values = idfConstruction.eraseExtensibleGroup(1);
  ASSERT_EQ(static_cast<unsigned>(1),values.size());
  EXPECT_EQ("Gypsum",values[0]);
  ASSERT_EQ(static_cast<unsigned>(2),idfConstruction.numFields());
  EXPECT_EQ("More Insulation",idfConstruction.getString(1).get());

  values = idfConstruction.eraseExtensibleGroup(0);
  ASSERT_EQ(static_cast<unsigned>(1),values.size());
  EXPECT_EQ("More Insulation",values[0]);
  ASSERT_EQ(static_cast<unsigned>(1),idfConstruction.numFields());

  // WorkspaceObject
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus); // test that bad data inserts fail, nulls ok
  
  // add some objects
  IdfObjectVector objects;
  objects.push_back(IdfObject(IddObjectType::Construction));
  objects.push_back(IdfObject(IddObjectType::Material));
  objects[1].setName("Brick");
  objects.push_back(IdfObject(IddObjectType::Material));
  objects[2].setName("Insulation");
  objects.push_back(IdfObject(IddObjectType::Material));
  objects[3].setName("Gypsum");
  WorkspaceObjectVector temp = ws.addObjects(objects);
  ASSERT_EQ(static_cast<unsigned>(4),temp.size());
  ASSERT_TRUE(temp[0].iddObject().type() == IddObjectType::Construction);
  ASSERT_TRUE(temp[1].name().get() == "Brick");
  ASSERT_TRUE(temp[2].name().get() == "Insulation");
  ASSERT_TRUE(temp[3].name().get() == "Gypsum");
  WorkspaceObject wsConstruction = temp[0];

  values[0] = "Brick";
  eg = wsConstruction.pushExtensibleGroup(values);
  ASSERT_FALSE(eg.empty()); // Brick
  values[0] = "Insulation";
  eg = wsConstruction.pushExtensibleGroup(values);
  ASSERT_FALSE(eg.empty()); // Brick, Insulation
  eg = wsConstruction.pushExtensibleGroup(values);
  ASSERT_FALSE(eg.empty()); // Brick, Insulation, Insulation
  values[0] = "Gypsum";
  eg = wsConstruction.pushExtensibleGroup(values);
  ASSERT_FALSE(eg.empty()); // Brick, Insulation, Insulation, Gypsum

  // erase and check pointers
  EXPECT_EQ(static_cast<unsigned>(2),temp[2].numSources());
  values = wsConstruction.eraseExtensibleGroup(1);
  ASSERT_EQ(static_cast<unsigned>(1),values.size());
  EXPECT_EQ("Insulation",values[0]); // Brick, Insulation, Gypsum
  EXPECT_EQ(static_cast<unsigned>(1),temp[2].numSources());
  values = wsConstruction.eraseExtensibleGroup(2);
  ASSERT_EQ(static_cast<unsigned>(1),values.size());
  EXPECT_EQ("Gypsum",values[0]); // Brick, Insulation
  EXPECT_EQ(static_cast<unsigned>(1),temp[1].numSources());
  EXPECT_EQ(static_cast<unsigned>(1),temp[2].numSources());
  EXPECT_EQ(static_cast<unsigned>(0),temp[3].numSources());
  ASSERT_EQ(static_cast<unsigned>(3),wsConstruction.numFields());
  EXPECT_EQ("Brick",wsConstruction.getString(1).get());
  EXPECT_EQ("Insulation",wsConstruction.getString(2).get());
}

TEST_F(IdfFixture,ExtensibleGroup_Clone) {
  // IdfObject
  IdfObject surfaceColorScheme(IddObjectType::OutputControl_SurfaceColorScheme);
  StringVector values;
  values.push_back("Windows");
  values.push_back("126");
  IdfExtensibleGroup drawingElement1 = surfaceColorScheme.pushExtensibleGroup(values);
  ASSERT_FALSE(drawingElement1.empty());
  IdfExtensibleGroup drawingElement2 = drawingElement1.pushClone();
  ASSERT_FALSE(drawingElement2.empty());
  EXPECT_EQ(static_cast<unsigned>(2),surfaceColorScheme.numExtensibleGroups());
  EXPECT_EQ(static_cast<unsigned>(0),drawingElement1.groupIndex());
  EXPECT_EQ(static_cast<unsigned>(1),drawingElement2.groupIndex());
  for (unsigned fieldIndex = 0, n = drawingElement1.numFields(); fieldIndex < n; ++fieldIndex) {
    ASSERT_TRUE(drawingElement1.getString(fieldIndex));
    ASSERT_TRUE(drawingElement2.getString(fieldIndex));
    ASSERT_EQ(drawingElement1.getString(fieldIndex).get(),drawingElement2.getString(fieldIndex).get());
  }
  ASSERT_TRUE(drawingElement2.setString(0,"Floors"));
  ASSERT_TRUE(drawingElement2.setUnsigned(1,255));
  drawingElement1 = drawingElement2.insertClone(1);
  ASSERT_FALSE(drawingElement1.empty());
  // known issue: drawingElement2 is now pointing to the same group as drawingElement1!
  EXPECT_EQ(static_cast<unsigned>(1),drawingElement1.groupIndex());
  EXPECT_EQ(static_cast<unsigned>(1),drawingElement2.groupIndex());
  EXPECT_EQ(static_cast<unsigned>(3),surfaceColorScheme.numExtensibleGroups());
  drawingElement2 = surfaceColorScheme.getExtensibleGroup(2);
  ASSERT_FALSE(drawingElement2.empty());
  for (unsigned fieldIndex = 0, n = drawingElement1.numFields(); fieldIndex < n; ++fieldIndex) {
    ASSERT_TRUE(drawingElement1.getString(fieldIndex));
    ASSERT_TRUE(drawingElement2.getString(fieldIndex));
    ASSERT_EQ(drawingElement1.getString(fieldIndex).get(),drawingElement2.getString(fieldIndex).get());
  }

  // WorkspaceObject
  Workspace ws(StrictnessLevel::Draft, IddFileType::EnergyPlus);
  
  // add some objects
  IdfObjectVector objects;
  objects.push_back(IdfObject(IddObjectType::Construction));
  objects[0].setName("Mega-Brick Wall");
  objects.push_back(IdfObject(IddObjectType::Material));
  objects[1].setName("Brick");
  WorkspaceObjectVector wsObjects = ws.addObjects(objects);
  ASSERT_EQ(static_cast<unsigned>(2),wsObjects.size());
  EXPECT_EQ("Mega-Brick Wall",wsObjects[0].name().get());
  EXPECT_EQ("Brick",wsObjects[1].name().get());

  // do clones, test pointer following
  values.clear();
  values.push_back("Brick");
  IdfExtensibleGroup layer0 = wsObjects[0].pushExtensibleGroup(values);
  ASSERT_FALSE(layer0.empty());
  WorkspaceExtensibleGroup wsLayer0 = layer0.cast<WorkspaceExtensibleGroup>();
  OptionalWorkspaceObject oTarget = wsLayer0.getTarget(0);
  ASSERT_TRUE(oTarget);
  EXPECT_TRUE(*oTarget == wsObjects[1]);
  
  IdfExtensibleGroup newLayer = layer0.pushClone();
  ASSERT_FALSE(newLayer.empty());
  WorkspaceExtensibleGroup wsNewLayer = newLayer.cast<WorkspaceExtensibleGroup>();
  oTarget = wsNewLayer.getTarget(0);
  ASSERT_TRUE(oTarget);
  EXPECT_TRUE(*oTarget == wsObjects[1]);
  
  newLayer = layer0.insertClone(1);
  ASSERT_FALSE(newLayer.empty());
  wsNewLayer = newLayer.cast<WorkspaceExtensibleGroup>();
  oTarget = wsNewLayer.getTarget(0);
  ASSERT_TRUE(oTarget);
  EXPECT_TRUE(*oTarget == wsObjects[1]);
  
  newLayer = layer0.insertClone(1);
  ASSERT_FALSE(newLayer.empty());
  wsNewLayer = newLayer.cast<WorkspaceExtensibleGroup>();
  oTarget = wsNewLayer.getTarget(0);
  ASSERT_TRUE(oTarget);
  EXPECT_TRUE(*oTarget == wsObjects[1]);

  newLayer = layer0.insertClone(1);
  ASSERT_FALSE(newLayer.empty());
  wsNewLayer = newLayer.cast<WorkspaceExtensibleGroup>();
  oTarget = wsNewLayer.getTarget(0);
  ASSERT_TRUE(oTarget);
  EXPECT_TRUE(*oTarget == wsObjects[1]);

  newLayer = layer0.insertClone(2);
  ASSERT_FALSE(newLayer.empty());
  wsNewLayer = newLayer.cast<WorkspaceExtensibleGroup>();
  oTarget = wsNewLayer.getTarget(0);
  ASSERT_TRUE(oTarget);
  EXPECT_TRUE(*oTarget == wsObjects[1]);

  newLayer = layer0.insertClone(0);
  ASSERT_FALSE(newLayer.empty());
  wsNewLayer = newLayer.cast<WorkspaceExtensibleGroup>();
  oTarget = wsNewLayer.getTarget(0);
  ASSERT_TRUE(oTarget);
  EXPECT_TRUE(*oTarget == wsObjects[1]);
}

TEST_F(IdfFixture, ExtensibleGroup_ErrorHandling) {
  // IdfObject
  IdfObject zone(IddObjectType::Zone);

  StringVector values;
  IdfExtensibleGroup group = zone.pushExtensibleGroup(values);
  EXPECT_TRUE(group.empty());

  values.clear();
  values.push_back("Cool Stuff");
  group = zone.pushExtensibleGroup(values);
  EXPECT_TRUE(group.empty());
}