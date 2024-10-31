/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CHILLERABSORPTION_HPP
#define MODEL_CHILLERABSORPTION_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ChillerAbsorption_Impl;

  }  // namespace detail

  /** ChillerAbsorption is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:Chiller:Absorption'. */
  class MODEL_API ChillerAbsorption : public WaterToWaterComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ChillerAbsorption(const Model& model);

    virtual ~ChillerAbsorption() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ChillerAbsorption(const ChillerAbsorption& other) = default;
    ChillerAbsorption(ChillerAbsorption&& other) = default;
    ChillerAbsorption& operator=(const ChillerAbsorption&) = default;
    ChillerAbsorption& operator=(ChillerAbsorption&&) = default;

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

    boost::optional<double> designChilledWaterFlowRate() const;

    bool isDesignChilledWaterFlowRateAutosized() const;

    boost::optional<double> designCondenserWaterFlowRate() const;

    bool isDesignCondenserWaterFlowRateAutosized() const;

    double coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve() const;

    double coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve() const;

    double coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve() const;

    double coefficient1ofthePumpElectricUsePartLoadRatioCurve() const;

    double coefficient2ofthePumpElectricUsePartLoadRatioCurve() const;

    double coefficient3ofthePumpElectricUsePartLoadRatioCurve() const;

    double chilledWaterOutletTemperatureLowerLimit() const;

    std::string chillerFlowMode() const;

    std::string generatorHeatSourceType() const;

    boost::optional<double> designGeneratorFluidFlowRate() const;

    bool isDesignGeneratorFluidFlowRateAutosized() const;

    double degreeofSubcoolinginSteamGenerator() const;

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

    bool setDesignChilledWaterFlowRate(double designChilledWaterFlowRate);

    void autosizeDesignChilledWaterFlowRate();

    bool setDesignCondenserWaterFlowRate(double designCondenserWaterFlowRate);

    void autosizeDesignCondenserWaterFlowRate();

    bool setCoefficient1oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve);

    bool setCoefficient2oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve);

    bool setCoefficient3oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve);

    bool setCoefficient1ofthePumpElectricUsePartLoadRatioCurve(double coefficient1ofthePumpElectricUsePartLoadRatioCurve);

    bool setCoefficient2ofthePumpElectricUsePartLoadRatioCurve(double coefficient2ofthePumpElectricUsePartLoadRatioCurve);

    bool setCoefficient3ofthePumpElectricUsePartLoadRatioCurve(double coefficient3ofthePumpElectricUsePartLoadRatioCurve);

    bool setChilledWaterOutletTemperatureLowerLimit(double chilledWaterOutletTemperatureLowerLimit);

    bool setChillerFlowMode(const std::string& chillerFlowMode);

    // Will not accept "Steam" if chiller is connected to a Generator Loop as we do not support Steam Loops (yet)
    bool setGeneratorHeatSourceType(const std::string& generatorHeatSourceType);

    bool setDesignGeneratorFluidFlowRate(double designGeneratorFluidFlowRate);

    void autosizeDesignGeneratorFluidFlowRate();

    bool setDegreeofSubcoolinginSteamGenerator(double degreeofSubcoolinginSteamGenerator);

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

    /** Convenience Function to return the Generator Loop (chiller on the demand side  - tertiary) **/
    boost::optional<PlantLoop> generatorLoop() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ChillerAbsorption_Impl;

    explicit ChillerAbsorption(std::shared_ptr<detail::ChillerAbsorption_Impl> impl);

    friend class detail::ChillerAbsorption_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ChillerAbsorption");
  };

  /** \relates ChillerAbsorption*/
  using OptionalChillerAbsorption = boost::optional<ChillerAbsorption>;

  /** \relates ChillerAbsorption*/
  using ChillerAbsorptionVector = std::vector<ChillerAbsorption>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CHILLERABSORPTION_HPP
