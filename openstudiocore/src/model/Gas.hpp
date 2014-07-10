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

#ifndef MODEL_GAS_HPP
#define MODEL_GAS_HPP

#include "ModelAPI.hpp"
#include "GasLayer.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class Gas_Impl;

} // detail

/** Gas is a GasLayer that wraps the OpenStudio IDD object 'OS:WindowMaterial:Gas'. */
class MODEL_API Gas : public GasLayer {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit Gas(const Model& model,
    std::string gasType = "Air",
    double thickness = 0.003);

  virtual ~Gas() {}

  //@}
  /** @name Static Methods */
  //@{

  static const std::vector<std::string>& validGasTypes();

  static IddObjectType iddObjectType();

  static std::vector<std::string> gasTypeValues();

  //@}

  /** @name Getters */
  //@{

  /** Gets the gas type. */
  std::string gasType() const;

  /** The conductivity (W/m*K) at temperature (K). Will throw if gasType == "Custom" and 
   *  necessary values are not available. */
  double getThermalConductivity(double temperature) const;

  /** The conductance (W/m^2*K) at temperature (K). Will throw if gasType == "Custom" and 
   *  necessary values are not available. */
  double getThermalConductance(double temperature) const;

  /** The resistivity (m*K/W) at temperature (K). Will throw if gasType == "Custom" and 
   *  necessary values are not available. */
  double getThermalResistivity(double temperature) const;

  /** The resistance (m^2*K/W) at temperature (K). Will throw if gasType == "Custom" and 
   *  necessary values are not available. */
  double getThermalResistance(double temperature) const;

  /** The viscosity (kg/m*s) at temperature (K). Will throw if gasType == "Custom" and 
   *  necessary values are not available. */
  double getViscosity(double temperature) const;

  /** The specific heat (J/kg*K) at temperature (K). Will throw if gasType == "Custom" and 
   *  necessary values are not available. */
  double getSpecificHeat(double temperature) const;

  /** The first conductivity coefficient of gasType "Custom" (W/m*K). The conductivity (W/m*K) = 
   *  A (W/m*K) + B (W/m*K^2) * temperature (K) + C (W/m*K^3) * temperature (K^2). */
  boost::optional<double> customConductivityCoefficientA() const;

  /** The second conductivity coefficient of gasType "Custom" (W/m*K^2). The conductivity (W/m*K) = 
   *  A (W/m*K) + B (W/m*K^2) * temperature (K) + C (W/m*K^3) * temperature (K^2). */
  boost::optional<double> customConductivityCoefficientB() const;

  /** The third conductivity coefficient of gasType "Custom" (W/m*K^3). The conductivity (W/m*K) = 
   *  A (W/m*K) + B (W/m*K^2) * temperature (K) + C (W/m*K^3) * temperature (K^2). */
  boost::optional<double> customConductivityCoefficientC() const;

  /** The first viscosity coefficient of gasType "Custom" (kg/m*s). The viscosity (kg/m*s) = 
   *  A (kg/m*s) + B (kg/m*s*K) * temperature (K) + C (kg/m*s*K^2) * temperature (K^2). */
  boost::optional<double> customViscosityCoefficientA() const;

  /** The second viscosity coefficient of gasType "Custom" (kg/m*s). The viscosity (kg/m*s) = 
   *  A (kg/m*s) + B (kg/m*s*K) * temperature (K) + C (kg/m*s*K^2) * temperature (K^2). */
  boost::optional<double> customViscosityCoefficientB() const;

  /** The third viscosity coefficient of gasType "Custom" (kg/m*s). The viscosity (kg/m*s) = 
   *  A (kg/m*s) + B (kg/m*s*K) * temperature (K) + C (kg/m*s*K^2) * temperature (K^2). */
  boost::optional<double> customViscosityCoefficientC() const;

  /** The first specific heat coefficient of gasType "Custom" (J/kg*K). The specific heat (J/kg*K)
   *  = A (J/kg*K) + B (J/kg*K^2) * temperature (K) + C (J/kg*K^3) * temperature (K^2). */
  boost::optional<double> customSpecificHeatCoefficientA() const;

  /** The second specific heat coefficient of gasType "Custom" (J/kg*K). The specific heat (J/kg*K)
   *  = A (J/kg*K) + B (J/kg*K^2) * temperature (K) + C (J/kg*K^3) * temperature (K^2). */
  boost::optional<double> customSpecificHeatCoefficientB() const;

  /** The third specific heat coefficient of gasType "Custom" (J/kg*K). The specific heat (J/kg*K)
   *  = A (J/kg*K) + B (J/kg*K^2) * temperature (K) + C (J/kg*K^3) * temperature (K^2). */
  boost::optional<double> customSpecificHeatCoefficientC() const;

  /** The molecular weight (u) of gasType "Custom". */
  boost::optional<double> customMolecularWeight() const;

  boost::optional<double> specificHeatRatio() const;

  double thickness() const;

  Quantity getThickness(bool returnIP=false) const;

  boost::optional<double> conductivityCoefficientA() const;

  OSOptionalQuantity getConductivityCoefficientA(bool returnIP=false) const;

  boost::optional<double> conductivityCoefficientB() const;

  OSOptionalQuantity getConductivityCoefficientB(bool returnIP=false) const;

  boost::optional<double> conductivityCoefficientC() const;

  OSOptionalQuantity getConductivityCoefficientC(bool returnIP=false) const;

  boost::optional<double> viscosityCoefficientA() const;

  OSOptionalQuantity getViscosityCoefficientA(bool returnIP=false) const;

  boost::optional<double> viscosityCoefficientB() const;

  OSOptionalQuantity getViscosityCoefficientB(bool returnIP=false) const;

  boost::optional<double> viscosityCoefficientC() const;

  OSOptionalQuantity getViscosityCoefficientC(bool returnIP=false) const;

  boost::optional<double> specificHeatCoefficientA() const;

  OSOptionalQuantity getSpecificHeatCoefficientA(bool returnIP=false) const;

  boost::optional<double> specificHeatCoefficientB() const;

  OSOptionalQuantity getSpecificHeatCoefficientB(bool returnIP=false) const;

  boost::optional<double> specificHeatCoefficientC() const;

  OSOptionalQuantity getSpecificHeatCoefficientC(bool returnIP=false) const;

  boost::optional<double> molecularWeight() const;

  OSOptionalQuantity getMolecularWeight(bool returnIP=false) const;

  OSOptionalQuantity getSpecificHeatRatio(bool returnIP=false) const;

  //@}
  /** @name Setters */
  //@{

  /** If gasType() == "Custom", thermalConductivity (W/m*K) = A + B*temperature + C*temperature^2. */
  bool setCustomConductivity(double A, double B, double C);

  /** If gasType() == "Custom", viscosity (g/m*s) = A + B*temperature + C*temperature^2. */
  bool setCustomViscosity(double A, double B, double C);

  /** If gasType() == "Custom", specific heat (J/kg*K) = A + B*temperature + C*temperature^2. */
  bool setCustomSpecificHeat(double A, double B, double C);

  /** If gasType() == "Custom", specifies the molecular weight of the gas. */
  bool setCustomMolecularWeight(double value);

  bool setGasType(std::string gasType);

  bool setThickness(double thickness);

  bool setThickness(const Quantity& thickness);

  void setConductivityCoefficientA(double conductivityCoefficientA);

  bool setConductivityCoefficientA(const Quantity& conductivityCoefficientA);

  void resetConductivityCoefficientA();

  void setConductivityCoefficientB(double conductivityCoefficientB);

  bool setConductivityCoefficientB(const Quantity& conductivityCoefficientB);

  void resetConductivityCoefficientB();

  void setConductivityCoefficientC(double conductivityCoefficientC);

  bool setConductivityCoefficientC(const Quantity& conductivityCoefficientC);

  void resetConductivityCoefficientC();

  bool setViscosityCoefficientA(double viscosityCoefficientA);

  bool setViscosityCoefficientA(const Quantity& viscosityCoefficientA);

  void resetViscosityCoefficientA();

  void setViscosityCoefficientB(double viscosityCoefficientB);

  bool setViscosityCoefficientB(const Quantity& viscosityCoefficientB);

  void resetViscosityCoefficientB();

  void setViscosityCoefficientC(double viscosityCoefficientC);

  bool setViscosityCoefficientC(const Quantity& viscosityCoefficientC);

  void resetViscosityCoefficientC();

  bool setSpecificHeatCoefficientA(double specificHeatCoefficientA);

  bool setSpecificHeatCoefficientA(const Quantity& specificHeatCoefficientA);

  void resetSpecificHeatCoefficientA();

  void setSpecificHeatCoefficientB(double specificHeatCoefficientB);

  bool setSpecificHeatCoefficientB(const Quantity& specificHeatCoefficientB);

  void resetSpecificHeatCoefficientB();

  void setSpecificHeatCoefficientC(double specificHeatCoefficientC);

  bool setSpecificHeatCoefficientC(const Quantity& specificHeatCoefficientC);

  void resetSpecificHeatCoefficientC();

  bool setMolecularWeight(double molecularWeight);

  bool setMolecularWeight(const Quantity& molecularWeight);

  void resetMolecularWeight();

  bool setSpecificHeatRatio(double specificHeatRatio);

  bool setSpecificHeatRatio(const Quantity& specificHeatRatio);

  void resetSpecificHeatRatio();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::Gas_Impl ImplType;

  explicit Gas(std::shared_ptr<detail::Gas_Impl> impl);

  friend class detail::Gas_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.Gas");
};

/** \relates Gas*/
typedef boost::optional<Gas> OptionalGas;

/** \relates Gas*/
typedef std::vector<Gas> GasVector;

} // model
} // openstudio

#endif // MODEL_GAS_HPP

