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

#ifndef MODEL_SCREEN_HPP
#define MODEL_SCREEN_HPP

#include "ModelAPI.hpp"
#include "ShadingMaterial.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class Screen_Impl;

} // detail

/** Screen is a ShadingMaterial that wraps the OpenStudio IDD object 'OS:WindowMaterial:Screen'. */
class MODEL_API Screen : public ShadingMaterial {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit Screen(const Model& model,
                  double diffuseSolarReflectance = 0.08,
                  double diffuseVisibleReflectance = 0.08,
                  double screenMaterialSpacing = 0.00157,
                  double screenMaterialDiameter = 0.000381);

  virtual ~Screen() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> reflectedBeamTransmittanceAccountingMethodValues();

  static std::vector<std::string> angleofResolutionforScreenTransmittanceOutputMapValues();

  /** @name Getters */
  //@{

  /** The conductivitiy of the material in W/m*K. */
  double thermalConductivity() const;

  /** The conductance of the material in W/m^2*K. */
  double thermalConductance() const;

  /** The resistivity of the material in m*K/W. */
  double thermalResistivity() const;

  /** The resistance of the material in m^2*K/W. */
  double thermalResistance() const;

  std::string reflectedBeamTransmittanceAccountingMethod() const;

  bool isReflectedBeamTransmittanceAccountingMethodDefaulted() const;

  double diffuseSolarReflectance() const;

  Quantity getDiffuseSolarReflectance(bool returnIP=false) const;

  double diffuseVisibleReflectance() const;

  Quantity getDiffuseVisibleReflectance(bool returnIP=false) const;

  double thermalHemisphericalEmissivity() const;

  Quantity getThermalHemisphericalEmissivity(bool returnIP=false) const;

  bool isThermalHemisphericalEmissivityDefaulted() const;

  double conductivity() const;

  Quantity getConductivity(bool returnIP=false) const;

  bool isConductivityDefaulted() const;

  double screenMaterialSpacing() const;

  Quantity getScreenMaterialSpacing(bool returnIP=false) const;

  double screenMaterialDiameter() const;

  Quantity getScreenMaterialDiameter(bool returnIP=false) const;

  double screentoGlassDistance() const;

  Quantity getScreentoGlassDistance(bool returnIP=false) const;

  bool isScreentoGlassDistanceDefaulted() const;

  double topOpeningMultiplier() const;

  Quantity getTopOpeningMultiplier(bool returnIP=false) const;

  bool isTopOpeningMultiplierDefaulted() const;

  double bottomOpeningMultiplier() const;

  Quantity getBottomOpeningMultiplier(bool returnIP=false) const;

  bool isBottomOpeningMultiplierDefaulted() const;

  double leftSideOpeningMultiplier() const;

  Quantity getLeftSideOpeningMultiplier(bool returnIP=false) const;

  bool isLeftSideOpeningMultiplierDefaulted() const;

  double rightSideOpeningMultiplier() const;

  Quantity getRightSideOpeningMultiplier(bool returnIP=false) const;

  bool isRightSideOpeningMultiplierDefaulted() const;

  std::string angleofResolutionforScreenTransmittanceOutputMap() const;

  bool isAngleofResolutionforScreenTransmittanceOutputMapDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the conductivity of the material in W/m*K, if possible. */
  bool setThermalConductivity(double value);

  /** Sets the resistivity of the material in m*K/W, if possible. */
  bool setThermalResistivity(double value);

  bool setReflectedBeamTransmittanceAccountingMethod(std::string reflectedBeamTransmittanceAccountingMethod);

  void resetReflectedBeamTransmittanceAccountingMethod();

  bool setDiffuseSolarReflectance(double diffuseSolarReflectance);

  bool setDiffuseSolarReflectance(const Quantity& diffuseSolarReflectance);

  bool setDiffuseVisibleReflectance(double diffuseVisibleReflectance);

  bool setDiffuseVisibleReflectance(const Quantity& diffuseVisibleReflectance);

  bool setThermalHemisphericalEmissivity(double thermalHemisphericalEmissivity);

  bool setThermalHemisphericalEmissivity(const Quantity& thermalHemisphericalEmissivity);

  void resetThermalHemisphericalEmissivity();

  bool setConductivity(double conductivity);

  bool setConductivity(const Quantity& conductivity);

  void resetConductivity();

  bool setScreenMaterialSpacing(double screenMaterialSpacing);

  bool setScreenMaterialSpacing(const Quantity& screenMaterialSpacing);

  bool setScreenMaterialDiameter(double screenMaterialDiameter);

  bool setScreenMaterialDiameter(const Quantity& screenMaterialDiameter);

  bool setScreentoGlassDistance(double screentoGlassDistance);

  bool setScreentoGlassDistance(const Quantity& screentoGlassDistance);

  void resetScreentoGlassDistance();

  bool setTopOpeningMultiplier(double topOpeningMultiplier);

  bool setTopOpeningMultiplier(const Quantity& topOpeningMultiplier);

  void resetTopOpeningMultiplier();

  bool setBottomOpeningMultiplier(double bottomOpeningMultiplier);

  bool setBottomOpeningMultiplier(const Quantity& bottomOpeningMultiplier);

  void resetBottomOpeningMultiplier();

  bool setLeftSideOpeningMultiplier(double leftSideOpeningMultiplier);

  bool setLeftSideOpeningMultiplier(const Quantity& leftSideOpeningMultiplier);

  void resetLeftSideOpeningMultiplier();

  bool setRightSideOpeningMultiplier(double rightSideOpeningMultiplier);

  bool setRightSideOpeningMultiplier(const Quantity& rightSideOpeningMultiplier);

  void resetRightSideOpeningMultiplier();

  bool setAngleofResolutionforScreenTransmittanceOutputMap(std::string angleofResolutionforScreenTransmittanceOutputMap);

  void resetAngleofResolutionforScreenTransmittanceOutputMap();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::Screen_Impl ImplType;

  explicit Screen(std::shared_ptr<detail::Screen_Impl> impl);

  friend class detail::Screen_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.Screen");
};

/** \relates Screen*/
typedef boost::optional<Screen> OptionalScreen;

/** \relates Screen*/
typedef std::vector<Screen> ScreenVector;

} // model
} // openstudio

#endif // MODEL_SCREEN_HPP

