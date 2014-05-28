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

#ifndef MODEL_FENESTRATIONMATERIAL_HPP
#define MODEL_FENESTRATIONMATERIAL_HPP

#include "ModelAPI.hpp"
#include "Material.hpp"

namespace openstudio {
namespace model {

namespace detail{
  class FenestrationMaterial_Impl;
}

/** FenestrationMaterial is a Material that serves as a base class for materials that can be
 *  used in fenestration constructions. FenestrationMaterial also serves as a repository for
 *  a number of pieces of fenestration-related data used by EnergyPlus. */
class MODEL_API FenestrationMaterial : public Material {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~FenestrationMaterial() {}

  //@}
  /** @name Static Methods */
  //@{

  // ETH@20110615 Remove const & return values.

  /** Returns the inside air temperature used by EnergyPlus for calculating fenestration
   *  u-factors (K). */
  static double ufactorInsideAirTemperature();

  /** Returns the outside air temperature used by EnergyPlus for calculating fenestration
   *  u-factors (K). */
  static double ufactorOutsideAirTemperature();

  /** Returns the conductivity coefficients for air. */
  static const std::vector<double> airThermalConductivityCoefficients();

  /** Returns the viscosity coefficients for air. */
  static const std::vector<double> airViscosityCoefficients();

  /** Returns the specific heat coefficients for air. */
  static const std::vector<double> airSpecificHeatCoefficients();

  /** The molecular weight of air (u). */
  static double airMolecularWeight();

  /** Returns the conductivity coefficients for argon. */
  static const std::vector<double> argonThermalConductivityCoefficients();

  /** Returns the viscosity coefficients for argon. */
  static const std::vector<double> argonViscosityCoefficients();

  /** Returns the specific heat coefficients for argon. */
  static const std::vector<double> argonSpecificHeatCoefficients();

  /** The molecular weight of argon (u). */
  static double argonMolecularWeight();

  /** Returns the conductivity coefficients for krypton. */
  static const std::vector<double> kryptonThermalConductivityCoefficients();

  /** Returns the viscosity coefficients for krypton. */
  static const std::vector<double> kryptonViscosityCoefficients();

  /** Returns the specific heat coefficients for krypton. */
  static const std::vector<double> kryptonSpecificHeatCoefficients();

  /** The molecular weight of krypton (u). */
  static double kryptonMolecularWeight();

  /** Returns the conductivity coefficients for xenon. */
  static const std::vector<double> xenonThermalConductivityCoefficients();

  /** Returns the viscosity coefficients for xenon. */
  static const std::vector<double> xenonViscosityCoefficients();

  /** Returns the specific heat coefficients for xenon. */
  static const std::vector<double> xenonSpecificHeatCoefficients();

  /** The molecular weight of xenon (u). */
  static double xenonMolecularWeight();

  //@}
 protected:
  /// @cond
  typedef detail::FenestrationMaterial_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::FenestrationMaterial_Impl;

  FenestrationMaterial(IddObjectType type,const Model& model);

  explicit FenestrationMaterial(std::shared_ptr<detail::FenestrationMaterial_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.FenestrationMaterial");
};

/** \relates FenestrationMaterial */
typedef boost::optional<FenestrationMaterial> OptionalFenestrationMaterial;

/** \relates FenestrationMaterial */
typedef std::vector<FenestrationMaterial> FenestrationMaterialVector;

} // model
} // openstudio

#endif // MODEL_FENESTRATIONMATERIAL_HPP
