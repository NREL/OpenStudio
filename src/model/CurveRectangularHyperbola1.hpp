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

#ifndef MODEL_CURVERECTANGULARHYPERBOLA1_HPP
#define MODEL_CURVERECTANGULARHYPERBOLA1_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveRectangularHyperbola1_Impl;

  }  // namespace detail

  /** CurveRectangularHyperbola1 is a Curve that wraps the OpenStudio IDD object
 *  'OS:Curve:RectangularHyperbola1'. The functional form is \f$\displaystyle f(x) = \frac{c_1 \cdot x}{c_2 + x} + c_3\f$ */
  class MODEL_API CurveRectangularHyperbola1 : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Initializes \f$c_1 = c_2 = 1.0,\ c_3 = 0.0,\ xmin = 0.0,\ xmax = 1.0\f$ */
    explicit CurveRectangularHyperbola1(const Model& model);

    virtual ~CurveRectangularHyperbola1() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveRectangularHyperbola1(const CurveRectangularHyperbola1& other) = default;
    CurveRectangularHyperbola1(CurveRectangularHyperbola1&& other) = default;
    CurveRectangularHyperbola1& operator=(const CurveRectangularHyperbola1&) = default;
    CurveRectangularHyperbola1& operator=(CurveRectangularHyperbola1&&) = default;

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

    bool setCoefficient1C1(double coefficient1C1);

    bool setCoefficient2C2(double coefficient2C2);

    bool setCoefficient3C3(double coefficient3C3);

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
    using ImplType = detail::CurveRectangularHyperbola1_Impl;

    explicit CurveRectangularHyperbola1(std::shared_ptr<detail::CurveRectangularHyperbola1_Impl> impl);

    friend class detail::CurveRectangularHyperbola1_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveRectangularHyperbola1");
  };

  /** \relates CurveRectangularHyperbola1*/
  using OptionalCurveRectangularHyperbola1 = boost::optional<CurveRectangularHyperbola1>;

  /** \relates CurveRectangularHyperbola1*/
  using CurveRectangularHyperbola1Vector = std::vector<CurveRectangularHyperbola1>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVERECTANGULARHYPERBOLA1_HPP
