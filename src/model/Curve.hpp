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

#ifndef MODEL_CURVE_HPP
#define MODEL_CURVE_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class Curve_Impl;

  }  // namespace detail

  /** Curve is a ModelObject that serves as a base class for a number of specific types of curves,
 *  each of which is a function of one, two, or three variables. Curve requires its derived
 *  classes to implement numVariables and evaluate. For examples of objects that use \link Curve
 *  Curves \endlink, see CoilCoolingDXSingleSpeed and PumpVariableSpeed. */
  class MODEL_API Curve : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~Curve() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Curve(const Curve& other) = default;
    Curve(Curve&& other) = default;
    Curve& operator=(const Curve&) = default;
    Curve& operator=(Curve&&) = default;

    //@}
    /** @name Other */
    //@{

    /** Returns the number of independent variable values required to evaluate this Curve.
   *  Typically 1 <= numVariables() <= 3. */
    int numVariables() const;

    /** Returns the value of this Curve at x. Throws if x.size() != numVariables(), or if the curve
   *  cannot be evaluated for some type-specific reason. */
    double evaluate(const std::vector<double>& x) const;

    /** Convenience overload that throws if numVariables() != 1. */
    double evaluate(double x) const;

    /** Convenience overload that throws if numVariables() != 2. */
    double evaluate(double x, double y) const;

    /** Convenience overload that throws if numVariables() != 3. */
    double evaluate(double x, double y, double z) const;

    /** Convenience overload that throws if numVariables() != 4. */
    double evaluate(double w, double x, double y, double z) const;

    /** Convenience overload that throws if numVariables() != 5. */
    double evaluate(double v, double w, double x, double y, double z) const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::Curve_Impl;

    Curve(IddObjectType type, const Model& model);

    explicit Curve(std::shared_ptr<detail::Curve_Impl> impl);

    friend class detail::Curve_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Curve");
  };

  /** \relates Curve*/
  using OptionalCurve = boost::optional<Curve>;

  /** \relates Curve*/
  using CurveVector = std::vector<Curve>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVE_HPP
