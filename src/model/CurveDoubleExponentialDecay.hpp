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

#ifndef MODEL_CURVEDOUBLEEXPONENTIALDECAY_HPP
#define MODEL_CURVEDOUBLEEXPONENTIALDECAY_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveDoubleExponentialDecay_Impl;

  }  // namespace detail

  /** CurveDoubleExponentialDecay is a Curve that wraps the OpenStudio IDD object
 *  'OS:Curve:DoubleExponentialDecay'. The functional form is \f$f(x) = ?\f$ */
  class MODEL_API CurveDoubleExponentialDecay : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Sets all coefficients to 0.0 and sets the range for x to [0.0,1.0]. */
    explicit CurveDoubleExponentialDecay(const Model& model);

    virtual ~CurveDoubleExponentialDecay() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveDoubleExponentialDecay(const CurveDoubleExponentialDecay& other) = default;
    CurveDoubleExponentialDecay(CurveDoubleExponentialDecay&& other) = default;
    CurveDoubleExponentialDecay& operator=(const CurveDoubleExponentialDecay&) = default;
    CurveDoubleExponentialDecay& operator=(CurveDoubleExponentialDecay&&) = default;

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
    OS_DEPRECATED double coefficient3C4() const;  // TODO: ill-named, should be coefficient4C4

    double coefficient5C5() const;
    OS_DEPRECATED double coefficient3C5() const;  // TODO: ill-named, should be coefficient5C5

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

    bool setCoefficient1C1(double coefficient1C1);

    bool setCoefficient2C2(double coefficient2C2);

    bool setCoefficient3C3(double coefficient3C3);

    bool setCoefficient4C4(double coefficient4C4);
    OS_DEPRECATED bool setCoefficient3C4(double coefficient4C4);

    bool setCoefficient5C5(double coefficient5C5);
    OS_DEPRECATED bool setCoefficient3C5(double coefficient5C5);

    bool setMinimumValueofx(double minimumValueofx);

    bool setMaximumValueofx(double maximumValueofx);

    bool setMinimumCurveOutput(double minimumCurveOutput);

    void resetMinimumCurveOutput();

    bool setMaximumCurveOutput(double maximumCurveOutput);

    void resetMaximumCurveOutput();

    bool setInputUnitTypeforx(const std::string& inputUnitTypeforx);

    void resetInputUnitTypeforx();

    bool setOutputUnitType(const std::string& outputUnitType);

    void resetOutputUnitType();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CurveDoubleExponentialDecay_Impl;

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
  using OptionalCurveDoubleExponentialDecay = boost::optional<CurveDoubleExponentialDecay>;

  /** \relates CurveDoubleExponentialDecay*/
  using CurveDoubleExponentialDecayVector = std::vector<CurveDoubleExponentialDecay>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEDOUBLEEXPONENTIALDECAY_HPP
