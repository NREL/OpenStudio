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

