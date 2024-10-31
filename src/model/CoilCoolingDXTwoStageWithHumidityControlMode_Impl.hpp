/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGDXTWOSTAGEWITHHUMIDITYCONTROLMODE_IMPL_HPP
#define MODEL_COILCOOLINGDXTWOSTAGEWITHHUMIDITYCONTROLMODE_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class CoilPerformanceDXCooling;
  class Curve;
  class Schedule;

  namespace detail {

    /** CoilCoolingDXTwoStageWithHumidityControlMode_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingDXTwoStageWithHumidityControlMode.*/
    class MODEL_API CoilCoolingDXTwoStageWithHumidityControlMode_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingDXTwoStageWithHumidityControlMode_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingDXTwoStageWithHumidityControlMode_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingDXTwoStageWithHumidityControlMode_Impl(const CoilCoolingDXTwoStageWithHumidityControlMode_Impl& other, Model_Impl* model,
                                                        bool keepHandle);

      virtual ~CoilCoolingDXTwoStageWithHumidityControlMode_Impl() override = default;

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

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      double crankcaseHeaterCapacity() const;

      boost::optional<Curve> crankcaseHeaterCapacityFunctionofTemperatureCurve() const;

      double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

      int numberofCapacityStages() const;

      int numberofEnhancedDehumidificationModes() const;

      boost::optional<CoilPerformanceDXCooling> normalModeStage1CoilPerformance() const;

      boost::optional<CoilPerformanceDXCooling> normalModeStage1Plus2CoilPerformance() const;

      boost::optional<CoilPerformanceDXCooling> dehumidificationMode1Stage1CoilPerformance() const;

      boost::optional<CoilPerformanceDXCooling> dehumidificationMode1Stage1Plus2CoilPerformance() const;

      double basinHeaterCapacity() const;

      double basinHeaterSetpointTemperature() const;

      boost::optional<Schedule> basinHeaterOperatingSchedule() const;

      double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

      bool setCrankcaseHeaterCapacityFunctionofTemperatureCurve(const Curve& curve);
      void resetCrankcaseHeaterCapacityFunctionofTemperatureCurve();

      bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

      bool setNumberofCapacityStages(int numberofCapacityStages);

      bool setNumberofEnhancedDehumidificationModes(int numberofEnhancedDehumidificationModes);

      bool setNormalModeStage1CoilPerformance(const boost::optional<CoilPerformanceDXCooling>& coilPerformanceDX);

      void resetNormalModeStage1CoilPerformance();

      bool setNormalModeStage1Plus2CoilPerformance(const boost::optional<CoilPerformanceDXCooling>& coilPerformanceDX);

      void resetNormalModeStage1Plus2CoilPerformance();

      bool setDehumidificationMode1Stage1CoilPerformance(const boost::optional<CoilPerformanceDXCooling>& coilPerformanceDX);

      void resetDehumidificationMode1Stage1CoilPerformance();

      bool setDehumidificationMode1Stage1Plus2CoilPerformance(const boost::optional<CoilPerformanceDXCooling>& coilPerformanceDX);

      void resetDehumidificationMode1Stage1Plus2CoilPerformance();

      bool setBasinHeaterCapacity(double basinHeaterCapacity);

      bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

      bool setBasinHeaterOperatingSchedule(Schedule& schedule);

      void resetBasinHeaterOperatingSchedule();

      bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

      //@}
      /** @name Other */
      //@{

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDXTwoStageWithHumidityControlMode");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXTWOSTAGEWITHHUMIDITYCONTROLMODE_IMPL_HPP
