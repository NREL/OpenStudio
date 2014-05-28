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

#ifndef MODEL_SHADE_HPP
#define MODEL_SHADE_HPP

#include "ModelAPI.hpp"
#include "ShadingMaterial.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

namespace detail {

  class Shade_Impl;

} // detail

/** Shade is a ShadingMaterial that wraps the OpenStudio IDD object 'OS:WindowMaterial:Shade'. */
class MODEL_API Shade : public ShadingMaterial {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit Shade(const Model& model,
    double solarTransmittance = 0.4,
    double solarReflectance = 0.5,
    double visibleTransmittance = 0.4,
    double visibleReflectance = 0.5,
    double thermalHemisphericalEmissivity = 0.9,
    double thermalTransmittance = 0.0,
    double thickness = 0.005,
    double conductivity = 0.1);

  virtual ~Shade() {}

  //@}

  static IddObjectType iddObjectType();

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

  double solarTransmittance() const;

  Quantity getSolarTransmittance(bool returnIP=false) const;

  double solarReflectance() const;

  Quantity getSolarReflectance(bool returnIP=false) const;

  double visibleTransmittance() const;

  Quantity getVisibleTransmittance(bool returnIP=false) const;

  double visibleReflectance() const;

  Quantity getVisibleReflectance(bool returnIP=false) const;

  double thermalHemisphericalEmissivity() const;

  Quantity getThermalHemisphericalEmissivity(bool returnIP=false) const;

  double thermalTransmittance() const;

  double solarAbsorptance() const;

  double visibleAbsorptance() const;

  Quantity getThermalTransmittance(bool returnIP=false) const;

  double thickness() const;

  Quantity getThickness(bool returnIP=false) const;

  double conductivity() const;

  Quantity getConductivity(bool returnIP=false) const;

  double shadetoGlassDistance() const;

  Quantity getShadetoGlassDistance(bool returnIP=false) const;

  bool isShadetoGlassDistanceDefaulted() const;

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

  double airflowPermeability() const;

  Quantity getAirflowPermeability(bool returnIP=false) const;

  bool isAirflowPermeabilityDefaulted() const;

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

  bool setSolarTransmittance(double value);

  bool setSolarReflectance(double value);

  bool setVisibleTransmittance(double value);

  bool setVisibleReflectance(double value);

  bool setSolarTransmittance(const Quantity& solarTransmittance);

  bool setSolarReflectance(const Quantity& solarReflectance);

  bool setVisibleTransmittance(const Quantity& visibleTransmittance);

  bool setVisibleReflectance(const Quantity& visibleReflectance);

  bool setThermalHemisphericalEmissivity(double thermalHemisphericalEmissivity);

  bool setThermalHemisphericalEmissivity(const Quantity& thermalHemisphericalEmissivity);

  bool setThermalTransmittance(const Quantity& thermalTransmittance);

  bool setThickness(double thickness);

  bool setThickness(const Quantity& thickness);

  bool setConductivity(double conductivity);

  bool setConductivity(const Quantity& conductivity);

  bool setShadetoGlassDistance(double shadetoGlassDistance);

  bool setShadetoGlassDistance(const Quantity& shadetoGlassDistance);

  void resetShadetoGlassDistance();

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

  bool setAirflowPermeability(double airflowPermeability);

  bool setAirflowPermeability(const Quantity& airflowPermeability);

  void resetAirflowPermeability();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::Shade_Impl ImplType;

  explicit Shade(std::shared_ptr<detail::Shade_Impl> impl);

  friend class detail::Shade_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.Shade");
};

/** \relates Shade*/
typedef boost::optional<Shade> OptionalShade;

/** \relates Shade*/
typedef std::vector<Shade> ShadeVector;

} // model
} // openstudio

#endif // MODEL_SHADE_HPP

