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

#ifndef MODEL_CURVEQUINTLINEAR_HPP
#define MODEL_CURVEQUINTLINEAR_HPP

#include <model/ModelAPI.hpp>
#include "Curve.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class CurveQuintLinear_Impl;

  }  // namespace detail

  /** CurveQuintLinear is a Curve that wraps the OpenStudio IDD object 'OS:Curve:QuintLinear'.
 *  The functional form is \f$f(v, w, x, y, z) = c_1 + c_2 \cdot v + c_3 \cdot w + c_4 \cdot x + c_5 \cdot y + c_6 \cdot z\f$ */
  class MODEL_API CurveQuintLinear : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CurveQuintLinear(const Model& model);

    virtual ~CurveQuintLinear() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveQuintLinear(const CurveQuintLinear& other) = default;
    CurveQuintLinear(CurveQuintLinear&& other) = default;
    CurveQuintLinear& operator=(const CurveQuintLinear&) = default;
    CurveQuintLinear& operator=(CurveQuintLinear&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> inputUnitTypeforvValues();

    static std::vector<std::string> inputUnitTypeforwValues();

    static std::vector<std::string> inputUnitTypeforxValues();

    static std::vector<std::string> inputUnitTypeforyValues();

    static std::vector<std::string> inputUnitTypeforzValues();

    /** @name Getters */
    //@{

    double coefficient1Constant() const;

    double coefficient2v() const;

    double coefficient3w() const;

    double coefficient4x() const;

    double coefficient5y() const;

    double coefficient6z() const;

    double minimumValueofv() const;

    double maximumValueofv() const;

    double minimumValueofw() const;

    double maximumValueofw() const;

    double minimumValueofx() const;

    double maximumValueofx() const;

    double minimumValueofy() const;

    double maximumValueofy() const;

    double minimumValueofz() const;

    double maximumValueofz() const;

    boost::optional<double> minimumCurveOutput() const;

    boost::optional<double> maximumCurveOutput() const;

    std::string inputUnitTypeforv() const;

    bool isInputUnitTypeforvDefaulted() const;

    std::string inputUnitTypeforw() const;

    bool isInputUnitTypeforwDefaulted() const;

    std::string inputUnitTypeforx() const;

    bool isInputUnitTypeforxDefaulted() const;

    std::string inputUnitTypefory() const;

    bool isInputUnitTypeforyDefaulted() const;

    std::string inputUnitTypeforz() const;

    bool isInputUnitTypeforzDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setCoefficient1Constant(double coefficient1Constant);

    bool setCoefficient2v(double coefficient2v);

    bool setCoefficient3w(double coefficient3w);

    bool setCoefficient4x(double coefficient4x);

    bool setCoefficient5y(double coefficient5y);

    bool setCoefficient6z(double coefficient6z);

    bool setMinimumValueofv(double minimumValueofv);

    bool setMaximumValueofv(double maximumValueofv);

    bool setMinimumValueofw(double minimumValueofw);

    bool setMaximumValueofw(double maximumValueofw);

    bool setMinimumValueofx(double minimumValueofx);

    bool setMaximumValueofx(double maximumValueofx);

    bool setMinimumValueofy(double minimumValueofy);

    bool setMaximumValueofy(double maximumValueofy);

    bool setMinimumValueofz(double minimumValueofz);

    bool setMaximumValueofz(double maximumValueofz);

    bool setMinimumCurveOutput(double minimumCurveOutput);

    void resetMinimumCurveOutput();

    bool setMaximumCurveOutput(double maximumCurveOutput);

    void resetMaximumCurveOutput();

    bool setInputUnitTypeforv(const std::string& inputUnitTypeforv);

    void resetInputUnitTypeforv();

    bool setInputUnitTypeforw(const std::string& inputUnitTypeforw);

    void resetInputUnitTypeforw();

    bool setInputUnitTypeforx(const std::string& inputUnitTypeforx);

    void resetInputUnitTypeforx();

    bool setInputUnitTypefory(const std::string& inputUnitTypefory);

    void resetInputUnitTypefory();

    bool setInputUnitTypeforz(const std::string& inputUnitTypeforz);

    void resetInputUnitTypeforz();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CurveQuintLinear_Impl;

    explicit CurveQuintLinear(std::shared_ptr<detail::CurveQuintLinear_Impl> impl);

    friend class detail::CurveQuintLinear_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveQuintLinear");
  };

  /** \relates CurveQuintLinear*/
  using OptionalCurveQuintLinear = boost::optional<CurveQuintLinear>;

  /** \relates CurveQuintLinear*/
  using CurveQuintLinearVector = std::vector<CurveQuintLinear>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEQUINTLINEAR_HPP
