/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

    virtual ModelObject clone(Model model) const override;

    virtual std::vector<IdfObject> remove() override;

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual std::vector<ModelObject> children() const override;

    virtual unsigned inletPort() const override;

    virtual unsigned outletPort() const override;

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

    boost::optional<Schedule> supplyAirFanOperatingModeSchedule() const;

    bool setSupplyAirFanOperatingModeSchedule(Schedule& schedule);

    void resetSupplyAirFanOperatingModeSchedule();

    boost::optional<double> minimumSupplyAirTemperatureInCoolingMode() const;

    bool isMinimumSupplyAirTemperatureInCoolingModeAutosized() const;

    void autosizeMinimumSupplyAirTemperatureInCoolingMode();

    bool setMinimumSupplyAirTemperatureInCoolingMode(double minimumSupplyAirTemperatureInCoolingMode);

    boost::optional<double> maximumSupplyAirTemperatureInHeatingMode() const;

    bool isMaximumSupplyAirTemperatureInHeatingModeAutosized() const;

    void autosizeMaximumSupplyAirTemperatureInHeatingMode();

    bool setMaximumSupplyAirTemperatureInHeatingMode(double maximumSupplyAirTemperatureInHeatingMode);

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

