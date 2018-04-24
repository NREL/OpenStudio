/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
 *  functional form is \f$f(x) = c_1 + c_2 \cdot x + c_3 \cdot x^2 + c_4 \cdot x^3\f$ */
class MODEL_API CurveCubic : public Curve {
 public:

  /** @name Constructors and Destructors */
  //@{

  /** Sets \f$c_1 = c_2 = c_3 = 0.0,\ c_4 = 1.0\f$, and sets the range for x at [0.0,1.0]. */
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

  bool setCoefficient1Constant(double coefficient1Constant);

  bool setCoefficient2x(double coefficient2x);

  bool setCoefficient3xPOW2(double coefficient3xPOW2);

  bool setCoefficient4xPOW3(double coefficient4xPOW3);

  bool setMinimumValueofx(double minimumValueofx);

  bool setMaximumValueofx(double maximumValueofx);

  bool setMinimumCurveOutput(double minimumCurveOutput);

  void resetMinimumCurveOutput();

  bool setMaximumCurveOutput(double maximumCurveOutput);

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
