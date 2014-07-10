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

#ifndef MODEL_ZONEHVACPACKAGEDTERMINALAIRCONDITIONER_IMPL_HPP
#define MODEL_ZONEHVACPACKAGEDTERMINALAIRCONDITIONER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  /** ZoneHVACPackagedTerminalAirConditioner_Impl is a ZoneHVACComponent_Impl that is the implementation class for
   *  ZoneHVACPackagedTerminalAirConditioner. */
  class MODEL_API ZoneHVACPackagedTerminalAirConditioner_Impl : public ZoneHVACComponent_Impl
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
    Q_PROPERTY(std::string fanPlacement READ fanPlacement WRITE setFanPlacement RESET resetFanPlacement);
    Q_PROPERTY(bool isFanPlacementDefaulted READ isFanPlacementDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> supplyAirFan READ supplyAirFanAsModelObject WRITE setSupplyAirFanAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> heatingCoil READ heatingCoilAsModelObject WRITE setHeatingCoilAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> coolingCoil READ coolingCoilAsModelObject WRITE setCoolingCoilAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> supplyAirFanOperatingModeSchedule READ supplyAirFanOperatingModeScheduleAsModelObject WRITE setSupplyAirFanOperatingModeScheduleAsModelObject RESET resetSupplyAirFanOperatingModeSchedule);
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACPackagedTerminalAirConditioner_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ZoneHVACPackagedTerminalAirConditioner_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    ZoneHVACPackagedTerminalAirConditioner_Impl(const ZoneHVACPackagedTerminalAirConditioner_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    virtual ~ZoneHVACPackagedTerminalAirConditioner_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const;

    virtual std::vector<IdfObject> remove();

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

    HVACComponent coolingCoil() const;

    std::string fanPlacement() const;

    bool isFanPlacementDefaulted() const;

    boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule );

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

    void setSupplyAirFan( HVACComponent & fan );

    void setHeatingCoil( HVACComponent & heatingCoil );

    void setCoolingCoil( HVACComponent & coolingCoil );

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
    REGISTER_LOGGER("openstudio.model.ZoneHVACPackagedTerminalAirConditioner");

    // optional getters so can remove() if constructor fails
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<HVACComponent> optionalSupplyAirFan() const;
    boost::optional<HVACComponent> optionalHeatingCoil() const;
    boost::optional<HVACComponent> optionalCoolingCoil() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> supplyAirFanAsModelObject() const;
    boost::optional<ModelObject> heatingCoilAsModelObject() const;
    boost::optional<ModelObject> coolingCoilAsModelObject() const;
    boost::optional<ModelObject> supplyAirFanOperatingModeScheduleAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setSupplyAirFanAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setSupplyAirFanOperatingModeScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACPACKAGEDTERMINALAIRCONDITIONER_IMPL_HPP

