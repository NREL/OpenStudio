/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXMULTISPEEDSTAGEDATA_IMPL_HPP
#define MODEL_COILCOOLINGDXMULTISPEEDSTAGEDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class CoilCoolingDXMultiSpeed;

  namespace detail {

    /** CoilCoolingDXMultiSpeedStageData_Impl is a ParentObject_Impl that is the implementation class for CoilCoolingDXMultiSpeedStageData.*/
    class MODEL_API CoilCoolingDXMultiSpeedStageData_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingDXMultiSpeedStageData_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingDXMultiSpeedStageData_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingDXMultiSpeedStageData_Impl(const CoilCoolingDXMultiSpeedStageData_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingDXMultiSpeedStageData_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual ModelObject clone(Model model) const override;

      // If this object is used by any CoilCoolingDXMultiSpeed, remove the corresponding extensible group to avoid having 'blanks'
      virtual std::vector<IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> grossRatedTotalCoolingCapacity() const;

      bool isGrossRatedTotalCoolingCapacityAutosized() const;

      boost::optional<double> grossRatedSensibleHeatRatio() const;

      bool isGrossRatedSensibleHeatRatioAutosized() const;

      double grossRatedCoolingCOP() const;

      boost::optional<double> ratedAirFlowRate() const;

      bool isRatedAirFlowRateAutosized() const;

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

      bool setGrossRatedTotalCoolingCapacity(boost::optional<double> grossRatedTotalCoolingCapacity);

      void autosizeGrossRatedTotalCoolingCapacity();

      bool setGrossRatedSensibleHeatRatio(boost::optional<double> grossRatedSensibleHeatRatio);

      void autosizeGrossRatedSensibleHeatRatio();

      bool setGrossRatedCoolingCOP(double grossRatedCoolingCOP);

      bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

      void autosizeRatedAirFlowRate();

      bool setRatedEvaporatorFanPowerPerVolumeFlowRate2017(double ratedEvaporatorFanPowerPerVolumeFlowRate2017);

      bool setRatedEvaporatorFanPowerPerVolumeFlowRate2023(double ratedEvaporatorFanPowerPerVolumeFlowRate2023);

      bool setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve);

      bool setTotalCoolingCapacityFunctionofFlowFractionCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve);

      bool setPartLoadFractionCorrelationCurve(const Curve& curve);

      bool setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin);

      bool setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(
        double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

      bool setMaximumCyclingRate(double maximumCyclingRate);

      bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);

      bool setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput);

      bool setWasteHeatFunctionofTemperatureCurve(const Curve& curve);

      bool setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness);

      bool setEvaporativeCondenserAirFlowRate(boost::optional<double> evaporativeCondenserAirFlowRate);

      void autosizeEvaporativeCondenserAirFlowRate();

      bool setRatedEvaporativeCondenserPumpPowerConsumption(boost::optional<double> ratedEvaporativeCondenserPumpPowerConsumption);

      void autosizeRatedEvaporativeCondenserPumpPowerConsumption();

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedGrossRatedTotalCoolingCapacity() const;

      boost::optional<double> autosizedGrossRatedSensibleHeatRatio() const;

      boost::optional<double> autosizedRatedAirFlowRate() const;

      boost::optional<double> autosizedEvaporativeCondenserAirFlowRate() const;

      boost::optional<double> autosizedRatedEvaporativeCondenserPumpPowerConsumption() const;

      void autosize();

      void applySizingValues();

      // Returns the CoilCoolingDXMultiSpeed that references it if any
      boost::optional<CoilCoolingDXMultiSpeed> parentCoil() const;

      // Used to determine the index of this performance data in the
      // list of stages in the parent object.
      boost::optional<std::tuple<int, CoilCoolingDXMultiSpeed>> stageIndexAndParentCoil() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDXMultiSpeedStageData");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Curve> optionalTotalCoolingCapacityFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalTotalCoolingCapacityFunctionofFlowFractionCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionofFlowFractionCurve() const;
      boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXMULTISPEEDSTAGEDATA_IMPL_HPP
