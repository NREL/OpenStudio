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
class ZoneControlHumidistat;
class ThermalZone;
class SizingZone;
class PortList;
class ZoneHVACEquipmentList;

namespace detail {

  /** ThermalZone_Impl is a HVACComponent_Impl that is the implementation class for ThermalZone.*/
  class MODEL_API ThermalZone_Impl : public HVACComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(int multiplier READ multiplier WRITE setMultiplier RESET resetMultiplier);
    Q_PROPERTY(bool isMultiplierDefaulted READ isMultiplierDefaulted);
    
    Q_PROPERTY(boost::optional<double> ceilingHeight READ ceilingHeight WRITE setCeilingHeight RESET resetCeilingHeight);
    Q_PROPERTY(openstudio::OSOptionalQuantity ceilingHeight_SI READ ceilingHeight_SI WRITE setCeilingHeight RESET resetCeilingHeight);
    Q_PROPERTY(openstudio::OSOptionalQuantity ceilingHeight_IP READ ceilingHeight_IP WRITE setCeilingHeight RESET resetCeilingHeight);
    Q_PROPERTY(bool isCeilingHeightDefaulted READ isCeilingHeightDefaulted);
    Q_PROPERTY(bool isCeilingHeightAutocalculated READ isCeilingHeightAutocalculated);

    Q_PROPERTY(boost::optional<double> volume READ volume WRITE setVolume RESET resetVolume);
    Q_PROPERTY(openstudio::OSOptionalQuantity volume_SI READ volume_SI WRITE setVolume RESET resetVolume);
    Q_PROPERTY(openstudio::OSOptionalQuantity volume_IP READ volume_IP WRITE setVolume RESET resetVolume);
    Q_PROPERTY(bool isVolumeDefaulted READ isVolumeDefaulted);
    Q_PROPERTY(bool isVolumeAutocalculated READ isVolumeAutocalculated);

    Q_PROPERTY(boost::optional<std::string> zoneInsideConvectionAlgorithm READ zoneInsideConvectionAlgorithm WRITE setZoneInsideConvectionAlgorithm RESET resetZoneInsideConvectionAlgorithm);
    
    Q_PROPERTY(boost::optional<std::string> zoneOutsideConvectionAlgorithm READ zoneOutsideConvectionAlgorithm WRITE setZoneOutsideConvectionAlgorithm RESET resetZoneOutsideConvectionAlgorithm);
    
    Q_PROPERTY(std::string zoneConditioningEquipmentListName READ zoneConditioningEquipmentListName WRITE setZoneConditioningEquipmentListName);
    
    Q_PROPERTY(double fractionofZoneControlledbyPrimaryDaylightingControl READ fractionofZoneControlledbyPrimaryDaylightingControl WRITE setFractionofZoneControlledbyPrimaryDaylightingControl RESET resetFractionofZoneControlledbyPrimaryDaylightingControl);
    Q_PROPERTY(openstudio::Quantity fractionofZoneControlledbyPrimaryDaylightingControl_SI READ fractionofZoneControlledbyPrimaryDaylightingControl_SI WRITE setFractionofZoneControlledbyPrimaryDaylightingControl RESET resetFractionofZoneControlledbyPrimaryDaylightingControl);
    Q_PROPERTY(openstudio::Quantity fractionofZoneControlledbyPrimaryDaylightingControl_IP READ fractionofZoneControlledbyPrimaryDaylightingControl_IP WRITE setFractionofZoneControlledbyPrimaryDaylightingControl RESET resetFractionofZoneControlledbyPrimaryDaylightingControl);    
    Q_PROPERTY(bool isFractionofZoneControlledbyPrimaryDaylightingControlDefaulted READ isFractionofZoneControlledbyPrimaryDaylightingControlDefaulted);
    
    Q_PROPERTY(double fractionofZoneControlledbySecondaryDaylightingControl READ fractionofZoneControlledbySecondaryDaylightingControl WRITE setFractionofZoneControlledbySecondaryDaylightingControl RESET resetFractionofZoneControlledbySecondaryDaylightingControl);
    Q_PROPERTY(openstudio::Quantity fractionofZoneControlledbySecondaryDaylightingControl_SI READ fractionofZoneControlledbySecondaryDaylightingControl_SI WRITE setFractionofZoneControlledbySecondaryDaylightingControl RESET resetFractionofZoneControlledbySecondaryDaylightingControl);
    Q_PROPERTY(openstudio::Quantity fractionofZoneControlledbySecondaryDaylightingControl_IP READ fractionofZoneControlledbySecondaryDaylightingControl_IP WRITE setFractionofZoneControlledbySecondaryDaylightingControl RESET resetFractionofZoneControlledbySecondaryDaylightingControl);    
    Q_PROPERTY(bool isFractionofZoneControlledbySecondaryDaylightingControlDefaulted READ isFractionofZoneControlledbySecondaryDaylightingControlDefaulted);

    Q_PROPERTY(double floorArea READ floorArea);
    Q_PROPERTY(double numberOfPeople READ numberOfPeople);
    Q_PROPERTY(double peoplePerFloorArea READ peoplePerFloorArea);
    Q_PROPERTY(double floorAreaPerPerson READ floorAreaPerPerson);
    Q_PROPERTY(double lightingPower READ lightingPower);
    Q_PROPERTY(double lightingPowerPerFloorArea READ lightingPowerPerFloorArea);
    Q_PROPERTY(double lightingPowerPerPerson READ lightingPowerPerPerson);
    Q_PROPERTY(double electricEquipmentPower READ electricEquipmentPower);
    Q_PROPERTY(double electricEquipmentPowerPerFloorArea READ electricEquipmentPowerPerFloorArea);
    Q_PROPERTY(double electricEquipmentPowerPerPerson READ electricEquipmentPowerPerPerson);
    Q_PROPERTY(double gasEquipmentPower READ gasEquipmentPower);
    Q_PROPERTY(double gasEquipmentPowerPerFloorArea READ gasEquipmentPowerPerFloorArea);
    Q_PROPERTY(double gasEquipmentPowerPerPerson READ gasEquipmentPowerPerPerson);
    Q_PROPERTY(bool useIdealAirLoads READ useIdealAirLoads WRITE setUseIdealAirLoads);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> thermostatSetpointDualSetpoint READ thermostatSetpointDualSetpointAsModelObject WRITE setThermostatSetpointDualSetpointAsModelObject RESET resetThermostatSetpointDualSetpoint);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> zoneControlHumidistat READ zoneControlHumidistatAsModelObject WRITE setZoneControlHumidistatAsModelObject RESET resetZoneControlHumidistat);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> primaryDaylightingControl READ primaryDaylightingControlAsModelObject WRITE setPrimaryDaylightingControlAsModelObject RESET resetPrimaryDaylightingControl);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> secondaryDaylightingControl READ secondaryDaylightingControlAsModelObject WRITE setSecondaryDaylightingControlAsModelObject RESET resetSecondaryDaylightingControl);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> illuminanceMap READ illuminanceMapAsModelObject WRITE setIlluminanceMapAsModelObject RESET resetIlluminanceMap);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> renderingColor READ renderingColorAsModelObject WRITE setRenderingColorAsModelObject RESET resetRenderingColor);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> equipment READ equipmentAsModelObjects);
    Q_PROPERTY(std::vector<openstudio::model::ModelObject> spaces READ spacesAsModelObjects);
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
    virtual boost::optional<ParentObject> parent() const;

    // set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    // return any children objects in the hierarchy
    virtual std::vector<ModelObject> children() const;

    /// get a vector of allowable children types
    virtual std::vector<IddObjectType> allowableChildTypes() const;

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    /** This function returns a vector of HVACComponent that are directly downstream
     *  from this object on an AirLoopHVAC or PlantLoop. 
     *  @param[in]  isDemandComponent  Boolean passed in whether object is a demand or supply component
    **/
    virtual std::vector<HVACComponent> edges(bool isDemandComponent);

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

    //@}
    /** @name Setters */
    //@{

    bool setMultiplier(int multiplier);

    void resetMultiplier();

    void setCeilingHeight(boost::optional<double> ceilingHeight);

    void setCeilingHeight(double ceilingHeight);
    
    bool setCeilingHeight(const OSOptionalQuantity& ceilingHeight);

    void resetCeilingHeight();

    void autocalculateCeilingHeight();

    void setVolume(boost::optional<double> volume);

    void setVolume(double volume);
    
    bool setVolume(const OSOptionalQuantity& volume);

    void resetVolume();

    void autocalculateVolume();

    bool setZoneInsideConvectionAlgorithm(boost::optional<std::string> zoneInsideConvectionAlgorithm);

    bool setZoneInsideConvectionAlgorithm(std::string zoneInsideConvectionAlgorithm);

    void resetZoneInsideConvectionAlgorithm();

    bool setZoneOutsideConvectionAlgorithm(boost::optional<std::string> zoneOutsideConvectionAlgorithm);

    bool setZoneOutsideConvectionAlgorithm(std::string zoneOutsideConvectionAlgorithm);

    void resetZoneOutsideConvectionAlgorithm();

    void setZoneConditioningEquipmentListName(std::string zoneConditioningEquipmentListName);

    bool setThermostatSetpointDualSetpoint(const ThermostatSetpointDualSetpoint& thermostat);

    void resetThermostatSetpointDualSetpoint();

    bool setZoneControlHumidistat(const ZoneControlHumidistat & humidistat);

    void resetZoneControlHumidistat();

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

    void disconnect();

    bool isRemovable() const;

    std::vector<IdfObject> remove();

    bool useIdealAirLoads() const;

    void setUseIdealAirLoads(bool useIdealAirLoads);

    bool addToNode(Node & node);

    PortList inletPortList() const;

    PortList exhaustPortList() const;

    void addEquipment(const ModelObject & equipment);

    void removeEquipment(const ModelObject & equipment);

    void setCoolingPriority(const ModelObject & equipment, unsigned priority);

    void setHeatingPriority(const ModelObject & euqipment, unsigned priority);

    std::vector<ModelObject> equipmentInHeatingOrder();

    std::vector<ModelObject> equipmentInCoolingOrder();

    ZoneHVACEquipmentList zoneHVACEquipmentList() const;

    virtual ModelObject clone(Model model) const;

    boost::optional<AirLoopHVACSupplyPlenum> airLoopHVACSupplyPlenum() const;

    boost::optional<AirLoopHVACReturnPlenum> airLoopHVACReturnPlenum() const;

    bool isPlenum() const;
    bool canBePlenum() const;
    bool setSupplyPlenum(const ThermalZone & plenumZone);
    void removeSupplyPlenum();
    bool setReturnPlenum(const ThermalZone & plenumZone);
    void removeReturnPlenum();

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

