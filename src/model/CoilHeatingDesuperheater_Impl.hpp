/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGDESUPERHEATER_IMPL_HPP
#define MODEL_COILHEATINGDESUPERHEATER_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class ModelObject;

  namespace detail {

    /** CoilHeatingDesuperheater_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingDesuperheater.*/
    class MODEL_API CoilHeatingDesuperheater_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingDesuperheater_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingDesuperheater_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingDesuperheater_Impl(const CoilHeatingDesuperheater_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingDesuperheater_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual ModelObject clone(Model model) const override;

      virtual bool addToNode(Node& node) override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      double heatReclaimRecoveryEfficiency() const;

      bool isHeatReclaimRecoveryEfficiencyDefaulted() const;

      boost::optional<ModelObject> heatingSource() const;

      double onCycleParasiticElectricLoad() const;

      bool isOnCycleParasiticElectricLoadDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setHeatReclaimRecoveryEfficiency(double heatReclaimRecoveryEfficiency);

      void resetHeatReclaimRecoveryEfficiency();

      bool setHeatingSource(const boost::optional<ModelObject>& modelObject);

      void resetHeatingSource();

      bool setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad);

      void resetOnCycleParasiticElectricLoad();

      //@}
      /** @name Other */
      //@{

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingDesuperheater");

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGDESUPERHEATER_IMPL_HPP
