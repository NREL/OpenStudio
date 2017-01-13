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

#ifndef MODEL_AVAILABILITYMANAGERNIGHTCYCLE_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERNIGHTCYCLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ThermalZone;

namespace detail {

  /** AvailabilityManagerNightCycle_Impl is a ModelObject_Impl that is the implementation class for AvailabilityManagerNightCycle.*/
  class MODEL_API AvailabilityManagerNightCycle_Impl : public AvailabilityManager_Impl {
    

    
    
    

    
    
    
    

    
    
    
    

    
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

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

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

