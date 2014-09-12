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

#ifndef MODEL_ZONECONTROLTHERMOSTATSTAGEDDUALSETPOINT_IMPL_HPP
#define MODEL_ZONECONTROLTHERMOSTATSTAGEDDUALSETPOINT_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/Thermostat_Impl.hpp>

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** ZoneControlThermostatStagedDualSetpoint_Impl is a ModelObject_Impl that is the implementation class for ZoneControlThermostatStagedDualSetpoint.*/
  class MODEL_API ZoneControlThermostatStagedDualSetpoint_Impl : public Thermostat_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    ZoneControlThermostatStagedDualSetpoint_Impl(const IdfObject& idfObject,
                                                 Model_Impl* model,
                                                 bool keepHandle);

    ZoneControlThermostatStagedDualSetpoint_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle);

    ZoneControlThermostatStagedDualSetpoint_Impl(const ZoneControlThermostatStagedDualSetpoint_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle);

    virtual ~ZoneControlThermostatStagedDualSetpoint_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    int numberofHeatingStages() const;

    boost::optional<Schedule> heatingTemperatureSetpointSchedule() const;

    double heatingThrottlingTemperatureRange() const;

    double stage1HeatingTemperatureOffset() const;

    double stage2HeatingTemperatureOffset() const;

    double stage3HeatingTemperatureOffset() const;

    double stage4HeatingTemperatureOffset() const;

    int numberofCoolingStages() const;

    boost::optional<Schedule> coolingTemperatureSetpointBaseSchedule() const;

    double coolingThrottlingTemperatureRange() const;

    double stage1CoolingTemperatureOffset() const;

    double stage2CoolingTemperatureOffset() const;

    double stage3CoolingTemperatureOffset() const;

    double stage4CoolingTemperatureOffset() const;

    //@}
    /** @name Setters */
    //@{

    bool setNumberofHeatingStages(int numberofHeatingStages);

    bool setHeatingTemperatureSetpointSchedule(Schedule& schedule);

    void resetHeatingTemperatureSetpointSchedule();

    bool setHeatingThrottlingTemperatureRange(double heatingThrottlingTemperatureRange);

    bool setStage1HeatingTemperatureOffset(double stage1HeatingTemperatureOffset);

    bool setStage2HeatingTemperatureOffset(double stage2HeatingTemperatureOffset);

    bool setStage3HeatingTemperatureOffset(double stage3HeatingTemperatureOffset);

    bool setStage4HeatingTemperatureOffset(double stage4HeatingTemperatureOffset);

    bool setNumberofCoolingStages(int numberofCoolingStages);

    bool setCoolingTemperatureSetpointBaseSchedule(Schedule& schedule);

    void resetCoolingTemperatureSetpointBaseSchedule();

    bool setCoolingThrottlingTemperatureRange(double coolingThrottlingTemperatureRange);

    bool setStage1CoolingTemperatureOffset(double stage1CoolingTemperatureOffset);

    bool setStage2CoolingTemperatureOffset(double stage2CoolingTemperatureOffset);

    bool setStage3CoolingTemperatureOffset(double stage3CoolingTemperatureOffset);

    bool setStage4CoolingTemperatureOffset(double stage4CoolingTemperatureOffset);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ZoneControlThermostatStagedDualSetpoint");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ZONECONTROLTHERMOSTATSTAGEDDUALSETPOINT_IMPL_HPP

