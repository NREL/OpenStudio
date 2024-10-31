/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVEBICUBIC_IMPL_HPP
#define MODEL_CURVEBICUBIC_IMPL_HPP

#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CurveBicubic_Impl is a Curve_Impl that is the implementation class for CurveBicubic.*/
    class MODEL_API CurveBicubic_Impl : public Curve_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      CurveBicubic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CurveBicubic_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CurveBicubic_Impl(const CurveBicubic_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CurveBicubic_Impl() override = default;

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

      bool setMinimumCurveOutput(boost::optional<double> minimumCurveOutput);

      void resetMinimumCurveOutput();

      bool setMaximumCurveOutput(boost::optional<double> maximumCurveOutput);

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
     private:
      REGISTER_LOGGER("openstudio.model.CurveBicubic");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEBICUBIC_IMPL_HPP
