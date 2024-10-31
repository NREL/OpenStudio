/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEED_IMPL_HPP
#define MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData;
  class Curve;
  class ModelObjectList;

  namespace detail {

    /** CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl is a ModelObject_Impl that is the implementation class for CoilWaterHeatingAirToWaterHeatPumpVariableSpeed.*/
    class MODEL_API CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl : public HVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl& other, Model_Impl* model,
                                                           bool keepHandle);

      virtual ~CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      std::vector<ModelObject> children() const override;

      ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      int nominalSpeedLevel() const;

      double ratedWaterHeatingCapacity() const;

      double ratedEvaporatorInletAirDryBulbTemperature() const;

      double ratedEvaporatorInletAirWetBulbTemperature() const;

      double ratedCondenserInletWaterTemperature() const;

      boost::optional<double> ratedEvaporatorAirFlowRate() const;

      bool isRatedEvaporatorAirFlowRateAutocalculated() const;

      boost::optional<double> ratedCondenserWaterFlowRate() const;

      bool isRatedCondenserWaterFlowRateAutocalculated() const;

      std::string evaporatorFanPowerIncludedinRatedCOP() const;

      std::string condenserPumpPowerIncludedinRatedCOP() const;

      std::string condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP() const;

      double fractionofCondenserPumpHeattoWater() const;

      double crankcaseHeaterCapacity() const;

      boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

      double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;

      std::string evaporatorAirTemperatureTypeforCurveObjects() const;

      Curve partLoadFractionCorrelationCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setNominalSpeedLevel(int nominalSpeedLevel);

      bool setRatedWaterHeatingCapacity(double ratedWaterHeatingCapacity);

      bool setRatedEvaporatorInletAirDryBulbTemperature(double ratedEvaporatorInletAirDryBulbTemperature);

      bool setRatedEvaporatorInletAirWetBulbTemperature(double ratedEvaporatorInletWetDryBulbTemperature);

      bool setRatedCondenserInletWaterTemperature(double ratedCondenserInletWaterTemperature);

      bool setRatedEvaporatorAirFlowRate(boost::optional<double> ratedEvaporatorAirFlowRate);

      void autocalculateRatedEvaporatorAirFlowRate();

      bool setRatedCondenserWaterFlowRate(boost::optional<double> ratedCondenserWaterFlowRate);

      void autocalculateRatedCondenserWaterFlowRate();

      bool setEvaporatorFanPowerIncludedinRatedCOP(const std::string& evaporatorFanPowerIncludedinRatedCOP);

      bool setCondenserPumpPowerIncludedinRatedCOP(const std::string& condenserPumpPowerIncludedinRatedCOP);

      bool
        setCondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP(const std::string& condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP);

      bool setFractionofCondenserPumpHeattoWater(double fractionofCondenserPumpHeattoWater);

      bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

      bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
      void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

      bool setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation);

      bool setEvaporatorAirTemperatureTypeforCurveObjects(const std::string& evaporatorAirTemperatureTypeforCurveObjects);

      bool setPartLoadFractionCorrelationCurve(const Curve& partLoadFractionCorrelationCurve);

      //@}
      /** @name Other */
      //@{

      bool setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList);

      void resetSpeedDataList();

      boost::optional<ModelObjectList> speedDataList() const;

      std::vector<CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData> speeds() const;

      bool addSpeed(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& speed);

      void removeSpeed(const CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData& speed);

      void removeAllSpeeds();

      // Autosize methods
      boost::optional<double> autocalculatedRatedEvaporatorAirFlowRate() const;

      boost::optional<double> autocalculatedRatedCondenserWaterFlowRate() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilWaterHeatingAirToWaterHeatPumpVariableSpeed");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILWATERHEATINGAIRTOWATERHEATPUMPVARIABLESPEED_IMPL_HPP
