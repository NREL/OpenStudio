/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_AVAILABILITYMANAGERNIGHTCYCLE_HPP
#define MODEL_AVAILABILITYMANAGERNIGHTCYCLE_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;
  class AirLoopHVAC;

  namespace detail {

    class AvailabilityManagerNightCycle_Impl;

  }  // namespace detail

  /** AvailabilityManagerNightCycle is a ModelObject that wraps the OpenStudio IDD object 'OS:AvailabilityManager:NightCycle'. */
  class MODEL_API AvailabilityManagerNightCycle : public AvailabilityManager
  {
   public:
    explicit AvailabilityManagerNightCycle(const Model& model);

    virtual ~AvailabilityManagerNightCycle() {}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlTypeValues();

    boost::optional<AirLoopHVAC> airLoopHVAC() const;

    Schedule applicabilitySchedule() const;
    bool setApplicabilitySchedule(Schedule& schedule);

    /** Helper getter which will fetch the AirLoopHVAC::AvailabilitySchedule */
    boost::optional<Schedule> fanSchedule() const;

    std::string controlType() const;
    bool setControlType(std::string controlType);
    void resetControlType();
    bool isControlTypeDefaulted() const;

    double thermostatTolerance() const;
    bool setThermostatTolerance(double thermostatTolerance);
    bool isThermostatToleranceDefaulted() const;
    void resetThermostatTolerance();

    double cyclingRunTime() const;
    bool setCyclingRunTime(double cyclingRunTime);
    void resetCyclingRunTime();
    bool isCyclingRunTimeDefaulted() const;

    static std::vector<std::string> cyclingRunTimeControlTypeValues();
    std::string cyclingRunTimeControlType() const;
    bool isCyclingRunTimeControlTypeDefaulted() const;
    bool setCyclingRunTimeControlType(std::string cyclingRunTimeControlType);
    void resetCyclingRunTimeControlType();

    /** \deprecated AvailabilityManagerNightCycle::controlThermalZone has been deprecated and will be removed in a future release, please use AvailabilityManagerNightCycle::controlThermalZones\n
    * Get the controlThermalZone if it's unique, otherwise returns the first one found and issues a warning. **/
    OS_DEPRECATED boost::optional<ThermalZone> controlThermalZone() const;

    /** \deprecated AvailabilityManagerNightCycle::setControlThermalZone has been deprecated and will be removed in a future release, please use AvailabilityManagerNightCycle::setControlThermalZones\n
  * sets the list of controlThermalZones by calling setControlThermalZones([thermalZone]) **/
    OS_DEPRECATED bool setControlThermalZone(const ThermalZone& thermalZone);

    /** \deprecated AvailabilityManagerNightCycle::resetControlThermalZone has been deprecated and will be removed in a future release, please use AvailabilityManagerNightCycle::resetControlThermalZones\n
  * Resets the list of controlThermalZones **/
    OS_DEPRECATED void resetControlThermalZone();

    std::vector<ThermalZone> controlThermalZones() const;
    bool setControlThermalZones(const std::vector<ThermalZone>& thermalZones);
    void resetControlThermalZones();

    std::vector<ThermalZone> coolingControlThermalZones() const;
    bool setCoolingControlThermalZones(const std::vector<ThermalZone>& thermalZones);
    void resetCoolingControlThermalZones();

    std::vector<ThermalZone> heatingControlThermalZones() const;
    bool setHeatingControlThermalZones(const std::vector<ThermalZone>& thermalZones);
    void resetHeatingControlThermalZones();

    std::vector<ThermalZone> heatingZoneFansOnlyThermalZones() const;
    bool setHeatingZoneFansOnlyThermalZones(const std::vector<ThermalZone>& thermalZones);
    void resetHeatingZoneFansOnlyThermalZones();

   protected:
    /// @cond
    typedef detail::AvailabilityManagerNightCycle_Impl ImplType;

    explicit AvailabilityManagerNightCycle(std::shared_ptr<detail::AvailabilityManagerNightCycle_Impl> impl);

    friend class detail::AvailabilityManagerNightCycle_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.AvailabilityManagerNightCycle");
  };

  /** \relates AvailabilityManagerNightCycle*/
  typedef boost::optional<AvailabilityManagerNightCycle> OptionalAvailabilityManagerNightCycle;

  /** \relates AvailabilityManagerNightCycle*/
  typedef std::vector<AvailabilityManagerNightCycle> AvailabilityManagerNightCycleVector;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERNIGHTCYCLE_HPP