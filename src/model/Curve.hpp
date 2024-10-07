/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~Curve() override = default;
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
