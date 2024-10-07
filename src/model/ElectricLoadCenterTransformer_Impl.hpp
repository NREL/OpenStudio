/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

      virtual ~ElectricLoadCenterTransformer_Impl() override = default;

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
