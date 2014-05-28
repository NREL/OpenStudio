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

#ifndef MODEL_CURVEFUNCTIONALPRESSUREDROP_HPP
#define MODEL_CURVEFUNCTIONALPRESSUREDROP_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class CurveFunctionalPressureDrop_Impl;

} // detail

/** CurveFunctionalPressureDrop is a Curve that wraps the OpenStudio IDD object 
 *  'OS:Curve:Functional:PressureDrop'. The functional form is 
 *  deltaP = (K + f * (L/D)) * (rho * V^2) / 2.0,
 *  where
 *  K = minorLossCoefficient
 *  f calculated from roughness and Moody-chart, or set by fixed friction factor
 *  L = length
 *  D = diameter
 *  rho, V from plant simulation. */
class MODEL_API CurveFunctionalPressureDrop : public Curve {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Sets D = 0.05. */
  explicit CurveFunctionalPressureDrop(const Model& model);

  virtual ~CurveFunctionalPressureDrop() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double diameter() const;

  boost::optional<double> minorLossCoefficient() const;

  boost::optional<double> length() const;

  boost::optional<double> roughness() const;

  boost::optional<double> fixedFrictionFactor() const;

  //@}
  /** @name Setters */
  //@{

  void setDiameter(double diameter);

  void setMinorLossCoefficient(double minorLossCoefficient);

  void resetMinorLossCoefficient();

  void setLength(double length);

  void resetLength();

  void setRoughness(double roughness);

  void resetRoughness();

  void setFixedFrictionFactor(double fixedFrictionFactor);

  void resetFixedFrictionFactor();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CurveFunctionalPressureDrop_Impl ImplType;

  explicit CurveFunctionalPressureDrop(std::shared_ptr<detail::CurveFunctionalPressureDrop_Impl> impl);

  friend class detail::CurveFunctionalPressureDrop_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.CurveFunctionalPressureDrop");
};

/** \relates CurveFunctionalPressureDrop*/
typedef boost::optional<CurveFunctionalPressureDrop> OptionalCurveFunctionalPressureDrop;

/** \relates CurveFunctionalPressureDrop*/
typedef std::vector<CurveFunctionalPressureDrop> CurveFunctionalPressureDropVector;

} // model
} // openstudio

#endif // MODEL_CURVEFUNCTIONALPRESSUREDROP_HPP

