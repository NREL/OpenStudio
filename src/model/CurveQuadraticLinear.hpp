/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVEQUADRATICLINEAR_HPP
#define MODEL_CURVEQUADRATICLINEAR_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveQuadraticLinear_Impl;

  }  // namespace detail

  /** CurveQuadraticLinear is a Curve that wraps the OpenStudio IDD object
 *  'OS:Curve:QuadraticLinear'. The functional form is
 *  \f$f(x) = c_1 + c_2 \cdot x + c_3 \cdot x^2 + (c_4 + c_5 \cdot x + c_6 \cdot x^2) \cdot y\f$ */
  class MODEL_API CurveQuadraticLinear : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Initializes \f$c_1 = c_2 = c_3 = c_4 = c_5 = 0.0,\ c_6 = 1.0,\ x\ \text{and}\ y\ \text{within}\ [0.0,1.0]\f$ */
    explicit CurveQuadraticLinear(const Model& model);

    virtual ~CurveQuadraticLinear() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveQuadraticLinear(const CurveQuadraticLinear& other) = default;
    CurveQuadraticLinear(CurveQuadraticLinear&& other) = default;
    CurveQuadraticLinear& operator=(const CurveQuadraticLinear&) = default;
    CurveQuadraticLinear& operator=(CurveQuadraticLinear&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validInputUnitTypeforXValues();

    static std::vector<std::string> validInputUnitTypeforYValues();

    static std::vector<std::string> validOutputUnitTypeValues();

    /** @name Getters */
    //@{

    double coefficient1Constant() const;

    double coefficient2x() const;

    double coefficient3xPOW2() const;

    double coefficient4y() const;

    double coefficient5xTIMESY() const;

    double coefficient6xPOW2TIMESY() const;

    double minimumValueofx() const;

    double maximumValueofx() const;

    double minimumValueofy() const;

    double maximumValueofy() const;

    boost::optional<double> minimumCurveOutput() const;

    boost::optional<double> maximumCurveOutput() const;

    std::string inputUnitTypeforX() const;

    bool isInputUnitTypeforXDefaulted() const;

    std::string inputUnitTypeforY() const;

    bool isInputUnitTypeforYDefaulted() const;

    std::string outputUnitType() const;

    bool isOutputUnitTypeDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setCoefficient1Constant(double coefficient1Constant);

    bool setCoefficient2x(double coefficient2x);

    bool setCoefficient3xPOW2(double coefficient3xPOW2);

    bool setCoefficient4y(double coefficient4y);

    bool setCoefficient5xTIMESY(double coefficient5xTIMESY);

    bool setCoefficient6xPOW2TIMESY(double coefficient6xPOW2TIMESY);

    bool setMinimumValueofx(double minimumValueofx);

    bool setMaximumValueofx(double maximumValueofx);

    bool setMinimumValueofy(double minimumValueofy);

    bool setMaximumValueofy(double maximumValueofy);

    bool setMinimumCurveOutput(double minimumCurveOutput);

    void resetMinimumCurveOutput();

    bool setMaximumCurveOutput(double maximumCurveOutput);

    void resetMaximumCurveOutput();

    bool setInputUnitTypeforX(const std::string& inputUnitTypeforX);

    void resetInputUnitTypeforX();

    bool setInputUnitTypeforY(const std::string& inputUnitTypeforY);

    void resetInputUnitTypeforY();

    bool setOutputUnitType(const std::string& outputUnitType);

    void resetOutputUnitType();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CurveQuadraticLinear_Impl;

    explicit CurveQuadraticLinear(std::shared_ptr<detail::CurveQuadraticLinear_Impl> impl);

    friend class detail::CurveQuadraticLinear_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveQuadraticLinear");
  };

  /** \relates CurveQuadraticLinear*/
  using OptionalCurveQuadraticLinear = boost::optional<CurveQuadraticLinear>;

  /** \relates CurveQuadraticLinear*/
  using CurveQuadraticLinearVector = std::vector<CurveQuadraticLinear>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEQUADRATICLINEAR_HPP
