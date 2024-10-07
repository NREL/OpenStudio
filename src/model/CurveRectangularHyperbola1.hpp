/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVERECTANGULARHYPERBOLA1_HPP
#define MODEL_CURVERECTANGULARHYPERBOLA1_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveRectangularHyperbola1_Impl;

  }  // namespace detail

  /** CurveRectangularHyperbola1 is a Curve that wraps the OpenStudio IDD object
 *  'OS:Curve:RectangularHyperbola1'. The functional form is \f$\displaystyle f(x) = \frac{c_1 \cdot x}{c_2 + x} + c_3\f$ */
  class MODEL_API CurveRectangularHyperbola1 : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Initializes \f$c_1 = c_2 = 1.0,\ c_3 = 0.0,\ xmin = 0.0,\ xmax = 1.0\f$ */
    explicit CurveRectangularHyperbola1(const Model& model);

    virtual ~CurveRectangularHyperbola1() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveRectangularHyperbola1(const CurveRectangularHyperbola1& other) = default;
    CurveRectangularHyperbola1(CurveRectangularHyperbola1&& other) = default;
    CurveRectangularHyperbola1& operator=(const CurveRectangularHyperbola1&) = default;
    CurveRectangularHyperbola1& operator=(CurveRectangularHyperbola1&&) = default;

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
    using ImplType = detail::CurveRectangularHyperbola1_Impl;

    explicit CurveRectangularHyperbola1(std::shared_ptr<detail::CurveRectangularHyperbola1_Impl> impl);

    friend class detail::CurveRectangularHyperbola1_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveRectangularHyperbola1");
  };

  /** \relates CurveRectangularHyperbola1*/
  using OptionalCurveRectangularHyperbola1 = boost::optional<CurveRectangularHyperbola1>;

  /** \relates CurveRectangularHyperbola1*/
  using CurveRectangularHyperbola1Vector = std::vector<CurveRectangularHyperbola1>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVERECTANGULARHYPERBOLA1_HPP
