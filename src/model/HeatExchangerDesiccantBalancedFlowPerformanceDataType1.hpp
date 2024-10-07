/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATEXCHANGERDESICCANTBALANCEDFLOWPERFORMANCEDATATYPE1_HPP
#define MODEL_HEATEXCHANGERDESICCANTBALANCEDFLOWPERFORMANCEDATATYPE1_HPP

#include <model/ModelAPI.hpp>
#include "ResourceObject.hpp"

namespace openstudio {

namespace model {

  class HeatExchangerDesiccantBalancedFlow;

  namespace detail {

    class HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl;

  }  // namespace detail

  /** HeatExchangerDesiccantBalancedFlowPerformanceDataType1 is a ResourceObject that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:CurveFit:Performance'. */
  class MODEL_API HeatExchangerDesiccantBalancedFlowPerformanceDataType1 : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit HeatExchangerDesiccantBalancedFlowPerformanceDataType1(const Model& model);

    virtual ~HeatExchangerDesiccantBalancedFlowPerformanceDataType1() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HeatExchangerDesiccantBalancedFlowPerformanceDataType1(const HeatExchangerDesiccantBalancedFlowPerformanceDataType1& other) = default;
    HeatExchangerDesiccantBalancedFlowPerformanceDataType1(HeatExchangerDesiccantBalancedFlowPerformanceDataType1&& other) = default;
    HeatExchangerDesiccantBalancedFlowPerformanceDataType1& operator=(const HeatExchangerDesiccantBalancedFlowPerformanceDataType1&) = default;
    HeatExchangerDesiccantBalancedFlowPerformanceDataType1& operator=(HeatExchangerDesiccantBalancedFlowPerformanceDataType1&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    std::vector<HeatExchangerDesiccantBalancedFlow> heatExchangerDesiccantBalancedFlows() const;

    bool isNominalAirFlowRateAutosized() const;

    boost::optional<double> nominalAirFlowRate() const;

    bool isNominalAirFaceVelocityAutosized() const;

    boost::optional<double> nominalAirFaceVelocity() const;

    double nominalElectricPower() const;

    double temperatureEquationCoefficient1() const;

    double temperatureEquationCoefficient2() const;

    double temperatureEquationCoefficient3() const;

    double temperatureEquationCoefficient4() const;

    double temperatureEquationCoefficient5() const;

    double temperatureEquationCoefficient6() const;

    double temperatureEquationCoefficient7() const;

    double temperatureEquationCoefficient8() const;

    double minimumRegenerationInletAirHumidityRatioforTemperatureEquation() const;

    double maximumRegenerationInletAirHumidityRatioforTemperatureEquation() const;

    double minimumRegenerationInletAirTemperatureforTemperatureEquation() const;

    double maximumRegenerationInletAirTemperatureforTemperatureEquation() const;

    double minimumProcessInletAirHumidityRatioforTemperatureEquation() const;

    double maximumProcessInletAirHumidityRatioforTemperatureEquation() const;

    double minimumProcessInletAirTemperatureforTemperatureEquation() const;

    double maximumProcessInletAirTemperatureforTemperatureEquation() const;

    double minimumRegenerationAirVelocityforTemperatureEquation() const;

    double maximumRegenerationAirVelocityforTemperatureEquation() const;

    double minimumRegenerationOutletAirTemperatureforTemperatureEquation() const;

    double maximumRegenerationOutletAirTemperatureforTemperatureEquation() const;

    double minimumRegenerationInletAirRelativeHumidityforTemperatureEquation() const;

    double maximumRegenerationInletAirRelativeHumidityforTemperatureEquation() const;

    double minimumProcessInletAirRelativeHumidityforTemperatureEquation() const;

    double maximumProcessInletAirRelativeHumidityforTemperatureEquation() const;

    double humidityRatioEquationCoefficient1() const;

    double humidityRatioEquationCoefficient2() const;

    double humidityRatioEquationCoefficient3() const;

    double humidityRatioEquationCoefficient4() const;

    double humidityRatioEquationCoefficient5() const;

    double humidityRatioEquationCoefficient6() const;

    double humidityRatioEquationCoefficient7() const;

    double humidityRatioEquationCoefficient8() const;

    double minimumRegenerationInletAirHumidityRatioforHumidityRatioEquation() const;

    double maximumRegenerationInletAirHumidityRatioforHumidityRatioEquation() const;

    double minimumRegenerationInletAirTemperatureforHumidityRatioEquation() const;

    double maximumRegenerationInletAirTemperatureforHumidityRatioEquation() const;

    double minimumProcessInletAirHumidityRatioforHumidityRatioEquation() const;

    double maximumProcessInletAirHumidityRatioforHumidityRatioEquation() const;

    double minimumProcessInletAirTemperatureforHumidityRatioEquation() const;

    double maximumProcessInletAirTemperatureforHumidityRatioEquation() const;

    double minimumRegenerationAirVelocityforHumidityRatioEquation() const;

    double maximumRegenerationAirVelocityforHumidityRatioEquation() const;

    double minimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation() const;

    double maximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation() const;

    double minimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation() const;

    double maximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation() const;

    double minimumProcessInletAirRelativeHumidityforHumidityRatioEquation() const;

    double maximumProcessInletAirRelativeHumidityforHumidityRatioEquation() const;

    //@}
    /** @name Setters */
    //@{

    void autosizeNominalAirFlowRate();

    bool setNominalAirFlowRate(double nominalAirFlowRate);

    void autosizeNominalAirFaceVelocity();

    bool setNominalAirFaceVelocity(double nominalAirFaceVelocity);

    bool setNominalElectricPower(double nominalElectricPower);

    bool setTemperatureEquationCoefficient1(double temperatureEquationCoefficient1);

    bool setTemperatureEquationCoefficient2(double temperatureEquationCoefficient2);

    bool setTemperatureEquationCoefficient3(double temperatureEquationCoefficient3);

    bool setTemperatureEquationCoefficient4(double temperatureEquationCoefficient4);

    bool setTemperatureEquationCoefficient5(double temperatureEquationCoefficient5);

    bool setTemperatureEquationCoefficient6(double temperatureEquationCoefficient6);

    bool setTemperatureEquationCoefficient7(double temperatureEquationCoefficient7);

    bool setTemperatureEquationCoefficient8(double temperatureEquationCoefficient8);

    bool setMinimumRegenerationInletAirHumidityRatioforTemperatureEquation(double minimumRegenerationInletAirHumidityRatioforTemperatureEquation);

    bool setMaximumRegenerationInletAirHumidityRatioforTemperatureEquation(double maximumRegenerationInletAirHumidityRatioforTemperatureEquation);

    bool setMinimumRegenerationInletAirTemperatureforTemperatureEquation(double minimumRegenerationInletAirTemperatureforTemperatureEquation);

    bool setMaximumRegenerationInletAirTemperatureforTemperatureEquation(double maximumRegenerationInletAirTemperatureforTemperatureEquation);

    bool setMinimumProcessInletAirHumidityRatioforTemperatureEquation(double minimumProcessInletAirHumidityRatioforTemperatureEquation);

    bool setMaximumProcessInletAirHumidityRatioforTemperatureEquation(double maximumProcessInletAirHumidityRatioforTemperatureEquation);

    bool setMinimumProcessInletAirTemperatureforTemperatureEquation(double minimumProcessInletAirTemperatureforTemperatureEquation);

    bool setMaximumProcessInletAirTemperatureforTemperatureEquation(double maximumProcessInletAirTemperatureforTemperatureEquation);

    bool setMinimumRegenerationAirVelocityforTemperatureEquation(double minimumRegenerationAirVelocityforTemperatureEquation);

    bool setMaximumRegenerationAirVelocityforTemperatureEquation(double maximumRegenerationAirVelocityforTemperatureEquation);

    bool setMinimumRegenerationOutletAirTemperatureforTemperatureEquation(double minimumRegenerationOutletAirTemperatureforTemperatureEquation);

    bool setMaximumRegenerationOutletAirTemperatureforTemperatureEquation(double maximumRegenerationOutletAirTemperatureforTemperatureEquation);

    bool
      setMinimumRegenerationInletAirRelativeHumidityforTemperatureEquation(double minimumRegenerationInletAirRelativeHumidityforTemperatureEquation);

    bool
      setMaximumRegenerationInletAirRelativeHumidityforTemperatureEquation(double maximumRegenerationInletAirRelativeHumidityforTemperatureEquation);

    bool setMinimumProcessInletAirRelativeHumidityforTemperatureEquation(double minimumProcessInletAirRelativeHumidityforTemperatureEquation);

    bool setMaximumProcessInletAirRelativeHumidityforTemperatureEquation(double maximumProcessInletAirRelativeHumidityforTemperatureEquation);

    bool setHumidityRatioEquationCoefficient1(double humidityRatioEquationCoefficient1);

    bool setHumidityRatioEquationCoefficient2(double humidityRatioEquationCoefficient2);

    bool setHumidityRatioEquationCoefficient3(double humidityRatioEquationCoefficient3);

    bool setHumidityRatioEquationCoefficient4(double humidityRatioEquationCoefficient4);

    bool setHumidityRatioEquationCoefficient5(double humidityRatioEquationCoefficient5);

    bool setHumidityRatioEquationCoefficient6(double humidityRatioEquationCoefficient6);

    bool setHumidityRatioEquationCoefficient7(double humidityRatioEquationCoefficient7);

    bool setHumidityRatioEquationCoefficient8(double humidityRatioEquationCoefficient8);

    bool setMinimumRegenerationInletAirHumidityRatioforHumidityRatioEquation(double minimumRegenerationInletAirHumidityRatioforHumidityRatioEquation);

    bool setMaximumRegenerationInletAirHumidityRatioforHumidityRatioEquation(double maximumRegenerationInletAirHumidityRatioforHumidityRatioEquation);

    bool setMinimumRegenerationInletAirTemperatureforHumidityRatioEquation(double minimumRegenerationInletAirTemperatureforHumidityRatioEquation);

    bool setMaximumRegenerationInletAirTemperatureforHumidityRatioEquation(double maximumRegenerationInletAirTemperatureforHumidityRatioEquation);

    bool setMinimumProcessInletAirHumidityRatioforHumidityRatioEquation(double minimumProcessInletAirHumidityRatioforHumidityRatioEquation);

    bool setMaximumProcessInletAirHumidityRatioforHumidityRatioEquation(double maximumProcessInletAirHumidityRatioforHumidityRatioEquation);

    bool setMinimumProcessInletAirTemperatureforHumidityRatioEquation(double minimumProcessInletAirTemperatureforHumidityRatioEquation);

    bool setMaximumProcessInletAirTemperatureforHumidityRatioEquation(double maximumProcessInletAirTemperatureforHumidityRatioEquation);

    bool setMinimumRegenerationAirVelocityforHumidityRatioEquation(double minimumRegenerationAirVelocityforHumidityRatioEquation);

    bool setMaximumRegenerationAirVelocityforHumidityRatioEquation(double maximumRegenerationAirVelocityforHumidityRatioEquation);

    bool
      setMinimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(double minimumRegenerationOutletAirHumidityRatioforHumidityRatioEquation);

    bool
      setMaximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation(double maximumRegenerationOutletAirHumidityRatioforHumidityRatioEquation);

    bool setMinimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(
      double minimumRegenerationInletAirRelativeHumidityforHumidityRatioEquation);

    bool setMaximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation(
      double maximumRegenerationInletAirRelativeHumidityforHumidityRatioEquation);

    bool setMinimumProcessInletAirRelativeHumidityforHumidityRatioEquation(double minimumProcessInletAirRelativeHumidityforHumidityRatioEquation);

    bool setMaximumProcessInletAirRelativeHumidityforHumidityRatioEquation(double maximumProcessInletAirRelativeHumidityforHumidityRatioEquation);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedNominalAirFlowRate();

    boost::optional<double> autosizedNominalAirFaceVelocity();

    void autosize();

    void applySizingValues();

    //@}
   protected:
    /// @cond
    using ImplType = detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl;

    explicit HeatExchangerDesiccantBalancedFlowPerformanceDataType1(
      std::shared_ptr<detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl> impl);

    friend class detail::HeatExchangerDesiccantBalancedFlowPerformanceDataType1_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.HeatExchangerDesiccantBalancedFlowPerformanceDataType1");
  };

  /** \relates HeatExchangerDesiccantBalancedFlowPerformanceDataType1*/
  using OptionalHeatExchangerDesiccantBalancedFlowPerformanceDataType1 = boost::optional<HeatExchangerDesiccantBalancedFlowPerformanceDataType1>;

  /** \relates HeatExchangerDesiccantBalancedFlowPerformanceDataType1*/
  using HeatExchangerDesiccantBalancedFlowPerformanceDataType1Vector = std::vector<HeatExchangerDesiccantBalancedFlowPerformanceDataType1>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATEXCHANGERDESICCANTBALANCEDFLOWPERFORMANCEDATATYPE1_HPP
