/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
 *  'OS:Curve:ExponentialSkewNormal'. The functional form is \f[\displaystyle f(x) = \frac{{e^{ - 0.5 \cdot {z_1}^2}} \cdot \left( {1 + \frac{z_2}{\left| {z_2} \right|} \cdot erf\left( {\frac{\left| {z_2} \right|}{\sqrt 2 }} \right)} \right)}{{e^{ - 0.5 \cdot {z_3}^2}} \cdot \left( {1 + \frac{z_3}{\left| {z_3} \right|} \cdot erf\left( \frac{\left| {z_3} \right|}{\sqrt 2 } \right)} \right)},\ \text{where}\ {z_1} = \frac{x - {c_1}}{c_2}, {z_2} = \frac{{e^{{c_3} \cdot x}} \cdot {c_4} \cdot x - {c_1}}{c_2},\ \text{and}\ {z_3} = \frac{-{c_1}}{c_2}\f] */
class MODEL_API CurveExponentialSkewNormal : public Curve {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Sets \f$c_1 = c_2 = c_4 = 1.0,\ c_3 = -1.0\f$, x within [-1.0,1.0]. */
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

