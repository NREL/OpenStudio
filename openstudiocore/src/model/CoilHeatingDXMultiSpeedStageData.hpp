/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_HPP
#define MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

class CurveBiquadratic;
class CurveQuadratic;

namespace detail {

  class CoilHeatingDXMultiSpeedStageData_Impl;

} // detail

/** CoilHeatingDXMultiSpeedStageData is a ParentObject that wraps the OpenStudio IDD object 'OS:Coil:Heating:DX:MultiSpeed:StageData'. */
class MODEL_API CoilHeatingDXMultiSpeedStageData : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilHeatingDXMultiSpeedStageData(const Model& model);

  virtual ~CoilHeatingDXMultiSpeedStageData() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<double> grossRatedTotalHeatingCapacity() const;

  bool isGrossRatedTotalHeatingCapacityAutosized() const;

  boost::optional<double> grossRatedSensibleHeatRatio() const;

  bool isGrossRatedSensibleHeatRatioAutosized() const;

  double grossRatedHeatingCOP() const;

  boost::optional<double> ratedAirFlowRate() const;

  bool isRatedAirFlowRateAutosized() const;

  double ratedEvaporatorFanPowerPerVolumeFlowRate() const;

  CurveBiquadratic totalHeatingCapacityFunctionofTemperatureCurve() const;

  CurveQuadratic totalHeatingCapacityFunctionofFlowFractionCurve() const;

  CurveBiquadratic energyInputRatioFunctionofTemperatureCurve() const;

  CurveQuadratic energyInputRatioFunctionofFlowFractionCurve() const;

  CurveQuadratic partLoadFractionCorrelationCurve() const;

  double nominalTimeforCondensateRemovaltoBegin() const;

  double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity() const;

  double maximumCyclingRate() const;

  double latentCapacityTimeConstant() const;

  double ratedWasteHeatFractionofPowerInput() const;

  CurveBiquadratic wasteHeatFunctionofTemperatureCurve() const;

  double evaporativeCondenserEffectiveness() const;

  boost::optional<double> evaporativeCondenserAirFlowRate() const;

  bool isEvaporativeCondenserAirFlowRateAutosized() const;

  boost::optional<double> ratedEvaporativeCondenserPumpPowerConsumption() const;

  bool isRatedEvaporativeCondenserPumpPowerConsumptionAutosized() const;

  //@}
  /** @name Setters */
  //@{

  bool setGrossRatedTotalHeatingCapacity(double grossRatedTotalHeatingCapacity);

  void autosizeGrossRatedTotalHeatingCapacity();

  bool setGrossRatedSensibleHeatRatio(double grossRatedSensibleHeatRatio);

  void autosizeGrossRatedSensibleHeatRatio();

  bool setGrossRatedHeatingCOP(double grossRatedHeatingCOP);

  bool setRatedAirFlowRate(double ratedAirFlowRate);

  void autosizeRatedAirFlowRate();

  bool setRatedEvaporatorFanPowerPerVolumeFlowRate(double ratedEvaporatorFanPowerPerVolumeFlowRate);

  bool setTotalHeatingCapacityFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

  bool setTotalHeatingCapacityFunctionofFlowFractionCurve(const CurveQuadratic& curveQuadratic);

  bool setEnergyInputRatioFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

  bool setEnergyInputRatioFunctionofFlowFractionCurve(const CurveQuadratic& curveQuadratic);

  bool setPartLoadFractionCorrelationCurve(const CurveQuadratic& curveQuadratic);

  bool setNominalTimeforCondensateRemovaltoBegin(double nominalTimeforCondensateRemovaltoBegin);

  bool setRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity(double ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);

  bool setMaximumCyclingRate(double maximumCyclingRate);

  bool setLatentCapacityTimeConstant(double latentCapacityTimeConstant);

  bool setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput);

  bool setWasteHeatFunctionofTemperatureCurve(const CurveBiquadratic& curveBiquadratic);

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
  typedef detail::CoilHeatingDXMultiSpeedStageData_Impl ImplType;

  explicit CoilHeatingDXMultiSpeedStageData(std::shared_ptr<detail::CoilHeatingDXMultiSpeedStageData_Impl> impl);

  friend class detail::CoilHeatingDXMultiSpeedStageData_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingDXMultiSpeedStageData");
};

/** \relates CoilHeatingDXMultiSpeedStageData*/
typedef boost::optional<CoilHeatingDXMultiSpeedStageData> OptionalCoilHeatingDXMultiSpeedStageData;

/** \relates CoilHeatingDXMultiSpeedStageData*/
typedef std::vector<CoilHeatingDXMultiSpeedStageData> CoilHeatingDXMultiSpeedStageDataVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_HPP

