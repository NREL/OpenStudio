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

#ifndef MODEL_FANZONEEXHAUST_IMPL_HPP
#define MODEL_FANZONEEXHAUST_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** FanZoneExhaust_Impl is a ZoneHVACComponent_Impl that is the implementation class for FanZoneExhaust.*/
    class MODEL_API FanZoneExhaust_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      FanZoneExhaust_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      FanZoneExhaust_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      FanZoneExhaust_Impl(const FanZoneExhaust_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~FanZoneExhaust_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual boost::optional<ThermalZone> thermalZone() const override;

      virtual bool addToThermalZone(ThermalZone& thermalZone) override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      double fanTotalEfficiency() const;

      double pressureRise() const;

      boost::optional<double> maximumFlowRate() const;

      std::string endUseSubcategory() const;

      boost::optional<Schedule> flowFractionSchedule() const;

      std::string systemAvailabilityManagerCouplingMode() const;

      boost::optional<Schedule> minimumZoneTemperatureLimitSchedule() const;

      boost::optional<Schedule> balancedExhaustFractionSchedule() const;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setFanTotalEfficiency(double fanTotalEfficiency);

      bool setPressureRise(double pressureRise);

      bool setMaximumFlowRate(boost::optional<double> maximumFlowRate);

      void resetMaximumFlowRate();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      bool setFlowFractionSchedule(Schedule& schedule);

      void resetFlowFractionSchedule();

      bool setSystemAvailabilityManagerCouplingMode(const std::string& systemAvailabilityManagerCouplingMode);

      bool setMinimumZoneTemperatureLimitSchedule(Schedule& schedule);

      void resetMinimumZoneTemperatureLimitSchedule();

      bool setBalancedExhaustFractionSchedule(Schedule& schedule);

      void resetBalancedExhaustFractionSchedule();

      //@}
      /** @name Other */
      //@{

      AirflowNetworkZoneExhaustFan getAirflowNetworkZoneExhaustFan(const AirflowNetworkCrack& crack);
      boost::optional<AirflowNetworkZoneExhaustFan> airflowNetworkZoneExhaustFan() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.FanZoneExhaust");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANZONEEXHAUST_IMPL_HPP
