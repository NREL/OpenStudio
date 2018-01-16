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

#ifndef MODEL_THERMALZONE_IMPL_HPP
#define MODEL_THERMALZONE_IMPL_HPP

#include "ModelAPI.hpp"
#include "HVACComponent_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

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

namespace detail {

  /** ThermalZone_Impl is a HVACComponent_Impl that is the implementation class for ThermalZone.*/
  class MODEL_API ThermalZone_Impl : public HVACComponent_Impl {

   public:
    /** @name Constructors and Destructors */
    //@{

    ThermalZone_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ThermalZone_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    ThermalZone_Impl(const ThermalZone_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    virtual ~ThermalZone_Impl() {}

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

    virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent> & prev) override;

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

    boost::optional<Thermostat> thermostat() const;

    boost::optional<ZoneControlContaminantController> zoneControlContaminantController() const;

    boost::optional<ZoneControlHumidistat> zoneControlHumidistat() const;

    double fractionofZoneControlledbyPrimaryDaylightingControl() const;

    Quantity getFractionofZoneControlledbyPrimaryDaylightingControl(bool returnIP=false) const;

    bool isFractionofZoneControlledbyPrimaryDaylightingControlDefaulted() const;

    double fractionofZoneControlledbySecondaryDaylightingControl() const;

    Quantity getFractionofZoneControlledbySecondaryDaylightingControl(bool returnIP=false) const;

    bool isFractionofZoneControlledbySecondaryDaylightingControlDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setMultiplier(int multiplier);

    void resetMultiplier();

    bool setCeilingHeight(boost::optional<double> ceilingHeight);

    bool setCeilingHeight(double ceilingHeight);

    bool setCeilingHeight(const OSOptionalQuantity& ceilingHeight);

    void resetCeilingHeight();

    void autocalculateCeilingHeight();

    bool setVolume(boost::optional<double> volume);

    bool setVolume(double volume);

    bool setVolume(const OSOptionalQuantity& volume);

    void resetVolume();

    void autocalculateVolume();

    bool setZoneInsideConvectionAlgorithm(boost::optional<std::string> zoneInsideConvectionAlgorithm);

    bool setZoneInsideConvectionAlgorithm(std::string zoneInsideConvectionAlgorithm);

    void resetZoneInsideConvectionAlgorithm();

    bool setZoneOutsideConvectionAlgorithm(boost::optional<std::string> zoneOutsideConvectionAlgorithm);

    bool setZoneOutsideConvectionAlgorithm(std::string zoneOutsideConvectionAlgorithm);

    void resetZoneOutsideConvectionAlgorithm();

    bool setZoneConditioningEquipmentListName(std::string zoneConditioningEquipmentListName);

    bool setThermostatSetpointDualSetpoint(const ThermostatSetpointDualSetpoint& thermostat);

    void resetThermostatSetpointDualSetpoint();

    bool setThermostat(const Thermostat& thermostat);

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

    //@}

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

    bool setDaylightingControlsAndIlluminanceMaps(const boost::optional<DaylightingControl>& primaryDaylightingControl,
                                                  const boost::optional<DaylightingControl>& secondaryDaylightingControl,
                                                  const boost::optional<IlluminanceMap>& illuminanceMap);

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

    bool addToNode(Node & node) override;

    PortList inletPortList() const;

    PortList exhaustPortList() const;

    bool addEquipment(const ModelObject & equipment);

    bool removeEquipment(const ModelObject & equipment);

    bool setCoolingPriority(const ModelObject & equipment, unsigned priority);

    bool setHeatingPriority(const ModelObject & euqipment, unsigned priority);

    std::vector<ModelObject> equipmentInHeatingOrder();

    std::vector<ModelObject> equipmentInCoolingOrder();

    ZoneHVACEquipmentList zoneHVACEquipmentList() const;

    virtual ModelObject clone(Model model) const override;

    boost::optional<AirLoopHVACSupplyPlenum> airLoopHVACSupplyPlenum() const;

    boost::optional<AirLoopHVACReturnPlenum> airLoopHVACReturnPlenum() const;

    bool isPlenum() const;
    bool canBePlenum() const;
    bool setSupplyPlenum(const ThermalZone & plenumZone);
    bool setSupplyPlenum(const ThermalZone & plenumZone, unsigned branchIndex);
    void removeSupplyPlenum();
    bool setReturnPlenum(const ThermalZone & plenumZone);
    void removeReturnPlenum();
    void removeSupplyPlenum(unsigned branchIndex);

    std::vector<ZoneMixing> zoneMixing() const;
    std::vector<ZoneMixing> supplyZoneMixing() const;
    std::vector<ZoneMixing> exhaustZoneMixing() const;

    boost::optional<HVACComponent> airLoopHVACTerminal() const;

   protected:

   private:
    REGISTER_LOGGER("openstudio.model.ThermalZone");

    openstudio::OSOptionalQuantity ceilingHeight_SI() const;
    openstudio::OSOptionalQuantity ceilingHeight_IP() const;
    openstudio::OSOptionalQuantity volume_SI() const;
    openstudio::OSOptionalQuantity volume_IP() const;
    openstudio::Quantity fractionofZoneControlledbyPrimaryDaylightingControl_SI() const;
    openstudio::Quantity fractionofZoneControlledbyPrimaryDaylightingControl_IP() const;
    openstudio::Quantity fractionofZoneControlledbySecondaryDaylightingControl_SI() const;
    openstudio::Quantity fractionofZoneControlledbySecondaryDaylightingControl_IP() const;

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

} // detail

} // model
} // openstudio

#endif // MODEL_THERMALZONE_IMPL_HPP
