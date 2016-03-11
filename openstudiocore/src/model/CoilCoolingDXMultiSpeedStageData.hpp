/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_COILCOOLINGDXMULTISPEEDSTAGEDATA_HPP
#define MODEL_COILCOOLINGDXMULTISPEEDSTAGEDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

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
};

/** \relates CoilCoolingDXMultiSpeedStageData*/
typedef boost::optional<CoilCoolingDXMultiSpeedStageData> OptionalCoilCoolingDXMultiSpeedStageData;

/** \relates CoilCoolingDXMultiSpeedStageData*/
typedef std::vector<CoilCoolingDXMultiSpeedStageData> CoilCoolingDXMultiSpeedStageDataVector;

} // model
} // openstudio

#endif // MODEL_COILCOOLINGDXMULTISPEEDSTAGEDATA_HPP

