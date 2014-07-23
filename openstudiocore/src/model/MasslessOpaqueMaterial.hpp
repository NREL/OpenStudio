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

#ifndef MODEL_MASSLESSOPAQUEMATERIAL_HPP
#define MODEL_MASSLESSOPAQUEMATERIAL_HPP

#include "ModelAPI.hpp"
#include "OpaqueMaterial.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class MasslessOpaqueMaterial_Impl;

} // detail

/** MasslessOpaqueMaterial is a OpaqueMaterial that wraps the OpenStudio IDD object 'OS:Material:NoMass'. */
class MODEL_API MasslessOpaqueMaterial : public OpaqueMaterial {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit MasslessOpaqueMaterial(const Model& model,
    std::string roughness = "Smooth",
    double thermalResistance = 0.1);

  virtual ~MasslessOpaqueMaterial() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> roughnessValues();

  /** @name Getters */
  //@{

  std::string roughness() const;

  double thermalResistance() const;

  Quantity getThermalResistance(bool returnIP=false) const;

  boost::optional<double> thermalAbsorptance() const;

  Quantity getThermalAbsorptance(bool returnIP=false) const;

  bool isThermalAbsorptanceDefaulted() const;

  boost::optional<double> solarAbsorptance() const;

  Quantity getSolarAbsorptance(bool returnIP=false) const;

  bool isSolarAbsorptanceDefaulted() const;

  boost::optional<double> visibleAbsorptance() const;

  Quantity getVisibleAbsorptance(bool returnIP=false) const;

  bool isVisibleAbsorptanceDefaulted() const;

  double conductivity() const;

  Quantity getConductivity(bool returnIP=false) const;

  double density() const;

  Quantity getDensity(bool returnIP=false) const;

  double specificHeat() const;

  Quantity getSpecificHeat(bool returnIP=false) const;

  //@}
  /** @name Setters */
  //@{

  bool setRoughness(std::string roughness);

  bool setThermalResistance(double thermalResistance);

  bool setThermalResistance(const Quantity& thermalResistance);

  bool setThermalAbsorptance(double thermalAbsorptance);

  bool setThermalAbsorptance(const Quantity& thermalAbsorptance);

  void resetThermalAbsorptance();

  bool setSolarAbsorptance(double solarAbsorptance);

  bool setSolarAbsorptance(const Quantity& solarAbsorptance);

  void resetSolarAbsorptance();

  bool setVisibleAbsorptance(double visibleAbsorptance);

  bool setVisibleAbsorptance(const Quantity& visibleAbsorptance);

  void resetVisibleAbsorptance();

  bool setConductivity(double conductivity);

  bool setConductivity(const Quantity& conductivity);

  bool setDensity(double density);

  bool setDensity(const Quantity& density);

  bool setSpecificHeat(double specificHeat);

  bool setSpecificHeat(const Quantity& specificHeat);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::MasslessOpaqueMaterial_Impl ImplType;

  explicit MasslessOpaqueMaterial(std::shared_ptr<detail::MasslessOpaqueMaterial_Impl> impl);

  friend class detail::MasslessOpaqueMaterial_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.MasslessOpaqueMaterial");
};

/** \relates MasslessOpaqueMaterial*/
typedef boost::optional<MasslessOpaqueMaterial> OptionalMasslessOpaqueMaterial;

/** \relates MasslessOpaqueMaterial*/
typedef std::vector<MasslessOpaqueMaterial> MasslessOpaqueMaterialVector;

} // model
} // openstudio

#endif // MODEL_MASSLESSOPAQUEMATERIAL_HPP

