/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
class UTILITIES_API FSBase : public FSDispatchable
{
 public:
  explicit FSBase() = default;
  explicit FSBase(const Json::Value& root);

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
  explicit FSGeometryBase() = default;
  explicit FSGeometryBase(const Json::Value& root);
  void load(const Json::Value& root);

  const std::string& id() const;

 private:
  std::string m_id;
};

class UTILITIES_API FSVertex : public FSGeometryBase
{
 public:
  explicit FSVertex(const Json::Value& root, const FSModel& model);
  void load(const Json::Value& root, const FSModel& model);

  double x() const;
  double y() const;

  void setX(double x);
  void setY(double y);
  void setXY(double x, double y);

 private:
  double m_x = 0.0;
  double m_y = 0.0;
};

class FSGeometry;

class UTILITIES_API FSEdge : public FSGeometryBase
{
 public:
  explicit FSEdge(const Json::Value& root, const FSGeometry& geometry);
  explicit FSEdge(FSVertex& v1, FSVertex& v2);
  void load(const Json::Value& root, const FSGeometry& geometry);

  std::vector<FSVertex> getVertices() const;
  const FSVertex& firstVertex() const;
  const FSVertex& secondVertex() const;
  Vector3d edgeVector() const;
  Point3d vertex(double alpha) const;

 private:
  std::vector<FSVertex> m_vertices;
};

// Reference to an edge by a face, combines the edge and the edge order (direction)
class UTILITIES_API FSEdgeReference
{
 public:
  explicit FSEdgeReference(FSEdge edge, int edgeOrder);
  const FSEdge& edge() const;
  int edgeOrder() const;
  // Gets the next vertex as determined by the edge order
  const FSVertex& getNextVertex() const;

 private:
  FSEdge m_edge;
  int m_edgeOrder = 1;
};

class UTILITIES_API FSFace : public FSGeometryBase
{
 public:
  explicit FSFace(const Json::Value& root, const FSGeometry& geometry);
  void load(const Json::Value& root, const FSGeometry& geometry);
  std::vector<FSEdgeReference> edgeRefs() const;
  void setEdgeRefs(std::vector<FSEdgeReference> edgeRefs);

 private:
  std::vector<FSEdgeReference> m_edgeRefs;
};

class UTILITIES_API FSGeometry : public FSGeometryBase
{
 public:
  explicit FSGeometry() = default;
  explicit FSGeometry(const Json::Value& root, const FSModel& model);
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
  explicit FSAssignment() = default;
  explicit FSAssignment(const Json::Value& root);

 private:
};

class UTILITIES_API FSThermalZone : public FSAssignment
{
 public:
  explicit FSThermalZone(const Json::Value& root);

  void Accept(FSVisitor& visitor) const override;
};

class UTILITIES_API FSBuildingUnit : public FSAssignment
{
 public:
  explicit FSBuildingUnit(const Json::Value& root);

  void Accept(FSVisitor& visitor) const override;
};

class UTILITIES_API FSBuildingType : public FSAssignment
{
 public:
  explicit FSBuildingType(const Json::Value& root);

  void Accept(FSVisitor& visitor) const override;
};

class UTILITIES_API FSSpaceType : public FSAssignment
{
 public:
  explicit FSSpaceType(const Json::Value& root);

  void Accept(FSVisitor& visitor) const override;
};

class UTILITIES_API FSConstructionSet : public FSAssignment
{
 public:
  explicit FSConstructionSet(const Json::Value& root);

  void Accept(FSVisitor& visitor) const override;
};

class FSStory;
class FSSpace;

class UTILITIES_API FSDaylightingControlDefinition : public FSBase
{
 public:
  explicit FSDaylightingControlDefinition(const Json::Value& root, const FSModel& model);

  void Accept(FSVisitor& visitor) const override;

  double illuminanceSetpoint() const;
  double height() const;

 private:
  double m_illuminanceSetpoint = 300.0;
  double m_height = 0.0;
};

class UTILITIES_API FSDaylightingControl : public FSBase
{
 public:
  explicit FSDaylightingControl(const Json::Value& root, const FSModel& model, const FSStory& story);

  void Accept(FSVisitor& visitor) const override;

  boost::optional<FSVertex> vertex() const;
  boost::optional<FSDaylightingControlDefinition> definition() const;
  Point3d point() const;

 private:
  boost::optional<FSVertex> m_vertex;
  boost::optional<FSDaylightingControlDefinition> m_definition;
};

class UTILITIES_API FSSpace : public FSBase
{
 public:
  explicit FSSpace(const Json::Value& root, const FSModel& model, FSStory& story);
  void Accept(FSVisitor& visitor) const override;

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
  std::vector<FSDaylightingControl> daylightingControls() const;

 private:
  boost::optional<FSThermalZone> m_thermalZone;
  boost::optional<FSBuildingUnit> m_buildingUnit;
  boost::optional<FSSpaceType> m_spaceType;
  boost::optional<FSConstructionSet> m_constructionSet;
  boost::optional<FSFace> m_face;

  double m_below_floor_plenum_height = 0.0;
  double m_floor_to_ceiling_height = 0.0;
  double m_above_ceiling_plenum_height = 0.0;
  double m_offset = 0.0;
  bool m_openToBelow = false;
  int m_multiplier = 1;
  std::vector<FSDaylightingControl> m_daylightingControls;
  void simplifyFace(const FSGeometry& geometry);
};

class UTILITIES_API FSWindowDefinition : public FSBase
{
 public:
  explicit FSWindowDefinition(const Json::Value& root, const FSModel& model);
  void Accept(FSVisitor& visitor) const override;

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
  std::string m_windowType;            // Fixed, Operable
  double m_wwr = 0.0;
  double m_sillHeight = 0.0;
  double m_windowSpacing = 0.0;
  double m_height = 0.0;
  double m_width = 0.0;
  double m_overhangProjectionFactor = 0.0;
  double m_finProjectionFactor = 0.0;
};

// Represents something that fills an opening - door or window
class UTILITIES_API FSFiller : public FSBase
{
 public:
  explicit FSFiller() = default;
  explicit FSFiller(const Json::Value& root, const FSStory& story);

  boost::optional<FSEdge> edge() const;
  boost::optional<FSSpace> space() const;
  std::vector<double> alphas() const;
  // Calculates and returns the center vertex position of the window or door
  Point3d centerVertex(double alpha) const;

 protected:
  boost::optional<FSEdge> m_edge;
  boost::optional<FSSpace> m_space;
  // For repeating windows alphas is a list otherwise it is a single value
  // not quite sure th ebest way to handle this yet so have both properties
  double m_alpha = 0.5;
  std::vector<double> m_alphas;
};

class UTILITIES_API FSWindow : public FSFiller
{
 public:
  explicit FSWindow(const Json::Value& root, const FSModel& model, FSStory& story);
  void Accept(FSVisitor& visitor) const override;

  boost::optional<FSWindowDefinition> windowDefinition() const;

 private:
  boost::optional<FSWindowDefinition> m_windowDefinition;
};

class UTILITIES_API FSDoorDefinition : public FSBase
{
 public:
  explicit FSDoorDefinition(const Json::Value& root, const FSModel& model);

  void Accept(FSVisitor& visitor) const override;

  double height() const;
  double width() const;
  const std::string& doorType() const;

 private:
  double m_height = 2.0;
  double m_width = 1.0;
  std::string m_doorType;  // Door, Glass Door, Overhead Door
};

class UTILITIES_API FSDoor : public FSFiller
{
 public:
  explicit FSDoor(const Json::Value& root, const FSModel& model, FSStory& story);

  void Accept(FSVisitor& visitor) const override;
  boost::optional<FSDoorDefinition> doorDefinition() const;

 private:
  boost::optional<FSDoorDefinition> m_doorDefinition;
};

class UTILITIES_API FSShading : public FSBase
{
 public:
  explicit FSShading(const Json::Value& root, const FSModel& model, const FSStory& story);
  double belowFloorPlenumHeight() const;
  double floorToCeilingHeight() const;
  double aboveCeilingPlenumHeight() const;
  void Accept(FSVisitor& visitor) const override;
  boost::optional<FSFace> face() const;

 private:
  boost::optional<FSFace> m_face;
  double m_floor_to_ceiling_height = 0.0;
  double m_below_floor_plenum_height = 0.0;
  double m_above_ceiling_plenum_height = 0.0;
};

class UTILITIES_API FSStory : public FSBase
{
 public:
  explicit FSStory(const Json::Value& root, const FSModel& model);
  void Accept(FSVisitor& visitor) const override;

  double belowFloorPlenumHeight() const;
  double floorToCeilingHeight() const;
  double aboveCeilingPlenumHeight() const;
  double floorToFloorHeight() const;
  double nominalZCoordinate() const;
  int multiplier() const;
  bool imageVisible() const;
  const FSGeometry& geometry() const;
  std::vector<FSSpace> spaces() const;
  std::vector<FSWindow> windows() const;
  std::vector<FSDoor> doors() const;
  std::vector<FSShading> shadings() const;

 private:
  bool m_image_visible = false;
  int m_multiplier = 1;

  double m_below_floor_plenum_height = 0.0;
  double m_floor_to_ceiling_height = 3.0;
  double m_above_ceiling_plenum_height = 0.0;
  double m_nominalZCoordinate = 0.0;

  FSGeometry m_geometry;
  std::vector<FSSpace> m_spaces;
  std::vector<FSWindow> m_windows;
  std::vector<FSDoor> m_doors;
  std::vector<FSShading> m_shadings;
};

class UTILITIES_API FSConfig
{
 public:
  explicit FSConfig() = default;
  explicit FSConfig(const Json::Value& root);

  std::string units() const;
  std::string language() const;
  bool unitsEditable() const;
  double northAxis() const;

 private:
  std::string m_units;
  bool m_unitsEditable = false;
  std::string m_language;
  double m_north_axis = 0.0;
};

class UTILITIES_API FSGround
{
 public:
  explicit FSGround() = default;
  explicit FSGround(const Json::Value& root, const FSModel& model);

  double floorOffset() const;
  double azimuthAngle() const;
  double tiltSlope() const;

 private:
  double m_floor_offset = 0.0;
  double m_azimuth_angle = 0.0;
  double m_tilt_slope = 0.0;
};

class UTILITIES_API FSProject
{
 public:
  explicit FSProject() = default;
  explicit FSProject(const Json::Value& root, const FSModel& model);
  void load(const Json::Value& root, const FSModel& model);

  double northAxis() const;
  FSConfig& config();
  FSGround& ground();

 private:
  double m_north_axis = 0.0;
  FSConfig m_config;
  FSGround m_ground;
};

class UTILITIES_API FSModel
{
 public:
  /// default constructor
  explicit FSModel() = default;
  explicit FSModel(const std::string& json);

  void Accept(FSVisitor& visitor) const;

  void load(const std::string& json);

  std::vector<FSThermalZone> thermalZones() const;
  std::vector<FSBuildingUnit> buildingUnits() const;
  std::vector<FSBuildingType> buildingTypes() const;
  std::vector<FSSpaceType> spaceTypes() const;
  std::vector<FSConstructionSet> constructionSets() const;
  std::vector<FSStory> stories() const;
  std::vector<FSDaylightingControlDefinition> daylightingControlDefinitions() const;
  std::vector<FSWindowDefinition> windowDefinitions() const;
  std::vector<FSDoorDefinition> doorDefinitions() const;

  boost::optional<FSThermalZone> thermalZone(const std::string& thermalZoneId) const;
  boost::optional<FSBuildingUnit> buildingUnit(const std::string& buildingUnitId) const;
  boost::optional<FSBuildingType> buildingType(const std::string& buildingTypeId) const;
  boost::optional<FSSpaceType> spaceType(const std::string& spaceTypeId) const;
  boost::optional<FSConstructionSet> constructionSet(const std::string& constructionSetId) const;
  boost::optional<FSDaylightingControlDefinition> daylightingControlDefinition(const std::string& daylightingControlDefinitionId) const;
  boost::optional<FSWindowDefinition> windowDefinition(const std::string& windowDefinitionId) const;
  boost::optional<FSDoorDefinition> doorDefinition(const std::string& doorDefinitionId) const;
  boost::optional<FSProject> project() const;
  double lengthToMeters() const;

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
  double m_lengthToMeters = 1.0;
  void setUnits();
};

}  // namespace openstudio

#endif
