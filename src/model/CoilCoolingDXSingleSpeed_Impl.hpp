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

#ifndef MODEL_COILCOOLINGDXSINGLESPEED_IMPL_HPP
#define MODEL_COILCOOLINGDXSINGLESPEED_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class Information;
  class Schedule;

  namespace detail {

    // derive CoilCoolingDXSingleSpeed_Impl from ModelObject_Impl to override virtual methods
    class MODEL_API CoilCoolingDXSingleSpeed_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      CoilCoolingDXSingleSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      CoilCoolingDXSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      CoilCoolingDXSingleSpeed_Impl(const CoilCoolingDXSingleSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingDXSingleSpeed_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      double ratedCOP() const;

      double ratedEvaporatorFanPowerPerVolumeFlowRate2017() const;

      double ratedEvaporatorFanPowerPerVolumeFlowRate2023() const;

      Curve totalCoolingCapacityFunctionOfTemperatureCurve() const;

      Curve totalCoolingCapacityFunctionOfFlowFractionCurve() const;

      Curve energyInputRatioFunctionOfTemperatureCurve() const;

      Curve energyInputRatioFunctionOfFlowFractionCurve() const;

      Curve partLoadFractionCorrelationCurve() const;

      double nominalTimeForCondensateRemovalToBegin() const;

      double ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity() const;

      double maximumCyclingRate();

      double latentCapacityTimeConstant() const;

      boost::optional<std::string> condenserAirInletNodeName() const;

      std::string condenserType() const;

      double evaporativeCondenserEffectiveness() const;

      boost::optional<double> evaporativeCondenserAirFlowRate() const;

      boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption() const;

      double crankcaseHeaterCapacity() const;

      double maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation() const;

      double basinHeaterCapacity() const;

      double basinHeaterSetpointTemperature() const;

      boost::optional<Schedule> basinHeaterOperatingSchedule() const;

      double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

      // Autosizing methods

      virtual void autosize() override;

      virtual void applySizingValues() override;

      boost::optional<double> autosizedRatedAirFlowRate() const;

      boost::optional<double> autosizedRatedTotalCoolingCapacity() const;

      boost::optional<double> autosizedRatedSensibleHeatRatio() const;

      boost::optional<double> autosizedEvaporativeCondenserAirFlowRate() const;

      boost::optional<double> autosizedEvaporativeCondenserPumpRatedPowerConsumption() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setRatedCOP(boost::optional<double> value);

      bool setRatedCOP(double value);

      bool setRatedEvaporatorFanPowerPerVolumeFlowRate2017(boost::optional<double> value);

      bool setRatedEvaporatorFanPowerPerVolumeFlowRate2017(double value);

      bool setRatedEvaporatorFanPowerPerVolumeFlowRate2023(boost::optional<double> value);

      bool setRatedEvaporatorFanPowerPerVolumeFlowRate2023(double value);

      bool setTotalCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve);

      bool setTotalCoolingCapacityFunctionOfFlowFractionCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionOfTemperatureCurve(const Curve& curve);

      bool setEnergyInputRatioFunctionOfFlowFractionCurve(const Curve& curve);

      bool setPartLoadFractionCorrelationCurve(const Curve& curve);

      bool setNominalTimeForCondensateRemovalToBegin(boost::optional<double> value);

      bool setNominalTimeForCondensateRemovalToBegin(double value);

      bool setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(boost::optional<double> value);

      bool setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(double value);

      bool setMaximumCyclingRate(boost::optional<double> value);

      bool setMaximumCyclingRate(double value);

      bool setLatentCapacityTimeConstant(boost::optional<double> value);

      bool setLatentCapacityTimeConstant(double value);

      bool setCondenserAirInletNodeName(const boost::optional<std::string>& value);

      bool setCondenserType(const std::string& value);

      bool setEvaporativeCondenserEffectiveness(boost::optional<double> value);

      bool setEvaporativeCondenserEffectiveness(double value);

      bool isEvaporativeCondenserAirFlowRateAutosized() const;

      bool setEvaporativeCondenserAirFlowRate(boost::optional<double> value);

      bool setEvaporativeCondenserAirFlowRate(double value);

      void autosizeEvaporativeCondenserAirFlowRate();

      bool isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const;

      bool setEvaporativeCondenserPumpRatedPowerConsumption(boost::optional<double> value);

      bool setEvaporativeCondenserPumpRatedPowerConsumption(double value);

      void autosizeEvaporativeCondenserPumpRatedPowerConsumption();

      bool setCrankcaseHeaterCapacity(boost::optional<double> value);

      bool setCrankcaseHeaterCapacity(double value);

      bool setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(boost::optional<double> value);

      bool setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(double value);

      bool setBasinHeaterCapacity(boost::optional<double> value);

      bool setBasinHeaterCapacity(double value);

      bool setBasinHeaterSetpointTemperature(boost::optional<double> value);

      bool setBasinHeaterSetpointTemperature(double value);

      bool setBasinHeaterOperatingSchedule(Schedule& schedule);

      void resetBasinHeaterOperatingSchedule();

      bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

      boost::optional<double> ratedTotalCoolingCapacity() const;

      bool isRatedTotalCoolingCapacityAutosized() const;

      bool setRatedTotalCoolingCapacity(boost::optional<double> ratedTotalCoolingCapacity);

      void autosizeRatedTotalCoolingCapacity();

      boost::optional<double> ratedSensibleHeatRatio() const;

      bool isRatedSensibleHeatRatioAutosized() const;

      bool setRatedSensibleHeatRatio(boost::optional<double> ratedSensibleHeatRatio);

      void autosizeRatedSensibleHeatRatio();

      boost::optional<double> ratedAirFlowRate() const;

      bool isRatedAirFlowRateAutosized() const;

      bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

      void autosizeRatedAirFlowRate();

      bool addToNode(Node& node) override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDXSingleSpeed");

      // optional getters for use by children so can remove() this object if the constructor fails
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<Curve> optionalTotalCoolingCapacityFunctionOfTemperatureCurve() const;
      boost::optional<Curve> optionalTotalCoolingCapacityFunctionOfFlowFractionCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionOfTemperatureCurve() const;
      boost::optional<Curve> optionalEnergyInputRatioFunctionOfFlowFractionCurve() const;
      boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
      boost::optional<ModelObject> basinHeaterOperatingScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setBasinHeaterOperatingScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXSINGLESPEED_IMPL_HPP
