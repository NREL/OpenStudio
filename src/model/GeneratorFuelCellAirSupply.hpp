/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLAIRSUPPLY_HPP
#define MODEL_GENERATORFUELCELLAIRSUPPLY_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class Node;
  class CurveCubic;
  class CurveQuadratic;
  class GeneratorFuelCell;

  /** This class implements a constituent */
  class MODEL_API AirSupplyConstituent
  {
   public:
    AirSupplyConstituent(const std::string& constituentName, double molarFraction);

    std::string constituentName() const;
    double molarFraction() const;

    static bool isValid(const std::string& constituentName);
    static std::vector<std::string> constituentNameValues();
    static std::vector<std::string> validConstituentNameValues();

   private:
    std::string m_name;
    double m_molarFraction;
    REGISTER_LOGGER("openstudio.model.AirSupplyConstituent");
  };

  // Overload operator<<
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::AirSupplyConstituent& constituent);

  namespace detail {

    class GeneratorFuelCellAirSupply_Impl;

  }  // namespace detail

  /** GeneratorFuelCellAirSupply is a ModelObject that wraps the OpenStudio IDD object 'OS:Generator:FuelCell:AirSupply'. */
  class MODEL_API GeneratorFuelCellAirSupply : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GeneratorFuelCellAirSupply(const Model& model);

    explicit GeneratorFuelCellAirSupply(const Model& model, const Node& airInletNode);

    explicit GeneratorFuelCellAirSupply(const Model& model, const Node& airInletNode, const CurveQuadratic& electricPowerCurve,
                                        const CurveQuadratic& fuelRateCurve);

    explicit GeneratorFuelCellAirSupply(const Model& model, const Node& airInletNode, const CurveQuadratic& electricPowerCurve,
                                        const CurveQuadratic& fuelRateCurve, const CurveCubic& blowerPowerCurve);

    virtual ~GeneratorFuelCellAirSupply() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorFuelCellAirSupply(const GeneratorFuelCellAirSupply& other) = default;
    GeneratorFuelCellAirSupply(GeneratorFuelCellAirSupply&& other) = default;
    GeneratorFuelCellAirSupply& operator=(const GeneratorFuelCellAirSupply&) = default;
    GeneratorFuelCellAirSupply& operator=(GeneratorFuelCellAirSupply&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> airSupplyRateCalculationModeValues();

    static std::vector<std::string> airIntakeHeatRecoveryModeValues();

    static std::vector<std::string> airSupplyConstituentModeValues();

    //extensible fields
    bool addConstituent(const AirSupplyConstituent& constituent);
    // Convenience function to add a constituent without explicitly creating a FuelSupplyConstituent
    bool addConstituent(const std::string& name, double molarFraction);

    // TODO: this should return bool (to indicate whether groupIndex is valid...)
    void removeConstituent(int groupIndex);

    void removeAllConstituents();

    std::vector<AirSupplyConstituent> constituents() const;

    /** @name Getters */
    //@{

    boost::optional<Node> airInletNode() const;

    boost::optional<CurveCubic> blowerPowerCurve() const;

    double blowerHeatLossFactor() const;

    std::string airSupplyRateCalculationMode() const;

    boost::optional<double> stoichiometricRatio() const;

    boost::optional<CurveQuadratic> airRateFunctionofElectricPowerCurve() const;

    boost::optional<double> airRateAirTemperatureCoefficient() const;

    boost::optional<CurveQuadratic> airRateFunctionofFuelRateCurve() const;

    std::string airIntakeHeatRecoveryMode() const;

    std::string airSupplyConstituentMode() const;

    // TODO: this should be a non optional
    boost::optional<unsigned int> numberofUserDefinedConstituents() const;

    // Convenience function to check that it's equal to 1.0 (If no constituents, returns 0 and warns)
    double sumofConstituentsMolarFractions() const;

    // Return optional parent generator
    boost::optional<GeneratorFuelCell> fuelCell() const;

    //@}
    /** @name Setters */
    //@{

    bool setAirInletNode(const Node& connection);

    void resetAirInletNode();

    bool setBlowerPowerCurve(const CurveCubic& cubicCurves);

    void resetBlowerPowerCurve();

    bool setBlowerHeatLossFactor(double blowerHeatLossFactor);

    void resetBlowerHeatLossFactor();

    bool setAirSupplyRateCalculationMode(const std::string& airSupplyRateCalculationMode);

    bool setStoichiometricRatio(double stoichiometricRatio);

    void resetStoichiometricRatio();

    bool setAirRateFunctionofElectricPowerCurve(const CurveQuadratic& quadraticCurves);

    void resetAirRateFunctionofElectricPowerCurve();

    bool setAirRateAirTemperatureCoefficient(double airRateAirTemperatureCoefficient);

    void resetAirRateAirTemperatureCoefficient();

    bool setAirRateFunctionofFuelRateCurve(const CurveQuadratic& quadraticCurves);

    void resetAirRateFunctionofFuelRateCurve();

    bool setAirIntakeHeatRecoveryMode(const std::string& airIntakeHeatRecoveryMode);

    bool setAirSupplyConstituentMode(const std::string& airSupplyConstituentMode);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GeneratorFuelCellAirSupply_Impl;

    explicit GeneratorFuelCellAirSupply(std::shared_ptr<detail::GeneratorFuelCellAirSupply_Impl> impl);

    friend class detail::GeneratorFuelCellAirSupply_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorFuelCellAirSupply");
  };

  /** \relates GeneratorFuelCellAirSupply*/
  using OptionalGeneratorFuelCellAirSupply = boost::optional<GeneratorFuelCellAirSupply>;

  /** \relates GeneratorFuelCellAirSupply*/
  using GeneratorFuelCellAirSupplyVector = std::vector<GeneratorFuelCellAirSupply>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLAIRSUPPLY_HPP
