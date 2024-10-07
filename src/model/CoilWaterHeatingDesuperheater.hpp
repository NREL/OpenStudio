/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILWATERHEATINGDESUPERHEATER_HPP
#define MODEL_COILWATERHEATINGDESUPERHEATER_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class CurveBiquadratic;
  class HVACComponent;

  namespace detail {

    class CoilWaterHeatingDesuperheater_Impl;

  }  // namespace detail

  /** CoilWaterHeatingDesuperheater is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:WaterHeating:Desuperheater'. */
  class MODEL_API CoilWaterHeatingDesuperheater : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CoilWaterHeatingDesuperheater(const Model& model, Schedule& setpointTemperatureSchedule);

    virtual ~CoilWaterHeatingDesuperheater() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CoilWaterHeatingDesuperheater(const CoilWaterHeatingDesuperheater& other) = default;
    CoilWaterHeatingDesuperheater(CoilWaterHeatingDesuperheater&& other) = default;
    CoilWaterHeatingDesuperheater& operator=(const CoilWaterHeatingDesuperheater&) = default;
    CoilWaterHeatingDesuperheater& operator=(CoilWaterHeatingDesuperheater&&) = default;

    //@}

    static IddObjectType iddObjectType();

    boost::optional<ModelObject> heatRejectionTarget() const;

    /** \fn bool addToHeatRejectionTarget(const ModelObject& heatRejectionTarget)
      \brief Sets the heating rejection target from which the desuperheater heating coil rejects energy.
      \param[in] heatRejectionTarget The heating rejection target.

      Valid input: \n
      \ref WaterHeaterMixed
      \ref WaterHeaterStratified
  */
    bool addToHeatRejectionTarget(const ModelObject& heatRejectionTarget);

    void removeFromHeatRejectionTarget();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    Schedule setpointTemperatureSchedule() const;

    double deadBandTemperatureDifference() const;

    bool isDeadBandTemperatureDifferenceDefaulted() const;

    boost::optional<double> ratedHeatReclaimRecoveryEfficiency() const;

    double ratedInletWaterTemperature() const;

    double ratedOutdoorAirTemperature() const;

    double maximumInletWaterTemperatureforHeatReclaim() const;

    boost::optional<CurveBiquadratic> heatReclaimEfficiencyFunctionofTemperatureCurve() const;

    // boost::optional<HVACComponent> tank() const;

    boost::optional<ModelObject> heatingSource() const;

    double waterFlowRate() const;

    double waterPumpPower() const;

    bool isWaterPumpPowerDefaulted() const;

    double fractionofPumpHeattoWater() const;

    bool isFractionofPumpHeattoWaterDefaulted() const;

    double onCycleParasiticElectricLoad() const;

    bool isOnCycleParasiticElectricLoadDefaulted() const;

    double offCycleParasiticElectricLoad() const;

    bool isOffCycleParasiticElectricLoadDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setSetpointTemperatureSchedule(Schedule& schedule);

    bool setDeadBandTemperatureDifference(double deadBandTemperatureDifference);

    void resetDeadBandTemperatureDifference();

    bool setRatedHeatReclaimRecoveryEfficiency(double ratedHeatReclaimRecoveryEfficiency);

    void resetRatedHeatReclaimRecoveryEfficiency();

    bool setRatedInletWaterTemperature(double ratedInletWaterTemperature);

    bool setRatedOutdoorAirTemperature(double ratedOutdoorAirTemperature);

    bool setMaximumInletWaterTemperatureforHeatReclaim(double maximumInletWaterTemperatureforHeatReclaim);

    bool setHeatReclaimEfficiencyFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

    void resetHeatReclaimEfficiencyFunctionofTemperatureCurve();

    // bool setTank(const HVACComponent& waterHeater);

    /** \fn bool setHeatingSource(const ModelObject& heatingSource)
      \brief Sets the heating source from which the desuperheater heating coil recovers energy.
      \param[in] heatingSource The heating source.

      Valid inputs: \n
      \ref CoilCoolingDXSingleSpeed \n
      \ref CoilCoolingDXTwoSpeed \n
      \ref CoilCoolingDXTwoStageWithHumidityControlMode \n
      \ref CoilCoolingDXVariableSpeed \n
      \ref CoilCoolingDxMultiSpeed \n
      \ref CoilCoolingWaterToAirHeatPumpEquationFit \n
      \ref RefrigerationCompressorRack \n
      \ref RefrigerationCondenserAirCooled \n
      \ref RefrigerationCondenserEvaporativeCooled \n
      \ref RefrigerationCondenserWaterCooled \n
  */
    bool setHeatingSource(const ModelObject& heatingSource);

    void resetHeatingSource();

    bool setWaterFlowRate(double waterFlowRate);

    bool setWaterPumpPower(double waterPumpPower);

    void resetWaterPumpPower();

    bool setFractionofPumpHeattoWater(double fractionofPumpHeattoWater);

    void resetFractionofPumpHeattoWater();

    bool setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad);

    void resetOnCycleParasiticElectricLoad();

    bool setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad);

    void resetOffCycleParasiticElectricLoad();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CoilWaterHeatingDesuperheater_Impl;

    explicit CoilWaterHeatingDesuperheater(std::shared_ptr<detail::CoilWaterHeatingDesuperheater_Impl> impl);

    friend class detail::CoilWaterHeatingDesuperheater_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CoilWaterHeatingDesuperheater");
  };

  /** \relates CoilWaterHeatingDesuperheater*/
  using OptionalCoilWaterHeatingDesuperheater = boost::optional<CoilWaterHeatingDesuperheater>;

  /** \relates CoilWaterHeatingDesuperheater*/
  using CoilWaterHeatingDesuperheaterVector = std::vector<CoilWaterHeatingDesuperheater>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILWATERHEATINGDESUPERHEATER_HPP
