/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXMULTISPEEDSTAGEDATA_HPP
#define MODEL_COILCOOLINGDXMULTISPEEDSTAGEDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../model/CoilCoolingDXMultiSpeed.hpp"

namespace openstudio {

namespace model {

class Curve;

namespace detail {

  class CoilCoolingDXMultiSpeedStageData_Impl;

} // detail

/** CoilCoolingDXMultiSpeedStageData is a ModelObject that wraps the OpenStudio IDD object 'OS:Coil:Cooling:DX:MultiSpeed:StageData'. */
class MODEL_API CoilCoolingDXMultiSpeedStageData : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilCoolingDXMultiSpeedStageData(const Model& model,
    Curve& coolingCapacityFunctionofTemperature,
    Curve& coolingCapacityFuncionofFlowFraction,
    Curve& energyInputRatioFunctionofTemperature,
    Curve& energyInputRatioFunctionofFlowFraction,
    Curve& partLoadFractionCorrelation,
    Curve& wasteHeatFunctionofTemperature);

  /** Create CoilCoolingDXMultiSpeedStageData with default curves **/
  explicit CoilCoolingDXMultiSpeedStageData(const Model& model);

  virtual ~CoilCoolingDXMultiSpeedStageData() {}

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

  double ratedEvaporatorFanPowerPerVolumeFlowRate() const;

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

  bool setRatedEvaporatorFanPowerPerVolumeFlowRate(double ratedEvaporatorFanPowerPerVolumeFlowRate);

  bool setTotalCoolingCapacityFunctionofTemperatureCurve(const Curve& curve);

  bool setTotalCoolingCapacityFunctionofFlowFractionCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve);

  bool setPartLoadFractionCorrelationCurve(const Curve& curve);

  bool setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin);

  bool setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

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

  boost::optional<double> autosizedGrossRatedTotalCoolingCapacity() const ;

  boost::optional<double> autosizedGrossRatedSensibleHeatRatio() const ;

  boost::optional<double> autosizedRatedAirFlowRate() const ;

  boost::optional<double> autosizedEvaporativeCondenserAirFlowRate() const ;

  boost::optional<double> autosizedRatedEvaporativeCondenserPumpPowerConsumption() const ;

  void autosize();

  void applySizingValues();

  //@}
 protected:
  /// @cond
  typedef detail::CoilCoolingDXMultiSpeedStageData_Impl ImplType;

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
typedef boost::optional<CoilCoolingDXMultiSpeedStageData> OptionalCoilCoolingDXMultiSpeedStageData;

/** \relates CoilCoolingDXMultiSpeedStageData*/
typedef std::vector<CoilCoolingDXMultiSpeedStageData> CoilCoolingDXMultiSpeedStageDataVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXMULTISPEEDSTAGEDATA_HPP

