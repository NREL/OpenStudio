/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~CurveFanPressureRise() override = default;
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
