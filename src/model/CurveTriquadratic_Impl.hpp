/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVETRIQUADRATIC_IMPL_HPP
#define MODEL_CURVETRIQUADRATIC_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CurveTriquadratic_Impl is a Curve_Impl that is the implementation class for CurveTriquadratic.*/
    class MODEL_API CurveTriquadratic_Impl : public Curve_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      CurveTriquadratic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CurveTriquadratic_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CurveTriquadratic_Impl(const CurveTriquadratic_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CurveTriquadratic_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual int numVariables() const override;

      virtual double evaluate(const std::vector<double>& independentVariables) const override;

      //@}
      /** @name Getters */
      //@{

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

      bool setCoefficient1Constant(boost::optional<double> coefficient1Constant);

      void resetCoefficient1Constant();

      bool setCoefficient2xPOW2(boost::optional<double> coefficient2xPOW2);

      void resetCoefficient2xPOW2();

      bool setCoefficient3x(boost::optional<double> coefficient3x);

      void resetCoefficient3x();

      bool setCoefficient4yPOW2(boost::optional<double> coefficient4yPOW2);

      void resetCoefficient4yPOW2();

      bool setCoefficient5y(boost::optional<double> coefficient5y);

      void resetCoefficient5y();

      bool setCoefficient6zPOW2(boost::optional<double> coefficient6zPOW2);

      void resetCoefficient6zPOW2();

      bool setCoefficient7z(boost::optional<double> coefficient7z);

      void resetCoefficient7z();

      bool setCoefficient8xPOW2TIMESYPOW2(boost::optional<double> coefficient8xPOW2TIMESYPOW2);

      void resetCoefficient8xPOW2TIMESYPOW2();

      bool setCoefficient9xTIMESY(boost::optional<double> coefficient9xTIMESY);

      void resetCoefficient9xTIMESY();

      bool setCoefficient10xTIMESYPOW2(boost::optional<double> coefficient10xTIMESYPOW2);

      void resetCoefficient10xTIMESYPOW2();

      bool setCoefficient11xPOW2TIMESY(boost::optional<double> coefficient11xPOW2TIMESY);

      void resetCoefficient11xPOW2TIMESY();

      bool setCoefficient12xPOW2TIMESZPOW2(boost::optional<double> coefficient12xPOW2TIMESZPOW2);

      void resetCoefficient12xPOW2TIMESZPOW2();

      bool setCoefficient13xTIMESZ(boost::optional<double> coefficient13xTIMESZ);

      void resetCoefficient13xTIMESZ();

      bool setCoefficient14xTIMESZPOW2(boost::optional<double> coefficient14xTIMESZPOW2);

      void resetCoefficient14xTIMESZPOW2();

      bool setCoefficient15xPOW2TIMESZ(boost::optional<double> coefficient15xPOW2TIMESZ);

      void resetCoefficient15xPOW2TIMESZ();

      bool setCoefficient16yPOW2TIMESZPOW2(boost::optional<double> coefficient16yPOW2TIMESZPOW2);

      void resetCoefficient16yPOW2TIMESZPOW2();

      bool setCoefficient17yTIMESZ(boost::optional<double> coefficient17yTIMESZ);

      void resetCoefficient17yTIMESZ();

      bool setCoefficient18yTIMESZPOW2(boost::optional<double> coefficient18yTIMESZPOW2);

      void resetCoefficient18yTIMESZPOW2();

      bool setCoefficient19yPOW2TIMESZ(boost::optional<double> coefficient19yPOW2TIMESZ);

      void resetCoefficient19yPOW2TIMESZ();

      bool setCoefficient20xPOW2TIMESYPOW2TIMESZPOW2(boost::optional<double> coefficient20xPOW2TIMESYPOW2TIMESZPOW2);

      void resetCoefficient20xPOW2TIMESYPOW2TIMESZPOW2();

      bool setCoefficient21xPOW2TIMESYPOW2TIMESZ(boost::optional<double> coefficient21xPOW2TIMESYPOW2TIMESZ);

      void resetCoefficient21xPOW2TIMESYPOW2TIMESZ();

      bool setCoefficient22xPOW2TIMESYTIMESZPOW2(boost::optional<double> coefficient22xPOW2TIMESYTIMESZPOW2);

      void resetCoefficient22xPOW2TIMESYTIMESZPOW2();

      bool setCoefficient23xTIMESYPOW2TIMESZPOW2(boost::optional<double> coefficient23xTIMESYPOW2TIMESZPOW2);

      void resetCoefficient23xTIMESYPOW2TIMESZPOW2();

      bool setCoefficient24xPOW2TIMESYTIMESZ(boost::optional<double> coefficient24xPOW2TIMESYTIMESZ);

      void resetCoefficient24xPOW2TIMESYTIMESZ();

      bool setCoefficient25xTIMESYPOW2TIMESZ(boost::optional<double> coefficient25xTIMESYPOW2TIMESZ);

      void resetCoefficient25xTIMESYPOW2TIMESZ();

      bool setCoefficient26xTIMESYTIMESZPOW2(boost::optional<double> coefficient26xTIMESYTIMESZPOW2);

      void resetCoefficient26xTIMESYTIMESZPOW2();

      bool setCoefficient27xTIMESYTIMESZ(boost::optional<double> coefficient27xTIMESYTIMESZ);

      void resetCoefficient27xTIMESYTIMESZ();

      bool setMinimumValueofx(boost::optional<double> minimumValueofx);

      void resetMinimumValueofx();

      bool setMaximumValueofx(boost::optional<double> maximumValueofx);

      void resetMaximumValueofx();

      bool setMinimumValueofy(boost::optional<double> minimumValueofy);

      void resetMinimumValueofy();

      bool setMaximumValueofy(boost::optional<double> maximumValueofy);

      void resetMaximumValueofy();

      bool setMinimumValueofz(boost::optional<double> minimumValueofz);

      void resetMinimumValueofz();

      bool setMaximumValueofz(boost::optional<double> maximumValueofz);

      void resetMaximumValueofz();

      bool setMinimumCurveOutput(boost::optional<double> minimumCurveOutput);

      void resetMinimumCurveOutput();

      bool setMaximumCurveOutput(boost::optional<double> maximumCurveOutput);

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
     private:
      REGISTER_LOGGER("openstudio.model.CurveTriquadratic");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVETRIQUADRATIC_IMPL_HPP
