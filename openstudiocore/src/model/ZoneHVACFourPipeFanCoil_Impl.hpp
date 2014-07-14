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

#ifndef MODEL_ZONEHVACFOURPIPEFANCOIL_IMPL_HPP
#define MODEL_ZONEHVACFOURPIPEFANCOIL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  /** ZoneHVACFourPipeFanCoil_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACFourPipeFanCoil.*/
  class MODEL_API ZoneHVACFourPipeFanCoil_Impl : public ZoneHVACComponent_Impl {
    Q_OBJECT;

    Q_PROPERTY(std::string capacityControlMethod READ capacityControlMethod WRITE setCapacityControlMethod);
    Q_PROPERTY(std::vector<std::string> capacityControlMethodValues READ capacityControlMethodValues);

    Q_PROPERTY(boost::optional<double> maximumSupplyAirFlowRate READ maximumSupplyAirFlowRate WRITE setMaximumSupplyAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumSupplyAirFlowRate_SI READ maximumSupplyAirFlowRate_SI WRITE setMaximumSupplyAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumSupplyAirFlowRate_IP READ maximumSupplyAirFlowRate_IP WRITE setMaximumSupplyAirFlowRate);
    Q_PROPERTY(bool isMaximumSupplyAirFlowRateAutosized READ isMaximumSupplyAirFlowRateAutosized);

    Q_PROPERTY(double lowSpeedSupplyAirFlowRatio READ lowSpeedSupplyAirFlowRatio WRITE setLowSpeedSupplyAirFlowRatio RESET resetLowSpeedSupplyAirFlowRatio);
    Q_PROPERTY(openstudio::Quantity lowSpeedSupplyAirFlowRatio_SI READ lowSpeedSupplyAirFlowRatio_SI WRITE setLowSpeedSupplyAirFlowRatio RESET resetLowSpeedSupplyAirFlowRatio);
    Q_PROPERTY(openstudio::Quantity lowSpeedSupplyAirFlowRatio_IP READ lowSpeedSupplyAirFlowRatio_IP WRITE setLowSpeedSupplyAirFlowRatio RESET resetLowSpeedSupplyAirFlowRatio);
    Q_PROPERTY(bool isLowSpeedSupplyAirFlowRatioDefaulted READ isLowSpeedSupplyAirFlowRatioDefaulted);

    Q_PROPERTY(double mediumSpeedSupplyAirFlowRatio READ mediumSpeedSupplyAirFlowRatio WRITE setMediumSpeedSupplyAirFlowRatio RESET resetMediumSpeedSupplyAirFlowRatio);
    Q_PROPERTY(openstudio::Quantity mediumSpeedSupplyAirFlowRatio_SI READ mediumSpeedSupplyAirFlowRatio_SI WRITE setMediumSpeedSupplyAirFlowRatio RESET resetMediumSpeedSupplyAirFlowRatio);
    Q_PROPERTY(openstudio::Quantity mediumSpeedSupplyAirFlowRatio_IP READ mediumSpeedSupplyAirFlowRatio_IP WRITE setMediumSpeedSupplyAirFlowRatio RESET resetMediumSpeedSupplyAirFlowRatio);
    Q_PROPERTY(bool isMediumSpeedSupplyAirFlowRatioDefaulted READ isMediumSpeedSupplyAirFlowRatioDefaulted);

    Q_PROPERTY(boost::optional<double> maximumOutdoorAirFlowRate READ maximumOutdoorAirFlowRate WRITE setMaximumOutdoorAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumOutdoorAirFlowRate_SI READ maximumOutdoorAirFlowRate_SI WRITE setMaximumOutdoorAirFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumOutdoorAirFlowRate_IP READ maximumOutdoorAirFlowRate_IP WRITE setMaximumOutdoorAirFlowRate);
    Q_PROPERTY(bool isMaximumOutdoorAirFlowRateAutosized READ isMaximumOutdoorAirFlowRateAutosized);

    Q_PROPERTY(std::string outdoorAirMixerObjectType READ outdoorAirMixerObjectType WRITE setOutdoorAirMixerObjectType);
    Q_PROPERTY(std::vector<std::string> outdoorAirMixerObjectTypeValues READ outdoorAirMixerObjectTypeValues);

    Q_PROPERTY(std::string outdoorAirMixerName READ outdoorAirMixerName WRITE setOutdoorAirMixerName);

    Q_PROPERTY(boost::optional<double> maximumColdWaterFlowRate READ maximumColdWaterFlowRate WRITE setMaximumColdWaterFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumColdWaterFlowRate_SI READ maximumColdWaterFlowRate_SI WRITE setMaximumColdWaterFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumColdWaterFlowRate_IP READ maximumColdWaterFlowRate_IP WRITE setMaximumColdWaterFlowRate);
    Q_PROPERTY(bool isMaximumColdWaterFlowRateAutosized READ isMaximumColdWaterFlowRateAutosized);

    Q_PROPERTY(double minimumColdWaterFlowRate READ minimumColdWaterFlowRate WRITE setMinimumColdWaterFlowRate RESET resetMinimumColdWaterFlowRate);
    Q_PROPERTY(openstudio::Quantity minimumColdWaterFlowRate_SI READ minimumColdWaterFlowRate_SI WRITE setMinimumColdWaterFlowRate RESET resetMinimumColdWaterFlowRate);
    Q_PROPERTY(openstudio::Quantity minimumColdWaterFlowRate_IP READ minimumColdWaterFlowRate_IP WRITE setMinimumColdWaterFlowRate RESET resetMinimumColdWaterFlowRate);
    Q_PROPERTY(bool isMinimumColdWaterFlowRateDefaulted READ isMinimumColdWaterFlowRateDefaulted);

    Q_PROPERTY(double coolingConvergenceTolerance READ coolingConvergenceTolerance WRITE setCoolingConvergenceTolerance RESET resetCoolingConvergenceTolerance);
    Q_PROPERTY(openstudio::Quantity coolingConvergenceTolerance_SI READ coolingConvergenceTolerance_SI WRITE setCoolingConvergenceTolerance RESET resetCoolingConvergenceTolerance);
    Q_PROPERTY(openstudio::Quantity coolingConvergenceTolerance_IP READ coolingConvergenceTolerance_IP WRITE setCoolingConvergenceTolerance RESET resetCoolingConvergenceTolerance);
    Q_PROPERTY(bool isCoolingConvergenceToleranceDefaulted READ isCoolingConvergenceToleranceDefaulted);

    Q_PROPERTY(boost::optional<double> maximumHotWaterFlowRate READ maximumHotWaterFlowRate WRITE setMaximumHotWaterFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumHotWaterFlowRate_SI READ maximumHotWaterFlowRate_SI WRITE setMaximumHotWaterFlowRate);
    Q_PROPERTY(openstudio::OSOptionalQuantity maximumHotWaterFlowRate_IP READ maximumHotWaterFlowRate_IP WRITE setMaximumHotWaterFlowRate);
    Q_PROPERTY(bool isMaximumHotWaterFlowRateAutosized READ isMaximumHotWaterFlowRateAutosized);

    Q_PROPERTY(double minimumHotWaterFlowRate READ minimumHotWaterFlowRate WRITE setMinimumHotWaterFlowRate RESET resetMinimumHotWaterFlowRate);
    Q_PROPERTY(openstudio::Quantity minimumHotWaterFlowRate_SI READ minimumHotWaterFlowRate_SI WRITE setMinimumHotWaterFlowRate RESET resetMinimumHotWaterFlowRate);
    Q_PROPERTY(openstudio::Quantity minimumHotWaterFlowRate_IP READ minimumHotWaterFlowRate_IP WRITE setMinimumHotWaterFlowRate RESET resetMinimumHotWaterFlowRate);
    Q_PROPERTY(bool isMinimumHotWaterFlowRateDefaulted READ isMinimumHotWaterFlowRateDefaulted);

    Q_PROPERTY(double heatingConvergenceTolerance READ heatingConvergenceTolerance WRITE setHeatingConvergenceTolerance RESET resetHeatingConvergenceTolerance);
    Q_PROPERTY(openstudio::Quantity heatingConvergenceTolerance_SI READ heatingConvergenceTolerance_SI WRITE setHeatingConvergenceTolerance RESET resetHeatingConvergenceTolerance);
    Q_PROPERTY(openstudio::Quantity heatingConvergenceTolerance_IP READ heatingConvergenceTolerance_IP WRITE setHeatingConvergenceTolerance RESET resetHeatingConvergenceTolerance);
    Q_PROPERTY(bool isHeatingConvergenceToleranceDefaulted READ isHeatingConvergenceToleranceDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> availabilitySchedule READ availabilityScheduleAsModelObject WRITE setAvailabilityScheduleAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> outdoorAirSchedule READ outdoorAirScheduleAsModelObject WRITE setOutdoorAirScheduleAsModelObject RESET resetOutdoorAirSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> supplyAirFan READ supplyAirFanAsModelObject WRITE setSupplyAirFanAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> coolingCoil READ coolingCoilAsModelObject WRITE setCoolingCoilAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> heatingCoil READ heatingCoilAsModelObject WRITE setHeatingCoilAsModelObject);
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACFourPipeFanCoil_Impl(const IdfObject& idfObject,
                                 Model_Impl* model,
                                 bool keepHandle);

    ZoneHVACFourPipeFanCoil_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    ZoneHVACFourPipeFanCoil_Impl(const ZoneHVACFourPipeFanCoil_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle);

    virtual ~ZoneHVACFourPipeFanCoil_Impl() {}

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

    std::string capacityControlMethod() const;

    boost::optional<double> maximumSupplyAirFlowRate() const;

    OSOptionalQuantity getMaximumSupplyAirFlowRate(bool returnIP=false) const;

    bool isMaximumSupplyAirFlowRateAutosized() const;

    double lowSpeedSupplyAirFlowRatio() const;

    Quantity getLowSpeedSupplyAirFlowRatio(bool returnIP=false) const;

    bool isLowSpeedSupplyAirFlowRatioDefaulted() const;

    double mediumSpeedSupplyAirFlowRatio() const;

    Quantity getMediumSpeedSupplyAirFlowRatio(bool returnIP=false) const;

    bool isMediumSpeedSupplyAirFlowRatioDefaulted() const;

    boost::optional<double> maximumOutdoorAirFlowRate() const;

    OSOptionalQuantity getMaximumOutdoorAirFlowRate(bool returnIP=false) const;

    bool isMaximumOutdoorAirFlowRateAutosized() const;

    boost::optional<Schedule> outdoorAirSchedule() const;

    std::string outdoorAirMixerObjectType() const;

    std::string outdoorAirMixerName() const;

    HVACComponent supplyAirFan() const;

    HVACComponent coolingCoil() const;

    boost::optional<double> maximumColdWaterFlowRate() const;

    OSOptionalQuantity getMaximumColdWaterFlowRate(bool returnIP=false) const;

    bool isMaximumColdWaterFlowRateAutosized() const;

    double minimumColdWaterFlowRate() const;

    Quantity getMinimumColdWaterFlowRate(bool returnIP=false) const;

    bool isMinimumColdWaterFlowRateDefaulted() const;

    double coolingConvergenceTolerance() const;

    Quantity getCoolingConvergenceTolerance(bool returnIP=false) const;

    bool isCoolingConvergenceToleranceDefaulted() const;

    HVACComponent heatingCoil() const;

    boost::optional<double> maximumHotWaterFlowRate() const;

    OSOptionalQuantity getMaximumHotWaterFlowRate(bool returnIP=false) const;

    bool isMaximumHotWaterFlowRateAutosized() const;

    double minimumHotWaterFlowRate() const;

    Quantity getMinimumHotWaterFlowRate(bool returnIP=false) const;

    bool isMinimumHotWaterFlowRateDefaulted() const;

    double heatingConvergenceTolerance() const;

    Quantity getHeatingConvergenceTolerance(bool returnIP=false) const;

    bool isHeatingConvergenceToleranceDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setCapacityControlMethod(std::string capacityControlMethod);

    void setMaximumSupplyAirFlowRate(boost::optional<double> maximumSupplyAirFlowRate);

    bool setMaximumSupplyAirFlowRate(const OSOptionalQuantity& maximumSupplyAirFlowRate);

    void autosizeMaximumSupplyAirFlowRate();

    bool setLowSpeedSupplyAirFlowRatio(double lowSpeedSupplyAirFlowRatio);

    bool setLowSpeedSupplyAirFlowRatio(const Quantity& lowSpeedSupplyAirFlowRatio);

    void resetLowSpeedSupplyAirFlowRatio();

    bool setMediumSpeedSupplyAirFlowRatio(double mediumSpeedSupplyAirFlowRatio);

    bool setMediumSpeedSupplyAirFlowRatio(const Quantity& mediumSpeedSupplyAirFlowRatio);

    void resetMediumSpeedSupplyAirFlowRatio();

    void setMaximumOutdoorAirFlowRate(boost::optional<double> maximumOutdoorAirFlowRate);

    bool setMaximumOutdoorAirFlowRate(const OSOptionalQuantity& maximumOutdoorAirFlowRate);

    void autosizeMaximumOutdoorAirFlowRate();

    bool setOutdoorAirSchedule(Schedule& schedule);

    void resetOutdoorAirSchedule();

    bool setOutdoorAirMixerObjectType(std::string outdoorAirMixerObjectType);

    void setOutdoorAirMixerName(std::string outdoorAirMixerName);

    bool setSupplyAirFan( HVACComponent & fan );

    bool setCoolingCoil( HVACComponent & coolingCoil );

    void setMaximumColdWaterFlowRate(boost::optional<double> maximumColdWaterFlowRate);

    bool setMaximumColdWaterFlowRate(const OSOptionalQuantity& maximumColdWaterFlowRate);

    void autosizeMaximumColdWaterFlowRate();

    void setMinimumColdWaterFlowRate(double minimumColdWaterFlowRate);

    bool setMinimumColdWaterFlowRate(const Quantity& minimumColdWaterFlowRate);

    void resetMinimumColdWaterFlowRate();

    bool setCoolingConvergenceTolerance(double coolingConvergenceTolerance);

    bool setCoolingConvergenceTolerance(const Quantity& coolingConvergenceTolerance);

    void resetCoolingConvergenceTolerance();

    bool setHeatingCoil( HVACComponent & heatingCoil );

    void setMaximumHotWaterFlowRate(boost::optional<double> maximumHotWaterFlowRate);

    bool setMaximumHotWaterFlowRate(const OSOptionalQuantity& maximumHotWaterFlowRate);

    void autosizeMaximumHotWaterFlowRate();

    void setMinimumHotWaterFlowRate(double minimumHotWaterFlowRate);

    bool setMinimumHotWaterFlowRate(const Quantity& minimumHotWaterFlowRate);

    void resetMinimumHotWaterFlowRate();

    bool setHeatingConvergenceTolerance(double heatingConvergenceTolerance);

    bool setHeatingConvergenceTolerance(const Quantity& heatingConvergenceTolerance);

    void resetHeatingConvergenceTolerance();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACFourPipeFanCoil");

    std::vector<std::string> capacityControlMethodValues() const;
    openstudio::OSOptionalQuantity maximumSupplyAirFlowRate_SI() const;
    openstudio::OSOptionalQuantity maximumSupplyAirFlowRate_IP() const;
    openstudio::Quantity lowSpeedSupplyAirFlowRatio_SI() const;
    openstudio::Quantity lowSpeedSupplyAirFlowRatio_IP() const;
    openstudio::Quantity mediumSpeedSupplyAirFlowRatio_SI() const;
    openstudio::Quantity mediumSpeedSupplyAirFlowRatio_IP() const;
    openstudio::OSOptionalQuantity maximumOutdoorAirFlowRate_SI() const;
    openstudio::OSOptionalQuantity maximumOutdoorAirFlowRate_IP() const;
    std::vector<std::string> outdoorAirMixerObjectTypeValues() const;
    openstudio::OSOptionalQuantity maximumColdWaterFlowRate_SI() const;
    openstudio::OSOptionalQuantity maximumColdWaterFlowRate_IP() const;
    openstudio::Quantity minimumColdWaterFlowRate_SI() const;
    openstudio::Quantity minimumColdWaterFlowRate_IP() const;
    openstudio::Quantity coolingConvergenceTolerance_SI() const;
    openstudio::Quantity coolingConvergenceTolerance_IP() const;
    openstudio::OSOptionalQuantity maximumHotWaterFlowRate_SI() const;
    openstudio::OSOptionalQuantity maximumHotWaterFlowRate_IP() const;
    openstudio::Quantity minimumHotWaterFlowRate_SI() const;
    openstudio::Quantity minimumHotWaterFlowRate_IP() const;
    openstudio::Quantity heatingConvergenceTolerance_SI() const;
    openstudio::Quantity heatingConvergenceTolerance_IP() const;

    // optional getters so can remove() if constructor fails
    boost::optional<Schedule> optionalAvailabilitySchedule() const;
    boost::optional<HVACComponent> optionalSupplyAirFan() const;
    boost::optional<HVACComponent> optionalHeatingCoil() const;
    boost::optional<HVACComponent> optionalCoolingCoil() const;

    boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> outdoorAirScheduleAsModelObject() const;
    boost::optional<ModelObject> supplyAirFanAsModelObject() const;
    boost::optional<ModelObject> coolingCoilAsModelObject() const;
    boost::optional<ModelObject> heatingCoilAsModelObject() const;

    bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setOutdoorAirScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setSupplyAirFanAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACFOURPIPEFANCOIL_IMPL_HPP

