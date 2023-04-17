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

#ifndef MODEL_GENERATORWINDTURBINE_IMPL_HPP
#define MODEL_GENERATORWINDTURBINE_IMPL_HPP

#include "ModelAPI.hpp"
#include "Generator_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** GeneratorWindTurbine_Impl is a Generator_Impl that is the implementation class for GeneratorWindTurbine.*/
    class MODEL_API GeneratorWindTurbine_Impl : public Generator_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorWindTurbine_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorWindTurbine_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorWindTurbine_Impl(const GeneratorWindTurbine_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorWindTurbine_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::string generatorObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual boost::optional<double> ratedElectricPowerOutput() const override;

      virtual boost::optional<Schedule> availabilitySchedule() const override;

      virtual boost::optional<double> ratedThermaltoElectricalPowerRatio() const override;

      //@}
      /** @name Getters */
      //@{

      std::string rotorType() const;

      std::string powerControl() const;

      double ratedRotorSpeed() const;

      double rotorDiameter() const;

      double overallHeight() const;

      int numberofBlades() const;

      double ratedPower() const;

      double ratedWindSpeed() const;

      double cutInWindSpeed() const;

      double cutOutWindSpeed() const;

      double fractionSystemEfficiency() const;

      double maximumTipSpeedRatio() const;

      double maximumPowerCoefficient() const;

      boost::optional<double> annualLocalAverageWindSpeed() const;

      double heightforLocalAverageWindSpeed() const;

      double bladeChordArea() const;

      double bladeDragCoefficient() const;

      double bladeLiftCoefficient() const;

      double powerCoefficientC1() const;

      double powerCoefficientC2() const;

      double powerCoefficientC3() const;

      double powerCoefficientC4() const;

      double powerCoefficientC5() const;

      double powerCoefficientC6() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setRotorType(const std::string& rotorType);

      bool setPowerControl(const std::string& powerControl);

      bool setRatedRotorSpeed(double ratedRotorSpeed);

      bool setRotorDiameter(double rotorDiameter);

      bool setOverallHeight(double overallHeight);

      bool setNumberofBlades(int numberofBlades);

      bool setRatedPower(double ratedPower);

      bool setRatedWindSpeed(double ratedWindSpeed);

      bool setCutInWindSpeed(double cutInWindSpeed);

      bool setCutOutWindSpeed(double cutOutWindSpeed);

      bool setFractionSystemEfficiency(double fractionSystemEfficiency);

      bool setMaximumTipSpeedRatio(double maximumTipSpeedRatio);

      bool setMaximumPowerCoefficient(double maximumPowerCoefficient);

      bool setAnnualLocalAverageWindSpeed(double annualLocalAverageWindSpeed);

      void resetAnnualLocalAverageWindSpeed();

      bool setHeightforLocalAverageWindSpeed(double heightforLocalAverageWindSpeed);

      bool setBladeChordArea(double bladeChordArea);

      bool setBladeDragCoefficient(double bladeDragCoefficient);

      bool setBladeLiftCoefficient(double bladeLiftCoefficient);

      bool setPowerCoefficientC1(double powerCoefficientC1);

      bool setPowerCoefficientC2(double powerCoefficientC2);

      bool setPowerCoefficientC3(double powerCoefficientC3);

      bool setPowerCoefficientC4(double powerCoefficientC4);

      bool setPowerCoefficientC5(double powerCoefficientC5);

      bool setPowerCoefficientC6(double powerCoefficientC6);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorWindTurbine");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORWINDTURBINE_IMPL_HPP
