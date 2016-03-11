/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_ZONEHVACENERGYRECOVERYVENTILATORCONTROLLER_IMPL_HPP
#define MODEL_ZONEHVACENERGYRECOVERYVENTILATORCONTROLLER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

class Curve;
class Schedule;

namespace detail {

  /** ZoneHVACEnergyRecoveryVentilatorController_Impl is a ParentObject_Impl that is the implementation class for ZoneHVACEnergyRecoveryVentilatorController.*/
  class MODEL_API ZoneHVACEnergyRecoveryVentilatorController_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneHVACEnergyRecoveryVentilatorController_Impl(const IdfObject& idfObject,
                                                    Model_Impl* model,
                                                    bool keepHandle);

    ZoneHVACEnergyRecoveryVentilatorController_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                    Model_Impl* model,
                                                    bool keepHandle);

    ZoneHVACEnergyRecoveryVentilatorController_Impl(const ZoneHVACEnergyRecoveryVentilatorController_Impl& other,
                                                    Model_Impl* model,
                                                    bool keepHandle);

    virtual ~ZoneHVACEnergyRecoveryVentilatorController_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

    virtual std::vector<ModelObject> children() const override;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> temperatureHighLimit() const;

    boost::optional<double> temperatureLowLimit() const;

    boost::optional<double> enthalpyHighLimit() const;

    boost::optional<double> dewpointTemperatureLimit() const;

    boost::optional<Curve> electronicEnthalpyLimitCurve() const;

    std::string exhaustAirTemperatureLimit() const;

    std::string exhaustAirEnthalpyLimit() const;

    boost::optional<Schedule> timeofDayEconomizerFlowControlSchedule() const;

    bool highHumidityControlFlag() const;

    double highHumidityOutdoorAirFlowRatio() const;

    bool controlHighIndoorHumidityBasedonOutdoorHumidityRatio() const;

    //@}
    /** @name Setters */
    //@{

    void setTemperatureHighLimit(boost::optional<double> temperatureHighLimit);

    void resetTemperatureHighLimit();

    void setTemperatureLowLimit(boost::optional<double> temperatureLowLimit);

    void resetTemperatureLowLimit();

    void setEnthalpyHighLimit(boost::optional<double> enthalpyHighLimit);

    void resetEnthalpyHighLimit();

    void setDewpointTemperatureLimit(boost::optional<double> dewpointTemperatureLimit);

    void resetDewpointTemperatureLimit();

    bool setElectronicEnthalpyLimitCurve(const boost::optional<Curve>& curve);

    void resetElectronicEnthalpyLimitCurve();

    bool setExhaustAirTemperatureLimit(std::string exhaustAirTemperatureLimit);

    bool setExhaustAirEnthalpyLimit(std::string exhaustAirEnthalpyLimit);

    bool setTimeofDayEconomizerFlowControlSchedule(Schedule& schedule);

    void resetTimeofDayEconomizerFlowControlSchedule();

    void setHighHumidityControlFlag(bool highHumidityControlFlag);

    bool setHighHumidityOutdoorAirFlowRatio(double highHumidityOutdoorAirFlowRatio);

    void setControlHighIndoorHumidityBasedonOutdoorHumidityRatio(bool controlHighIndoorHumidityBasedonOutdoorHumidityRatio);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ZoneHVACEnergyRecoveryVentilatorController");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONEHVACENERGYRECOVERYVENTILATORCONTROLLER_IMPL_HPP

