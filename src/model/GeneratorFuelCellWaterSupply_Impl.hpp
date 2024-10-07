/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GENERATORFUELCELLWATERSUPPLY_IMPL_HPP
#define MODEL_GENERATORFUELCELLWATERSUPPLY_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class CurveQuadratic;
  class CurveCubic;
  class Node;
  class Schedule;
  class GeneratorFuelCell;

  namespace detail {

    /** GeneratorFuelCellWaterSupply_Impl is a ModelObject_Impl that is the implementation class for GeneratorFuelCellWaterSupply.*/
    class MODEL_API GeneratorFuelCellWaterSupply_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      GeneratorFuelCellWaterSupply_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellWaterSupply_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GeneratorFuelCellWaterSupply_Impl(const GeneratorFuelCellWaterSupply_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GeneratorFuelCellWaterSupply_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const;

      virtual std::vector<ModelObject> children() const;

      //@}
      /** @name Getters */
      //@{

      CurveQuadratic reformerWaterFlowRateFunctionofFuelRateCurve() const;

      CurveCubic reformerWaterPumpPowerFunctionofFuelRateCurve() const;

      double pumpHeatLossFactor() const;

      std::string waterTemperatureModelingMode() const;

      boost::optional<Node> waterTemperatureReferenceNode() const;

      boost::optional<Schedule> waterTemperatureSchedule() const;

      // Return optional parent generator
      boost::optional<GeneratorFuelCell> fuelCell() const;

      //@}
      /** @name Setters */
      //@{

      bool setReformerWaterFlowRateFunctionofFuelRateCurve(const CurveQuadratic& quadraticCurves);

      bool setReformerWaterPumpPowerFunctionofFuelRateCurve(const CurveCubic& cubicCurves);

      bool setPumpHeatLossFactor(double pumpHeatLossFactor);

      void resetPumpHeatLossFactor();

      bool setWaterTemperatureModelingMode(const std::string& waterTemperatureModelingMode);

      void resetWaterTemperatureModelingMode();

      bool setWaterTemperatureReferenceNode(const Node& connection);

      void resetWaterTemperatureReferenceNode();

      bool setWaterTemperatureSchedule(Schedule& schedule);

      void resetWaterTemperatureSchedule();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.GeneratorFuelCellWaterSupply");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATORFUELCELLWATERSUPPLY_IMPL_HPP
