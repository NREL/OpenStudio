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

#ifndef MODEL_REFRACTIONEXTINCTIONGLAZING_HPP
#define MODEL_REFRACTIONEXTINCTIONGLAZING_HPP

#include "ModelAPI.hpp"
#include "Glazing.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class RefractionExtinctionGlazing_Impl;

} // detail

/** RefractionExtinctionGlazing is a Glazing that wraps the OpenStudio IDD object 'OS:WindowMaterial:Glazing:RefractionExtinctionMethod'. */
class MODEL_API RefractionExtinctionGlazing : public Glazing {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefractionExtinctionGlazing(const Model& model,
    double thickness = 0.1,
    double solarIndexofRefraction = 1.1,
    double solarExtinctionCoefficient = 0.1,
    double visibleIndexofRefraction = 1.1,
    double visibleExtinctionCoefficient = 0.1);
    //double dirtCorrectionFactorforSolarandVisibleTransmittance = 0.1);

  virtual ~RefractionExtinctionGlazing() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  //@}
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

  /** The thermal transmittance of the material (dimensionless fraction). */
  double thermalTransmittance() const;

  double thickness() const;

  Quantity getThickness(bool returnIP=false) const;

  double solarIndexofRefraction() const;

  Quantity getSolarIndexofRefraction(bool returnIP=false) const;

  double solarExtinctionCoefficient() const;

  Quantity getSolarExtinctionCoefficient(bool returnIP=false) const;

  double visibleIndexofRefraction() const;

  Quantity getVisibleIndexofRefraction(bool returnIP=false) const;

  double visibleExtinctionCoefficient() const;

  Quantity getVisibleExtinctionCoefficient(bool returnIP=false) const;

  double infraredTransmittanceatNormalIncidence() const;

  Quantity getInfraredTransmittanceatNormalIncidence(bool returnIP=false) const;

  bool isInfraredTransmittanceatNormalIncidenceDefaulted() const;

  double infraredHemisphericalEmissivity() const;

  Quantity getInfraredHemisphericalEmissivity(bool returnIP=false) const;

  bool isInfraredHemisphericalEmissivityDefaulted() const;

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

  /** Sets the conductivity of the material in W/m*K, if possible. */
  bool setThermalConductivity(double value);

  /** Sets the conductance of the material in W/m^2*K, if possible. */
  bool setThermalConductance(double value);

  /** Sets the resistivity of the material in m*K/W, if possible. */
  bool setThermalResistivity(double value);

  /** Sets the resistance of the material in m^2*K/W, if possible. */
  bool setThermalResistance(double value);

  bool setThermalTransmittance(double value);

  bool setThickness(double thickness);

  bool setThickness(const Quantity& thickness);

  bool setSolarIndexofRefraction(double solarIndexofRefraction);

  bool setSolarIndexofRefraction(const Quantity& solarIndexofRefraction);

  bool setSolarExtinctionCoefficient(double solarExtinctionCoefficient);

  bool setSolarExtinctionCoefficient(const Quantity& solarExtinctionCoefficient);

  bool setVisibleIndexofRefraction(double visibleIndexofRefraction);

  bool setVisibleIndexofRefraction(const Quantity& visibleIndexofRefraction);

  bool setVisibleExtinctionCoefficient(double visibleExtinctionCoefficient);

  bool setVisibleExtinctionCoefficient(const Quantity& visibleExtinctionCoefficient);

  bool setInfraredTransmittanceatNormalIncidence(double infraredTransmittanceatNormalIncidence);

  bool setInfraredTransmittanceatNormalIncidence(const Quantity& infraredTransmittanceatNormalIncidence);

  void resetInfraredTransmittanceatNormalIncidence();

  bool setInfraredHemisphericalEmissivity(double infraredHemisphericalEmissivity);

  bool setInfraredHemisphericalEmissivity(const Quantity& infraredHemisphericalEmissivity);

  void resetInfraredHemisphericalEmissivity();

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
 protected:
  /// @cond
  typedef detail::RefractionExtinctionGlazing_Impl ImplType;

  explicit RefractionExtinctionGlazing(std::shared_ptr<detail::RefractionExtinctionGlazing_Impl> impl);

  friend class detail::RefractionExtinctionGlazing_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefractionExtinctionGlazing");
};

/** \relates RefractionExtinctionGlazing*/
typedef boost::optional<RefractionExtinctionGlazing> OptionalRefractionExtinctionGlazing;

/** \relates RefractionExtinctionGlazing*/
typedef std::vector<RefractionExtinctionGlazing> RefractionExtinctionGlazingVector;

} // model
} // openstudio

#endif // MODEL_REFRACTIONEXTINCTIONGLAZING_HPP

