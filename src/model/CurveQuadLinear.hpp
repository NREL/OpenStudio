/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVEQUADLINEAR_HPP
#define MODEL_CURVEQUADLINEAR_HPP

#include <model/ModelAPI.hpp>
#include "Curve.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class CurveQuadLinear_Impl;

  }  // namespace detail

  /** CurveQuadLinear is a Curve that wraps the OpenStudio IDD object 'OS:Curve:QuadLinear'.
 *  The functional form is \f$f(w, x, y, z) = c_1 + c_2 \cdot w + c_3 \cdot x + c_4 \cdot y +c_5 \cdot z\f$ */
  class MODEL_API CurveQuadLinear : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CurveQuadLinear(const Model& model);

    virtual ~CurveQuadLinear() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveQuadLinear(const CurveQuadLinear& other) = default;
    CurveQuadLinear(CurveQuadLinear&& other) = default;
    CurveQuadLinear& operator=(const CurveQuadLinear&) = default;
    CurveQuadLinear& operator=(CurveQuadLinear&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> inputUnitTypeforwValues();

    static std::vector<std::string> inputUnitTypeforxValues();

    static std::vector<std::string> inputUnitTypeforyValues();

    static std::vector<std::string> inputUnitTypeforzValues();

    /** @name Getters */
    //@{

    double coefficient1Constant() const;

    double coefficient2w() const;

    double coefficient3x() const;

    double coefficient4y() const;

    double coefficient5z() const;

    double minimumValueofw() const;

    double maximumValueofw() const;

    double minimumValueofx() const;

    double maximumValueofx() const;

    double minimumValueofy() const;

    double maximumValueofy() const;

    double minimumValueofz() const;

    double maximumValueofz() const;

    boost::optional<double> minimumCurveOutput() const;

    boost::optional<double> maximumCurveOutput() const;

    std::string inputUnitTypeforw() const;

    bool isInputUnitTypeforwDefaulted() const;

    std::string inputUnitTypeforx() const;

    bool isInputUnitTypeforxDefaulted() const;

    std::string inputUnitTypefory() const;

    bool isInputUnitTypeforyDefaulted() const;

    std::string inputUnitTypeforz() const;

    bool isInputUnitTypeforzDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setCoefficient1Constant(double coefficient1Constant);

    bool setCoefficient2w(double coefficient2w);

    bool setCoefficient3x(double coefficient3x);

    bool setCoefficient4y(double coefficient4y);

    bool setCoefficient5z(double coefficient5z);

    bool setMinimumValueofw(double minimumValueofw);

    bool setMaximumValueofw(double maximumValueofw);

    bool setMinimumValueofx(double minimumValueofx);

    bool setMaximumValueofx(double maximumValueofx);

    bool setMinimumValueofy(double minimumValueofy);

    bool setMaximumValueofy(double maximumValueofy);

    bool setMinimumValueofz(double minimumValueofz);

    bool setMaximumValueofz(double maximumValueofz);

    bool setMinimumCurveOutput(double minimumCurveOutput);

    void resetMinimumCurveOutput();

    bool setMaximumCurveOutput(double maximumCurveOutput);

    void resetMaximumCurveOutput();

    bool setInputUnitTypeforw(const std::string& inputUnitTypeforw);

    void resetInputUnitTypeforw();

    bool setInputUnitTypeforx(const std::string& inputUnitTypeforx);

    void resetInputUnitTypeforx();

    bool setInputUnitTypefory(const std::string& inputUnitTypefory);

    void resetInputUnitTypefory();

    bool setInputUnitTypeforz(const std::string& inputUnitTypeforz);

    void resetInputUnitTypeforz();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CurveQuadLinear_Impl;

    explicit CurveQuadLinear(std::shared_ptr<detail::CurveQuadLinear_Impl> impl);

    friend class detail::CurveQuadLinear_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveQuadLinear");
  };

  /** \relates CurveQuadLinear*/
  using OptionalCurveQuadLinear = boost::optional<CurveQuadLinear>;

  /** \relates CurveQuadLinear*/
  using CurveQuadLinearVector = std::vector<CurveQuadLinear>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEQUADLINEAR_HPP
