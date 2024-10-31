/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVEQUADRATICLINEAR_IMPL_HPP
#define MODEL_CURVEQUADRATICLINEAR_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CurveQuadraticLinear_Impl is a Curve_Impl that is the implementation class for CurveQuadraticLinear.*/
    class MODEL_API CurveQuadraticLinear_Impl : public Curve_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      CurveQuadraticLinear_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CurveQuadraticLinear_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CurveQuadraticLinear_Impl(const CurveQuadraticLinear_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CurveQuadraticLinear_Impl() override = default;

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

      double coefficient5xTIMESY() const;

      double coefficient6xPOW2TIMESY() const;

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

      bool setCoefficient5xTIMESY(double coefficient5xTIMESY);

      bool setCoefficient6xPOW2TIMESY(double coefficient6xPOW2TIMESY);

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
      REGISTER_LOGGER("openstudio.model.CurveQuadraticLinear");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEQUADRATICLINEAR_IMPL_HPP
