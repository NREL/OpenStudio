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

#ifndef MODEL_CHILLERABSORPTION_HPP
#define MODEL_CHILLERABSORPTION_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class ChillerAbsorption_Impl;

} // detail

/** ChillerAbsorption is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:Chiller:Absorption'. */
class MODEL_API ChillerAbsorption : public WaterToWaterComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ChillerAbsorption(const Model& model);

  virtual ~ChillerAbsorption() {}

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

  boost::optional<double> designChilledWaterFlowRate() const;

  bool isDesignChilledWaterFlowRateAutosized() const;

  boost::optional<double> designCondenserWaterFlowRate() const;

  bool isDesignCondenserWaterFlowRateAutosized() const;

  double coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve() const;

  double coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve() const;

  double coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve() const;

  double coefficient1ofthePumpElectricUsePartLoadRatioCurve() const;

  double coefficient2ofthePumpElectricUsePartLoadRatioCurve() const;

  double coefficient3ofthePumpElectricUsePartLoadRatioCurve() const;

  double chilledWaterOutletTemperatureLowerLimit() const;

  std::string chillerFlowMode() const;

  std::string generatorHeatSourceType() const;

  boost::optional<double> designGeneratorFluidFlowRate() const;

  bool isDesignGeneratorFluidFlowRateAutosized() const;

  double degreeofSubcoolinginSteamGenerator() const;

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

  bool setDesignCondenserInletTemperature(double designCondenserInletTemperature);

  bool setDesignChilledWaterFlowRate(double designChilledWaterFlowRate);

  void autosizeDesignChilledWaterFlowRate();

  bool setDesignCondenserWaterFlowRate(double designCondenserWaterFlowRate);

  void autosizeDesignCondenserWaterFlowRate();

  bool setCoefficient1oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient1oftheHotWaterorSteamUsePartLoadRatioCurve);

  bool setCoefficient2oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient2oftheHotWaterorSteamUsePartLoadRatioCurve);

  bool setCoefficient3oftheHotWaterorSteamUsePartLoadRatioCurve(double coefficient3oftheHotWaterorSteamUsePartLoadRatioCurve);

  bool setCoefficient1ofthePumpElectricUsePartLoadRatioCurve(double coefficient1ofthePumpElectricUsePartLoadRatioCurve);

  bool setCoefficient2ofthePumpElectricUsePartLoadRatioCurve(double coefficient2ofthePumpElectricUsePartLoadRatioCurve);

  bool setCoefficient3ofthePumpElectricUsePartLoadRatioCurve(double coefficient3ofthePumpElectricUsePartLoadRatioCurve);

  bool setChilledWaterOutletTemperatureLowerLimit(double chilledWaterOutletTemperatureLowerLimit);

  bool setChillerFlowMode(std::string chillerFlowMode);

  bool setGeneratorHeatSourceType(std::string generatorHeatSourceType);

  bool setDesignGeneratorFluidFlowRate(double designGeneratorFluidFlowRate);

  void autosizeDesignGeneratorFluidFlowRate();

  bool setDegreeofSubcoolinginSteamGenerator(double degreeofSubcoolinginSteamGenerator);

  bool setSizingFactor(double sizingFactor);

  //@}
  /** @name Other */
  //@{

  boost::optional<double> autosizedNominalCapacity() const ;

  boost::optional<double> autosizedNominalPumpingPower() const ;

  boost::optional<double> autosizedDesignChilledWaterFlowRate() const ;

  boost::optional<double> autosizedDesignCondenserWaterFlowRate() const ;

  boost::optional<double> autosizedDesignGeneratorFluidFlowRate() const ;



  //@}
 protected:
  /// @cond
  typedef detail::ChillerAbsorption_Impl ImplType;

  explicit ChillerAbsorption(std::shared_ptr<detail::ChillerAbsorption_Impl> impl);

  friend class detail::ChillerAbsorption_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ChillerAbsorption");
};

/** \relates ChillerAbsorption*/
typedef boost::optional<ChillerAbsorption> OptionalChillerAbsorption;

/** \relates ChillerAbsorption*/
typedef std::vector<ChillerAbsorption> ChillerAbsorptionVector;

} // model
} // openstudio

#endif // MODEL_CHILLERABSORPTION_HPP
