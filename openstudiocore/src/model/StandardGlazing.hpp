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

#ifndef MODEL_STANDARDGLAZING_HPP
#define MODEL_STANDARDGLAZING_HPP

#include "ModelAPI.hpp"
#include "Glazing.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class MaterialPropertyGlazingSpectralData;

namespace detail {

  class StandardGlazing_Impl;

} // detail

/** StandardGlazing is a Glazing that wraps the OpenStudio IDD object 'OS:WindowMaterial:Glazing'. */
class MODEL_API StandardGlazing : public Glazing {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit StandardGlazing(const Model& model,
                           std::string opticalDataType = "SpectralAverage",
                           double thickness = 0.1);

  virtual ~StandardGlazing() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  static std::vector<std::string> opticalDataTypeValues();

  /** @name Getters */
  //@{

  std::string opticalDataType() const;

  double thickness() const;

  /** deprecated **/
  boost::optional<std::string> windowGlassSpectralDataSetName() const;

  boost::optional<MaterialPropertyGlazingSpectralData> windowGlassSpectralDataSet() const;

  double solarTransmittance() const;

  Quantity getThickness(bool returnIP=false) const;

  boost::optional<double> solarTransmittanceatNormalIncidence() const;

  OSOptionalQuantity getSolarTransmittanceatNormalIncidence(bool returnIP=false) const;

  boost::optional<double> frontSideSolarReflectanceatNormalIncidence() const;

  OSOptionalQuantity getFrontSideSolarReflectanceatNormalIncidence(bool returnIP=false) const;

  boost::optional<double> backSideSolarReflectanceatNormalIncidence() const;

  OSOptionalQuantity getBackSideSolarReflectanceatNormalIncidence(bool returnIP=false) const;

  boost::optional<double> visibleTransmittanceatNormalIncidence() const;

  OSOptionalQuantity getVisibleTransmittanceatNormalIncidence(bool returnIP=false) const;

  boost::optional<double> frontSideVisibleReflectanceatNormalIncidence() const;

  OSOptionalQuantity getFrontSideVisibleReflectanceatNormalIncidence(bool returnIP=false) const;

  boost::optional<double> backSideVisibleReflectanceatNormalIncidence() const;

  double infraredTransmittance() const;

  OSOptionalQuantity getBackSideVisibleReflectanceatNormalIncidence(bool returnIP=false) const;

  double infraredTransmittanceatNormalIncidence() const;

  Quantity getInfraredTransmittanceatNormalIncidence(bool returnIP=false) const;

  bool isInfraredTransmittanceatNormalIncidenceDefaulted() const;

  double frontSideInfraredHemisphericalEmissivity() const;

  Quantity getFrontSideInfraredHemisphericalEmissivity(bool returnIP=false) const;

  bool isFrontSideInfraredHemisphericalEmissivityDefaulted() const;

  double backSideInfraredHemisphericalEmissivity() const;

  Quantity getBackSideInfraredHemisphericalEmissivity(bool returnIP=false) const;

  bool isBackSideInfraredHemisphericalEmissivityDefaulted() const;

  /** The conductivitiy of the material in W/m*K. */
  double thermalConductivity() const;

  double conductivity() const;

  Quantity getConductivity(bool returnIP=false) const;

  bool isConductivityDefaulted() const;

  double dirtCorrectionFactorforSolarandVisibleTransmittance() const;

  Quantity getDirtCorrectionFactorforSolarandVisibleTransmittance(bool returnIP=false) const;

  bool isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted() const;

  bool solarDiffusing() const;

  bool isSolarDiffusingDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setOpticalDataType(std::string opticalDataType);

  /** deprecated **/
  bool setWindowGlassSpectralDataSetName(const std::string& windowGlassSpectralDataSetName);
  void resetWindowGlassSpectralDataSetName();

  bool setWindowGlassSpectralDataSet(const MaterialPropertyGlazingSpectralData& spectralData);
  void resetWindowGlassSpectralDataSet();

  bool setThickness(double thickness);

  bool setThickness(const Quantity& thickness);

  bool setSolarTransmittance(double value);

  bool setSolarTransmittanceatNormalIncidence(double solarTransmittanceatNormalIncidence);

  bool setSolarTransmittanceatNormalIncidence(const Quantity& solarTransmittanceatNormalIncidence);

  void resetSolarTransmittanceatNormalIncidence();

  bool setFrontSideSolarReflectanceatNormalIncidence(double frontSideSolarReflectanceatNormalIncidence);

  bool setFrontSideSolarReflectanceatNormalIncidence(const Quantity& frontSideSolarReflectanceatNormalIncidence);

  void resetFrontSideSolarReflectanceatNormalIncidence();

  bool setBackSideSolarReflectanceatNormalIncidence(double backSideSolarReflectanceatNormalIncidence);

  bool setBackSideSolarReflectanceatNormalIncidence(const Quantity& backSideSolarReflectanceatNormalIncidence);

  void resetBackSideSolarReflectanceatNormalIncidence();

  bool setVisibleTransmittance(double value);

  bool setVisibleTransmittanceatNormalIncidence(double visibleTransmittanceatNormalIncidence);

  bool setVisibleTransmittanceatNormalIncidence(const Quantity& visibleTransmittanceatNormalIncidence);

  void resetVisibleTransmittanceatNormalIncidence();

  bool setFrontSideVisibleReflectanceatNormalIncidence(double frontSideVisibleReflectanceatNormalIncidence);

  bool setFrontSideVisibleReflectanceatNormalIncidence(const Quantity& frontSideVisibleReflectanceatNormalIncidence);

  void resetFrontSideVisibleReflectanceatNormalIncidence();

  bool setBackSideVisibleReflectanceatNormalIncidence(double backSideVisibleReflectanceatNormalIncidence);

  bool setBackSideVisibleReflectanceatNormalIncidence(const Quantity& backSideVisibleReflectanceatNormalIncidence);

  void resetBackSideVisibleReflectanceatNormalIncidence();

  bool setInfraredTransmittance(double value);

  bool setInfraredTransmittanceatNormalIncidence(double infraredTransmittanceatNormalIncidence);

  bool setInfraredTransmittanceatNormalIncidence(const Quantity& infraredTransmittanceatNormalIncidence);

  void resetInfraredTransmittanceatNormalIncidence();

  bool setFrontSideInfraredHemisphericalEmissivity(double frontSideInfraredHemisphericalEmissivity);

  bool setFrontSideInfraredHemisphericalEmissivity(const Quantity& frontSideInfraredHemisphericalEmissivity);

  void resetFrontSideInfraredHemisphericalEmissivity();

  bool setBackSideInfraredHemisphericalEmissivity(double backSideInfraredHemisphericalEmissivity);

  bool setBackSideInfraredHemisphericalEmissivity(const Quantity& backSideInfraredHemisphericalEmissivity);

  void resetBackSideInfraredHemisphericalEmissivity();

  /** Sets the conductivity of the material in W/m*K, if possible. */
  bool setThermalConductivity(double value);

  bool setConductivity(double conductivity);

  bool setConductivity(const Quantity& conductivity);

  void resetConductivity();

  bool setDirtCorrectionFactorforSolarandVisibleTransmittance(double dirtCorrectionFactorforSolarandVisibleTransmittance);

  bool setDirtCorrectionFactorforSolarandVisibleTransmittance(const Quantity& dirtCorrectionFactorforSolarandVisibleTransmittance);

  void resetDirtCorrectionFactorforSolarandVisibleTransmittance();

  bool setSolarDiffusing(bool solarDiffusing);

  void setSolarDiffusingNoFail(bool solarDiffusing);

  void resetSolarDiffusing();

  //@}
  /** @name Other */
  //@{

  //@}
  double interiorVisibleReflectance() const;

  double exteriorVisibleReflectance() const;

  /** The conductance of the material in W/m^2*K. */
  double thermalConductance() const;

  /** The resistivity of the material in m*K/W. */
  double thermalResistivity() const;

  /** The resistance of the material in m^2*K/W. */
  double thermalResistance() const;

  /** Sets the conductance of the material in W/m^2*K, if possible. */
  bool setThermalConductance(double value);

  /** Sets the resistivity of the material in m*K/W, if possible. */
  bool setThermalResistivity(double value);

  /** Sets the resistance of the material in m^2*K/W, if possible. */
  bool setThermalResistance(double value);

 protected:
  /// @cond
  typedef detail::StandardGlazing_Impl ImplType;

  explicit StandardGlazing(std::shared_ptr<detail::StandardGlazing_Impl> impl);

  friend class detail::StandardGlazing_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.StandardGlazing");
};

/** \relates StandardGlazing*/
typedef boost::optional<StandardGlazing> OptionalStandardGlazing;

/** \relates StandardGlazing*/
typedef std::vector<StandardGlazing> StandardGlazingVector;

} // model
} // openstudio

#endif // MODEL_STANDARDGLAZING_HPP
