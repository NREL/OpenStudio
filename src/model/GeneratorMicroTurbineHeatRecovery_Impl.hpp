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

#ifndef MODEL_GENERATORMICROTURBINEHEATRECOVERY_IMPL_HPP
#define MODEL_GENERATORMICROTURBINEHEATRECOVERY_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Curve;
  class GeneratorMicroTurbine;

  namespace detail {

    class MODEL_API GeneratorMicroTurbineHeatRecovery_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorMicroTurbineHeatRecovery_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorMicroTurbineHeatRecovery_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorMicroTurbineHeatRecovery_Impl(const GeneratorMicroTurbineHeatRecovery_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorMicroTurbineHeatRecovery_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      // DLM: consider reimplementing any other virtual methods of StraightComponent or HVACComponent

      //@}
      /** @name Getters */
      //@{

      //boost::optional<Connection> heatRecoveryWaterInletNode() const;

      //boost::optional<Connection> heatRecoveryWaterOutletNode() const;

      // TODO: I might need to make this one default to something that makes sense...
      double referenceThermalEfficiencyUsingLowerHeatValue() const;
      bool isReferenceThermalEfficiencyUsingLowerHeatValueDefaulted() const;

      double referenceInletWaterTemperature() const;

      std::string heatRecoveryWaterFlowOperatingMode() const;
      bool isHeatRecoveryWaterFlowOperatingModeDefaulted() const;

      double referenceHeatRecoveryWaterFlowRate() const;

      // BiquadraticCurves, BiVariateTables.
      boost::optional<Curve> heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve() const;

      // BicubicBiquadraticCurves, BiVariateTables.
      boost::optional<Curve> thermalEfficiencyFunctionofTemperatureandElevationCurve() const;

      // QuadraticCubicCurves, UniVariateTables.
      boost::optional<Curve> heatRecoveryRateFunctionofPartLoadRatioCurve() const;

      //QuadraticCurves, UniVariateTables.
      boost::optional<Curve> heatRecoveryRateFunctionofInletWaterTemperatureCurve() const;

      // QuadraticCurves, UniVariateTables.
      boost::optional<Curve> heatRecoveryRateFunctionofWaterFlowRateCurve() const;

      double minimumHeatRecoveryWaterFlowRate() const;
      bool isMinimumHeatRecoveryWaterFlowRateDefaulted() const;

      double maximumHeatRecoveryWaterFlowRate() const;
      bool isMaximumHeatRecoveryWaterFlowRateDefaulted() const;

      boost::optional<double> maximumHeatRecoveryWaterTemperature() const;

      double ratedThermaltoElectricalPowerRatio() const;
      bool isRatedThermaltoElectricalPowerRatioDefaulted() const;

      // Return parent generator
      GeneratorMicroTurbine generatorMicroTurbine() const;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      // TODO: Check argument type. From object lists, some candidates are: Connection.
      //bool setHeatRecoveryWaterInletNode(const Connection& connection);
      //void resetHeatRecoveryWaterInletNode();

      // TODO: Check argument type. From object lists, some candidates are: Connection.
      //bool setHeatRecoveryWaterOutletNode(const Connection& connection);
      //void resetHeatRecoveryWaterOutletNode();

      bool setReferenceThermalEfficiencyUsingLowerHeatValue(double referenceThermalEfficiencyUsingLowerHeatValue);
      void resetReferenceThermalEfficiencyUsingLowerHeatValue();

      bool setReferenceInletWaterTemperature(double referenceInletWaterTemperature);

      bool setHeatRecoveryWaterFlowOperatingMode(const std::string& heatRecoveryWaterFlowOperatingMode);
      void resetHeatRecoveryWaterFlowOperatingMode();

      bool setReferenceHeatRecoveryWaterFlowRate(double referenceHeatRecoveryWaterFlowRate);

      // BiquadraticCurves, BiVariateTables.
      bool setHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve(const Curve& heatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve);
      void resetHeatRecoveryWaterFlowRateFunctionofTemperatureandPowerCurve();

      // BicubicBiquadraticCurves, BiVariateTables.
      bool setThermalEfficiencyFunctionofTemperatureandElevationCurve(const Curve& thermalEfficiencyFunctionofTemperatureandElevationCurve);
      void resetThermalEfficiencyFunctionofTemperatureandElevationCurve();

      // QuadraticCubicCurves, UniVariateTables.
      bool setHeatRecoveryRateFunctionofPartLoadRatioCurve(const Curve& heatRecoveryRateFunctionofPartLoadRatioCurve);
      void resetHeatRecoveryRateFunctionofPartLoadRatioCurve();

      // QuadraticCurves, UniVariateTables.
      bool setHeatRecoveryRateFunctionofInletWaterTemperatureCurve(const Curve& heatRecoveryRateFunctionofInletWaterTemperatureCurve);
      void resetHeatRecoveryRateFunctionofInletWaterTemperatureCurve();

      // QuadraticCurves, UniVariateTables.
      bool setHeatRecoveryRateFunctionofWaterFlowRateCurve(const Curve& heatRecoveryRateFunctionofWaterFlowRateCurve);
      void resetHeatRecoveryRateFunctionofWaterFlowRateCurve();

      bool setMinimumHeatRecoveryWaterFlowRate(double minimumHeatRecoveryWaterFlowRate);
      void resetMinimumHeatRecoveryWaterFlowRate();

      bool setMaximumHeatRecoveryWaterFlowRate(double maximumHeatRecoveryWaterFlowRate);
      void resetMaximumHeatRecoveryWaterFlowRate();

      bool setMaximumHeatRecoveryWaterTemperature(double maximumHeatRecoveryWaterTemperature);
      void resetMaximumHeatRecoveryWaterTemperature();

      bool setRatedThermaltoElectricalPowerRatio(double ratedThermaltoElectricalPowerRatio);
      void resetRatedThermaltoElectricalPowerRatio();

      //@}
      /** @name Other */
      //@{

      ModelObject clone(Model model) const override;

      std::vector<IddObjectType> allowableChildTypes() const override;

      std::vector<ModelObject> children() const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorMicroTurbineHeatRecovery");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORMICROTURBINEHEATRECOVERY_IMPL_HPP
