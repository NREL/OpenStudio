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

#ifndef MODEL_STANDARDGLAZING_HPP
#define MODEL_STANDARDGLAZING_HPP

#include "ModelAPI.hpp"
#include "Glazing.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

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

  boost::optional<std::string> windowGlassSpectralDataSetName() const;

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

  void setWindowGlassSpectralDataSetName(std::string windowGlassSpectralDataSetName);

  void resetWindowGlassSpectralDataSetName();

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

  void setSolarDiffusing(bool solarDiffusing);

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

