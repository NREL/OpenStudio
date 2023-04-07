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

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_IMPL_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_IMPL_HPP

#include "ModelAPI.hpp"
#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    /** CoilWaterHeatingAirToWaterHeatPump_Impl is a ModelObject_Impl that is the implementation class for CoilWaterHeatingAirToWaterHeatPump.*/
    class MODEL_API CoilWaterHeatingAirToWaterHeatPump_Impl : public HVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilWaterHeatingAirToWaterHeatPump_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilWaterHeatingAirToWaterHeatPump_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilWaterHeatingAirToWaterHeatPump_Impl(const CoilWaterHeatingAirToWaterHeatPump_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilWaterHeatingAirToWaterHeatPump_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      double ratedHeatingCapacity() const;

      double ratedCOP() const;

      double ratedSensibleHeatRatio() const;

      double ratedEvaporatorInletAirDryBulbTemperature() const;

      double ratedEvaporatorInletAirWetBulbTemperature() const;

      double ratedCondenserInletWaterTemperature() const;

      boost::optional<double> ratedEvaporatorAirFlowRate() const;

      bool isRatedEvaporatorAirFlowRateAutosized() const;

      boost::optional<double> ratedCondenserWaterFlowRate() const;

      bool isRatedCondenserWaterFlowRateAutosized() const;

      bool evaporatorFanPowerIncludedinRatedCOP() const;

      bool condenserPumpPowerIncludedinRatedCOP() const;

      bool condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP() const;

      double condenserWaterPumpPower() const;

      double fractionofCondenserPumpHeattoWater() const;

      double crankcaseHeaterCapacity() const;

      double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;

      std::string evaporatorAirTemperatureTypeforCurveObjects() const;

      Curve heatingCapacityFunctionofTemperatureCurve() const;

      Curve heatingCapacityFunctionofAirFlowFractionCurve() const;

      Curve heatingCapacityFunctionofWaterFlowFractionCurve() const;

      Curve heatingCOPFunctionofTemperatureCurve() const;

      Curve heatingCOPFunctionofAirFlowFractionCurve() const;

      Curve heatingCOPFunctionofWaterFlowFractionCurve() const;

      Curve partLoadFractionCorrelationCurve() const;

      boost::optional<double> autosizedRatedEvaporatorAirFlowRate() const;

      boost::optional<double> autosizedRatedCondenserWaterFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setRatedHeatingCapacity(double ratedHeatingCapacity);

      bool setRatedCOP(double ratedCOP);

      bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

      bool setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature);

      bool setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletAirWetBulbTemperature);

      bool setRatedCondenserInletWaterTemperature(double ratedCondenserInletWaterTemperature);

      bool setRatedEvaporatorAirFlowRate(boost::optional<double> ratedEvaporatorAirFlowRate);

      void autosizeRatedEvaporatorAirFlowRate();

      bool setRatedCondenserWaterFlowRate(boost::optional<double> ratedCondenserWaterFlowRate);

      void autosizeRatedCondenserWaterFlowRate();

      bool setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP);

      bool setCondenserPumpPowerIncludedinRatedCOP(bool condenserPumpPowerIncludedinRatedCOP);

      bool setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(bool condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP);

      bool setCondenserWaterPumpPower(double condenserWaterPumpPower);

      bool setFractionofCondenserPumpHeattoWater(double fractionofCondenserPumpHeattoWater);

      bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

      bool setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation);

      bool setEvaporatorAirTemperatureTypeforCurveObjects(const std::string& evaporatorAirTemperatureTypeforCurveObjects);

      bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

      bool setHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

      bool setHeatingCapacityFunctionofWaterFlowFractionCurve(const Curve& curve);

      bool setHeatingCOPFunctionofTemperatureCurve(const Curve& curve);

      bool setHeatingCOPFunctionofAirFlowFractionCurve(const Curve& curve);

      bool setHeatingCOPFunctionofWaterFlowFractionCurve(const Curve& curve);

      bool setPartLoadFractionCorrelationCurve(const Curve& curve);

      //@}
      /** @name Other */
      //@{

      std::vector<ModelObject> children() const override;
      ModelObject clone(Model model) const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPump");

      boost::optional<Curve> optionalHeatingCapacityFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalHeatingCapacityFunctionofAirFlowFractionCurve() const;
      boost::optional<Curve> optionalHeatingCapacityFunctionofWaterFlowFractionCurve() const;
      boost::optional<Curve> optionalHeatingCOPFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalHeatingCOPFunctionofAirFlowFractionCurve() const;
      boost::optional<Curve> optionalHeatingCOPFunctionofWaterFlowFractionCurve() const;
      boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMP_IMPL_HPP
