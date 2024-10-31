/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GAS_HPP
#define MODEL_GAS_HPP

#include "ModelAPI.hpp"
#include "GasLayer.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class Gas_Impl;

  }  // namespace detail

  /** Gas is a GasLayer that wraps the OpenStudio IDD object 'OS:WindowMaterial:Gas'. */
  class MODEL_API Gas : public GasLayer
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit Gas(const Model& model, std::string gasType = "Air", double thickness = 0.003);

    virtual ~Gas() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Gas(const Gas& other) = default;
    Gas(Gas&& other) = default;
    Gas& operator=(const Gas&) = default;
    Gas& operator=(Gas&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static std::vector<std::string> validGasTypes();

    static IddObjectType iddObjectType();

    static std::vector<std::string> gasTypeValues();

    //@}

    /** @name Getters */
    //@{

    /** Gets the gas type. */
    std::string gasType() const;

    /** The conductivity (W/m*K) at temperature (K). Will throw if gasType == "Custom" and
   *  necessary values are not available. */
    double getThermalConductivity(double temperature) const;

    /** The conductance (W/m^2*K) at temperature (K). Will throw if gasType == "Custom" and
   *  necessary values are not available. */
    double getThermalConductance(double temperature) const;

    /** The resistivity (m*K/W) at temperature (K). Will throw if gasType == "Custom" and
   *  necessary values are not available. */
    double getThermalResistivity(double temperature) const;

    /** The resistance (m^2*K/W) at temperature (K). Will throw if gasType == "Custom" and
   *  necessary values are not available. */
    double getThermalResistance(double temperature) const;

    /** The viscosity (kg/m*s) at temperature (K). Will throw if gasType == "Custom" and
   *  necessary values are not available. */
    double getViscosity(double temperature) const;

    /** The specific heat (J/kg*K) at temperature (K). Will throw if gasType == "Custom" and
   *  necessary values are not available. */
    double getSpecificHeat(double temperature) const;

    /** The first conductivity coefficient of gasType "Custom" (W/m*K). The conductivity (W/m*K) =
   *  A (W/m*K) + B (W/m*K^2) * temperature (K) + C (W/m*K^3) * temperature (K^2). */
    boost::optional<double> customConductivityCoefficientA() const;

    /** The second conductivity coefficient of gasType "Custom" (W/m*K^2). The conductivity (W/m*K) =
   *  A (W/m*K) + B (W/m*K^2) * temperature (K) + C (W/m*K^3) * temperature (K^2). */
    boost::optional<double> customConductivityCoefficientB() const;

    /** The third conductivity coefficient of gasType "Custom" (W/m*K^3). The conductivity (W/m*K) =
   *  A (W/m*K) + B (W/m*K^2) * temperature (K) + C (W/m*K^3) * temperature (K^2). */
    boost::optional<double> customConductivityCoefficientC() const;

    /** The first viscosity coefficient of gasType "Custom" (kg/m*s). The viscosity (kg/m*s) =
   *  A (kg/m*s) + B (kg/m*s*K) * temperature (K) + C (kg/m*s*K^2) * temperature (K^2). */
    boost::optional<double> customViscosityCoefficientA() const;

    /** The second viscosity coefficient of gasType "Custom" (kg/m*s). The viscosity (kg/m*s) =
   *  A (kg/m*s) + B (kg/m*s*K) * temperature (K) + C (kg/m*s*K^2) * temperature (K^2). */
    boost::optional<double> customViscosityCoefficientB() const;

    /** The third viscosity coefficient of gasType "Custom" (kg/m*s). The viscosity (kg/m*s) =
   *  A (kg/m*s) + B (kg/m*s*K) * temperature (K) + C (kg/m*s*K^2) * temperature (K^2). */
    boost::optional<double> customViscosityCoefficientC() const;

    /** The first specific heat coefficient of gasType "Custom" (J/kg*K). The specific heat (J/kg*K)
   *  = A (J/kg*K) + B (J/kg*K^2) * temperature (K) + C (J/kg*K^3) * temperature (K^2). */
    boost::optional<double> customSpecificHeatCoefficientA() const;

    /** The second specific heat coefficient of gasType "Custom" (J/kg*K). The specific heat (J/kg*K)
   *  = A (J/kg*K) + B (J/kg*K^2) * temperature (K) + C (J/kg*K^3) * temperature (K^2). */
    boost::optional<double> customSpecificHeatCoefficientB() const;

    /** The third specific heat coefficient of gasType "Custom" (J/kg*K). The specific heat (J/kg*K)
   *  = A (J/kg*K) + B (J/kg*K^2) * temperature (K) + C (J/kg*K^3) * temperature (K^2). */
    boost::optional<double> customSpecificHeatCoefficientC() const;

    /** The molecular weight (u) of gasType "Custom". */
    boost::optional<double> customMolecularWeight() const;

    boost::optional<double> specificHeatRatio() const;

    double thickness() const;

    boost::optional<double> conductivityCoefficientA() const;

    boost::optional<double> conductivityCoefficientB() const;

    boost::optional<double> conductivityCoefficientC() const;

    boost::optional<double> viscosityCoefficientA() const;

    boost::optional<double> viscosityCoefficientB() const;

    boost::optional<double> viscosityCoefficientC() const;

    boost::optional<double> specificHeatCoefficientA() const;

    boost::optional<double> specificHeatCoefficientB() const;

    boost::optional<double> specificHeatCoefficientC() const;

    boost::optional<double> molecularWeight() const;

    //@}
    /** @name Setters */
    //@{

    /** If gasType() == "Custom", thermalConductivity (W/m*K) = A + B*temperature + C*temperature^2. */
    bool setCustomConductivity(double A, double B, double C);

    /** If gasType() == "Custom", viscosity (g/m*s) = A + B*temperature + C*temperature^2. */
    bool setCustomViscosity(double A, double B, double C);

    /** If gasType() == "Custom", specific heat (J/kg*K) = A + B*temperature + C*temperature^2. */
    bool setCustomSpecificHeat(double A, double B, double C);

    /** If gasType() == "Custom", specifies the molecular weight of the gas. */
    bool setCustomMolecularWeight(double value);

    bool setGasType(const std::string& gasType);

    bool setThickness(double thickness);

    bool setConductivityCoefficientA(double conductivityCoefficientA);

    void resetConductivityCoefficientA();

    bool setConductivityCoefficientB(double conductivityCoefficientB);

    void resetConductivityCoefficientB();

    bool setConductivityCoefficientC(double conductivityCoefficientC);

    void resetConductivityCoefficientC();

    bool setViscosityCoefficientA(double viscosityCoefficientA);

    void resetViscosityCoefficientA();

    bool setViscosityCoefficientB(double viscosityCoefficientB);

    void resetViscosityCoefficientB();

    bool setViscosityCoefficientC(double viscosityCoefficientC);

    void resetViscosityCoefficientC();

    bool setSpecificHeatCoefficientA(double specificHeatCoefficientA);

    void resetSpecificHeatCoefficientA();

    bool setSpecificHeatCoefficientB(double specificHeatCoefficientB);

    void resetSpecificHeatCoefficientB();

    bool setSpecificHeatCoefficientC(double specificHeatCoefficientC);

    void resetSpecificHeatCoefficientC();

    bool setMolecularWeight(double molecularWeight);

    void resetMolecularWeight();

    bool setSpecificHeatRatio(double specificHeatRatio);

    void resetSpecificHeatRatio();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::Gas_Impl;

    explicit Gas(std::shared_ptr<detail::Gas_Impl> impl);

    friend class detail::Gas_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Gas");
  };

  /** \relates Gas*/
  using OptionalGas = boost::optional<Gas>;

  /** \relates Gas*/
  using GasVector = std::vector<Gas>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GAS_HPP
