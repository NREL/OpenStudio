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

#ifndef UTILITIES_FLOORSPACE_FSMODEL_HPP
#define UTILITIES_FLOORSPACE_FSMODEL_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Logger.hpp"
#include <json/json.h>
#include "FSVisitor.hpp"

#include <boost/optional.hpp>

#include <vector>

namespace openstudio {

// forward declaration
class Point3d;
class Vector3d;
class FSModel;

// Base class for all floorspace model items
class UTILITIES_API FSBase : FSDispatchable
{
 public:
  FSBase();
  FSBase(const Json::Value& root);

  const std::string& name() const;
  const std::string& handle() const;
  const std::string& id() const;
  const std::string& color() const;

  void setName(const std::string& name);
  void setHandle(const std::string& handle);
  void setId(const std::string& id);
  void setColor(const std::string& color);

 protected:
  std::string m_name;
  std::string m_handle;
  std::string m_id;
  std::string m_color;
};

// Base class for all geometry items
class UTILITIES_API FSGeometryBase
{
 public:
  FSGeometryBase();
  FSGeometryBase(const Json::Value& root);
  virtual void load(const Json::Value& root);

  const std::string& id() const;

 private:
  std::string m_id;
};

class UTILITIES_API FSVertex : public FSGeometryBase
{
 public:
  FSVertex();
  FSVertex(const Json::Value& root, const FSModel& model);
  void load(const Json::Value& root, const FSModel& model);

  double x() const;
  double y() const;

  void setX(double x);
  void setY(double y);
  void setXY(double x, double y);

 private:
  double m_x;
  double m_y;
};

class FSGeometry;

class UTILITIES_API FSEdge : public FSGeometryBase
{
 public:
  FSEdge();
  FSEdge(const Json::Value& root, const FSGeometry& geometry);
  FSEdge(FSVertex& v1, FSVertex& v2);
  void load(const Json::Value& root, const FSGeometry& geometry);

  std::vector<FSVertex> getVertices() const;
  const FSVertex& firstVertex() const;
  const FSVertex& secondVertex() const;
  const Vector3d& edgeVector() const;
  Point3d vertex(double alpha) const;

 private:
  std::vector<FSVertex> m_vertices;
};

// Reference to an edge by a face, combines the edge and the edge order (direction)
class UTILITIES_API FSEdgeReference
{
 public:
  FSEdgeReference();
  FSEdgeReference(FSEdge& edge, int order);
  const FSEdge& edge() const;
  int edgeOrder() const;
  // Gets the next vertex as determined by the edge order
  const FSVertex& getNextVertex();

 private:
  FSEdge m_edge;
  int m_edgeOrder;
};

class UTILITIES_API FSFace : public FSGeometryBase
{
 public:
  FSFace();
  FSFace(const Json::Value& root, const FSGeometry& geometry);
  void load(const Json::Value& root, const FSGeometry& geometry);
  std::vector<FSEdgeReference> edgeRefs();
  void clearEdgeRefs();
  void setEdgeRefs(std::vector<FSEdgeReference> edgeRefs);

 private:
  std::vector<FSEdgeReference> m_edgeRefs;
};

class UTILITIES_API FSGeometry : public FSGeometryBase
{
 public:
  FSGeometry();
  FSGeometry(const Json::Value& root, const FSModel& model);
  void load(const Json::Value& root, const FSModel& model);

  // gets a vertex by id
  boost::optional<FSVertex> vertex(const std::string& id) const;
  // gets a vertex by coordinates
  boost::optional<FSVertex> vertex(const Point3d& point) const;
  // gets an edge by edge id
  boost::optional<FSEdge> edge(const std::string& id) const;
  // gets an edge by vertices
  boost::optional<FSEdge> edge(const FSVertex& v1, const FSVertex& v2) const;
  // gets a face by id
  boost::optional<FSFace> face(const std::string& id) const;

 private:
  std::vector<FSVertex> m_vertices;
  std::vector<FSEdge> m_edges;
  std::vector<FSFace> m_faces;
};

// Represents things that other things can be assigned to, such as thermal zone, building unit etc
// might not be needed, I thought this woulkd impolement some common property
class UTILITIES_API FSAssignment : public FSBase
{
 public:
  FSAssignment();
  FSAssignment(const Json::Value& root);

 private:
};

class UTILITIES_API FSThermalZone : public FSAssignment
{
 public:
  FSThermalZone();
  FSThermalZone(const Json::Value& root);

  void Accept(FSVisitor& visitor) const;
};

class UTILITIES_API FSBuildingUnit : public FSAssignment
{
 public:
  FSBuildingUnit();
  FSBuildingUnit(const Json::Value& root);

  void Accept(FSVisitor& visitor) const;
};

class UTILITIES_API FSBuildingType : public FSAssignment
{
 public:
  FSBuildingType();
  FSBuildingType(const Json::Value& root);

  void Accept(FSVisitor& visitor) const;
};

class UTILITIES_API FSSpaceType : public FSAssignment
{
 public:
  FSSpaceType();
  FSSpaceType(const Json::Value& root);

  void Accept(FSVisitor& visitor) const;
};

class UTILITIES_API FSConstructionSet : public FSAssignment
{
 public:
  FSConstructionSet();
  FSConstructionSet(const Json::Value& root);

  void Accept(FSVisitor& visitor) const;
};

class FSStory;
class FSSpace;

class UTILITIES_API FSDaylightingControlDefinition : public FSBase
{
 public:
  FSDaylightingControlDefinition();
  FSDaylightingControlDefinition(const Json::Value& root, const FSModel& model);

  void Accept(FSVisitor& visitor) const;

  double illuminanceSetpoint() const;
  double height() const;

 private:
  double m_illuminanceSetpoint;
  double m_height;
};

class UTILITIES_API FSDaylightingControl : public FSBase
{
 public:
  FSDaylightingControl();
  FSDaylightingControl(const Json::Value& root, const FSModel& model, FSStory& story);

  void Accept(FSVisitor& visitor) const;

  boost::optional<FSVertex> vertex() const;
  const Point3d& point() const;
  boost::optional<FSDaylightingControlDefinition> definition() const;

 private:
  boost::optional<FSVertex> m_vertex;
  boost::optional<FSDaylightingControlDefinition> m_definition;
};

class UTILITIES_API FSSpace : public FSBase
{
 public:
  FSSpace();
  FSSpace(const Json::Value& root, const FSModel& model, FSStory& story);
  void Accept(FSVisitor& visitor) const;

  boost::optional<FSThermalZone> thermalZone() const;
  boost::optional<FSBuildingUnit> buildingUnit() const;
  boost::optional<FSSpaceType> spaceType() const;
  boost::optional<FSConstructionSet> constructionSet() const;
  boost::optional<FSFace> face() const;
  int multiplier() const;
  double belowFloorPlenumHeight() const;
  double floorToCeilingHeight() const;
  double aboveCeilingHeight() const;
  double offset() const;
  bool openToBelow() const;
  const std::vector<FSDaylightingControl> daylightingControls() const;

 private:
  boost::optional<FSThermalZone> m_thermalZone;
  boost::optional<FSBuildingUnit> m_buildingUnit;
  boost::optional<FSSpaceType> m_spaceType;
  boost::optional<FSConstructionSet> m_constructionSet;
  boost::optional<FSStory&> m_story;
  boost::optional<FSFace> m_face;

  double m_belowFloorPlenumHeight;
  double m_floorToCeilingHeight;
  double m_aboveCeilingHeight;
  double m_offset;
  bool m_openToBelow;
  int m_multiplier;
  std::vector<FSDaylightingControl> m_daylightingControls;
  void simplifyFace(FSGeometry& geometry);
};

class UTILITIES_API FSWindowDefinition : public FSBase
{
 public:
  FSWindowDefinition();
  FSWindowDefinition(const Json::Value& root, const FSModel& model);
  void Accept(FSVisitor& visitor) const;

  std::string windowDefinitionMode() const;
  double wwr() const;
  double sillHeight() const;
  double windowSpacing() const;
  double height() const;
  double width() const;
  std::string windowType() const;
  double overhangProjectionFactor() const;
  double finProjectionFactor() const;

 private:
  std::string m_windowDefinitionMode;  // Single Window, Repeating Window, Window To Wall Ratio
  double m_wwr;
  double m_sillHeight;
  double m_windowSpacing;
  double m_height;
  double m_width;
  std::string m_windowType;  // Fixed, Operable
  double m_overhangProjectionFactor;
  double m_finProjectionFactor;
};

// Represents something that fills an opening - door or window
class UTILITIES_API FSFiller : public FSBase
{
 public:
  FSFiller();
  FSFiller(const Json::Value& root, const FSModel& model, FSStory& story);

  boost::optional<FSEdge> edge() const;
  boost::optional<FSSpace> space() const;
  std::vector<double> alphas() const;
  // Calculates and returns the center vertex position of the window or door
  Point3d centerVertex(double alpha);

 protected:
  boost::optional<FSEdge> m_edge;
  boost::optional<FSSpace> m_space;
  // For repeating windows alphas is a list otherwise it is a single value
  // not quite sure th ebest way to handle this yet so have both properties
  double m_alpha;
  std::vector<double> m_alphas;
};

class UTILITIES_API FSWindow : public FSFiller
{
 public:
  FSWindow();
  FSWindow(const Json::Value& root, const FSModel& model, FSStory& story);
  void Accept(FSVisitor& visitor) const;

  boost::optional<FSWindowDefinition> windowDefinition() const;

 private:
  boost::optional<FSWindowDefinition> m_windowDefinition;
};

class UTILITIES_API FSDoorDefinition : public FSBase
{
 public:
  FSDoorDefinition();
  FSDoorDefinition(const Json::Value& root, const FSModel& model);

  void Accept(FSVisitor& visitor) const;

  double height() const;
  double width() const;
  const std::string& doorType() const;

 private:
  double m_height;
  double m_width;
  std::string m_doorType;  // Door, Glass Door, Overhead Door
};

class UTILITIES_API FSDoor : public FSFiller
{
 public:
  FSDoor();
  FSDoor(const Json::Value& root, const FSModel& model, FSStory& story);

  void Accept(FSVisitor& visitor) const;
  boost::optional<FSDoorDefinition> doorDefinition() const;

 private:
  boost::optional<FSDoorDefinition> m_doorDefinition;
};

class UTILITIES_API FSShading : public FSBase
{
 public:
  FSShading();
  FSShading(const Json::Value& root, const FSModel& model, FSStory& story);

  void Accept(FSVisitor& visitor) const;

  boost::optional<FSFace> face() const;

 private:
  boost::optional<FSFace> m_face;
};

class UTILITIES_API FSStory : public FSBase
{
 public:
  FSStory();
  FSStory(const Json::Value& root, const FSModel& model);
  void Accept(FSVisitor& visitor) const;

  double gelowFloorPlenumHeight() const;
  double floorToCeilingHeight() const;
  double aboveCeilingPlenumHeight() const;
  double floorToFloorHeight() const;
  double nominalZCoordinate() const;
  int multiplier() const;
  bool imageVisible() const;
  FSGeometry& geometry();
  std::vector<FSSpace> spaces() const;
  std::vector<FSWindow> windows() const;
  std::vector<FSDoor> doors() const;
  std::vector<FSShading> shadings() const;

 private:
  bool m_image_visible;
  double m_below_floor_plenum_height;
  double m_floor_to_ceiling_height;
  double m_above_ceiling_plenum_height;
  double m_nominalZCoordinate;

  int m_multiplier;
  FSGeometry m_geometry;
  std::vector<FSSpace> m_spaces;
  std::vector<FSWindow> m_windows;
  std::vector<FSDoor> m_doors;
  std::vector<FSShading> m_shadings;
};

class UTILITIES_API FSConfig
{
 public:
  FSConfig();
  FSConfig(const Json::Value& root);

  std::string units();
  std::string language();
  bool unitsEditable();
  double northAxis() const;

 private:
  std::string m_units;
  bool m_unitsEditable;
  std::string m_language;
  double m_north_axis;
};

class UTILITIES_API FSGround
{
 public:
  FSGround();
  FSGround(const Json::Value& root, const FSModel& model);

  double floorOffset();
  double azimuthAngle();
  double tiltSlope();

 private:
  double m_floor_offset;
  double m_azimuth_angle;
  double m_tilt_slope;
};

class UTILITIES_API FSProject
{
 public:
  FSProject();
  FSProject(const Json::Value& root, const FSModel& model);
  void load(const Json::Value& root, const FSModel& model);

  double northAxis();
  FSConfig& config();
  FSGround& ground();

 private:
  double m_north_axis;
  FSConfig m_config;
  FSGround m_ground;
};

/** Plane defines an infinite plane in 3D space.  The equation of a plane is
   *  a*x + b*y + c*z = d, any point that satisfies this equation is on the plane.
   */
class UTILITIES_API FSModel
{
 public:
  /// default constructor
  FSModel();
  FSModel(const std::string& json);

  void Accept(FSVisitor& visitor) const;

  void load(const std::string& json);

  std::vector<FSThermalZone> thermalZones() const;
  std::vector<FSBuildingUnit> buildingUnits() const;
  std::vector<FSBuildingType> buildingTypes() const;
  std::vector<FSSpaceType> spaceTypes() const;
  std::vector<FSConstructionSet> constructionSets() const;
  std::vector<FSStory> stories() const;
  const std::vector<FSDaylightingControlDefinition> daylightingControlDefinitions() const;
  const std::vector<FSWindowDefinition> windowDefinitions() const;
  const std::vector<FSDoorDefinition> doorDefinitions() const;

  boost::optional<FSThermalZone> thermalZone(const std::string& thermalZoneId) const;
  boost::optional<FSBuildingUnit> buildingUnit(const std::string& buildingUnitId) const;
  boost::optional<FSBuildingType> buildingType(const std::string& buildingTypeId) const;
  boost::optional<FSSpaceType> spaceType(const std::string& spacetypeId) const;
  boost::optional<FSConstructionSet> constructionSet(const std::string& construtionSetId) const;
  boost::optional<FSDaylightingControlDefinition> daylightingControlDefinition(const std::string& id) const;
  boost::optional<FSWindowDefinition> windowDefinition(const std::string id) const;
  boost::optional<FSDoorDefinition> doorDefinition(const std::string id) const;
  boost::optional<FSProject> project() const;
  double lengthToMeters() const;

  static bool checkKeyAndType(const Json::Value& value, const std::string& key, const Json::ValueType& valueType);
  static void assertKeyAndType(const Json::Value& value, const std::string& key, const Json::ValueType& valueType);
  static void assertKey(const Json::Value& value, const std::string& key);
  static void assertType(const Json::Value& value, const std::string& key, const Json::ValueType& valueType);

 private:
  REGISTER_LOGGER("utilities.FSModel");

  FSProject m_project;
  std::vector<FSThermalZone> m_thermalZones;
  std::vector<FSBuildingUnit> m_buildingUnits;
  std::vector<FSBuildingType> m_buildingTypes;
  std::vector<FSSpaceType> m_spaceTypes;
  std::vector<FSConstructionSet> m_constructionSets;
  std::vector<FSDaylightingControlDefinition> m_daylightingControlDefinitions;
  std::vector<FSWindowDefinition> m_windowDefinitions;
  std::vector<FSDoorDefinition> m_doorDefinitions;
  std::vector<FSStory> m_stories;
  Json::Value m_value;
  double m_lengthToMeters;
  void setUnits();
};

/// ostream operator
//UTILITIES_API std::ostream& operator<<(std::ostream& os, const Plane& plane);

}  // namespace openstudio

#endif
