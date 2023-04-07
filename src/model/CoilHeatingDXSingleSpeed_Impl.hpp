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

#ifndef MODEL_COILHEATINGDXSINGLESPEED_IMPL_HPP
#define MODEL_COILHEATINGDXSINGLESPEED_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;
  class ZoneHVACComponent;

  namespace detail {

    /** CoilHeatingDXSingleSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingDXSingleSpeed.*/
    class MODEL_API CoilHeatingDXSingleSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingDXSingleSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingDXSingleSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingDXSingleSpeed_Impl(const CoilHeatingDXSingleSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingDXSingleSpeed_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

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

      boost::optional<double> ratedTotalHeatingCapacity() const;

      bool isRatedTotalHeatingCapacityAutosized() const;

      double ratedCOP() const;

      boost::optional<double> ratedAirFlowRate() const;

      double ratedSupplyFanPowerPerVolumeFlowRate2017() const;

      double ratedSupplyFanPowerPerVolumeFlowRate2023() const;

      bool isRatedAirFlowRateAutosized() const;

      double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

      bool isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted() const;

      double maximumOutdoorDryBulbTemperatureforDefrostOperation() const;

      bool isMaximumOutdoorDryBulbTemperatureforDefrostOperationDefaulted() const;

      double crankcaseHeaterCapacity() const;

      bool isCrankcaseHeaterCapacityDefaulted() const;

      double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

      bool isMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperationDefaulted() const;

      std::string defrostStrategy() const;

      bool isDefrostStrategyDefaulted() const;

      std::string defrostControl() const;

      bool isDefrostControlDefaulted() const;

      double defrostTimePeriodFraction() const;

      bool isDefrostTimePeriodFractionDefaulted() const;

      boost::optional<double> resistiveDefrostHeaterCapacity() const;

      bool isResistiveDefrostHeaterCapacityDefaulted() const;

      bool isResistiveDefrostHeaterCapacityAutosized() const;

      boost::optional<double> autosizedRatedTotalHeatingCapacity() const;

      boost::optional<double> autosizedRatedAirFlowRate() const;

      boost::optional<double> autosizedResistiveDefrostHeaterCapacity() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setRatedTotalHeatingCapacity(boost::optional<double> ratedTotalHeatingCapacity);

      void autosizeRatedTotalHeatingCapacity();

      bool setRatedCOP(double ratedCOP);

      bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

      void autosizeRatedAirFlowRate();

      bool setRatedSupplyFanPowerPerVolumeFlowRate2017(double ratedSupplyFanPowerPerVolumeFlowRate2017);

      bool setRatedSupplyFanPowerPerVolumeFlowRate2023(double ratedSupplyFanPowerPerVolumeFlowRate2023);

      bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

      void resetMinimumOutdoorDryBulbTemperatureforCompressorOperation();

      bool setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation);

      void resetMaximumOutdoorDryBulbTemperatureforDefrostOperation();

      bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

      void resetCrankcaseHeaterCapacity();

      bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

      void resetMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation();

      bool setDefrostStrategy(const std::string& defrostStrategy);

      void resetDefrostStrategy();

      bool setDefrostControl(const std::string& defrostControl);

      void resetDefrostControl();

      bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

      void resetDefrostTimePeriodFraction();

      bool setResistiveDefrostHeaterCapacity(boost::optional<double> resistiveDefrostHeaterCapacity);

      void resetResistiveDefrostHeaterCapacity();

      void autosizeResistiveDefrostHeaterCapacity();

      //@}
      /** @name Curves */
      //@{

      // A5 , \field Total Heating Capacity Function of Temperature Curve Name
      // \object-list BiquadraticQuadraticCubicCurves
      Curve totalHeatingCapacityFunctionofTemperatureCurve() const;
      bool setTotalHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

      // A6 , \field Total Heating Capacity Function of Flow Fraction Curve Name
      // \object-list QuadraticCubicCurves
      Curve totalHeatingCapacityFunctionofFlowFractionCurve() const;
      bool setTotalHeatingCapacityFunctionofFlowFractionCurve(const Curve& curve);

      // A7 ,  \field Energy Input Ratio Function of Temperature Curve Name
      // \object-list BiquadraticQuadraticCubicCurves
      Curve energyInputRatioFunctionofTemperatureCurve() const;
      bool setEnergyInputRatioFunctionofTemperatureCurve(const Curve& curve);

      // A8 , \field Energy Input Ratio Function of Flow Fraction Curve Name
      // \object-list QuadraticCubicCurves
      Curve energyInputRatioFunctionofFlowFractionCurve() const;
      bool setEnergyInputRatioFunctionofFlowFractionCurve(const Curve& curve);

      // A9 , \field Part Load Fraction Correlation Curve Name
      // \object-list QuadraticCubicCurves
      Curve partLoadFractionCorrelationCurve() const;
      bool setPartLoadFractionCorrelationCurve(const Curve& curve);

      // A10, \field Defrost Energy Input Ratio Function of Temperature Curve Name
      // \object-list BiquadraticCurves
      // not a required curve so it needs a reset
      boost::optional<Curve> defrostEnergyInputRatioFunctionofTemperatureCurve() const;
      bool setDefrostEnergyInputRatioFunctionofTemperatureCurve(const boost::optional<Curve> curve);

      bool addToNode(Node& node) override;

      //@}

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingDXSingleSpeed");

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_COILHEATINGDXSINGLESPEED_IMPL_HPP
