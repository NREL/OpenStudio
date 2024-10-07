/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGELECTRICMULTISTAGE_IMPL_HPP
#define MODEL_COILHEATINGELECTRICMULTISTAGE_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class CoilHeatingElectricMultiStageStageData;

  namespace detail {

    /** CoilHeatingElectricMultiStage_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingElectricMultiStage.*/
    class MODEL_API CoilHeatingElectricMultiStage_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingElectricMultiStage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingElectricMultiStage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingElectricMultiStage_Impl(const CoilHeatingElectricMultiStage_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingElectricMultiStage_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      ModelObject clone(Model model) const override;

      std::vector<ModelObject> children() const override;

      boost::optional<HVACComponent> containingHVACComponent() const override;

      bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      //@}
      /** @name Other */
      //@{

      std::vector<CoilHeatingElectricMultiStageStageData> stages() const;
      unsigned numberOfStages() const;
      boost::optional<unsigned> stageIndex(const CoilHeatingElectricMultiStageStageData& stage) const;

      bool addStage(const CoilHeatingElectricMultiStageStageData& stage);
      bool addStage(const CoilHeatingElectricMultiStageStageData& stage, unsigned index);
      bool setStageIndex(const CoilHeatingElectricMultiStageStageData& stage, unsigned index);
      bool setStages(const std::vector<CoilHeatingElectricMultiStageStageData>& stages);
      void removeAllStages();
      bool removeStage(const CoilHeatingElectricMultiStageStageData& stage);
      bool removeStage(unsigned index);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingElectricMultiStage");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGELECTRICMULTISTAGE_IMPL_HPP
