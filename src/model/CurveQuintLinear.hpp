/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVEQUINTLINEAR_HPP
#define MODEL_CURVEQUINTLINEAR_HPP

#include <model/ModelAPI.hpp>
#include "Curve.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class CurveQuintLinear_Impl;

  }  // namespace detail

  /** CurveQuintLinear is a Curve that wraps the OpenStudio IDD object 'OS:Curve:QuintLinear'.
 *  The functional form is \f$f(v, w, x, y, z) = c_1 + c_2 \cdot v + c_3 \cdot w + c_4 \cdot x + c_5 \cdot y + c_6 \cdot z\f$ */
  class MODEL_API CurveQuintLinear : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CurveQuintLinear(const Model& model);

    virtual ~CurveQuintLinear() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveQuintLinear(const CurveQuintLinear& other) = default;
    CurveQuintLinear(CurveQuintLinear&& other) = default;
    CurveQuintLinear& operator=(const CurveQuintLinear&) = default;
    CurveQuintLinear& operator=(CurveQuintLinear&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> inputUnitTypeforvValues();

    static std::vector<std::string> inputUnitTypeforwValues();

    static std::vector<std::string> inputUnitTypeforxValues();

    static std::vector<std::string> inputUnitTypeforyValues();

    static std::vector<std::string> inputUnitTypeforzValues();

    /** @name Getters */
    //@{

    double coefficient1Constant() const;

    double coefficient2v() const;

    double coefficient3w() const;

    double coefficient4x() const;

    double coefficient5y() const;

    double coefficient6z() const;

    double minimumValueofv() const;

    double maximumValueofv() const;

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

    std::string inputUnitTypeforv() const;

    bool isInputUnitTypeforvDefaulted() const;

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

    bool setCoefficient2v(double coefficient2v);

    bool setCoefficient3w(double coefficient3w);

    bool setCoefficient4x(double coefficient4x);

    bool setCoefficient5y(double coefficient5y);

    bool setCoefficient6z(double coefficient6z);

    bool setMinimumValueofv(double minimumValueofv);

    bool setMaximumValueofv(double maximumValueofv);

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

    bool setInputUnitTypeforv(const std::string& inputUnitTypeforv);

    void resetInputUnitTypeforv();

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
    using ImplType = detail::CurveQuintLinear_Impl;

    explicit CurveQuintLinear(std::shared_ptr<detail::CurveQuintLinear_Impl> impl);

    friend class detail::CurveQuintLinear_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveQuintLinear");
  };

  /** \relates CurveQuintLinear*/
  using OptionalCurveQuintLinear = boost::optional<CurveQuintLinear>;

  /** \relates CurveQuintLinear*/
  using CurveQuintLinearVector = std::vector<CurveQuintLinear>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEQUINTLINEAR_HPP
