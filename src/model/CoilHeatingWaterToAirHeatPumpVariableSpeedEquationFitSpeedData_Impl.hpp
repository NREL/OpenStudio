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

#ifndef MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_IMPL_HPP
#define MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    /** CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl is a ParentObject_Impl that is the implementation class for CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.*/
    class MODEL_API CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                          bool keepHandle);

      CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl(
        const CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      //@}
      /** @name Getters */
      //@{

      double referenceUnitGrossRatedHeatingCapacity() const;

      double referenceUnitGrossRatedHeatingCOP() const;

      double referenceUnitRatedAirFlow() const;

      double referenceUnitRatedWaterFlowRate() const;

      Curve heatingCapacityFunctionofTemperatureCurve() const;

      Curve totalHeatingCapacityFunctionofAirFlowFractionCurve() const;

      Curve heatingCapacityFunctionofWaterFlowFractionCurve() const;

      Curve energyInputRatioFunctionofTemperatureCurve() const;

      Curve energyInputRatioFunctionofAirFlowFractionCurve() const;

      Curve energyInputRatioFunctionofWaterFlowFractionCurve() const;

      double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions() const;

      Curve wasteHeatFunctionofTemperatureCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setReferenceUnitGrossRatedHeatingCapacity(double referenceUnitGrossRatedHeatingCapacity);

      bool setReferenceUnitGrossRatedHeatingCOP(double referenceUnitGrossRatedHeatingCOP);

      bool setReferenceUnitRatedAirFlow(double referenceUnitRatedAirFlow);

      bool setReferenceUnitRatedWaterFlowRate(double referenceUnitRatedWaterFlowRate);

      bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

      bool setTotalHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

      bool setHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionofAirFlowFractionCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionofWaterFlowFractionCurve(const Curve& curve);

      bool setReferenceUnitWasteHeatFractionofInputPowerAtRatedConditions(double referenceUnitWasteHeatFractionofInputPowerAtRatedConditions);

      bool setWasteHeatFunctionofTemperatureCurve(const Curve& curve);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Curve> optionalHeatingCapacityFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalTotalHeatingCapacityFunctionofAirFlowFractionCurve() const;
      boost::optional<Curve> optionalHeatingCapacityFunctionofWaterFlowFractionCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionofAirFlowFractionCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionofWaterFlowFractionCurve() const;
      boost::optional<Curve> optionalWasteHeatFunctionofTemperatureCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGWATERTOAIRHEATPUMPVARIABLESPEEDEQUATIONFITSPEEDDATA_IMPL_HPP
