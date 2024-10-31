/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVESIGMOID_HPP
#define MODEL_CURVESIGMOID_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveSigmoid_Impl;

  }  // namespace detail

  /** CurveSigmoid is a Curve that wraps the OpenStudio IDD object 'OS:Curve:Sigmoid'. The
 *  functional form is \f$\displaystyle f(x) = c_1 + \frac{c_2}{\left(1 + e^\left[\frac{c_3 - x}{c_4}\right]\right)^{c_5}}\f$ */
  class MODEL_API CurveSigmoid : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Initializes \f$c_1 = 0.0,\ c_2 = c_3 = c_4 = c_5 = 1.0,\ xmin = 0.0,\ xmax = 1.0\f$ */
    explicit CurveSigmoid(const Model& model);

    virtual ~CurveSigmoid() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveSigmoid(const CurveSigmoid& other) = default;
    CurveSigmoid(CurveSigmoid&& other) = default;
    CurveSigmoid& operator=(const CurveSigmoid&) = default;
    CurveSigmoid& operator=(CurveSigmoid&&) = default;

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

    double coefficient5C5() const;

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

    bool setCoefficient5C5(double coefficient5C5);

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
    using ImplType = detail::CurveSigmoid_Impl;

    explicit CurveSigmoid(std::shared_ptr<detail::CurveSigmoid_Impl> impl);

    friend class detail::CurveSigmoid_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveSigmoid");
  };

  /** \relates CurveSigmoid*/
  using OptionalCurveSigmoid = boost::optional<CurveSigmoid>;

  /** \relates CurveSigmoid*/
  using CurveSigmoidVector = std::vector<CurveSigmoid>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVESIGMOID_HPP
