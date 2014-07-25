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

#ifndef MODEL_AVAILABILITYMANAGERNIGHTCYCLE_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERNIGHTCYCLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  /** AvailabilityManagerNightCycle_Impl is a ModelObject_Impl that is the implementation class for AvailabilityManagerNightCycle.*/
  class MODEL_API AvailabilityManagerNightCycle_Impl : public ModelObject_Impl {
    Q_OBJECT;

    Q_PROPERTY(std::string controlType READ controlType WRITE setControlType RESET resetControlType);
    Q_PROPERTY(bool isControlTypeDefaulted READ isControlTypeDefaulted);
    Q_PROPERTY(std::vector<std::string> controlTypeValues READ controlTypeValues);

    Q_PROPERTY(double thermostatTolerance READ thermostatTolerance WRITE setThermostatTolerance RESET resetThermostatTolerance);
    Q_PROPERTY(openstudio::Quantity thermostatTolerance_SI READ thermostatTolerance_SI WRITE setThermostatTolerance RESET resetThermostatTolerance);
    Q_PROPERTY(openstudio::Quantity thermostatTolerance_IP READ thermostatTolerance_IP WRITE setThermostatTolerance RESET resetThermostatTolerance);
    Q_PROPERTY(bool isThermostatToleranceDefaulted READ isThermostatToleranceDefaulted);

    Q_PROPERTY(double cyclingRunTime READ cyclingRunTime WRITE setCyclingRunTime RESET resetCyclingRunTime);
    Q_PROPERTY(openstudio::Quantity cyclingRunTime_SI READ cyclingRunTime_SI WRITE setCyclingRunTime RESET resetCyclingRunTime);
    Q_PROPERTY(openstudio::Quantity cyclingRunTime_IP READ cyclingRunTime_IP WRITE setCyclingRunTime RESET resetCyclingRunTime);
    Q_PROPERTY(bool isCyclingRunTimeDefaulted READ isCyclingRunTimeDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> controlThermalZone READ controlThermalZoneAsModelObject WRITE setControlThermalZoneAsModelObject RESET resetControlThermalZone);
   public:

    AvailabilityManagerNightCycle_Impl(const IdfObject& idfObject,
                                       Model_Impl* model,
                                       bool keepHandle);

    AvailabilityManagerNightCycle_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    AvailabilityManagerNightCycle_Impl(const AvailabilityManagerNightCycle_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle);

    virtual ~AvailabilityManagerNightCycle_Impl() {}

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    Schedule applicabilitySchedule() const;

    Schedule fanSchedule() const;

    std::string controlType() const;

    bool isControlTypeDefaulted() const;

    double thermostatTolerance() const;

    Quantity getThermostatTolerance(bool returnIP=false) const;

    bool isThermostatToleranceDefaulted() const;

    double cyclingRunTime() const;

    Quantity getCyclingRunTime(bool returnIP=false) const;

    bool isCyclingRunTimeDefaulted() const;

    boost::optional<ThermalZone> controlThermalZone() const;

    bool setApplicabilitySchedule(Schedule& schedule);

    bool setFanSchedule(Schedule& schedule);

    bool setControlType(std::string controlType);

    void resetControlType();

    void setThermostatTolerance(double thermostatTolerance);

    bool setThermostatTolerance(const Quantity& thermostatTolerance);

    void resetThermostatTolerance();

    void setCyclingRunTime(double cyclingRunTime);

    bool setCyclingRunTime(const Quantity& cyclingRunTime);

    void resetCyclingRunTime();

    bool setControlThermalZone(const boost::optional<ThermalZone>& thermalZone);

    void resetControlThermalZone();

   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerNightCycle");

    boost::optional<Schedule> optionalApplicabilitySchedule() const;
    boost::optional<Schedule> optionalFanSchedule() const;

    std::vector<std::string> controlTypeValues() const;
    openstudio::Quantity thermostatTolerance_SI() const;
    openstudio::Quantity thermostatTolerance_IP() const;
    openstudio::Quantity cyclingRunTime_SI() const;
    openstudio::Quantity cyclingRunTime_IP() const;

    boost::optional<ModelObject> applicabilityScheduleAsModelObject() const;
    boost::optional<ModelObject> fanScheduleAsModelObject() const;
    boost::optional<ModelObject> controlThermalZoneAsModelObject() const;

    bool setApplicabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setFanScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setControlThermalZoneAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_AVAILABILITYMANAGERNIGHTCYCLE_IMPL_HPP

