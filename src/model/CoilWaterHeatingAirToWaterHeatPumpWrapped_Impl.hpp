/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPWRAPPED_IMPL_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPWRAPPED_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    /** CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl is a ModelObject_Impl that is the implementation class for CoilWaterHeatingAirToWaterHeatPumpWrapped.*/
    class MODEL_API CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl(const CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      // TODO: This is NOT an HVACComponent
      // virtual ComponentType componentType() const override;
      // virtual std::vector<FuelType> coolingFuelTypes() const override;
      // virtual std::vector<FuelType> heatingFuelTypes() const override;
      // virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      double ratedHeatingCapacity() const;

      double ratedCOP() const;

      double ratedSensibleHeatRatio() const;

      double ratedEvaporatorInletAirDryBulbTemperature() const;

      double ratedEvaporatorInletAirWetBulbTemperature() const;

      double ratedCondenserWaterTemperature() const;

      boost::optional<double> ratedEvaporatorAirFlowRate() const;

      bool isRatedEvaporatorAirFlowRateAutocalculated() const;

      bool evaporatorFanPowerIncludedinRatedCOP() const;

      double crankcaseHeaterCapacity() const;

      boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

      double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;

      std::string evaporatorAirTemperatureTypeforCurveObjects() const;

      Curve heatingCapacityFunctionofTemperatureCurve() const;

      Curve heatingCapacityFunctionofAirFlowFractionCurve() const;

      Curve heatingCOPFunctionofTemperatureCurve() const;

      Curve heatingCOPFunctionofAirFlowFractionCurve() const;

      Curve partLoadFractionCorrelationCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setRatedHeatingCapacity(double ratedHeatingCapacity);

      bool setRatedCOP(double ratedCOP);

      bool setRatedSensibleHeatRatio(double ratedSensibleHeatRatio);

      bool setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature);

      bool setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletAirWetBulbTemperature);

      bool setRatedCondenserWaterTemperature(double ratedCondenserWaterTemperature);

      bool setRatedEvaporatorAirFlowRate(boost::optional<double> ratedEvaporatorAirFlowRate);

      void autocalculateRatedEvaporatorAirFlowRate();

      bool setEvaporatorFanPowerIncludedinRatedCOP(bool evaporatorFanPowerIncludedinRatedCOP);

      bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

      bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
      void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

      bool setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation);

      bool setEvaporatorAirTemperatureTypeforCurveObjects(const std::string& evaporatorAirTemperatureTypeforCurveObjects);

      bool setHeatingCapacityFunctionofTemperatureCurve(const Curve& curve);

      bool setHeatingCapacityFunctionofAirFlowFractionCurve(const Curve& curve);

      bool setHeatingCOPFunctionofTemperatureCurve(const Curve& curve);

      bool setHeatingCOPFunctionofAirFlowFractionCurve(const Curve& curve);

      bool setPartLoadFractionCorrelationCurve(const Curve& curve);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPumpWrapped");

      boost::optional<Curve> optionalHeatingCapacityFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalHeatingCapacityFunctionofAirFlowFractionCurve() const;
      boost::optional<Curve> optionalHeatingCOPFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalHeatingCOPFunctionofAirFlowFractionCurve() const;
      boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPWRAPPED_IMPL_HPP
