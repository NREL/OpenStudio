/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVEQUADRATIC_HPP
#define MODEL_CURVEQUADRATIC_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveQuadratic_Impl;

  }  // namespace detail

  /** CurveQuadratic is a Curve that wraps the OpenStudio IDD object 'OS:Curve:Quadratic'. The
 *  functional form is \f$f(x) = c_1 + c_2 \cdot x + c_3 \cdot x^2\f$ */
  class MODEL_API CurveQuadratic : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Initializes \f$c_1 = c_2 = 0.0,\ c_3 = 1.0,\ xmin = 0,\ xmax = 1\f$ */
    explicit CurveQuadratic(const Model& model);

    virtual ~CurveQuadratic() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveQuadratic(const CurveQuadratic& other) = default;
    CurveQuadratic(CurveQuadratic&& other) = default;
    CurveQuadratic& operator=(const CurveQuadratic&) = default;
    CurveQuadratic& operator=(CurveQuadratic&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validInputUnitTypeforXValues();

    static std::vector<std::string> validOutputUnitTypeValues();

    /** @name Getters */
    //@{

    double coefficient1Constant() const;

    double coefficient2x() const;

    double coefficient3xPOW2() const;

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
    using ImplType = detail::CurveQuadratic_Impl;

    explicit CurveQuadratic(std::shared_ptr<detail::CurveQuadratic_Impl> impl);

    friend class detail::CurveQuadratic_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveQuadratic");
  };

  /** \relates CurveQuadratic*/
  using OptionalCurveQuadratic = boost::optional<CurveQuadratic>;

  /** \relates CurveQuadratic*/
  using CurveQuadraticVector = std::vector<CurveQuadratic>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEQUADRATIC_HPP
