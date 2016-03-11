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

#ifndef MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_HPP
#define MODEL_COILHEATINGDXMULTISPEEDSTAGEDATA_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

class Curve;

namespace detail {

  class CoilHeatingDXMultiSpeedStageData_Impl;

} // detail

/** CoilHeatingDXMultiSpeedStageData is a ParentObject that wraps the OpenStudio IDD object 'OS:Coil:Heating:DX:MultiSpeed:StageData'. */
class MODEL_API CoilHeatingDXMultiSpeedStageData : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilHeatingDXMultiSpeedStageData(const Model& model);

  explicit CoilHeatingDXMultiSpeedStageData(const Model& model,
    const Curve& heatingCapacityFunctionofTemperature,
    const Curve& heatingCapacityFunctionofFlowFraction,
    const Curve& energyInputRatioFunctionofTemperature,
    const Curve& energyInputRatioFunctionofFlowFraction,
    const Curve& partLoadFractionCorrelation,
    const Curve& wasteHeatFunctionofTemperature);

  virtual ~CoilHeatingDXMultiSpeedStageData() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<double> grossRatedHeatingCapacity() const;

  bool isGrossRatedHeatingCapacityAutosized() const;

  double grossRatedHeatingCOP() const;

  boost::optional<double> ratedAirFlowRate() const;

  bool isRatedAirFlowRateAutosized() const;

  double ratedSupplyAirFanPowerPerVolumeFlowRate() const;

  Curve heatingCapacityFunctionofTemperatureCurve() const;

  Curve heatingCapacityFunctionofFlowFractionCurve() const;

  Curve energyInputRatioFunctionofTemperatureCurve() const;

  Curve energyInputRatioFunctionofFlowFractionCurve() const;

  Curve partLoadFractionCorrelationCurve() const;

  double ratedWasteHeatFractionofPowerInput() const;

  Curve wasteHeatFunctionofTemperatureCurve() const;

  //@}
  /** @name Setters */
  //@{

  bool setGrossRatedHeatingCapacity(double grossRatedHeatingCapacity);

  void autosizeGrossRatedHeatingCapacity();

  bool setGrossRatedHeatingCOP(double grossRatedHeatingCOP);

  bool setRatedAirFlowRate(double ratedAirFlowRate);

  void autosizeRatedAirFlowRate();

  bool setRatedSupplyAirFanPowerPerVolumeFlowRate(double ratedSupplyAirFanPowerPerVolumeFlowRate);

  bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

  bool setHeatingCapacityFunctionofFlowFractionCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

  bool setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve);

  bool setPartLoadFractionCorrelationCurve(const Curve& curve);

  bool setRatedWasteHeatFractionofPowerInput(double ratedWasteHeatFractionofPowerInput);

  bool setWasteHeatFunctionofTemperatureCurve(const Curve& curve);

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

