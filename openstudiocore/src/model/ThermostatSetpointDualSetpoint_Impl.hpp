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
#ifndef MODEL_THERMOSTATSETPOINTDUALSETPOINT_IMPL_HPP
#define MODEL_THERMOSTATSETPOINTDUALSETPOINT_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {
  class MODEL_API ThermostatSetpointDualSetpoint_Impl : public ModelObject_Impl {
    Q_OBJECT;

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> heatingSetpointTemperatureSchedule READ heatingSetpointTemperatureScheduleAsModelObject WRITE setHeatingSetpointTemperatureScheduleAsModelObject RESET resetHeatingSetpointTemperatureSchedule);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> coolingSetpointTemperatureSchedule READ coolingSetpointTemperatureScheduleAsModelObject WRITE setCoolingSetpointTemperatureScheduleAsModelObject RESET resetCoolingSetpointTemperatureSchedule);
   public:
    /** @name Constructors and Destructors */
    //@{

    ThermostatSetpointDualSetpoint_Impl(const IdfObject& idfObject,
                                        Model_Impl* model,
                                        bool keepHandle);

    ThermostatSetpointDualSetpoint_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    ThermostatSetpointDualSetpoint_Impl(const ThermostatSetpointDualSetpoint_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    virtual ~ThermostatSetpointDualSetpoint_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> heatingSetpointTemperatureSchedule() const;

    boost::optional<Schedule> coolingSetpointTemperatureSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setHeatingSetpointTemperatureSchedule(Schedule& schedule);

    void resetHeatingSetpointTemperatureSchedule();

    bool setCoolingSetpointTemperatureSchedule(Schedule& schedule);

    void resetCoolingSetpointTemperatureSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ThermostatSetpointDualSetpoint");

    boost::optional<ModelObject> heatingSetpointTemperatureScheduleAsModelObject() const;
    boost::optional<ModelObject> coolingSetpointTemperatureScheduleAsModelObject() const;

    bool setHeatingSetpointTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setCoolingSetpointTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_THERMOSTATSETPOINTDUALSETPOINT_IMPL_HPP
