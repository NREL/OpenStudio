/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_BOILERSTEAM_HPP
#define MODEL_BOILERSTEAM_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class BoilerSteam_Impl;

} // detail

/** BoilerSteam is a StraightComponent that wraps the OpenStudio IDD object 'OS:Boiler:Steam'. */
class MODEL_API BoilerSteam : public StraightComponent {

 public:

  explicit BoilerSteam(const Model& model);

  virtual ~BoilerSteam() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> fuelTypeValues();

  /** @name Getters and Setters */
  //@{

  std::string fuelType() const;
  bool setFuelType(std::string fuelType);

  boost::optional<double> maximumOperatingPressure() const;
  OSOptionalQuantity getMaximumOperatingPressure(bool returnIP=false) const;
  bool setMaximumOperatingPressure(double maximumOperatingPressure);
  bool setMaximumOperatingPressure(const Quantity& maximumOperatingPressure);
  void resetMaximumOperatingPressure();

  boost::optional<double> theoreticalEfficiency() const;
  OSOptionalQuantity getTheoreticalEfficiency(bool returnIP=false) const;
  bool setTheoreticalEfficiency(double theoreticalEfficiency);
  bool setTheoreticalEfficiency(const Quantity& theoreticalEfficiency);
  void resetTheoreticalEfficiency();

  boost::optional<double> designOutletSteamTemperature() const;
  OSOptionalQuantity getDesignOutletSteamTemperature(bool returnIP=false) const;
  bool setDesignOutletSteamTemperature(double designOutletSteamTemperature);
  bool setDesignOutletSteamTemperature(const Quantity& designOutletSteamTemperature);
  void resetDesignOutletSteamTemperature();

  boost::optional<double> nominalCapacity() const;
  OSOptionalQuantity getNominalCapacity(bool returnIP=false) const;
  bool isNominalCapacityAutosized() const;
  bool setNominalCapacity(double nominalCapacity);
  bool setNominalCapacity(const Quantity& nominalCapacity);
  void resetNominalCapacity();
  void autosizeNominalCapacity();

  boost::optional<double> minimumPartLoadRatio() const;
  OSOptionalQuantity getMinimumPartLoadRatio(bool returnIP=false) const;
  bool setMinimumPartLoadRatio(double minimumPartLoadRatio);
  bool setMinimumPartLoadRatio(const Quantity& minimumPartLoadRatio);
  void resetMinimumPartLoadRatio();

  boost::optional<double> maximumPartLoadRatio() const;
  OSOptionalQuantity getMaximumPartLoadRatio(bool returnIP=false) const;
  bool setMaximumPartLoadRatio(double maximumPartLoadRatio);
  bool setMaximumPartLoadRatio(const Quantity& maximumPartLoadRatio);
  void resetMaximumPartLoadRatio();

  boost::optional<double> optimumPartLoadRatio() const;
  OSOptionalQuantity getOptimumPartLoadRatio(bool returnIP=false) const;
  bool setOptimumPartLoadRatio(double optimumPartLoadRatio);
  bool setOptimumPartLoadRatio(const Quantity& optimumPartLoadRatio);
  void resetOptimumPartLoadRatio();

  boost::optional<double> coefficient1ofFuelUseFunctionofPartLoadRatioCurve() const;
  OSOptionalQuantity getCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(bool returnIP=false) const;
  bool setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(double coefficient1ofFuelUseFunctionofPartLoadRatioCurve);
  bool setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(const Quantity& coefficient1ofFuelUseFunctionofPartLoadRatioCurve);
  void resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve();

  boost::optional<double> coefficient2ofFuelUseFunctionofPartLoadRatioCurve() const;
  OSOptionalQuantity getCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(bool returnIP=false) const;
  bool setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(double coefficient2ofFuelUseFunctionofPartLoadRatioCurve);
  bool setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(const Quantity& coefficient2ofFuelUseFunctionofPartLoadRatioCurve);
  void resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve();

  boost::optional<double> coefficient3ofFuelUseFunctionofPartLoadRatioCurve() const;
  OSOptionalQuantity getCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(bool returnIP=false) const;
  bool setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(double coefficient3ofFuelUseFunctionofPartLoadRatioCurve);
  bool setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(const Quantity& coefficient3ofFuelUseFunctionofPartLoadRatioCurve);
  void resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve();

  double sizingFactor() const;
  Quantity getSizingFactor(bool returnIP=false) const;
  bool isSizingFactorDefaulted() const;
  bool setSizingFactor(double sizingFactor);
  bool setSizingFactor(const Quantity& sizingFactor);
  void resetSizingFactor();

  std::string endUseSubcategory() const;
  bool setEndUseSubcategory(const std::string & endUseSubcategory);


  //@}
  /** @name Other */
  //@{

  //@}

  boost::optional<double> autosizedNominalCapacity() const ;



 protected:

  /// @cond
  typedef detail::BoilerSteam_Impl ImplType;

  explicit BoilerSteam(std::shared_ptr<detail::BoilerSteam_Impl> impl);

  friend class detail::BoilerSteam_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.BoilerSteam");
};

/** \relates BoilerSteam*/
typedef boost::optional<BoilerSteam> OptionalBoilerSteam;

/** \relates BoilerSteam*/
typedef std::vector<BoilerSteam> BoilerSteamVector;

} // model
} // openstudio

#endif // MODEL_BOILERSTEAM_HPP
