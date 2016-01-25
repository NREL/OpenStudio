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

#ifndef MODEL_CHILLERABSORPTIONINDIRECT_HPP
#define MODEL_CHILLERABSORPTIONINDIRECT_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

class Curve;

namespace detail {

  class ChillerAbsorptionIndirect_Impl;

} // detail

/** ChillerAbsorptionIndirect is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:Chiller:Absorption:Indirect'. */
class MODEL_API ChillerAbsorptionIndirect : public WaterToWaterComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ChillerAbsorptionIndirect(const Model& model);

  virtual ~ChillerAbsorptionIndirect() {}

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

  double condenserInletTemperatureLowerLimit() const;

  double chilledWaterOutletTemperatureLowerLimit() const;

  boost::optional<double> designChilledWaterFlowRate() const;

  bool isDesignChilledWaterFlowRateAutosized() const;

  boost::optional<double> designCondenserWaterFlowRate() const;

  bool isDesignCondenserWaterFlowRateAutosized() const;

  std::string chillerFlowMode() const;

  Curve generatorHeatInputFunctionofPartLoadRatioCurve() const;

  Curve pumpElectricInputFunctionofPartLoadRatioCurve() const;

  Curve capacityCorrectionFunctionofCondenserTemperatureCurve() const;

  Curve capacityCorrectionFunctionofChilledWaterTemperatureCurve() const;

  Curve capacityCorrectionFunctionofGeneratorTemperatureCurve() const;

  Curve generatorHeatInputCorrectionFunctionofCondenserTemperatureCurve() const;

  Curve generatorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve() const;

  std::string generatorHeatSourceType() const;

  boost::optional<double> designGeneratorFluidFlowRate() const;

  bool isDesignGeneratorFluidFlowRateAutosized() const;

  double temperatureLowerLimitGeneratorInlet() const;

  double degreeofSubcoolinginSteamGenerator() const;

  double degreeofSubcoolinginSteamCondensateLoop() const;

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

  void setDesignCondenserInletTemperature(double designCondenserInletTemperature);

  void setCondenserInletTemperatureLowerLimit(double condenserInletTemperatureLowerLimit);

  void setChilledWaterOutletTemperatureLowerLimit(double chilledWaterOutletTemperatureLowerLimit);

  bool setDesignChilledWaterFlowRate(double designChilledWaterFlowRate);

  void autosizeDesignChilledWaterFlowRate();

  bool setDesignCondenserWaterFlowRate(double designCondenserWaterFlowRate);

  void autosizeDesignCondenserWaterFlowRate();

  bool setChillerFlowMode(std::string chillerFlowMode);

  bool setGeneratorHeatInputFunctionofPartLoadRatioCurve(const Curve& curve);

  bool setPumpElectricInputFunctionofPartLoadRatioCurve(const Curve& curve);

  bool setCapacityCorrectionFunctionofCondenserTemperatureCurve(const Curve& curve);

  bool setCapacityCorrectionFunctionofChilledWaterTemperatureCurve(const Curve& curve);

  bool setCapacityCorrectionFunctionofGeneratorTemperatureCurve(const Curve& curve);

  bool setGeneratorHeatInputCorrectionFunctionofCondenserTemperatureCurve(const Curve& curve);

  bool setGeneratorHeatInputCorrectionFunctionofChilledWaterTemperatureCurve(const Curve& curve);

  bool setGeneratorHeatSourceType(std::string generatorHeatSourceType);

  void setDesignGeneratorFluidFlowRate(double designGeneratorFluidFlowRate);

  void autosizeDesignGeneratorFluidFlowRate();

  void setTemperatureLowerLimitGeneratorInlet(double temperatureLowerLimitGeneratorInlet);

  bool setDegreeofSubcoolinginSteamGenerator(double degreeofSubcoolinginSteamGenerator);

  bool setDegreeofSubcoolinginSteamCondensateLoop(double degreeofSubcoolinginSteamCondensateLoop);

  bool setSizingFactor(double sizingFactor);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ChillerAbsorptionIndirect_Impl ImplType;

  explicit ChillerAbsorptionIndirect(std::shared_ptr<detail::ChillerAbsorptionIndirect_Impl> impl);

  friend class detail::ChillerAbsorptionIndirect_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ChillerAbsorptionIndirect");
};

/** \relates ChillerAbsorptionIndirect*/
typedef boost::optional<ChillerAbsorptionIndirect> OptionalChillerAbsorptionIndirect;

/** \relates ChillerAbsorptionIndirect*/
typedef std::vector<ChillerAbsorptionIndirect> ChillerAbsorptionIndirectVector;

} // model
} // openstudio

#endif // MODEL_CHILLERABSORPTIONINDIRECT_HPP

