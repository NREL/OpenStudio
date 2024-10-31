/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVEQUADLINEAR_IMPL_HPP
#define MODEL_CURVEQUADLINEAR_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CurveQuadLinear_Impl is a Curve_Impl that is the implementation class for CurveQuadLinear.*/
    class MODEL_API CurveQuadLinear_Impl : public Curve_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CurveQuadLinear_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CurveQuadLinear_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CurveQuadLinear_Impl(const CurveQuadLinear_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CurveQuadLinear_Impl() override = default;

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
     private:
      REGISTER_LOGGER("openstudio.model.CurveQuadLinear");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEQUADLINEAR_IMPL_HPP
