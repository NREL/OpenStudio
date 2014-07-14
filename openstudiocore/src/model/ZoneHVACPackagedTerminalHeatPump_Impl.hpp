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

#ifndef MODEL_ZONEHVACPACKAGEDTERMINALHEATPUMP_IMPL_HPP
#define MODEL_ZONEHVACPACKAGEDTERMINALHEATPUMP_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

class Schedule;

class HVACComponent;

namespace detail {

  /** ZoneHVACPackagedTerminalHeatPump_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACPackagedTerminalHeatPump.*/
  class MODEL_API ZoneHVACPackagedTerminalHeatPump_Impl : public ZoneHVACComponent_Impl
  {
    Q_OBJECT;
    Q_PROPERTY(std::string outdoorAirMixerObjectType READ outdoorAirMixerObjectType WRITE setOutdoorAirMixerObjectType);
    Q_PROPERTY(std::string outdoorAirMixerName READ outdoorAirMixerName WRITE setOutdoorAirMixerName);
    Q_PROPERTY(boost::optional<double> supplyAirFlowRateDuringCoolingOperation READ supplyAirFlowRateDuringCoolingOperation WRITE setSupplyAirFlowRateDuringCoolingOperation);
    Q_PROPERTY(bool isSupplyAirFlowRateDuringCoolingOperationAutosized READ isSupplyAirFlowRateDuringCoolingOperationAutosized);
    Q_PROPERTY(boost::optional<double> supplyAirFlowRateDuringHeatingOperation READ supplyAirFlowRateDuringHeatingOperation WRITE setSupplyAirFlowRateDuringHeatingOperation);
    Q_PROPERTY(bool isSupplyAirFlowRateDuringHeatingOperationAutosized READ isSupplyAirFlowRateDuringHeatingOperationAutosized);
    Q_PROPERTY(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded READ supplyAirFlowRateWhenNoCoolingorHeatingisNeeded WRITE setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded RESET resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded);
    Q_PROPERTY(bool isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized READ isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized);
    Q_PROPERTY(boost::optional<double> outdoorAirFlowRateDuringCoolingOperation READ outdoorAirFlowRateDuringCoolingOperation WRITE setOutdoorAirFlowRateDuringCoolingOperation);
    Q_PROPERTY(bool isOutdoorAirFlowRateDuringCoolingOperationAutosized READ isOutdoorAirFlowRateDuringCoolingOperationAutosized);
    Q_PROPERTY(boost::optional<double> outdoorAirFlowRateDuringHeatingOperation READ outdoorAirFlowRateDuringHeatingOperation WRITE setOutdoorAirFlowRateDuringHeatingOperation);
    Q_PROPERTY(bool isOutdoorAirFlowRateDuringHeatingOperationAutosized READ isOutdoorAirFlowRateDuringHeatingOperationAutosized);
    Q_PROPERTY(boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded READ outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded WRITE setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded RESET resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);
    Q_PROPERTY(bool isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized READ isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized);
    Q_PROPERTY(double heatingConvergenceTolerance READ heatingConvergenceTolerance WRITE setHeatingConvergenceTolerance RESET resetHeatingConvergenceTolerance);
    Q_PROPERTY(bool isHeatingConvergenceToleranceDefaulted READ isHeatingConvergenceToleranceDefaulted);
    Q_PROPERTY(double minimumOutdoorDryBulbTemperatureforCompressorOperation READ minimumOutdoorDryBulbTemperatureforCompressorOperation WRITE setMinimumOutdoorDryBulbTemperatureforCompressorOperation RESET resetMinimumOutdoorDryBulbTemperatureforCompressorOperation);
    Q_PROPERTY(bool isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted READ isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted);
    Q_PROPERTY(double coolingConvergenceTolerance READ coolingConvergenceTolerance WRITE setCoolingConvergenceTolerance RESET resetCoolingConvergenceTolerance);
    Q_PROPERTY(bool isCoolingConvergenceToleranceDefaulted READ isCoolingConvergenceToleranceDefaulted);
    Q_PROPERTY(boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater READ maximumSupplyAirTemperaturefromSupplementalHeater WRITE setMaximumSupplyAirTemperaturefromSupplementalHeater);
    Q_PROPERTY(bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized READ isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized);
    Q_PROPERTY(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation READ maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation WRITE setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation RESET resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
    Q_PROPERTY(bool isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted READ isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted);
    Q_PROPERTY(std::string fanPlacement READ fanPlacement WRITE setFanPlacement RESET resetFanPlacement);
    Q_PROPERTY(bool isFanPlacementDefaulted READ isFanPlacementDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> supplyAirFan READ supplyAirFanAsModelObject WRITE setSupplyAirFanAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> heatingCoil READ heatingCoilAsModelObject WRITE setHeatingCoilAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> coolingCoil READ coolingCoilAsModelObject WRITE setCoolingCoilAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> supplementalHeatingCoil READ supplementalHeatingCoilAsModelObject WRITE setSupplementalHeatingCoilAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> supplyAirFanOperatingModeSchedule READ supplyAirFanOperatingModeScheduleAsModelObject WRITE setSupplyAirFanOperatingModeScheduleAsModelObject RESET resetSupplyAirFanOperatingModeSchedule);
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACPackagedTerminalHeatPump_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ZoneHVACPackagedTerminalHeatPump_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    ZoneHVACPackagedTerminalHeatPump_Impl(const ZoneHVACPackagedTerminalHeatPump_Impl& other,
                                          Model_Impl* model,
                                          bool keepHandle);

    virtual ~ZoneHVACPackagedTerminalHeatPump_Impl() {}

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

    //@}
    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    std::string outdoorAirMixerObjectType() const;

    std::string outdoorAirMixerName() const;

    boost::optional<double> supplyAirFlowRateDuringCoolingOperation() const;

    bool isSupplyAirFlowRateDuringCoolingOperationAutosized() const;

    boost::optional<double> supplyAirFlowRateDuringHeatingOperation() const;

    bool isSupplyAirFlowRateDuringHeatingOperationAutosized() const;

    boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    bool isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

    boost::optional<double> outdoorAirFlowRateDuringCoolingOperation() const;

    bool isOutdoorAirFlowRateDuringCoolingOperationAutosized() const;

    boost::optional<double> outdoorAirFlowRateDuringHeatingOperation() const;

    bool isOutdoorAirFlowRateDuringHeatingOperationAutosized() const;

    boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const;

    bool isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const;

    HVACComponent supplyAirFan() const;

    HVACComponent heatingCoil() const;

    double heatingConvergenceTolerance() const;

    bool isHeatingConvergenceToleranceDefaulted() const;

    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    bool isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted() const;

    HVACComponent coolingCoil() const;

    double coolingConvergenceTolerance() const;

    bool isCoolingConvergenceToleranceDefaulted() const;

    HVACComponent supplementalHeatingCoil() const;

    boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater() const;

    bool isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const;

    double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const;

    bool isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const;

    std::string fanPlacement() const;

    bool isFanPlacementDefaulted() const;

    boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule & schedule );

    bool setOutdoorAirMixerObjectType(std::string outdoorAirMixerObjectType);

    void setOutdoorAirMixerName(std::string outdoorAirMixerName);

    bool setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation);

    void autosizeSupplyAirFlowRateDuringCoolingOperation();

    bool setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> supplyAirFlowRateDuringHeatingOperation);

    void autosizeSupplyAirFlowRateDuringHeatingOperation();

    bool setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    void autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();

    bool setOutdoorAirFlowRateDuringCoolingOperation(boost::optional<double> outdoorAirFlowRateDuringCoolingOperation);

    void autosizeOutdoorAirFlowRateDuringCoolingOperation();

    bool setOutdoorAirFlowRateDuringHeatingOperation(boost::optional<double> outdoorAirFlowRateDuringHeatingOperation);

    void autosizeOutdoorAirFlowRateDuringHeatingOperation();

    bool setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);

    void resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

    void autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

    void setSupplyAirFan( HVACComponent & hvacComponent );

    void setHeatingCoil( HVACComponent & hvacComponent );

    bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

    void resetHeatingConvergenceTolerance();

    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    void resetMinimumOutdoorDryBulbTemperatureforCompressorOperation();

    void setCoolingCoil( HVACComponent & hvacComponent );

    bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

    void resetCoolingConvergenceTolerance();

    void setSupplementalHeatingCoil( HVACComponent & hvacComponent );

    void setMaximumSupplyAirTemperaturefromSupplementalHeater(boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater);

    void autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();

    bool setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);

    void resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();

    bool setFanPlacement(std::string fanPlacement);

    void resetFanPlacement();

    bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

    void resetSupplyAirFanOperatingModeSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACPackagedTerminalHeatPump");

    // optional getters for children() in case need to remove() because constructor fails
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<HVACComponent> optionalSupplyAirFan() const;
    boost::optional<HVACComponent> optionalHeatingCoil() const;
    boost::optional<HVACComponent> optionalCoolingCoil() const;
    boost::optional<HVACComponent> optionalSupplementalHeatingCoil() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> supplyAirFanAsModelObject() const;
    boost::optional<ModelObject> heatingCoilAsModelObject() const;
    boost::optional<ModelObject> coolingCoilAsModelObject() const;
    boost::optional<ModelObject> supplementalHeatingCoilAsModelObject() const;
    boost::optional<ModelObject> supplyAirFanOperatingModeScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setSupplyAirFanAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setSupplementalHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setSupplyAirFanOperatingModeScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACPACKAGEDTERMINALHEATPUMP_IMPL_HPP

