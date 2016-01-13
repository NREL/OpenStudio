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

#ifndef MODEL_CURVETRIQUADRATIC_HPP
#define MODEL_CURVETRIQUADRATIC_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class CurveTriquadratic_Impl;

} // detail

/** CurveTriquadratic is a Curve that wraps the OpenStudio IDD object 'OS:Curve:Triquadratic'. The
 *  functional form is
 *  \f[\displaystyle \begin{array}{ll}
    f(x) = & c_1 + c_2 \cdot x^2 + c_3 \cdot x + c_4 \cdot y^2 + c_5 \cdot y + c_6 \cdot z^2 + c_7 \cdot z + c_8 \cdot x^2 \cdot y^2 + \\
           & c_9 \cdot x \cdot y + c_{10} \cdot x \cdot y^2 + c_{11} \cdot x^2 \cdot y + c_{12} \cdot x^2 \cdot z^2 + c_{13} \cdot x \cdot z + c_{14} \cdot x \cdot z^2 + \\
           & c_{15} \cdot x^2 \cdot z + c_{16} \cdot y^2 \cdot z^2 + c_{17} \cdot y \cdot z + c_{18} \cdot y \cdot z^2 + c_{19} \cdot y^2 \cdot z + \\
           & c_{20} \cdot x^2 \cdot y^2 \cdot z^2 + c_{21} \cdot z^2 \cdot y^2 \cdot z + c_{22} \cdot x^2 \cdot y \cdot z^2 + c_{23} \cdot x \cdot y^2 \cdot z^2 + \\
           & c_{24} \cdot x^2 \cdot y \cdot z + c_{25} \cdot x \cdot y^2 \cdot z + c_{26} \cdot x \cdot y \cdot z^2 + c_{27} \cdot x \cdot y \cdot z
    \end{array}\f] */
class MODEL_API CurveTriquadratic : public Curve {
 public:

  /** @name Constructors and Destructors */
  //@{

  explicit CurveTriquadratic(const Model& model);

  virtual ~CurveTriquadratic() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validInputUnitTypeforXValues();

  static std::vector<std::string> validInputUnitTypeforYValues();

  static std::vector<std::string> validInputUnitTypeforZValues();

  static std::vector<std::string> validOutputUnitTypeValues();

  /** @name Getters */
  //@{

  boost::optional<double> coefficient1Constant() const;

  boost::optional<double> coefficient2xPOW2() const;

  boost::optional<double> coefficient3x() const;

  boost::optional<double> coefficient4yPOW2() const;

  boost::optional<double> coefficient5y() const;

  boost::optional<double> coefficient6zPOW2() const;

  boost::optional<double> coefficient7z() const;

  boost::optional<double> coefficient8xPOW2TIMESYPOW2() const;

  boost::optional<double> coefficient9xTIMESY() const;

  boost::optional<double> coefficient10xTIMESYPOW2() const;

  boost::optional<double> coefficient11xPOW2TIMESY() const;

  boost::optional<double> coefficient12xPOW2TIMESZPOW2() const;

  boost::optional<double> coefficient13xTIMESZ() const;

  boost::optional<double> coefficient14xTIMESZPOW2() const;

  boost::optional<double> coefficient15xPOW2TIMESZ() const;

  boost::optional<double> coefficient16yPOW2TIMESZPOW2() const;

  boost::optional<double> coefficient17yTIMESZ() const;

  boost::optional<double> coefficient18yTIMESZPOW2() const;

  boost::optional<double> coefficient19yPOW2TIMESZ() const;

  boost::optional<double> coefficient20xPOW2TIMESYPOW2TIMESZPOW2() const;

  boost::optional<double> coefficient21xPOW2TIMESYPOW2TIMESZ() const;

  boost::optional<double> coefficient22xPOW2TIMESYTIMESZPOW2() const;

  boost::optional<double> coefficient23xTIMESYPOW2TIMESZPOW2() const;

  boost::optional<double> coefficient24xPOW2TIMESYTIMESZ() const;

  boost::optional<double> coefficient25xTIMESYPOW2TIMESZ() const;

  boost::optional<double> coefficient26xTIMESYTIMESZPOW2() const;

  boost::optional<double> coefficient27xTIMESYTIMESZ() const;

  boost::optional<double> minimumValueofx() const;

  boost::optional<double> maximumValueofx() const;

  boost::optional<double> minimumValueofy() const;

  boost::optional<double> maximumValueofy() const;

  boost::optional<double> minimumValueofz() const;

  boost::optional<double> maximumValueofz() const;

  boost::optional<double> minimumCurveOutput() const;

  boost::optional<double> maximumCurveOutput() const;

  std::string inputUnitTypeforX() const;

  bool isInputUnitTypeforXDefaulted() const;

  std::string inputUnitTypeforY() const;

  bool isInputUnitTypeforYDefaulted() const;

  std::string inputUnitTypeforZ() const;

  bool isInputUnitTypeforZDefaulted() const;

  std::string outputUnitType() const;

  bool isOutputUnitTypeDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  void setCoefficient1Constant(double coefficient1Constant);

  void resetCoefficient1Constant();

  void setCoefficient2xPOW2(double coefficient2xPOW2);

  void resetCoefficient2xPOW2();

  void setCoefficient3x(double coefficient3x);

  void resetCoefficient3x();

  void setCoefficient4yPOW2(double coefficient4yPOW2);

  void resetCoefficient4yPOW2();

  void setCoefficient5y(double coefficient5y);

  void resetCoefficient5y();

  void setCoefficient6zPOW2(double coefficient6zPOW2);

  void resetCoefficient6zPOW2();

  void setCoefficient7z(double coefficient7z);

  void resetCoefficient7z();

  void setCoefficient8xPOW2TIMESYPOW2(double coefficient8xPOW2TIMESYPOW2);

  void resetCoefficient8xPOW2TIMESYPOW2();

  void setCoefficient9xTIMESY(double coefficient9xTIMESY);

  void resetCoefficient9xTIMESY();

  void setCoefficient10xTIMESYPOW2(double coefficient10xTIMESYPOW2);

  void resetCoefficient10xTIMESYPOW2();

  void setCoefficient11xPOW2TIMESY(double coefficient11xPOW2TIMESY);

  void resetCoefficient11xPOW2TIMESY();

  void setCoefficient12xPOW2TIMESZPOW2(double coefficient12xPOW2TIMESZPOW2);

  void resetCoefficient12xPOW2TIMESZPOW2();

  void setCoefficient13xTIMESZ(double coefficient13xTIMESZ);

  void resetCoefficient13xTIMESZ();

  void setCoefficient14xTIMESZPOW2(double coefficient14xTIMESZPOW2);

  void resetCoefficient14xTIMESZPOW2();

  void setCoefficient15xPOW2TIMESZ(double coefficient15xPOW2TIMESZ);

  void resetCoefficient15xPOW2TIMESZ();

  void setCoefficient16yPOW2TIMESZPOW2(double coefficient16yPOW2TIMESZPOW2);

  void resetCoefficient16yPOW2TIMESZPOW2();

  void setCoefficient17yTIMESZ(double coefficient17yTIMESZ);

  void resetCoefficient17yTIMESZ();

  void setCoefficient18yTIMESZPOW2(double coefficient18yTIMESZPOW2);

  void resetCoefficient18yTIMESZPOW2();

  void setCoefficient19yPOW2TIMESZ(double coefficient19yPOW2TIMESZ);

  void resetCoefficient19yPOW2TIMESZ();

  void setCoefficient20xPOW2TIMESYPOW2TIMESZPOW2(double coefficient20xPOW2TIMESYPOW2TIMESZPOW2);

  void resetCoefficient20xPOW2TIMESYPOW2TIMESZPOW2();

  void setCoefficient21xPOW2TIMESYPOW2TIMESZ(double coefficient21xPOW2TIMESYPOW2TIMESZ);

  void resetCoefficient21xPOW2TIMESYPOW2TIMESZ();

  void setCoefficient22xPOW2TIMESYTIMESZPOW2(double coefficient22xPOW2TIMESYTIMESZPOW2);

  void resetCoefficient22xPOW2TIMESYTIMESZPOW2();

  void setCoefficient23xTIMESYPOW2TIMESZPOW2(double coefficient23xTIMESYPOW2TIMESZPOW2);

  void resetCoefficient23xTIMESYPOW2TIMESZPOW2();

  void setCoefficient24xPOW2TIMESYTIMESZ(double coefficient24xPOW2TIMESYTIMESZ);

  void resetCoefficient24xPOW2TIMESYTIMESZ();

  void setCoefficient25xTIMESYPOW2TIMESZ(double coefficient25xTIMESYPOW2TIMESZ);

  void resetCoefficient25xTIMESYPOW2TIMESZ();

  void setCoefficient26xTIMESYTIMESZPOW2(double coefficient26xTIMESYTIMESZPOW2);

  void resetCoefficient26xTIMESYTIMESZPOW2();

  void setCoefficient27xTIMESYTIMESZ(double coefficient27xTIMESYTIMESZ);

  void resetCoefficient27xTIMESYTIMESZ();

  void setMinimumValueofx(double minimumValueofx);

  void resetMinimumValueofx();

  void setMaximumValueofx(double maximumValueofx);

  void resetMaximumValueofx();

  void setMinimumValueofy(double minimumValueofy);

  void resetMinimumValueofy();

  void setMaximumValueofy(double maximumValueofy);

  void resetMaximumValueofy();

  void setMinimumValueofz(double minimumValueofz);

  void resetMinimumValueofz();

  void setMaximumValueofz(double maximumValueofz);

  void resetMaximumValueofz();

  void setMinimumCurveOutput(double minimumCurveOutput);

  void resetMinimumCurveOutput();

  void setMaximumCurveOutput(double maximumCurveOutput);

  void resetMaximumCurveOutput();

  bool setInputUnitTypeforX(std::string inputUnitTypeforX);

  void resetInputUnitTypeforX();

  bool setInputUnitTypeforY(std::string inputUnitTypeforY);

  void resetInputUnitTypeforY();

  bool setInputUnitTypeforZ(std::string inputUnitTypeforZ);

  void resetInputUnitTypeforZ();

  bool setOutputUnitType(std::string outputUnitType);

  void resetOutputUnitType();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::CurveTriquadratic_Impl ImplType;

  explicit CurveTriquadratic(std::shared_ptr<detail::CurveTriquadratic_Impl> impl);

  friend class detail::CurveTriquadratic_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.CurveTriquadratic");
};

/** \relates CurveTriquadratic*/
typedef boost::optional<CurveTriquadratic> OptionalCurveTriquadratic;

/** \relates CurveTriquadratic*/
typedef std::vector<CurveTriquadratic> CurveTriquadraticVector;

} // model
} // openstudio

#endif // MODEL_CURVETRIQUADRATIC_HPP

