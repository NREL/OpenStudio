/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDXMULTISPEED_IMPL_HPP
#define MODEL_COILHEATINGDXMULTISPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class CoilHeatingDXMultiSpeedStageData;
  class Curve;
  class Schedule;

  namespace detail {

    /** CoilHeatingDXMultiSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingDXMultiSpeed.*/
    class MODEL_API CoilHeatingDXMultiSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingDXMultiSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingDXMultiSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingDXMultiSpeed_Impl(const CoilHeatingDXMultiSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingDXMultiSpeed_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

      boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor() const;

      double crankcaseHeaterCapacity() const;

      boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

      double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

      boost::optional<Curve> defrostEnergyInputRatioFunctionofTemperatureCurve() const;

      double maximumOutdoorDryBulbTemperatureforDefrostOperation() const;

      std::string defrostStrategy() const;

      std::string defrostControl() const;

      double defrostTimePeriodFraction() const;

      boost::optional<double> resistiveDefrostHeaterCapacity() const;

      bool isResistiveDefrostHeaterCapacityAutosized() const;

      bool applyPartLoadFractiontoSpeedsGreaterthan1() const;

      std::string fuelType() const;

      int regionnumberforCalculatingHSPF() const;

      boost::optional<double> autosizedResistiveDefrostHeaterCapacity() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

      bool setOutdoorDryBulbTemperaturetoTurnOnCompressor(boost::optional<double> outdoorDryBulbTemperaturetoTurnOnCompressor);

      void resetOutdoorDryBulbTemperaturetoTurnOnCompressor();

      bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

      bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
      void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

      bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

      bool setDefrostEnergyInputRatioFunctionofTemperatureCurve(const boost::optional<Curve>& curve);

      void resetDefrostEnergyInputRatioFunctionofTemperatureCurve();

      bool setMaximumOutdoorDryBulbTemperatureforDefrostOperation(double maximumOutdoorDryBulbTemperatureforDefrostOperation);

      bool setDefrostStrategy(const std::string& defrostStrategy);

      bool setDefrostControl(const std::string& defrostControl);

      bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

      bool setResistiveDefrostHeaterCapacity(boost::optional<double> resistiveDefrostHeaterCapacity);

      void autosizeResistiveDefrostHeaterCapacity();

      bool setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1);

      bool setFuelType(const std::string& fuelType);

      bool setRegionnumberforCalculatingHSPF(int regionnumberforCalculatingHSPF);

      //@}
      /** @name Other */
      //@{

      // Extensible: Stages
      std::vector<CoilHeatingDXMultiSpeedStageData> stages() const;
      unsigned numberOfStages() const;
      boost::optional<unsigned> stageIndex(const CoilHeatingDXMultiSpeedStageData& stage) const;

      // Note: a CoilHeatingDXMultiSpeedStageData can be used only by one CoilHeatingDXMultiSpeed
      bool addStage(const CoilHeatingDXMultiSpeedStageData& stage);
      bool addStage(const CoilHeatingDXMultiSpeedStageData& stage, unsigned index);
      bool setStageIndex(const CoilHeatingDXMultiSpeedStageData& stage, unsigned index);
      bool setStages(const std::vector<CoilHeatingDXMultiSpeedStageData>& stages);
      void removeAllStages();
      bool removeStage(const CoilHeatingDXMultiSpeedStageData& stage);
      bool removeStage(unsigned index);

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingDXMultiSpeed");

      boost::optional<Connection> optionalAirInletNode() const;
      boost::optional<Connection> optionalAirOutletNode() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGDXMULTISPEED_IMPL_HPP
