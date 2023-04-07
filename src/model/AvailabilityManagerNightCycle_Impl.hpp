/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_AVAILABILITYMANAGERNIGHTCYCLE_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERNIGHTCYCLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"
#include "ModelObjectList.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;
  class AirLoopHVAC;

  namespace detail {

    /** AvailabilityManagerNightCycle_Impl is a ModelObject_Impl that is the implementation class for AvailabilityManagerNightCycle.*/
    class MODEL_API AvailabilityManagerNightCycle_Impl : public AvailabilityManager_Impl
    {

     public:
      AvailabilityManagerNightCycle_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AvailabilityManagerNightCycle_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AvailabilityManagerNightCycle_Impl(const AvailabilityManagerNightCycle_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AvailabilityManagerNightCycle_Impl() = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      // Cloning should ensure we do have modelObjectLists for control zones, but they should be empty
      // as it is not connected to an AirLoopHVAC (yet) and it wouldn't make sense to have zones there
      virtual ModelObject clone(Model model) const override;

      // Clears the ModelObjectLists, then remove them
      virtual std::vector<IdfObject> remove() override;

      boost::optional<AirLoopHVAC> airLoopHVAC() const;

      Schedule applicabilitySchedule() const;
      bool setApplicabilitySchedule(Schedule& schedule);

      boost::optional<Schedule> fanSchedule() const;
      // Not implemented, it uses AirLoopHVAC::availabilitySchedule
      // bool setFanSchedule(Schedule& schedule);

      std::string controlType() const;
      bool isControlTypeDefaulted() const;
      bool setControlType(const std::string& controlType);
      void resetControlType();

      double thermostatTolerance() const;
      bool isThermostatToleranceDefaulted() const;
      bool setThermostatTolerance(double thermostatTolerance);
      void resetThermostatTolerance();

      std::string cyclingRunTimeControlType() const;
      bool isCyclingRunTimeControlTypeDefaulted() const;
      bool setCyclingRunTimeControlType(const std::string& cyclingRunTimeControlType);
      void resetCyclingRunTimeControlType();

      double cyclingRunTime() const;
      bool isCyclingRunTimeDefaulted() const;
      bool setCyclingRunTime(double cyclingRunTime);
      void resetCyclingRunTime();

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

     private:
      REGISTER_LOGGER("openstudio.model.AvailabilityManagerNightCycle");

      boost::optional<Schedule> optionalApplicabilitySchedule() const;

      std::vector<std::string> controlTypeValues() const;
      std::vector<std::string> cyclingRunTimeControlTypeValues() const;

      ModelObjectList controlThermalZoneList() const;
      void clearControlThermalZoneList();

      ModelObjectList coolingControlThermalZoneList() const;
      void clearCoolingControlThermalZoneList();

      ModelObjectList heatingControlThermalZoneList() const;
      void clearHeatingControlThermalZoneList();

      ModelObjectList heatingZoneFansOnlyThermalZoneList() const;
      void clearHeatingZoneFansOnlyThermalZoneList();
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERNIGHTCYCLE_IMPL_HPP
