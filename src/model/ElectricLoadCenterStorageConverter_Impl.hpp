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

#ifndef MODEL_ELECTRICLOADCENTERSTORAGECONVERTER_IMPL_HPP
#define MODEL_ELECTRICLOADCENTERSTORAGECONVERTER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;  //UniVariateCurves;
  class ThermalZone;
  class ElectricLoadCenterDistribution;

  namespace detail {

    /** ElectricLoadCenterStorageConverter_Impl is a ParentObject_Impl that is the implementation class for ElectricLoadCenterStorageConverter.*/
    class MODEL_API ElectricLoadCenterStorageConverter_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ElectricLoadCenterStorageConverter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ElectricLoadCenterStorageConverter_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ElectricLoadCenterStorageConverter_Impl(const ElectricLoadCenterStorageConverter_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ElectricLoadCenterStorageConverter_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

      Schedule availabilitySchedule() const;
      bool isAvailabilityScheduleDefaulted() const;

      std::string powerConversionEfficiencyMethod() const;

      // Required if powerConversionEfficiencyMethod == "SimpleFixed"
      boost::optional<double> simpleFixedEfficiency() const;

      // Required if powerConversionEfficiencyMethod == "FunctionOfPower"
      boost::optional<double> designMaximumContinuousInputPower() const;

      // Required if powerConversionEfficiencyMethod == "FunctionOfPower"
      // TODO: Later may need to add support for Tables. (UniVariateCurves, UniVariateTables.)
      boost::optional<Curve> efficiencyFunctionofPowerCurve() const;

      // Defaults to 0.0
      double ancillaryPowerConsumedInStandby() const;

      bool isAncillaryPowerConsumedInStandbyDefaulted() const;

      boost::optional<ThermalZone> thermalZone() const;

      // Defualts to 0.0, because E+ doc says that if omitted, all heat gains are conductive
      double radiativeFraction() const;

      bool isRadiativeFractionDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      // TODO: Check argument type. From object lists, some candidates are: Schedule.
      // Note Schedules are passed by reference, not const reference.
      bool setAvailabilitySchedule(Schedule& schedule);
      void resetAvailabilitySchedule();

      // No Setter, will be done when calling one of the three following functions
      //bool setPowerConversionEfficiencyMethod(const std::string& powerConversionEfficiencyMethod);

      bool setSimpleFixedEfficiency(double simpleFixedEfficiency);

      bool setDesignMaximumContinuousInputPower(double designMaximumContinuousInputPower);

      bool setEfficiencyFunctionofPowerCurve(const Curve& efficiencyFunctionofPowerCurve);

      bool setAncillaryPowerConsumedInStandby(double ancillaryPowerConsumedInStandby);

      void resetAncillaryPowerConsumedInStandby();

      bool setThermalZone(const ThermalZone& thermalZone);

      void resetThermalZone();

      bool setRadiativeFraction(double radiativeFraction);

      void resetRadiativeFraction();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ElectricLoadCenterStorageConverter");
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICLOADCENTERSTORAGECONVERTER_IMPL_HPP
