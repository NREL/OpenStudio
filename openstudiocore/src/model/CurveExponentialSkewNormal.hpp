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

#ifndef MODEL_CURVEEXPONENTIALSKEWNORMAL_HPP
#define MODEL_CURVEEXPONENTIALSKEWNORMAL_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class CurveExponentialSkewNormal_Impl;

} // detail

/** CurveExponentialSkewNormal is a Curve that wraps the OpenStudio IDD object 
 *  'OS:Curve:ExponentialSkewNormal'. The functional form is f(x) = 
 *  ( exp(-0.5 * z1^2) * (1 + z2/abs(z2) * erf (abs(z2)/sqrt(2)) ) / 
 *  ( exp(-.05 * z3^2) * (1 + z3/abs(z3) * erf (abs(z3)/sqrt(2)) ), where
 *  z1 = (x - c1)/c2, 
 *  z2 = (exp(c3 * x) * c4 * x - c1)/c2, and
 *  z3 = -c1 / c2 */
class MODEL_API CurveExponentialSkewNormal : public Curve {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Sets c1 = c2 = c4 = 1.0, c3 = -1.0, x within [-1.0,1.0]. */
  explicit CurveExponentialSkewNormal(const Model& model);

  virtual ~CurveExponentialSkewNormal() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validInputUnitTypeforxValues();

  static std::vector<std::string> validOutputUnitTypeValues();

  /** @name Getters */
  //@{

  double coefficient1C1() const;

  double coefficient2C2() const;

  double coefficient3C3() const;

  double coefficient4C4() const;

  double minimumValueofx() const;

  double maximumValueofx() const;

  boost::optional<double> minimumCurveOutput() const;

  boost::optional<double> maximumCurveOutput() const;

  std::string inputUnitTypeforx() const;

  bool isInputUnitTypeforxDefaulted() const;

  std::string outputUnitType() const;

  bool isOutputUnitTypeDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  void setCoefficient1C1(double coefficient1C1);

  void setCoefficient2C2(double coefficient2C2);

  void setCoefficient3C3(double coefficient3C3);

  void setCoefficient4C4(double coefficient4C4);

  void setMinimumValueofx(double minimumValueofx);

  void setMaximumValueofx(double maximumValueofx);

  void setMinimumCurveOutput(double minimumCurveOutput);

  void resetMinimumCurveOutput();

  void setMaximumCurveOutput(double maximumCurveOutput);

  void resetMaximumCurveOutput();

  bool setInputUnitTypeforx(std::string inputUnitTypeforx);

  void resetInputUnitTypeforx();

  bool setOutputUnitType(std::string outputUnitType);

  void resetOutputUnitType();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CurveExponentialSkewNormal_Impl ImplType;

  explicit CurveExponentialSkewNormal(std::shared_ptr<detail::CurveExponentialSkewNormal_Impl> impl);

  friend class detail::CurveExponentialSkewNormal_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.CurveExponentialSkewNormal");
};

/** \relates CurveExponentialSkewNormal*/
typedef boost::optional<CurveExponentialSkewNormal> OptionalCurveExponentialSkewNormal;

/** \relates CurveExponentialSkewNormal*/
typedef std::vector<CurveExponentialSkewNormal> CurveExponentialSkewNormalVector;

} // model
} // openstudio

#endif // MODEL_CURVEEXPONENTIALSKEWNORMAL_HPP

