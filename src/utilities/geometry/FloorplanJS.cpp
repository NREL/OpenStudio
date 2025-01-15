/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "FloorplanJS.hpp"
#include "ThreeJS.hpp"
#include "Vector3d.hpp"
#include "Geometry.hpp"
#include "Intersection.hpp"

#include "../core/Assert.hpp"
//#include "../core/Path.hpp"
#include "../core/Json.hpp"

namespace openstudio {

static constexpr auto BELOWFLOORPLENUMPOSTFIX(" Floor Plenum");
static constexpr auto ABOVECEILINGPLENUMPOSTFIX(" Plenum");
static constexpr auto PLENUMSPACETYPENAME("Plenum Space Type");  // DLM: needs to be coordinated with name in Model_Impl::plenumSpaceType()
static constexpr auto PLENUMCOLOR("#C0C0C0");                    // DLM: needs to be coordinated with plenum colors in makeStandardThreeMaterials

FloorplanObject::FloorplanObject(const std::string& id, const std::string& name, const std::string& handleString)
  : m_id(id), m_name(name), m_handle(toUUID(handleString)), m_handleString(handleString) {}

FloorplanObject::FloorplanObject(const std::string& id, const std::string& name, const UUID& handle)
  : m_id(id), m_name(name), m_handle(handle), m_handleString(toString(handle)) {}

FloorplanObject::FloorplanObject(const Json::Value& value) : m_data(value) {
  m_id = m_data.get("id", "").asString();
  m_data.removeMember("id");

  m_name = m_data.get("name", "").asString();
  m_data.removeMember("name");

  m_handleString = m_data.get("handle", "").asString();
  m_handle = toUUID(m_handleString);
  m_data.removeMember("handle");

  // look for different references
  std::string building_unit_id = m_data.get("building_unit_id", "").asString();
  if (!building_unit_id.empty()) {
    m_objectReferenceMap.insert(std::make_pair("building_unit_id", FloorplanObject(building_unit_id, "", "")));
  }
  m_data.removeMember("building_unit_id");

  std::string thermal_zone_id = m_data.get("thermal_zone_id", "").asString();
  if (!thermal_zone_id.empty()) {
    m_objectReferenceMap.insert(std::make_pair("thermal_zone_id", FloorplanObject(thermal_zone_id, "", "")));
  }
  m_data.removeMember("thermal_zone_id");

  std::string space_type_id = m_data.get("space_type_id", "").asString();
  if (!space_type_id.empty()) {
    m_objectReferenceMap.insert(std::make_pair("space_type_id", FloorplanObject(space_type_id, "", "")));
  }
  m_data.removeMember("space_type_id");

  std::string construction_set_id = m_data.get("construction_set_id", "").asString();
  if (!construction_set_id.empty()) {
    m_objectReferenceMap.insert(std::make_pair("construction_set_id", FloorplanObject(construction_set_id, "", "")));
  }
  m_data.removeMember("construction_set_id");

  //windows::window_definition_id

  //daylighting_controls::daylighting_control_definition_id

  //doors::door_definition_id
}

std::string FloorplanObject::id() const {
  return m_id;
}

std::string FloorplanObject::name() const {
  return m_name;
}

UUID FloorplanObject::handle() const {
  return m_handle;
}

std::string FloorplanObject::handleString() const {
  return m_handleString;
}

boost::optional<std::string> FloorplanObject::parentHandleString() const {
  return m_parentHandleString;
}

void FloorplanObject::setParentHandleString(const std::string& parentHandleString) {
  m_parentHandleString = parentHandleString;
}

void FloorplanObject::resetParentHandleString() {
  m_parentHandleString.reset();
}

boost::optional<double> FloorplanObject::getDataDouble(const std::string& key) const {
  Json::Value value = m_data.get(key, Json::nullValue);
  if (!value.isNull() && value.isNumeric()) {
    return value.asDouble();
  }
  return boost::none;
}

boost::optional<int> FloorplanObject::getDataInt(const std::string& key) const {
  Json::Value value = m_data.get(key, Json::nullValue);
  if (!value.isNull() && value.isNumeric()) {
    return value.asInt();
  }
  return boost::none;
}

boost::optional<bool> FloorplanObject::getDataBool(const std::string& key) const {
  Json::Value value = m_data.get(key, Json::nullValue);
  if (!value.isNull() && value.isBool()) {
    return value.asBool();
  }
  return boost::none;
}

boost::optional<std::string> FloorplanObject::getDataString(const std::string& key) const {
  Json::Value value = m_data.get(key, Json::nullValue);
  if (!value.isNull() && value.isString()) {
    return value.asString();
  }
  return boost::none;
}

boost::optional<FloorplanObject> FloorplanObject::getDataReference(const std::string& key) const {
  const auto& it = m_objectReferenceMap.find(key);
  if (it != m_objectReferenceMap.end()) {
    return it->second;
  }
  return boost::none;
}

void FloorplanObject::setDataDouble(const std::string& key, double value) {
  m_data[key] = value;
}

void FloorplanObject::setDataInt(const std::string& key, int value) {
  m_data[key] = value;
}

void FloorplanObject::setDataBool(const std::string& key, bool value) {
  m_data[key] = value;
}

void FloorplanObject::setDataString(const std::string& key, const std::string& value) {
  m_data[key] = value;
}

void FloorplanObject::setDataReference(const std::string& key, const FloorplanObject& value) {
  m_objectReferenceMap.insert(std::make_pair(key, value));
}

Json::Value FloorplanObject::data() const {
  return m_data;
}

std::map<std::string, FloorplanObject> FloorplanObject::objectReferenceMap() const {
  return m_objectReferenceMap;
}

FloorplanJS::FloorplanJS() : m_value(Json::Value(Json::objectValue)), m_lastId(0) {}

// cppcheck-suppress funcArgNamesDifferent
FloorplanJS::FloorplanJS(const std::string& s) : m_lastId(0) {
  Json::CharReaderBuilder rbuilder;
  std::istringstream ss(s);
  std::string formattedErrors;
  bool parsingSuccessful = Json::parseFromStream(rbuilder, ss, &m_value, &formattedErrors);

  if (!parsingSuccessful) {

    // see if this is a path
    openstudio::path p = toPath(s);
    if (boost::filesystem::exists(p) && boost::filesystem::is_regular_file(p)) {
      // open file
      std::ifstream ifs(openstudio::toSystemFilename(p));
      m_value.clear();
      formattedErrors.clear();
      parsingSuccessful = Json::parseFromStream(rbuilder, ifs, &m_value, &formattedErrors);
    }

    if (!parsingSuccessful) {
      LOG_AND_THROW("ThreeJS JSON cannot be processed, " << formattedErrors);
    }
  }

  // DLM: should update value to current schema
  // Code in FloorspaceJS, importFloorplan.js, importState
  // Code in FloorspaceJS, export.js, exportData

  setLastId(m_value);
}

FloorplanJS::FloorplanJS(const Json::Value& value) : m_value(value), m_lastId(0) {
  // DLM: should update value to current schema
  // Code in FloorspaceJS, importFloorplan.js, importState
  // Code in FloorspaceJS, export.js, exportData
  // past files for testing: floorspace.js/test/e2e/fixtures/

  setLastId(m_value);
}

boost::optional<FloorplanJS> FloorplanJS::load(const std::string& json) {
  try {
    FloorplanJS result(json);
    return result;
  } catch (...) {
    LOG(Error, "Could not parse JSON input");
  }
  return boost::none;
}

std::string FloorplanJS::toJSON(bool prettyPrint) const {
  // Write to string
  Json::StreamWriterBuilder wbuilder;

  if (prettyPrint) {
    // mimic the old StyledWriter behavior:
    wbuilder["commentStyle"] = "All";
    // From source, it seems indentation was set to 3 spaces, rather than the new default of '\t'
    wbuilder["indentation"] = "   ";
  } else {
    // mimic the old FastWriter behavior:
    wbuilder["commentStyle"] = "None";
    wbuilder["indentation"] = "";
  }

  std::string result = Json::writeString(wbuilder, m_value);

  return result;
}

size_t getVertexIndex(const Point3d& point3d, std::vector<Point3d>& allPoints, double tol = 0.001) {
  size_t n = allPoints.size();
  for (size_t i = 0; i < n; ++i) {
    if (std::sqrt(std::pow(point3d.x() - allPoints[i].x(), 2) + std::pow(point3d.y() - allPoints[i].y(), 2)
                  + std::pow(point3d.z() - allPoints[i].z(), 2))
        < tol) {
      return i;
    }
  }
  allPoints.push_back(point3d);
  return (allPoints.size() - 1);
}

std::string FloorplanJS::makeSurface(const Json::Value& story, const Json::Value& spaceOrShading, const std::string& parentSurfaceName,
                                     const std::string& parentSubSurfaceName, bool belowFloorPlenum, bool aboveCeilingPlenum,
                                     const std::string& surfaceType, const Point3dVectorVector& finalFaceVertices, size_t faceFormat,
                                     std::vector<ThreeGeometry>& geometries, std::vector<ThreeSceneChild>& sceneChildren, double illuminanceSetpoint,
                                     bool airWall) const {
  std::string finalSurfaceType = surfaceType;

  bool isShading = false;
  bool isSpace = false;
  assertKeyAndType(spaceOrShading, "type", Json::stringValue);
  std::string type = spaceOrShading.get("type", "").asString();
  if (istringEqual(type, "space")) {
    isSpace = true;
  } else if (istringEqual(type, "shading")) {
    isShading = true;
    // DLM: TODO add key to distinguish site and building shading
    finalSurfaceType = "SiteShading";
  } else {
    LOG(Error, "Unknown type '" << type << "'");
  }

  bool plenum = false;
  std::string spaceOrShadingNamePostFix;
  if (belowFloorPlenum) {
    plenum = true;
    spaceOrShadingNamePostFix = BELOWFLOORPLENUMPOSTFIX;
  } else if (aboveCeilingPlenum) {
    plenum = true;
    spaceOrShadingNamePostFix = ABOVECEILINGPLENUMPOSTFIX;
  }

  std::string geometryId = std::string("Geometry ") + std::to_string(geometries.size());
  std::string faceId = std::string("Face ") + std::to_string(geometries.size());

  Point3dVector allVertices;
  std::vector<size_t> faceIndices;
  for (const auto& finalFaceVerts : finalFaceVertices) {
    faceIndices.push_back(faceFormat);
    for (const auto& vert : finalFaceVerts) {
      faceIndices.push_back(getVertexIndex(vert, allVertices));
    }
  }

  {
    std::string uuid = geometryId;
    type = "Geometry";
    ThreeGeometryData data(toThreeVector(allVertices), faceIndices);
    ThreeGeometry geometry(uuid, type, data);
    geometries.push_back(geometry);
  }

  {
    std::string uuid = faceId;
    std::string name = faceId;
    type = "Mesh";
    std::string materialId;

    ThreeUserData userData;
    userData.setName(faceId);

    std::string s;
    std::string id;

    // story
    assertKeyAndType(story, "name", Json::stringValue);
    s = story.get("name", "").asString();
    userData.setBuildingStoryName(s);
    userData.setBuildingStoryMaterialName(getObjectThreeMaterialName("OS:BuildingStory", s));

    if (checkKeyAndType(story, "handle", Json::stringValue)) {
      s = story.get("handle", "").asString();
      userData.setBuildingStoryHandle(s);
    }

    // space or shading
    assertKeyAndType(spaceOrShading, "name", Json::stringValue);
    s = spaceOrShading.get("name", "").asString();
    if (isSpace) {
      userData.setSpaceName(s + spaceOrShadingNamePostFix);
    } else if (isShading) {
      userData.setShadingName(s + spaceOrShadingNamePostFix);
    }

    if (checkKeyAndType(spaceOrShading, "handle", Json::stringValue)) {
      s = spaceOrShading.get("handle", "").asString();
      if (isSpace) {
        userData.setSpaceHandle(s);
      } else if (isShading) {
        userData.setShadingHandle(s);
      }
    }

    // parent surface
    userData.setSurfaceName(parentSurfaceName);

    // parent sub surface
    userData.setSubSurfaceName(parentSubSurfaceName);

    // building unit
    if (checkKeyAndType(spaceOrShading, "building_unit_id", Json::stringValue)) {
      id = spaceOrShading.get("building_unit_id", "").asString();
      if (const Json::Value* buildingUnit = findById(m_value["building_units"], id)) {
        assertKeyAndType(*buildingUnit, "name", Json::stringValue);
        s = buildingUnit->get("name", "").asString();
        userData.setBuildingUnitName(s);
        userData.setBuildingUnitMaterialName(getObjectThreeMaterialName("OS:BuildingUnit", s));

        if (checkKeyAndType(*buildingUnit, "handle", Json::stringValue)) {
          s = buildingUnit->get("handle", "").asString();
          userData.setBuildingUnitHandle(s);
        }
      } else {
        LOG(Error, "Cannot find BuildingUnit '" << id << "'");
      }
    }

    // thermal zone
    if (checkKeyAndType(spaceOrShading, "thermal_zone_id", Json::stringValue)) {
      id = spaceOrShading.get("thermal_zone_id", "").asString();
      if (const Json::Value* thermalZone = findById(m_value["thermal_zones"], id)) {
        assertKeyAndType(*thermalZone, "name", Json::stringValue);
        s = thermalZone->get("name", "").asString();
        std::string thermalZoneName = s + spaceOrShadingNamePostFix;
        userData.setThermalZoneName(thermalZoneName);
        userData.setThermalZoneMaterialName(getObjectThreeMaterialName("OS:ThermalZone", s));

        if (plenum) {
          // DLM: we do not have this plenum thermal zone in the floorplan so user can't edit its properties
          m_plenumThermalZoneNames.insert(thermalZoneName);

          // reset to ThermalZone_Plenum in makeStandardThreeMaterials
          userData.setThermalZoneMaterialName(getObjectThreeMaterialName("OS:ThermalZone", "Plenum"));
        } else {
          if (checkKeyAndType(*thermalZone, "handle", Json::stringValue)) {
            s = thermalZone->get("handle", "").asString();
            userData.setThermalZoneHandle(s);
          }
        }
      } else {
        LOG(Error, "Cannot find ThermalZone '" << id << "'");
      }
    }

    // space type
    if (plenum) {
      userData.setSpaceTypeName(PLENUMSPACETYPENAME);
      userData.setSpaceTypeMaterialName(getObjectThreeMaterialName("OS:SpaceType", "Plenum"));
    } else {
      if (checkKeyAndType(spaceOrShading, "space_type_id", Json::stringValue)) {
        id = spaceOrShading.get("space_type_id", "").asString();
        if (const Json::Value* spaceType = findById(m_value["space_types"], id)) {
          assertKeyAndType(*spaceType, "name", Json::stringValue);
          s = spaceType->get("name", "").asString();
          userData.setSpaceTypeName(s);
          userData.setSpaceTypeMaterialName(getObjectThreeMaterialName("OS:SpaceType", s));

          if (checkKeyAndType(*spaceType, "handle", Json::stringValue)) {
            s = spaceType->get("handle", "").asString();
            userData.setSpaceTypeHandle(s);
          }
        } else {
          LOG(Error, "Cannot find SpaceType '" << id << "'");
        }
      }
    }

    // construction set
    if (checkKeyAndType(spaceOrShading, "construction_set_id", Json::stringValue)) {
      id = spaceOrShading.get("construction_set_id", "").asString();
      if (const Json::Value* constructionSet = findById(m_value["construction_sets"], id)) {
        assertKeyAndType(*constructionSet, "name", Json::stringValue);
        s = constructionSet->get("name", "").asString();
        userData.setConstructionSetName(s);
        //userData.setConstructionSetNameMaterialName(getObjectThreeMaterialName("OS:DefaultConstructionSet", s));

        if (checkKeyAndType(*constructionSet, "handle", Json::stringValue)) {
          s = constructionSet->get("handle", "").asString();
          userData.setConstructionSetHandle(s);
        }
      } else {
        LOG(Error, "Cannot find ConstructionSet '" << id << "'");
      }
    }

    userData.setSurfaceType(finalSurfaceType);
    userData.setSurfaceTypeMaterialName(getSurfaceTypeThreeMaterialName(finalSurfaceType));
    //userData.setAboveCeilingPlenum(aboveCeilingPlenum);
    //userData.setBelowFloorPlenum(belowFloorPlenum);
    userData.setIlluminanceSetpoint(illuminanceSetpoint);
    userData.setAirWall(airWall);

    ThreeSceneChild sceneChild(uuid, name, type, geometryId, materialId, userData);
    sceneChildren.push_back(sceneChild);
  }

  return faceId;
}

void FloorplanJS::makeGeometries(const Json::Value& story, const Json::Value& spaceOrShading, bool belowFloorPlenum, bool aboveCeilingPlenum,
                                 double lengthToMeters, double minZ, double maxZ, const Json::Value& vertices, const Json::Value& edges,
                                 const Json::Value& faces, const std::string& faceId, bool openstudioFormat, std::vector<ThreeGeometry>& geometries,
                                 std::vector<ThreeSceneChild>& sceneChildren, bool openToBelow) const {
  std::vector<Point3d> faceVertices;
  std::vector<Point3d> windowCenterVertices;
  std::vector<std::string> windowDefinitionIds;
  // TODO? std::vector<Point3d> daylightingControlVertices;
  std::vector<Point3d> doorCenterVertices;
  std::vector<std::string> doorDefinitionIds;

  const Json::Value windowDefinitions = m_value.get("window_definitions", Json::arrayValue);
  const Json::Value daylightingControlDefinitions = m_value.get("daylighting_control_definitions", Json::arrayValue);
  const Json::Value doorDefinitions = m_value.get("door_definitions", Json::arrayValue);

  // get all the windows on this story unless this is a plenum
  std::map<std::string, std::vector<Json::Value>> edgeIdToWindowsMap;
  if (!belowFloorPlenum && !aboveCeilingPlenum) {
    for (const auto& window : story.get("windows", Json::arrayValue)) {
      assertKeyAndType(window, "edge_id", Json::stringValue);

      std::string edgeId = window.get("edge_id", "").asString();
      if (edgeIdToWindowsMap.find(edgeId) == edgeIdToWindowsMap.end()) {
        // cppcheck-suppress stlFindInsert
        edgeIdToWindowsMap[edgeId] = std::vector<Json::Value>();
      }
      edgeIdToWindowsMap[edgeId].push_back(window);
    }
  }

  // get all the doors on this story unless this is a plenum
  std::map<std::string, std::vector<Json::Value>> edgeIdToDoorsMap;
  if (!belowFloorPlenum && !aboveCeilingPlenum) {
    for (const auto& door : story.get("doors", Json::arrayValue)) {
      assertKeyAndType(door, "edge_id", Json::stringValue);

      std::string edgeId = door.get("edge_id", "").asString();
      if (edgeIdToDoorsMap.find(edgeId) == edgeIdToDoorsMap.end()) {
        // cppcheck-suppress stlFindInsert
        edgeIdToDoorsMap[edgeId] = std::vector<Json::Value>();
      }
      edgeIdToDoorsMap[edgeId].push_back(door);
    }
  }

  // get the face
  const Json::Value* face = findById(faces, faceId);
  if (face) {

    // get the edges
    Json::Value edgeIds = face->get("edge_ids", Json::arrayValue);
    Json::Value edgeOrders = face->get("edge_order", Json::arrayValue);
    Json::ArrayIndex edgeN = edgeIds.size();
    OS_ASSERT(edgeN == edgeOrders.size());

    // loop over edges
    for (Json::ArrayIndex edgeIdx = 0; edgeIdx < edgeN; ++edgeIdx) {
      std::string edgeId = edgeIds[edgeIdx].asString();
      unsigned edgeOrder = edgeOrders[edgeIdx].asUInt();

      // get the edge
      const Json::Value* edge = findById(edges, edgeId);
      if (edge) {
        Json::Value vertexIds = edge->get("vertex_ids", Json::arrayValue);
        OS_ASSERT(2u == vertexIds.size());

        // get the vertices
        const Json::Value* nextVertex;
        const Json::Value* vertex1 = findById(vertices, vertexIds[0].asString());
        const Json::Value* vertex2 = findById(vertices, vertexIds[1].asString());

        if (edgeOrder == 1) {
          nextVertex = vertex1;
        } else {
          nextVertex = vertex2;
        }
        OS_ASSERT(nextVertex);
        OS_ASSERT(vertex1);
        OS_ASSERT(vertex2);

        assertKeyAndType(*nextVertex, "x", Json::realValue);
        assertKeyAndType(*nextVertex, "y", Json::realValue);
        faceVertices.push_back(
          Point3d(lengthToMeters * nextVertex->get("x", 0.0).asDouble(), lengthToMeters * nextVertex->get("y", 0.0).asDouble(), 0.0));

        // check if there are windows on this edge
        if (edgeIdToWindowsMap.find(edgeId) != edgeIdToWindowsMap.end()) {
          std::vector<Json::Value> windows = edgeIdToWindowsMap[edgeId];
          for (const auto& window : windows) {
            assertKeyAndType(window, "window_definition_id", Json::stringValue);
            std::string windowDefinitionId = window.get("window_definition_id", "").asString();

            std::vector<double> alphas;
            // do an extra check here to avoid a warning message
            if (window.isMember("alpha") && window["alpha"].isConvertibleTo(Json::realValue)) {
              if (checkKeyAndType(window, "alpha", Json::realValue)) {
                alphas.push_back(window.get("alpha", 0.0).asDouble());
              }
            } else {
              if (checkKeyAndType(window, "alpha", Json::arrayValue)) {
                Json::Value temp = window.get("alpha", Json::arrayValue);
                Json::ArrayIndex tempN = temp.size();
                for (Json::ArrayIndex tempIdx = 0; tempIdx < tempN; ++tempIdx) {
                  alphas.push_back(temp[tempIdx].asDouble());
                }
              }
            }

            for (const auto& alpha : alphas) {
              double x = (1.0 - alpha) * vertex1->get("x", 0.0).asDouble() + alpha * vertex2->get("x", 0.0).asDouble();
              double y = (1.0 - alpha) * vertex1->get("y", 0.0).asDouble() + alpha * vertex2->get("y", 0.0).asDouble();

              windowDefinitionIds.push_back(windowDefinitionId);
              windowCenterVertices.push_back(Point3d(lengthToMeters * x, lengthToMeters * y, 0.0));
            }
          }
        }

        // check if there are doors on this edge
        if (edgeIdToDoorsMap.find(edgeId) != edgeIdToDoorsMap.end()) {
          std::vector<Json::Value> doors = edgeIdToDoorsMap[edgeId];
          for (const auto& door : doors) {
            assertKeyAndType(door, "door_definition_id", Json::stringValue);
            std::string doorDefinitionId = door.get("door_definition_id", "").asString();

            std::vector<double> alphas;
            if (checkKeyAndType(door, "alpha", Json::realValue)) {
              alphas.push_back(door.get("alpha", 0.0).asDouble());
            } else if (checkKeyAndType(door, "alpha", Json::arrayValue)) {
              Json::Value temp = door.get("alpha", Json::arrayValue);
              Json::ArrayIndex tempN = temp.size();
              for (Json::ArrayIndex tempIdx = 0; tempIdx < tempN; ++tempIdx) {
                alphas.push_back(temp[tempIdx].asDouble());
              }
            }

            for (const auto& alpha : alphas) {
              double x = (1.0 - alpha) * vertex1->get("x", 0.0).asDouble() + alpha * vertex2->get("x", 0.0).asDouble();
              double y = (1.0 - alpha) * vertex1->get("y", 0.0).asDouble() + alpha * vertex2->get("y", 0.0).asDouble();

              doorDefinitionIds.push_back(doorDefinitionId);
              doorCenterVertices.push_back(Point3d(lengthToMeters * x, lengthToMeters * y, 0.0));
            }
          }
        }
      }
    }
  }

  // correct the floor vertices

  // simplify the vertices to remove potential duplicate, colinear points
  double tol = 0.001;
  //std::vector<Point3d> testFaceVertices(faceVertices);
  faceVertices = simplify(faceVertices, false, tol);

  size_t numPoints = faceVertices.size();
  if (numPoints < 3) {
    //LOG(Error, "Cannot create a space for floorPrint of size " << faceVertices.size() << ".");
    return;
  }

  boost::optional<Vector3d> outwardNormal = getOutwardNormal(faceVertices);
  if (!outwardNormal) {
    //LOG(Error, "Cannot compute outwardNormal for floorPrint.");
    return;
  }

  if (outwardNormal->z() > 0) {
    faceVertices = reverse(faceVertices);
  }

  Point3dVectorVector allFinalFaceVertices;
  unsigned roofCeilingFaceFormat = openstudioFaceFormatId();
  unsigned wallFaceFormat = openstudioFaceFormatId();
  if (openstudioFormat) {
    allFinalFaceVertices.push_back(faceVertices);
  } else {
    roofCeilingFaceFormat = 0;  // triangle
    wallFaceFormat = 1;         // quad
    //std::ostringstream ss;
    //ss << testFaceVertices;
    //std::string testStr = ss.str();
    allFinalFaceVertices = computeTriangulation(faceVertices, Point3dVectorVector());
  }

  // create floor and ceiling
  Point3dVectorVector allFinalfloorVertices;
  Point3dVectorVector allFinalRoofCeilingVertices;
  for (const auto& finalFaceVertices : allFinalFaceVertices) {
    Point3dVector finalfloorVertices;
    Point3dVector finalRoofCeilingVertices;
    for (const auto& v : finalFaceVertices) {
      Point3d floorVert(v.x(), v.y(), minZ);
      Point3d ceilVert(v.x(), v.y(), maxZ);
      finalfloorVertices.push_back(floorVert);
      finalRoofCeilingVertices.push_back(ceilVert);
      m_boundingBox.addPoint(floorVert);
      m_boundingBox.addPoint(ceilVert);
    }
    std::reverse(finalRoofCeilingVertices.begin(), finalRoofCeilingVertices.end());
    allFinalfloorVertices.push_back(finalfloorVertices);
    allFinalRoofCeilingVertices.push_back(finalRoofCeilingVertices);
  }
  makeSurface(story, spaceOrShading, "", "", belowFloorPlenum, aboveCeilingPlenum, "Floor", allFinalfloorVertices, roofCeilingFaceFormat, geometries,
              sceneChildren, 0, openToBelow);
  makeSurface(story, spaceOrShading, "", "", belowFloorPlenum, aboveCeilingPlenum, "RoofCeiling", allFinalRoofCeilingVertices, roofCeilingFaceFormat,
              geometries, sceneChildren, 0, false);

  // create each wall
  std::set<unsigned> mappedWindows;
  std::set<unsigned> mappedDoors;
  for (unsigned i = 1; i <= numPoints; ++i) {
    Point3dVector wallVertices{
      {faceVertices[i - 1].x(), faceVertices[i - 1].y(), maxZ},
      {faceVertices[i % numPoints].x(), faceVertices[i % numPoints].y(), maxZ},
      {faceVertices[i % numPoints].x(), faceVertices[i % numPoints].y(), minZ},
      {faceVertices[i - 1].x(), faceVertices[i - 1].y(), minZ},
    };

    // find windows that appear on this edge, can't use edge ids after simplify algorithm
    std::vector<Point3d> testSegment{
      {faceVertices[i - 1].x(), faceVertices[i - 1].y(), 0.0},
      {faceVertices[i % numPoints].x(), faceVertices[i % numPoints].y(), 0.0},
    };

    Vector3d edgeVector = testSegment[1] - testSegment[0];
    edgeVector.setLength(1.0);
    Vector3d upVector(0, 0, 1);
    Vector3d crossVector = upVector.cross(edgeVector);

    Point3dVectorVector allFinalWindowVertices;
    std::vector<std::string> allFinalWindowTypes;
    Point3dVectorVector allFinalShadeVertices;
    std::vector<unsigned> allFinalShadeParentSubSurfaceIndices;

    size_t windowN = windowCenterVertices.size();
    OS_ASSERT(windowN == windowDefinitionIds.size());
    for (unsigned windowIdx = 0; windowIdx < windowN; ++windowIdx) {
      if (mappedWindows.find(windowIdx) == mappedWindows.end()) {
        if (getDistancePointToLineSegment(windowCenterVertices[windowIdx], testSegment) < tol) {

          // get window definition

          const Json::Value* windowDefinition = findById(windowDefinitions, windowDefinitionIds[windowIdx]);
          if (windowDefinition) {
            std::string windowDefinitionMode;
            if (checkKeyAndType(*windowDefinition, "window_definition_type", Json::stringValue)) {
              windowDefinitionMode = windowDefinition->get("window_definition_type", "").asString();
            } else {
              assertKeyAndType(*windowDefinition, "window_definition_mode", Json::stringValue);
              windowDefinitionMode = windowDefinition->get("window_definition_mode", "").asString();
            }

            // "Fixed","Operable"
            std::string windowType = "FixedWindow";
            if (checkKeyAndType(*windowDefinition, "window_type", Json::stringValue)) {
              windowType = windowDefinition->get("window_type", windowType).asString();
            }
            if (istringEqual(windowType, "Fixed")) {
              windowType = "FixedWindow";
            } else if (istringEqual(windowType, "Operable")) {
              windowType = "OperableWindow";
            }
            allFinalWindowTypes.push_back(windowType);

            if (istringEqual("Single Window", windowDefinitionMode) || istringEqual("Repeating Windows", windowDefinitionMode)) {
              assertKeyAndType(*windowDefinition, "sill_height", Json::realValue);
              assertKeyAndType(*windowDefinition, "height", Json::realValue);
              assertKeyAndType(*windowDefinition, "width", Json::realValue);

              double sillHeight = lengthToMeters * windowDefinition->get("sill_height", 0.0).asDouble();
              double height = lengthToMeters * windowDefinition->get("height", 0.0).asDouble();
              double width = lengthToMeters * windowDefinition->get("width", 0.0).asDouble();

              Vector3d widthVector = edgeVector;
              widthVector.setLength(0.5 * width);
              Point3d window1 = windowCenterVertices[windowIdx] + widthVector;
              widthVector.setLength(-0.5 * width);
              Point3d window2 = windowCenterVertices[windowIdx] + widthVector;

              Point3dVector windowVertices{
                {window1.x(), window1.y(), minZ + sillHeight + height},
                {window1.x(), window1.y(), minZ + sillHeight},
                {window2.x(), window2.y(), minZ + sillHeight},
                {window2.x(), window2.y(), minZ + sillHeight + height},
              };

              size_t parentSubSurfaceIndex = allFinalWindowVertices.size();
              allFinalWindowVertices.push_back(windowVertices);

              if (checkKeyAndType(*windowDefinition, "overhang_projection_factor", Json::realValue)
                  || checkKeyAndType(*windowDefinition, "overhang_projection_factor", Json::intValue)) {
                double projectionFactor = windowDefinition->get("overhang_projection_factor", 0.0).asDouble();

                if (projectionFactor > 0) {
                  Vector3d outVector = crossVector;
                  outVector.setLength(projectionFactor * height);

                  Point3d window3 = window1 + outVector;
                  Point3d window4 = window2 + outVector;

                  Point3dVector shadeVertices{
                    {window1.x(), window1.y(), minZ + sillHeight + height},
                    {window3.x(), window3.y(), minZ + sillHeight + height},
                    {window4.x(), window4.y(), minZ + sillHeight + height},
                    {window2.x(), window2.y(), minZ + sillHeight + height},
                  };

                  allFinalShadeVertices.push_back(shadeVertices);
                  allFinalShadeParentSubSurfaceIndices.push_back(parentSubSurfaceIndex);
                }
              }

              if (checkKeyAndType(*windowDefinition, "fin_projection_factor", Json::realValue)
                  || checkKeyAndType(*windowDefinition, "fin_projection_factor", Json::intValue)) {
                double projectionFactor = windowDefinition->get("fin_projection_factor", 0.0).asDouble();

                if (projectionFactor > 0) {
                  Vector3d outVector = crossVector;
                  outVector.setLength(projectionFactor * height);

                  Point3d window3 = window1 + outVector;
                  Point3d window4 = window2 + outVector;

                  Point3dVector shadeVertices{
                    {window1.x(), window1.y(), minZ + sillHeight + height},
                    {window1.x(), window1.y(), minZ + sillHeight},
                    {window3.x(), window3.y(), minZ + sillHeight},
                    {window3.x(), window3.y(), minZ + sillHeight + height},
                  };

                  allFinalShadeVertices.push_back(shadeVertices);
                  allFinalShadeParentSubSurfaceIndices.push_back(parentSubSurfaceIndex);

                  shadeVertices = {
                    {window4.x(), window4.y(), minZ + sillHeight + height},
                    {window4.x(), window4.y(), minZ + sillHeight},
                    {window2.x(), window2.y(), minZ + sillHeight},
                    {window2.x(), window2.y(), minZ + sillHeight + height},
                  };

                  allFinalShadeVertices.push_back(shadeVertices);
                  allFinalShadeParentSubSurfaceIndices.push_back(parentSubSurfaceIndex);
                }
              }

            } else if (istringEqual("Window to Wall Ratio", windowDefinitionMode)) {
              assertKeyAndType(*windowDefinition, "sill_height", Json::realValue);
              assertKeyAndType(*windowDefinition, "wwr", Json::realValue);

              double sillHeight = lengthToMeters * windowDefinition->get("sill_height", 0.0).asDouble();
              double wwr = windowDefinition->get("wwr", 0.0).asDouble();

              double projectionFactor = 0.0;
              if (checkKeyAndType(*windowDefinition, "overhang_projection_factor", Json::realValue)
                  || checkKeyAndType(*windowDefinition, "overhang_projection_factor", Json::intValue)) {
                projectionFactor = windowDefinition->get("overhang_projection_factor", 0.0).asDouble();
              }

              // applyViewAndDaylightingGlassRatios does not currently take argument for fins

              Point3dVector viewVertices;
              Point3dVector daylightingVertices;  // not populated
              Point3dVector exteriorShadingVertices;
              Point3dVector interiorShelfVertices;  // not populated

              bool test = applyViewAndDaylightingGlassRatios(wwr, 0.0, sillHeight, 0.0, projectionFactor, 0.0, wallVertices, viewVertices,
                                                             daylightingVertices, exteriorShadingVertices, interiorShelfVertices);
              if (test) {
                if (!viewVertices.empty()) {
                  size_t parentSubSurfaceIndex = allFinalWindowVertices.size();
                  allFinalWindowVertices.push_back(viewVertices);

                  if (!exteriorShadingVertices.empty()) {
                    allFinalShadeVertices.push_back(exteriorShadingVertices);
                    allFinalShadeParentSubSurfaceIndices.push_back(parentSubSurfaceIndex);
                  }
                }
              }
            }
          }

          mappedWindows.insert(windowIdx);
        }
      }
    }

    Point3dVectorVector allFinalDoorVertices;
    std::vector<std::string> allFinalDoorTypes;

    size_t doorN = doorCenterVertices.size();
    OS_ASSERT(doorN == doorDefinitionIds.size());
    for (unsigned doorIdx = 0; doorIdx < doorN; ++doorIdx) {
      if (mappedDoors.find(doorIdx) == mappedDoors.end()) {
        if (getDistancePointToLineSegment(doorCenterVertices[doorIdx], testSegment) < tol) {

          // get door definition

          const Json::Value* doorDefinition = findById(doorDefinitions, doorDefinitionIds[doorIdx]);
          if (doorDefinition) {

            // "Door","Glass Door","Overhead Door"
            std::string doorType = "Door";
            if (checkKeyAndType(*doorDefinition, "door_type", Json::stringValue)) {
              doorType = doorDefinition->get("door_type", doorType).asString();
            }
            if (istringEqual(doorType, "Glass Door")) {
              doorType = "GlassDoor";
            } else if (istringEqual(doorType, "Overhead Door")) {
              doorType = "OverheadDoor";
            }
            allFinalDoorTypes.push_back(doorType);

            assertKeyAndType(*doorDefinition, "height", Json::realValue);
            assertKeyAndType(*doorDefinition, "width", Json::realValue);

            double height = lengthToMeters * doorDefinition->get("height", 0.0).asDouble();
            double width = lengthToMeters * doorDefinition->get("width", 0.0).asDouble();

            Vector3d widthVector = edgeVector;
            widthVector.setLength(0.5 * width);
            Point3d door1 = doorCenterVertices[doorIdx] + widthVector;
            widthVector.setLength(-0.5 * width);
            Point3d doorw2 = doorCenterVertices[doorIdx] + widthVector;

            Point3dVector doorVertices{
              {door1.x(), door1.y(), minZ + height},
              {door1.x(), door1.y(), minZ + 0.0},
              {doorw2.x(), doorw2.y(), minZ + 0.0},
              {doorw2.x(), doorw2.y(), minZ + height},
            };

            // size_t parentSubSurfaceIndex = allFinalDoorVertices.size();
            allFinalDoorVertices.push_back(doorVertices);
          }

          mappedDoors.insert(doorIdx);
        }
      }
    }

    Point3dVectorVector allFinalWallVertices;
    unsigned finalWallFaceFormat = wallFaceFormat;
    if (openstudioFormat) {
      allFinalWallVertices.push_back(wallVertices);
    } else if (allFinalWindowVertices.empty() && allFinalDoorVertices.empty()) {
      allFinalWallVertices.push_back(wallVertices);
    } else {
      finalWallFaceFormat = 0;  // triangle

      Transformation t = Transformation::alignFace(wallVertices);
      //Transformation r = t.rotationMatrix();
      Transformation tInv = t.inverse();
      wallVertices = reverse(tInv * wallVertices);
      // get vertices of all sub surfaces
      Point3dVectorVector wallSubVertices;
      for (const auto& finalWindowVertices : allFinalWindowVertices) {
        wallSubVertices.push_back(reverse(tInv * finalWindowVertices));
      }
      for (const auto& finalDoorVertices : allFinalDoorVertices) {
        wallSubVertices.push_back(reverse(tInv * finalDoorVertices));
      }

      Point3dVectorVector finalWallVertices = computeTriangulation(wallVertices, wallSubVertices, tol);
      for (const auto& finalWallVerts : finalWallVertices) {
        Point3dVector finalVerts = t * finalWallVerts;
        allFinalWallVertices.push_back(reverse(finalVerts));
      }
    }

    std::string parentSurfaceName;
    parentSurfaceName = makeSurface(story, spaceOrShading, "", "", belowFloorPlenum, aboveCeilingPlenum, "Wall", allFinalWallVertices,
                                    finalWallFaceFormat, geometries, sceneChildren, 0, false);

    std::vector<std::string> parentSubSurfaceNames;
    size_t finalWindowN = allFinalWindowVertices.size();
    OS_ASSERT(finalWindowN == allFinalWindowTypes.size());
    for (size_t finalWindowIdx = 0; finalWindowIdx < finalWindowN; ++finalWindowIdx) {
      const auto& finalWindowVertices = allFinalWindowVertices[finalWindowIdx];
      std::string parentSubSurfaceName =
        makeSurface(story, spaceOrShading, parentSurfaceName, "", belowFloorPlenum, aboveCeilingPlenum, allFinalWindowTypes[finalWindowIdx],
                    Point3dVectorVector(1, finalWindowVertices), wallFaceFormat, geometries, sceneChildren, 0, false);
      parentSubSurfaceNames.push_back(parentSubSurfaceName);
    }

    size_t finalDoorN = allFinalDoorVertices.size();
    OS_ASSERT(finalDoorN == allFinalDoorTypes.size());
    for (size_t finalDoorIdx = 0; finalDoorIdx < finalDoorN; ++finalDoorIdx) {
      const auto& finalDoorVertices = allFinalDoorVertices[finalDoorIdx];
      std::string parentSubSurfaceName =
        makeSurface(story, spaceOrShading, parentSurfaceName, "", belowFloorPlenum, aboveCeilingPlenum, allFinalDoorTypes[finalDoorIdx],
                    Point3dVectorVector(1, finalDoorVertices), wallFaceFormat, geometries, sceneChildren, 0, false);
      parentSubSurfaceNames.push_back(parentSubSurfaceName);
    }

    size_t shadeN = allFinalShadeVertices.size();
    OS_ASSERT(shadeN == allFinalShadeParentSubSurfaceIndices.size());
    for (size_t shadeIdx = 0; shadeIdx < shadeN; ++shadeIdx) {
      std::string parentSubSurfaceName = parentSubSurfaceNames[allFinalShadeParentSubSurfaceIndices[shadeIdx]];
      makeSurface(story, spaceOrShading, "", parentSubSurfaceName, belowFloorPlenum, aboveCeilingPlenum, "SpaceShading",
                  Point3dVectorVector(1, allFinalShadeVertices[shadeIdx]), wallFaceFormat, geometries, sceneChildren, 0, false);
    }
  }

  // get daylighting controls
  for (const auto& daylightingControl : spaceOrShading.get("daylighting_controls", Json::arrayValue)) {
    assertKeyAndType(daylightingControl, "vertex_id", Json::stringValue);
    std::string vertexId = daylightingControl.get("vertex_id", "").asString();

    const Json::Value* vertex = findById(vertices, vertexId);
    if (vertex) {
      assertKeyAndType(daylightingControl, "daylighting_control_definition_id", Json::stringValue);
      std::string daylightingControlDefinitionId = daylightingControl.get("daylighting_control_definition_id", "").asString();

      assertKeyAndType(*vertex, "x", Json::realValue);
      assertKeyAndType(*vertex, "y", Json::realValue);

      const Json::Value* daylightingControlDefinition = findById(daylightingControlDefinitions, daylightingControlDefinitionId);
      if (daylightingControlDefinition) {
        assertKey(*daylightingControlDefinition, "height");
        assertKey(*daylightingControlDefinition, "illuminance_setpoint");

        double height = lengthToMeters * daylightingControlDefinition->get("height", 0.0).asDouble();
        double illuminanceSetpoint = daylightingControlDefinition->get("illuminance_setpoint", 0.0).asDouble();  // DLM: TODO put in unit conversion

        Point3dVector dcVertices;
        dcVertices.push_back(
          Point3d(lengthToMeters * vertex->get("x", 0.0).asDouble() - 0.1, lengthToMeters * vertex->get("y", 0.0).asDouble() + 0.1, minZ + height));
        dcVertices.push_back(
          Point3d(lengthToMeters * vertex->get("x", 0.0).asDouble() + 0.1, lengthToMeters * vertex->get("y", 0.0).asDouble() + 0.1, minZ + height));
        dcVertices.push_back(
          Point3d(lengthToMeters * vertex->get("x", 0.0).asDouble() + 0.1, lengthToMeters * vertex->get("y", 0.0).asDouble() - 0.1, minZ + height));
        dcVertices.push_back(
          Point3d(lengthToMeters * vertex->get("x", 0.0).asDouble() - 0.1, lengthToMeters * vertex->get("y", 0.0).asDouble() - 0.1, minZ + height));

        makeSurface(story, spaceOrShading, "", "", belowFloorPlenum, aboveCeilingPlenum, "DaylightingControl", Point3dVectorVector(1, dcVertices),
                    wallFaceFormat, geometries, sceneChildren, illuminanceSetpoint, false);
      }
    }
  }
}

void FloorplanJS::makeMaterial(const Json::Value& object, const std::string& iddObjectType, std::vector<ThreeMaterial>& materials,
                               std::map<std::string, std::string>& materialMap) {
  assertKeyAndType(object, "name", Json::stringValue);
  std::string name = object.get("name", "").asString();

  if (checkKeyAndType(object, "color", Json::stringValue)) {
    std::string color = object.get("color", "").asString();

    std::string materialName = getObjectThreeMaterialName(iddObjectType, name);
    ThreeMaterial material = makeThreeMaterial(materialName, toThreeColor(color), 1, ThreeSide::DoubleSide);
    addThreeMaterial(materials, materialMap, material);
  }
}

ThreeModelObjectMetadata FloorplanJS::makeModelObjectMetadata(const std::string& iddObjectType, const Json::Value& object) {
  std::string handle;
  if (checkKeyAndType(object, "handle", Json::stringValue)) {
    handle = object.get("handle", "").asString();
  }

  std::string name;
  if (checkKeyAndType(object, "name", Json::stringValue)) {
    name = object.get("name", "").asString();
  }

  ThreeModelObjectMetadata result(iddObjectType, handle, name);

  if (checkKeyAndType(object, "color", Json::stringValue)) {
    std::string color = object.get("color", "").asString();
    result.setColor(color);
  }

  return result;
}

ThreeScene FloorplanJS::toThreeScene(bool openstudioFormat) const {
  m_plenumThermalZoneNames.clear();
  m_boundingBox = BoundingBox();

  std::vector<ThreeGeometry> geometries;
  std::vector<ThreeSceneChild> children;
  std::vector<std::string> buildingStoryNames;
  std::vector<ThreeModelObjectMetadata> modelObjectMetadata;

  std::vector<ThreeMaterial> materials;
  std::map<std::string, std::string> materialMap;

  // DLM: always add the standard materials
  // add standard materials if we will be rendering
  //if (!openstudioFormat){
  for (const auto& material : makeStandardThreeMaterials()) {
    addThreeMaterial(materials, materialMap, material);
  }
  //}
  // add model specific materials in loop with makeMaterial

  double currentStoryZ = 0;

  // read project config
  Json::Value project = m_value.get("project", Json::objectValue);
  if (!project.isNull()) {
    // DLM: TODO move this to a function
    Json::Value ground = project.get("ground", Json::objectValue);
    if (!ground.isNull()) {
      if (checkKeyAndType(ground, "floor_offset", Json::realValue)) {
        currentStoryZ = ground.get("floor_offset", 0.0).asDouble();
      }
    }
  }

  // DLM: geometry in ThreeJS output is always in meters in building coordinates
  // north angle is applied directly to osm, does not impact this translation

  double lengthToMeters = 1;
  if (istringEqual(units(), "ip")) {
    lengthToMeters = 0.3048;  // don't use openstudio convert to keep dependencies low
  }

  // are there any plenums in the entire model
  bool anyPlenums = false;

  // loop over stories
  Json::Value stories = m_value.get("stories", Json::arrayValue);
  Json::ArrayIndex storyN = stories.size();
  for (Json::ArrayIndex storyIdx = 0; storyIdx < storyN; ++storyIdx) {

    if (checkKeyAndType(stories[storyIdx], "name", Json::stringValue)) {
      buildingStoryNames.push_back(stories[storyIdx].get("name", "").asString());
    }

    std::string storyColor;
    if (checkKeyAndType(stories[storyIdx], "color", Json::stringValue)) {
      storyColor = stories[storyIdx].get("color", storyColor).asString();
    }

    unsigned storyMultiplier = 1;
    if (checkKeyAndType(stories[storyIdx], "multiplier", Json::uintValue)) {
      storyMultiplier = stories[storyIdx].get("multiplier", storyMultiplier).asUInt();
    }

    double storyBelowFloorPlenumHeight = 0;
    if (checkKeyAndType(stories[storyIdx], "below_floor_plenum_height", Json::realValue)) {
      storyBelowFloorPlenumHeight = lengthToMeters * stories[storyIdx].get("below_floor_plenum_height", storyBelowFloorPlenumHeight).asDouble();
    }

    double storyFloorToCeilingHeight = 3;  // default is 3 m
    if (checkKeyAndType(stories[storyIdx], "floor_to_ceiling_height", Json::realValue)) {
      storyFloorToCeilingHeight = lengthToMeters * stories[storyIdx].get("floor_to_ceiling_height", storyFloorToCeilingHeight).asDouble();
    }

    double storyAboveCeilingPlenumHeight = 0;
    if (checkKeyAndType(stories[storyIdx], "above_ceiling_plenum_height", Json::realValue)) {
      storyAboveCeilingPlenumHeight = lengthToMeters * stories[storyIdx].get("above_ceiling_plenum_height", storyAboveCeilingPlenumHeight).asDouble();
    }

    // DLM: temp code
    if (storyFloorToCeilingHeight < 0.1) {
      //belowFloorPlenumHeight = 1;
      storyFloorToCeilingHeight = 3;
      //aboveCeilingPlenumHeight = 1;
    }
    if (storyMultiplier == 0) {
      storyMultiplier = 1;
    } else if (storyMultiplier > 1) {
      //LOG(Warn, "Multiplier translation not yet implemented");
      //storyMultiplier = 1;
    }

    // get story properties
    ThreeModelObjectMetadata storyMetadata = makeModelObjectMetadata("OS:BuildingStory", stories[storyIdx]);
    storyMetadata.setColor(storyColor);
    storyMetadata.setMultiplier(storyMultiplier);
    storyMetadata.setNominalZCoordinate(currentStoryZ);
    storyMetadata.setBelowFloorPlenumHeight(storyBelowFloorPlenumHeight);
    storyMetadata.setFloorToCeilingHeight(storyFloorToCeilingHeight);
    storyMetadata.setAboveCeilingPlenumHeight(storyAboveCeilingPlenumHeight);
    modelObjectMetadata.push_back(storyMetadata);

    // make story material
    makeMaterial(stories[storyIdx], "OS:BuildingStory", materials, materialMap);

    // increment currentStoryZ, will be zero for multiplier == 1
    // DLM: TODO need to get the intersection and matching code in utilities, move stories after intersecting and matching
    //currentStoryZ += 0.5*(storyMultiplier - 1)*(storyBelowFloorPlenumHeight + storyFloorToCeilingHeight + storyAboveCeilingPlenumHeight);

    // get the geometry
    assertKeyAndType(stories[storyIdx], "geometry", Json::objectValue);
    Json::Value geometry = stories[storyIdx].get("geometry", Json::arrayValue);
    Json::Value vertices = geometry.get("vertices", Json::arrayValue);
    Json::Value edges = geometry.get("edges", Json::arrayValue);
    Json::Value faces = geometry.get("faces", Json::arrayValue);

    // loop over spaces
    Json::Value spaces = stories[storyIdx].get("spaces", Json::arrayValue);
    Json::ArrayIndex spaceN = spaces.size();
    for (Json::ArrayIndex spaceIdx = 0; spaceIdx < spaceN; ++spaceIdx) {

      // each space should have one face
      if (checkKeyAndType(spaces[spaceIdx], "face_id", Json::stringValue)) {
        std::string faceId = spaces[spaceIdx].get("face_id", "").asString();

        assertKeyAndType(spaces[spaceIdx], "name", Json::stringValue);
        std::string spaceName = spaces[spaceIdx].get("name", "").asString();

        // translate space properties, default to story values
        unsigned spaceMultiplier = storyMultiplier;
        if (checkKeyAndType(spaces[spaceIdx], "multiplier", Json::uintValue)) {
          spaceMultiplier = spaces[spaceIdx].get("multiplier", spaceMultiplier).asUInt();
        }
        if (spaceMultiplier == 0) {
          spaceMultiplier = 1;
        } else if (spaceMultiplier > 1) {
          //LOG(Warn, "Multiplier translation not yet implemented");
          //spaceMultiplier = 1;
        }

        double spaceBelowFloorPlenumHeight = storyBelowFloorPlenumHeight;
        if (checkKeyAndType(spaces[spaceIdx], "below_floor_plenum_height", Json::realValue)) {
          spaceBelowFloorPlenumHeight = lengthToMeters * spaces[spaceIdx].get("below_floor_plenum_height", spaceBelowFloorPlenumHeight).asDouble();
        }

        double spaceFloorToCeilingHeight = storyFloorToCeilingHeight;
        if (checkKeyAndType(spaces[spaceIdx], "floor_to_ceiling_height", Json::realValue)) {
          spaceFloorToCeilingHeight = lengthToMeters * spaces[spaceIdx].get("floor_to_ceiling_height", spaceFloorToCeilingHeight).asDouble();
        }

        double spaceAboveCeilingPlenumHeight = storyAboveCeilingPlenumHeight;
        if (checkKeyAndType(spaces[spaceIdx], "above_ceiling_plenum_height", Json::realValue)) {
          spaceAboveCeilingPlenumHeight =
            lengthToMeters * spaces[spaceIdx].get("above_ceiling_plenum_height", spaceAboveCeilingPlenumHeight).asDouble();
        }

        double spaceFloorOffset = 0;
        if (checkKeyAndType(spaces[spaceIdx], "floor_offset", Json::realValue)) {
          spaceFloorOffset = lengthToMeters * spaces[spaceIdx].get("floor_offset", spaceFloorOffset).asDouble();
        }

        [[maybe_unused]] bool openToBelow = false;
        if (checkKeyAndType(spaces[spaceIdx], "open_to_below", Json::booleanValue)) {
          openToBelow = spaces[spaceIdx].get("open_to_below", openToBelow).asBool();
        }

        // create geometry
        double minZ = currentStoryZ + spaceFloorOffset;
        double maxZ = currentStoryZ + spaceFloorOffset + spaceBelowFloorPlenumHeight;
        if (spaceBelowFloorPlenumHeight > 0) {
          anyPlenums = true;
          ThreeModelObjectMetadata spaceMetadata("OS:Space", "", spaceName + BELOWFLOORPLENUMPOSTFIX);
          spaceMetadata.setMultiplier(spaceMultiplier);
          spaceMetadata.setOpenToBelow(openToBelow);
          modelObjectMetadata.push_back(spaceMetadata);
          makeGeometries(stories[storyIdx], spaces[spaceIdx], true, false, lengthToMeters, minZ, maxZ, vertices, edges, faces, faceId,
                         openstudioFormat, geometries, children, openToBelow);
          openToBelow = false;  // no longer open
        }

        minZ = maxZ;
        maxZ += spaceFloorToCeilingHeight;
        if (spaceFloorToCeilingHeight > 0) {
          ThreeModelObjectMetadata spaceMetadata = makeModelObjectMetadata("OS:Space", spaces[spaceIdx]);
          spaceMetadata.setMultiplier(spaceMultiplier);
          spaceMetadata.setOpenToBelow(openToBelow);
          modelObjectMetadata.push_back(spaceMetadata);
          makeGeometries(stories[storyIdx], spaces[spaceIdx], false, false, lengthToMeters, minZ, maxZ, vertices, edges, faces, faceId,
                         openstudioFormat, geometries, children, openToBelow);
          openToBelow = false;  // no longer open
        }

        minZ = maxZ;
        maxZ += spaceAboveCeilingPlenumHeight;
        if (spaceAboveCeilingPlenumHeight > 0) {
          anyPlenums = true;
          ThreeModelObjectMetadata spaceMetadata("OS:Space", "", spaceName + ABOVECEILINGPLENUMPOSTFIX);
          spaceMetadata.setMultiplier(spaceMultiplier);
          spaceMetadata.setOpenToBelow(openToBelow);
          modelObjectMetadata.push_back(spaceMetadata);
          makeGeometries(stories[storyIdx], spaces[spaceIdx], false, true, lengthToMeters, minZ, maxZ, vertices, edges, faces, faceId,
                         openstudioFormat, geometries, children, openToBelow);
          openToBelow = false;  // no longer open
        }
      }

    }  // spaces

    // loop over shading
    Json::Value shading = stories[storyIdx].get("shading", Json::arrayValue);
    Json::ArrayIndex shadingN = shading.size();
    for (Json::ArrayIndex shadingdx = 0; shadingdx < shadingN; ++shadingdx) {

      // each shading should have one face
      if (checkKeyAndType(shading[shadingdx], "face_id", Json::stringValue)) {
        std::string faceId = shading[shadingdx].get("face_id", "").asString();

        assertKeyAndType(shading[shadingdx], "name", Json::stringValue);
        std::string shadingName = shading[shadingdx].get("name", "").asString();

        // translate shading properties, default to story values
        unsigned shadingMultiplier = storyMultiplier;
        if (checkKeyAndType(shading[shadingdx], "multiplier", Json::uintValue)) {
          shadingMultiplier = shading[shadingdx].get("multiplier", shadingMultiplier).asUInt();
        }
        if (shadingMultiplier == 0) {
          shadingMultiplier = 1;
        } else if (shadingMultiplier > 1) {
          //LOG(Warn, "Multiplier translation not yet implemented");
          //shadingMultiplier = 1;
        }

        double shadingBelowFloorPlenumHeight = storyBelowFloorPlenumHeight;
        if (checkKeyAndType(shading[shadingdx], "below_floor_plenum_height", Json::realValue)) {
          shadingBelowFloorPlenumHeight =
            lengthToMeters * shading[shadingdx].get("below_floor_plenum_height", shadingBelowFloorPlenumHeight).asDouble();
        }

        double shadingFloorToCeilingHeight = storyFloorToCeilingHeight;
        if (checkKeyAndType(shading[shadingdx], "floor_to_ceiling_height", Json::realValue)) {
          shadingFloorToCeilingHeight = lengthToMeters * shading[shadingdx].get("floor_to_ceiling_height", shadingFloorToCeilingHeight).asDouble();
        }

        double shadingAboveCeilingPlenumHeight = storyAboveCeilingPlenumHeight;
        if (checkKeyAndType(shading[shadingdx], "above_ceiling_plenum_height", Json::realValue)) {
          shadingAboveCeilingPlenumHeight =
            lengthToMeters * shading[shadingdx].get("above_ceiling_plenum_height", shadingAboveCeilingPlenumHeight).asDouble();
        }

        double shadingFloorOffset = 0;
        if (checkKeyAndType(shading[shadingdx], "floor_offset", Json::realValue)) {
          shadingFloorOffset = lengthToMeters * shading[shadingdx].get("floor_offset", shadingFloorOffset).asDouble();
        }

        [[maybe_unused]] bool openToBelow = false;
        if (checkKeyAndType(shading[shadingdx], "open_to_below", Json::booleanValue)) {
          openToBelow = shading[shadingdx].get("open_to_below", openToBelow).asBool();
        }

        // create geometry
        double minZ = currentStoryZ + shadingFloorOffset;
        double maxZ = currentStoryZ + shadingFloorOffset + shadingBelowFloorPlenumHeight;
        if (shadingBelowFloorPlenumHeight > 0) {
          anyPlenums = true;
          ThreeModelObjectMetadata shadingMetadata("OS:ShadingSurfaceGroup", "", shadingName + BELOWFLOORPLENUMPOSTFIX);
          shadingMetadata.setMultiplier(shadingMultiplier);
          shadingMetadata.setOpenToBelow(openToBelow);
          modelObjectMetadata.push_back(shadingMetadata);
          makeGeometries(stories[storyIdx], shading[shadingdx], true, false, lengthToMeters, minZ, maxZ, vertices, edges, faces, faceId,
                         openstudioFormat, geometries, children, openToBelow);
          openToBelow = false;  // no longer open
        }

        minZ = maxZ;
        maxZ += shadingFloorToCeilingHeight;
        if (shadingFloorToCeilingHeight > 0) {
          ThreeModelObjectMetadata shadingMetadata = makeModelObjectMetadata("OS:ShadingSurfaceGroup", shading[shadingdx]);
          shadingMetadata.setMultiplier(shadingMultiplier);
          shadingMetadata.setOpenToBelow(openToBelow);
          modelObjectMetadata.push_back(shadingMetadata);
          makeGeometries(stories[storyIdx], shading[shadingdx], false, false, lengthToMeters, minZ, maxZ, vertices, edges, faces, faceId,
                         openstudioFormat, geometries, children, openToBelow);
          openToBelow = false;  // no longer open
        }

        minZ = maxZ;
        maxZ += shadingAboveCeilingPlenumHeight;
        if (shadingAboveCeilingPlenumHeight > 0) {
          anyPlenums = true;
          ThreeModelObjectMetadata shadingMetadata("OS:ShadingSurfaceGroup", "", shadingName + ABOVECEILINGPLENUMPOSTFIX);
          shadingMetadata.setMultiplier(shadingMultiplier);
          shadingMetadata.setOpenToBelow(openToBelow);
          modelObjectMetadata.push_back(shadingMetadata);
          makeGeometries(stories[storyIdx], shading[shadingdx], false, true, lengthToMeters, minZ, maxZ, vertices, edges, faces, faceId,
                         openstudioFormat, geometries, children, openToBelow);
          openToBelow = false;  // no longer open
        }
      }

    }  // shading

    // increment height for next story, will be (storyBelowFloorPlenumHeight + storyFloorToCeilingHeight + storyAboveCeilingPlenumHeight) for multiplier == 1
    // DLM: TODO need to get the intersection and matching code in utilities, move stories after intersecting and matching
    //currentStoryZ += 0.5*(storyMultiplier + 1)*(storyBelowFloorPlenumHeight + storyFloorToCeilingHeight + storyAboveCeilingPlenumHeight);
    currentStoryZ += storyBelowFloorPlenumHeight + storyFloorToCeilingHeight + storyAboveCeilingPlenumHeight;

  }  // stories

  // loop over building_units
  Json::Value buildingUnits = m_value.get("building_units", Json::arrayValue);
  Json::ArrayIndex n = buildingUnits.size();
  for (Json::ArrayIndex i = 0; i < n; ++i) {
    modelObjectMetadata.push_back(makeModelObjectMetadata("OS:BuildingUnit", buildingUnits[i]));
    makeMaterial(buildingUnits[i], "OS:BuildingUnit", materials, materialMap);
  }

  // loop over thermal_zones
  Json::Value thermalZones = m_value.get("thermal_zones", Json::arrayValue);
  n = thermalZones.size();
  for (Json::ArrayIndex i = 0; i < n; ++i) {
    modelObjectMetadata.push_back(makeModelObjectMetadata("OS:ThermalZone", thermalZones[i]));
    makeMaterial(thermalZones[i], "OS:ThermalZone", materials, materialMap);
  }

  // DLM: how will we merge this plenum zone with existing plenum zones?
  for (const auto& thermalZoneName : m_plenumThermalZoneNames) {
    ThreeModelObjectMetadata thermalZoneMetadata("OS:ThermalZone", "", thermalZoneName);
    modelObjectMetadata.push_back(thermalZoneMetadata);
    thermalZoneMetadata.setColor(PLENUMCOLOR);  // already made in makeStandardThreeMaterials
  }

  // loop over space_types
  Json::Value spaceTypes = m_value.get("space_types", Json::arrayValue);
  n = spaceTypes.size();
  for (Json::ArrayIndex i = 0; i < n; ++i) {
    modelObjectMetadata.push_back(makeModelObjectMetadata("OS:SpaceType", spaceTypes[i]));
    makeMaterial(spaceTypes[i], "OS:SpaceType", materials, materialMap);
  }
  if (anyPlenums) {
    ThreeModelObjectMetadata spaceTypeMetadata("OS:SpaceType", "", PLENUMSPACETYPENAME);
    modelObjectMetadata.push_back(spaceTypeMetadata);
    spaceTypeMetadata.setColor(PLENUMCOLOR);  // already made in makeStandardThreeMaterials
  }

  // loop over construction_sets
  Json::Value constructionSets = m_value.get("construction_sets", Json::arrayValue);
  n = constructionSets.size();
  for (Json::ArrayIndex i = 0; i < n; ++i) {
    modelObjectMetadata.push_back(makeModelObjectMetadata("OS:DefaultConstructionSet", constructionSets[i]));
    //makeMaterial(constructionSets[i], "OS:DefaultConstructionSet", materials, materialMap);
  }

  m_boundingBox.addPoint(Point3d(0, 0, 0));

  double lookAtX = 0;  // (boundingBox.minX().get() + boundingBox.maxX().get()) / 2.0
  double lookAtY = 0;  // (boundingBox.minY().get() + boundingBox.maxY().get()) / 2.0
  double lookAtZ = 0;  // (boundingBox.minZ().get() + boundingBox.maxZ().get()) / 2.0
  double lookAtR = sqrt(std::pow(m_boundingBox.maxX().get() / 2.0, 2) + std::pow(m_boundingBox.maxY().get() / 2.0, 2)
                        + std::pow(m_boundingBox.maxZ().get() / 2.0, 2));
  lookAtR = std::max(lookAtR, sqrt(std::pow(m_boundingBox.minX().get() / 2.0, 2) + std::pow(m_boundingBox.maxY().get() / 2.0, 2)
                                   + std::pow(m_boundingBox.maxZ().get() / 2.0, 2)));
  lookAtR = std::max(lookAtR, sqrt(std::pow(m_boundingBox.maxX().get() / 2.0, 2) + std::pow(m_boundingBox.minY().get() / 2.0, 2)
                                   + std::pow(m_boundingBox.maxZ().get() / 2.0, 2)));
  lookAtR = std::max(lookAtR, sqrt(std::pow(m_boundingBox.maxX().get() / 2.0, 2) + std::pow(m_boundingBox.maxY().get() / 2.0, 2)
                                   + std::pow(m_boundingBox.minZ().get() / 2.0, 2)));
  lookAtR = std::max(lookAtR, sqrt(std::pow(m_boundingBox.minX().get() / 2.0, 2) + std::pow(m_boundingBox.minY().get() / 2.0, 2)
                                   + std::pow(m_boundingBox.maxZ().get() / 2.0, 2)));
  lookAtR = std::max(lookAtR, sqrt(std::pow(m_boundingBox.minX().get() / 2.0, 2) + std::pow(m_boundingBox.maxY().get() / 2.0, 2)
                                   + std::pow(m_boundingBox.minZ().get() / 2.0, 2)));
  lookAtR = std::max(lookAtR, sqrt(std::pow(m_boundingBox.maxX().get() / 2.0, 2) + std::pow(m_boundingBox.minY().get() / 2.0, 2)
                                   + std::pow(m_boundingBox.minZ().get() / 2.0, 2)));
  lookAtR = std::max(lookAtR, sqrt(std::pow(m_boundingBox.minX().get() / 2.0, 2) + std::pow(m_boundingBox.minY().get() / 2.0, 2)
                                   + std::pow(m_boundingBox.minZ().get() / 2.0, 2)));

  ThreeBoundingBox boundingBox(m_boundingBox.minX().get(), m_boundingBox.minY().get(), m_boundingBox.minZ().get(), m_boundingBox.maxX().get(),
                               m_boundingBox.maxY().get(), m_boundingBox.maxZ().get(), lookAtX, lookAtY, lookAtZ, lookAtR);

  ThreeSceneMetadata metadata(buildingStoryNames, boundingBox, northAxis(), modelObjectMetadata);

  ThreeSceneObject sceneObject("", children);

  ThreeScene result(metadata, geometries, materials, sceneObject);

  return result;
}

std::string FloorplanJS::units() const {
  std::string units = "ip";
  Json::Value project = m_value.get("project", Json::objectValue);
  if (!project.isNull()) {
    Json::Value config = project.get("config", Json::objectValue);
    if (!config.isNull()) {
      units = config.get("units", units).asString();
    }
  }
  if (istringEqual(units, "ft")) {
    units = "ip";
  }
  return units;
}

bool FloorplanJS::setUnits(const std::string& units) {
  if (!(istringEqual(units, "ip") || istringEqual(units, "si"))) {
    return false;
  }

  if (!checkKeyAndType(m_value, "project", Json::objectValue)) {
    m_value["project"] = Json::Value(Json::objectValue);
  }
  Json::Value& project = m_value["project"];

  if (!checkKeyAndType(project, "config", Json::objectValue)) {
    project["config"] = Json::Value(Json::objectValue);
  }
  Json::Value& config = project["config"];

  config["units"] = units;

  return true;
}

void FloorplanJS::resetUnits() {
  setUnits("ip");
}

double FloorplanJS::northAxis() const {
  double result = 0;
  Json::Value project = m_value.get("project", Json::objectValue);
  if (!project.isNull()) {
    if (project.isMember("north_axis")) {
      // current location
      result = project.get("north_axis", result).asDouble();
    } else {
      // previous location
      Json::Value config = project.get("config", Json::objectValue);
      if (!config.isNull()) {
        result = config.get("north_axis", result).asDouble();
      }
    }
  }
  return result;
}

bool FloorplanJS::setNorthAxis(double northAxis) {
  if (!checkKeyAndType(m_value, "project", Json::objectValue)) {
    m_value["project"] = Json::Value(Json::objectValue);
  }
  Json::Value& project = m_value["project"];

  project["north_axis"] = northAxis;

  if (!checkKeyAndType(project, "map", Json::objectValue)) {
    project["map"] = Json::Value(Json::objectValue);
  }
  Json::Value& map = project["map"];

  map["rotation"] = degToRad(northAxis);

  return true;
}

void FloorplanJS::resetNorthAxis() {
  setNorthAxis(0);
}

double FloorplanJS::latitude() const {
  double result = 0;
  Json::Value project = m_value.get("project", Json::objectValue);
  if (!project.isNull()) {
    Json::Value map = project.get("map", Json::objectValue);
    if (!map.isNull()) {
      result = map.get("latitude", result).asDouble();
    }
  }
  return result;
}

bool FloorplanJS::setLatitude(double latitude) {
  if (!checkKeyAndType(m_value, "project", Json::objectValue)) {
    m_value["project"] = Json::Value(Json::objectValue);
  }
  Json::Value& project = m_value["project"];

  if (!checkKeyAndType(project, "map", Json::objectValue)) {
    project["map"] = Json::Value(Json::objectValue);
  }
  Json::Value& map = project["map"];

  map["latitude"] = latitude;

  return true;
}

void FloorplanJS::resetLatitude() {
  setLatitude(0);
}

double FloorplanJS::longitude() const {
  double result = 0;
  Json::Value project = m_value.get("project", Json::objectValue);
  if (!project.isNull()) {
    Json::Value map = project.get("map", Json::objectValue);
    if (!map.isNull()) {
      result = map.get("longitude", result).asDouble();
    }
  }
  return result;
}

bool FloorplanJS::setLongitude(double longitude) {
  if (!checkKeyAndType(m_value, "project", Json::objectValue)) {
    m_value["project"] = Json::Value(Json::objectValue);
  }
  Json::Value& project = m_value["project"];

  if (!checkKeyAndType(project, "map", Json::objectValue)) {
    project["map"] = Json::Value(Json::objectValue);
  }
  Json::Value& map = project["map"];

  map["longitude"] = longitude;

  return true;
}

void FloorplanJS::resetLongitude() {
  setLongitude(0);
}

double FloorplanJS::elevation() const {
  double result = 0;
  Json::Value project = m_value.get("project", Json::objectValue);
  if (!project.isNull()) {
    Json::Value map = project.get("map", Json::objectValue);
    if (!map.isNull()) {
      result = map.get("elevation", result).asDouble();
    }
  }
  return result;
}

bool FloorplanJS::setElevation(double elevation) {
  if (!checkKeyAndType(m_value, "project", Json::objectValue)) {
    m_value["project"] = Json::Value(Json::objectValue);
  }
  Json::Value& project = m_value["project"];

  if (!checkKeyAndType(project, "map", Json::objectValue)) {
    project["map"] = Json::Value(Json::objectValue);
  }
  Json::Value& map = project["map"];

  map["elevation"] = elevation;

  return true;
}

void FloorplanJS::resetElevation() {
  setElevation(0);
}

void FloorplanJS::updateStories(const std::vector<FloorplanObject>& objects, bool removeMissingObjects) {
  updateObjects(m_value, "stories", objects, removeMissingObjects);
}

void FloorplanJS::updateSpaces(const std::vector<FloorplanObject>& objects, bool removeMissingObjects) {
  std::map<std::string, std::vector<FloorplanObject>> storyHandleToSpaceObejctIds;

  for (const auto& object : objects) {
    boost::optional<std::string> parentHandleString = object.parentHandleString();

    if (!parentHandleString) {
      continue;
    }

    if (storyHandleToSpaceObejctIds.find(*parentHandleString) == storyHandleToSpaceObejctIds.end()) {
      // cppcheck-suppress stlFindInsert
      storyHandleToSpaceObejctIds[*parentHandleString] = std::vector<FloorplanObject>();
    }
    storyHandleToSpaceObejctIds[*parentHandleString].push_back(object);
  }

  for (const auto& keyValue : storyHandleToSpaceObejctIds) {
    // no need to check by name, assume stories have been updated
    Json::Value* story = findByHandleString(m_value, "stories", keyValue.first);
    if (story) {
      updateObjects(*story, "spaces", keyValue.second, removeMissingObjects);
    }
  }
}

void FloorplanJS::updateBuildingUnits(const std::vector<FloorplanObject>& objects, bool removeMissingObjects) {
  updateObjects(m_value, "building_units", objects, removeMissingObjects);
}

void FloorplanJS::updateThermalZones(const std::vector<FloorplanObject>& objects, bool removeMissingObjects) {
  updateObjects(m_value, "thermal_zones", objects, removeMissingObjects);
}

void FloorplanJS::updateSpaceTypes(const std::vector<FloorplanObject>& objects, bool removeMissingObjects) {
  updateObjects(m_value, "space_types", objects, removeMissingObjects);
}

void FloorplanJS::updateConstructionSets(const std::vector<FloorplanObject>& objects, bool removeMissingObjects) {
  updateObjects(m_value, "construction_sets", objects, removeMissingObjects);
}

std::string FloorplanJS::getHandleString(const Json::Value& value) {
  return value.get("handle", "").asString();
}

std::string FloorplanJS::getName(const Json::Value& value) {
  return value.get("name", "").asString();
}

std::string FloorplanJS::getId(const Json::Value& value) {
  return value.get("id", "").asString();
}

std::string FloorplanJS::getFaceId(const Json::Value& value) {
  return value.get("face_id", "").asString();
}

std::string FloorplanJS::getNextId() {
  ++m_lastId;
  return std::to_string(m_lastId);
}

void FloorplanJS::setLastId(const Json::Value& value) {
  if (value.isObject()) {
    for (const auto& key : value.getMemberNames()) {
      const auto& value2 = value[key];
      if (value2.isArray()) {
        Json::ArrayIndex n = value2.size();
        for (Json::ArrayIndex i = 0; i < n; ++i) {
          setLastId(value2[i]);
        }
      } else if (value2.isObject()) {
        setLastId(value2);
      } else if (key == "id") {
        if (value2.isString()) {
          std::string s = value2.asString();
          unsigned id = strtoul(s.c_str(), nullptr, 0);
          //if (id > 100){
          //// DLM: TODO test code, remove
          //bool test= false;
          //}
          m_lastId = std::max(m_lastId, id);
        } else if (value2.isConvertibleTo(Json::ValueType::uintValue)) {
          unsigned id = value2.asUInt();
          //if (id > 100){
          //// DLM: TODO test code, remove
          //bool test = false;
          //}
          m_lastId = std::max(m_lastId, id);
        }
      }
    }
  }
}

Json::Value* FloorplanJS::findByHandleString(Json::Value& value, const std::string& key, const std::string& handleString) {
  if (handleString.empty()) {
    return nullptr;
  }

  Json::Value& values = value[key];
  Json::ArrayIndex n = values.size();
  for (Json::ArrayIndex i = 0; i < n; ++i) {
    if (getHandleString(values[i]) == handleString) {
      return &values[i];
    }
  }

  return nullptr;
}

Json::Value* FloorplanJS::findByName(Json::Value& value, const std::string& key, const std::string& name, bool requireEmptyHandle) {
  if (name.empty()) {
    return nullptr;
  }

  Json::Value& values = value[key];
  Json::ArrayIndex n = values.size();
  for (Json::ArrayIndex i = 0; i < n; ++i) {
    if (getName(values[i]) == name) {
      // TODO: Both branches do the same!
      // cppcheck-suppress duplicateBranch
      if (requireEmptyHandle && getHandleString(values[i]).empty()) {
        return &values[i];
      } else {
        return &values[i];
      }
    }
  }

  return nullptr;
}

Json::Value* FloorplanJS::findById(Json::Value& value, const std::string& key, const std::string& id) {
  if (id.empty()) {
    return nullptr;
  }

  Json::Value& values = value[key];
  Json::ArrayIndex n = values.size();
  for (Json::ArrayIndex i = 0; i < n; ++i) {
    if (getId(values[i]) == id) {
      return &values[i];
    }
  }

  return nullptr;
}

const Json::Value* FloorplanJS::findById(const Json::Value& values, const std::string& id) const {
  if (id.empty()) {
    return nullptr;
  }

  Json::ArrayIndex n = values.size();
  for (Json::ArrayIndex i = 0; i < n; ++i) {
    if (getId(values[i]) == id) {
      return &values[i];
    }
  }

  return nullptr;
}

void FloorplanJS::updateObjects(Json::Value& value, const std::string& key, const std::vector<FloorplanObject>& objects, bool removeMissingObjects) {
  // ensure key exists
  if (!value.isMember(key)) {
    value[key] = Json::Value(Json::arrayValue);
  }

  // remove all objects that aren't found by handle or name
  if (removeMissingObjects) {
    std::set<std::string> ids;
    std::set<std::string> names;
    std::set<std::string> handleStrings;
    for (const auto& object : objects) {
      ids.insert(object.id());
      names.insert(object.name());
      handleStrings.insert(object.handleString());
    }

    std::vector<Json::ArrayIndex> indicesToRemove;
    std::set<std::string> faceIdsToRemove;
    Json::Value& values = value[key];
    Json::ArrayIndex n = values.size();
    for (Json::ArrayIndex i = 0; i < n; ++i) {
      std::string handle = getHandleString(values[i]);
      // empty handle indicates that object has not been synched with the osm yet
      if (!handle.empty() && (handleStrings.find(handle) == handleStrings.end())) {

        // no object in floorplan should have an empty name
        if (names.find(getName(values[i])) == names.end()) {
          indicesToRemove.push_back(i);
          std::string faceId = getFaceId(values[i]);
          if (!faceId.empty()) {
            faceIdsToRemove.insert(faceId);
          }
        }
      }
    }
    std::reverse(indicesToRemove.begin(), indicesToRemove.end());

    for (const auto& i : indicesToRemove) {
      Json::Value removed;
      values.removeIndex(i, &removed);
    }

    if (!faceIdsToRemove.empty()) {
      if (checkKeyAndType(value, "geometry", Json::objectValue)) {
        removeFaces(value["geometry"], faceIdsToRemove);
      }
    }
  }

  // now update names and data
  for (const auto& object : objects) {

    Json::Value* v = findByHandleString(value, key, object.handleString());
    if (v) {
      // ensure name is the same
      (*v)["name"] = object.name();
    } else {
      // find object by name only if handle is empty
      v = findByName(value, key, object.name(), true);

      if (v) {
        // set handle
        (*v)["handle"] = object.handleString();
      } else {
        // create new object
        Json::Value newObject(Json::objectValue);
        newObject["id"] = getNextId();
        newObject["name"] = object.name();
        newObject["handle"] = object.handleString();
        v = &(value[key].append(newObject));
      }
    }

    if (v) {
      // update properties
      Json::Value data = object.data();
      for (const auto& k : data.getMemberNames()) {
        (*v)[k] = data[k];
      }

      // update references
      for (const auto& p : object.objectReferenceMap()) {
        updateObjectReference(*v, p.first, p.second, removeMissingObjects);
      }
    }
  }
}

void FloorplanJS::updateObjectReference(Json::Value& value, const std::string& key, const FloorplanObject& objectReference,
                                        bool /*removeMissingObjects*/) {
  std::string searchKey;
  if (key == "thermal_zone_id") {
    searchKey = "thermal_zones";
  } else if (key == "space_type_id") {
    searchKey = "space_types";
  } else if (key == "building_unit_id") {
    searchKey = "building_units";
  } else if (key == "construction_set_id") {
    searchKey = "construction_sets";
  }

  if (searchKey.empty()) {
    LOG(Error, "Could not find objects to search for key '" << key << "'");
    return;
  }

  Json::Value* v = findByHandleString(m_value, searchKey, objectReference.handleString());
  if (v) {
    value[key] = v->get("id", "").asString();
    return;
  }

  v = findById(m_value, searchKey, objectReference.id());
  if (v) {
    value[key] = v->get("id", "").asString();
    return;
  }

  v = findByName(m_value, searchKey, objectReference.name(), true);
  if (v) {
    value[key] = v->get("id", "").asString();
    return;
  }

  value[key] = "";
}

void FloorplanJS::removeFaces(Json::Value& value, const std::set<std::string>& faceIdsToRemove) {
  if (!checkKeyAndType(value, "faces", Json::arrayValue)) {
    return;
  }

  std::map<std::string, unsigned> edgeRefCount;
  std::set<std::string> edgeIdsToRemove;

  std::vector<Json::ArrayIndex> indicesToRemove;
  Json::Value& faces = value["faces"];
  Json::ArrayIndex n = faces.size();
  for (Json::ArrayIndex i = 0; i < n; ++i) {
    bool removeFace = false;
    if (faceIdsToRemove.find(getId(faces[i])) != faceIdsToRemove.end()) {
      removeFace = true;
      indicesToRemove.push_back(i);
    }
    if (checkKeyAndType(faces[i], "edge_ids", Json::arrayValue)) {
      Json::Value& edgeIds = faces[i]["edge_ids"];
      Json::ArrayIndex n2 = edgeIds.size();
      for (Json::ArrayIndex i2 = 0; i2 < n2; ++i2) {
        std::string edgeId = edgeIds[i2].asString();

        if (removeFace) {
          edgeIdsToRemove.insert(edgeId);
        }

        if (edgeRefCount.find(edgeId) == edgeRefCount.end()) {
          edgeRefCount[edgeId] = 1;
        } else {
          edgeRefCount[edgeId] = edgeRefCount[edgeId] + 1;
        }
      }
    }
  }
  std::reverse(indicesToRemove.begin(), indicesToRemove.end());

  // remove the faces
  for (const auto& i : indicesToRemove) {
    Json::Value removed;
    faces.removeIndex(i, &removed);
  }

  // don't remove edges with ref count > 1
  for (const auto& pair : edgeRefCount) {
    if (pair.second > 1) {
      edgeIdsToRemove.erase(pair.first);
    }
  }
  removeEdges(value, edgeIdsToRemove);
}

void FloorplanJS::removeEdges(Json::Value& value, const std::set<std::string>& edgeIdsToRemove) {
  if (!checkKeyAndType(value, "edges", Json::arrayValue)) {
    return;
  }

  std::map<std::string, unsigned> vertexRefCount;
  std::set<std::string> vertexIdsToRemove;

  std::vector<Json::ArrayIndex> indicesToRemove;
  Json::Value& edges = value["edges"];
  Json::ArrayIndex n = edges.size();
  for (Json::ArrayIndex i = 0; i < n; ++i) {
    bool removeEdge = false;
    if (edgeIdsToRemove.find(getId(edges[i])) != edgeIdsToRemove.end()) {
      removeEdge = true;
      indicesToRemove.push_back(i);
    }
    if (checkKeyAndType(edges[i], "vertex_ids", Json::arrayValue)) {
      Json::Value& vertexIds = edges[i]["vertex_ids"];
      Json::ArrayIndex n2 = vertexIds.size();
      for (Json::ArrayIndex i2 = 0; i2 < n2; ++i2) {
        std::string vertexId = vertexIds[i2].asString();

        if (removeEdge) {
          vertexIdsToRemove.insert(vertexId);
        }

        if (vertexRefCount.find(vertexId) == vertexRefCount.end()) {
          vertexRefCount[vertexId] = 1;
        } else {
          vertexRefCount[vertexId] = vertexRefCount[vertexId] + 1;
        }
      }
    }
  }
  std::reverse(indicesToRemove.begin(), indicesToRemove.end());

  // remove the edges
  for (const auto& i : indicesToRemove) {
    Json::Value removed;
    edges.removeIndex(i, &removed);
  }

  // don't remove vertices with ref count > 1
  for (const auto& pair : vertexRefCount) {
    if (pair.second > 2) {
      vertexIdsToRemove.erase(pair.first);
    }
  }
  removeVertices(value, vertexIdsToRemove);
}

// cppcheck-suppress functionConst
void FloorplanJS::removeVertices(Json::Value& value, const std::set<std::string>& vertexIdsToRemove) {
  if (!checkKeyAndType(value, "vertices", Json::arrayValue)) {
    return;
  }

  std::vector<Json::ArrayIndex> indicesToRemove;
  Json::Value& vertices = value["vertices"];
  Json::ArrayIndex n = vertices.size();
  for (Json::ArrayIndex i = 0; i < n; ++i) {
    if (vertexIdsToRemove.find(getId(vertices[i])) != vertexIdsToRemove.end()) {
      indicesToRemove.push_back(i);
    }
  }
  std::reverse(indicesToRemove.begin(), indicesToRemove.end());

  // remove the vertices
  for (const auto& i : indicesToRemove) {
    Json::Value removed;
    vertices.removeIndex(i, &removed);
  }
}

std::string floorplanToThreeJS(const std::string& json, bool openstudioFormat) {
  boost::optional<FloorplanJS> floorplan = FloorplanJS::load(json);
  if (floorplan) {
    ThreeScene scene = floorplan->toThreeScene(openstudioFormat);
    return scene.toJSON(false);
  }

  return "{}";
}

}  // namespace openstudio
