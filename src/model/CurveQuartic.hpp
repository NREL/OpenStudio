/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVEQUARTIC_HPP
#define MODEL_CURVEQUARTIC_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveQuartic_Impl;

  }  // namespace detail

  /** CurveQuartic is a Curve that wraps the OpenStudio IDD object 'OS:Curve:Quartic'. The
 *  functional form is \f$f(x) = c_1 + c_2 \cdot x + c_3 \cdot x^2 + c_4 \cdot x^3 + c_5 \cdot x^4\f$ */
  class MODEL_API CurveQuartic : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Initializes \f$c_1 = c_2 = c_3 = c_4 = 0.0,\ c_5 = 1.0,\ x\ \text{in}\ [0.0,1.0]\f$ */
    explicit CurveQuartic(const Model& model);

    virtual ~CurveQuartic() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveQuartic(const CurveQuartic& other) = default;
    CurveQuartic(CurveQuartic&& other) = default;
    CurveQuartic& operator=(const CurveQuartic&) = default;
    CurveQuartic& operator=(CurveQuartic&&) = default;

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

    double coefficient5xPOW4() const;

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

    bool setCoefficient5xPOW4(double coefficient5xPOW4);

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
    using ImplType = detail::CurveQuartic_Impl;

    explicit CurveQuartic(std::shared_ptr<detail::CurveQuartic_Impl> impl);

    friend class detail::CurveQuartic_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveQuartic");
  };

  /** \relates CurveQuartic*/
  using OptionalCurveQuartic = boost::optional<CurveQuartic>;

  /** \relates CurveQuartic*/
  using CurveQuarticVector = std::vector<CurveQuartic>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEQUARTIC_HPP
