/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "FSModel.hpp"

#include "../core/Assert.hpp"
#include "../core/Json.hpp"
#include "../geometry/Point3d.hpp"
#include "../geometry/Vector3d.hpp"
#include "../geometry/Geometry.hpp"
#include "../geometry/Intersection.hpp"

namespace openstudio {

FSModel::FSModel(const std::string& json) {
  load(json);
}

void FSModel::load(const std::string& json) {
  Json::CharReaderBuilder rbuilder;
  std::istringstream ss(json);
  std::string formattedErrors;
  // Parse the json string, if this fails we will check to see if a path has been provided
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
  }

  if (parsingSuccessful) {

    Json::Value project = m_value.get("project", Json::objectValue);
    m_project.load(project, *this);

    // Establishes the unit conversion from the project's config property.
    setUnits();

    // Load all the space assignment objects so that when the space is loaded
    // we can find the objects via their ids
    for (const auto& thermalZone : m_value.get("thermal_zones", Json::arrayValue)) {
      m_thermalZones.emplace_back(thermalZone);
    }

    for (const auto& buildingUnit : m_value.get("building_units", Json::arrayValue)) {
      m_buildingUnits.emplace_back(buildingUnit);
    }

    // TODO: not doing anything yet
    for (const auto& buildingType : m_value.get("building_type", Json::arrayValue)) {
      m_buildingTypes.emplace_back(buildingType);
    }

    for (const auto& spaceType : m_value.get("space_types", Json::arrayValue)) {
      m_spaceTypes.emplace_back(spaceType);
    }

    for (const auto& constructionSet : m_value.get("construction_sets", Json::arrayValue)) {
      m_constructionSets.emplace_back(constructionSet);
    }

    for (const auto& daylightingControlDefinition : m_value.get("daylighting_control_definitions", Json::arrayValue)) {
      m_daylightingControlDefinitions.emplace_back(daylightingControlDefinition, *this);
    }

    for (const auto& windowDefinition : m_value.get("window_definitions", Json::arrayValue)) {
      m_windowDefinitions.emplace_back(windowDefinition, *this);
    }

    for (const auto& doorDefinition : m_value.get("door_definitions", Json::arrayValue)) {
      m_doorDefinitions.emplace_back(doorDefinition, *this);
    }

    for (const auto& story : m_value.get("stories", Json::arrayValue)) {
      m_stories.emplace_back(story, *this);
    }
  }
}

boost::optional<FSThermalZone> FSModel::thermalZone(const std::string& thermalZoneId) const {

  for (const auto& zone : m_thermalZones) {
    if (zone.id() == thermalZoneId) {
      return zone;
    }
  }

  return boost::none;
}

boost::optional<FSBuildingUnit> FSModel::buildingUnit(const std::string& buildingUnitId) const {

  for (const auto& unit : m_buildingUnits) {
    if (unit.id() == buildingUnitId) {
      return unit;
    }
  }

  return boost::none;
}

boost::optional<FSBuildingType> FSModel::buildingType(const std::string& buildingTypeId) const {

  for (const auto& type : m_buildingTypes) {
    if (type.id() == buildingTypeId) {
      return type;
    }
  }

  return boost::none;
}

boost::optional<FSSpaceType> FSModel::spaceType(const std::string& spaceTypeId) const {

  for (const auto& spacetype : m_spaceTypes) {
    if (spacetype.id() == spaceTypeId) {
      return spacetype;
    }
  }

  return boost::none;
}

boost::optional<FSConstructionSet> FSModel::constructionSet(const std::string& constructionSetId) const {
  for (const auto& constructionSet : m_constructionSets) {
    if (constructionSet.id() == constructionSetId) {
      return constructionSet;
    }
  }

  return boost::none;
}

boost::optional<FSDaylightingControlDefinition> FSModel::daylightingControlDefinition(const std::string& daylightingControlDefinitionId) const {
  for (const auto& definition : m_daylightingControlDefinitions) {
    if (definition.id() == daylightingControlDefinitionId) {
      return definition;
    }
  }

  return boost::none;
}

boost::optional<FSWindowDefinition> FSModel::windowDefinition(const std::string& windowDefinitionId) const {
  for (const auto& definition : m_windowDefinitions) {
    if (definition.id() == windowDefinitionId) {
      return definition;
    }
  }
  return boost::none;
}

boost::optional<FSDoorDefinition> FSModel::doorDefinition(const std::string& doorDefinitionId) const {
  for (const auto& definition : m_doorDefinitions) {
    if (definition.id() == doorDefinitionId) {
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

std::vector<FSDaylightingControlDefinition> FSModel::daylightingControlDefinitions() const {
  return m_daylightingControlDefinitions;
}

std::vector<FSWindowDefinition> FSModel::windowDefinitions() const {
  return m_windowDefinitions;
}

std::vector<FSDoorDefinition> FSModel::doorDefinitions() const {
  return m_doorDefinitions;
}

std::vector<FSStory> FSModel::stories() const {
  return m_stories;
}

void FSModel::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

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

double FSProject::northAxis() const {
  return m_north_axis;
}

FSConfig& FSProject::config() {
  return m_config;
}

FSGround& FSProject::ground() {
  return m_ground;
}

///////////////////////////////////////////////////////////////////////////////////

FSConfig::FSConfig(const Json::Value& root) {
  m_units = root.get("units", m_units).asString();
  m_unitsEditable = root.get("unitsEditable", m_unitsEditable).asBool();
  m_language = root.get("language", m_language).asString();
  if (root.isMember("north_axis")) {
    m_north_axis = root.get("north_axis", m_north_axis).asDouble();
  }
}

std::string FSConfig::units() const {
  return m_units;
}

std::string FSConfig::language() const {
  return m_language;
}

bool FSConfig::unitsEditable() const {
  return m_unitsEditable;
}

double FSConfig::northAxis() const {
  return m_north_axis;
}
///////////////////////////////////////////////////////////////////////////////////

FSGround::FSGround(const Json::Value& root, const FSModel& model) {

  m_floor_offset = root.get("floor_offset", 0).asDouble() * model.lengthToMeters();
  m_azimuth_angle = root.get("azimuth_angle", 0).asDouble();
  m_tilt_slope = root.get("tilt_slope", 0).asDouble();
}

double FSGround::floorOffset() const {
  return m_floor_offset;
}

double FSGround::azimuthAngle() const {
  return m_azimuth_angle;
}

double FSGround::tiltSlope() const {
  return m_tilt_slope;
}

///////////////////////////////////////////////////////////////////////////////////

FSBase::FSBase(const Json::Value& root) {
  assertKeyAndType(root, "name", Json::stringValue);
  m_name = root.get("name", m_name).asString();
  //assertKeyAndType(root, "handle", Json::stringValue);
  m_handle = root.get("handle", m_handle).asString();
  assertKeyAndType(root, "id", Json::stringValue);
  m_id = root.get("id", m_id).asString();
  //assertKeyAndType(root, "color", Json::stringValue);
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
  for (const auto& json_space : spaces) {
    FSSpace space = FSSpace(json_space, model, *this);
    if (space.face().has_value()) {
      m_spaces.push_back(space);
    }
  }

  const Json::Value& windows = root.get("windows", Json::arrayValue);
  for (const auto& json_window : windows) {
    FSWindow window = FSWindow(json_window, model, *this);
    if (window.edge().has_value()) {
      m_windows.push_back(window);
    }
  }

  const Json::Value& doors = root.get("doors", Json::arrayValue);
  for (const auto& json_door : doors) {
    FSDoor door = FSDoor(json_door, model, *this);
    if (door.edge().has_value()) {
      m_doors.push_back(door);
    }
  }

  const Json::Value& shadings = root.get("shading", Json::arrayValue);
  for (const auto& json_shading : shadings) {
    FSShading shading = FSShading(json_shading, model, *this);
    if (shading.face().has_value()) {
      m_shadings.push_back(shading);
    }
  }
}

double FSStory::belowFloorPlenumHeight() const {
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

const FSGeometry& FSStory::geometry() const {
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

FSAssignment::FSAssignment(const Json::Value& root) : FSBase(root) {}

///////////////////////////////////////////////////////////////////////////////////

FSThermalZone::FSThermalZone(const Json::Value& root) : FSAssignment(root) {}

void FSThermalZone::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSBuildingUnit::FSBuildingUnit(const Json::Value& root) : FSAssignment(root) {}

void FSBuildingUnit::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSBuildingType::FSBuildingType(const Json::Value& root) : FSAssignment(root) {}

void FSBuildingType::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSSpaceType::FSSpaceType(const Json::Value& root) : FSAssignment(root) {}

void FSSpaceType::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSConstructionSet::FSConstructionSet(const Json::Value& root) : FSAssignment(root) {}

void FSConstructionSet::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSSpace::FSSpace(const Json::Value& root, const FSModel& model, FSStory& story)
  : FSBase(root), m_below_floor_plenum_height(story.belowFloorPlenumHeight()) {

  if (checkKeyAndType(root, "thermal_zone_id", Json::stringValue)) {
    m_thermalZone = model.thermalZone(root.get("thermal_zone_id", "").asString());
  }

  if (checkKeyAndType(root, "building_unit_id", Json::stringValue)) {
    m_buildingUnit = model.buildingUnit(root.get("building_unit_id", "").asString());
  }

  if (checkKeyAndType(root, "space_type_id", Json::stringValue)) {
    m_spaceType = model.spaceType(root.get("space_type_id", "").asString());
  }

  if (checkKeyAndType(root, "construction_set_id", Json::stringValue)) {
    m_constructionSet = model.constructionSet(root.get("construction_set_id", "").asString());
  }

  // Heights are optional, if not defined then the value is inherited from the story, if
  // defined then the value overrides the value from the story

  m_below_floor_plenum_height = story.belowFloorPlenumHeight();
  if (checkKeyAndType(root, "below_floor_plenum_height", Json::realValue)) {
    double defaultValue = m_below_floor_plenum_height / model.lengthToMeters();
    m_below_floor_plenum_height = root.get("below_floor_plenum_height", defaultValue).asDouble() * model.lengthToMeters();
  }

  m_floor_to_ceiling_height = story.floorToCeilingHeight();
  if (checkKeyAndType(root, "floor_to_ceiling_height", Json::realValue)) {  // Doing this skips null values for exmaple
    double defaultValue = m_floor_to_ceiling_height / model.lengthToMeters();
    m_floor_to_ceiling_height = root.get("floor_to_ceiling_height", defaultValue).asDouble() * model.lengthToMeters();
  }

  m_above_ceiling_plenum_height = story.aboveCeilingPlenumHeight();
  if (checkKeyAndType(root, "above_ceiling_plenum_height", Json::realValue)) {
    double defaultValue = story.aboveCeilingPlenumHeight() / model.lengthToMeters();
    m_above_ceiling_plenum_height = root.get("above_ceiling_plenum_height", defaultValue).asDouble() * model.lengthToMeters();
  }

  m_offset = root.get("floor_offset", 0).asDouble() * model.lengthToMeters();
  m_openToBelow = root.get("open_to_below", false).asBool();
  m_multiplier = std::max(root.get("multiplier", story.multiplier()).asInt(), 1);

  // Get the face. Note: a space must have a face for it to be added to the model
  if (checkKeyAndType(root, "face_id", Json::stringValue)) {
    std::string face_id = root.get("face_id", "").asString();
    m_face = story.geometry().face(face_id);
  }

  for (const auto& daylightingControl : root.get("daylighting_controls", Json::arrayValue)) {
    m_daylightingControls.emplace_back(daylightingControl, model, story);
  }
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

boost::optional<FSFace> FSSpace::face() const {
  return m_face;
}

int FSSpace::multiplier() const {
  return m_multiplier;
}

double FSSpace::belowFloorPlenumHeight() const {
  return m_below_floor_plenum_height;
}

double FSSpace::floorToCeilingHeight() const {
  return m_floor_to_ceiling_height;
}

double FSSpace::aboveCeilingHeight() const {
  return m_above_ceiling_plenum_height;
}

double FSSpace::offset() const {
  return m_offset;
}

bool FSSpace::openToBelow() const {
  return m_openToBelow;
}

std::vector<FSDaylightingControl> FSSpace::daylightingControls() const {
  return m_daylightingControls;
}

void FSSpace::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

void FSSpace::simplifyFace(const FSGeometry& geometry) {
  Point3dVector faceVertices;
  for (const auto& edgeRef : m_face->edgeRefs()) {
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
        const Point3d& p1 = simplifiedVertices[i];
        const Point3d& p2 = simplifiedVertices[(i + 1) % simplifiedVertices.size()];

        auto v1 = geometry.vertex(p1);
        auto v2 = geometry.vertex(p2);
        if (v1.has_value() && v2.has_value()) {
          //boost::optional<FSEdge> edge = geometry.getEdge(*v1, *v2);
          //if (edge == boost::none)
          //  edge = FSEdge (*v1, *v2);
          //edgeRefs.emplace_back(FSEdgeReference(*edge, 1));
          FSEdge edge(*v1, *v2);
          edgeRefs.emplace_back(std::move(edge), 1);
        }
      }

      // Update the face
      m_face->setEdgeRefs(std::move(edgeRefs));
    }
    // Get a list of windows and doors that were associated with the old edges
    // Remap these windows and doors to the new edges
  }
}

///////////////////////////////////////////////////////////////////////////////////

FSGeometry::FSGeometry(const Json::Value& root, const FSModel& model) {
  load(root, model);
}

void FSGeometry::load(const Json::Value& root, const FSModel& model) {
  FSGeometryBase::load(root);

  const Json::Value& vertices = root.get("vertices", Json::arrayValue);
  for (const auto& vertice : vertices) {
    m_vertices.emplace_back(vertice, model);
  }

  const Json::Value& edges = root.get("edges", Json::arrayValue);
  for (const auto& edge : edges) {
    m_edges.emplace_back(edge, *this);
  }

  const Json::Value& faces = root.get("faces", Json::arrayValue);
  for (const auto& face : faces) {
    m_faces.emplace_back(face, *this);
  }
}

boost::optional<FSVertex> FSGeometry::vertex(const std::string& id) const {

  for (const auto& vertex : m_vertices) {
    if (vertex.id() == id) {
      return vertex;
    }
  }

  return boost::none;
}

boost::optional<FSVertex> FSGeometry::vertex(const Point3d& point) const {
  double tol = 0.01;
  for (const auto& vertex : m_vertices) {
    if (std::abs(vertex.x() - point.x()) < tol && std::abs(vertex.y() - point.y()) < tol) {
      return vertex;
    }
  }

  return boost::none;
}

boost::optional<FSEdge> FSGeometry::edge(const std::string& id) const {

  for (const auto& edge : m_edges) {
    if (edge.id() == id) {
      return edge;
    }
  }

  return boost::none;
}

boost::optional<FSEdge> FSGeometry::edge(const FSVertex& v1, const FSVertex& v2) const {
  for (const auto& edge : m_edges) {
    if ((edge.firstVertex().id() == v1.id() && edge.secondVertex().id() == v2.id())
        || (edge.firstVertex().id() == v2.id() && edge.secondVertex().id() == v1.id())) {
      return edge;
    }
  }

  return boost::none;
}

boost::optional<FSFace> FSGeometry::face(const std::string& id) const {

  for (const auto& face : m_faces) {
    if (face.id() == id) {
      return face;
    }
  }

  return boost::none;
}

///////////////////////////////////////////////////////////////////////////////////

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

FSEdge::FSEdge(const Json::Value& root, const FSGeometry& geometry) {
  load(root, geometry);
}

// cppcheck-suppress constParameterReference
FSEdge::FSEdge(FSVertex& v1, FSVertex& v2) {
  m_vertices.push_back(v1);
  m_vertices.push_back(v2);
}

void FSEdge::load(const Json::Value& root, const FSGeometry& geometry) {

  FSGeometryBase::load(root);
  const Json::Value& vertexIds = root.get("vertex_ids", Json::arrayValue);
  for (const auto& vertexId : vertexIds) {
    m_vertices.push_back(geometry.vertex(vertexId.asString()).value());
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

Vector3d FSEdge::edgeVector() const {
  Point3d pt1(m_vertices[0].x(), m_vertices[0].y(), 0);
  Point3d pt2(m_vertices[1].x(), m_vertices[1].y(), 0);
  return (pt2 - pt1);
}

Point3d FSEdge::vertex(double alpha) const {
  double x = (1.0 - alpha) * m_vertices[0].x() + alpha * m_vertices[1].x();
  double y = (1.0 - alpha) * m_vertices[0].y() + alpha * m_vertices[1].y();
  return {x, y, 0};
}

///////////////////////////////////////////////////////////////////////////////////

FSFace::FSFace(const Json::Value& root, const FSGeometry& geometry) {
  load(root, geometry);
}

void FSFace::load(const Json::Value& root, const FSGeometry& geometry) {

  FSGeometryBase::load(root);
  const Json::Value& edgeIds = root.get("edge_ids", Json::arrayValue);
  const Json::Value& edgeOrders = root.get("edge_order", Json::arrayValue);
  if (edgeIds.size() != edgeOrders.size()) {
    throw openstudio::Exception(std::string("The number or edge orders does not equal the number of edge ids"));
  }

  for (unsigned int i = 0; i < edgeIds.size(); i++) {
    boost::optional<FSEdge> edge = geometry.edge(edgeIds[i].asString()).value();
    m_edgeRefs.emplace_back(FSEdgeReference(*edge, edgeOrders[i].asInt()));
  }
}

std::vector<FSEdgeReference> FSFace::edgeRefs() const {
  return m_edgeRefs;
}

void FSFace::setEdgeRefs(std::vector<FSEdgeReference> edgeRefs) {
  m_edgeRefs = std::move(edgeRefs);
}
///////////////////////////////////////////////////////////////////////////////////

FSEdgeReference::FSEdgeReference(FSEdge edge, int edgeOrder) : m_edge(std::move(edge)), m_edgeOrder(edgeOrder) {}

const FSEdge& FSEdgeReference::edge() const {
  return m_edge;
}

int FSEdgeReference::edgeOrder() const {
  return m_edgeOrder;
}

const FSVertex& FSEdgeReference::getNextVertex() const {
  if (m_edgeOrder == 1) {
    return m_edge.firstVertex();
  } else {
    return m_edge.secondVertex();
  }
}

///////////////////////////////////////////////////////////////////////////////////

FSFiller::FSFiller(const Json::Value& root, const FSStory& story) : FSBase(root) {

  // Alpha can be a single value or a list
  if (root.isMember("alpha") && root["alpha"].isConvertibleTo(Json::realValue)) {
    m_alphas.push_back(root.get("alpha", 0.5).asDouble());
  } else {
    Json::Value alphas = root.get("alpha", Json::arrayValue);
    for (const auto& alpha : alphas) {
      m_alphas.push_back(alpha.asDouble());
    }
  }

  if (checkKeyAndType(root, "edge_id", Json::stringValue)) {
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

Point3d FSFiller::centerVertex(double alpha) const {
  return m_edge->vertex(alpha);
}
///////////////////////////////////////////////////////////////////////////////////

FSWindow::FSWindow(const Json::Value& root, const FSModel& model, FSStory& story) : FSFiller(root, story) {

  if (checkKeyAndType(root, "window_definition_id", Json::stringValue)) {
    std::string windowDefinitionId = root.get("window_definition_id", "").asString();
    m_windowDefinition = model.windowDefinition(windowDefinitionId);
    // If the window definition is window to wall ratios then the windows should be centered
    // on the wall surface so make sure the alpha is 0.5 and then it will be so
    if (m_windowDefinition.has_value()) {
      if (m_windowDefinition->windowDefinitionMode() == "Window to Wall Ratio") {
        m_alphas.clear();
        m_alphas.push_back(0.5);
      }
    }
  }
}

boost::optional<FSWindowDefinition> FSWindow::windowDefinition() const {
  return m_windowDefinition;
}

void FSWindow::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSDoor::FSDoor(const Json::Value& root, const FSModel& model, FSStory& story) : FSFiller(root, story) {

  if (checkKeyAndType(root, "door_definition_id", Json::stringValue)) {
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

FSShading::FSShading(const Json::Value& root, const FSModel& model, const FSStory& story) : FSBase(root) {

  // Get the face. Note: a shading must have a face for it to be added to the model
  if (checkKeyAndType(root, "face_id", Json::stringValue)) {
    std::string face_id = root.get("face_id", "").asString();
    m_face = story.geometry().face(face_id);
  }

  // Heights are optional, if not defined then the value is inherited from the story, if
  // defined then the value overrides the value from the story

  m_below_floor_plenum_height = story.belowFloorPlenumHeight();
  if (checkKeyAndType(root, "below_floor_plenum_height", Json::realValue)) {
    double defaultValue = m_below_floor_plenum_height / model.lengthToMeters();
    m_below_floor_plenum_height = root.get("below_floor_plenum_height", defaultValue).asDouble() * model.lengthToMeters();
  }

  m_floor_to_ceiling_height = story.floorToCeilingHeight();
  if (checkKeyAndType(root, "floor_to_ceiling_height", Json::realValue)) {  // Doing this skips null values for exmaple
    double defaultValue = m_floor_to_ceiling_height / model.lengthToMeters();
    m_floor_to_ceiling_height = root.get("floor_to_ceiling_height", defaultValue).asDouble() * model.lengthToMeters();
  }

  m_above_ceiling_plenum_height = story.aboveCeilingPlenumHeight();
  if (checkKeyAndType(root, "above_ceiling_plenum_height", Json::realValue)) {
    double defaultValue = story.aboveCeilingPlenumHeight() / model.lengthToMeters();
    m_above_ceiling_plenum_height = root.get("above_ceiling_plenum_height", defaultValue).asDouble() * model.lengthToMeters();
  }
}

boost::optional<FSFace> FSShading::face() const {
  return m_face;
}

double FSShading::belowFloorPlenumHeight() const {
  return m_below_floor_plenum_height;
}

double FSShading::floorToCeilingHeight() const {
  return m_floor_to_ceiling_height;
}

double FSShading::aboveCeilingPlenumHeight() const {
  return m_above_ceiling_plenum_height;
}

void FSShading::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

FSDaylightingControl::FSDaylightingControl(const Json::Value& root, const FSModel& model, const FSStory& story) : FSBase(root) {

  if (checkKeyAndType(root, "vertex_id", Json::stringValue)) {
    std::string vertex_id = root.get("vertex_id", "").asString();
    m_vertex = story.geometry().vertex(vertex_id);
  }

  if (checkKeyAndType(root, "daylighting_control_definition_id", Json::stringValue)) {
    std::string control_id = root.get("daylighting_control_definition_id", "").asString();
    m_definition = model.daylightingControlDefinition(control_id);
  }
}

boost::optional<FSVertex> FSDaylightingControl::vertex() const {
  return m_vertex;
}

Point3d FSDaylightingControl::point() const {
  if (m_vertex.has_value() && m_definition.has_value()) {
    return {m_vertex->x(), m_vertex->y(), m_definition->height()};
  }
  return {0.0, 0.0, 0.0};
}

boost::optional<FSDaylightingControlDefinition> FSDaylightingControl::definition() const {
  return m_definition;
}

void FSDaylightingControl::Accept(FSVisitor& visitor) const {
  visitor.Dispatch(*this);
}

///////////////////////////////////////////////////////////////////////////////////

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

FSWindowDefinition::FSWindowDefinition(const Json::Value& root, const FSModel& model) : FSBase(root) {

  // Property can be window_definition_mode or window_definition_type
  if (checkKeyAndType(root, "window_definition_type", Json::stringValue)) {
    m_windowDefinitionMode = root.get("window_definition_type", "Single Window").asString();
  } else {
    assertKeyAndType(root, "window_definition_mode", Json::stringValue);
    m_windowDefinitionMode = root.get("window_definition_mode", "Single Window").asString();
  }
  m_windowType = root.get("window_type", "Fixed").asString();
  if (root.get("window_type", "Fixed").asString() == "Fixed") {
    m_windowType = "FixedWindow";
  } else if (root.get("window_type", "Fixed").asString() == "Operable") {
    m_windowType = "OperableWindow";
  }
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
