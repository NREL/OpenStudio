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

#ifndef MODEL_PUMPVARIABLESPEED_IMPL_HPP
#define MODEL_PUMPVARIABLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;

  namespace detail {

    /** PumpVariableSpeed_Impl is a StraightComponent_Impl that is the implementation class for
   *  PumpVariableSpeed.*/
    class MODEL_API PumpVariableSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PumpVariableSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PumpVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PumpVariableSpeed_Impl(const PumpVariableSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PumpVariableSpeed_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> ratedFlowRate() const;

      bool isRatedFlowRateDefaulted() const;

      bool isRatedFlowRateAutosized() const;

      double ratedPumpHead() const;

      bool isRatedPumpHeadDefaulted() const;

      boost::optional<double> ratedPowerConsumption() const;

      bool isRatedPowerConsumptionDefaulted() const;

      bool isRatedPowerConsumptionAutosized() const;

      double motorEfficiency() const;

      bool isMotorEfficiencyDefaulted() const;

      double fractionofMotorInefficienciestoFluidStream() const;

      bool isFractionofMotorInefficienciestoFluidStreamDefaulted() const;

      double coefficient1ofthePartLoadPerformanceCurve() const;

      bool isCoefficient1ofthePartLoadPerformanceCurveDefaulted() const;

      double coefficient2ofthePartLoadPerformanceCurve() const;

      bool isCoefficient2ofthePartLoadPerformanceCurveDefaulted() const;

      double coefficient3ofthePartLoadPerformanceCurve() const;

      bool isCoefficient3ofthePartLoadPerformanceCurveDefaulted() const;

      double coefficient4ofthePartLoadPerformanceCurve() const;

      bool isCoefficient4ofthePartLoadPerformanceCurveDefaulted() const;

      double minimumFlowRate() const;

      bool isMinimumFlowRateDefaulted() const;

      std::string pumpControlType() const;

      bool isPumpControlTypeDefaulted() const;

      boost::optional<Schedule> pumpFlowRateSchedule() const;

      boost::optional<Curve> pumpCurve() const;

      boost::optional<double> impellerDiameter() const;

      boost::optional<std::string> vFDControlType() const;

      boost::optional<Schedule> pumpRPMSchedule() const;

      boost::optional<Schedule> minimumPressureSchedule() const;

      boost::optional<Schedule> maximumPressureSchedule() const;

      boost::optional<Schedule> minimumRPMSchedule() const;

      boost::optional<Schedule> maximumRPMSchedule() const;

      boost::optional<double> autosizedRatedFlowRate() const;

      boost::optional<double> autosizedRatedPowerConsumption() const;

      std::string endUseSubcategory() const;

      //@}
      /** @name Setters */
      //@{

      bool setRatedFlowRate(boost::optional<double> ratedFlowRate);

      void resetRatedFlowRate();

      void autosizeRatedFlowRate();

      bool setRatedPumpHead(double ratedPumpHead);

      void resetRatedPumpHead();

      bool setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption);

      void resetRatedPowerConsumption();

      void autosizeRatedPowerConsumption();

      bool setMotorEfficiency(double motorEfficiency);

      void resetMotorEfficiency();

      bool setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream);

      void resetFractionofMotorInefficienciestoFluidStream();

      bool setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve);

      void resetCoefficient1ofthePartLoadPerformanceCurve();

      bool setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve);

      void resetCoefficient2ofthePartLoadPerformanceCurve();

      bool setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve);

      void resetCoefficient3ofthePartLoadPerformanceCurve();

      bool setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve);

      void resetCoefficient4ofthePartLoadPerformanceCurve();

      bool setMinimumFlowRate(double minimumFlowRate);

      void resetMinimumFlowRate();

      bool setPumpControlType(const std::string& pumpControlType);

      void resetPumpControlType();

      bool setPumpFlowRateSchedule(Schedule& schedule);

      void resetPumpFlowRateSchedule();

      bool setPumpCurve(const Curve& curve);

      void resetPumpCurve();

      bool setImpellerDiameter(boost::optional<double> impellerDiameter);

      void resetImpellerDiameter();

      bool setVFDControlType(boost::optional<std::string> vFDControlType);

      void resetVFDControlType();

      bool setPumpRPMSchedule(Schedule& schedule);

      void resetPumpRPMSchedule();

      bool setMinimumPressureSchedule(Schedule& schedule);

      void resetMinimumPressureSchedule();

      bool setMaximumPressureSchedule(Schedule& schedule);

      void resetMaximumPressureSchedule();

      bool setMinimumRPMSchedule(Schedule& schedule);

      void resetMinimumRPMSchedule();

      bool setMaximumRPMSchedule(Schedule& schedule);

      void resetMaximumRPMSchedule();

      std::string designPowerSizingMethod() const;

      bool setDesignPowerSizingMethod(const std::string& designPowerSizingMethod);

      double designElectricPowerPerUnitFlowRate() const;

      bool setDesignElectricPowerPerUnitFlowRate(double designElectricPowerPerUnitFlowRate);

      double designShaftPowerPerUnitFlowRatePerUnitHead() const;

      bool setDesignShaftPowerPerUnitFlowRatePerUnitHead(double designShaftPowerPerUnitFlowRatePerUnitHead);

      boost::optional<ThermalZone> zone() const;

      bool setZone(const ThermalZone& thermalZone);

      void resetZone();

      double skinLossRadiativeFraction() const;

      bool setSkinLossRadiativeFraction(double skinLossRadiativeFraction);

      double designMinimumFlowRateFraction() const;

      bool setDesignMinimumFlowRateFraction(double designMinimumFlowRateFraction);

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.PumpVariableSpeed");

      std::vector<std::string> pumpControlTypeValues() const;
      std::vector<std::string> vfdControlTypeValues() const;

      boost::optional<ModelObject> pumpFlowRateScheduleAsModelObject() const;
      boost::optional<ModelObject> pumpCurveAsModelObject() const;
      boost::optional<ModelObject> pumpRPMScheduleAsModelObject() const;
      boost::optional<ModelObject> minimumPressureScheduleAsModelObject() const;
      boost::optional<ModelObject> maximumPressureScheduleAsModelObject() const;
      boost::optional<ModelObject> minimumRPMScheduleAsModelObject() const;
      boost::optional<ModelObject> maximumRPMScheduleAsModelObject() const;

      bool setPumpFlowRateScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setPumpCurveAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setPumpRPMScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setMinimumPressureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setMaximumPressureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setMinimumRPMScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setMaximumRPMScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PUMPVARIABLESPEED_IMPL_HPP
