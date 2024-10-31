/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVEBICUBIC_HPP
#define MODEL_CURVEBICUBIC_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveBicubic_Impl;

  }  // namespace detail

  /** CurveBicubic is a Curve that wraps the OpenStudio IDD object 'OS:Curve:Bicubic'. The
 *  functional form is \f$f(x) = c_1 + c_2 \cdot x + c_3 \cdot x^2 + c_4 \cdot y + c_5 \cdot y^2 + c_6 \cdot x \cdot y + c_7 \cdot x^3 + c_8 \cdot y^3 + c_9 \cdot x^2 \cdot y + c_{10} \cdot x \cdot y^2\f$ */
  class MODEL_API CurveBicubic : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Initializes all coefficients to 0.0, and bounds x and y within [0.0,1.0]. */
    explicit CurveBicubic(const Model& model);

    virtual ~CurveBicubic() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveBicubic(const CurveBicubic& other) = default;
    CurveBicubic(CurveBicubic&& other) = default;
    CurveBicubic& operator=(const CurveBicubic&) = default;
    CurveBicubic& operator=(CurveBicubic&&) = default;

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

    double coefficient5yPOW2() const;

    double coefficient6xTIMESY() const;

    double coefficient7xPOW3() const;

    double coefficient8yPOW3() const;

    double coefficient9xPOW2TIMESY() const;

    double coefficient10xTIMESYPOW2() const;

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

    bool setCoefficient5yPOW2(double coefficient5yPOW2);

    bool setCoefficient6xTIMESY(double coefficient6xTIMESY);

    bool setCoefficient7xPOW3(double coefficient7xPOW3);

    bool setCoefficient8yPOW3(double coefficient8yPOW3);

    bool setCoefficient9xPOW2TIMESY(double coefficient9xPOW2TIMESY);

    bool setCoefficient10xTIMESYPOW2(double coefficient10xTIMESYPOW2);

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
    using ImplType = detail::CurveBicubic_Impl;

    explicit CurveBicubic(std::shared_ptr<detail::CurveBicubic_Impl> impl);

    friend class detail::CurveBicubic_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveBicubic");
  };

  /** \relates CurveBicubic*/
  using OptionalCurveBicubic = boost::optional<CurveBicubic>;

  /** \relates CurveBicubic*/
  using CurveBicubicVector = std::vector<CurveBicubic>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEBICUBIC_HPP
