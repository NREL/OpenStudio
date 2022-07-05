/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "FSModel.hpp"

#include "../core/Assert.hpp"
#include "../core/Json.hpp"
#include "../geometry/Point3d.hpp"
#include "../geometry/Vector3d.hpp"
#include "../geometry/Geometry.hpp"
#include "../geometry/Intersection.hpp"

#include <iomanip>

namespace openstudio {

FSModel::FSModel() {
  m_lengthToMeters = 1;
}

FSModel::FSModel(const std::string& json) {
  load(json);
}

void FSModel::load(const std::string& json) {
  Json::CharReaderBuilder rbuilder;
  std::istringstream ss(json);
  std::string formattedErrors;
  bool parsingSuccessful = Json::parseFromStream(rbuilder, ss, &m_value, &formattedErrors);

  if (!parsingSuccessful) {

    // see if this is a path
    openstudio::path p = toPath(json);
    if (boost::filesystem::exists(p) && boost::filesystem::is_regular_file(p)) {
      // open file
      std::ifstream ifs(openstudio::toSystemFilename(p));
      m_value.clear();
      formattedErrors.clear();
      parsingSuccessful = Json::parseFromStream(rbuilder, ifs, &m_value, &formattedErrors);
    }

    if (parsingSuccessful) {

      Json::Value project = m_value.get("project", Json::objectValue);
      m_project.load(project, *this);

      // Establishes the unit conversion from the project's config property.
      setUnits();

      // Load all the space assignment objects so that when the space is loaded
      // we can find the objects via their ids
      const Json::Value& thermalZones = m_value.get("thermal_zones", Json::arrayValue);
      for (Json::ArrayIndex t = 0; t < thermalZones.size(); t++) {
        m_thermalZones.emplace_back(FSThermalZone(thermalZones[t]));
      }

      const Json::Value& buildingUnits = m_value.get("building_units", Json::arrayValue);
      for (Json::ArrayIndex b = 0; b < buildingUnits.size(); b++) {
        m_buildingUnits.emplace_back(FSBuildingUnit(buildingUnits[b]));
      }

      const Json::Value& buildingTypes = m_value.get("building+type", Json::arrayValue);
      for (Json::ArrayIndex i = 0; i < buildingTypes.size(); i++) {
      }

      const Json::Value& spaceTypes = m_value.get("space_types", Json::arrayValue);
      for (Json::ArrayIndex s = 0; s < spaceTypes.size(); s++) {
        m_spaceTypes.emplace_back(FSSpaceType(spaceTypes[s]));
      }

      const Json::Value& constructionSets = m_value.get("construction_sets", Json::arrayValue);
      for (Json::ArrayIndex s = 0; s < constructionSets.size(); s++) {
        m_constructionSets.emplace_back(FSConstructionSet(constructionSets[s]));
      }

      const Json::Value& daylightingControlDefinitions = m_value.get("daylighting_control_definitions", Json::arrayValue);
      for (Json::ArrayIndex i = 0; i < daylightingControlDefinitions.size(); i++) {
        m_daylightingControlDefinitions.emplace_back(FSDaylightingControlDefinition(daylightingControlDefinitions[i], *this));
      }

      const Json::Value& windowDefinitions = m_value.get("window_definitions", Json::arrayValue);
      for (Json::ArrayIndex i = 0; i < windowDefinitions.size(); i++) {
        m_windowDefinitions.emplace_back(FSWindowDefinition(windowDefinitions[i], *this));
      }

      const Json::Value& doorDefinitions = m_value.get("door_definitions", Json::arrayValue);
      for (Json::ArrayIndex i = 0; i < doorDefinitions.size(); i++) {
        m_doorDefinitions.emplace_back(FSDoorDefinition(doorDefinitions[i], *this));
      }

      const Json::Value& stories = m_value.get("stories", Json::arrayValue);
      for (Json::ArrayIndex storyIdx = 0; storyIdx < stories.size(); ++storyIdx) {
        m_stories.emplace_back(FSStory(stories[storyIdx], *this));
      }
    }
  }
}

boost::optional<FSThermalZone> FSModel::thermalZone(const std::string& thermalZoneId) const {

  for (auto& zone : m_thermalZones) {
    if (zone.id() == thermalZoneId) {
      return zone;
    }
  }

  return boost::none;
}

boost::optional<FSBuildingUnit> FSModel::buildingUnit(const std::string& buildingUnitId) const {

  for (auto& unit : m_buildingUnits) {
    if (unit.id() == buildingUnitId) {
      return unit;
    }
  }

  return boost::none;
}

boost::optional<FSBuildingType> FSModel::buildingType(const std::string& buildingTypeId) const {

  for (auto& type : m_buildingTypes) {
    if (type.id() == buildingTypeId) {
      return type;
    }
  }

  return boost::none;
}

boost::optional<FSSpaceType> FSModel::spaceType(const std::string& spaceTypeId) const {

  for (auto& spacetype : m_spaceTypes) {
    if (spacetype.id() == spaceTypeId) {
      return spacetype;
    }
  }

  return boost::none;
}

boost::optional<FSConstructionSet> FSModel::constructionSet(const std::string& constructionSetId) const {
  for (auto& constructionSet : m_constructionSets) {
    if (constructionSet.id() == constructionSetId) {
      return constructionSet;
    }
  }

  return boost::none;
}

boost::optional<FSDaylightingControlDefinition> FSModel::daylightingControlDefinition(const std::string& id) const {
  for (auto& definition : m_daylightingControlDefinitions) {
    if (definition.id() == id) {
      return definition;
    }
  }

  return boost::none;
}

boost::optional<FSWindowDefinition> FSModel::windowDefinition(const std::string id) const {
  for (const auto& definition : m_windowDefinitions) {
    if (definition.id() == id) {
      return definition;
    }
  }
  return boost::none;
}

boost::optional<FSDoorDefinition> FSModel::doorDefinition(const std::string id) const {
  for (const auto& definition : m_doorDefinitions) {
    if (definition.id() == id) {
      return definition;
    }
  }
  return boost::none;
}

boost::optional<FSProject> FSModel::project() const {
  return m_project;
}

double FSModel::lengthToMeters() const {
  return m_lengthToMeters;
}

void FSModel::setUnits() {
  m_lengthToMeters = 1;
  if (m_project.config().units() == "ip" || m_project.config().units() == "ft") {
    m_lengthToMeters = 0.3048;
  }
}

std::vector<FSThermalZone> FSModel::thermalZones() const {
  return m_thermalZones;
}

std::vector<FSBuildingUnit> FSModel::buildingUnits() const {
  return m_buildingUnits;
}

std::vector<FSBuildingType> FSModel::buildingTypes() const {
  return m_buildingTypes;
}

std::vector<FSSpaceType> FSModel::spaceTypes() const {
  return m_spaceTypes;
}

std::vector<FSConstructionSet> FSModel::constructionSets() const {
  return m_constructionSets;
}

const std::vector<FSDaylightingControlDefinition> FSModel::daylightingControlDefinitions() const {
  return m_daylightingControlDefinitions;
}

const std::vector<FSWindowDefinition> FSModel::windowDefinitions() const {
  return m_windowDefinitions;
}

const std::vector<FSDoorDefinition> FSModel::doorDefinitions() const {
  return m_doorDefinitions;
}

std::vector<FSStory> FSModel::stories() const {
  return m_stories;
}

bool FSModel::checkKeyAndType(const Json::Value& value, const std::string& key, const Json::ValueType& valueType) {
  if (value.isMember(key)) {
    if (value[key].isConvertibleTo(valueType)) {
      if (value[key].isNull()) {
        if (valueType == Json::nullValue) {
          return true;
        } else {
          return false;
        }
      }
      // not null and is convertible
      return true;
    } else {
      // not convertible to valueType
      LOG_FREE(Warn, "JSON", "Key '" << key << "' exists but is not the correct type");
    }
  }
  return false;
}

void FSModel::assertKeyAndType(const Json::Value& value, const std::string& key, const Json::ValueType& valueType) {
  assertKey(value, key);
  assertType(value, key, valueType);
}

void FSModel::assertKey(const Json::Value& value, const std::string& key) {
  if (!checkKey(value, key)) {
    throw openstudio::Exception(std::string("Cannot find key '" + key + "'"));
  }
}

// assert type is correct if key is present
void FSModel::assertType(const Json::Value& value, const std::string& key, const Json::ValueType& valueType) {
  if (!checkType(value, key, valueType)) {
    throw openstudio::Exception(std::string("Key '" + key + "' is of wrong type"));
  }
}

void FSModel::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

//std::ostream& operator<<(std::ostream& os, const Plane& plane) {
//  os << "[" << plane.a() << ", " << plane.b() << ", " << plane.c() << ", " << plane.d() << "]";
//  return os;
//}
//
///////////////////////////////////////////////////////////////////////////////////

FSProject::FSProject() {
  m_north_axis = 0;
}

FSProject::FSProject(const Json::Value& root, const FSModel& model) {
  load(root, model);
}

void FSProject::load(const Json::Value& root, const FSModel& model) {

  // Get config
  Json::Value config = root.get("config", Json::objectValue);
  m_config = FSConfig(config);

  Json::Value ground = root.get("ground", Json::objectValue);
  m_ground = FSGround(ground, model);

  // Get north_Axis
  if (root.isMember("north_axis")) {
    m_north_axis = root.get("north_axis", m_north_axis).asDouble();
  } else {
    m_north_axis = m_config.northAxis();
  }
}

double FSProject::northAxis() {
  return m_north_axis;
}

FSConfig& FSProject::config() {
  return m_config;
}

FSGround& FSProject::ground() {
  return m_ground;
}

///////////////////////////////////////////////////////////////////////////////////

FSConfig::FSConfig() {
  m_unitsEditable = false;
}

FSConfig::FSConfig(const Json::Value& root) {
  m_units = root.get("units", m_units).asString();
  m_unitsEditable = root.get("unitsEditable", m_unitsEditable).asBool();
  m_language = root.get("language", m_language).asString();
  if (root.isMember("north_axis")) {
    m_north_axis = root.get("north_axis", m_north_axis).asDouble();
  }
}

std::string FSConfig::units() {
  return m_units;
}

std::string FSConfig::language() {
  return m_language;
}

bool FSConfig::unitsEditable() {
  return m_unitsEditable;
}

double FSConfig::northAxis() const {
  return m_north_axis;
}
///////////////////////////////////////////////////////////////////////////////////

FSGround::FSGround() {
  m_floor_offset = 0;
  m_azimuth_angle = 0;
  m_tilt_slope = 0;
}

FSGround::FSGround(const Json::Value& root, const FSModel& model) {

  m_floor_offset = root.get("floor_offset", 0).asDouble() * model.lengthToMeters();
  m_azimuth_angle = root.get("azimuth_angle", 0).asDouble();
  m_tilt_slope = root.get("tilt_slope", 0).asDouble();
}

double FSGround::floorOffset() {
  return m_floor_offset;
}

double FSGround::azimuthAngle() {
  return m_azimuth_angle;
}

double FSGround::tiltSlope() {
  return m_tilt_slope;
}

///////////////////////////////////////////////////////////////////////////////////

FSBase::FSBase() {}

FSBase::FSBase(const Json::Value& root) {
  FSModel::assertKeyAndType(root, "name", Json::stringValue);
  m_name = root.get("name", m_name).asString();
  //FSModel::assertKeyAndType(root, "handle", Json::stringValue);
  m_handle = root.get("handle", m_handle).asString();
  FSModel::assertKeyAndType(root, "id", Json::stringValue);
  m_id = root.get("id", m_id).asString();
  //FSModel::assertKeyAndType(root, "color", Json::stringValue);
  m_color = root.get("color", "").asString();
}

const std::string& FSBase::name() const {
  return m_name;
}

const std::string& FSBase::handle() const {
  return m_handle;
}

const std::string& FSBase::id() const {
  return m_id;
}

const std::string& FSBase::color() const {
  return m_color;
}

void FSBase::setName(const std::string& name) {
  m_name = name;
}

void FSBase::setHandle(const std::string& handle) {
  m_handle = handle;
}

void FSBase::setId(const std::string& id) {
  m_id = id;
}

void FSBase::setColor(const std::string& color) {
  m_color = color;
}

///////////////////////////////////////////////////////////////////////////////////

FSStory::FSStory() {

  m_below_floor_plenum_height = 0;
  m_floor_to_ceiling_height = 8;
  m_above_ceiling_plenum_height = 0;
  m_multiplier = 1;
  m_image_visible = false;
}

FSStory::FSStory(const Json::Value& root, const FSModel& model) : FSBase(root) {

  m_below_floor_plenum_height = root.get("below_floor_plenum_height", 0).asDouble() * model.lengthToMeters();
  // Defaults to 3m but also cannot be less than 0.1
  m_floor_to_ceiling_height = root.get("floor_to_ceiling_height", 3 / model.lengthToMeters()).asDouble() * model.lengthToMeters();
  if (m_floor_to_ceiling_height < 0.1) {
    m_floor_to_ceiling_height = 3;
  }
  m_above_ceiling_plenum_height = root.get("above_ceiling_plenum_height", 0).asDouble() * model.lengthToMeters();
  m_image_visible = root.get("image_visible", m_image_visible).asBool();
  m_multiplier = std::max(root.get("multiplier", 1).asInt(), 1);  // Make sure minimum value is 1

  const Json::Value& geometry = root.get("geometry", Json::arrayValue);
  m_geometry.load(geometry, model);

  const Json::Value& spaces = root.get("spaces", Json::arrayValue);
  for (Json::ArrayIndex i = 0; i < spaces.size(); i++) {
    FSSpace space = FSSpace(spaces[i], model, *this);
    if (space.face().has_value()) {
      m_spaces.push_back(space);
    }
  }

  const Json::Value& windows = root.get("windows", Json::arrayValue);
  for (Json::ArrayIndex i = 0; i < windows.size(); i++) {
    FSWindow window = FSWindow(windows[i], model, *this);
    if (window.edge().has_value()) {
      m_windows.push_back(window);
    }
  }

  const Json::Value& doors = root.get("doors", Json::arrayValue);
  for (Json::ArrayIndex i = 0; i < doors.size(); i++) {
    FSDoor door = FSDoor(doors[i], model, *this);
    if (door.edge().has_value()) {
      m_doors.push_back(door);
    }
  }

  const Json::Value& shadings = root.get("shading", Json::arrayValue);
  for (Json::ArrayIndex i = 0; i < shadings.size(); i++) {
    FSShading shading = FSShading(shadings[i], model, *this);
    if (shading.face().has_value()) {
      m_shadings.push_back(shading);
    }
  }
}

double FSStory::gelowFloorPlenumHeight() const {
  return m_below_floor_plenum_height;
}

double FSStory::floorToCeilingHeight() const {
  return m_floor_to_ceiling_height;
}

double FSStory::aboveCeilingPlenumHeight() const {
  return m_above_ceiling_plenum_height;
}

double FSStory::floorToFloorHeight() const {
  return m_below_floor_plenum_height + m_floor_to_ceiling_height + m_above_ceiling_plenum_height;
}

// TODO: MUst be calculated as stories are created
double FSStory::nominalZCoordinate() const {
  return m_nominalZCoordinate;
}

int FSStory::multiplier() const {
  return m_multiplier;
}

bool FSStory::imageVisible() const {
  return m_image_visible;
}

FSGeometry& FSStory::geometry() {
  return m_geometry;
}

std::vector<FSSpace> FSStory::spaces() const {
  return m_spaces;
}

std::vector<FSWindow> FSStory::windows() const {
  return m_windows;
}

std::vector<FSDoor> FSStory::doors() const {
  return m_doors;
}

std::vector<FSShading> FSStory::shadings() const {
  return m_shadings;
}

void FSStory::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSAssignment::FSAssignment() {}

FSAssignment::FSAssignment(const Json::Value& root) : FSBase(root) {}

///////////////////////////////////////////////////////////////////////////////////

FSThermalZone::FSThermalZone() {}

FSThermalZone::FSThermalZone(const Json::Value& root) : FSAssignment(root) {}

void FSThermalZone::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSBuildingUnit::FSBuildingUnit() {}

FSBuildingUnit::FSBuildingUnit(const Json::Value& root) : FSAssignment(root) {}

void FSBuildingUnit::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSBuildingType::FSBuildingType() {}

FSBuildingType::FSBuildingType(const Json::Value& root) : FSAssignment(root) {}

void FSBuildingType::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSSpaceType::FSSpaceType() {}

FSSpaceType::FSSpaceType(const Json::Value& root) : FSAssignment(root) {}

void FSSpaceType::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSConstructionSet::FSConstructionSet() {}

FSConstructionSet::FSConstructionSet(const Json::Value& root) : FSAssignment(root) {}

void FSConstructionSet::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSSpace::FSSpace() {
  m_belowFloorPlenumHeight = 0;
  m_floorToCeilingHeight = 0;
  m_aboveCeilingHeight = 0;
  m_offset = 0;
  m_multiplier = 1;
  m_openToBelow = false;
}

FSSpace::FSSpace(const Json::Value& root, const FSModel& model, FSStory& story) : FSBase(root) {

  std::string id = root.get("thermal_zone_id", "").asString();
  m_thermalZone = model.thermalZone(id);
  id = root.get("building_unit_id", "").asString();
  m_buildingUnit = model.buildingUnit(id);
  id = root.get("space_type_id", "").asString();
  m_spaceType = model.spaceType(id);
  id = root.get("construction_set_id", "").asString();
  m_constructionSet = model.constructionSet(id);
  m_story = story;

  // Heights are optional, if not defined then the value is inherited from the story, if
  // defined then the value overrides the value from the story
  m_belowFloorPlenumHeight = story.gelowFloorPlenumHeight();
  if (checkKeyAndType(root, "below_floor_plenum_height", Json::realValue)) {
    double defaultValue = m_belowFloorPlenumHeight / model.lengthToMeters();
    m_belowFloorPlenumHeight = root.get("below_floor_plenum_height", defaultValue).asDouble() * model.lengthToMeters();
  }

  m_floorToCeilingHeight = story.floorToCeilingHeight();
  if (checkKeyAndType(root, "floor_to_ceiling_height", Json::realValue)) {  // Doing this skips null values for exmaple
    double defaultValue = m_floorToCeilingHeight / model.lengthToMeters();
    m_floorToCeilingHeight = root.get("floor_to_ceiling_height", defaultValue).asDouble() * model.lengthToMeters();
  }

  m_aboveCeilingHeight = story.aboveCeilingPlenumHeight();
  if (checkKeyAndType(root, "above_ceiling_plenum_height", Json::realValue)) {
    double defaultValue = story.aboveCeilingPlenumHeight() / model.lengthToMeters();
    m_aboveCeilingHeight = root.get("above_ceiling_plenum_height", defaultValue).asDouble() * model.lengthToMeters();
  }

  m_offset = root.get("floor_offset", 0).asDouble() * model.lengthToMeters();
  m_openToBelow = root.get("open_to_below", false).asBool();
  m_multiplier = std::max(root.get("multiplier", story.multiplier()).asInt(), 1);

  // Get the face. Note: a space must have a face for it to be added to the model
  if (FSModel::checkKeyAndType(root, "face_id", Json::stringValue)) {
    std::string face_id = root.get("face_id", "").asString();
    m_face = story.geometry().face(face_id);
  }

  const Json::Value& daylightingControls = root.get("daylighting_controls", Json::arrayValue);
  for (Json::ArrayIndex i = 0; i < daylightingControls.size(); i++) {
    m_daylightingControls.emplace_back(FSDaylightingControl(daylightingControls[i], model, story));
  }

  simplifyFace(story.geometry());
}

boost::optional<FSThermalZone> FSSpace::thermalZone() const {
  return m_thermalZone;
}

boost::optional<FSBuildingUnit> FSSpace::buildingUnit() const {
  return m_buildingUnit;
}

boost::optional<FSSpaceType> FSSpace::spaceType() const {
  return m_spaceType;
}

boost::optional<FSConstructionSet> FSSpace::constructionSet() const {
  return m_constructionSet;
}

//boost::optional<FSStory&> FSSpace::getStory() const {
//  return m_story;
//}

boost::optional<FSFace> FSSpace::face() const {
  return m_face;
}

int FSSpace::multiplier() const {
  return m_multiplier;
}

double FSSpace::belowFloorPlenumHeight() const {
  return m_belowFloorPlenumHeight;
}

double FSSpace::floorToCeilingHeight() const {
  return m_floorToCeilingHeight;
}

double FSSpace::aboveCeilingHeight() const {
  return m_aboveCeilingHeight;
}

double FSSpace::offset() const {
  return m_offset;
}

bool FSSpace::openToBelow() const {
  return m_openToBelow;
}

const std::vector<FSDaylightingControl> FSSpace::daylightingControls() const {
  return m_daylightingControls;
}

void FSSpace::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

void FSSpace::simplifyFace(FSGeometry& geometry) {
  Point3dVector faceVertices;
  for (auto edgeRef : m_face->edgeRefs()) {
    const FSVertex& vertex = edgeRef.getNextVertex();
    faceVertices.emplace_back(Point3d(vertex.x(), vertex.y(), 0));
  }

  // Simplify and keep inline vertices, the count should be the same
  // but if the vertex order has changed then there were overlapping
  // edges. If this is the case then a new face and new edges are created
  Point3dVector simplifiedVertices = simplify(faceVertices, false, 0.01);
  if (!circularEqual(simplifiedVertices, faceVertices)) {

    // Now simplify and remove inline vertices giviong us a cleanpolygon to work with
    simplifiedVertices = simplify(faceVertices, true, 0.01);
    // If the number of vertices has changed then we will create new edges
    // based on the simplified vertices
    if (simplifiedVertices.size() != faceVertices.size()) {
      std::vector<FSEdgeReference> edgeRefs;
      // Create new edges for the face
      for (unsigned i = 0; i < simplifiedVertices.size(); i++) {
        Point3d& p1 = simplifiedVertices[i];
        Point3d& p2 = simplifiedVertices[(i + 1) % simplifiedVertices.size()];

        auto v1 = geometry.vertex(p1);
        auto v2 = geometry.vertex(p2);
        if (v1.has_value() && v2.has_value()) {
          //boost::optional<FSEdge> edge = geometry.getEdge(*v1, *v2);
          //if (edge == boost::none)
          //  edge = FSEdge (*v1, *v2);
          //edgeRefs.emplace_back(FSEdgeReference(*edge, 1));
          FSEdge edge(*v1, *v2);
          FSEdgeReference edgeRef(edge, 1);
          edgeRefs.emplace_back(edgeRef);
        }
      }

      // Update the face
      m_face->clearEdgeRefs();
      m_face->setEdgeRefs(edgeRefs);
    }
    // Get a list of windows and doors that were associated with the old edges
    // Remap these windows and doors to the new edges
  }
}

///////////////////////////////////////////////////////////////////////////////////

FSGeometry::FSGeometry() {}

FSGeometry::FSGeometry(const Json::Value& root, const FSModel& model) {
  load(root, model);
}

void FSGeometry::load(const Json::Value& root, const FSModel& model) {
  FSGeometryBase::load(root);

  const Json::Value& vertices = root.get("vertices", Json::arrayValue);
  for (Json::ArrayIndex i = 0; i < vertices.size(); i++) {
    m_vertices.emplace_back(FSVertex(vertices[i], model));
  }

  const Json::Value& edges = root.get("edges", Json::arrayValue);
  for (Json::ArrayIndex i = 0; i < edges.size(); i++) {
    m_edges.emplace_back(FSEdge(edges[i], *this));
  }

  const Json::Value& faces = root.get("faces", Json::arrayValue);
  for (Json::ArrayIndex i = 0; i < faces.size(); i++) {
    m_faces.emplace_back(FSFace(faces[i], *this));
  }
}

boost::optional<FSVertex> FSGeometry::vertex(const std::string& id) const {

  for (auto& vertex : m_vertices) {
    if (vertex.id() == id) {
      return vertex;
    }
  }

  return boost::none;
}

boost::optional<FSVertex> FSGeometry::vertex(const Point3d& p) const {
  double tol = 0.01;
  for (auto& vertex : m_vertices) {
    if (std::abs(vertex.x() - p.x()) < tol && std::abs(vertex.y() - p.y()) < tol) {
      return vertex;
    }
  }

  return boost::none;
}

boost::optional<FSEdge> FSGeometry::edge(const std::string& id) const {

  for (auto& edge : m_edges) {
    if (edge.id() == id) {
      return edge;
    }
  }

  return boost::none;
}

boost::optional<FSEdge> FSGeometry::edge(const FSVertex& v1, const FSVertex& v2) const {
  for (auto& edge : m_edges) {
    if (edge.firstVertex().id() == v1.id() && edge.secondVertex().id() == v2.id()) {
      return edge;
    } else if (edge.firstVertex().id() == v2.id() && edge.secondVertex().id() == v1.id()) {
      return edge;
    }
  }

  return boost::none;
}

boost::optional<FSFace> FSGeometry::face(const std::string& id) const {

  for (auto& face : m_faces) {
    if (face.id() == id) {
      return face;
    }
  }

  return boost::none;
}

///////////////////////////////////////////////////////////////////////////////////

FSGeometryBase::FSGeometryBase() {
  m_id = "";
}

FSGeometryBase::FSGeometryBase(const Json::Value& root) {
  load(root);
}

void FSGeometryBase::load(const Json::Value& root) {
  m_id = root.get("id", m_id).asString();
}

const std::string& FSGeometryBase::id() const {
  return m_id;
}

///////////////////////////////////////////////////////////////////////////////////

FSVertex::FSVertex() {
  m_x = 0;
  m_y = 0;
}

FSVertex::FSVertex(const Json::Value& root, const FSModel& model) {
  load(root, model);
}

void FSVertex::load(const Json::Value& root, const FSModel& model) {

  FSGeometryBase::load(root);
  m_x = root.get("x", 0).asDouble() * model.lengthToMeters();
  m_y = root.get("y", 0).asDouble() * model.lengthToMeters();
}

double FSVertex::x() const {
  return m_x;
}

double FSVertex::y() const {
  return m_y;
}

void FSVertex::setX(double x) {
  m_x = x;
}

void FSVertex::setY(double y) {
  m_y = y;
}

void FSVertex::setXY(double x, double y) {
  m_x = x;
  m_y = y;
}

///////////////////////////////////////////////////////////////////////////////////

FSEdge::FSEdge() {}

FSEdge::FSEdge(const Json::Value& root, const FSGeometry& geometry) {
  load(root, geometry);
}

FSEdge::FSEdge(FSVertex& v1, FSVertex& v2) {
  m_vertices.push_back(v1);
  m_vertices.push_back(v2);
}

void FSEdge::load(const Json::Value& root, const FSGeometry& geometry) {

  FSGeometryBase::load(root);
  const Json::Value& vertexIds = root.get("vertex_ids", Json::arrayValue);
  for (Json::ArrayIndex i = 0; i < vertexIds.size(); i++) {
    m_vertices.push_back(geometry.vertex(vertexIds[i].asString()).value());
  }
}

std::vector<FSVertex> FSEdge::getVertices() const {
  return m_vertices;
}

const FSVertex& FSEdge::firstVertex() const {
  return m_vertices[0];
}

const FSVertex& FSEdge::secondVertex() const {
  return m_vertices[1];
}

const Vector3d& FSEdge::edgeVector() const {
  Point3d p1(m_vertices[0].x(), m_vertices[0].y(), 0);
  Point3d p2(m_vertices[1].x(), m_vertices[1].y(), 0);
  return *new Vector3d(p2 - p1);
}

Point3d FSEdge::vertex(double alpha) const {
  double x = (1.0 - alpha) * m_vertices[0].x() + alpha * m_vertices[1].x();
  double y = (1.0 - alpha) * m_vertices[0].y() + alpha * m_vertices[1].y();
  return Point3d(x, y, 0);
}

///////////////////////////////////////////////////////////////////////////////////

FSFace::FSFace() {}

FSFace::FSFace(const Json::Value& root, const FSGeometry& geometry) {
  load(root, geometry);
}

void FSFace::load(const Json::Value& root, const FSGeometry& geometry) {

  FSGeometryBase::load(root);
  const Json::Value& edgeIds = root.get("edge_ids", Json::arrayValue);
  const Json::Value& edgeOrders = root.get("edge_order", Json::arrayValue);
  if (edgeIds.size() != edgeOrders.size())
    throw openstudio::Exception(std::string("The number or edge orders does not equal the number of edge ids"));

  for (unsigned int i = 0; i < edgeIds.size(); i++) {
    boost::optional<FSEdge> edge = geometry.edge(edgeIds[i].asString()).value();
    m_edgeRefs.emplace_back(FSEdgeReference(*edge, edgeOrders[i].asInt()));
  }
}

std::vector<FSEdgeReference> FSFace::edgeRefs() {
  return m_edgeRefs;
}

void FSFace::clearEdgeRefs() {
  m_edgeRefs.clear();
}

void FSFace::setEdgeRefs(std::vector<FSEdgeReference> edgeRefs) {
  for (auto& edgeRef : edgeRefs) {
    m_edgeRefs.push_back(edgeRef);
  }
}
///////////////////////////////////////////////////////////////////////////////////

FSEdgeReference::FSEdgeReference() {
  m_edgeOrder = 1;
}

FSEdgeReference::FSEdgeReference(FSEdge& edge, int edgeOrder) {
  m_edge = edge;
  m_edgeOrder = edgeOrder;
}

const FSEdge& FSEdgeReference::edge() const {
  return m_edge;
}

int FSEdgeReference::edgeOrder() const {
  return m_edgeOrder;
}

const FSVertex& FSEdgeReference::getNextVertex() {
  if (m_edgeOrder == 1)
    return m_edge.firstVertex();
  else
    return m_edge.secondVertex();
}

///////////////////////////////////////////////////////////////////////////////////

FSFiller::FSFiller() {
  m_alpha = 0.5;
}

FSFiller::FSFiller(const Json::Value& root, const FSModel& model, FSStory& story) : FSBase(root) {

  m_alpha = 0.5;
  // Alhpa can be a single value or a list
  if (root.isMember("alpha") && root["alpha"].isConvertibleTo(Json::realValue)) {
    m_alphas.push_back(root.get("alpha", 0.5).asDouble());
  } else {
    Json::Value alphas = root.get("alpha", Json::arrayValue);
    for (Json::ArrayIndex i = 0; i < alphas.size(); i++) {
      m_alphas.push_back(alphas[i].asDouble());
    }
  }

  if (FSModel::checkKeyAndType(root, "edge_id", Json::stringValue)) {
    std::string edge_id = root.get("edge_id", "").asString();
    m_edge = story.geometry().edge(edge_id).value();

    // TODO: If edges knew about faces then we could use the edge to get the face
    // and that would lead us to the space and that would mean we could associate
    // a window with a space (if we need to). Either wa we need to be able to use
    // the edge to dind the wall which is the parent surface for the sub-surface
  }
}

boost::optional<FSEdge> FSFiller::edge() const {
  return m_edge;
}

boost::optional<FSSpace> FSFiller::space() const {
  return m_space;
}

std::vector<double> FSFiller::alphas() const {
  return m_alphas;
}

Point3d FSFiller::centerVertex(double alpha) {
  return m_edge->vertex(alpha);
}
///////////////////////////////////////////////////////////////////////////////////

FSWindow::FSWindow() {}

FSWindow::FSWindow(const Json::Value& root, const FSModel& model, FSStory& story) : FSFiller(root, model, story) {

  if (FSModel::checkKeyAndType(root, "window_definition_id", Json::stringValue)) {
    std::string windowDefinitionId = root.get("window_definition_id", "").asString();
    m_windowDefinition = model.windowDefinition(windowDefinitionId);
  }
}

boost::optional<FSWindowDefinition> FSWindow::windowDefinition() const {
  return m_windowDefinition;
}

void FSWindow::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSDoor::FSDoor() {}

FSDoor::FSDoor(const Json::Value& root, const FSModel& model, FSStory& story) : FSFiller(root, model, story) {

  if (FSModel::checkKeyAndType(root, "door_definition_id", Json::stringValue)) {
    std::string doorDefinitionId = root.get("door_definition_id", "").asString();
    m_doorDefinition = model.doorDefinition(doorDefinitionId);
  }
}

void FSDoor::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

boost::optional<FSDoorDefinition> FSDoor::doorDefinition() const {
  return m_doorDefinition;
}

///////////////////////////////////////////////////////////////////////////////////

FSShading::FSShading() {}

FSShading::FSShading(const Json::Value& root, const FSModel& model, FSStory& story) : FSBase(root) {

  // Get the face. Note: a shading must have a face for it to be added to the model
  if (FSModel::checkKeyAndType(root, "face_id", Json::stringValue)) {
    std::string face_id = root.get("face_id", "").asString();
    m_face = story.geometry().face(face_id);
  }
}

boost::optional<FSFace> FSShading::face() const {
  return m_face;
}

void FSShading::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSDaylightingControl::FSDaylightingControl() {}

FSDaylightingControl::FSDaylightingControl(const Json::Value& root, const FSModel& model, FSStory& story) : FSBase(root) {

  std::string vertex_id = root.get("vertex_id", "").asString();
  if (FSModel::checkKeyAndType(root, "vertex_id", Json::stringValue)) {
    std::string vertex_id = root.get("vertex_id", "").asString();
    m_vertex = story.geometry().vertex(vertex_id);
  }

  if (FSModel::checkKeyAndType(root, "daylighting_control_definition_id", Json::stringValue)) {
    std::string control_id = root.get("daylighting_control_definition_id", "").asString();
    m_definition = model.daylightingControlDefinition(control_id);
  }
}

boost::optional<FSVertex> FSDaylightingControl::vertex() const {
  return m_vertex;
}

const Point3d& FSDaylightingControl::point() const {
  if (m_vertex.has_value() && m_definition.has_value()) {
    return *new Point3d(m_vertex->x(), m_vertex->y(), m_definition->height());
  } else {

    return *new Point3d(0, 0, 0);
  }
}

boost::optional<FSDaylightingControlDefinition> FSDaylightingControl::definition() const {
  return m_definition;
}

void FSDaylightingControl::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSDaylightingControlDefinition::FSDaylightingControlDefinition() {
  m_illuminanceSetpoint = 300;
  m_height = 0;
}

FSDaylightingControlDefinition::FSDaylightingControlDefinition(const Json::Value& root, const FSModel& model) : FSBase(root) {
  m_illuminanceSetpoint = root.get("illuminance_Setpoint", 300).asDouble();
  m_height = root.get("height", 0).asDouble() * model.lengthToMeters();
}

double FSDaylightingControlDefinition::illuminanceSetpoint() const {
  return m_illuminanceSetpoint;
}

double FSDaylightingControlDefinition::height() const {
  return m_height;
}

void FSDaylightingControlDefinition::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSWindowDefinition::FSWindowDefinition() {
  m_overhangProjectionFactor = 0;
  m_finProjectionFactor = 0;
  m_height = 0;
  m_width = 0;
  m_sillHeight = 0;
  m_wwr = 0;
  m_windowSpacing = 0;
}

FSWindowDefinition::FSWindowDefinition(const Json::Value& root, const FSModel& model) : FSBase(root) {

  // Property can be window_definition_mode or window_definition_type
  if (checkKeyAndType(root, "window_definition_type", Json::stringValue)) {
    m_windowDefinitionMode = root.get("window_definition_type", "Single Window").asString();
  } else {
    assertKeyAndType(root, "window_definition_mode", Json::stringValue);
    m_windowDefinitionMode = root.get("window_definition_mode", "Single Window").asString();
  }
  m_windowType = root.get("window_type", "Fixed").asString();
  if (root.get("window_type", "Fixed").asString() == "Fixed")
    m_windowType = "FixedWindow";
  else if (root.get("window_type", "Fixed").asString() == "Operable")
    m_windowType = "OperableWindow";
  m_wwr = root.get("wwr", 0.5).asDouble();
  m_sillHeight = root.get("sill_height", 0).asDouble() * model.lengthToMeters();
  m_windowSpacing = root.get("window_spacing", 0).asDouble() * model.lengthToMeters();
  m_height = root.get("height", 0).asDouble() * model.lengthToMeters();
  m_width = root.get("width", 0).asDouble() * model.lengthToMeters();
  m_overhangProjectionFactor = root.get("overhang_projection_factor", 0).asDouble();
  m_finProjectionFactor = root.get("fin_projection_factor", 0).asDouble();
}

std::string FSWindowDefinition::windowDefinitionMode() const {
  return m_windowDefinitionMode;
}

double FSWindowDefinition::wwr() const {
  return m_wwr;
}

double FSWindowDefinition::sillHeight() const {
  return m_sillHeight;
}

double FSWindowDefinition::windowSpacing() const {
  return m_windowSpacing;
}

double FSWindowDefinition::height() const {
  return m_height;
}

double FSWindowDefinition::width() const {
  return m_width;
}

std::string FSWindowDefinition::windowType() const {
  return m_windowType;
}

double FSWindowDefinition::overhangProjectionFactor() const {
  return m_overhangProjectionFactor;
}

double FSWindowDefinition::finProjectionFactor() const {
  return m_finProjectionFactor;
}

void FSWindowDefinition::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSDoorDefinition::FSDoorDefinition() {
  m_height = 2;
  m_width = 1;
}

FSDoorDefinition::FSDoorDefinition(const Json::Value& root, const FSModel& model) : FSBase(root) {
  m_height = root.get("height", 2 / model.lengthToMeters()).asDouble() * model.lengthToMeters();
  m_width = root.get("width", 1 / model.lengthToMeters()).asDouble() * model.lengthToMeters();
  m_doorType = root.get("door_type", "Door").asString();
}

double FSDoorDefinition::height() const {
  return m_height;
}

double FSDoorDefinition::width() const {
  return m_width;
}

const std::string& FSDoorDefinition::doorType() const {
  return m_doorType;
}

void FSDoorDefinition::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

}  // namespace openstudio
