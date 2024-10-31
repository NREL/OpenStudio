/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVETRIQUADRATIC_HPP
#define MODEL_CURVETRIQUADRATIC_HPP

#include "ModelAPI.hpp"
#include "Curve.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class CurveTriquadratic_Impl;

  }  // namespace detail

  /** CurveTriquadratic is a Curve that wraps the OpenStudio IDD object 'OS:Curve:Triquadratic'. The
 *  functional form is
 *  \f[\displaystyle \begin{array}{ll}
    f(x) = & c_1 + c_2 \cdot x^2 + c_3 \cdot x + c_4 \cdot y^2 + c_5 \cdot y + c_6 \cdot z^2 + c_7 \cdot z + c_8 \cdot x^2 \cdot y^2 + \\
           & c_9 \cdot x \cdot y + c_{10} \cdot x \cdot y^2 + c_{11} \cdot x^2 \cdot y + c_{12} \cdot x^2 \cdot z^2 + c_{13} \cdot x \cdot z + c_{14} \cdot x \cdot z^2 + \\
           & c_{15} \cdot x^2 \cdot z + c_{16} \cdot y^2 \cdot z^2 + c_{17} \cdot y \cdot z + c_{18} \cdot y \cdot z^2 + c_{19} \cdot y^2 \cdot z + \\
           & c_{20} \cdot x^2 \cdot y^2 \cdot z^2 + c_{21} \cdot z^2 \cdot y^2 \cdot z + c_{22} \cdot x^2 \cdot y \cdot z^2 + c_{23} \cdot x \cdot y^2 \cdot z^2 + \\
           & c_{24} \cdot x^2 \cdot y \cdot z + c_{25} \cdot x \cdot y^2 \cdot z + c_{26} \cdot x \cdot y \cdot z^2 + c_{27} \cdot x \cdot y \cdot z
    \end{array}\f] */
  class MODEL_API CurveTriquadratic : public Curve
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit CurveTriquadratic(const Model& model);

    virtual ~CurveTriquadratic() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurveTriquadratic(const CurveTriquadratic& other) = default;
    CurveTriquadratic(CurveTriquadratic&& other) = default;
    CurveTriquadratic& operator=(const CurveTriquadratic&) = default;
    CurveTriquadratic& operator=(CurveTriquadratic&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validInputUnitTypeforXValues();

    static std::vector<std::string> validInputUnitTypeforYValues();

    static std::vector<std::string> validInputUnitTypeforZValues();

    static std::vector<std::string> validOutputUnitTypeValues();

    /** @name Getters */
    //@{

    // TODO: Instead of having optionals, we could just default them to zero...
    boost::optional<double> coefficient1Constant() const;

    boost::optional<double> coefficient2xPOW2() const;

    boost::optional<double> coefficient3x() const;

    boost::optional<double> coefficient4yPOW2() const;

    boost::optional<double> coefficient5y() const;

    boost::optional<double> coefficient6zPOW2() const;

    boost::optional<double> coefficient7z() const;

    boost::optional<double> coefficient8xPOW2TIMESYPOW2() const;

    boost::optional<double> coefficient9xTIMESY() const;

    boost::optional<double> coefficient10xTIMESYPOW2() const;

    boost::optional<double> coefficient11xPOW2TIMESY() const;

    boost::optional<double> coefficient12xPOW2TIMESZPOW2() const;

    boost::optional<double> coefficient13xTIMESZ() const;

    boost::optional<double> coefficient14xTIMESZPOW2() const;

    boost::optional<double> coefficient15xPOW2TIMESZ() const;

    boost::optional<double> coefficient16yPOW2TIMESZPOW2() const;

    boost::optional<double> coefficient17yTIMESZ() const;

    boost::optional<double> coefficient18yTIMESZPOW2() const;

    boost::optional<double> coefficient19yPOW2TIMESZ() const;

    boost::optional<double> coefficient20xPOW2TIMESYPOW2TIMESZPOW2() const;

    boost::optional<double> coefficient21xPOW2TIMESYPOW2TIMESZ() const;

    boost::optional<double> coefficient22xPOW2TIMESYTIMESZPOW2() const;

    boost::optional<double> coefficient23xTIMESYPOW2TIMESZPOW2() const;

    boost::optional<double> coefficient24xPOW2TIMESYTIMESZ() const;

    boost::optional<double> coefficient25xTIMESYPOW2TIMESZ() const;

    boost::optional<double> coefficient26xTIMESYTIMESZPOW2() const;

    boost::optional<double> coefficient27xTIMESYTIMESZ() const;

    boost::optional<double> minimumValueofx() const;

    boost::optional<double> maximumValueofx() const;

    boost::optional<double> minimumValueofy() const;

    boost::optional<double> maximumValueofy() const;

    boost::optional<double> minimumValueofz() const;

    boost::optional<double> maximumValueofz() const;

    boost::optional<double> minimumCurveOutput() const;

    boost::optional<double> maximumCurveOutput() const;

    std::string inputUnitTypeforX() const;

    bool isInputUnitTypeforXDefaulted() const;

    std::string inputUnitTypeforY() const;

    bool isInputUnitTypeforYDefaulted() const;

    std::string inputUnitTypeforZ() const;

    bool isInputUnitTypeforZDefaulted() const;

    std::string outputUnitType() const;

    bool isOutputUnitTypeDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setCoefficient1Constant(double coefficient1Constant);

    void resetCoefficient1Constant();

    bool setCoefficient2xPOW2(double coefficient2xPOW2);

    void resetCoefficient2xPOW2();

    bool setCoefficient3x(double coefficient3x);

    void resetCoefficient3x();

    bool setCoefficient4yPOW2(double coefficient4yPOW2);

    void resetCoefficient4yPOW2();

    bool setCoefficient5y(double coefficient5y);

    void resetCoefficient5y();

    bool setCoefficient6zPOW2(double coefficient6zPOW2);

    void resetCoefficient6zPOW2();

    bool setCoefficient7z(double coefficient7z);

    void resetCoefficient7z();

    bool setCoefficient8xPOW2TIMESYPOW2(double coefficient8xPOW2TIMESYPOW2);

    void resetCoefficient8xPOW2TIMESYPOW2();

    bool setCoefficient9xTIMESY(double coefficient9xTIMESY);

    void resetCoefficient9xTIMESY();

    bool setCoefficient10xTIMESYPOW2(double coefficient10xTIMESYPOW2);

    void resetCoefficient10xTIMESYPOW2();

    bool setCoefficient11xPOW2TIMESY(double coefficient11xPOW2TIMESY);

    void resetCoefficient11xPOW2TIMESY();

    bool setCoefficient12xPOW2TIMESZPOW2(double coefficient12xPOW2TIMESZPOW2);

    void resetCoefficient12xPOW2TIMESZPOW2();

    bool setCoefficient13xTIMESZ(double coefficient13xTIMESZ);

    void resetCoefficient13xTIMESZ();

    bool setCoefficient14xTIMESZPOW2(double coefficient14xTIMESZPOW2);

    void resetCoefficient14xTIMESZPOW2();

    bool setCoefficient15xPOW2TIMESZ(double coefficient15xPOW2TIMESZ);

    void resetCoefficient15xPOW2TIMESZ();

    bool setCoefficient16yPOW2TIMESZPOW2(double coefficient16yPOW2TIMESZPOW2);

    void resetCoefficient16yPOW2TIMESZPOW2();

    bool setCoefficient17yTIMESZ(double coefficient17yTIMESZ);

    void resetCoefficient17yTIMESZ();

    bool setCoefficient18yTIMESZPOW2(double coefficient18yTIMESZPOW2);

    void resetCoefficient18yTIMESZPOW2();

    bool setCoefficient19yPOW2TIMESZ(double coefficient19yPOW2TIMESZ);

    void resetCoefficient19yPOW2TIMESZ();

    bool setCoefficient20xPOW2TIMESYPOW2TIMESZPOW2(double coefficient20xPOW2TIMESYPOW2TIMESZPOW2);

    void resetCoefficient20xPOW2TIMESYPOW2TIMESZPOW2();

    bool setCoefficient21xPOW2TIMESYPOW2TIMESZ(double coefficient21xPOW2TIMESYPOW2TIMESZ);

    void resetCoefficient21xPOW2TIMESYPOW2TIMESZ();

    bool setCoefficient22xPOW2TIMESYTIMESZPOW2(double coefficient22xPOW2TIMESYTIMESZPOW2);

    void resetCoefficient22xPOW2TIMESYTIMESZPOW2();

    bool setCoefficient23xTIMESYPOW2TIMESZPOW2(double coefficient23xTIMESYPOW2TIMESZPOW2);

    void resetCoefficient23xTIMESYPOW2TIMESZPOW2();

    bool setCoefficient24xPOW2TIMESYTIMESZ(double coefficient24xPOW2TIMESYTIMESZ);

    void resetCoefficient24xPOW2TIMESYTIMESZ();

    bool setCoefficient25xTIMESYPOW2TIMESZ(double coefficient25xTIMESYPOW2TIMESZ);

    void resetCoefficient25xTIMESYPOW2TIMESZ();

    bool setCoefficient26xTIMESYTIMESZPOW2(double coefficient26xTIMESYTIMESZPOW2);

    void resetCoefficient26xTIMESYTIMESZPOW2();

    bool setCoefficient27xTIMESYTIMESZ(double coefficient27xTIMESYTIMESZ);

    void resetCoefficient27xTIMESYTIMESZ();

    bool setMinimumValueofx(double minimumValueofx);

    void resetMinimumValueofx();

    bool setMaximumValueofx(double maximumValueofx);

    void resetMaximumValueofx();

    bool setMinimumValueofy(double minimumValueofy);

    void resetMinimumValueofy();

    bool setMaximumValueofy(double maximumValueofy);

    void resetMaximumValueofy();

    bool setMinimumValueofz(double minimumValueofz);

    void resetMinimumValueofz();

    bool setMaximumValueofz(double maximumValueofz);

    void resetMaximumValueofz();

    bool setMinimumCurveOutput(double minimumCurveOutput);

    void resetMinimumCurveOutput();

    bool setMaximumCurveOutput(double maximumCurveOutput);

    void resetMaximumCurveOutput();

    bool setInputUnitTypeforX(const std::string& inputUnitTypeforX);

    void resetInputUnitTypeforX();

    bool setInputUnitTypeforY(const std::string& inputUnitTypeforY);

    void resetInputUnitTypeforY();

    bool setInputUnitTypeforZ(const std::string& inputUnitTypeforZ);

    void resetInputUnitTypeforZ();

    bool setOutputUnitType(const std::string& outputUnitType);

    void resetOutputUnitType();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::CurveTriquadratic_Impl;

    explicit CurveTriquadratic(std::shared_ptr<detail::CurveTriquadratic_Impl> impl);

    friend class detail::CurveTriquadratic_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.CurveTriquadratic");
  };

  /** \relates CurveTriquadratic*/
  using OptionalCurveTriquadratic = boost::optional<CurveTriquadratic>;

  /** \relates CurveTriquadratic*/
  using CurveTriquadraticVector = std::vector<CurveTriquadratic>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVETRIQUADRATIC_HPP
