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

#ifndef MODEL_CURVEFANPRESSURERISE_HPP
#define MODEL_CURVEFANPRESSURERISE_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveFanPressureRise_Impl;

  }  // namespace detail

  /** CurveFanPressureRise is a Curve that wraps the OpenStudio IDD object
 *  'OS:Curve:FanPressureRise'. The functional form is \f$\Delta P_{fan,tot} = c_1 \cdot {Q_{fan}}^2 + c_2 \cdot Q_{fan} + c_3 \cdot Q_{fan} \cdot \sqrt{P_{sm} - P_o} + c_4 \cdot (P_{sm} - P_o),\ \text{where}\ P_o = 0.0\f$ */
  class MODEL_API CurveFanPressureRise : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Sets \f$c_1 = c_2 = c_3 = c_4 = 1.0,\ Q_{fan}\ \text{within}\ [0.0,10.0], P_{sm}\ \text{within}\ [0.0,500.0]\f$ */
    explicit CurveFanPressureRise(const Model& model);

    virtual ~CurveFanPressureRise() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveFanPressureRise(const CurveFanPressureRise& other) = default;
    CurveFanPressureRise(CurveFanPressureRise&& other) = default;
    CurveFanPressureRise& operator=(const CurveFanPressureRise&) = default;
    CurveFanPressureRise& operator=(CurveFanPressureRise&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double coefficient1C1() const;

    double coefficient2C2() const;

    double coefficient3C3() const;

    double coefficient4C4() const;

    double minimumValueofQfan() const;

    double maximumValueofQfan() const;

    double minimumValueofPsm() const;

    double maximumValueofPsm() const;

    boost::optional<double> minimumCurveOutput() const;

    boost::optional<double> maximumCurveOutput() const;

    //@}
    /** @name Setters */
    //@{

    bool setCoefficient1C1(double coefficient1C1);

    bool setCoefficient2C2(double coefficient2C2);

    bool setCoefficient3C3(double coefficient3C3);

    bool setCoefficient4C4(double coefficient4C4);

    bool setMinimumValueofQfan(double minimumValueofQfan);

    bool setMaximumValueofQfan(double maximumValueofQfan);

    bool setMinimumValueofPsm(double minimumValueofPsm);

    bool setMaximumValueofPsm(double maximumValueofPsm);

    bool setMinimumCurveOutput(double minimumCurveOutput);

    void resetMinimumCurveOutput();

    bool setMaximumCurveOutput(double maximumCurveOutput);

    void resetMaximumCurveOutput();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CurveFanPressureRise_Impl;

    explicit CurveFanPressureRise(std::shared_ptr<detail::CurveFanPressureRise_Impl> impl);

    friend class detail::CurveFanPressureRise_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveFanPressureRise");
  };

  /** \relates CurveFanPressureRise*/
  using OptionalCurveFanPressureRise = boost::optional<CurveFanPressureRise>;

  /** \relates CurveFanPressureRise*/
  using CurveFanPressureRiseVector = std::vector<CurveFanPressureRise>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEFANPRESSURERISE_HPP
