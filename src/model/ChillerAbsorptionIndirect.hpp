/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CHILLERABSORPTIONINDIRECT_HPP
#define MODEL_CHILLERABSORPTIONINDIRECT_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class ChillerAbsorptionIndirect_Impl;

  }  // namespace detail

  /** ChillerAbsorptionIndirect is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:Chiller:Absorption:Indirect'. */
  class MODEL_API ChillerAbsorptionIndirect : public WaterToWaterComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ChillerAbsorptionIndirect(const Model& model);

    virtual ~ChillerAbsorptionIndirect() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ChillerAbsorptionIndirect(const ChillerAbsorptionIndirect& other) = default;
    ChillerAbsorptionIndirect(ChillerAbsorptionIndirect&& other) = default;
    ChillerAbsorptionIndirect& operator=(const ChillerAbsorptionIndirect&) = default;
    ChillerAbsorptionIndirect& operator=(ChillerAbsorptionIndirect&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> chillerFlowModeValues();

    static std::vector<std::string> generatorHeatSourceTypeValues();

    /** @name Getters */
    //@{

    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityAutosized() const;

    boost::optional<double> nominalPumpingPower() const;

    bool isNominalPumpingPowerAutosized() const;

    double minimumPartLoadRatio() const;

    double maximumPartLoadRatio() const;

    double optimumPartLoadRatio() const;

    double designCondenserInletTemperature() const;

    double condenserInletTemperatureLowerLimit() const;

    double chilledWaterOutletTemperatureLowerLimit() const;

    boost::optional<double> designChilledWaterFlowRate() const;

    bool isDesignChilledWaterFlowRateAutosized() const;

    boost::optional<double> designCondenserWaterFlowRate() const;

    bool isDesignCondenserWaterFlowRateAutosized() const;

    std::string chillerFlowMode() const;

    Curve generatorHeatInputFunctionofPartLoadRatioCurve() const;

    Curve pumpElectricInputFunctionofPartLoadRatioCurve() const;

    Curve capacityCorrectionFunctionofCondenserTemperatureCurve() const;

    Curve capacityCorrectionFunctionofChilledWaterTemperatureCurve() const;

    Curve capacityCorrectionFunctionofGeneratorTemperatureCurve() const;

    Curve generatorHeatInputCorrectionFunctionofCondenserTemperatureCurve() const;

    Curve generatorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve() const;

    std::string generatorHeatSourceType() const;

    boost::optional<double> designGeneratorFluidFlowRate() const;

    bool isDesignGeneratorFluidFlowRateAutosized() const;

    double temperatureLowerLimitGeneratorInlet() const;

    double degreeofSubcoolinginSteamGenerator() const;

    double degreeofSubcoolinginSteamCondensateLoop() const;

    double sizingFactor() const;

    //@}
    /** @name Setters */
    //@{

    bool setNominalCapacity(double nominalCapacity);

    void autosizeNominalCapacity();

    bool setNominalPumpingPower(double nominalPumpingPower);

    void autosizeNominalPumpingPower();

    bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

    bool setMaximumPartLoadRatio(double maximumPartLoadRatio);

    bool setOptimumPartLoadRatio(double optimumPartLoadRatio);

    bool setDesignCondenserInletTemperature(double designCondenserInletTemperature);

    bool setCondenserInletTemperatureLowerLimit(double condenserInletTemperatureLowerLimit);

    bool setChilledWaterOutletTemperatureLowerLimit(double chilledWaterOutletTemperatureLowerLimit);

    bool setDesignChilledWaterFlowRate(double designChilledWaterFlowRate);

    void autosizeDesignChilledWaterFlowRate();

    bool setDesignCondenserWaterFlowRate(double designCondenserWaterFlowRate);

    void autosizeDesignCondenserWaterFlowRate();

    bool setChillerFlowMode(const std::string& chillerFlowMode);

    bool setGeneratorHeatInputFunctionofPartLoadRatioCurve(const Curve& curve);

    bool setPumpElectricInputFunctionofPartLoadRatioCurve(const Curve& curve);

    bool setCapacityCorrectionFunctionofCondenserTemperatureCurve(const Curve& curve);

    bool setCapacityCorrectionFunctionofChilledWaterTemperatureCurve(const Curve& curve);

    bool setCapacityCorrectionFunctionofGeneratorTemperatureCurve(const Curve& curve);

    bool setGeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurve(const Curve& curve);

    bool setGeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve(const Curve& curve);

    // Will not accept "Steam" if chiller is connected to a Generator Loop as we do not support Steam Loops (yet)
    bool setGeneratorHeatSourceType(const std::string& generatorHeatSourceType);

    bool setDesignGeneratorFluidFlowRate(double designGeneratorFluidFlowRate);

    void autosizeDesignGeneratorFluidFlowRate();

    bool setTemperatureLowerLimitGeneratorInlet(double temperatureLowerLimitGeneratorInlet);

    bool setDegreeofSubcoolinginSteamGenerator(double degreeofSubcoolinginSteamGenerator);

    bool setDegreeofSubcoolinginSteamCondensateLoop(double degreeofSubcoolinginSteamCondensateLoop);

    bool setSizingFactor(double sizingFactor);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedNominalCapacity() const;

    boost::optional<double> autosizedNominalPumpingPower() const;

    boost::optional<double> autosizedDesignChilledWaterFlowRate() const;

    boost::optional<double> autosizedDesignCondenserWaterFlowRate() const;

    boost::optional<double> autosizedDesignGeneratorFluidFlowRate() const;

    /** Convenience Function to return the Chilled Water Loop (chiller on supply) **/
    boost::optional<PlantLoop> chilledWaterLoop() const;

    /** Convenience Function to return the Condenser Water Loop (chiller on demand side) **/
    boost::optional<PlantLoop> condenserWaterLoop() const;

    /** Convenience Function to return the Generator Loop (chiller on the demand side - tertiary) **/
    boost::optional<PlantLoop> generatorLoop() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ChillerAbsorptionIndirect_Impl;

    explicit ChillerAbsorptionIndirect(std::shared_ptr<detail::ChillerAbsorptionIndirect_Impl> impl);

    friend class detail::ChillerAbsorptionIndirect_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ChillerAbsorptionIndirect");
  };

  /** \relates ChillerAbsorptionIndirect*/
  using OptionalChillerAbsorptionIndirect = boost::optional<ChillerAbsorptionIndirect>;

  /** \relates ChillerAbsorptionIndirect*/
  using ChillerAbsorptionIndirectVector = std::vector<ChillerAbsorptionIndirect>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CHILLERABSORPTIONINDIRECT_HPP
