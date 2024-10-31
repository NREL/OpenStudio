/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PIPEINDOOR_IMPL_HPP
#define MODEL_PIPEINDOOR_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Construction;
  class Schedule;
  class ThermalZone;

  namespace detail {

    /** PipeIndoor_Impl is a  StraightComponent_Impl that is the implementation class for PipeIndoor.*/
    class MODEL_API PipeIndoor_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PipeIndoor_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PipeIndoor_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PipeIndoor_Impl(const PipeIndoor_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PipeIndoor_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Construction> construction() const;

      std::string environmentType() const;

      boost::optional<ThermalZone> ambientTemperatureZone() const;

      boost::optional<Schedule> ambientTemperatureSchedule() const;

      boost::optional<Schedule> ambientAirVelocitySchedule() const;

      double pipeInsideDiameter() const;

      double pipeLength() const;

      //@}
      /** @name Setters */
      //@{

      bool setConstruction(const boost::optional<Construction>& construction);

      void resetConstruction();

      bool setEnvironmentType(const std::string& environmentType);

      bool setAmbientTemperatureZone(const boost::optional<ThermalZone>& thermalZone);

      void resetAmbientTemperatureZone();

      bool setAmbientTemperatureSchedule(Schedule& schedule);

      void resetAmbientTemperatureSchedule();

      bool setAmbientAirVelocitySchedule(Schedule& schedule);

      void resetAmbientAirVelocitySchedule();

      bool setPipeInsideDiameter(double pipeInsideDiameter);

      bool setPipeLength(double pipeLength);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.PipeIndoor");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PIPEINDOOR_IMPL_HPP
