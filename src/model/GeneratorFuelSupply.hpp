/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELSUPPLY_HPP
#define MODEL_GENERATORFUELSUPPLY_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class Node;
  class Schedule;
  class CurveCubic;
  class GeneratorFuelCell;

  namespace detail {

    class GeneratorFuelSupply_Impl;

  }  // namespace detail

  /** This class implements a constituent */
  class MODEL_API FuelSupplyConstituent
  {
   public:
    FuelSupplyConstituent(const std::string& constituentName, double molarFraction);

    std::string constituentName() const;
    double molarFraction() const;

    static bool isValid(const std::string& constituentName);
    static std::vector<std::string> constituentNameValues();
    static std::vector<std::string> validConstituentNameValues();

   private:
    std::string m_name;
    double m_molarFraction;
    REGISTER_LOGGER("openstudio.model.FuelSupplyConstituent");
  };

  // Overload operator<<
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::FuelSupplyConstituent& constituent);

  /** GeneratorFuelSupply is a ModelObject that wraps the OpenStudio IDD object 'OS:Generator:FuelSupply'. */
  class MODEL_API GeneratorFuelSupply : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GeneratorFuelSupply(const Model& model);

    explicit GeneratorFuelSupply(const Model& model, Schedule& tempSchedule, const CurveCubic& powerCurve);

    virtual ~GeneratorFuelSupply() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorFuelSupply(const GeneratorFuelSupply& other) = default;
    GeneratorFuelSupply(GeneratorFuelSupply&& other) = default;
    GeneratorFuelSupply& operator=(const GeneratorFuelSupply&) = default;
    GeneratorFuelSupply& operator=(GeneratorFuelSupply&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> fuelTemperatureModelingModeValues();

    static std::vector<std::string> fuelTypeValues();

    //extensible fields

    bool addConstituent(const FuelSupplyConstituent& constituent);
    // Convenience function to add a constituent without explicitly creating a FuelSupplyConstituent
    bool addConstituent(const std::string& name, double molarFraction);

    // TODO: this should return bool (to indicate whether groupIndex is valid...)
    void removeConstituent(int groupIndex);

    void removeAllConstituents();

    std::vector<FuelSupplyConstituent> constituents() const;

    /** @name Getters */
    //@{

    std::string fuelTemperatureModelingMode() const;

    boost::optional<Node> fuelTemperatureReferenceNode() const;

    boost::optional<Schedule> fuelTemperatureSchedule() const;

    CurveCubic compressorPowerMultiplierFunctionofFuelRateCurve() const;

    double compressorHeatLossFactor() const;

    std::string fuelType() const;

    boost::optional<double> liquidGenericFuelLowerHeatingValue() const;

    boost::optional<double> liquidGenericFuelHigherHeatingValue() const;

    boost::optional<double> liquidGenericFuelMolecularWeight() const;

    boost::optional<double> liquidGenericFuelCO2EmissionFactor() const;

    // TODO: this should be non optional
    boost::optional<unsigned int> numberofConstituentsinGaseousConstituentFuelSupply() const;

    // Convenience function to check that it's equal to 1.0 (If no constituents, returns 0 and warns)
    double sumofConstituentsMolarFractions() const;

    // Return optional parent generator
    boost::optional<GeneratorFuelCell> fuelCell() const;

    //@}
    /** @name Setters */
    //@{

    bool setFuelTemperatureModelingMode(const std::string& fuelTemperatureModelingMode);

    void resetFuelTemperatureModelingMode();

    bool setFuelTemperatureReferenceNode(const Node& connection);

    void resetFuelTemperatureReferenceNode();

    bool setFuelTemperatureSchedule(Schedule& schedule);

    void resetFuelTemperatureSchedule();

    bool setCompressorPowerMultiplierFunctionofFuelRateCurve(const CurveCubic& cubicCurves);

    bool setCompressorHeatLossFactor(double compressorHeatLossFactor);

    void resetCompressorHeatLossFactor();

    bool setFuelType(const std::string& fuelType);

    void resetFuelType();

    bool setLiquidGenericFuelLowerHeatingValue(double liquidGenericFuelLowerHeatingValue);

    void resetLiquidGenericFuelLowerHeatingValue();

    bool setLiquidGenericFuelHigherHeatingValue(double liquidGenericFuelHigherHeatingValue);

    void resetLiquidGenericFuelHigherHeatingValue();

    bool setLiquidGenericFuelMolecularWeight(double liquidGenericFuelMolecularWeight);

    void resetLiquidGenericFuelMolecularWeight();

    bool setLiquidGenericFuelCO2EmissionFactor(double liquidGenericFuelCO2EmissionFactor);

    void resetLiquidGenericFuelCO2EmissionFactor();

    // Automatically handled by addConstituent, removeConstituent
    // bool setNumberofConstituentsinGaseousConstituentFuelSupply(unsigned int numberofConstituentsinGaseousConstituentFuelSupply);
    // void resetNumberofConstituentsinGaseousConstituentFuelSupply();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GeneratorFuelSupply_Impl;

    explicit GeneratorFuelSupply(std::shared_ptr<detail::GeneratorFuelSupply_Impl> impl);

    friend class detail::GeneratorFuelSupply_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorFuelSupply");
  };

  /** \relates GeneratorFuelSupply*/
  using OptionalGeneratorFuelSupply = boost::optional<GeneratorFuelSupply>;

  /** \relates GeneratorFuelSupply*/
  using GeneratorFuelSupplyVector = std::vector<GeneratorFuelSupply>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELSUPPLY_HPP
