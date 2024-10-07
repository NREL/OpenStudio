/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVERECTANGULARHYPERBOLA2_IMPL_HPP
#define MODEL_CURVERECTANGULARHYPERBOLA2_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CurveRectangularHyperbola2_Impl is a Curve_Impl that is the implementation class for CurveRectangularHyperbola2.*/
    class MODEL_API CurveRectangularHyperbola2_Impl : public Curve_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      CurveRectangularHyperbola2_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CurveRectangularHyperbola2_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CurveRectangularHyperbola2_Impl(const CurveRectangularHyperbola2_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CurveRectangularHyperbola2_Impl() override = default;

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

      bool setMinimumCurveOutput(boost::optional<double> minimumCurveOutput);

      void resetMinimumCurveOutput();

      bool setMaximumCurveOutput(boost::optional<double> maximumCurveOutput);

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
     private:
      REGISTER_LOGGER("openstudio.model.CurveRectangularHyperbola2");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVERECTANGULARHYPERBOLA2_IMPL_HPP
