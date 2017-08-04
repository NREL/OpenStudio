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

  FloorplanObject::FloorplanObject(const std::string& id, const std::string& name, const std::string& handleString)
    : m_id(id), m_name(name), m_handle(toUUID(handleString)), m_handleString(handleString)
  {}
  
  FloorplanObject::FloorplanObject(const std::string& id, const std::string& name, const UUID& handle)
    : m_id(id), m_name(name), m_handle(handle), m_handleString(toString(handle))
  {}

  FloorplanObject::FloorplanObject(const Json::Value& value)
    : m_data(value)
  {
    m_id = m_data.get("id", "").asString();
    m_data.removeMember("id");

    m_name = m_data.get("name", "").asString();
    m_data.removeMember("name");

    m_handleString = m_data.get("handle", "").asString();
    m_handle = toUUID(m_handleString);
    m_data.removeMember("handle");

    // look for different references
    std::string building_unit_id = m_data.get("building_unit_id", "").asString();
    if (!building_unit_id.empty()){
      m_objectReferenceMap.insert(std::make_pair("building_unit_id", FloorplanObject(building_unit_id, "", "")));
    }
    m_data.removeMember("building_unit_id");

    std::string thermal_zone_id = m_data.get("thermal_zone_id", "").asString();
    if (!thermal_zone_id.empty()){
      m_objectReferenceMap.insert(std::make_pair("thermal_zone_id", FloorplanObject(thermal_zone_id, "", "")));
    }
    m_data.removeMember("thermal_zone_id");

    std::string space_type_id = m_data.get("space_type_id", "").asString();
    if (!space_type_id.empty()){
      m_objectReferenceMap.insert(std::make_pair("space_type_id", FloorplanObject(space_type_id, "", "")));
    }
    m_data.removeMember("space_type_id");

    std::string construction_set_id = m_data.get("construction_set_id", "").asString();
    if (!construction_set_id.empty()){
      m_objectReferenceMap.insert(std::make_pair("construction_set_id", FloorplanObject(construction_set_id, "", "")));
    }
    m_data.removeMember("construction_set_id");

    //window_id
   
    //daylighting_control_id

  }

  std::string FloorplanObject::id() const
  {
    return m_id;
  }

  std::string FloorplanObject::name() const
  {
    return m_name;
  }

  UUID FloorplanObject::handle() const
  {
    return m_handle;
  }

  std::string FloorplanObject::handleString() const
  {
    return m_handleString;
  }

  boost::optional<std::string> FloorplanObject::parentHandleString() const
  {
    return m_parentHandleString;
  }

  void FloorplanObject::setParentHandleString(const std::string& parentHandleString)
  {
    m_parentHandleString = parentHandleString;
  }

  void FloorplanObject::resetParentHandleString()
  {
    m_parentHandleString.reset();
  }

  boost::optional<double> FloorplanObject::getDataDouble(const std::string& key) const
  {
    Json::Value value = m_data.get(key, Json::nullValue);
    if (!value.isNull() && value.isNumeric()){
      return value.asDouble();
    }
    return boost::none;
  }

  boost::optional<int> FloorplanObject::getDataInt(const std::string& key) const
  {
    Json::Value value = m_data.get(key, Json::nullValue);
    if (!value.isNull() && value.isNumeric()){
      return value.asInt();
    }
    return boost::none;
  }

  boost::optional<bool> FloorplanObject::getDataBool(const std::string& key) const
  {
    Json::Value value = m_data.get(key, Json::nullValue);
    if (!value.isNull() && value.isBool()){
      return value.asBool();
    }
    return boost::none;
  }
 
  boost::optional<std::string> FloorplanObject::getDataString(const std::string& key) const
  {
    Json::Value value = m_data.get(key, Json::nullValue);
    if (!value.isNull() && value.isString()){
      return value.asString();
    }
    return boost::none;
  }

  boost::optional<FloorplanObject> FloorplanObject::getDataReference(const std::string& key) const
  {
    const auto& it = m_objectReferenceMap.find(key);
    if (it != m_objectReferenceMap.end()){
      return it->second;
    }
    return boost::none;
  }

  void FloorplanObject::setDataDouble(const std::string& key, double value)
  {
    m_data[key] = value;
  }
  
  void FloorplanObject::setDataInt(const std::string& key, int value)
  {
    m_data[key] = value;
  }

  void FloorplanObject::setDataBool(const std::string& key, bool value)
  {
    m_data[key] = value;
  }

  void FloorplanObject::setDataString(const std::string& key, const std::string& value)
  {
    m_data[key] = value;
  }

  void FloorplanObject::setDataReference(const std::string& key, const FloorplanObject& value)
  {
    m_objectReferenceMap.insert(std::make_pair(key, value));
  }

  Json::Value FloorplanObject::data() const
  {
    return m_data;
  }

  std::map<std::string, FloorplanObject> FloorplanObject::objectReferenceMap() const
  {
    return m_objectReferenceMap;
  }

  FloorplanJS::FloorplanJS()
    : m_lastId(0)
  {
    m_value = Json::Value(Json::objectValue);
  }

  FloorplanJS::FloorplanJS(const std::string& s)
    : m_lastId(0)
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

    setLastId(m_value);
  }

  FloorplanJS::FloorplanJS(const Json::Value& value)
    : m_value(value),  m_lastId(0)
  {
    setLastId(m_value);
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

  void FloorplanJS::makeSurface(const Json::Value& story, const Json::Value& space, const std::string& spaceNamePostFix,
    const std::string& surfaceType, const Point3dVector& vertices, size_t faceFormat, 
    std::vector<ThreeGeometry>& geometries, std::vector<ThreeSceneChild>& sceneChildren) const
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

      std::string s;
      std::string id;

      // story
      assertKeyAndType(story, "name", Json::stringValue);
      s = story.get("name", "").asString();
      userData.setBuildingStoryName(s);

      if (checkKeyAndType(story, "handle", Json::stringValue)){
        s = story.get("handle", "").asString();
        userData.setBuildingStoryHandle(s);
      }

      // space
      assertKeyAndType(space, "name", Json::stringValue);
      s = space.get("name", "").asString();
      userData.setSpaceName(s);

      if (checkKeyAndType(space, "handle", Json::stringValue)){
        s = space.get("handle", "").asString();
        userData.setSpaceHandle(s);
      }

      // building unit
      if (checkKeyAndType(space, "building_unit_id", Json::stringValue)){
        id = space.get("building_unit_id", "").asString();
        if (const Json::Value* buildingUnit = findById(m_value["building_units"], id)){
          if (checkKeyAndType(*buildingUnit, "name", Json::stringValue)){
            s = buildingUnit->get("name", "").asString();
            userData.setBuildingStoryName(s);
          }

          if (checkKeyAndType(*buildingUnit, "handle", Json::stringValue)){
            s = buildingUnit->get("handle", "").asString();
            userData.setBuildingStoryHandle(s);
          }
        } else{
          userData.setBuildingUnitName(id);
        }
      }

      // thermal zone
      if (checkKeyAndType(space, "thermal_zone_id", Json::stringValue)){
        id = space.get("thermal_zone_id", "").asString();
        if (const Json::Value* thermalZone = findById(m_value["thermal_zones"], id)){
          if (checkKeyAndType(*thermalZone, "name", Json::stringValue)){
            s = thermalZone->get("name", "").asString();
            userData.setThermalZoneName(s);
          }

          if (checkKeyAndType(*thermalZone, "handle", Json::stringValue)){
            s = thermalZone->get("handle", "").asString();
            userData.setThermalZoneHandle(s);
          }
        } else{
          userData.setThermalZoneName(id);
        }
      }
      
      // space type
      if (checkKeyAndType(space, "space_type_id", Json::stringValue)){
        id = space.get("space_type_id", "").asString();
        if (const Json::Value* spaceType = findById(m_value["space_types"], id)){
          if (checkKeyAndType(*spaceType, "name", Json::stringValue)){
            s = spaceType->get("name", "").asString();
            userData.setSpaceTypeName(s);
          }

          if (checkKeyAndType(*spaceType, "handle", Json::stringValue)){
            s = spaceType->get("handle", "").asString();
            userData.setSpaceTypeHandle(s);
          }
        } else{
          userData.setSpaceTypeName(id);
        }
      }

      // construction set
      if (checkKeyAndType(space, "construction_set_id", Json::stringValue)){
        id = space.get("construction_set_id", "").asString();
        if (const Json::Value* constructionSet = findById(m_value["construction_sets"], id)){
          if (checkKeyAndType(*constructionSet, "name", Json::stringValue)){
            s = constructionSet->get("name", "").asString();
            userData.setConstructionSetName(s);
          }

          if (checkKeyAndType(*constructionSet, "handle", Json::stringValue)){
            s = constructionSet->get("handle", "").asString();
            userData.setConstructionSetHandle(s);
          }
        } else{
          userData.setConstructionSetName(id);
        }
      }

      userData.setSurfaceType(surfaceType);

      ThreeSceneChild sceneChild(uuid, name, type, geometryId, materialId, userData);
      sceneChildren.push_back(sceneChild);
    }
  }

  void FloorplanJS::makeGeometries(const Json::Value& story, const Json::Value& space, const std::string& spaceNamePostFix, double minZ, double maxZ,
    const Json::Value& vertices, const Json::Value& edges, const Json::Value& faces, const std::string& faceId,
    bool openstudioFormat, std::vector<ThreeGeometry>& geometries, std::vector<ThreeSceneChild>& sceneChildren) const
  {
    std::vector<Point3d> faceVertices;

    // get the face
    const Json::Value* face = findById(faces, faceId);
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
        const Json::Value* edge = findById(edges, edgeId);
        if (edge){
          Json::Value vertexIds = edge->get("vertex_ids", Json::arrayValue);
          OS_ASSERT(2u == vertexIds.size());

          // get the vertices
          const Json::Value* vertex1 = findById(vertices, vertexIds[0].asString());
          const Json::Value* vertex2 = findById(vertices, vertexIds[1].asString());

          if (edgeOrder == 1){
            vertex1 = findById(vertices, vertexIds[0].asString());
            vertex2 = findById(vertices, vertexIds[1].asString());
          }else{
            vertex1 = findById(vertices, vertexIds[1].asString());
            vertex2 = findById(vertices, vertexIds[0].asString());
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

  void FloorplanJS::updateStories(const std::vector<FloorplanObject>& objects, bool removeMissingObjects)
  {
    updateObjects(m_value, "stories", objects, removeMissingObjects);
  }

  void FloorplanJS::updateSpaces(const std::vector<FloorplanObject>& objects, bool removeMissingObjects)
  {
    std::map<std::string, std::vector<FloorplanObject> > storyHandleToSpaceObejctIds;

    for (const auto& object : objects){
      boost::optional<std::string> parentHandleString = object.parentHandleString();

      if (!parentHandleString){
        continue;
      }

      if (storyHandleToSpaceObejctIds.find(*parentHandleString) == storyHandleToSpaceObejctIds.end()){
        storyHandleToSpaceObejctIds[*parentHandleString] = std::vector<FloorplanObject>();
      }
      storyHandleToSpaceObejctIds[*parentHandleString].push_back(object);
    }

    for (const auto& keyValue: storyHandleToSpaceObejctIds){
      // no need to check by name, assume stories have been updated
      Json::Value* story = findByHandleString(m_value, "stories", keyValue.first);
      if (story){
        updateObjects(*story, "spaces", keyValue.second, removeMissingObjects);
      }
    }
  }

  void FloorplanJS::updateBuildingUnits(const std::vector<FloorplanObject>& objects, bool removeMissingObjects)
  {
    updateObjects(m_value, "building_units", objects, removeMissingObjects);
  }

  void FloorplanJS::updateThermalZones(const std::vector<FloorplanObject>& objects, bool removeMissingObjects)
  {
    updateObjects(m_value, "thermal_zones", objects, removeMissingObjects);
  }

  void FloorplanJS::updateSpaceTypes(const std::vector<FloorplanObject>& objects, bool removeMissingObjects)
  {
    updateObjects(m_value, "space_types", objects, removeMissingObjects);
  }

  void FloorplanJS::updateConstructionSets(const std::vector<FloorplanObject>& objects, bool removeMissingObjects)
  {
    updateObjects(m_value, "construction_sets", objects, removeMissingObjects);
  }

  std::string FloorplanJS::getHandleString(const Json::Value& value) const
  {
    return value.get("handle", "").asString();
  }

  std::string FloorplanJS::getName(const Json::Value& value) const
  {
    return value.get("name", "").asString();
  }

  std::string FloorplanJS::getId(const Json::Value& value) const
  {
    return value.get("id", "").asString();
  }

  std::string FloorplanJS::getNextId()
  {
    ++m_lastId;
    return std::to_string(m_lastId);
  }

  void FloorplanJS::setLastId(const Json::Value& value)
  {
    if (value.isObject()){
      for (const auto& key : value.getMemberNames()){
        const auto& value2 = value[key];
        if (value2.isArray()){
          Json::ArrayIndex n = value2.size();
          for (Json::ArrayIndex i = 0; i < n; ++i){
            setLastId(value2[i]);
          }
        } else if (value2.isObject()){
          setLastId(value2);
        } else if (key == "id"){
          if (value2.isString()){
            std::string s = value2.asString();
            unsigned id = strtoul(s.c_str(), nullptr, 0);
            if (id > 100){
              bool test= false;
            }
            m_lastId = std::max(m_lastId, id);
          } else if (value2.isConvertibleTo(Json::ValueType::uintValue)){
            unsigned id = value2.asUInt();
            if (id > 100){
              bool test = false;
            }
            m_lastId = std::max(m_lastId, value2.asUInt());
          }
        }
      }
    }
  }

  Json::Value* FloorplanJS::findByHandleString(Json::Value& value, const std::string& key, const std::string& handleString)
  {
    if (handleString.empty()){
      return nullptr;
    }

    Json::Value& values = value[key];
    Json::ArrayIndex n = values.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      if (getHandleString(values[i]) == handleString){
        return &values[i];
      }
    }

    return nullptr;
  }

  Json::Value* FloorplanJS::findByName(Json::Value& value, const std::string& key, const std::string& name, bool requireEmptyHandle)
  {
    if (name.empty()){
      return nullptr;
    }

    Json::Value& values = value[key];
    Json::ArrayIndex n = values.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      if (getName(values[i]) == name){
        if (requireEmptyHandle && getHandleString(values[i]).empty()){
          return &values[i];
        } else{
          return &values[i];
        }
      }
    }

    return nullptr;
  }

  Json::Value* FloorplanJS::findById(Json::Value& value, const std::string& key, const std::string& id)
  {
    if (id.empty()){
      return nullptr;
    }

    Json::Value& values = value[key];
    Json::ArrayIndex n = values.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      if (getId(values[i]) == id){
        return &values[i];
      }
    }

    return nullptr;
  }

  const Json::Value* FloorplanJS::findById(const Json::Value& values, const std::string& id) const
  {
    if (id.empty()){
      return nullptr;
    }

    Json::ArrayIndex n = values.size();
    for (Json::ArrayIndex i = 0; i < n; ++i){
      if (getId(values[i]) == id){
        return &values[i];
      }
    }

    return nullptr;
  }

  void FloorplanJS::updateObjects(Json::Value& value, const std::string& key, const std::vector<FloorplanObject>& objects, bool removeMissingObjects)
  { 
    // ensure key exists
    if (!value.isMember(key)){
      value[key] = Json::Value(Json::arrayValue);
    }

    // remove all objects that aren't found by handle or name
    if (removeMissingObjects)
    {
      std::set<std::string> ids;
      std::set<std::string> names;
      std::set<std::string> handleStrings;
      for (const auto& object : objects){
        ids.insert(object.id());
        names.insert(object.name());
        handleStrings.insert(object.handleString());
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

    // now update names and data
    for (const auto& object : objects){

      Json::Value* v = findByHandleString(value, key, object.handleString());
      if (v){
        // ensure name is the same
        (*v)["name"] = object.name();
      } else {
        // find object by name only if handle is empty
        v = findByName(value, key, object.name(), true);

        if (v){
          // set handle
          (*v)["handle"] = object.handleString();
        } else{
          // create new object
          Json::Value newObject(Json::objectValue);
          newObject["id"] = getNextId();
          newObject["name"] = object.name();
          newObject["handle"] = object.handleString();
          v = &(value[key].append(newObject));
        }
      }
     
      if (v){
        // update properties
        Json::Value data = object.data();
        for (const auto& key : data.getMemberNames()){
          (*v)[key] = data[key];
        }

        // update references
        for (const auto& p : object.objectReferenceMap()){
          updateObjectReference(*v, p.first, p.second, removeMissingObjects);
        }
      }
    }
  }

  void FloorplanJS::updateObjectReference(Json::Value& value, const std::string& key, const FloorplanObject& objectReference, bool removeMissingObjects)
  {
    std::string searchKey;
    if (key == "thermal_zone_id"){
      searchKey = "thermal_zones";
    }else if (key == "space_type_id"){
      searchKey = "space_types";
    }else if (key == "building_unit_id"){
      searchKey = "building_units";
    } else if (key == "construction_set_id"){
      searchKey = "construction_sets";
    }

    if (searchKey.empty()){
      LOG(Error, "Could not find objects to search for key '" << key << "'");
      return;
    }

    Json::Value* v = findByHandleString(m_value, searchKey, objectReference.handleString());
    if (v){
      value[key] = v->get("id", "").asString();
      return;
    } 

    v = findById(m_value, searchKey, objectReference.id());
    if (v){
      value[key] = v->get("id", "").asString();
      return;
    } 

    v = findByName(m_value, searchKey, objectReference.name(), true);
    if (v){
      value[key] = v->get("id", "").asString();
      return;
    } 

    value[key] = "";
  }

} // openstudio
