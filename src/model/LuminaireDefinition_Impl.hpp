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

#ifndef MODEL_LUMINAIREDEFINITION_IMPL_HPP
#define MODEL_LUMINAIREDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition_Impl.hpp"

namespace openstudio {
namespace model {

  class LuminaireDefinition;

  namespace detail {

    /** LuminaireDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for LuminaireDefinition.*/
    class MODEL_API LuminaireDefinition_Impl : public SpaceLoadDefinition_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      LuminaireDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      LuminaireDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      LuminaireDefinition_Impl(const LuminaireDefinition_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~LuminaireDefinition_Impl() = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      // TODO: Handle Non-Extensible IddField IES File Path.

      double lightingPower() const;
      bool isLightingPowerDefaulted() const;

      double fractionRadiant() const;
      bool isFractionRadiantDefaulted() const;

      double fractionVisible() const;
      bool isFractionVisibleDefaulted() const;

      double returnAirFraction() const;
      bool isReturnAirFractionDefaulted() const;

      bool returnAirFractionCalculatedfromPlenumTemperature() const;
      bool isReturnAirFractionCalculatedfromPlenumTemperatureDefaulted() const;

      double returnAirFractionFunctionofPlenumTemperatureCoefficient1() const;
      bool isReturnAirFractionFunctionofPlenumTemperatureCoefficient1Defaulted() const;

      double returnAirFractionFunctionofPlenumTemperatureCoefficient2() const;
      bool isReturnAirFractionFunctionofPlenumTemperatureCoefficient2Defaulted() const;

      //@}
      /** @name Setters */
      //@{

      // TODO: Handle Non-Extensible IddField IES File Path.

      bool setLightingPower(double lightingPower);
      void resetLightingPower();

      bool setFractionRadiant(double fractionRadiant);
      void resetFractionRadiant();

      bool setFractionVisible(double fractionVisible);
      void resetFractionVisible();

      bool setReturnAirFraction(double returnAirFraction);
      void resetReturnAirFraction();

      bool setReturnAirFractionCalculatedfromPlenumTemperature(bool returnAirFractionCalculatedfromPlenumTemperature);
      void resetReturnAirFractionCalculatedfromPlenumTemperature();

      bool setReturnAirFractionFunctionofPlenumTemperatureCoefficient1(double returnAirFractionFunctionofPlenumTemperatureCoefficient1);
      void resetReturnAirFractionFunctionofPlenumTemperatureCoefficient1();

      bool setReturnAirFractionFunctionofPlenumTemperatureCoefficient2(double returnAirFractionFunctionofPlenumTemperatureCoefficient2);
      void resetReturnAirFractionFunctionofPlenumTemperatureCoefficient2();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.LuminaireDefinition");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LUMINAIREDEFINITION_IMPL_HPP
