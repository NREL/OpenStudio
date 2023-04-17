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

#ifndef MODEL_ELECTRICLOADCENTERTRANSFORMER_IMPL_HPP
#define MODEL_ELECTRICLOADCENTERTRANSFORMER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ThermalZone;
  class OutputMeter;

  namespace detail {

    /** ElectricLoadCenterTransformer_Impl is a ModelObject_Impl that is the implementation class for ElectricLoadCenterTransformer.*/
    class MODEL_API ElectricLoadCenterTransformer_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ElectricLoadCenterTransformer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ElectricLoadCenterTransformer_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ElectricLoadCenterTransformer_Impl(const ElectricLoadCenterTransformer_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ElectricLoadCenterTransformer_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      std::string transformerUsage() const;

      bool isTransformerUsageDefaulted() const;

      boost::optional<ThermalZone> zone() const;

      double radiativeFraction() const;

      bool isRadiativeFractionDefaulted() const;

      boost::optional<double> ratedCapacity() const;

      std::string phase() const;

      bool isPhaseDefaulted() const;

      std::string conductorMaterial() const;

      bool isConductorMaterialDefaulted() const;

      double fullLoadTemperatureRise() const;

      bool isFullLoadTemperatureRiseDefaulted() const;

      double fractionofEddyCurrentLosses() const;

      bool isFractionofEddyCurrentLossesDefaulted() const;

      std::string performanceInputMethod() const;

      bool isPerformanceInputMethodDefaulted() const;

      boost::optional<double> ratedNoLoadLoss() const;

      boost::optional<double> ratedLoadLoss() const;

      double nameplateEfficiency() const;

      bool isNameplateEfficiencyDefaulted() const;

      double perUnitLoadforNameplateEfficiency() const;

      bool isPerUnitLoadforNameplateEfficiencyDefaulted() const;

      double referenceTemperatureforNameplateEfficiency() const;

      bool isReferenceTemperatureforNameplateEfficiencyDefaulted() const;

      boost::optional<double> perUnitLoadforMaximumEfficiency() const;

      bool considerTransformerLossforUtilityCost() const;

      bool isConsiderTransformerLossforUtilityCostDefaulted() const;

      std::vector<std::string> meters() const;

      boost::optional<std::string> getMeter(unsigned index) const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setTransformerUsage(const std::string& transformerUsage);

      void resetTransformerUsage();

      bool setZone(const ThermalZone& thermalZone);

      void resetZone();

      bool setRadiativeFraction(double radiativeFraction);

      void resetRadiativeFraction();

      bool setRatedCapacity(double ratedCapacity);

      void resetRatedCapacity();

      bool setPhase(const std::string& phase);

      void resetPhase();

      bool setConductorMaterial(const std::string& conductorMaterial);

      void resetConductorMaterial();

      bool setFullLoadTemperatureRise(double fullLoadTemperatureRise);

      void resetFullLoadTemperatureRise();

      bool setFractionofEddyCurrentLosses(double fractionofEddyCurrentLosses);

      void resetFractionofEddyCurrentLosses();

      bool setPerformanceInputMethod(const std::string& performanceInputMethod);

      void resetPerformanceInputMethod();

      bool setRatedNoLoadLoss(double ratedNoLoadLoss);

      void resetRatedNoLoadLoss();

      bool setRatedLoadLoss(double ratedLoadLoss);

      void resetRatedLoadLoss();

      bool setNameplateEfficiency(double nameplateEfficiency);

      void resetNameplateEfficiency();

      bool setPerUnitLoadforNameplateEfficiency(double perUnitLoadforNameplateEfficiency);

      void resetPerUnitLoadforNameplateEfficiency();

      bool setReferenceTemperatureforNameplateEfficiency(double referenceTemperatureforNameplateEfficiency);

      void resetReferenceTemperatureforNameplateEfficiency();

      bool setPerUnitLoadforMaximumEfficiency(double perUnitLoadforMaximumEfficiency);

      void resetPerUnitLoadforMaximumEfficiency();

      bool setConsiderTransformerLossforUtilityCost(bool considerTransformerLossforUtilityCost);

      void resetConsiderTransformerLossforUtilityCost();

      void eraseMeters();

      bool eraseMeter(unsigned index);

      bool addMeter(const std::string& meterName);

      bool setMeter(const std::string& meterName, unsigned index);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ElectricLoadCenterTransformer");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICLOADCENTERTRANSFORMER_IMPL_HPP
