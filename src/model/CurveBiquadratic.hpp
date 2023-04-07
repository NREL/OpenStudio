/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_CURVEBIQUADRATIC_HPP
#define MODEL_CURVEBIQUADRATIC_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveBiquadratic_Impl;

  }  // namespace detail

  /** CurveBiquadratic is a Curve that wraps the OpenStudio IDD object 'OS:Curve:Biquadratic'. The
 *  functional form is \f$f(x) = c_1 + c_2 \cdot x + c_3 \cdot x^2 + c_4 \cdot y + c_5 \cdot y^2 + c_6 \cdot x \cdot y\f$ */
  class MODEL_API CurveBiquadratic : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Initializes all coefficients to 0.0, and sets the bounds on x and y to [0.0,1.0]. */
    explicit CurveBiquadratic(const Model& model);

    virtual ~CurveBiquadratic() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveBiquadratic(const CurveBiquadratic& other) = default;
    CurveBiquadratic(CurveBiquadratic&& other) = default;
    CurveBiquadratic& operator=(const CurveBiquadratic&) = default;
    CurveBiquadratic& operator=(CurveBiquadratic&&) = default;

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

    bool setCoefficient1Constant(double coefficient1Constant);

    bool setCoefficient2x(double coefficient2x);

    bool setCoefficient3xPOW2(double coefficient3xPOW2);

    bool setCoefficient4y(double coefficient4y);

    bool setCoefficient5yPOW2(double coefficient5yPOW2);

    bool setCoefficient6xTIMESY(double coefficient6xTIMESY);

    bool setMinimumValueofx(double minimumValueofx);

    bool setMaximumValueofx(double maximumValueofx);

    bool setMinimumValueofy(double minimumValueofy);

    bool setMaximumValueofy(double maximumValueofy);

    bool setMinimumCurveOutput(double minimumCurveOutput);

    void resetMinimumCurveOutput();

    bool setMaximumCurveOutput(double maximumCurveOutput);

    void resetMaximumCurveOutput();

    bool setInputUnitTypeforX(const std::string& inputUnitTypeforX);

    void resetInputUnitTypeforX();

    bool setInputUnitTypeforY(const std::string& inputUnitTypeforY);

    void resetInputUnitTypeforY();

    bool setOutputUnitType(const std::string& outputUnitType);

    void resetOutputUnitType();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CurveBiquadratic_Impl;

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
  using OptionalCurveBiquadratic = boost::optional<CurveBiquadratic>;

  /** \relates CurveBiquadratic*/
  using CurveBiquadraticVector = std::vector<CurveBiquadratic>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEBIQUADRATIC_HPP
