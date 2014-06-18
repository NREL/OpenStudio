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

#ifndef MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIR_IMPL_HPP
#define MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIR_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

class Schedule;

class HVACComponent;

class ThermalZone;

class Node;

namespace detail {

  /** AirLoopHVACUnitaryHeatPumpAirToAir_Impl is a StraightComponent_Impl that is the implementation class for AirLoopHVACUnitaryHeatPumpAirToAir.*/
  class MODEL_API AirLoopHVACUnitaryHeatPumpAirToAir_Impl : public StraightComponent_Impl
  {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<double> supplyAirFlowRateDuringCoolingOperation READ supplyAirFlowRateDuringCoolingOperation WRITE setSupplyAirFlowRateDuringCoolingOperation);
    Q_PROPERTY(bool isSupplyAirFlowRateDuringCoolingOperationAutosized READ isSupplyAirFlowRateDuringCoolingOperationAutosized);
    Q_PROPERTY(boost::optional<double> supplyAirFlowRateDuringHeatingOperation READ supplyAirFlowRateDuringHeatingOperation WRITE setSupplyAirFlowRateDuringHeatingOperation);
    Q_PROPERTY(bool isSupplyAirFlowRateDuringHeatingOperationAutosized READ isSupplyAirFlowRateDuringHeatingOperationAutosized);
    Q_PROPERTY(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded READ supplyAirFlowRateWhenNoCoolingorHeatingisNeeded WRITE setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded RESET resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded);
    Q_PROPERTY(bool isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized READ isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized);
    Q_PROPERTY(boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater READ maximumSupplyAirTemperaturefromSupplementalHeater WRITE setMaximumSupplyAirTemperaturefromSupplementalHeater);
    Q_PROPERTY(bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized READ isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized);
    Q_PROPERTY(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation READ maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation WRITE setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation RESET resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
    Q_PROPERTY(bool isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted READ isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted);
    Q_PROPERTY(std::string fanPlacement READ fanPlacement WRITE setFanPlacement RESET resetFanPlacement);
    Q_PROPERTY(bool isFanPlacementDefaulted READ isFanPlacementDefaulted);
    Q_PROPERTY(std::string dehumidificationControlType READ dehumidificationControlType WRITE setDehumidificationControlType RESET resetDehumidificationControlType);
    Q_PROPERTY(bool isDehumidificationControlTypeDefaulted READ isDehumidificationControlTypeDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> controllingZone READ controllingZoneAsModelObject WRITE setControllingZoneAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> supplyAirFan READ supplyAirFanAsModelObject WRITE setSupplyAirFanAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> heatingCoil READ heatingCoilAsModelObject WRITE setHeatingCoilAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> coolingCoil READ coolingCoilAsModelObject WRITE setCoolingCoilAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> supplementalHeatingCoil READ supplementalHeatingCoilAsModelObject WRITE setSupplementalHeatingCoilAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> supplyAirFanOperatingModeSchedule READ supplyAirFanOperatingModeScheduleAsModelObject WRITE setSupplyAirFanOperatingModeScheduleAsModelObject RESET resetSupplyAirFanOperatingModeSchedule);
   public:
    /** @name Constructors and Destructors */
    //@{

    AirLoopHVACUnitaryHeatPumpAirToAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    AirLoopHVACUnitaryHeatPumpAirToAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                            Model_Impl* model,
                                            bool keepHandle);

    AirLoopHVACUnitaryHeatPumpAirToAir_Impl(const AirLoopHVACUnitaryHeatPumpAirToAir_Impl& other,
                                            Model_Impl* model,
                                            bool keepHandle);

    virtual ~AirLoopHVACUnitaryHeatPumpAirToAir_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual std::vector<ModelObject> children() const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    virtual bool addToNode(Node & node);

    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> supplyAirFlowRateDuringCoolingOperation() const;

    bool isSupplyAirFlowRateDuringCoolingOperationAutosized() const;

    boost::optional<double> supplyAirFlowRateDuringHeatingOperation() const;

    bool isSupplyAirFlowRateDuringHeatingOperationAutosized() const;

    boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    bool isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

    boost::optional<ThermalZone> controllingZone() const;

    HVACComponent supplyAirFan() const;

    HVACComponent heatingCoil() const;

    HVACComponent coolingCoil() const;

    HVACComponent supplementalHeatingCoil() const;

    boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater() const;

    bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const;

    double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;

    bool isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const;

    std::string fanPlacement() const;

    bool isFanPlacementDefaulted() const;

    boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

    std::string dehumidificationControlType() const;

    bool isDehumidificationControlTypeDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation);

    void autosizeSupplyAirFlowRateDuringCoolingOperation();

    bool setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> n2);

    void autosizeSupplyAirFlowRateDuringHeatingOperation();

    bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    void setControllingZone( ThermalZone & zone );

    void resetControllingZone();

    void setSupplyAirFan( HVACComponent & hvacComponent );

    void setHeatingCoil( HVACComponent & hvacComponent );

    void setCoolingCoil( HVACComponent & hvacComponent );

    void setSupplementalHeatingCoil( HVACComponent & hvacComponent );
    void setMaximumSupplyAirTemperaturefromSupplementalHeater(boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater);

    void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

    bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

    void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

    bool setFanPlacement(std::string fanPlacement);

    void resetFanPlacement();

    bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

    void resetSupplyAirFanOperatingModeSchedule();

    bool setDehumidificationControlType(std::string dehumidificationControlType);

    void resetDehumidificationControlType();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.AirLoopHVACUnitaryHeatPumpAirToAir");

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> controllingZoneAsModelObject() const;
    boost::optional<ModelObject> supplyAirFanAsModelObject() const;
    boost::optional<ModelObject> heatingCoilAsModelObject() const;
    boost::optional<ModelObject> coolingCoilAsModelObject() const;
    boost::optional<ModelObject> supplementalHeatingCoilAsModelObject() const;
    boost::optional<ModelObject> supplyAirFanOperatingModeScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setControllingZoneAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setSupplyAirFanAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setSupplementalHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setSupplyAirFanOperatingModeScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AIRLOOPHVACUNITARYHEATPUMPAIRTOAIR_IMPL_HPP

