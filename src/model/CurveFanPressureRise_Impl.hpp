/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURVEFANPRESSURERISE_IMPL_HPP
#define MODEL_CURVEFANPRESSURERISE_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CurveFanPressureRise_Impl is a Curve_Impl that is the implementation class for CurveFanPressureRise.*/
    class MODEL_API CurveFanPressureRise_Impl : public Curve_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      CurveFanPressureRise_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CurveFanPressureRise_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CurveFanPressureRise_Impl(const CurveFanPressureRise_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CurveFanPressureRise_Impl() override = default;

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

      double coefficient4C4() const;

      double minimumValueofQfan() const;

      double maximumValueofQfan() const;

      double minimumValueofPsm() const;

      double maximumValueofPsm() const;

      boost::optional<double> minimumCurveOutput() const;

      boost::optional<double> maximumCurveOutput() const;

      //@}
      /** @name Setters */
      //@{

      bool setCoefficient1C1(double coefficient1C1);

      bool setCoefficient2C2(double coefficient2C2);

      bool setCoefficient3C3(double coefficient3C3);

      bool setCoefficient4C4(double coefficient4C4);

      bool setMinimumValueofQfan(double minimumValueofQfan);

      bool setMaximumValueofQfan(double maximumValueofQfan);

      bool setMinimumValueofPsm(double minimumValueofPsm);

      bool setMaximumValueofPsm(double maximumValueofPsm);

      bool setMinimumCurveOutput(boost::optional<double> minimumCurveOutput);

      void resetMinimumCurveOutput();

      bool setMaximumCurveOutput(boost::optional<double> maximumCurveOutput);

      void resetMaximumCurveOutput();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CurveFanPressureRise");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEFANPRESSURERISE_IMPL_HPP
