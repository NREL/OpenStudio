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

#ifndef MODEL_CURVE_HPP
#define MODEL_CURVE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class Curve_Impl;

} // detail

/** Curve is a ModelObject that serves as a base class for a number of specific types of curves,
 *  each of which is a function of one, two, or three variables. Curve requires its derived 
 *  classes to implement numVariables and evaluate. For examples of objects that use \link Curve
 *  Curves \endlink, see CoilCoolingDXSingleSpeed and PumpVariableSpeed. */
class MODEL_API Curve : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~Curve() {}

  //@}
  /** @name Other */
  //@{

  /** Returns the number of independent variable values required to evaluate this Curve. 
   *  Typically 1 <= numVariables() <= 3. */
  int numVariables() const;

  /** Returns the value of this Curve at x. Throws if x.size() != numVariables(), or if the curve
   *  cannot be evaluated for some type-specific reason. */
  double evaluate(const std::vector<double>& x) const;

  /** Convenience overload that throws if numVariables() != 1. */
  double evaluate(double x) const;

  /** Convenience overload that throws if numVariables() != 2. */
  double evaluate(double x, double y) const;

  /** Convenience overload that throws if numVariables() != 3. */
  double evaluate(double x, double y, double z) const;

  //@}
 protected:
  /// @cond
  typedef detail::Curve_Impl ImplType;

  Curve(IddObjectType type, const Model& model);

  explicit Curve(std::shared_ptr<detail::Curve_Impl> impl);

  friend class detail::Curve_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.Curve");
};

/** \relates Curve*/
typedef boost::optional<Curve> OptionalCurve;

/** \relates Curve*/
typedef std::vector<Curve> CurveVector;

} // model
} // openstudio

#endif // MODEL_CURVE_HPP

