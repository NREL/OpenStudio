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

#ifndef MODEL_THERMALZONE_HPP
#define MODEL_THERMALZONE_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Space;
class DaylightingControl;
class IlluminanceMap;
class RenderingColor;
class ThermostatSetpointDualSetpoint;
class ZoneControlHumidistat;
class SizingZone;
class PortList;

namespace detail {

  class ThermalZone_Impl;

} // detail

/** ThermalZone is a HVACComponent that wraps the OpenStudio IDD object 'OS_ThermalZone'. */
class MODEL_API ThermalZone : public HVACComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ThermalZone(const Model& model);

  virtual ~ThermalZone() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validZoneInsideConvectionAlgorithmValues();

  static std::vector<std::string> validZoneOutsideConvectionAlgorithmValues();

  /** @name Getters */
  //@{

  int multiplier() const;

  bool isMultiplierDefaulted() const;

  boost::optional<double> ceilingHeight() const;

  OSOptionalQuantity getCeilingHeight(bool returnIP=false) const;

  bool isCeilingHeightDefaulted() const;

  bool isCeilingHeightAutocalculated() const;

  boost::optional<double> volume() const;

  OSOptionalQuantity getVolume(bool returnIP=false) const;

  bool isVolumeDefaulted() const;

  bool isVolumeAutocalculated() const;

  boost::optional<std::string> zoneInsideConvectionAlgorithm() const;

  boost::optional<std::string> zoneOutsideConvectionAlgorithm() const;

  std::string zoneConditioningEquipmentListName() const;

  boost::optional<ThermostatSetpointDualSetpoint> thermostatSetpointDualSetpoint() const;

  boost::optional<ZoneControlHumidistat> zoneControlHumidistat() const;

  double fractionofZoneControlledbyPrimaryDaylightingControl() const;

  Quantity getFractionofZoneControlledbyPrimaryDaylightingControl(bool returnIP=false) const;

  bool isFractionofZoneControlledbyPrimaryDaylightingControlDefaulted() const;

  double fractionofZoneControlledbySecondaryDaylightingControl() const;
  
  Quantity getFractionofZoneControlledbySecondaryDaylightingControl(bool returnIP=false) const;

  bool isFractionofZoneControlledbySecondaryDaylightingControlDefaulted() const;

  SizingZone sizingZone() const;

  //@}
  /** @name Setters */
  //@{

  bool setMultiplier(int multiplier);

  void resetMultiplier();

  void setCeilingHeight(boost::optional<double> ceilingHeight);
  
  void setCeilingHeight(double ceilingHeight);
  
  bool setCeilingHeight(const Quantity& ceilingHeight);

  void resetCeilingHeight();

  void autocalculateCeilingHeight();

  void setVolume(boost::optional<double> volume);

  void setVolume(double volume);
  
  bool setVolume(const Quantity& volume);

  void resetVolume();

  void autocalculateVolume();

  bool setZoneInsideConvectionAlgorithm(boost::optional<std::string> zoneInsideConvectionAlgorithm);

  bool setZoneInsideConvectionAlgorithm(std::string zoneInsideConvectionAlgorithm);

  void resetZoneInsideConvectionAlgorithm();

  bool setZoneOutsideConvectionAlgorithm(boost::optional<std::string> zoneOutsideConvectionAlgorithm);

  bool setZoneOutsideConvectionAlgorithm(std::string zoneOutsideConvectionAlgorithm);

  void resetZoneOutsideConvectionAlgorithm();

  void setZoneConditioningEquipmentListName(std::string zoneConditioningEquipmentListName);

  bool setThermostatSetpointDualSetpoint(const ThermostatSetpointDualSetpoint & thermostat);

  void resetThermostatSetpointDualSetpoint();

  bool setZoneControlHumidistat(const ZoneControlHumidistat & humidistat);

  void resetZoneControlHumidistat();

  bool setFractionofZoneControlledbyPrimaryDaylightingControl(double fractionofZoneControlledbyPrimaryDaylightingControl);
  
  bool setFractionofZoneControlledbyPrimaryDaylightingControl(const Quantity& fractionofZoneControlledbyPrimaryDaylightingControl);

  void resetFractionofZoneControlledbyPrimaryDaylightingControl();

  bool setFractionofZoneControlledbySecondaryDaylightingControl(double fractionofZoneControlledbySecondaryDaylightingControl);
  
  bool setFractionofZoneControlledbySecondaryDaylightingControl(const Quantity& fractionofZoneControlledbySecondaryDaylightingControl);

  void resetFractionofZoneControlledbySecondaryDaylightingControl();

  // TODO: Handle Non-Extensible IddField Space Group Rendering Name.

  //@}
  /** @name Other */
  //@{

  unsigned returnAirPort();

  unsigned zoneAirPort();

  OptionalModelObject returnAirModelObject();

  Node zoneAirNode();

  boost::optional<DaylightingControl> primaryDaylightingControl() const;

  bool setPrimaryDaylightingControl(const DaylightingControl& daylightingControl);

  void resetPrimaryDaylightingControl();

  boost::optional<DaylightingControl> secondaryDaylightingControl() const;

  bool setSecondaryDaylightingControl(const DaylightingControl& daylightingControl);

  void resetSecondaryDaylightingControl();

  boost::optional<IlluminanceMap> illuminanceMap() const;

  bool setIlluminanceMap(const IlluminanceMap& illuminanceMap);

  void resetIlluminanceMap();

  void checkDaylightingControlsAndIlluminanceMaps();

  /// Returns the rendering color.
  boost::optional<RenderingColor> renderingColor() const;

  /// Sets the rendering color.
  bool setRenderingColor(const RenderingColor& renderingColor);

  /// Resets the rendering color.
  void resetRenderingColor();

  std::vector<ModelObject> equipment() const;

  /// returns all spaces in this thermal zone
  std::vector<Space> spaces() const;

  /** Accumulates the floorArea (m^2) of spaces. Does not include space multiplier. */
  double floorArea() const;

  /** Accumulates the exterior surface area (m^2) of spaces. Does not include space
   *  multiplier. */
  double exteriorSurfaceArea() const;

  /** Accumulates the exterior wall area (m^2) of spaces. Does not include space
   *  multiplier. */
  double exteriorWallArea() const;

  // TODO: How should this interact with the volume field. If there is an interaction,
  // how should Building calculate its airVolume and accumulate infiltration design
  // flow rate?
  /** Accumulates the air volume (m^3) of spaces. Does not include space multiplier. */
  double airVolume() const;

  /** Returns the number of people in the thermal zone. Does not include space multiplier. Does include people multiplier. */
  double numberOfPeople() const;

  /** Returns the number of people per floor area (people/m^2). Does not include space multiplier. Does include people multiplier. */
  double peoplePerFloorArea() const;

  /** Returns the total floor area per person in this thermal zone (m^2/person). Does not include space multiplier. Does include people multiplier. */
  double floorAreaPerPerson() const;

  /** Returns the lighting power (W) in this thermal zone. Does not include space multiplier. Does include lighting multiplier. */
  double lightingPower() const;
  
  /** Returns the lighting power density (W/m^2) of this thermal zone. Does not include space multiplier. Does include lighting multiplier. */
  double lightingPowerPerFloorArea() const;

  /** Returns the lighting power per person (W/person) of this thermal zone. Does not include space multiplier. Does include lighting multiplier. */
  double lightingPowerPerPerson() const;

  /** Returns the electric equipment power (W) of this thermal zone. Does not include space multiplier. Does include equipment multiplier. */
  double electricEquipmentPower() const;

  /** Returns the electric equipment power density (W/m^2) of this thermal zone. Does not include space multiplier. Does include equipment multiplier. */
  double electricEquipmentPowerPerFloorArea() const;

  /** Returns the electric equipment power per person (W/person) of this thermal zone. Does not include space multiplier. Does include equipment multiplier. */
  double electricEquipmentPowerPerPerson() const;

  /** Returns the gas equipment power (W) of this thermal zone. Does not include space multiplier. Does include equipment multiplier. */
  double gasEquipmentPower() const;

  /** Returns the gas equipment power density (W/m^2) of this thermal zone. Does not include space multiplier. Does include equipment multiplier. */
  double gasEquipmentPowerPerFloorArea() const;

  /** Returns the gas equipment power per person (W/person) of this thermal zone. Does not include space multiplier. Does include equipment multiplier. */
  double gasEquipmentPowerPerPerson() const;

  /** Returns the infiltration design flow rate (m^3/s) in this thermal zone. Ignores
   *  SpaceInfiltrationEffectiveLeakageArea objects. Does not include space multiplier. */
  double infiltrationDesignFlowRate() const;

  /** Returns the infiltration design flow per space floor area (m^3/m^2*s) in this thermal zone.
   *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. Does not include space multiplier. */
  double infiltrationDesignFlowPerSpaceFloorArea() const;

  /** Returns the infiltration design flow per exterior surface area (m^3/m^2*s) in this thermal zone.
   *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. Does not include space multiplier. */
  double infiltrationDesignFlowPerExteriorSurfaceArea() const;

  /** Returns the infiltration design flow per exterior wall area (m^3/m^2*s) in this thermal zone.
   *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. Does not include space multiplier. */
  double infiltrationDesignFlowPerExteriorWallArea() const;

  /** Returns the infiltration design air changes per hour (1/h) in this thermal zone.
   *  Ignores SpaceInfiltrationEffectiveLeakageArea objects. Does not include space multiplier. */
  double infiltrationDesignAirChangesPerHour() const;

  /** Determines if this zone is conditioned, based on the SqlFile output. Returns 'Yes' if zone is conditioned. */
  boost::optional<std::string> isConditioned() const;

  /// Combines all spaces referencing this zone into a single space referencing this zone.
  /// If this zone has no spaces referencing it, then an uninitialized optional space is returned.
  /// If this zone has one space referencing it, then that space is returned.
  /// If this zone is referenced by more than one space, then geometry from all spaces is added to a single zone.  
  /// The space origin is at the minimum x, y, z coordinate of all space origins, direction of relative north is preserved if same for all spaces.
  /// If all spaces reference the same building story then that is preserved, otherwise it is cleared. 
  /// If all spaces reference the same space type then that is preserved, otherwise space loads from the space type are applied to the new space directly. 
  /// Direct child space loads are converted to absolute levels.  
  /// Constructions and schedules are hard applied to all child surfaces and loads.  
  /// Surfaces referencing other surfaces within the space are converted to interior partitions.  
  boost::optional<Space> combineSpaces();

  /** Removes connections to all other HVACComponent objects */
  void disconnect();

  /** ThermalZone must be disconnected from all loops to be removable
   */
  bool isRemovable() const;

  /** Indicates if the zone will use EnergyPlus ideal air loads.
   */
  bool useIdealAirLoads() const;

  /** Adds or removes ideal air loads for the zone.
   *  ThermalZones that are attached to air loops will be removed.
   *  If the ThermalZone is later added to a loop useIdealAirLoads
   *  will be reset to false.
   */
  void setUseIdealAirLoads(bool useIdealAirLoads);

  bool addToNode(Node & node);

  PortList inletPortList() const;

  PortList exhaustPortList() const;

  /** Add new equipment setting the heating and cooling priorities 
   *  to the next available priority level.
   *  Air terminals associated with AirLoopHVAC will be moved to first priority.
   *  This method is relatively dumb.  It will add any model object to the list
   *  even if it is not hvac equipment.  That might change in the future.
   */
  void addEquipment(const ModelObject & equipment);

  /** Remove equipment from the EquipmentList.
    * This will not remove the equipment from the model or 
    * disconnect any node connections.  Use only if you know what you are doing.
    */
  void removeEquipment(const ModelObject & equipment);

  /** Set cooling priority of equipment.
   *  Asserts when equipment is not in the ZoneHVACEquipmentList
   */
  void setCoolingPriority(const ModelObject & equipment, unsigned priority);

  /** Set heating priority of equipment.
   *  Asserts when equipment is not in the ZoneHVACEquipmentList
   */
  void setHeatingPriority(const ModelObject & euqipment, unsigned priority);

  /** Return all equipment.  Order is determined by heating priority */
  std::vector<ModelObject> equipmentInHeatingOrder();

  /** Return all equipment.  Order is determined by coooling priority */
  std::vector<ModelObject> equipmentInCoolingOrder();

  /** Return true if the ThermalZone is attached to 
  *   an AirLoopHVACSupplyPlenum or AirLoopHVACReturnPlenum
  */
  bool isPlenum() const;

  /** Retrun true if the ThermalZone is unconditioned and available to be used as a plenum
  *   This means the zone is not attached to an AirLoopHVAC structure as a conditioned zone
  *   and there is no zone equipment.
  */
  bool canBePlenum() const;

  /** Establish plenumZone as the supply plenum for this ThermalZone.
  *   This ThermalZone must already be attached to AirLoopHVAC.
  *   The plenumZone must not be used as a plenum on another AirLoopHVAC structure.
  *   The method canBePlenum called on plenumZone must return true.
  */
  bool setSupplyPlenum(const ThermalZone & plenumZone);

  /** Remove any supply plenum serving this zone
  */
  void removeSupplyPlenum();

  /** Establish plenumZone as the return plenum for this ThermalZone.
  *   This ThermalZone must already be attached to AirLoopHVAC.
  *   The plenumZone must not be used as a plenum on another AirLoopHVAC structure.
  *   The method canBePlenum called on plenumZone must return true.
  */
  bool setReturnPlenum(const ThermalZone & plenumZone);

  /** Remove any return plenum serving this zone
  */
  void removeReturnPlenum();

  //@}
 protected:
  /// @cond
  typedef detail::ThermalZone_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  explicit ThermalZone(std::shared_ptr<detail::ThermalZone_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.ThermalZone");
};

/** \relates ThermalZone*/
typedef boost::optional<ThermalZone> OptionalThermalZone;

/** \relates ThermalZone*/
typedef std::vector<ThermalZone> ThermalZoneVector;

} // model
} // openstudio

#endif // MODEL_THERMALZONE_HPP

