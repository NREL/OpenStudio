/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVEEXPONENTIALSKEWNORMAL_HPP
#define MODEL_CURVEEXPONENTIALSKEWNORMAL_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveExponentialSkewNormal_Impl;

  }  // namespace detail

  /** CurveExponentialSkewNormal is a Curve that wraps the OpenStudio IDD object
 *  'OS:Curve:ExponentialSkewNormal'. The functional form is \f[\displaystyle f(x) = \frac{{e^{ - 0.5 \cdot {z_1}^2}} \cdot \left( {1 + \frac{z_2}{\left| {z_2} \right|} \cdot erf\left( {\frac{\left| {z_2} \right|}{\sqrt 2 }} \right)} \right)}{{e^{ - 0.5 \cdot {z_3}^2}} \cdot \left( {1 + \frac{z_3}{\left| {z_3} \right|} \cdot erf\left( \frac{\left| {z_3} \right|}{\sqrt 2 } \right)} \right)},\ \text{where}\ {z_1} = \frac{x - {c_1}}{c_2}, {z_2} = \frac{{e^{{c_3} \cdot x}} \cdot {c_4} \cdot x - {c_1}}{c_2},\ \text{and}\ {z_3} = \frac{-{c_1}}{c_2}\f] */
  class MODEL_API CurveExponentialSkewNormal : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Sets \f$c_1 = c_2 = c_4 = 1.0,\ c_3 = -1.0\f$, x within [-1.0,1.0]. */
    explicit CurveExponentialSkewNormal(const Model& model);

    virtual ~CurveExponentialSkewNormal() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveExponentialSkewNormal(const CurveExponentialSkewNormal& other) = default;
    CurveExponentialSkewNormal(CurveExponentialSkewNormal&& other) = default;
    CurveExponentialSkewNormal& operator=(const CurveExponentialSkewNormal&) = default;
    CurveExponentialSkewNormal& operator=(CurveExponentialSkewNormal&&) = default;

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
    using ImplType = detail::CurveExponentialSkewNormal_Impl;

    explicit CurveExponentialSkewNormal(std::shared_ptr<detail::CurveExponentialSkewNormal_Impl> impl);

    friend class detail::CurveExponentialSkewNormal_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveExponentialSkewNormal");
  };

  /** \relates CurveExponentialSkewNormal*/
  using OptionalCurveExponentialSkewNormal = boost::optional<CurveExponentialSkewNormal>;

  /** \relates CurveExponentialSkewNormal*/
  using CurveExponentialSkewNormalVector = std::vector<CurveExponentialSkewNormal>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEEXPONENTIALSKEWNORMAL_HPP
