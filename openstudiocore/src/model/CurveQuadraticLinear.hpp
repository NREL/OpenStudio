/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
 *  \f$f(x) = c_1 + c_2 \cdot x + c_3 \cdot x^2 + (c_4 + c_5 \cdot x + c_6 \cdot x^2) \cdot y\f$ */
class MODEL_API CurveQuadraticLinear : public Curve {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Initializes \f$c_1 = c_2 = c_3 = c_4 = c_5 = 0.0,\ c_6 = 1.0,\ x\ \text{and}\ y\ \text{within}\ [0.0,1.0]\f$ */
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

