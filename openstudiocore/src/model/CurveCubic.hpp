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

#ifndef MODEL_CURVECUBIC_HPP
#define MODEL_CURVECUBIC_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class CurveCubic_Impl;

} // detail

/** CurveCubic is a Curve that wraps the OpenStudio IDD object 'OS:Curve:Cubic'. The
 *  functional form is f(x) = c1 + c2 * x + c3 * x^2 + c4 * x^3 */
class MODEL_API CurveCubic : public Curve {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Sets c1 = c2 = c3 = 0.0, c4 = 1.0, and sets the range for x at [0.0,1.0]. */
  explicit CurveCubic(const Model& model);

  virtual ~CurveCubic() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validInputUnitTypeforXValues();

  static std::vector<std::string> validOutputUnitTypeValues();

  /** @name Getters */
  //@{

  double coefficient1Constant() const;

  double coefficient2x() const;

  double coefficient3xPOW2() const;

  double coefficient4xPOW3() const;

  double minimumValueofx() const;

  double maximumValueofx() const;

  boost::optional<double> minimumCurveOutput() const;

  boost::optional<double> maximumCurveOutput() const;

  std::string inputUnitTypeforX() const;

  bool isInputUnitTypeforXDefaulted() const;

  std::string outputUnitType() const;

  bool isOutputUnitTypeDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  void setCoefficient1Constant(double coefficient1Constant);

  void setCoefficient2x(double coefficient2x);

  void setCoefficient3xPOW2(double coefficient3xPOW2);

  void setCoefficient4xPOW3(double coefficient4xPOW3);

  void setMinimumValueofx(double minimumValueofx);

  void setMaximumValueofx(double maximumValueofx);

  void setMinimumCurveOutput(double minimumCurveOutput);

  void resetMinimumCurveOutput();

  void setMaximumCurveOutput(double maximumCurveOutput);

  void resetMaximumCurveOutput();

  bool setInputUnitTypeforX(std::string inputUnitTypeforX);

  void resetInputUnitTypeforX();

  bool setOutputUnitType(std::string outputUnitType);

  void resetOutputUnitType();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CurveCubic_Impl ImplType;

  explicit CurveCubic(std::shared_ptr<detail::CurveCubic_Impl> impl);

  friend class detail::CurveCubic_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.CurveCubic");
};

/** \relates CurveCubic*/
typedef boost::optional<CurveCubic> OptionalCurveCubic;

/** \relates CurveCubic*/
typedef std::vector<CurveCubic> CurveCubicVector;

} // model
} // openstudio

#endif // MODEL_CURVECUBIC_HPP

