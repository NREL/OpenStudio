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

#ifndef MODEL_AIRFLOWNETWORKOCCUPANTVENTILATIONCONTROL_IMPL_HPP
#define MODEL_AIRFLOWNETWORKOCCUPANTVENTILATIONCONTROL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Curve;
  class Schedule;

  namespace detail {

    /** AirflowNetworkOccupantVentilationControl_Impl is a ModelObject_Impl that is the implementation class for AirflowNetworkOccupantVentilationControl.*/
    class MODEL_API AirflowNetworkOccupantVentilationControl_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkOccupantVentilationControl_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkOccupantVentilationControl_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkOccupantVentilationControl_Impl(const AirflowNetworkOccupantVentilationControl_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkOccupantVentilationControl_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      double minimumOpeningTime() const;

      bool isMinimumOpeningTimeDefaulted() const;

      double minimumClosingTime() const;

      bool isMinimumClosingTimeDefaulted() const;

      Curve thermalComfortLowTemperatureCurve() const;

      double thermalComfortTemperatureBoundaryPoint() const;

      bool isThermalComfortTemperatureBoundaryPointDefaulted() const;

      boost::optional<Curve> thermalComfortHighTemperatureCurve() const;

      double maximumPredictedPercentageofDissatisfiedThreshold() const;

      bool isMaximumPredictedPercentageofDissatisfiedThresholdDefaulted() const;

      bool occupancyCheck() const;

      bool isOccupancyCheckDefaulted() const;

      boost::optional<Schedule> openingProbabilitySchedule() const;

      boost::optional<Schedule> closingProbabilitySchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setMinimumOpeningTime(double minimumOpeningTime);

      void resetMinimumOpeningTime();

      bool setMinimumClosingTime(double minimumClosingTime);

      void resetMinimumClosingTime();

      bool setThermalComfortLowTemperatureCurve(const Curve& curve);

      //void resetThermalComfortLowTemperatureCurve();

      bool setThermalComfortTemperatureBoundaryPoint(double thermalComfortTemperatureBoundaryPoint);

      void resetThermalComfortTemperatureBoundaryPoint();

      bool setThermalComfortHighTemperatureCurve(const Curve& curve);

      void resetThermalComfortHighTemperatureCurve();

      bool setMaximumPredictedPercentageofDissatisfiedThreshold(double maximumPredictedPercentageofDissatisfiedThreshold);

      void resetMaximumPredictedPercentageofDissatisfiedThreshold();

      void setOccupancyCheck(bool occupancyCheck);

      void resetOccupancyCheck();

      bool setOpeningProbabilitySchedule(Schedule& schedule);

      void resetOpeningProbabilitySchedule();

      bool setClosingProbabilitySchedule(Schedule& schedule);

      void resetClosingProbabilitySchedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkOccupantVentilationControl");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKOCCUPANTVENTILATIONCONTROL_IMPL_HPP
