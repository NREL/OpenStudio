/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_CURVEBIQUADRATIC_IMPL_HPP
#define MODEL_CURVEBIQUADRATIC_IMPL_HPP

#include "ModelAPI.hpp"
#include "Curve_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CurveBiquadratic_Impl is a Curve_Impl that is the implementation class for CurveBiquadratic.*/
    class MODEL_API CurveBiquadratic_Impl : public Curve_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      CurveBiquadratic_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CurveBiquadratic_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CurveBiquadratic_Impl(const CurveBiquadratic_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CurveBiquadratic_Impl() = default;

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
      REGISTER_LOGGER("openstudio.model.CurveBiquadratic");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURVEBIQUADRATIC_IMPL_HPP
