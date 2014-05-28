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

#ifndef MODEL_CURVEEXPONENT_HPP
#define MODEL_CURVEEXPONENT_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class CurveExponent_Impl;

} // detail

/** CurveExponent is a Curve that wraps the OpenStudio IDD object 'OS:Curve:Exponent'. The
 *  functional form is f(x) = c1 + c2 * x^c3. */
class MODEL_API CurveExponent : public Curve {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Initializes c1 = 0.0, c2 = c3 = 1.0. Sets the range for x to [0.0,1.0]. */
  explicit CurveExponent(const Model& model);

  virtual ~CurveExponent() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validInputUnitTypeforXValues();

  static std::vector<std::string> validOutputUnitTypeValues();

  /** @name Getters */
  //@{

  double coefficient1Constant() const;

  double coefficient2Constant() const;

  double coefficient3Constant() const;

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

  void setCoefficient2Constant(double coefficient2Constant);

  void setCoefficient3Constant(double coefficient3Constant);

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
  typedef detail::CurveExponent_Impl ImplType;

  explicit CurveExponent(std::shared_ptr<detail::CurveExponent_Impl> impl);

  friend class detail::CurveExponent_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.CurveExponent");
};

/** \relates CurveExponent*/
typedef boost::optional<CurveExponent> OptionalCurveExponent;

/** \relates CurveExponent*/
typedef std::vector<CurveExponent> CurveExponentVector;

} // model
} // openstudio

#endif // MODEL_CURVEEXPONENT_HPP

