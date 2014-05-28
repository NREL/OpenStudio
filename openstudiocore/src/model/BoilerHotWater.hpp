/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_BOILERHOTWATER_HPP
#define MODEL_BOILERHOTWATER_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Curve;

namespace detail {

  class BoilerHotWater_Impl;

} // detail

/** BoilerHotWater is a StraightComponent that wraps the OpenStudio IDD object 
 *  'OS:Boiler:HotWater'. */
class MODEL_API BoilerHotWater : public StraightComponent {
 public:

  /** @name Constructors and Destructors */
  //@{

  explicit BoilerHotWater(const Model& model);

  virtual ~BoilerHotWater() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validFuelTypeValues();

  static std::vector<std::string> validEfficiencyCurveTemperatureEvaluationVariableValues();

  static std::vector<std::string> validBoilerFlowModeValues();

  /** @name Getters */
  //@{

  std::string fuelType() const;

  boost::optional<double> nominalCapacity() const;

  bool isNominalCapacityAutosized() const;

  double nominalThermalEfficiency() const;

  boost::optional<std::string> efficiencyCurveTemperatureEvaluationVariable() const;

  boost::optional<Curve> normalizedBoilerEfficiencyCurve() const;

  boost::optional<double> designWaterOutletTemperature() const;

  boost::optional<double> designWaterFlowRate() const;

  bool isDesignWaterFlowRateAutosized() const;

  double minimumPartLoadRatio() const;

  bool isMinimumPartLoadRatioDefaulted() const;

  double maximumPartLoadRatio() const;

  bool isMaximumPartLoadRatioDefaulted() const;

  double optimumPartLoadRatio() const;

  bool isOptimumPartLoadRatioDefaulted() const;

  // TODO: Handle Non-Extensible IddField Boiler Water Inlet Node Name.

  // TODO: Handle Non-Extensible IddField Boiler Water Outlet Node Name.

  double waterOutletUpperTemperatureLimit() const;

  bool isWaterOutletUpperTemperatureLimitDefaulted() const;

  std::string boilerFlowMode() const;

  bool isBoilerFlowModeDefaulted() const;

  boost::optional<double> parasiticElectricLoad() const;

  double sizingFactor() const;

  bool isSizingFactorDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setFuelType(std::string fuelType);

  bool setNominalCapacity(double nominalCapacity);

  void resetNominalCapacity();

  void autosizeNominalCapacity();

  bool setNominalThermalEfficiency(double nominalThermalEfficiency);

  bool setEfficiencyCurveTemperatureEvaluationVariable(
      std::string efficiencyCurveTemperatureEvaluationVariable);

  void resetEfficiencyCurveTemperatureEvaluationVariable();

  bool setNormalizedBoilerEfficiencyCurve(const Curve& normalizedBoilerEfficiencyCurve);

  void resetNormalizedBoilerEfficiencyCurve();

  void setDesignWaterOutletTemperature(double designWaterOutletTemperature);

  void resetDesignWaterOutletTemperature();

  bool setDesignWaterFlowRate(double designWaterFlowRate);

  void resetDesignWaterFlowRate();

  void autosizeDesignWaterFlowRate();

  bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

  void resetMinimumPartLoadRatio();

  bool setMaximumPartLoadRatio(double maximumPartLoadRatio);

  void resetMaximumPartLoadRatio();

  bool setOptimumPartLoadRatio(double optimumPartLoadRatio);

  void resetOptimumPartLoadRatio();

  // TODO: Handle Non-Extensible IddField Boiler Water Inlet Node Name.

  // TODO: Handle Non-Extensible IddField Boiler Water Outlet Node Name.

  void setWaterOutletUpperTemperatureLimit(double waterOutletUpperTemperatureLimit);

  void resetWaterOutletUpperTemperatureLimit();

  bool setBoilerFlowMode(std::string boilerFlowMode);

  void resetBoilerFlowMode();

  bool setParasiticElectricLoad(double parasiticElectricLoad);

  void resetParasiticElectricLoad();

  bool setSizingFactor(double sizingFactor);

  void resetSizingFactor();

  //@}
 protected:
  /// @cond
  typedef detail::BoilerHotWater_Impl ImplType;

  friend class detail::BoilerHotWater_Impl;
  friend class Model;
  friend class openstudio::detail::IdfObject_Impl;
  friend class IdfObject;

  explicit BoilerHotWater(std::shared_ptr<detail::BoilerHotWater_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.BoilerHotWater");
};

/** \relates BoilerHotWater*/
typedef boost::optional<BoilerHotWater> OptionalBoilerHotWater;

/** \relates BoilerHotWater*/
typedef std::vector<BoilerHotWater> BoilerHotWaterVector;

} // model
} // openstudio

#endif // MODEL_BOILERHOTWATER_HPP

