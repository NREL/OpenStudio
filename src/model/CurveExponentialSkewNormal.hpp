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

#ifndef MODEL_CURVEEXPONENTIALSKEWNORMAL_HPP
#define MODEL_CURVEEXPONENTIALSKEWNORMAL_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveExponentialSkewNormal_Impl;

  }  // namespace detail

  /** CurveExponentialSkewNormal is a Curve that wraps the OpenStudio IDD object
 *  'OS:Curve:ExponentialSkewNormal'. The functional form is \f[\displaystyle f(x) = \frac{{e^{ - 0.5 \cdot {z_1}^2}} \cdot \left( {1 + \frac{z_2}{\left| {z_2} \right|} \cdot erf\left( {\frac{\left| {z_2} \right|}{\sqrt 2 }} \right)} \right)}{{e^{ - 0.5 \cdot {z_3}^2}} \cdot \left( {1 + \frac{z_3}{\left| {z_3} \right|} \cdot erf\left( \frac{\left| {z_3} \right|}{\sqrt 2 } \right)} \right)},\ \text{where}\ {z_1} = \frac{x - {c_1}}{c_2}, {z_2} = \frac{{e^{{c_3} \cdot x}} \cdot {c_4} \cdot x - {c_1}}{c_2},\ \text{and}\ {z_3} = \frac{-{c_1}}{c_2}\f] */
  class MODEL_API CurveExponentialSkewNormal : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Sets \f$c_1 = c_2 = c_4 = 1.0,\ c_3 = -1.0\f$, x within [-1.0,1.0]. */
    explicit CurveExponentialSkewNormal(const Model& model);

    virtual ~CurveExponentialSkewNormal() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveExponentialSkewNormal(const CurveExponentialSkewNormal& other) = default;
    CurveExponentialSkewNormal(CurveExponentialSkewNormal&& other) = default;
    CurveExponentialSkewNormal& operator=(const CurveExponentialSkewNormal&) = default;
    CurveExponentialSkewNormal& operator=(CurveExponentialSkewNormal&&) = default;

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

    bool setCoefficient1C1(double coefficient1C1);

    bool setCoefficient2C2(double coefficient2C2);

    bool setCoefficient3C3(double coefficient3C3);

    bool setCoefficient4C4(double coefficient4C4);

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
    using ImplType = detail::CurveExponentialSkewNormal_Impl;

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
  using OptionalCurveExponentialSkewNormal = boost::optional<CurveExponentialSkewNormal>;

  /** \relates CurveExponentialSkewNormal*/
  using CurveExponentialSkewNormalVector = std::vector<CurveExponentialSkewNormal>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEEXPONENTIALSKEWNORMAL_HPP
