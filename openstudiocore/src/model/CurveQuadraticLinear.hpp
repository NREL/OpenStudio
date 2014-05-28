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

#ifndef MODEL_CURVEQUADRATICLINEAR_HPP
#define MODEL_CURVEQUADRATICLINEAR_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class CurveQuadraticLinear_Impl;

} // detail

/** CurveQuadraticLinear is a Curve that wraps the OpenStudio IDD object 
 *  'OS:Curve:QuadraticLinear'. The functional form is 
 *  f(x) = c1 + c2 * x + c3 * x^2 + (c4 + c5 * x + c6 * x^2) * y. */
class MODEL_API CurveQuadraticLinear : public Curve {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Initializes c1 = c2 = c3 = c4 = c5 = 0.0, c6 = 1.0, x and y within [0.0,1.0]. */
  explicit CurveQuadraticLinear(const Model& model);

  virtual ~CurveQuadraticLinear() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validInputUnitTypeforXValues();

  static std::vector<std::string> validInputUnitTypeforYValues();

  static std::vector<std::string> validOutputUnitTypeValues();

  /** @name Getters */
  //@{

  double coefficient1Constant() const;

  double coefficient2x() const;

  double coefficient3xPOW2() const;

  double coefficient4y() const;

  double coefficient5xTIMESY() const;

  double coefficient6xPOW2TIMESY() const;

  double minimumValueofx() const;

  double maximumValueofx() const;

  double minimumValueofy() const;

  double maximumValueofy() const;

  boost::optional<double> minimumCurveOutput() const;

  boost::optional<double> maximumCurveOutput() const;

  std::string inputUnitTypeforX() const;

  bool isInputUnitTypeforXDefaulted() const;

  std::string inputUnitTypeforY() const;

  bool isInputUnitTypeforYDefaulted() const;

  std::string outputUnitType() const;

  bool isOutputUnitTypeDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  void setCoefficient1Constant(double coefficient1Constant);

  void setCoefficient2x(double coefficient2x);

  void setCoefficient3xPOW2(double coefficient3xPOW2);

  void setCoefficient4y(double coefficient4y);

  void setCoefficient5xTIMESY(double coefficient5xTIMESY);

  void setCoefficient6xPOW2TIMESY(double coefficient6xPOW2TIMESY);

  void setMinimumValueofx(double minimumValueofx);

  void setMaximumValueofx(double maximumValueofx);

  void setMinimumValueofy(double minimumValueofy);

  void setMaximumValueofy(double maximumValueofy);

  void setMinimumCurveOutput(double minimumCurveOutput);

  void resetMinimumCurveOutput();

  void setMaximumCurveOutput(double maximumCurveOutput);

  void resetMaximumCurveOutput();

  bool setInputUnitTypeforX(std::string inputUnitTypeforX);

  void resetInputUnitTypeforX();

  bool setInputUnitTypeforY(std::string inputUnitTypeforY);

  void resetInputUnitTypeforY();

  bool setOutputUnitType(std::string outputUnitType);

  void resetOutputUnitType();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CurveQuadraticLinear_Impl ImplType;

  explicit CurveQuadraticLinear(std::shared_ptr<detail::CurveQuadraticLinear_Impl> impl);

  friend class detail::CurveQuadraticLinear_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.CurveQuadraticLinear");
};

/** \relates CurveQuadraticLinear*/
typedef boost::optional<CurveQuadraticLinear> OptionalCurveQuadraticLinear;

/** \relates CurveQuadraticLinear*/
typedef std::vector<CurveQuadraticLinear> CurveQuadraticLinearVector;

} // model
} // openstudio

#endif // MODEL_CURVEQUADRATICLINEAR_HPP

