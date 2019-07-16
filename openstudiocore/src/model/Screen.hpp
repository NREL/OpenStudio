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

