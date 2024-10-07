/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLEXHAUSTGASTOWATERHEATEXCHANGER_HPP
#define MODEL_GENERATORFUELCELLEXHAUSTGASTOWATERHEATEXCHANGER_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Connection;
  class Node;
  class GeneratorFuelCell;

  namespace detail {

    class GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl;

  }  // namespace detail

  /** GeneratorFuelCellExhaustGasToWaterHeatExchanger is a ModelObject that wraps the OpenStudio IDD object 'OS:Generator:FuelCell:ExhaustGasToWaterHeatExchanger'. */
  class MODEL_API GeneratorFuelCellExhaustGasToWaterHeatExchanger : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GeneratorFuelCellExhaustGasToWaterHeatExchanger(const Model& model);

    explicit GeneratorFuelCellExhaustGasToWaterHeatExchanger(const Model& model, const Node& exhaustOutletAirNode);

    virtual ~GeneratorFuelCellExhaustGasToWaterHeatExchanger() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GeneratorFuelCellExhaustGasToWaterHeatExchanger(const GeneratorFuelCellExhaustGasToWaterHeatExchanger& other) = default;
    GeneratorFuelCellExhaustGasToWaterHeatExchanger(GeneratorFuelCellExhaustGasToWaterHeatExchanger&& other) = default;
    GeneratorFuelCellExhaustGasToWaterHeatExchanger& operator=(const GeneratorFuelCellExhaustGasToWaterHeatExchanger&) = default;
    GeneratorFuelCellExhaustGasToWaterHeatExchanger& operator=(GeneratorFuelCellExhaustGasToWaterHeatExchanger&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> heatExchangerCalculationMethodValues();

    /** @name Getters */
    //@{

    double heatRecoveryWaterMaximumFlowRate() const;

    boost::optional<Node> exhaustOutletAirNode() const;

    std::string heatExchangerCalculationMethod() const;

    boost::optional<double> method1HeatExchangerEffectiveness() const;

    boost::optional<double> method2Parameterhxs0() const;

    boost::optional<double> method2Parameterhxs1() const;

    boost::optional<double> method2Parameterhxs2() const;

    boost::optional<double> method2Parameterhxs3() const;

    boost::optional<double> method2Parameterhxs4() const;

    boost::optional<double> method3h0GasCoefficient() const;

    boost::optional<double> method3NdotGasRefCoefficient() const;

    boost::optional<double> method3nCoefficient() const;

    boost::optional<double> method3GasArea() const;

    boost::optional<double> method3h0WaterCoefficient() const;

    boost::optional<double> method3NdotWaterrefCoefficient() const;

    boost::optional<double> method3mCoefficient() const;

    boost::optional<double> method3WaterArea() const;

    boost::optional<double> method3FAdjustmentFactor() const;

    boost::optional<double> method4hxl1Coefficient() const;

    boost::optional<double> method4hxl2Coefficient() const;

    boost::optional<double> method4CondensationThreshold() const;

    // Return optional parent generator
    boost::optional<GeneratorFuelCell> fuelCell() const;

    //@}
    /** @name Setters */
    //@{

    bool setHeatRecoveryWaterMaximumFlowRate(double heatRecoveryWaterMaximumFlowRate);

    void resetHeatRecoveryWaterMaximumFlowRate();

    bool setExhaustOutletAirNode(const Node& node);

    void resetExhaustOutletAirNode();

    bool setHeatExchangerCalculationMethod(const std::string& heatExchangerCalculationMethod);

    void resetHeatExchangerCalculationMethod();

    bool setMethod1HeatExchangerEffectiveness(double method1HeatExchangerEffectiveness);

    void resetMethod1HeatExchangerEffectiveness();

    bool setMethod2Parameterhxs0(double method2Parameterhxs0);

    void resetMethod2Parameterhxs0();

    bool setMethod2Parameterhxs1(double method2Parameterhxs1);

    void resetMethod2Parameterhxs1();

    bool setMethod2Parameterhxs2(double method2Parameterhxs2);

    void resetMethod2Parameterhxs2();

    bool setMethod2Parameterhxs3(double method2Parameterhxs3);

    void resetMethod2Parameterhxs3();

    bool setMethod2Parameterhxs4(double method2Parameterhxs4);

    void resetMethod2Parameterhxs4();

    bool setMethod3h0GasCoefficient(double method3h0GasCoefficient);

    void resetMethod3h0GasCoefficient();

    bool setMethod3NdotGasRefCoefficient(double method3NdotGasRefCoefficient);

    void resetMethod3NdotGasRefCoefficient();

    bool setMethod3nCoefficient(double method3nCoefficient);

    void resetMethod3nCoefficient();

    bool setMethod3GasArea(double method3GasArea);

    void resetMethod3GasArea();

    bool setMethod3h0WaterCoefficient(double method3h0WaterCoefficient);

    void resetMethod3h0WaterCoefficient();

    bool setMethod3NdotWaterrefCoefficient(double method3NdotWaterrefCoefficient);

    void resetMethod3NdotWaterrefCoefficient();

    bool setMethod3mCoefficient(double method3mCoefficient);

    void resetMethod3mCoefficient();

    bool setMethod3WaterArea(double method3WaterArea);

    void resetMethod3WaterArea();

    bool setMethod3FAdjustmentFactor(double method3FAdjustmentFactor);

    void resetMethod3FAdjustmentFactor();

    bool setMethod4hxl1Coefficient(double method4hxl1Coefficient);

    void resetMethod4hxl1Coefficient();

    bool setMethod4hxl2Coefficient(double method4hxl2Coefficient);

    void resetMethod4hxl2Coefficient();

    bool setMethod4CondensationThreshold(double method4CondensationThreshold);

    void resetMethod4CondensationThreshold();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl;

    explicit GeneratorFuelCellExhaustGasToWaterHeatExchanger(std::shared_ptr<detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl> impl);

    friend class detail::GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GeneratorFuelCellExhaustGasToWaterHeatExchanger");
  };

  /** \relates GeneratorFuelCellExhaustGasToWaterHeatExchanger*/
  using OptionalGeneratorFuelCellExhaustGasToWaterHeatExchanger = boost::optional<GeneratorFuelCellExhaustGasToWaterHeatExchanger>;

  /** \relates GeneratorFuelCellExhaustGasToWaterHeatExchanger*/
  using GeneratorFuelCellExhaustGasToWaterHeatExchangerVector = std::vector<GeneratorFuelCellExhaustGasToWaterHeatExchanger>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLEXHAUSTGASTOWATERHEATEXCHANGER_HPP
