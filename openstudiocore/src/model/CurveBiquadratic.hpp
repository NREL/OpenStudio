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

#ifndef MODEL_CURVEBIQUADRATIC_HPP
#define MODEL_CURVEBIQUADRATIC_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class CurveBiquadratic_Impl;

} // detail

/** CurveBiquadratic is a Curve that wraps the OpenStudio IDD object 'OS:Curve:Biquadratic'. The
 *  functional form is f(x) = c1 + c2 * x + c3 * x^2 + c4 * y + c5 * y^2 + c6 * x * y. */
class MODEL_API CurveBiquadratic : public Curve {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Initializes all coefficients to 0.0, and sets the bounds on x and y to [0.0,1.0]. */
  explicit CurveBiquadratic(const Model& model);

  virtual ~CurveBiquadratic() {}

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

  double coefficient5yPOW2() const;

  double coefficient6xTIMESY() const;

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

  void setCoefficient5yPOW2(double coefficient5yPOW2);

  void setCoefficient6xTIMESY(double coefficient6xTIMESY);

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
  typedef detail::CurveBiquadratic_Impl ImplType;

  explicit CurveBiquadratic(std::shared_ptr<detail::CurveBiquadratic_Impl> impl);

  friend class detail::CurveBiquadratic_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.CurveBiquadratic");
};

/** \relates CurveBiquadratic*/
typedef boost::optional<CurveBiquadratic> OptionalCurveBiquadratic;

/** \relates CurveBiquadratic*/
typedef std::vector<CurveBiquadratic> CurveBiquadraticVector;

} // model
} // openstudio

#endif // MODEL_CURVEBIQUADRATIC_HPP
