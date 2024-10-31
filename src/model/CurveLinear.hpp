/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVELINEAR_HPP
#define MODEL_CURVELINEAR_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveLinear_Impl;

  }  // namespace detail

  /** CurveLinear is a Curve that wraps the OpenStudio IDD object 'OS:Curve:Linear'. The
 *  functional form is \f$f(x) = c_1 + c_2 \cdot x\f$. */
  class MODEL_API CurveLinear : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Sets \f$c_1 = 0,\ c_2 = 1.0,\ x\ \text{within}\ [0.0,1.0]\f$ */
    explicit CurveLinear(const Model& model);

    virtual ~CurveLinear() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveLinear(const CurveLinear& other) = default;
    CurveLinear(CurveLinear&& other) = default;
    CurveLinear& operator=(const CurveLinear&) = default;
    CurveLinear& operator=(CurveLinear&&) = default;

    // This is the default Part Load Fraction Correlation curve for Heat Pump coils added in E+ 23.2.0
    static CurveLinear defaultHeatPumpCoilPLFCorrelationCurve(const Model& model, double maximumCyclingRatePerHour = 2.5,
                                                              double heatPumpTimeConstantSeconds = 60.0);

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validInputUnitTypeforXValues();

    static std::vector<std::string> validOutputUnitTypeValues();

    /** @name Getters */
    //@{

    double coefficient1Constant() const;

    double coefficient2x() const;

    double minimumValueofx() const;

    double maximumValueofx() const;

    boost::optional<double> minimumCurveOutput() const;

    boost::optional<double> maximumCurveOutput() const;

    std::string inputUnitTypeforX() const;

    bool isInputUnitTypeforXDefaulted() const;

    std::string outputUnitType() const;

    bool isOutputUnitTypeDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setCoefficient1Constant(double coefficient1Constant);

    bool setCoefficient2x(double coefficient2x);

    bool setMinimumValueofx(double minimumValueofx);

    bool setMaximumValueofx(double maximumValueofx);

    bool setMinimumCurveOutput(double minimumCurveOutput);

    void resetMinimumCurveOutput();

    bool setMaximumCurveOutput(double maximumCurveOutput);

    void resetMaximumCurveOutput();

    bool setInputUnitTypeforX(const std::string& inputUnitTypeforX);

    void resetInputUnitTypeforX();

    bool setOutputUnitType(const std::string& outputUnitType);

    void resetOutputUnitType();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CurveLinear_Impl;

    explicit CurveLinear(std::shared_ptr<detail::CurveLinear_Impl> impl);

    friend class detail::CurveLinear_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveLinear");
  };

  /** \relates CurveLinear*/
  using OptionalCurveLinear = boost::optional<CurveLinear>;

  /** \relates CurveLinear*/
  using CurveLinearVector = std::vector<CurveLinear>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVELINEAR_HPP
