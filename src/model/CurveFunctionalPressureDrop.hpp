/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~CurveFunctionalPressureDrop() override = default;
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
