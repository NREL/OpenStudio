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

#ifndef MODEL_PUMPCONSTANTSPEED_IMPL_HPP
#define MODEL_PUMPCONSTANTSPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Connection;
  class Schedule;
  class Curve;
  class ThermalZone;

  namespace detail {

    /** PumpConstantSpeed_Impl is a StraightComponent_Impl that is the implementation class for PumpConstantSpeed.*/
    class MODEL_API PumpConstantSpeed_Impl : public StraightComponent_Impl
    {
     public:
      PumpConstantSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PumpConstantSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PumpConstantSpeed_Impl(const PumpConstantSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PumpConstantSpeed_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{
      //
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

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

      bool isRatedFlowRateAutosized() const;

      double ratedPumpHead() const;

      bool isRatedPumpHeadDefaulted() const;

      boost::optional<double> ratedPowerConsumption() const;

      bool isRatedPowerConsumptionAutosized() const;

      double motorEfficiency() const;

      bool isMotorEfficiencyDefaulted() const;

      double fractionofMotorInefficienciestoFluidStream() const;

      bool isFractionofMotorInefficienciestoFluidStreamDefaulted() const;

      std::string pumpControlType() const;

      bool isPumpControlTypeDefaulted() const;

      boost::optional<Schedule> pumpFlowRateSchedule() const;

      boost::optional<Curve> pumpCurve() const;

      boost::optional<double> impellerDiameter() const;

      boost::optional<double> rotationalSpeed() const;

      boost::optional<ThermalZone> zone() const;

      boost::optional<double> skinLossRadiativeFraction() const;

      std::string endUseSubcategory() const;

      std::string designPowerSizingMethod() const;

      double designElectricPowerPerUnitFlowRate() const;

      double designShaftPowerPerUnitFlowRatePerUnitHead() const;

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

      bool setPumpControlType(const std::string& pumpControlType);

      void resetPumpControlType();

      bool setPumpFlowRateSchedule(Schedule& schedule);

      void resetPumpFlowRateSchedule();

      bool setPumpCurve(const boost::optional<Curve>& curve);

      void resetPumpCurve();

      bool setImpellerDiameter(boost::optional<double> impellerDiameter);

      void resetImpellerDiameter();

      bool setRotationalSpeed(boost::optional<double> rotationalSpeed);

      void resetRotationalSpeed();

      bool setZone(const boost::optional<ThermalZone>& thermalZone);

      void resetZone();

      bool setSkinLossRadiativeFraction(boost::optional<double> skinLossRadiativeFraction);

      void resetSkinLossRadiativeFraction();

      bool setDesignPowerSizingMethod(const std::string& designPowerSizingMethod);

      bool setDesignElectricPowerPerUnitFlowRate(double designElectricPowerPerUnitFlowRate);

      bool setDesignShaftPowerPerUnitFlowRatePerUnitHead(double designShaftPowerPerUnitFlowRatePerUnitHead);

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedRatedFlowRate() const;

      boost::optional<double> autosizedRatedPowerConsumption() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.PumpConstantSpeed");

      boost::optional<Connection> optionalInletNode() const;
      boost::optional<Connection> optionalOutletNode() const;

      std::vector<std::string> pumpControlTypeValues() const;
      boost::optional<ModelObject> pumpFlowRateScheduleAsModelObject() const;
      boost::optional<ModelObject> pumpCurveAsModelObject() const;
      boost::optional<ModelObject> zoneAsModelObject() const;

      bool setPumpFlowRateScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setPumpCurveAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setZoneAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PUMPCONSTANTSPEED_IMPL_HPP
