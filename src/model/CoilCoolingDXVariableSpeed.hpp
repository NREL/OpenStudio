/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXVARIABLESPEED_HPP
#define MODEL_COILCOOLINGDXVARIABLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Curve;
  // class WaterStorageTank;
  class Schedule;
  class CoilCoolingDXVariableSpeedSpeedData;

  namespace detail {

    class CoilCoolingDXVariableSpeed_Impl;

  }  // namespace detail

  /** CoilCoolingDXVariableSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:VariableSpeed'. */
  class MODEL_API CoilCoolingDXVariableSpeed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingDXVariableSpeed(const Model& model);

    explicit CoilCoolingDXVariableSpeed(const Model& model, const Curve& partLoadFraction);

    virtual ~CoilCoolingDXVariableSpeed() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDXVariableSpeed(const CoilCoolingDXVariableSpeed& other) = default;
    CoilCoolingDXVariableSpeed(CoilCoolingDXVariableSpeed&& other) = default;
    CoilCoolingDXVariableSpeed& operator=(const CoilCoolingDXVariableSpeed&) = default;
    CoilCoolingDXVariableSpeed& operator=(CoilCoolingDXVariableSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> condenserTypeValues();

    /** @name Getters */
    //@{

    int nominalSpeedLevel() const;

    boost::optional<double> grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const;

    bool isGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevelAutosized() const;

    boost::optional<double> ratedAirFlowRateAtSelectedNominalSpeedLevel() const;

    bool isRatedAirFlowRateAtSelectedNominalSpeedLevelAutosized() const;

    double nominalTimeforCondensatetoBeginLeavingtheCoil() const;

    double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity() const;

    double maximumCyclingRate() const;

    double latentCapacityTimeConstant() const;

    double fanDelayTime() const;

    Curve energyPartLoadFractionCurve() const;

    // boost::optional<std::string> condenserAirInletNodeName() const;

    std::string condenserType() const;

    boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption() const;

    bool isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const;

    double crankcaseHeaterCapacity() const;

    boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

    double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

    // boost::optional<WaterStorageTank> supplyWaterStorageTank() const;

    // boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank() const;

    double basinHeaterCapacity() const;

    double basinHeaterSetpointTemperature() const;

    boost::optional<Schedule> basinHeaterOperatingSchedule() const;

    double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

    //@}
    /** @name Setters */
    //@{

    bool setNominalSpeedLevel(int nominalSpeedLevel);

    bool setGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel(double grossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel);

    void autosizeGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel();

    bool setRatedAirFlowRateAtSelectedNominalSpeedLevel(double ratedAirFlowRateAtSelectedNominalSpeedLevel);

    void autosizeRatedAirFlowRateAtSelectedNominalSpeedLevel();

    bool setNominalTimeforCondensatetoBeginLeavingtheCoil(double nominalTimeforCondensatetoBeginLeavingtheCoil);

    bool setInitialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity(
      double initialMoistureEvaporationRateDividedbySteadyStateACLatentCapacity);

    bool setMaximumCyclingRate(double maximumCyclingRate);

    bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);

    bool setFanDelayTime(double fanDelayTime);

    bool setEnergyPartLoadFractionCurve(const Curve& curve);

    // bool setCondenserAirInletNodeName(const std::string& condenserAirInletNodeName);

    // void resetCondenserAirInletNodeName();

    bool setCondenserType(const std::string& condenserType);

    bool setEvaporativeCondenserPumpRatedPowerConsumption(double evaporativeCondenserPumpRatedPowerConsumption);

    void autosizeEvaporativeCondenserPumpRatedPowerConsumption();

    bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

    bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);

    void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

    bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

    // bool setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank);

    // void resetSupplyWaterStorageTank();

    // bool setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank);

    // void resetCondensateCollectionWaterStorageTank();

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    bool setBasinHeaterOperatingSchedule(Schedule& schedule);

    void resetBasinHeaterOperatingSchedule();

    bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

    //@}
    /** @name Other */
    //@{

    std::vector<CoilCoolingDXVariableSpeedSpeedData> speeds() const;

    bool addSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed);

    void removeSpeed(const CoilCoolingDXVariableSpeedSpeedData& speed);

    void removeAllSpeeds();

    boost::optional<double> autosizedGrossRatedTotalCoolingCapacityAtSelectedNominalSpeedLevel() const;

    boost::optional<double> autosizedRatedAirFlowRateAtSelectedNominalSpeedLevel() const;

    boost::optional<double> autosizedEvaporativeCondenserPumpRatedPowerConsumption() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingDXVariableSpeed_Impl;

    explicit CoilCoolingDXVariableSpeed(std::shared_ptr<detail::CoilCoolingDXVariableSpeed_Impl> impl);

    friend class detail::CoilCoolingDXVariableSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXVariableSpeed");
  };

  /** \relates CoilCoolingDXVariableSpeed*/
  using OptionalCoilCoolingDXVariableSpeed = boost::optional<CoilCoolingDXVariableSpeed>;

  /** \relates CoilCoolingDXVariableSpeed*/
  using CoilCoolingDXVariableSpeedVector = std::vector<CoilCoolingDXVariableSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXVARIABLESPEED_HPP
