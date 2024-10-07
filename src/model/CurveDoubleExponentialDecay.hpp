/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVEDOUBLEEXPONENTIALDECAY_HPP
#define MODEL_CURVEDOUBLEEXPONENTIALDECAY_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveDoubleExponentialDecay_Impl;

  }  // namespace detail

  /** CurveDoubleExponentialDecay is a Curve that wraps the OpenStudio IDD object
 *  'OS:Curve:DoubleExponentialDecay'. The functional form is \f$f(x) = ?\f$ */
  class MODEL_API CurveDoubleExponentialDecay : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Sets all coefficients to 0.0 and sets the range for x to [0.0,1.0]. */
    explicit CurveDoubleExponentialDecay(const Model& model);

    virtual ~CurveDoubleExponentialDecay() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveDoubleExponentialDecay(const CurveDoubleExponentialDecay& other) = default;
    CurveDoubleExponentialDecay(CurveDoubleExponentialDecay&& other) = default;
    CurveDoubleExponentialDecay& operator=(const CurveDoubleExponentialDecay&) = default;
    CurveDoubleExponentialDecay& operator=(CurveDoubleExponentialDecay&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validInputUnitTypeforxValues();

    static std::vector<std::string> validOutputUnitTypeValues();

    /** @name Getters */
    //@{

    double coefficient1C1() const;

    double coefficient2C2() const;

    double coefficient3C3() const;

    double coefficient4C4() const;
    OS_DEPRECATED(3, 1, 0) double coefficient3C4() const;  // TODO: ill-named, should be coefficient4C4

    double coefficient5C5() const;
    OS_DEPRECATED(3, 1, 0) double coefficient3C5() const;  // TODO: ill-named, should be coefficient5C5

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

    bool setCoefficient4C4(double coefficient4C4);
    OS_DEPRECATED(3, 1, 0) bool setCoefficient3C4(double coefficient4C4);

    bool setCoefficient5C5(double coefficient5C5);
    OS_DEPRECATED(3, 1, 0) bool setCoefficient3C5(double coefficient5C5);

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
    using ImplType = detail::CurveDoubleExponentialDecay_Impl;

    explicit CurveDoubleExponentialDecay(std::shared_ptr<detail::CurveDoubleExponentialDecay_Impl> impl);

    friend class detail::CurveDoubleExponentialDecay_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveDoubleExponentialDecay");
  };

  /** \relates CurveDoubleExponentialDecay*/
  using OptionalCurveDoubleExponentialDecay = boost::optional<CurveDoubleExponentialDecay>;

  /** \relates CurveDoubleExponentialDecay*/
  using CurveDoubleExponentialDecayVector = std::vector<CurveDoubleExponentialDecay>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEDOUBLEEXPONENTIALDECAY_HPP
