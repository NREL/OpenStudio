/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVEEXPONENTIALDECAY_HPP
#define MODEL_CURVEEXPONENTIALDECAY_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveExponentialDecay_Impl;

  }  // namespace detail

  /** CurveExponentialDecay is a Curve that wraps the OpenStudio IDD object
 *  'OS:Curve:ExponentialDecay'. The functional form is \f$f(x) = c_1 + c_2 \cdot e^{c_3 \cdot x}\f$ */
  class MODEL_API CurveExponentialDecay : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Sets \f$c_1 = 0.0,\ c_2 = 1.0,\ c_3 = -1.0\f$, and bounds x within [0.0,1.0]. */
    explicit CurveExponentialDecay(const Model& model);

    virtual ~CurveExponentialDecay() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveExponentialDecay(const CurveExponentialDecay& other) = default;
    CurveExponentialDecay(CurveExponentialDecay&& other) = default;
    CurveExponentialDecay& operator=(const CurveExponentialDecay&) = default;
    CurveExponentialDecay& operator=(CurveExponentialDecay&&) = default;

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
    using ImplType = detail::CurveExponentialDecay_Impl;

    explicit CurveExponentialDecay(std::shared_ptr<detail::CurveExponentialDecay_Impl> impl);

    friend class detail::CurveExponentialDecay_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveExponentialDecay");
  };

  /** \relates CurveExponentialDecay*/
  using OptionalCurveExponentialDecay = boost::optional<CurveExponentialDecay>;

  /** \relates CurveExponentialDecay*/
  using CurveExponentialDecayVector = std::vector<CurveExponentialDecay>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEEXPONENTIALDECAY_HPP
