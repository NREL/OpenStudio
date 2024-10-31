/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXMULTISPEED_IMPL_HPP
#define MODEL_COILCOOLINGDXMULTISPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class CoilCoolingDXMultiSpeedStageData;
  class Curve;
  class Schedule;

  namespace detail {

    /** CoilCoolingDXMultiSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingDXMultiSpeed.*/
    class MODEL_API CoilCoolingDXMultiSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingDXMultiSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingDXMultiSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingDXMultiSpeed_Impl(const CoilCoolingDXMultiSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingDXMultiSpeed_Impl() override = default;

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

      std::string condenserType() const;

      bool applyPartLoadFractiontoSpeedsGreaterthan1() const;

      bool applyLatentDegradationtoSpeedsGreaterthan1() const;

      bool isApplyLatentDegradationtoSpeedsGreaterthan1Defaulted() const;

      double crankcaseHeaterCapacity() const;

      boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

      double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

      double basinHeaterCapacity() const;

      double basinHeaterSetpointTemperature() const;

      boost::optional<Schedule> basinHeaterOperatingSchedule() const;

      std::string fuelType() const;

      double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setCondenserType(const std::string& condenserType);

      bool setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1);

      bool setApplyLatentDegradationtoSpeedsGreaterthan1(bool applyLatentDegradationtoSpeedsGreaterthan1);

      void resetApplyLatentDegradationtoSpeedsGreaterthan1();

      bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

      bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
      void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

      bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

      bool setBasinHeaterCapacity(double basinHeaterCapacity);

      bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

      bool setBasinHeaterOperatingSchedule(Schedule& schedule);

      void resetBasinHeaterOperatingSchedule();

      bool setFuelType(const std::string& fuelType);

      bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

      //@}
      /** @name Other */
      //@{

      // Extensible: Stages
      std::vector<CoilCoolingDXMultiSpeedStageData> stages() const;
      unsigned numberOfStages() const;
      boost::optional<unsigned> stageIndex(const CoilCoolingDXMultiSpeedStageData& stage) const;

      // Note: a CoilCoolingDXMultiSpeedStageData can be used only by one CoilCoolingDXMultiSpeed
      bool addStage(const CoilCoolingDXMultiSpeedStageData& stage);
      bool addStage(const CoilCoolingDXMultiSpeedStageData& stage, unsigned index);
      bool setStageIndex(const CoilCoolingDXMultiSpeedStageData& stage, unsigned index);
      bool setStages(const std::vector<CoilCoolingDXMultiSpeedStageData>& stages);
      void removeAllStages();
      bool removeStage(const CoilCoolingDXMultiSpeedStageData& stage);
      bool removeStage(unsigned index);

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDXMultiSpeed");

      boost::optional<Connection> optionalAirInletNode() const;
      boost::optional<Connection> optionalAirOutletNode() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXMULTISPEED_IMPL_HPP
