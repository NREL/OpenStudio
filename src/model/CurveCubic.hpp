/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVECUBIC_HPP
#define MODEL_CURVECUBIC_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveCubic_Impl;

  }  // namespace detail

  /** CurveCubic is a Curve that wraps the OpenStudio IDD object 'OS:Curve:Cubic'. The
 *  functional form is \f$f(x) = c_1 + c_2 \cdot x + c_3 \cdot x^2 + c_4 \cdot x^3\f$ */
  class MODEL_API CurveCubic : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Sets \f$c_1 = c_2 = c_3 = 0.0,\ c_4 = 1.0\f$, and sets the range for x at [0.0,1.0]. */
    explicit CurveCubic(const Model& model);

    virtual ~CurveCubic() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveCubic(const CurveCubic& other) = default;
    CurveCubic(CurveCubic&& other) = default;
    CurveCubic& operator=(const CurveCubic&) = default;
    CurveCubic& operator=(CurveCubic&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validInputUnitTypeforXValues();

    static std::vector<std::string> validOutputUnitTypeValues();

    /** @name Getters */
    //@{

    double coefficient1Constant() const;

    double coefficient2x() const;

    double coefficient3xPOW2() const;

    double coefficient4xPOW3() const;

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

    bool setCoefficient3xPOW2(double coefficient3xPOW2);

    bool setCoefficient4xPOW3(double coefficient4xPOW3);

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
    using ImplType = detail::CurveCubic_Impl;

    explicit CurveCubic(std::shared_ptr<detail::CurveCubic_Impl> impl);

    friend class detail::CurveCubic_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveCubic");
  };

  /** \relates CurveCubic*/
  using OptionalCurveCubic = boost::optional<CurveCubic>;

  /** \relates CurveCubic*/
  using CurveCubicVector = std::vector<CurveCubic>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVECUBIC_HPP
