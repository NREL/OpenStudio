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

#ifndef MODEL_SPACE_HPP
#define MODEL_SPACE_HPP

#include "ModelAPI.hpp"
#include "PlanarSurfaceGroup.hpp"

namespace openstudio {
namespace model {

// forward declarations
class SpaceType;
class ConstructionBase;
class DefaultConstructionSet;
class Schedule;
class DefaultScheduleType;
class DefaultScheduleSet;
class ThermalZone;
class BuildingStory;
class ShadingSurfaceGroup;
class InteriorPartitionSurfaceGroup;
class PlanarSurface;
class Surface;
class InternalMass;
class People;
class Lights;
class Luminaire;
class ElectricEquipment;
class GasEquipment;
class HotWaterEquipment;
class SteamEquipment;
class OtherEquipment;
class WaterUseEquipment;
class DaylightingControl;
class IlluminanceMap;
class GlareSensor;
class SpaceInfiltrationDesignFlowRate;
class SpaceInfiltrationEffectiveLeakageArea;
class DesignSpecificationOutdoorAir;

namespace detail {

  class Space_Impl;

} // detail

/** Space is a PlanarSurfaceGroup that wraps the OpenStudio IDD object 'OS_Space'. */
class MODEL_API Space : public PlanarSurfaceGroup {
 public:
  /** @name Constructors and Destructors */
  //@{

  /// Creates a new space in the model, if a building level space type is defined then
  /// the space inherits that by default.
  explicit Space(const Model& model);

  virtual ~Space() {}

  //@}

  static IddObjectType iddObjectType();

  /// Create a space from a floor print and extrude distance.
  static boost::optional<Space> fromFloorPrint(const std::vector<Point3d>& floorPrint, double floorHeight, Model& model);

  /** @name Getters */
  //@{

  bool partofTotalFloorArea() const;

  bool isPartofTotalFloorAreaDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  void setPartofTotalFloorArea(bool partofTotalFloorArea);

  void resetPartofTotalFloorArea();

  //@}
  /** @name Other */

  /// Returns the space type for this space.
  boost::optional<SpaceType> spaceType() const;

  /// Returns true if space type for this space is inherited from the building.
  bool isSpaceTypeDefaulted() const;

  /// Sets the space type for this space.
  bool setSpaceType(const SpaceType& spaceType);

  /// Resets the space type for this space.
  void resetSpaceType();

  /// Returns the default construction set that this space references directly.
  boost::optional<DefaultConstructionSet> defaultConstructionSet() const;

  /// Returns the default construction for this planar surface if available by searching (in order):
  /// This space's default construction set, search distance = 1
  /// This space's space type's default construction set, search distance = 2
  /// This space's building story's default construction set, search distance = 3
  /// This space's building's default construction set, search distance = 4
  /// This space's building's space type's default construction set, search distance = 5
  boost::optional<ConstructionBase> getDefaultConstruction(const PlanarSurface& planarSurface) const;
  boost::optional<std::pair<ConstructionBase, int> > getDefaultConstructionWithSearchDistance(const PlanarSurface& planarSurface) const;

  /// Sets the default construction set for this space directly.
  bool setDefaultConstructionSet(const DefaultConstructionSet& defaultConstructionSet);

  /// Resets the default construction set for this space.
  void resetDefaultConstructionSet();

  /// Returns the default schedule set that this space references directly.
  boost::optional<DefaultScheduleSet> defaultScheduleSet() const;

  /// Returns the default schedule set for the specified type if available by searching (in order):
  /// This space's default schedule set
  /// This space's space type's default schedule set
  /// This space's building story's default schedule set
  /// The building's default schedule set
  /// The building's space type's default schedule set
  boost::optional<Schedule> getDefaultSchedule(const DefaultScheduleType& defaultScheduleType) const;

  /// Sets the default schedule set for this space directly.
  bool setDefaultScheduleSet(const DefaultScheduleSet& defaultScheduleSet);

  /// Resets the default schedule set for this space.
  void resetDefaultScheduleSet();

  // returns the thermal zone if set
  boost::optional<ThermalZone> thermalZone() const;

  // sets the thermal zone
  bool setThermalZone(ThermalZone& thermalZone);

  // resets the thermal zone
  void resetThermalZone();

  // returns the building story if set
  boost::optional<BuildingStory> buildingStory() const;

  // sets the building story
  bool setBuildingStory(const BuildingStory& buildingStory);

  // resets the building story
  void resetBuildingStory();

  /// Returns all \link ShadingSurfaceGroup ShadingSurfaceGroups \endlink in this space.
  std::vector<ShadingSurfaceGroup> shadingSurfaceGroups() const;

  /// Returns all \link InteriorPartitionSurfaceGroup InteriorPartitionSurfaceGroups \endlink in this space.
  std::vector<InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroups() const;

  /// Returns all \link Surface Surfaces \endlink in this space.
  std::vector<Surface> surfaces() const;

  /// Returns all InternalMass in this space.
  std::vector<InternalMass> internalMass() const;

  /// Returns all People in this space.
  std::vector<People> people() const;

  /// Returns all Lights in this space.
  std::vector<Lights> lights() const;

  /// Returns all \link Luminaire Luminaires \endlink in this space.
  std::vector<Luminaire> luminaires() const;

  /// Returns all ElectricEquipment in this space.
  std::vector<ElectricEquipment> electricEquipment() const;

  /// Returns all GasEquipment in this space.
  std::vector<GasEquipment> gasEquipment() const;

  /// Returns all HotWaterEquipment in this space.
  std::vector<HotWaterEquipment> hotWaterEquipment() const;

  /// Returns all SteamEquipment in this space.
  std::vector<SteamEquipment> steamEquipment() const;

  /// Returns all OtherEquipment in this space.
  std::vector<OtherEquipment> otherEquipment() const;  

  /// Returns all WaterUseEquipment in this space.
  std::vector<WaterUseEquipment> waterUseEquipment() const; 

  /// Returns all DaylightingControls in this space.
  std::vector<DaylightingControl> daylightingControls() const;

  /// Returns all IlluminanceMaps in this space.
  std::vector<IlluminanceMap> illuminanceMaps() const;

  /// Returns all GlareSensors in this space.
  std::vector<GlareSensor> glareSensors() const;

  /// Returns all SpaceInfiltrationDesignFlowRate objects in this space.
  std::vector<SpaceInfiltrationDesignFlowRate> spaceInfiltrationDesignFlowRates() const;

  /// Returns all SpaceInfiltrationEffectiveLeakageArea objects in this space.
  std::vector<SpaceInfiltrationEffectiveLeakageArea> spaceInfiltrationEffectiveLeakageAreas() const;

  /// Returns DesignSpecificationOutdoorAir for this space, if not set directly on this space then
  /// the SpaceType's DesignSpecificationOutdoorAir will be returned.
  boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAir() const;

  /// Returns true if DesignSpecificationOutdoorAir is not directly specified for this space.
  bool isDesignSpecificationOutdoorAirDefaulted() const;

  /// Sets DesignSpecificationOutdoorAir for this space.
  bool setDesignSpecificationOutdoorAir(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAir) ;

  /// Resets DesignSpecificationOutdoorAir for this space.
  void resetDesignSpecificationOutdoorAir();

  /// Returns the multiplier for this space, comes from thermal zone, defaults to 1.
  int multiplier() const;

  /// Returns the floor area (m^2).
  /// Does not consider if space is included in building floor area.
  /// Does not include space multiplier in calculation.
  double floorArea() const;

  /// Returns the exterior area (gross area of surfaces with outdoor boundary condition) (m^2).
  /// Does not include space multiplier in calculation.
  double exteriorArea() const;

  /// Returns the exterior wall area (gross area of surfaces with outdoor boundary condition) (m^2).
  /// Does not include space multiplier in calculation.
  double exteriorWallArea() const;

  /// Returns the volume (m^3).
  /// Does not include space multiplier in calculation.
  double volume() const;

  /** Returns the number of people in the space (people). */
  /// Does not include space multiplier in calculation.
  /// Does include people multiplier in calculation.
  double numberOfPeople() const;

  /** Sets the number of people in this space by using people()[0], if it exists, as a template 
   *  for the remaining People and People definition parameters. All other people() in this 
   *  SpaceType will be removed, the SpaceType will be made unique and modified, if necessary, and
   *  the final People object will be attached directly to this Space, not to a SpaceType. */
  bool setNumberOfPeople(double numberOfPeople);

  /** Sets the number of people in this space using templatePeople as a template for the remaining 
   *  People and People definition parameters. All other people() in this SpaceType will be 
   *  removed, the SpaceType will be made unique and modified, if necessary, and the final People 
   *  object will be attached directly to this Space, not to a SpaceType. */
  bool setNumberOfPeople(double numberOfPeople, const People& templatePeople);

  /** Returns the total people per floor area in this space (people/m^2). */
  /// Does not include space multiplier in calculation.
  /// Does include people multiplier in calculation.
  double peoplePerFloorArea() const;

  /** Sets the people per floor area in this space by using people()[0], if it exists, as a 
   *  template for the remaining People and People definition parameters. All other people() in 
   *  this SpaceType will be removed, the SpaceType will be made unique and modified, if 
   *  necessary, and the final People object will be attached directly to this Space, not to a 
   *  SpaceType. */
  bool setPeoplePerFloorArea(double peoplePerFloorArea);

  /** Sets the people per floor area in this space using templatePeople as a template for the 
   *  remaining People and People definition parameters. All other people() in this SpaceType will 
   *  be removed, the SpaceType will be made unique and modified, if necessary, and the final 
   *  People object will be attached directly to this Space, not to a SpaceType. */
  bool setPeoplePerFloorArea(double peoplePerFloorArea, const People& templatePeople);

  /** Returns the total floor area per person in this space (m^2/person). */
  /// Does not include space multiplier in calculation.
  /// Does include people multiplier in calculation.
  double floorAreaPerPerson() const;

  /** Sets the floor area per person in this space by using people()[0], if it exists, as a 
   *  template for the remaining People and People definition parameters. All other people() in 
   *  this SpaceType will be removed, the SpaceType will be made unique and modified, if 
   *  necessary, and the final People object will be attached directly to this Space, not to a 
   *  SpaceType. */
  bool setFloorAreaPerPerson(double floorAreaPerPerson);

  /** Sets the floor area per person in this Space using templatePeople as a template for the 
   *  remaining People and People definition parameters. All other people() in this SpaceType will 
   *  be removed, the SpaceType will be made unique and modified, if necessary, and the final 
   *  People object will be attached directly to this Space, not to a SpaceType. */
  bool setFloorAreaPerPerson(double floorAreaPerPerson, const People& templatePeople);

  /** Returns the lighting power (W) of this space. */
  /// Does not include space multiplier in calculation.
  /// Does include lighting multiplier in calculation.
  double lightingPower() const;

  /** Sets the lighting power in this space by using lights()[0], if it exists, as a template 
   *  for the remaining Lights and Lights Definition parameters. All other lights() and luminaires() 
   *  in this space will be removed, the SpaceType will be made unique and modified, if necessary, and
   *  the final Lights object will be attached directly to this Space, not to a SpaceType. */
  bool setLightingPower(double lightingPower);

  /** Sets the lighting power in this space using templateLights as a template for the remaining 
   *  Lights and Lights Definition parameters. All other lights() and luminaires in this space 
   *  will be removed, the SpaceType will be made unique and modified, if necessary, and the final 
   *  Lights object will be attached directly to this Space, not to a SpaceType. */
  bool setLightingPower(double lightingPower, const Lights& templateLights);

  /** Returns the lighting power density (W/m^2) of this zone. */
  /// Does not include space multiplier in calculation.
  /// Does include lighting multiplier in calculation.
  double lightingPowerPerFloorArea() const;

  /** Sets the lighting power per floor area in this space by using lights()[0], if it exists, as a 
   *  template for the remaining Lights and Lights Definition parameters. All other lights() and 
   *  luminaires() in this space will be removed, the SpaceType will be made unique and modified, if 
   *  necessary, and the final Lights object will be attached directly to this Space, not to a 
   *  SpaceType. */
  bool setLightingPowerPerFloorArea(double lightingPowerPerFloorArea);

  /** Sets the lighting power per floor area in this space using templateLights as a template for 
   *  the remaining Lights and Lights Definition parameters. All other lights() and luminaires() 
   *  in this space will be removed, the SpaceType will be made unique and modified, if necessary, 
   *  and the final Lights object will be attached directly to this Space, not to a SpaceType. */
  bool setLightingPowerPerFloorArea(double lightingPowerPerFloorArea, const Lights& templateLights);

  /** Returns the lighting power per person (W/person) of this zone. */
  /// Does not include space multiplier in calculation.
  /// Does include lighting multiplier in calculation.
  double lightingPowerPerPerson() const;

  /** Sets the lighting power per person in this space by using lights()[0], if it exists, as a 
   *  template for the remaining Lights and Lights Definition parameters. All other lights() and 
   *  luminaires() in this space will be removed, the SpaceType will be made unique and modified, if 
   *  necessary, and the final Lights object will be attached directly to this Space, not to a 
   *  SpaceType. */
  bool setLightingPowerPerPerson(double lightingPowerPerPerson);

  /** Sets the lighting power per person in this space using templateLights as a template for 
   *  the remaining Lights and Lights Definition parameters. All other lights() and luminaires() 
   *  in this space will be removed, the SpaceType will be made unique and modified, if necessary, 
   *  and the final Lights object will be attached directly to this Space, not to a SpaceType. */
  bool setLightingPowerPerPerson(double lightingPowerPerPerson, const Lights& templateLights);

  /** Returns the electric equipment power (W) of this space. */
  /// Does not include space multiplier in calculation.
  /// Does include equipment multiplier in calculation.
  double electricEquipmentPower() const;

  /** Sets the electric equipment power in this space by using electricEquipment()[0], if it 
   *  exists, as a template for the remaining ElectricEquipment and ElectricEquipment Definition 
   *  parameters. All other electricEquipment() in this space will be removed, the SpaceType will 
   *  be made unique and modified, if necessary, and the final ElectricEquipment object will be 
   *  attached directly to this Space, not to a SpaceType. */
  bool setElectricEquipmentPower(double electricEquipmentPower);

  /** Sets the electric equipment power in this space using templateElectricEquipment as a 
   *  template for the remaining ElectricEquipment and ElectricEquipment Definition parameters. 
   *  All other electricEquipment() in this space will be removed, the SpaceType will be made 
   *  unique and modified, if necessary, and the final ElectricEquipment object will be attached 
   *  directly to this Space, not to a SpaceType. */
  bool setElectricEquipmentPower(double electricEquipmentPower, 
                                 const ElectricEquipment& templateElectricEquipment);

  /** Returns the electric equipment power density (W/m^2) of this space. */
  /// Does not include space multiplier in calculation.
  /// Does include equipment multiplier in calculation.
  double electricEquipmentPowerPerFloorArea() const;

  /** Sets the electric equipment power per floor area in this space by using 
   *  electricEquipment()[0], if it exists, as a template for the remaining ElectricEquipment 
   *  and ElectricEquipment Definition parameters. All other electricEquipment() in this space 
   *  will be removed, the SpaceType will be made unique and modified, if necessary, and the 
   *  final ElectricEquipment object will be attached directly to this Space, not to a 
   *  SpaceType. */
  bool setElectricEquipmentPowerPerFloorArea(double electricEquipmentPowerPerFloorArea);

  /** Sets the electric equipment power per floor area in this space using 
   *  templateElectricEquipment as a template for the remaining ElectricEquipment and 
   *  ElectricEquipment Definition parameters. All other electricEquipment() in this space will 
   *  be removed, the SpaceType will be made unique and modified, if necessary, and the final 
   *  ElectricEquipment object will be attached directly to this Space, not to a SpaceType. */
  bool setElectricEquipmentPowerPerFloorArea(double electricEquipmentPowerPerFloorArea, 
                                             const ElectricEquipment& templateElectricEquipment);

  /** Returns the electric equipment power per person (W/person) of this space. */
  /// Does not include space multiplier in calculation.
  /// Does include equipment multiplier in calculation.
  double electricEquipmentPowerPerPerson() const;

  /** Sets the electric equipment power per person in this space by using electricEquipment()[0],
   *  if it exists, as a template for the remaining ElectricEquipment and ElectricEquipment 
   *  Definition parameters. All other electricEquipment() in this space will be removed, the 
   *  SpaceType will be made unique and modified, if necessary, and the final ElectricEquipment 
   *  object will be attached directly to this Space, not to a SpaceType. */
  bool setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson);

  /** Sets the electric equipment power per person in this space using templateElectricEquipment 
   *  as a template for the remaining ElectricEquipment and ElectricEquipment Definition 
   *  parameters. All other electricEquipment() in this space will be removed, the SpaceType will 
   *  be made unique and modified, if necessary, and the final ElectricEquipment object will be 
   *  attached directly to this Space, not to a SpaceType. */
  bool setElectricEquipmentPowerPerPerson(double electricEquipmentPowerPerPerson, 
                                          const ElectricEquipment& templateElectricEquipment);

  /** Returns the gas equipment power (W) of this space. */
  /// Does not include space multiplier in calculation.
  /// Does include equipment multiplier in calculation.
  double gasEquipmentPower() const;

  /** Sets the gas equipment power in this space by using gasEquipment()[0], if it 
   *  exists, as a template for the remaining GasEquipment and GasEquipment Definition 
   *  parameters. All other gasEquipment() in this space will be removed, the SpaceType will 
   *  be made unique and modified, if necessary, and the final GasEquipment object will be 
   *  attached directly to this Space, not to a SpaceType. */
  bool setGasEquipmentPower(double gasEquipmentPower);

  /** Sets the gas equipment power in this space using templateGasEquipment as a 
   *  template for the remaining GasEquipment and GasEquipment Definition parameters. 
   *  All other gasEquipment() in this space will be removed, the SpaceType will be made 
   *  unique and modified, if necessary, and the final GasEquipment object will be attached 
   *  directly to this Space, not to a SpaceType. */
  bool setGasEquipmentPower(double gasEquipmentPower, 
                            const GasEquipment& templateGasEquipment);

  /** Returns the gas equipment power density (W/m^2) of this space. */
  /// Does not include space multiplier in calculation.
  /// Does include equipment multiplier in calculation.
  double gasEquipmentPowerPerFloorArea() const;

  /** Sets the gas equipment power per floor area in this space by using 
   *  gasEquipment()[0], if it exists, as a template for the remaining GasEquipment 
   *  and GasEquipment Definition parameters. All other gasEquipment() in this space 
   *  will be removed, the SpaceType will be made unique and modified, if necessary, and the 
   *  final GasEquipment object will be attached directly to this Space, not to a 
   *  SpaceType. */
  bool setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea);

  /** Sets the gas equipment power per floor area in this space using 
   *  templateGasEquipment as a template for the remaining GasEquipment and 
   *  GasEquipment Definition parameters. All other gasEquipment() in this space will 
   *  be removed, the SpaceType will be made unique and modified, if necessary, and the final 
   *  GasEquipment object will be attached directly to this Space, not to a SpaceType. */
  bool setGasEquipmentPowerPerFloorArea(double gasEquipmentPowerPerFloorArea, 
                                        const GasEquipment& templateGasEquipment);

  /** Returns the gas equipment power per person (W/person) of this space. */
  /// Does not include space multiplier in calculation.
  /// Does include equipment multiplier in calculation.
  double gasEquipmentPowerPerPerson() const;

  /** Sets the gas equipment power per person in this space by using gasEquipment()[0],
   *  if it exists, as a template for the remaining GasEquipment and GasEquipment 
   *  Definition parameters. All other gasEquipment() in this space will be removed, the 
   *  SpaceType will be made unique and modified, if necessary, and the final GasEquipment 
   *  object will be attached directly to this Space, not to a SpaceType. */
  bool setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson);

  /** Sets the gas equipment power per person in this space using templateGasEquipment 
   *  as a template for the remaining GasEquipment and GasEquipment Definition 
   *  parameters. All other gasEquipment() in this space will be removed, the SpaceType will 
   *  be made unique and modified, if necessary, and the final GasEquipment object will be 
   *  attached directly to this Space, not to a SpaceType. */
  bool setGasEquipmentPowerPerPerson(double gasEquipmentPowerPerPerson, 
                                     const GasEquipment& templateGasEquipment);

  /** Returns the infiltration design flow rate (m^3/s) in the space. Ignores
   *  SpaceInfiltrationEffectiveLeakageArea objects. */
  /// Does not include space multiplier in calculation.
  double infiltrationDesignFlowRate() const;

  /** Returns the infiltration design flow per space floor area (m^3/m^2*s) in the space.
   *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. */
  /// Does not include space multiplier in calculation.
  double infiltrationDesignFlowPerSpaceFloorArea() const;

  /** Returns the infiltration design flow per exterior surface area (m^3/m^2*s) in the space.
   *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. */
  /// Does not include space multiplier in calculation.
  double infiltrationDesignFlowPerExteriorSurfaceArea() const;

  /** Returns the infiltration design flow per exterior wall area (m^3/m^2*s) in the space.
   *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. */
  /// Does not include space multiplier in calculation.
  double infiltrationDesignFlowPerExteriorWallArea() const;

  /** Returns the infiltration design air changes per hour (1/h) in the space.
   *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. */
  /// Does not include space multiplier in calculation.
  double infiltrationDesignAirChangesPerHour() const;

  /** The following functionality is used by the EnergyPlus translator, the primary aim
   *  is to preserve information while putting the space into a state where it can easily
   *  be translated to EnergyPlus:
   *    Clone the space type if there is a space type
   *    Hard apply schedules to space type loads if there is a space type
   *    Move space loads from space type to space if there is a space type
   *    Merge space type default constructions with space default constructions if there is a space type
   *    Hard apply schedules to space loads
   *    If hard sizing loads, make each space load refer to unique definition and hard size it
   *    Remove the cloned space type if there is a space type
   */
  void hardApplySpaceType(bool hardSizeLoads);

  /** Set all schedules for child space loads to their default value if there is one. */
  void hardApplySpaceLoadSchedules();

  /** Set all constructions for child surfaces to their default value if there is one. */
  void hardApplyConstructions();

  /** Unmatch any matched surfaces and sub surfaces in this space. */
  void unmatchSurfaces();

  /** Match surfaces and sub surfaces in this space with those in the other. */
  void matchSurfaces(Space& other);

  /** Intersect surfaces in this space with those in the other. */
  void intersectSurfaces(Space& other);

  /** Find surfaces within angular range, specified in degrees and in the site coordinate system, an unset optional means no limit.
      Values for degrees from North are between 0 and 360 and for degrees tilt they are between 0 and 180.
      Note that maxDegreesFromNorth may be less than minDegreesFromNorth,
      e.g. to search between NorthWest and NorthEast set minDegreesFromNorth = 315 and maxDegreesFromNorth = 45.
      The value of maxDegreesTilt must be greater than or equal to minDegreesTilt.  A tolerance of one degree is applied by default.
  */
  std::vector <Surface> findSurfaces(boost::optional<double> minDegreesFromNorth,
                                     boost::optional<double> maxDegreesFromNorth,
                                     boost::optional<double> minDegreesTilt,
                                     boost::optional<double> maxDegreesTilt,
                                     double tol = 1);

  /** Returns the floor print of the space.
      Will return empty vector if all floors in space are not on the same x,y plane.
  */
  std::vector<Point3d> floorPrint() const;

  /** Return true if the ThermalZone is a plenum, otherwise returns false.
  */
  bool isPlenum() const;


  //@}
 protected:

  /// @cond
  typedef detail::Space_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  explicit Space(std::shared_ptr<detail::Space_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.Space");
};

/** Intersect surfaces within spaces. */
MODEL_API void intersectSurfaces(std::vector<Space>& spaces);

/** Match surfaces and sub surfaces within spaces. */
MODEL_API void matchSurfaces(std::vector<Space>& spaces);

/** Un-match surfaces and sub surfaces within spaces. */
MODEL_API void unmatchSurfaces(std::vector<Space>& spaces);

/// Returns a suggested pattern for skylights targeting a skylight to floor ratio with desired sizes.
/// Pattern will be generated in the grid coordinate system, specified by directionOfRelativeNorth. 
/// directionOfRelativeNorth is rotation of grid clockwise from the Building North Axis, in units of degrees.
/// If you want to align skylights with building coordinate system pass directionOfRelativeNorth = 0.  
/// If you want to align skylights with a space's coordinate system pass in that space's directionofRelativeNorth.
/// Resulting pattern will be in Building coordinates, on the z = 0 plane, with normal in positive z direction.
MODEL_API std::vector<std::vector<Point3d> > generateSkylightPattern(const std::vector<Space>& spaces, 
                                                                     double directionOfRelativeNorth,
                                                                     double skylightToProjectedFloorRatio, 
                                                                     double desiredWidth, double desiredHeight);

/** \relates Space*/
typedef boost::optional<Space> OptionalSpace;

/** \relates Space*/
typedef std::vector<Space> SpaceVector;

} // model
} // openstudio

#endif // MODEL_SPACE_HPP

