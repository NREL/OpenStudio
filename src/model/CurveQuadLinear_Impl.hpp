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

      virtual ~CurveQuadLinear_Impl() = default;

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
