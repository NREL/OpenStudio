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

#ifndef MODEL_CURVEDOUBLEEXPONENTIALDECAY_HPP
#define MODEL_CURVEDOUBLEEXPONENTIALDECAY_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class CurveDoubleExponentialDecay_Impl;

} // detail

/** CurveDoubleExponentialDecay is a Curve that wraps the OpenStudio IDD object 
 *  'OS:Curve:DoubleExponentialDecay'. The functional form is f(x) = ? */
class MODEL_API CurveDoubleExponentialDecay : public Curve {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Sets all coefficients to 0.0 and sets the range for x to [0.0,1.0]. */
  explicit CurveDoubleExponentialDecay(const Model& model);

  virtual ~CurveDoubleExponentialDecay() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validInputUnitTypeforxValues();

  static std::vector<std::string> validOutputUnitTypeValues();

  /** @name Getters */
  //@{

  double coefficient1C1() const;

  double coefficient2C2() const;

  double coefficient3C3() const;

  double coefficient3C4() const;

  double coefficient3C5() const;

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

  void setCoefficient3C4(double coefficient3C4);

  void setCoefficient3C5(double coefficient3C5);

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
  typedef detail::CurveDoubleExponentialDecay_Impl ImplType;

  explicit CurveDoubleExponentialDecay(std::shared_ptr<detail::CurveDoubleExponentialDecay_Impl> impl);

  friend class detail::CurveDoubleExponentialDecay_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.CurveDoubleExponentialDecay");
};

/** \relates CurveDoubleExponentialDecay*/
typedef boost::optional<CurveDoubleExponentialDecay> OptionalCurveDoubleExponentialDecay;

/** \relates CurveDoubleExponentialDecay*/
typedef std::vector<CurveDoubleExponentialDecay> CurveDoubleExponentialDecayVector;

} // model
} // openstudio

#endif // MODEL_CURVEDOUBLEEXPONENTIALDECAY_HPP

