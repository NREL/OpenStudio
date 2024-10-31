/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ELECTRICLOADCENTERTRANSFORMER_HPP
#define MODEL_ELECTRICLOADCENTERTRANSFORMER_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class ThermalZone;
  class OutputMeter;

  namespace detail {

    class ElectricLoadCenterTransformer_Impl;

  }  // namespace detail

  /** ElectricLoadCenterTransformer is a ModelObject that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Transformer'. */
  class MODEL_API ElectricLoadCenterTransformer : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ElectricLoadCenterTransformer(const Model& model);

    virtual ~ElectricLoadCenterTransformer() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ElectricLoadCenterTransformer(const ElectricLoadCenterTransformer& other) = default;
    ElectricLoadCenterTransformer(ElectricLoadCenterTransformer&& other) = default;
    ElectricLoadCenterTransformer& operator=(const ElectricLoadCenterTransformer&) = default;
    ElectricLoadCenterTransformer& operator=(ElectricLoadCenterTransformer&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> transformerUsageValues();

    static std::vector<std::string> phaseValues();

    static std::vector<std::string> conductorMaterialValues();

    static std::vector<std::string> performanceInputMethodValues();

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
    /// @cond
    using ImplType = detail::ElectricLoadCenterTransformer_Impl;

    explicit ElectricLoadCenterTransformer(std::shared_ptr<detail::ElectricLoadCenterTransformer_Impl> impl);

    friend class detail::ElectricLoadCenterTransformer_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ElectricLoadCenterTransformer");
  };

  /** \relates ElectricLoadCenterTransformer*/
  using OptionalElectricLoadCenterTransformer = boost::optional<ElectricLoadCenterTransformer>;

  /** \relates ElectricLoadCenterTransformer*/
  using ElectricLoadCenterTransformerVector = std::vector<ElectricLoadCenterTransformer>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ELECTRICLOADCENTERTRANSFORMER_HPP
