/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
class Thermostat;
class ZoneControlContaminantController;
class ZoneControlHumidistat;
class SizingZone;
class PortList;
class ZoneMixing;

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

  boost::optional<Thermostat> thermostat() const;

  boost::optional<ThermostatSetpointDualSetpoint> thermostatSetpointDualSetpoint() const;

  boost::optional<ZoneControlHumidistat> zoneControlHumidistat() const;

  boost::optional<ZoneControlContaminantController> zoneControlContaminantController() const;

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

  bool setCeilingHeight(boost::optional<double> ceilingHeight);

  bool setCeilingHeight(double ceilingHeight);

  bool setCeilingHeight(const Quantity& ceilingHeight);

  void resetCeilingHeight();

  void autocalculateCeilingHeight();

  bool setVolume(boost::optional<double> volume);

  bool setVolume(double volume);

  bool setVolume(const Quantity& volume);

  void resetVolume();

  void autocalculateVolume();

  bool setZoneInsideConvectionAlgorithm(boost::optional<std::string> zoneInsideConvectionAlgorithm);

  bool setZoneInsideConvectionAlgorithm(std::string zoneInsideConvectionAlgorithm);

  void resetZoneInsideConvectionAlgorithm();

  bool setZoneOutsideConvectionAlgorithm(boost::optional<std::string> zoneOutsideConvectionAlgorithm);

  bool setZoneOutsideConvectionAlgorithm(std::string zoneOutsideConvectionAlgorithm);

  void resetZoneOutsideConvectionAlgorithm();

  bool setZoneConditioningEquipmentListName(std::string zoneConditioningEquipmentListName);

  /** \deprecated */
  bool setThermostatSetpointDualSetpoint(const ThermostatSetpointDualSetpoint & thermostat);

  /** \deprecated */
  void resetThermostatSetpointDualSetpoint();

  bool setThermostat(const Thermostat & thermostat);

  void resetThermostat();

  bool setZoneControlHumidistat(const ZoneControlHumidistat & humidistat);

  void resetZoneControlHumidistat();

  bool setZoneControlContaminantController(const ZoneControlContaminantController & contaminantController);

  void resetZoneControlContaminantController();

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

  boost::optional<HVACComponent> airLoopHVACTerminal() const;

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
  bool setUseIdealAirLoads(bool useIdealAirLoads);

  bool addToNode(Node & node);

  PortList inletPortList() const;

  PortList exhaustPortList() const;

  /** Add new equipment setting the heating and cooling priorities
   *  to the next available priority level.
   *  Air terminals associated with AirLoopHVAC will be moved to first priority.
   *  This method is relatively dumb.  It will add any model object to the list
   *  even if it is not hvac equipment.  That might change in the future.
   */
  bool addEquipment(const ModelObject & equipment);

  /** Remove equipment from the EquipmentList.
    * This will not remove the equipment from the model or
    * disconnect any node connections.  Use only if you know what you are doing.
    */
  bool removeEquipment(const ModelObject & equipment);

  /** Set cooling priority of equipment.
   *  Returns false when equipment is not in the ZoneHVACEquipmentList
   */
  bool setCoolingPriority(const ModelObject & equipment, unsigned priority);

  /** Set heating priority of equipment.
   *  Returns false when equipment is not in the ZoneHVACEquipmentList
   */
  bool setHeatingPriority(const ModelObject & euqipment, unsigned priority);

  /** Return all equipment.  Order is determined by heating priority */
  std::vector<ModelObject> equipmentInHeatingOrder();

  /** Return all equipment.  Order is determined by cooling priority */
  std::vector<ModelObject> equipmentInCoolingOrder();

  /** Return true if the ThermalZone is attached to
  *   an AirLoopHVACSupplyPlenum or AirLoopHVACReturnPlenum
  */
  bool isPlenum() const;

  /** Return true if the ThermalZone is unconditioned and available to be used as a plenum
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

  /** Overload of setSupplyPlenum()
    * This variation can account for dual duct systems, branchIndex can be 0 or 1
    * indicating which branch of a dual duct system to attach to.
    * branchIndex 0 corresponds to the branch of demandInletNode(0).
    */
  bool setSupplyPlenum(const ThermalZone & plenumZone, unsigned branchIndex);

  /** Remove any supply plenum serving this zone
  */
  void removeSupplyPlenum();

  /** Overload of removeSupplyPlenum()
    * This variation can account for dual duct systems, branchIndex can be 0 or 1
    * indicating which branch of a dual duct system to attach to.
    * branchIndex 0 corresponds to the branch of demandInletNode(0).
  */
  void removeSupplyPlenum(unsigned branchIndex);

  /** Establish plenumZone as the return plenum for this ThermalZone.
  *   This ThermalZone must already be attached to AirLoopHVAC.
  *   The plenumZone must not be used as a plenum on another AirLoopHVAC structure.
  *   The method canBePlenum called on plenumZone must return true.
  */
  bool setReturnPlenum(const ThermalZone & plenumZone);

  /** Remove any return plenum serving this zone
  */
  void removeReturnPlenum();

  /** Returns all ZoneMixing objects associated with this zone, includes supply and exhaust mixing objects */
  std::vector<ZoneMixing> zoneMixing() const;

  /** Returns all ZoneMixing objects which supply air to this zone */
  std::vector<ZoneMixing> supplyZoneMixing() const;

  /** Returns all ZoneMixing objects which exhaust air from this zone */
  std::vector<ZoneMixing> exhaustZoneMixing() const;

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
