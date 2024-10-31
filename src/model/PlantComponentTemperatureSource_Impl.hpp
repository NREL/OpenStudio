/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTCOMPONENTTEMPERATURESOURCE_IMPL_HPP
#define MODEL_PLANTCOMPONENTTEMPERATURESOURCE_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    /** PlantComponentTemperatureSource_Impl is a StraightComponent_Impl that is the implementation class for PlantComponentTemperatureSource.*/
    class MODEL_API PlantComponentTemperatureSource_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PlantComponentTemperatureSource_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PlantComponentTemperatureSource_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PlantComponentTemperatureSource_Impl(const PlantComponentTemperatureSource_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PlantComponentTemperatureSource_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      // virtual bool addToNode(Node & node) override;

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

      boost::optional<double> designVolumeFlowRate() const;

      bool isDesignVolumeFlowRateAutosized() const;

      std::string temperatureSpecificationType() const;

      boost::optional<double> sourceTemperature() const;

      boost::optional<Schedule> sourceTemperatureSchedule() const;

      boost::optional<double> autosizedDesignVolumeFlowRate() const;

      //@}
      /** @name Setters */
      //@{

      bool setDesignVolumeFlowRate(boost::optional<double> designVolumeFlowRate);

      void autosizeDesignVolumeFlowRate();

      bool setTemperatureSpecificationType(const std::string& temperatureSpecificationType);

      bool setSourceTemperature(boost::optional<double> sourceTemperature);

      void resetSourceTemperature();

      bool setSourceTemperatureSchedule(Schedule& schedule);

      void resetSourceTemperatureSchedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.PlantComponentTemperatureSource");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTCOMPONENTTEMPERATURESOURCE_IMPL_HPP
