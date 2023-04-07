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

#ifndef MODEL_CURVEFUNCTIONALPRESSUREDROP_HPP
#define MODEL_CURVEFUNCTIONALPRESSUREDROP_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveFunctionalPressureDrop_Impl;

  }  // namespace detail

  /** CurveFunctionalPressureDrop is a Curve that wraps the OpenStudio IDD object
 *  'OS:Curve:Functional:PressureDrop'. The functional form is
 *  \f[\displaystyle \begin{array}{lll}
 *  \Delta P = \left(K + f \cdot \frac{L}{D} \right) \cdot \frac{\rho V^2}{2}\\\\
 *  \text{Where:}\\\\
 *  K & = & \text{minorLossCoefficient}\\
 *  f & = & \text{calculated from roughness and Moody-chart, or set by fixed friction factor}\\
 *  L & = & \text{length}\\
 *  D & = & \text{diameter}\\
 *  \rho & = & V\ \text{from plant simulation}\\
 *  \end{array}\f] */
  class MODEL_API CurveFunctionalPressureDrop : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Sets \f$D = 0.05\f$ */
    explicit CurveFunctionalPressureDrop(const Model& model);

    virtual ~CurveFunctionalPressureDrop() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveFunctionalPressureDrop(const CurveFunctionalPressureDrop& other) = default;
    CurveFunctionalPressureDrop(CurveFunctionalPressureDrop&& other) = default;
    CurveFunctionalPressureDrop& operator=(const CurveFunctionalPressureDrop&) = default;
    CurveFunctionalPressureDrop& operator=(CurveFunctionalPressureDrop&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double diameter() const;

    boost::optional<double> minorLossCoefficient() const;

    boost::optional<double> length() const;

    boost::optional<double> roughness() const;

    boost::optional<double> fixedFrictionFactor() const;

    //@}
    /** @name Setters */
    //@{

    bool setDiameter(double diameter);

    bool setMinorLossCoefficient(double minorLossCoefficient);

    void resetMinorLossCoefficient();

    bool setLength(double length);

    void resetLength();

    bool setRoughness(double roughness);

    void resetRoughness();

    bool setFixedFrictionFactor(double fixedFrictionFactor);

    void resetFixedFrictionFactor();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CurveFunctionalPressureDrop_Impl;

    explicit CurveFunctionalPressureDrop(std::shared_ptr<detail::CurveFunctionalPressureDrop_Impl> impl);

    friend class detail::CurveFunctionalPressureDrop_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveFunctionalPressureDrop");
  };

  /** \relates CurveFunctionalPressureDrop*/
  using OptionalCurveFunctionalPressureDrop = boost::optional<CurveFunctionalPressureDrop>;

  /** \relates CurveFunctionalPressureDrop*/
  using CurveFunctionalPressureDropVector = std::vector<CurveFunctionalPressureDrop>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEFUNCTIONALPRESSUREDROP_HPP
