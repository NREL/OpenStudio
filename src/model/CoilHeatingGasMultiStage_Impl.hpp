/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGGASMULTISTAGE_IMPL_HPP
#define MODEL_COILHEATINGGASMULTISTAGE_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;
  class CoilHeatingGasMultiStageStageData;

  namespace detail {

    /** CoilHeatingGasMultiStage_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingGasMultiStage.*/
    class MODEL_API CoilHeatingGasMultiStage_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingGasMultiStage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingGasMultiStage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingGasMultiStage_Impl(const CoilHeatingGasMultiStage_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingGasMultiStage_Impl() override = default;

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

      boost::optional<Curve> partLoadFractionCorrelationCurve() const;

      boost::optional<double> offCycleParasiticGasLoad() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setPartLoadFractionCorrelationCurve(const boost::optional<Curve>& curve);

      void resetPartLoadFractionCorrelationCurve();

      bool setOffCycleParasiticGasLoad(double offCycleParasiticGasLoad);

      void resetOffCycleParasiticGasLoad();

      //@}
      /** @name Other */
      //@{

      std::vector<CoilHeatingGasMultiStageStageData> stages() const;
      unsigned numberOfStages() const;
      boost::optional<unsigned> stageIndex(const CoilHeatingGasMultiStageStageData& stage) const;

      bool addStage(const CoilHeatingGasMultiStageStageData& stage);
      bool addStage(const CoilHeatingGasMultiStageStageData& stage, unsigned index);
      bool setStageIndex(const CoilHeatingGasMultiStageStageData& stage, unsigned index);
      bool setStages(const std::vector<CoilHeatingGasMultiStageStageData>& stages);
      void removeAllStages();
      bool removeStage(const CoilHeatingGasMultiStageStageData& stage);
      bool removeStage(unsigned index);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingGasMultiStage");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGGASMULTISTAGE_IMPL_HPP
