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

  /** @name Getters */
  //@{

  std::string fuelType() const;

  bool setFuelType(std::string fuelType);

  boost::optional<double> maximumOperatingPressure() const;

  OSOptionalQuantity getMaximumOperatingPressure(bool returnIP=false) const;

  void setMaximumOperatingPressure(double maximumOperatingPressure);

  bool setMaximumOperatingPressure(const Quantity& maximumOperatingPressure);

  void resetMaximumOperatingPressure();

  boost::optional<double> theoreticalEfficiency() const;

  OSOptionalQuantity getTheoreticalEfficiency(bool returnIP=false) const;

  bool setTheoreticalEfficiency(double theoreticalEfficiency);

  bool setTheoreticalEfficiency(const Quantity& theoreticalEfficiency);

  void resetTheoreticalEfficiency();

  boost::optional<double> designOutletSteamTemperature() const;

  OSOptionalQuantity getDesignOutletSteamTemperature(bool returnIP=false) const;

  void setDesignOutletSteamTemperature(double designOutletSteamTemperature);

  bool setDesignOutletSteamTemperature(const Quantity& designOutletSteamTemperature);

  void resetDesignOutletSteamTemperature();

  boost::optional<double> nominalCapacity() const;

  OSOptionalQuantity getNominalCapacity(bool returnIP=false) const;

  bool isNominalCapacityAutosized() const;

  void setNominalCapacity(double nominalCapacity);

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

  void setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(double coefficient1ofFuelUseFunctionofPartLoadRatioCurve);

  bool setCoefficient1ofFuelUseFunctionofPartLoadRatioCurve(const Quantity& coefficient1ofFuelUseFunctionofPartLoadRatioCurve);

  void resetCoefficient1ofFuelUseFunctionofPartLoadRatioCurve();

  boost::optional<double> coefficient2ofFuelUseFunctionofPartLoadRatioCurve() const;

  OSOptionalQuantity getCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(bool returnIP=false) const;

  void setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(double coefficient2ofFuelUseFunctionofPartLoadRatioCurve);

  bool setCoefficient2ofFuelUseFunctionofPartLoadRatioCurve(const Quantity& coefficient2ofFuelUseFunctionofPartLoadRatioCurve);

  void resetCoefficient2ofFuelUseFunctionofPartLoadRatioCurve();

  boost::optional<double> coefficient3ofFuelUseFunctionofPartLoadRatioCurve() const;

  OSOptionalQuantity getCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(bool returnIP=false) const;

  void setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(double coefficient3ofFuelUseFunctionofPartLoadRatioCurve);

  bool setCoefficient3ofFuelUseFunctionofPartLoadRatioCurve(const Quantity& coefficient3ofFuelUseFunctionofPartLoadRatioCurve);

  void resetCoefficient3ofFuelUseFunctionofPartLoadRatioCurve();

  double sizingFactor() const;

  Quantity getSizingFactor(bool returnIP=false) const;

  bool isSizingFactorDefaulted() const;

  bool setSizingFactor(double sizingFactor);

  bool setSizingFactor(const Quantity& sizingFactor);

  void resetSizingFactor();

  //@}
  /** @name Other */
  //@{

  //@}

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

