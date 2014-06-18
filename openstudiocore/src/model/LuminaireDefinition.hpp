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

#ifndef MODEL_LUMINAIREDEFINITION_HPP
#define MODEL_LUMINAIREDEFINITION_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class LuminaireDefinition_Impl;

} // detail

/** LuminaireDefinition is a SpaceLoadDefinition that wraps the OpenStudio IDD object 
 *  'OS_Luminaire_Definition'. \sa Luminaire */
class MODEL_API LuminaireDefinition : public SpaceLoadDefinition {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit LuminaireDefinition(const Model& model);

  virtual ~LuminaireDefinition() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  // TODO: Handle Non-Extensible IddField IES File Path.

  double lightingPower() const;

  bool isLightingPowerDefaulted() const;

  double fractionRadiant() const;

  bool isFractionRadiantDefaulted() const;

  double fractionVisible() const;

  bool isFractionVisibleDefaulted() const;

  double returnAirFraction() const;

  bool isReturnAirFractionDefaulted() const;

  bool returnAirFractionCalculatedfromPlenumTemperature() const;

  bool isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted() const;

  double returnAirFractionFunctionofPlenumTemperatureCoefficient1() const;

  bool isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted() const;

  double returnAirFractionFunctionofPlenumTemperatureCoefficient2() const;

  bool isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Handle Non-Extensible IddField IES File Path.

  bool setLightingPower(double lightingPower);

  void resetLightingPower();

  bool setFractionRadiant(double fractionRadiant);

  void resetFractionRadiant();

  bool setFractionVisible(double fractionVisible);

  void resetFractionVisible();

  bool setReturnAirFraction(double returnAirFraction);

  void resetReturnAirFraction();

  void setReturnAirFractionCalculatedfromPlenumTemperature(bool returnAirFractionCalculatedfromPlenumTemperature);

  void resetReturnAirFractionCalculatedfromPlenumTemperature();

  bool setReturnAirFractionFunctionofPlenumTemperatureCoefficient1(double returnAirFractionFunctionofPlenumTemperatureCoefficient1);

  void resetReturnAirFractionFunctionofPlenumTemperatureCoefficient1();

  bool setReturnAirFractionFunctionofPlenumTemperatureCoefficient2(double returnAirFractionFunctionofPlenumTemperatureCoefficient2);

  void resetReturnAirFractionFunctionofPlenumTemperatureCoefficient2();

  //@}
 protected:
  /// @cond
  typedef detail::LuminaireDefinition_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit LuminaireDefinition(std::shared_ptr<detail::LuminaireDefinition_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.LuminaireDefinition");
};

/** \relates LuminaireDefinition*/
typedef boost::optional<LuminaireDefinition> OptionalLuminaireDefinition;

/** \relates LuminaireDefinition*/
typedef std::vector<LuminaireDefinition> LuminaireDefinitionVector;

} // model
} // openstudio

#endif // MODEL_LUMINAIREDEFINITION_HPP

