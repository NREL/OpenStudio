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

#ifndef MODEL_AIRFLOWNETWORKZONE_IMPL_HPP
#define MODEL_AIRFLOWNETWORKZONE_IMPL_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkNode_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class Schedule;
  class AirflowNetworkOccupantVentilationControl;

  namespace detail {

    /** AirflowNetworkZone_Impl is a AirflowNetworkNode_Impl that is the implementation class for AirflowNetworkZone.*/
    class MODEL_API AirflowNetworkZone_Impl : public AirflowNetworkNode_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkZone_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkZone_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkZone_Impl(const AirflowNetworkZone_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AirflowNetworkZone_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      ThermalZone thermalZone() const;

      std::string ventilationControlMode() const;

      bool isVentilationControlModeDefaulted() const;

      boost::optional<Schedule> ventilationControlZoneTemperatureSetpointSchedule() const;

      double minimumVentingOpenFactor() const;

      bool isMinimumVentingOpenFactorDefaulted() const;

      double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor() const;

      bool isIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const;

      double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor() const;

      bool isIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const;

      double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor() const;

      bool isIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactorDefaulted() const;

      double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor() const;

      bool isIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactorDefaulted() const;

      boost::optional<Schedule> ventingAvailabilitySchedule() const;

      std::string singleSidedWindPressureCoefficientAlgorithm() const;

      bool isSingleSidedWindPressureCoefficientAlgorithmDefaulted() const;

      double facadeWidth() const;

      bool isFacadeWidthDefaulted() const;

      boost::optional<AirflowNetworkOccupantVentilationControl> occupantVentilationControl() const;

      //@}
      /** @name Setters */
      //@{

      bool setThermalZone(const ThermalZone& thermalZone);

      void resetThermalZone();

      bool setVentilationControlMode(const std::string& ventilationControlMode);

      void resetVentilationControlMode();

      // Note Schedules are passed by reference, not const reference.
      bool setVentilationControlZoneTemperatureSetpointSchedule(Schedule& schedule);

      void resetVentilationControlZoneTemperatureSetpointSchedule();

      bool setMinimumVentingOpenFactor(double minimumVentingOpenFactor);

      void resetMinimumVentingOpenFactor();

      bool setIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor(
        double indoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor);

      void resetIndoorandOutdoorTemperatureDifferenceLowerLimitForMaximumVentingOpenFactor();

      bool setIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor(
        double indoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor);

      void resetIndoorandOutdoorTemperatureDifferenceUpperLimitforMinimumVentingOpenFactor();

      bool setIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor(
        double indoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor);

      void resetIndoorandOutdoorEnthalpyDifferenceLowerLimitForMaximumVentingOpenFactor();

      bool setIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor(
        double indoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor);

      void resetIndoorandOutdoorEnthalpyDifferenceUpperLimitforMinimumVentingOpenFactor();

      // Note Schedules are passed by reference, not const reference.
      bool setVentingAvailabilitySchedule(Schedule& schedule);

      void resetVentingAvailabilitySchedule();

      bool setSingleSidedWindPressureCoefficientAlgorithm(const std::string& singleSidedWindPressureCoefficientAlgorithm);

      void resetSingleSidedWindPressureCoefficientAlgorithm();

      bool setFacadeWidth(double facadeWidth);

      void resetFacadeWidth();

      bool setOccupantVentilationControl(const AirflowNetworkOccupantVentilationControl& airflowNetworkOccupantVentilationControl);

      void resetOccupantVentilationControl();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkZone");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<ThermalZone> optionalThermalZone() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKZONE_IMPL_HPP
