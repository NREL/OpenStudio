/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_THERMALZONE_HPP
#define MODEL_THERMALZONE_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

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
  class AirflowNetworkZone;
  class ZonePropertyUserViewFactorsBySurfaceName;
  class Schedule;

  namespace detail {

    class ThermalZone_Impl;

  }  // namespace detail

  /** ThermalZone is a HVACComponent that wraps the OpenStudio IDD object 'OS_ThermalZone'. */
  class MODEL_API ThermalZone : public HVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ThermalZone(const Model& model);

    virtual ~ThermalZone() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ThermalZone(const ThermalZone& other) = default;
    ThermalZone(ThermalZone&& other) = default;
    ThermalZone& operator=(const ThermalZone&) = default;
    ThermalZone& operator=(ThermalZone&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validZoneInsideConvectionAlgorithmValues();

    static std::vector<std::string> validZoneOutsideConvectionAlgorithmValues();

    /** @name Getters */
    //@{

    int multiplier() const;

    bool isMultiplierDefaulted() const;

    boost::optional<double> ceilingHeight() const;

    bool isCeilingHeightDefaulted() const;

    bool isCeilingHeightAutocalculated() const;

    boost::optional<double> volume() const;

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

    bool isFractionofZoneControlledbyPrimaryDaylightingControlDefaulted() const;

    double fractionofZoneControlledbySecondaryDaylightingControl() const;

    bool isFractionofZoneControlledbySecondaryDaylightingControlDefaulted() const;

    SizingZone sizingZone() const;

    ZonePropertyUserViewFactorsBySurfaceName getZonePropertyUserViewFactorsBySurfaceName() const;

    //@}
    /** @name Setters */
    //@{

    bool setMultiplier(int multiplier);

    void resetMultiplier();

    bool setCeilingHeight(boost::optional<double> ceilingHeight);

    bool setCeilingHeight(double ceilingHeight);

    void resetCeilingHeight();

    void autocalculateCeilingHeight();

    bool setVolume(boost::optional<double> volume);

    bool setVolume(double volume);

    void resetVolume();

    void autocalculateVolume();

    bool setZoneInsideConvectionAlgorithm(boost::optional<std::string> zoneInsideConvectionAlgorithm);

    bool setZoneInsideConvectionAlgorithm(const std::string& zoneInsideConvectionAlgorithm);

    void resetZoneInsideConvectionAlgorithm();

    bool setZoneOutsideConvectionAlgorithm(boost::optional<std::string> zoneOutsideConvectionAlgorithm);

    bool setZoneOutsideConvectionAlgorithm(const std::string& zoneOutsideConvectionAlgorithm);

    void resetZoneOutsideConvectionAlgorithm();

    bool setZoneConditioningEquipmentListName(const std::string& zoneConditioningEquipmentListName);

    /** \deprecated */
    bool setThermostatSetpointDualSetpoint(const ThermostatSetpointDualSetpoint& thermostat);

    /** \deprecated */
    void resetThermostatSetpointDualSetpoint();

    bool setThermostat(const Thermostat& thermostat);

    void resetThermostat();

    bool setZoneControlHumidistat(const ZoneControlHumidistat& humidistat);

    void resetZoneControlHumidistat();

    bool setZoneControlContaminantController(const ZoneControlContaminantController& contaminantController);

    void resetZoneControlContaminantController();

    bool setFractionofZoneControlledbyPrimaryDaylightingControl(double fractionofZoneControlledbyPrimaryDaylightingControl);

    void resetFractionofZoneControlledbyPrimaryDaylightingControl();

    bool setFractionofZoneControlledbySecondaryDaylightingControl(double fractionofZoneControlledbySecondaryDaylightingControl);

    void resetFractionofZoneControlledbySecondaryDaylightingControl();

    // TODO: Handle Non-Extensible IddField Space Group Rendering Name.

    //@}
    /** @name Other */
    //@{

    // As of OS Version 2.6.1 this method returns the first port on the returnPortList
    // because multiple return air ports (and AirLoopHVAC instances) are allowed
    unsigned returnAirPort() const;

    unsigned zoneAirPort() const;

    OptionalModelObject returnAirModelObject() const;

    std::vector<ModelObject> returnAirModelObjects() const;

    Node zoneAirNode() const;

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

    boost::optional<Schedule> daylightingControlsAvailabilitySchedule() const;

    bool setDaylightingControlsAvailabilitySchedule(Schedule& schedule);

    void resetDaylightingControlsAvailabilitySchedule();

    /// Returns the rendering color.
    boost::optional<RenderingColor> renderingColor() const;

    /// Sets the rendering color.
    bool setRenderingColor(const RenderingColor& renderingColor);

    /// Resets the rendering color.
    void resetRenderingColor();

    std::vector<ModelObject> equipment() const;

    boost::optional<HVACComponent> airLoopHVACTerminal() const;

    std::vector<HVACComponent> airLoopHVACTerminals() const;

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

    /** Returns the infiltration design flow rate (m^3/s) in this thermal zone.
     *  Ignores SpaceInfiltrationEffectiveLeakageArea and SpaceInfiltrationFlowCoefficient objects. Does not include space multiplier. */
    double infiltrationDesignFlowRate() const;

    /** Returns the infiltration design flow per space floor area (m^3/m^2*s) in this thermal zone.
     *  Ignores SpaceInfiltrationEffectiveLeakageArea and SpaceInfiltrationFlowCoefficient objects. Does not include space multiplier. */
    double infiltrationDesignFlowPerSpaceFloorArea() const;

    /** Returns the infiltration design flow per exterior surface area (m^3/m^2*s) in this thermal zone.
     *  Ignores SpaceInfiltrationEffectiveLeakageArea and SpaceInfiltrationFlowCoefficient objects. Does not include space multiplier. */
    double infiltrationDesignFlowPerExteriorSurfaceArea() const;

    /** Returns the infiltration design flow per exterior wall area (m^3/m^2*s) in this thermal zone.
     *  Ignores SpaceInfiltrationEffectiveLeakageArea and SpaceInfiltrationFlowCoefficient objects. Does not include space multiplier. */
    double infiltrationDesignFlowPerExteriorWallArea() const;

    /** Returns the infiltration design air changes per hour (1/h) in this thermal zone.
     *  Ignores SpaceInfiltrationEffectiveLeakageArea and SpaceInfiltrationFlowCoefficient objects. Does not include space multiplier. */
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
    void disconnect();  // cppcheck-suppress [duplInheritedMember] for documentation purposes

    /** ThermalZone must be disconnected from all loops to be removable
    */
    bool isRemovable() const;  // cppcheck-suppress [duplInheritedMember] for documentation purposes

    /** Indicates if the zone will use EnergyPlus ideal air loads.
    */
    bool useIdealAirLoads() const;

    /** Adds or removes ideal air loads for the zone.
     *  ThermalZones that are attached to air loops will be removed.
     *  If the ThermalZone is later added to a loop useIdealAirLoads
     *  will be reset to false.
     */
    bool setUseIdealAirLoads(bool useIdealAirLoads);

    bool addToNode(Node& node);  // cppcheck-suppress [duplInheritedMember] for documentation purposes

    /** This method is the same as addToNode, except
     *  existing air loop connections will not be removed.
     *  This is because EnergyPlus gained the ability to attach multiple air loops.
     **/
    bool multiAddToNode(Node& node);

    PortList returnPortList() const;

    PortList inletPortList() const;

    PortList exhaustPortList() const;

    /** Add new equipment setting the heating and cooling priorities
     *  to the next available priority level.
     *  Air terminals associated with AirLoopHVAC will be moved to first priority.
     *  This method is relatively dumb.  It will add any model object to the list
     *  even if it is not hvac equipment.  That might change in the future.
     */
    bool addEquipment(const ModelObject& equipment);

    /** Remove equipment from the EquipmentList.
    * This will not remove the equipment from the model or
    * disconnect any node connections.  Use only if you know what you are doing.
    */
    bool removeEquipment(const ModelObject& equipment);

    std::string loadDistributionScheme() const;

    bool setLoadDistributionScheme(const std::string& scheme);

    /** Set cooling priority of equipment.
     *  Returns false when equipment is not in the ZoneHVACEquipmentList
     */
    bool setCoolingPriority(const ModelObject& equipment, unsigned priority);

    /** Set heating priority of equipment.
     *  Returns false when equipment is not in the ZoneHVACEquipmentList
     */
    bool setHeatingPriority(const ModelObject& equipment, unsigned priority);

    /** Return all equipment.  Order is determined by heating priority */
    std::vector<ModelObject> equipmentInHeatingOrder() const;

    /** Return all equipment.  Order is determined by cooling priority */
    std::vector<ModelObject> equipmentInCoolingOrder() const;

    /** Return the Sequential Cooling Fraction of equipment, if it's a ScheduleConstant.
     *  Returns nothing if when equipment is not in the ZoneHVACEquipmentList, its heating priority is zero,
     *  or the loadDistributionScheme isn't 'Sequential'
     */
    boost::optional<double> sequentialCoolingFraction(const ModelObject& equipment) const;

    /** Return the Sequential Cooling Fraction Schedule of equipment.
     *  Returns nothing if when equipment is not in the ZoneHVACEquipmentList, its heating priority is zero,
     *  or the loadDistributionScheme isn't 'Sequential'
     */
    boost::optional<Schedule> sequentialCoolingFractionSchedule(const ModelObject& equipment) const;

    /** Return the Sequential Heating Fraction of equipment, if it's a ScheduleConstant
     *  Returns nothing if when equipment is not in the ZoneHVACEquipmentList, its cooling priority is zero,
     *  or the loadDistributionScheme isn't 'Sequential'
     */
    boost::optional<double> sequentialHeatingFraction(const ModelObject& equipment) const;

    /** Return the Sequential Heating Fraction Schedule of equipment.
     *  Returns nothing if when equipment is not in the ZoneHVACEquipmentList, its cooling priority is zero,
     *  or the loadDistributionScheme isn't 'Sequential'
     */
    boost::optional<Schedule> sequentialHeatingFractionSchedule(const ModelObject& equipment) const;

    /** Set the Sequential Cooling Fraction of equipment, creates a ScheduleConstant for your convenience.
     *  Returns false when equipment is not in the ZoneHVACEquipmentList, its cooling priority is zero,
     *  or the loadDistributionScheme isn't 'Sequential'
     */
    bool setSequentialCoolingFraction(const ModelObject& equipment, double fraction);

    /** Set the Sequential Cooling Fraction Schedule of equipment.
     *  Returns false when equipement is not in the ZoneHVACEquipmentList, its cooling priority is zero,
     *  or the loadDistributionScheme isn't 'Sequential'
     */
    bool setSequentialCoolingFractionSchedule(const ModelObject& equipment, Schedule& schedule);

    /** Set the Sequential Heating Fraction of equipment, creates a ScheduleConstant for your convenience.
     *  Returns false when equipment is not in the ZoneHVACEquipmentList, its heating priority is zero,
     *  or the loadDistributionScheme isn't 'Sequential'
     */
    bool setSequentialHeatingFraction(const ModelObject& equipment, double fraction);

    /** Set the Sequential Heating Fraction Schedule of equipment.
     *  Returns false when equipment is not in the ZoneHVACEquipmentList, its heating priority is zero,
     *  or the loadDistributionScheme isn't 'Sequential'
     */
    bool setSequentialHeatingFractionSchedule(const ModelObject& equipment, Schedule& schedule);

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
    bool setSupplyPlenum(const ThermalZone& plenumZone);

    /** Overload of setSupplyPlenum()
    * This variation can account for dual duct systems, branchIndex can be 0 or 1
    * indicating which branch of a dual duct system to attach to.
    * branchIndex 0 corresponds to the branch of demandInletNode(0).
    */
    bool setSupplyPlenum(const ThermalZone& plenumZone, unsigned branchIndex);

    /** Remove any supply plenum serving this zone,
     * associated with the AirLoopHVAC returned by
     * ThermalZone::airLoopHVAC().
  */
    void removeSupplyPlenum();

    /** Remove any supply plenum associated with
     * the given AirLoopHVAC instance.
     * This method is important when a zone is connected to multiple AirLoopHVAC instances.
     */
    void removeSupplyPlenum(const AirLoopHVAC& airloop);

    /** Overload of removeSupplyPlenum()
    * This variation can account for dual duct systems, branchIndex can be 0 or 1
    * indicating which branch of a dual duct system to attach to.
    * branchIndex 0 corresponds to the branch of demandInletNode(0).
  */
    void removeSupplyPlenum(unsigned branchIndex);

    /** Remove any supply plenum associated with
     * the given AirLoopHVAC instance, and branchIndex in a dual duct system.
     *  This method is important when a zone is connected to multiple AirLoopHVAC instances.
     * This variation can account for dual duct systems, branchIndex can be 0 or 1
     * indicating which branch of a dual duct system to attach to.
     * branchIndex 0 corresponds to the branch of demandInletNode(0).
     */
    void removeSupplyPlenum(const AirLoopHVAC& airloop, unsigned branchIndex);

    /** Establish plenumZone as the return plenum for this ThermalZone.
     *   This ThermalZone must already be attached to AirLoopHVAC.
     *   The plenumZone must not be used as a plenum on another AirLoopHVAC structure.
     *   The method canBePlenum called on plenumZone must return true.
     */
    bool setReturnPlenum(const ThermalZone& plenumZone);

    /** setReturnPlenum for the specified air loop.
     * This method is used when there are multiple air loops attached to the zone
     */
    bool setReturnPlenum(const ThermalZone& plenumZone, AirLoopHVAC& airLoop);

    /** Remove any return plenum serving this zone
    */
    void removeReturnPlenum();

    /** Remove any return plenum serving this zone
    */
    void removeReturnPlenum(AirLoopHVAC& airLoop);

    /** Returns all ZoneMixing objects associated with this zone, includes supply and exhaust mixing objects */
    std::vector<ZoneMixing> zoneMixing() const;

    /** Returns all ZoneMixing objects which supply air to this zone */
    std::vector<ZoneMixing> supplyZoneMixing() const;

    /** Returns all ZoneMixing objects which exhaust air from this zone */
    std::vector<ZoneMixing> exhaustZoneMixing() const;

    /** Creates an AirflowNetworkZone object if an object is not already attached. */
    AirflowNetworkZone getAirflowNetworkZone();

    /** Returns the attached AirflowNetworkZone if there is one */
    boost::optional<AirflowNetworkZone> airflowNetworkZone() const;

    std::vector<AirLoopHVAC> airLoopHVACs() const;

    // SQL Queries
    boost::optional<double> getAutosizedValueFromZoneSizes(const std::string& columnName, const std::string& loadType) const;

    boost::optional<double> autosizedMinimumOutdoorAirFlowRate() const;
    boost::optional<double> autosizedCoolingDesignAirFlowRate() const;
    boost::optional<double> autosizedHeatingDesignAirFlowRate() const;
    boost::optional<double> autosizedCoolingDesignLoad() const;
    // The max of autosizedCoolingDesignAirFlowRate and autosizedHeatingDesignAirFlowRate if both are found
    boost::optional<double> autosizedDesignAirFlowRate() const;
    boost::optional<double> autosizedHeatingDesignLoad() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ThermalZone_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    explicit ThermalZone(std::shared_ptr<detail::ThermalZone_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ThermalZone");
  };

  /** \relates ThermalZone*/
  using OptionalThermalZone = boost::optional<ThermalZone>;

  /** \relates ThermalZone*/
  using ThermalZoneVector = std::vector<ThermalZone>;

  /** This class implements a transition zone, for DaylightingDeviceTubular */
  class MODEL_API TransitionZone
  {
   public:
    TransitionZone(const ThermalZone& zone, double length);

    ThermalZone thermalZone() const;
    double length() const;

   private:
    ThermalZone m_zone;
    double m_length;
    REGISTER_LOGGER("openstudio.model.TransitionZone");
  };

  // Overload operator<<
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::TransitionZone& transitionZone);

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_THERMALZONE_HPP
