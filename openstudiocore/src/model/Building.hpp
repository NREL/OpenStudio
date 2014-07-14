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

#ifndef MODEL_BUILDING_HPP
#define MODEL_BUILDING_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

class Point3d;
class Transformation;

namespace model {

class Facility;
class Meter;
class ShadingSurfaceGroup;
class Surface;
class Space;
class SpaceType;
class DefaultConstructionSet;
class DefaultScheduleSet;
class ThermalZone;

namespace detail {

  class Building_Impl;

} // detail

/** Building derives from ParentObject and is an interface to the OpenStudio IDD object named "Building".
 *
 *  Building is a unique object which parents all \link Space Spaces\endlink in the model.  Conceptually,
 *  the Building object represents the envelope shell and everything inside (lighting, equipment, etc).  
 *  The Building is a child of the Facility object which includes the Building as well as exterior equipment,
 *  parking lot lighting, water systems for grounds, etc.
 *   
 */
class MODEL_API Building : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~Building() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double northAxis() const;

  bool isNorthAxisDefaulted() const;

  double nominalFloortoFloorHeight() const;

  bool isNominalFloortoFloorHeightDefaulted() const;

  boost::optional<int> standardsNumberOfStories() const;

  boost::optional<int> standardsNumberOfAboveGroundStories() const;

  /// Returns the standards building type. This is a freeform field used to identify the building type for standards.
  /// Standards applied to this model will use this field to determine correct levels for lighting, occupancy, etc.
  /// More information can be found at https://github.com/NREL/openstudio-standards.
  boost::optional<std::string> standardsBuildingType() const;

  /// If standardsBuildingType is empty, returns a list of suggestions.  If standardsBuildingType is not empty,
  /// returns standardsBuildingType.
  std::vector<std::string> suggestedStandardsBuildingTypes() const;

  //@}
  /** @name Setters */
  //@{

  void setNorthAxis(double northAxis);

  void resetNorthAxis();

  bool setNominalFloortoFloorHeight(double nominalFloortoFloorHeight);

  void resetNominalFloortoFloorHeight();

  bool setStandardsNumberOfStories(int value);
  void resetStandardsNumberOfStories();

  bool setStandardsNumberOfAboveGroundStories(int value);
  void resetStandardsNumberOfAboveGroundStories();

  /// Sets the standards building type. This is a freeform field used to identify the building type for standards.
  /// Standards applied to this model will use this field to determine correct levels for lighting, occupancy, etc.
  /// More information can be found at https://github.com/NREL/openstudio-standards.
  bool setStandardsBuildingType(const std::string& standardsBuildingType);
  void resetStandardsBuildingType();

  //@}
  /** @name Other */
  //@{

  /// Returns the building's space type.
  boost::optional<SpaceType> spaceType() const;

  /// Sets the building's space type.
  bool setSpaceType(const SpaceType& spaceType);

  /// Resets the building's space type.
  void resetSpaceType();

  /// Returns the default construction set that this space references directly.
  boost::optional<DefaultConstructionSet> defaultConstructionSet() const;

  /// Sets the default construction set for this space directly.
  bool setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet);

  /// Resets the default construction set for this space.
  void resetDefaultConstructionSet();

  /// Returns the default schedule set that this space references directly.
  boost::optional<DefaultScheduleSet> defaultScheduleSet() const;

  /// Sets the default schedule set for this space directly.
  bool setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet);

  /// Resets the default schedule set for this space.
  void resetDefaultScheduleSet();

  /// Returns all Meter objects at the Building level. 
  std::vector<Meter> meters() const;

  /// Returns the parent Facility object if it exists.
  boost::optional<Facility> facility() const;

  // ETH@20140115 - Should take a bool as to whether to include spaces marked as
  // "not in floor area".
  /// Returns all of the \link Space Spaces\endlink in the Building.
  std::vector<Space> spaces() const;

  /// Returns all \link ShadingSurfaceGroup ShadingSurfaceGroups\endlink at the Building level.
  std::vector<ShadingSurfaceGroup> shadingSurfaceGroups() const;

  /// Returns all of the \link ThermalZone ThermalZones\endlink in the Building.
  std::vector<ThermalZone> thermalZones() const;

  /// Returns all \link Surface Surfaces\endlink which are exterior walls.
  std::vector<Surface> exteriorWalls() const;

  /// Returns all \link Surface Surfaces\endlink which are roofs.
  std::vector<Surface> roofs() const;

  // ETH@20140115 - Should take a bool as to whether to include spaces marked as
  // "not in floor area".
  /// Returns the total floor area in square meters.  
  /// Includes only spaces marked as included in floor area.
  /// Includes space multipliers in calculation.
  /// Attribute name: floorArea
  double floorArea() const;

  /// Returns the conditioned floor area in square meters.  Currently requires EnergyPlus
  /// simulation output to calculate.
  /// Attribute name: conditionedFloorArea
  boost::optional<double> conditionedFloorArea() const;

  // ETH@20140115 - Should take a bool as to whether to include spaces marked as
  // "not in floor area".
  /** Returns the total exterior surface area (m^2). Includes space multipliers in
   *  calculation. */
  double exteriorSurfaceArea() const;

  // ETH@20140115 - Should take a bool as to whether to include spaces marked as
  // "not in floor area".
  /** Returns the total exterior wall area (m^2). Includes space multipliers in the
   *  calculation. */
  double exteriorWallArea() const;

  // ETH@20140115 - Should take a bool as to whether to include spaces marked as
  // "not in floor area".
  /** Returns the total air volume (m^3) in the building. Includes space multipliers
   *  in the calculation. */
  double airVolume() const;

  // ETH@20140115 - Should take a bool as to whether to include spaces marked as
  // "not in floor area". (Etc. for the rest of the getters like this.)
  /** Returns the number of people in the building. */
  double numberOfPeople() const;

  /** Returns the number of people per floor area (people/m^2). */
  double peoplePerFloorArea() const;

  /** Returns the total floor area per person in this building (m^2/person). */
  double floorAreaPerPerson() const;

  /** Returns the lighting power (W) in this building. */
  double lightingPower() const;
  
  /** Returns the lighting power density (W/m^2) of this building. */
  double lightingPowerPerFloorArea() const;

  /** Returns the lighting power per person (W/person) of this building. */
  double lightingPowerPerPerson() const;

  /** Returns the electric equipment power (W) of this building. */
  double electricEquipmentPower() const;

  /** Returns the electric equipment power density (W/m^2) of this building. */
  double electricEquipmentPowerPerFloorArea() const;

  /** Returns the electric equipment power per person (W/person) of this building. */
  double electricEquipmentPowerPerPerson() const;

  /** Returns the gas equipment power (W) of this building. */
  double gasEquipmentPower() const;

  /** Returns the gas equipment power density (W/m^2) of this building. */
  double gasEquipmentPowerPerFloorArea() const;

  /** Returns the gas equipment power per person (W/person) of this building. */
  double gasEquipmentPowerPerPerson() const;

  /** Returns the infiltration design flow rate (m^3/s) of this building. Ignores
   *  SpaceInfiltrationEffectiveLeakageArea objects. */
  double infiltrationDesignFlowRate() const;

  /** Returns the infiltration design flow per space floor area (m^3/m^2*s) of this building.
   *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. */
  double infiltrationDesignFlowPerSpaceFloorArea() const;

  /** Returns the infiltration design flow per exterior surface area (m^3/m^2*s) of this building.
   *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. */
  double infiltrationDesignFlowPerExteriorSurfaceArea() const;

  /** Returns the infiltration design flow per exterior wall area (m^3/m^2*s) of this building.
   *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. */
  double infiltrationDesignFlowPerExteriorWallArea() const;

  /** Returns the infiltration design air changes per hour (1/h) of this building.
   *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. */
  double infiltrationDesignAirChangesPerHour() const;

  /// Returns the Transformation from the Building coordinate system to world coordinates.
  Transformation transformation() const;

  /// Returns a suggested pattern for skylights targeting a skylight to floor ratio with desired sizes.
  /// Pattern will be in Building coordinates, on the z = 0 plane, with normal in positive z direction.
  std::vector<std::vector<Point3d> > generateSkylightPattern(double skylightToProjectedFloorRatio, double desiredWidth, double desiredHeight) const;

  //@}
 protected:

  explicit Building(Model& model);

  /// @cond
  typedef detail::Building_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit Building(std::shared_ptr<detail::Building_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.Building");
};

/** \relates Building*/
typedef boost::optional<Building> OptionalBuilding;

/** \relates Building*/
typedef std::vector<Building> BuildingVector;

} // model
} // openstudio

#endif // MODEL_BUILDING_HPP

