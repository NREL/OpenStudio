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

#ifndef MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_IMPL_HPP
#define MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_IMPL_HPP

#include "ModelAPI.hpp"
#include "AvailabilityManager_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class Schedule;
  class Curve;
  class ModelObject;

  namespace detail {

    /** AvailabilityManagerHybridVentilation_Impl is a ModelObject_Impl that is the implementation class for AvailabilityManagerHybridVentilation.*/
    class MODEL_API AvailabilityManagerHybridVentilation_Impl : public AvailabilityManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AvailabilityManagerHybridVentilation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AvailabilityManagerHybridVentilation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AvailabilityManagerHybridVentilation_Impl(const AvailabilityManagerHybridVentilation_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AvailabilityManagerHybridVentilation_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ModelObject clone(Model model) const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<ThermalZone> controlledZone() const;

      Schedule ventilationControlModeSchedule() const;

      bool useWeatherFileRainIndicators() const;

      double maximumWindSpeed() const;

      double minimumOutdoorTemperature() const;

      double maximumOutdoorTemperature() const;

      double minimumOutdoorEnthalpy() const;

      double maximumOutdoorEnthalpy() const;

      double minimumOutdoorDewpoint() const;

      double maximumOutdoorDewpoint() const;

      Schedule minimumOutdoorVentilationAirSchedule() const;

      boost::optional<Curve> openingFactorFunctionofWindSpeedCurve() const;

      double minimumHVACOperationTime() const;

      double minimumVentilationTime() const;

      boost::optional<Schedule> airflowNetworkControlTypeSchedule() const;

      boost::optional<Schedule> simpleAirflowControlTypeSchedule() const;

      boost::optional<ModelObject> zoneVentilationObject() const;

      //@}
      /** @name Setters */
      //@{

      bool setControlledZone(const boost::optional<ThermalZone>& thermalZone);

      void resetControlledZone();

      bool setVentilationControlModeSchedule(Schedule& schedule);

      bool setUseWeatherFileRainIndicators(bool useWeatherFileRainIndicators);

      bool setMaximumWindSpeed(double maximumWindSpeed);

      bool setMinimumOutdoorTemperature(double minimumOutdoorTemperature);

      bool setMaximumOutdoorTemperature(double maximumOutdoorTemperature);

      bool setMinimumOutdoorEnthalpy(double minimumOutdoorEnthalpy);

      bool setMaximumOutdoorEnthalpy(double maximumOutdoorEnthalpy);

      bool setMinimumOutdoorDewpoint(double minimumOutdoorDewpoint);

      bool setMaximumOutdoorDewpoint(double maximumOutdoorDewpoint);

      bool setMinimumOutdoorVentilationAirSchedule(Schedule& schedule);

      bool setOpeningFactorFunctionofWindSpeedCurve(const boost::optional<Curve>& curve);

      void resetOpeningFactorFunctionofWindSpeedCurve();

      bool setMinimumHVACOperationTime(double minimumHVACOperationTime);

      bool setMinimumVentilationTime(double minimumVentilationTime);

      bool setAirflowNetworkControlTypeSchedule(Schedule& schedule);
      void resetAirflowNetworkControlTypeSchedule();

      bool setSimpleAirflowControlTypeSchedule(Schedule& schedule);
      void resetSimpleAirflowControlTypeSchedule();

      bool setZoneVentilationObject(const ModelObject& zv);
      void resetZoneVentilationObject();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AvailabilityManagerHybridVentilation");

      // TODO: Check the return types of these methods.
      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalVentilationControlModeSchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AVAILABILITYMANAGERHYBRIDVENTILATION_IMPL_HPP
