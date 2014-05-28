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

#ifndef MODEL_OPAQUEMATERIAL_HPP
#define MODEL_OPAQUEMATERIAL_HPP

#include "ModelAPI.hpp"
#include "Material.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class OpaqueMaterial_Impl;
}

/** OpaqueMaterial is a Material that serves as a base class for all \link LayeredConstruction
 *  LayeredConstructions \endlink for which isOpaque. OpaqueMaterial also defines a number of 
 *  virtually implemented attributes. */
class MODEL_API OpaqueMaterial : public Material {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~OpaqueMaterial() {}

  //@}
  /** @name Getters */
  //@{

  /** Thermal conductivity of the Material in W/m*K. Throws openstudio::Exception if unavailable. 
   *
   *  Attribute name: 'thermalConductivity' */
  double thermalConductivity() const;

  /** Thermal conductance of the Material in W/m^2*K. Throws openstudio::Exception if unavailable. 
   *
   *  Attribute name: 'thermalConductance' */ 
  double thermalConductance() const;

  /** Thermal resistivity of the Material in m*K/W. Throws openstudio::Exception if unavailable. 
   *
   *  Attribute name: 'thermalResistivity'. */
  double thermalResistivity() const;

  /** Thermal resistance of the Material in m^2*K/W. Throws openstudio::Exception if unavailable. 
   *
   *  Attribute name: 'thermalResistance'. */
  double thermalResistance() const;

  /** Thermal absorptance of the Material (dimensionless fraction). 
   *
   *  Attribute name: 'thermalAbsorptance' */
  double thermalAbsorptance() const;

  /** Thermal reflectance of the Material (dimensionless fraction). 
   *
   *  Attribute name: 'thermalReflectance' */
  boost::optional<double> thermalReflectance() const;

  /** Solar absorptance of the Material (dimensionless fraction). 
   *
   *  Attribute name: 'solarAbsorptance' */
  double solarAbsorptance() const;

  /** Solar reflectance of the Material (dimensionless fraction). 
   *
   *  Attribute name: 'solarReflectance' */
  boost::optional<double> solarReflectance() const;

  /** Visible absorptance of the Material (dimensionless fraction). 
   *
   *  Attribute name: 'visibleAbsorptance' */
  double visibleAbsorptance() const;

  /** Visible reflectance of the Material (dimensionless fraction). 
   *
   *  Attribute name: 'visibleReflectance' */
  boost::optional<double> visibleReflectance() const;

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
  bool setThermalAbsorptance(double value);

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

  //@}
  /** @name Queries */
  //@{

  /** Energy it takes to raise the temperature of this material 1 K (J/m^2*K). Equal to 
   *  specificHeat() * density() * thickness(). 
   *
   *  Attribute name: 'heatCapacity' */
  boost::optional<double> heatCapacity() const;

  //@}
 protected:
  /// @cond
  typedef detail::OpaqueMaterial_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::OpaqueMaterial_Impl;

  OpaqueMaterial(IddObjectType type,const Model& model);

  explicit OpaqueMaterial(std::shared_ptr<detail::OpaqueMaterial_Impl> impl);  

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.OpaqueMaterial");
};

/** \relates OpaqueMaterial */
typedef boost::optional<OpaqueMaterial> OptionalOpaqueMaterial;

/** \relates OpaqueMaterial */
typedef std::vector<OpaqueMaterial> OpaqueMaterialVector;

} // model
} // openstudio

#endif // MODEL_OPAQUEMATERIAL_HPP
