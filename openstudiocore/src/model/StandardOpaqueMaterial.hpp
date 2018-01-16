/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_STANDARDOPAQUEMATERIAL_HPP
#define MODEL_STANDARDOPAQUEMATERIAL_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class StandardOpaqueMaterial_Impl;

} // detail

/** StandardOpaqueMaterial is a OpaqueMaterial that wraps the OpenStudio IDD object 'OS:OpaqueMaterial'. */
class MODEL_API StandardOpaqueMaterial : public OpaqueMaterial {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit  StandardOpaqueMaterial(const Model& model,
                           std::string roughness = "Smooth",
                           double thickness = 0.1,
                           double conductivity = 0.1,
                           double density = 0.1,
                           double specificHeat = 0.1);

  virtual ~StandardOpaqueMaterial() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> roughnessValues();

  /** @name Getters */
  //@{

  /** Thermal conductivity of the OpaqueMaterial in W/m*K. Throws openstudio::Exception if unavailable.
   *
   *  Attribute name: 'thermalConductivity' */
  double thermalConductivity() const;

  /** Thermal conductance of the OpaqueMaterial in W/m^2*K. Throws openstudio::Exception if unavailable.
   *
   *  Attribute name: 'thermalConductance' */
  double thermalConductance() const;

  /** Thermal resistivity of the OpaqueMaterial in m*K/W. Throws openstudio::Exception if unavailable.
   *
   *  Attribute name: 'thermalResistivity'. */
  double thermalResistivity() const;

  /** Thermal resistance of the OpaqueMaterial in m^2*K/W. Throws openstudio::Exception if unavailable.
   *
   *  Attribute name: 'thermalResistance'. */
  double thermalResistance() const;

  /** Thermal reflectance of the OpaqueMaterial (dimensionless fraction).
   *
   *  Attribute name: 'thermalReflectance' */
  boost::optional<double> thermalReflectance() const;

  /** Solar reflectance of the OpaqueMaterial (dimensionless fraction).
   *
   *  Attribute name: 'solarReflectance' */
  boost::optional<double> solarReflectance() const;

  /** Visible reflectance of the OpaqueMaterial (dimensionless fraction).
   *
   *  Attribute name: 'visibleReflectance' */
  boost::optional<double> visibleReflectance() const;

  std::string roughness() const;

  double thickness() const;

  Quantity getThickness(bool returnIP=false) const;

  double conductivity() const;

  Quantity getConductivity(bool returnIP=false) const;

  double density() const;

  Quantity getDensity(bool returnIP=false) const;

  double specificHeat() const;

  Quantity getSpecificHeat(bool returnIP=false) const;

  double thermalAbsorptance() const;

  Quantity getThermalAbsorptance(bool returnIP=false) const;

  bool isThermalAbsorptanceDefaulted() const;

  double solarAbsorptance() const;

  Quantity getSolarAbsorptance(bool returnIP=false) const;

  bool isSolarAbsorptanceDefaulted() const;

  double visibleAbsorptance() const;

  Quantity getVisibleAbsorptance(bool returnIP=false) const;

  bool isVisibleAbsorptanceDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  /** Set the thermal conductivity (W/m*K).
   *
   *  Attribute name: 'thermalConductivity' */
  bool setThermalConductivity(double value);

  /** Set the thermal conductance (W/m^2*K).
   *
   *  Attribute name: 'thermalConductance' */
  bool setThermalConductance(double value);

  /** Set the thermal resistivity (m*K/W).
   *
   *  Attribute name: 'thermalResistivity' */
  bool setThermalResistivity(double value);

  /** Set the thermal resistance (m^2*K/W).
   *
   *  Attribute name: 'thermalResistance' */
  bool setThermalResistance(double value);

  /** Set the thermal absorptance (dimensionless fraction).
   *
   *  Attribute name: 'thermalAbsorptance' */
  bool setThermalAbsorptance(boost::optional<double> value);

  /** Set the thermal reflectance (dimensionless fraction).
   *
   *  Attribute name: 'thermalReflectance' */
  bool setThermalReflectance(boost::optional<double> value);

  /** Set the solar absorptance (dimensionless fraction).
   *
   *  Attribute name: 'solarAbsorptance' */
  bool setSolarAbsorptance(boost::optional<double> value);

  /** Set the solar reflectance (dimensionless fraction).
   *
   *  Attribute name: 'solarReflectance' */
  bool setSolarReflectance(boost::optional<double> value);

  /** Set the visible absorptance (dimensionless fraction).
   *
   *  Attribute name: 'visibleAbsorptance' */
  bool setVisibleAbsorptance(boost::optional<double> value);

  /** Set the visible reflectance (dimensionless fraction).
   *
   *  Attribute name: 'visibleReflectance' */
  bool setVisibleReflectance(boost::optional<double> value);

  bool setRoughness(std::string roughness);

  bool setThickness(double thickness);

  bool setThickness(const Quantity& thickness);

  bool setConductivity(double conductivity);

  bool setConductivity(const Quantity& conductivity);

  bool setDensity(double density);

  bool setDensity(const Quantity& density);

  bool setSpecificHeat(double specificHeat);

  bool setSpecificHeat(const Quantity& specificHeat);

  bool setThermalAbsorptance(double thermalAbsorptance);

  bool setThermalAbsorptance(const Quantity& thermalAbsorptance);

  void resetThermalAbsorptance();

  bool setSolarAbsorptance(double solarAbsorptance);

  bool setSolarAbsorptance(const Quantity& solarAbsorptance);

  void resetSolarAbsorptance();

  bool setVisibleAbsorptance(double visibleAbsorptance);

  bool setVisibleAbsorptance(const Quantity& visibleAbsorptance);

  void resetVisibleAbsorptance();

  //@}
  /** @name Queries */
  //@{

  /** Energy it takes to raise the temperature of this material 1 K (J/m^2*K). Equal to
   *  specificHeat() * density() * thickness().
   *
   *  Attribute name: 'heatCapacity' */
  boost::optional<double> heatCapacity() const;

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::StandardOpaqueMaterial_Impl ImplType;

  explicit StandardOpaqueMaterial(std::shared_ptr<detail::StandardOpaqueMaterial_Impl> impl);

  friend class detail::StandardOpaqueMaterial_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.StandardOpaqueMaterial");
};

/** \relates StandardOpaqueMaterial*/
typedef boost::optional<StandardOpaqueMaterial> OptionalStandardOpaqueMaterial;

/** \relates StandardOpaqueMaterial*/
typedef std::vector<StandardOpaqueMaterial> StadardOpaqueMaterialVector;

} // model
} // openstudio

#endif // MODEL_STANDARDOPAQUEMATERIAL_HPP

