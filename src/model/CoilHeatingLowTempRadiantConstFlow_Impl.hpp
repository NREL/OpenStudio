/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP
#define MODEL_COILHEATINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** CoilHeatingLowTempRadiantConstFlow_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingLowTempRadiantConstFlow.*/
    class MODEL_API CoilHeatingLowTempRadiantConstFlow_Impl : public StraightComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingLowTempRadiantConstFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingLowTempRadiantConstFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingLowTempRadiantConstFlow_Impl(const CoilHeatingLowTempRadiantConstFlow_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingLowTempRadiantConstFlow_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> heatingHighWaterTemperatureSchedule() const;

      boost::optional<Schedule> heatingLowWaterTemperatureSchedule() const;

      boost::optional<Schedule> heatingHighControlTemperatureSchedule() const;

      boost::optional<Schedule> heatingLowControlTemperatureSchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setHeatingHighWaterTemperatureSchedule(Schedule& schedule);

      void resetHeatingHighWaterTemperatureSchedule();

      bool setHeatingLowWaterTemperatureSchedule(Schedule& schedule);

      void resetHeatingLowWaterTemperatureSchedule();

      bool setHeatingHighControlTemperatureSchedule(Schedule& schedule);

      void resetHeatingHighControlTemperatureSchedule();

      bool setHeatingLowControlTemperatureSchedule(Schedule& schedule);

      void resetHeatingLowControlTemperatureSchedule();

      //@}
      /** @name Other */
      //@{

      bool addToNode(Node& node) override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingLowTempRadiantConstFlow");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals

      boost::optional<ModelObject> heatingWaterInletNodeAsModelObject() const;
      boost::optional<ModelObject> heatingWaterOutletNodeAsModelObject() const;
      boost::optional<ModelObject> heatingHighWaterTemperatureScheduleAsModelObject() const;
      boost::optional<ModelObject> heatingLowWaterTemperatureScheduleAsModelObject() const;
      boost::optional<ModelObject> heatingHighControlTemperatureScheduleAsModelObject() const;
      boost::optional<ModelObject> heatingLowControlTemperatureScheduleAsModelObject() const;

      bool setHeatingWaterInletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setHeatingWaterOutletNodeAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setHeatingHighWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setHeatingLowWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setHeatingHighControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setHeatingLowControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP
