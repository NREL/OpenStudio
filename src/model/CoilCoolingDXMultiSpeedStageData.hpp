/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXMULTISPEEDSTAGEDATA_HPP
#define MODEL_COILCOOLINGDXMULTISPEEDSTAGEDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../model/CoilCoolingDXMultiSpeed.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class CoilCoolingDXMultiSpeedStageData_Impl;

  }  // namespace detail

  /** CoilCoolingDXMultiSpeedStageData is a ParentObject that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:MultiSpeed:StageData'. */
  class MODEL_API CoilCoolingDXMultiSpeedStageData : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilCoolingDXMultiSpeedStageData(const Model& model, Curve& coolingCapacityFunctionofTemperature,
                                              Curve& coolingCapacityFuncionofFlowFraction, Curve& energyInputRatioFunctionofTemperature,
                                              Curve& energyInputRatioFunctionofFlowFraction, Curve& partLoadFractionCorrelation,
                                              Curve& wasteHeatFunctionofTemperature);

    /** Create CoilCoolingDXMultiSpeedStageData with default curves **/
    explicit CoilCoolingDXMultiSpeedStageData(const Model& model);

    virtual ~CoilCoolingDXMultiSpeedStageData() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilCoolingDXMultiSpeedStageData(const CoilCoolingDXMultiSpeedStageData& other) = default;
    CoilCoolingDXMultiSpeedStageData(CoilCoolingDXMultiSpeedStageData&& other) = default;
    CoilCoolingDXMultiSpeedStageData& operator=(const CoilCoolingDXMultiSpeedStageData&) = default;
    CoilCoolingDXMultiSpeedStageData& operator=(CoilCoolingDXMultiSpeedStageData&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<double> grossRatedTotalCoolingCapacity() const;

    bool isGrossRatedTotalCoolingCapacityAutosized() const;

    boost::optional<double> grossRatedSensibleHeatRatio() const;

    bool isGrossRatedSensibleHeatRatioAutosized() const;

    double grossRatedCoolingCOP() const;

    boost::optional<double> ratedAirFlowRate() const;

    bool isRatedAirFlowRateAutosized() const;

    OS_DEPRECATED(3, 5, 0) double ratedEvaporatorFanPowerPerVolumeFlowRate() const;

    double ratedEvaporatorFanPowerPerVolumeFlowRate2017() const;

    double ratedEvaporatorFanPowerPerVolumeFlowRate2023() const;

    Curve totalCoolingCapacityFunctionofTemperatureCurve() const;

    Curve totalCoolingCapacityFunctionofFlowFractionCurve() const;

    Curve energyInputRatioFunctionofTemperatureCurve() const;

    Curve energyInputRatioFunctionofFlowFractionCurve() const;

    Curve partLoadFractionCorrelationCurve() const;

    double nominalTimeforCondensateRemovaltoBegin() const;

    double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const;

    double maximumCyclingRate() const;

    double latentCapacityTimeConstant() const;

    double ratedWasteHeatFractionofPowerInput() const;

    Curve wasteHeatFunctionofTemperatureCurve() const;

    double evaporativeCondenserEffectiveness() const;

    boost::optional<double> evaporativeCondenserAirFlowRate() const;

    bool isEvaporativeCondenserAirFlowRateAutosized() const;

    boost::optional<double> ratedEvaporativeCondenserPumpPowerConsumption() const;

    bool isRatedEvaporativeCondenserPumpPowerConsumptionAutosized() const;

    //@}
    /** @name Setters */
    //@{

    bool setGrossRatedTotalCoolingCapacity(double grossRatedTotalCoolingCapacity);

    void autosizeGrossRatedTotalCoolingCapacity();

    bool setGrossRatedSensibleHeatRatio(double grossRatedSensibleHeatRatio);

    void autosizeGrossRatedSensibleHeatRatio();

    bool setGrossRatedCoolingCOP(double grossRatedCoolingCOP);

    bool setRatedAirFlowRate(double ratedAirFlowRate);

    void autosizeRatedAirFlowRate();

    OS_DEPRECATED(3, 5, 0) bool setRatedEvaporatorFanPowerPerVolumeFlowRate(double ratedEvaporatorFanPowerPerVolumeFlowRate);

    bool setRatedEvaporatorFanPowerPerVolumeFlowRate2017(double ratedEvaporatorFanPowerPerVolumeFlowRate2017);

    bool setRatedEvaporatorFanPowerPerVolumeFlowRate2023(double ratedEvaporatorFanPowerPerVolumeFlowRate2023);

    bool setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve);

    bool setTotalCoolingCapacityFunctionofFlowFractionCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

    bool setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve);

    bool setPartLoadFractionCorrelationCurve(const Curve& curve);

    bool setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin);

    bool
      setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

    bool setMaximumCyclingRate(double maximumCyclingRate);

    bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);

    bool setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput);

    bool setWasteHeatFunctionofTemperatureCurve(const Curve& curve);

    bool setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness);

    bool setEvaporativeCondenserAirFlowRate(double evaporativeCondenserAirFlowRate);

    void autosizeEvaporativeCondenserAirFlowRate();

    bool setRatedEvaporativeCondenserPumpPowerConsumption(double ratedEvaporativeCondenserPumpPowerConsumption);

    void autosizeRatedEvaporativeCondenserPumpPowerConsumption();

    //@}
    /** @name Other */
    //@{

    // Returns the CoilCoolingDXMultiSpeed that references it if any
    boost::optional<CoilCoolingDXMultiSpeed> parentCoil() const;

    boost::optional<double> autosizedGrossRatedTotalCoolingCapacity() const;

    boost::optional<double> autosizedGrossRatedSensibleHeatRatio() const;

    boost::optional<double> autosizedRatedAirFlowRate() const;

    boost::optional<double> autosizedEvaporativeCondenserAirFlowRate() const;

    boost::optional<double> autosizedRatedEvaporativeCondenserPumpPowerConsumption() const;

    void autosize();

    void applySizingValues();

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilCoolingDXMultiSpeedStageData_Impl;

    explicit CoilCoolingDXMultiSpeedStageData(std::shared_ptr<detail::CoilCoolingDXMultiSpeedStageData_Impl> impl);

    friend class detail::CoilCoolingDXMultiSpeedStageData_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilCoolingDXMultiSpeedStageData");

    // Used to determine the index of this performance data in the
    // list of stages in the parent object.
    boost::optional<std::tuple<int, CoilCoolingDXMultiSpeed>> stageIndexAndParentCoil() const;
  };

  /** \relates CoilCoolingDXMultiSpeedStageData*/
  using OptionalCoilCoolingDXMultiSpeedStageData = boost::optional<CoilCoolingDXMultiSpeedStageData>;

  /** \relates CoilCoolingDXMultiSpeedStageData*/
  using CoilCoolingDXMultiSpeedStageDataVector = std::vector<CoilCoolingDXMultiSpeedStageData>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXMULTISPEEDSTAGEDATA_HPP
