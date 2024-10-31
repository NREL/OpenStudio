/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP
#define MODEL_COILCOOLINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    /** CoilCoolingLowTempRadiantConstFlow_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingLowTempRadiantConstFlow.*/
    class MODEL_API CoilCoolingLowTempRadiantConstFlow_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingLowTempRadiantConstFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingLowTempRadiantConstFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingLowTempRadiantConstFlow_Impl(const CoilCoolingLowTempRadiantConstFlow_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingLowTempRadiantConstFlow_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> coolingHighWaterTemperatureSchedule() const;

      boost::optional<Schedule> coolingLowWaterTemperatureSchedule() const;

      boost::optional<Schedule> coolingHighControlTemperatureSchedule() const;

      boost::optional<Schedule> coolingLowControlTemperatureSchedule() const;

      std::string condensationControlType() const;

      bool isCondensationControlTypeDefaulted() const;

      double condensationControlDewpointOffset() const;

      bool isCondensationControlDewpointOffsetDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setCoolingHighWaterTemperatureSchedule(Schedule& schedule);

      void resetCoolingHighWaterTemperatureSchedule();

      bool setCoolingLowWaterTemperatureSchedule(Schedule& schedule);

      void resetCoolingLowWaterTemperatureSchedule();

      bool setCoolingHighControlTemperatureSchedule(Schedule& schedule);

      void resetCoolingHighControlTemperatureSchedule();

      bool setCoolingLowControlTemperatureSchedule(Schedule& schedule);

      void resetCoolingLowControlTemperatureSchedule();

      bool setCondensationControlType(const std::string& condensationControlType);

      void resetCondensationControlType();

      bool setCondensationControlDewpointOffset(double condensationControlDewpointOffset);

      void resetCondensationControlDewpointOffset();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingLowTempRadiantConstFlow");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals

      std::vector<std::string> condensationControlTypeValues() const;

      boost::optional<ModelObject> coolingHighWaterTemperatureScheduleAsModelObject() const;
      boost::optional<ModelObject> coolingLowWaterTemperatureScheduleAsModelObject() const;
      boost::optional<ModelObject> coolingHighControlTemperatureScheduleAsModelObject() const;
      boost::optional<ModelObject> coolingLowControlTemperatureScheduleAsModelObject() const;

      bool setCoolingHighWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setCoolingLowWaterTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setCoolingHighControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setCoolingLowControlTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGLOWTEMPRADIANTCONSTFLOW_IMPL_HPP
