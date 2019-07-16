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

  bool setSolarDiffusing(bool solarDiffusing);

  void setSolarDiffusingNoFail(bool solarDiffusing);

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
