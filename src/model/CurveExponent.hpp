/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVEEXPONENT_HPP
#define MODEL_CURVEEXPONENT_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveExponent_Impl;

  }  // namespace detail

  /** CurveExponent is a Curve that wraps the OpenStudio IDD object 'OS:Curve:Exponent'. The
 *  functional form is \f$f(x) = c_1 + c_2 \cdot x^{c_3}\f$ */
  class MODEL_API CurveExponent : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Initializes \f$c_1 = 0.0,\ c_2 = c_3 = 1.0\f$. Sets the range for x to [0.0,1.0]. */
    explicit CurveExponent(const Model& model);

    virtual ~CurveExponent() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveExponent(const CurveExponent& other) = default;
    CurveExponent(CurveExponent&& other) = default;
    CurveExponent& operator=(const CurveExponent&) = default;
    CurveExponent& operator=(CurveExponent&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validInputUnitTypeforXValues();

    static std::vector<std::string> validOutputUnitTypeValues();

    /** @name Getters */
    //@{

    double coefficient1Constant() const;

    double coefficient2Constant() const;

    double coefficient3Constant() const;

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

    bool setCoefficient2Constant(double coefficient2Constant);

    bool setCoefficient3Constant(double coefficient3Constant);

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
    using ImplType = detail::CurveExponent_Impl;

    explicit CurveExponent(std::shared_ptr<detail::CurveExponent_Impl> impl);

    friend class detail::CurveExponent_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveExponent");
  };

  /** \relates CurveExponent*/
  using OptionalCurveExponent = boost::optional<CurveExponent>;

  /** \relates CurveExponent*/
  using CurveExponentVector = std::vector<CurveExponent>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEEXPONENT_HPP
