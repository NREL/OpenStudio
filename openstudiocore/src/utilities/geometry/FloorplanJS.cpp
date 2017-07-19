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

#include "FloorplanJS.hpp"
#include "ThreeJS.hpp"
#include "Vector3d.hpp"
#include "Geometry.hpp"

#include "../core/Assert.hpp"
#include "../core/Path.hpp"
#include "../core/Json.hpp"

#include <jsoncpp/json.h>

#include <iostream>
#include <string>

namespace openstudio{

  
  FloorplanObjectId::FloorplanObjectId(const std::string& id, const std::string& name, const UUID& handle)
    : m_id(id), m_name(name), m_handle(handle), m_handleString(toString(handle))
  {}

  std::string FloorplanObjectId::id() const
  {
    return m_id;
  }

  std::string FloorplanObjectId::name() const
  {
    return m_name;
  }

  UUID FloorplanObjectId::handle() const
  {
    return m_handle;
  }

  std::string FloorplanObjectId::handleString() const
  {
    return m_handleString;
  }

  FloorplanJS::FloorplanJS()
  {
    m_value = Json::Value(Json::objectValue);
  }

  FloorplanJS::FloorplanJS(const std::string& s)
  {
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(s, m_value);

    if (!parsingSuccessful){
      std::string errors = reader.getFormattedErrorMessages();

      // see if this is a path
      openstudio::path p = toPath(s);
      if (boost::filesystem::exists(p) && boost::filesystem::is_regular_file(p)){
        // open file
        std::ifstream ifs(openstudio::toString(p));
        m_value.clear();
        parsingSuccessful = reader.parse(ifs, m_value);
      }

      if (!parsingSuccessful){
        LOG_AND_THROW("ThreeJS JSON cannot be processed, " << errors);
      }
    }
  }
    
  boost::optional<FloorplanJS> FloorplanJS::load(const std::string& json)
  {  
    try {
      FloorplanJS result(json);
      return result;
    } catch (...) {
      LOG(Error, "Could not parse JSON input");
    }
    return boost::none;
  }

  std::string FloorplanJS::toJSON(bool prettyPrint) const
  {
    // write to string
    std::string result;
    if (prettyPrint){
      Json::StyledWriter writer;
      result = writer.write(m_value);
    } else{
      Json::FastWriter writer;
      result = writer.write(m_value);
    }

    return result;
  }

  boost::optional<Json::Value> getById(const Json::Value& objects, const std::string& id)
  {
    boost::optional<Json::Value> result;
    Json::ArrayIndex n = objects.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      if (checkKeyAndType(objects[i], "id", Json::stringValue)){
        if (id == objects[i].get("id", "").asString()){
          return objects[i];
        }
      }
    }
    return boost::none;
  }

  void makeSurface(const Json::Value& story, const Json::Value& space, const std::string& spaceNamePostFix,
    const std::string& surfaceType, const Point3dVector& vertices, size_t faceFormat, 
    std::vector<ThreeGeometry>& geometries, std::vector<ThreeSceneChild>& sceneChildren)
  {
    std::string geometryId = std::string("Geometry ") + std::to_string(geometries.size());
    std::string faceId = std::string("Face ") + std::to_string(geometries.size());

    size_t n = vertices.size();

    std::vector<size_t> faces;
    faces.push_back(faceFormat);
    for (size_t i = 0; i < n; ++i){
      faces.push_back(i);
    }

    {
      std::string uuid = geometryId;
      std::string type = "Geometry";
      ThreeGeometryData data(toThreeVector(vertices), faces);
      ThreeGeometry geometry(uuid, type, data);
      geometries.push_back(geometry);
    }

    {
      std::string uuid = faceId;
      std::string name = faceId;
      std::string type = "Mesh";
      std::string materialId;

      ThreeUserData userData;

      assertKeyAndType(space, "name", Json::stringValue);
      std::string s = space.get("name", "").asString();
      userData.setSpaceName(s);

      assertKeyAndType(story, "name", Json::stringValue);
      s = story.get("name", "").asString();
      userData.setBuildingStoryName(s);

      if (checkKeyAndType(space, "building_unit_id", Json::stringValue)){
        s = space.get("building_unit_id", "").asString();
        userData.setBuildingUnitName(s);
      }

      if (checkKeyAndType(space, "thermal_zone_id", Json::stringValue)){
        s = space.get("thermal_zone_id", "").asString();
        userData.setThermalZoneName(s);
      }

      if (checkKeyAndType(space, "space_type_id", Json::stringValue)){
        s = space.get("space_type_id", "").asString();
        userData.setSpaceTypeName(s);
      }

      if (checkKeyAndType(space, "construction_set_id", Json::stringValue)){
        s = space.get("construction_set_id", "").asString();
        //userData.setConstructionSetName(s);
      }

      userData.setSurfaceType(surfaceType);

      ThreeSceneChild sceneChild(uuid, name, type, geometryId, materialId, userData);
      sceneChildren.push_back(sceneChild);
    }
  }

  void makeGeometries(const Json::Value& story, const Json::Value& space, const std::string& spaceNamePostFix, double minZ, double maxZ,
    const Json::Value& vertices, const Json::Value& edges, const Json::Value& faces, const std::string& faceId,
    bool openstudioFormat, std::vector<ThreeGeometry>& geometries, std::vector<ThreeSceneChild>& sceneChildren)
  {
    std::vector<Point3d> faceVertices;

    // get the face
    boost::optional<Json::Value> face = getById(faces, faceId);
    if (face){

      // get the edges
      Json::Value edgeIds = face->get("edge_ids", Json::arrayValue);
      Json::Value edgeOrders = face->get("edge_order", Json::arrayValue);
      Json::ArrayIndex edgeN = edgeIds.size();
      OS_ASSERT(edgeN == edgeOrders.size());

      // loop over edges
      for (Json::ArrayIndex edgeIdx = 0; edgeIdx < edgeN; ++edgeIdx){
        std::string edgeId = edgeIds[edgeIdx].asString();
        unsigned edgeOrder = edgeOrders[edgeIdx].asUInt();

        // get the edge
        boost::optional<Json::Value> edge = getById(edges, edgeId);
        if (edge){
          Json::Value vertexIds = edge->get("vertex_ids", Json::arrayValue);
          OS_ASSERT(2u == vertexIds.size());

          // get the vertices
          boost::optional<Json::Value> vertex1 = getById(vertices, vertexIds[0].asString());
          boost::optional<Json::Value> vertex2 = getById(vertices, vertexIds[1].asString());

          if (edgeOrder == 1){
            vertex1 = getById(vertices, vertexIds[0].asString());
            vertex2 = getById(vertices, vertexIds[1].asString());
          }else{
            vertex1 = getById(vertices, vertexIds[1].asString());
            vertex2 = getById(vertices, vertexIds[0].asString());
          }
          OS_ASSERT(vertex1);
          OS_ASSERT(vertex2);

          assertKeyAndType(*vertex1, "x", Json::realValue);
          assertKeyAndType(*vertex1, "y", Json::realValue);
          faceVertices.push_back(Point3d(vertex1->get("x", 0.0).asDouble(), vertex1->get("y", 0.0).asDouble(), 0.0));
        }
      }
    }

    // correct the floor vertices

    unsigned numPoints = faceVertices.size();
    if (numPoints < 3){
      //LOG(Error, "Cannot create a space for floorPrint of size " << faceVertices.size() << ".");
      return;
    }

    boost::optional<Vector3d> outwardNormal = getOutwardNormal(faceVertices);
    if (!outwardNormal){
      //LOG(Error, "Cannot compute outwardNormal for floorPrint.");
      return;
    }

    if (outwardNormal->z() > 0){
      faceVertices = reverse(faceVertices);
    }

    Point3dVectorVector allFinalFaceVertices;
    unsigned roofCeilingFaceFormat = 1024; 
    unsigned wallFaceFormat = 1024;
    if (openstudioFormat){
      allFinalFaceVertices.push_back(faceVertices);
    }else{
      roofCeilingFaceFormat = 0; // triangle
      wallFaceFormat = 1; // quad
      allFinalFaceVertices = computeTriangulation(faceVertices, Point3dVectorVector());
    }

    // create floor and ceiling
    for (const auto& finalFaceVertices : allFinalFaceVertices){
      Point3dVector finalfloorVertices;
      Point3dVector finalRoofCeilingVertices;

      for (auto& v : finalFaceVertices){
        finalfloorVertices.push_back(Point3d(v.x(), v.y(), minZ));
        finalRoofCeilingVertices.push_back(Point3d(v.x(), v.y(), maxZ));
      }

      makeSurface(story, space, spaceNamePostFix, "Floor", finalfloorVertices, roofCeilingFaceFormat, geometries, sceneChildren);
      makeSurface(story, space, spaceNamePostFix, "RoofCeiling", reverse(finalRoofCeilingVertices), roofCeilingFaceFormat, geometries, sceneChildren);
    }


    // create each wall
    for (unsigned i = 1; i <= numPoints; ++i){
      Point3dVector finalWallVertices;
      finalWallVertices.push_back(Point3d(faceVertices[i % numPoints].x(), faceVertices[i % numPoints].y(), maxZ));
      finalWallVertices.push_back(Point3d(faceVertices[i % numPoints].x(), faceVertices[i % numPoints].y(), minZ));
      finalWallVertices.push_back(Point3d(faceVertices[i - 1].x(), faceVertices[i - 1].y(), minZ));
      finalWallVertices.push_back(Point3d(faceVertices[i - 1].x(), faceVertices[i - 1].y(), maxZ));

      makeSurface(story, space, spaceNamePostFix, "Wall", finalWallVertices, wallFaceFormat, geometries, sceneChildren);
    }
    
  }

  ThreeScene FloorplanJS::toThreeScene(bool openstudioFormat) const
  {
    std::vector<ThreeGeometry> geometries;
    std::vector<ThreeMaterial> materials;
    std::vector<ThreeSceneChild> children;
    std::vector<std::string> buildingStoryNames;

    double currentZ = 0;

    // loop over stories
    Json::Value stories = m_value.get("stories", Json::arrayValue);
    Json::ArrayIndex storyN = stories.size();
    for (Json::ArrayIndex storyIdx = 0; storyIdx < storyN; ++storyIdx){

      // get story properties
      assertKeyAndType(stories[storyIdx], "name", Json::stringValue);
      std::string storyName = stories[storyIdx].get("name", "").asString();
      buildingStoryNames.push_back(storyName);

      double belowFloorPlenumHeight = 0;
      if (checkKeyAndType(stories[storyIdx], "below_floor_plenum_height", Json::realValue)){
        belowFloorPlenumHeight = stories[storyIdx].get("below_floor_plenum_height", belowFloorPlenumHeight).asDouble();
      }
       
      double floorToCeilingHeight = 3;
      if (checkKeyAndType(stories[storyIdx], "floor_to_ceiling_height", Json::realValue)){
        floorToCeilingHeight = stories[storyIdx].get("floor_to_ceiling_height", floorToCeilingHeight).asDouble();
      }
      // DLM: temp code
      if (floorToCeilingHeight < 0.1){
        floorToCeilingHeight = 3;
      }

      double aboveCeilingPlenumHeight = 0;
      if (checkKeyAndType(stories[storyIdx], "above_ceiling_plenum_height", Json::realValue)){
        aboveCeilingPlenumHeight = stories[storyIdx].get("above_ceiling_plenum_height", aboveCeilingPlenumHeight).asDouble();
      }

      // get the geometry
      assertKeyAndType(stories[storyIdx], "geometry", Json::objectValue);
      Json::Value geometry = stories[storyIdx].get("geometry", Json::arrayValue);
      Json::Value vertices = geometry.get("vertices", Json::arrayValue);
      Json::Value edges = geometry.get("edges", Json::arrayValue);
      Json::Value faces = geometry.get("faces", Json::arrayValue);

      // loop over spaces
      Json::Value spaces = stories[storyIdx].get("spaces", Json::arrayValue);
      Json::ArrayIndex spaceN = spaces.size();
      for (Json::ArrayIndex spaceIdx = 0; spaceIdx < spaceN; ++spaceIdx){

        // each space should have one face
        if (checkKeyAndType(spaces[spaceIdx], "face_id", Json::stringValue)){
          std::string faceId = spaces[spaceIdx].get("face_id", "").asString(); 

          double minZ = currentZ;
          double maxZ = currentZ + belowFloorPlenumHeight;
          if (belowFloorPlenumHeight > 0){
            makeGeometries(stories[storyIdx], spaces[spaceIdx], " Floor Plenum", minZ, maxZ, vertices, edges, faces, faceId, openstudioFormat, geometries, children);
          }

          minZ = maxZ;
          maxZ += floorToCeilingHeight;
          if (floorToCeilingHeight > 0){
            makeGeometries(stories[storyIdx], spaces[spaceIdx], "", minZ, maxZ, vertices, edges, faces, faceId, openstudioFormat, geometries, children);
          }

          minZ = maxZ;
          maxZ += aboveCeilingPlenumHeight;
          if (aboveCeilingPlenumHeight > 0){
            makeGeometries(stories[storyIdx], spaces[spaceIdx], " Plenum", minZ, maxZ, vertices, edges, faces, faceId, openstudioFormat, geometries, children);
          }
        } 

      } // spaces

      // increment height for next story
      currentZ += belowFloorPlenumHeight + floorToCeilingHeight + aboveCeilingPlenumHeight;

    } // stories

    ThreeBoundingBox boundingBox(0,0,0,0,0,0,0,0,0,0);
    ThreeSceneMetadata metadata(buildingStoryNames, boundingBox);

    ThreeSceneObject sceneObject("", children);

    ThreeScene result(metadata, geometries, materials, sceneObject);

    return result;
  }

  void FloorplanJS::updateStories(const std::vector<FloorplanObjectId>& objectIds)
  {
    updateObjects(m_value, "stories", objectIds);
  }

  void FloorplanJS::updateBuildingUnits(const std::vector<FloorplanObjectId>& objectIds)
  {
    updateObjects(m_value, "building_units", objectIds);
  }

  void FloorplanJS::updateThermalZones(const std::vector<FloorplanObjectId>& objectIds)
  {
    updateObjects(m_value, "thermal_zones", objectIds);
  }

  void FloorplanJS::updateSpaceTypes(const std::vector<FloorplanObjectId>& objectIds)
  {
    updateObjects(m_value, "space_types", objectIds);
  }

  void FloorplanJS::updateConstructionSets(const std::vector<FloorplanObjectId>& objectIds)
  {
    updateObjects(m_value, "construction_sets", objectIds);
  }

  std::string FloorplanJS::getHandleString(const Json::Value& value)
  {
    return value.get("handle", "").asString();
  }

  std::string FloorplanJS::getName(const Json::Value& value)
  {
    return value.get("name", "").asString();
  }

  Json::Value* FloorplanJS::findByHandleString(const Json::Value& value, const std::string& key, const std::string& handleString)
  {
    Json::Value values = value.get(key, Json::arrayValue);
    Json::ArrayIndex n = values.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      if (getHandleString(values[i]) == handleString){
        return &values[i];
      }
    }

    return nullptr;
  }

  Json::Value* FloorplanJS::findByNameOnly(const Json::Value& value, const std::string& key, const std::string& name)
  {
    if (name.empty()){
      return nullptr;
    }

    Json::Value values = value.get(key, Json::arrayValue);
    Json::ArrayIndex n = values.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      if (getName(values[i]) == name){
        if (getHandleString(values[i]).empty()){
          return &values[i];
        }
      }
    }

    return nullptr;
  }


  void FloorplanJS::updateObjects(Json::Value& value, const std::string& key, const std::vector<FloorplanObjectId>& objectIds)
  { 
    // ensure key exists
    if (!value.isMember(key)){
      value[key] = Json::Value(Json::arrayValue);
    }

    // remove all objects that aren't found by handle or name
    {
      std::set<std::string> names;
      std::set<std::string> handleStrings;
      for (const auto& objectId : objectIds){
        names.insert(objectId.name());
        handleStrings.insert(objectId.handleString());
      }

      std::vector<Json::ArrayIndex> indicesToRemove;
      Json::Value& values = value[key];
      Json::ArrayIndex n = values.size();
      for (Json::ArrayIndex i = 0; i < n; ++i){
        if (handleStrings.find(getHandleString(values[i])) == handleStrings.end()){
          if (names.find(getName(values[i])) == names.end()){
            indicesToRemove.push_back(i);
          }
        }
      }
      std::reverse(indicesToRemove.begin(), indicesToRemove.end());

      for (const auto& i : indicesToRemove){
        Json::Value removed;
        values.removeIndex(i, &removed);
      }
    }

    // now update names
    for (const auto& objectId : objectIds){

      Json::Value* object = findByHandleString(value, key, objectId.handleString());
      if (object){
        // ensure name is the same
        (*object)["name"] = objectId.name();
      } else {
        // find object by name only if handle is empty
        object = findByNameOnly(value, key, objectId.name());

        if (object){
          // set handle
          (*object)["handle"] = objectId.handleString();
        } else{
          // create new object
          Json::Value newObject(Json::objectValue);
          newObject["id"] = ""; // might have to provide a get next id method?
          newObject["name"] = objectId.name();
          newObject["handle"] = objectId.handleString();
          value[key].append(newObject);
        }
      }
    }
  }

} // openstudio
