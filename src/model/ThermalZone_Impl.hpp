/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_THERMALZONE_IMPL_HPP
#define MODEL_THERMALZONE_IMPL_HPP

#include "ModelAPI.hpp"
#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Space;
  class DaylightingControl;
  class IlluminanceMap;
  class RenderingColor;
  class HVACTemplateZoneIdealLoadsAirSystem;
  class ThermostatSetpointDualSetpoint;
  class Thermostat;
  class ZoneControlContaminantController;
  class ZoneControlHumidistat;
  class ThermalZone;
  class SizingZone;
  class PortList;
  class ZoneMixing;
  class ZoneHVACEquipmentList;
  class AirflowNetworkZone;
  class ZonePropertyUserViewFactorsBySurfaceName;
  class Schedule;

  namespace detail {

    /** ThermalZone_Impl is a HVACComponent_Impl that is the implementation class for ThermalZone.*/
    class MODEL_API ThermalZone_Impl : public HVACComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ThermalZone_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ThermalZone_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ThermalZone_Impl(const ThermalZone_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ThermalZone_Impl() override = default;

      //@}

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// get a vector of allowable children types
      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent>& prev) override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

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

      boost::optional<ThermostatSetpointDualSetpoint> thermostatSetpointDualSetpoint() const;

      boost::optional<Thermostat> thermostat() const;

      boost::optional<ZoneControlContaminantController> zoneControlContaminantController() const;

      boost::optional<ZoneControlHumidistat> zoneControlHumidistat() const;

      double fractionofZoneControlledbyPrimaryDaylightingControl() const;

      bool isFractionofZoneControlledbyPrimaryDaylightingControlDefaulted() const;

      double fractionofZoneControlledbySecondaryDaylightingControl() const;

      bool isFractionofZoneControlledbySecondaryDaylightingControlDefaulted() const;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

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

      bool setThermostatSetpointDualSetpoint(const ThermostatSetpointDualSetpoint& thermostat);

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

      //@}

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

      bool setDaylightingControlsAndIlluminanceMaps(const boost::optional<DaylightingControl>& primaryDaylightingControl,
                                                    const boost::optional<DaylightingControl>& secondaryDaylightingControl,
                                                    const boost::optional<IlluminanceMap>& illuminanceMap);

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

      /// returns all spaces in this thermal zone
      std::vector<Space> spaces() const;

      /** Accumulates the floorArea of spaces. Does not include space multiplier. */
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

      /** Returns the electric equipment power (W) of this thermal zone. Does not include space multiplier. */
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

      /// Returns the HVACTemplateZoneIdealLoadsAirSystem in this ThermalZone if present.
      boost::optional<HVACTemplateZoneIdealLoadsAirSystem> hvacTemplateZoneIdealLoadsAirSystem() const;

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

      SizingZone sizingZone() const;

      void disconnect() override;

      bool isRemovable() const override;

      std::vector<IdfObject> remove() override;

      bool useIdealAirLoads() const;

      bool setUseIdealAirLoads(bool useIdealAirLoads);

      // Handles the logic of connecting the thermal zone to a Node: allowing or disallowing, and adding the equipment (AirTerminal) to the
      // ZoneEquipmentList as appropriate
      // Only allows connection to a node that's on the demand branch of an AirLoopHVAC, on the node right before the Demand Mixer, and as long as there
      // are no other ThermalZones already connected on the branch
      bool addToNodeImpl(Node& node);

      // Calls addToNodeImpl, then removes connections to all pre-existing AirLoopHVAC
      bool addToNode(Node& node) override;

      // Calls addToNodeImpl, not touching connections to all pre-existing AirLoopHVAC
      bool multiAddToNode(Node& node);

      PortList inletPortList() const;

      PortList exhaustPortList() const;

      PortList returnPortList() const;

      ZoneHVACEquipmentList zoneHVACEquipmentList() const;

      bool addEquipment(const ModelObject& equipment);

      bool removeEquipment(const ModelObject& equipment);

      std::string loadDistributionScheme() const;

      bool setLoadDistributionScheme(const std::string& scheme);

      bool setCoolingPriority(const ModelObject& equipment, unsigned priority);

      bool setHeatingPriority(const ModelObject& equipment, unsigned priority);

      std::vector<ModelObject> equipmentInHeatingOrder() const;

      std::vector<ModelObject> equipmentInCoolingOrder() const;

      boost::optional<Schedule> sequentialCoolingFractionSchedule(const ModelObject& equipment) const;

      boost::optional<Schedule> sequentialHeatingFractionSchedule(const ModelObject& equipment) const;

      bool setSequentialCoolingFractionSchedule(const ModelObject& equipment, Schedule& schedule);

      bool setSequentialHeatingFractionSchedule(const ModelObject& equipment, Schedule& schedule);

      // Convenience methods
      boost::optional<double> sequentialCoolingFraction(const ModelObject& equipment) const;
      boost::optional<double> sequentialHeatingFraction(const ModelObject& equipment) const;

      bool setSequentialCoolingFraction(const ModelObject& equipment, double fraction);
      bool setSequentialHeatingFraction(const ModelObject& equipment, double fraction);

      virtual ModelObject clone(Model model) const override;

      boost::optional<AirLoopHVACSupplyPlenum> airLoopHVACSupplyPlenum() const;

      boost::optional<AirLoopHVACReturnPlenum> airLoopHVACReturnPlenum() const;

      bool isPlenum() const;
      bool canBePlenum() const;
      bool setSupplyPlenum(const ThermalZone& plenumZone);
      bool setSupplyPlenum(const ThermalZone& plenumZone, unsigned branchIndex);
      void removeSupplyPlenum();
      void removeSupplyPlenum(unsigned branchIndex);
      void removeSupplyPlenum(const AirLoopHVAC& airloop);
      void removeSupplyPlenum(const AirLoopHVAC& airloop, unsigned branchIndex);
      bool setReturnPlenum(const ThermalZone& plenumZone);
      bool setReturnPlenum(const ThermalZone& plenumZone, AirLoopHVAC& airLoop);
      void removeReturnPlenum();
      void removeReturnPlenum(AirLoopHVAC& airLoop);

      std::vector<ZoneMixing> zoneMixing() const;
      std::vector<ZoneMixing> supplyZoneMixing() const;
      std::vector<ZoneMixing> exhaustZoneMixing() const;

      boost::optional<HVACComponent> airLoopHVACTerminal() const;
      std::vector<HVACComponent> airLoopHVACTerminals() const;

      AirflowNetworkZone getAirflowNetworkZone();
      boost::optional<AirflowNetworkZone> airflowNetworkZone() const;

      std::vector<AirLoopHVAC> airLoopHVACs() const;

      // SQL Queries
      boost::optional<double> getAutosizedValueFromZoneSizes(const std::string& columnName, const std::string& loadType) const;

      boost::optional<double> autosizedMinimumOutdoorAirFlowRate() const;
      boost::optional<double> autosizedCoolingDesignAirFlowRate() const;
      boost::optional<double> autosizedHeatingDesignAirFlowRate() const;
      boost::optional<double> autosizedCoolingDesignLoad() const;
      boost::optional<double> autosizedDesignAirFlowRate() const;
      boost::optional<double> autosizedHeatingDesignLoad() const;

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ThermalZone");

      boost::optional<ModelObject> thermostatSetpointDualSetpointAsModelObject() const;
      boost::optional<ModelObject> zoneControlHumidistatAsModelObject() const;
      boost::optional<ModelObject> primaryDaylightingControlAsModelObject() const;
      boost::optional<ModelObject> secondaryDaylightingControlAsModelObject() const;
      boost::optional<ModelObject> illuminanceMapAsModelObject() const;
      boost::optional<ModelObject> renderingColorAsModelObject() const;
      std::vector<ModelObject> equipmentAsModelObjects();
      std::vector<ModelObject> spacesAsModelObjects() const;

      bool setThermostatSetpointDualSetpointAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setZoneControlHumidistatAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setPrimaryDaylightingControlAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setSecondaryDaylightingControlAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setIlluminanceMapAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setRenderingColorAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_THERMALZONE_IMPL_HPP
