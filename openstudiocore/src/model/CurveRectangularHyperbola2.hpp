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

#ifndef MODEL_CURVERECTANGULARHYPERBOLA2_HPP
#define MODEL_CURVERECTANGULARHYPERBOLA2_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class CurveRectangularHyperbola2_Impl;

} // detail

/** CurveRectangularHyperbola2 is a Curve that wraps the OpenStudio IDD object 
 *  'OS:Curve:RectangularHyperbola2'. The functional form is 
 *  f(x) = (c1 * x) / (c2 + x) + c3 * x. */
class MODEL_API CurveRectangularHyperbola2 : public Curve {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Initializes c1 = c2 = c3 = 1.0, xmin = 0.0, xmax = 1.0. */
  explicit CurveRectangularHyperbola2(const Model& model);

  virtual ~CurveRectangularHyperbola2() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validInputUnitTypeforxValues();

  static std::vector<std::string> validOutputUnitTypeValues();

  /** @name Getters */
  //@{

  double coefficient1C1() const;

  double coefficient2C2() const;

  double coefficient3C3() const;

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
  typedef detail::CurveRectangularHyperbola2_Impl ImplType;

  explicit CurveRectangularHyperbola2(std::shared_ptr<detail::CurveRectangularHyperbola2_Impl> impl);

  friend class detail::CurveRectangularHyperbola2_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.CurveRectangularHyperbola2");
};

/** \relates CurveRectangularHyperbola2*/
typedef boost::optional<CurveRectangularHyperbola2> OptionalCurveRectangularHyperbola2;

/** \relates CurveRectangularHyperbola2*/
typedef std::vector<CurveRectangularHyperbola2> CurveRectangularHyperbola2Vector;

} // model
} // openstudio

#endif // MODEL_CURVERECTANGULARHYPERBOLA2_HPP

