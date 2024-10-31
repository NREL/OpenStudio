/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONTROLLERWATERCOIL_IMPL_HPP
#define MODEL_CONTROLLERWATERCOIL_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** ControllerWaterCoil_Impl is a HVACComponent_Impl that is the implementation class for ControllerWaterCoil.*/
    class MODEL_API ControllerWaterCoil_Impl : public HVACComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ControllerWaterCoil_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ControllerWaterCoil_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ControllerWaterCoil_Impl(const ControllerWaterCoil_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ControllerWaterCoil_Impl() override = default;

      //@}
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<HVACComponent> waterCoil() const;

      std::string controlVariable() const;
      bool isControlVariableDefaulted() const;

      boost::optional<std::string> action() const;

      std::string actuatorVariable() const;
      bool isActuatorVariableDefaulted() const;

      boost::optional<Node> sensorNode() const;

      boost::optional<Node> actuatorNode() const;

      boost::optional<double> controllerConvergenceTolerance() const;

      bool isControllerConvergenceToleranceDefaulted() const;

      bool isControllerConvergenceToleranceAutosized() const;

      boost::optional<double> maximumActuatedFlow() const;

      bool isMaximumActuatedFlowAutosized() const;

      double minimumActuatedFlow() const;

      bool isMinimumActuatedFlowDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setWaterCoil(const HVACComponent& comp);

      bool setControlVariable(const std::string& controlVariable);

      void resetControlVariable();

      bool setAction(const std::string& action);

      void resetAction();

      bool setActuatorVariable(const std::string& actuatorVariable);

      void resetActuatorVariable();

      bool setSensorNode(const Node& node);

      bool setActuatorNode(const Node& node);

      bool setControllerConvergenceTolerance(double controllerConvergenceTolerance);

      void resetControllerConvergenceTolerance();

      void autosizeControllerConvergenceTolerance();

      bool setMaximumActuatedFlow(double maximumActuatedFlow);

      void resetMaximumActuatedFlow();

      void autosizeMaximumActuatedFlow();

      bool setMinimumActuatedFlow(double minimumActuatedFlow);

      void resetMinimumActuatedFlow();

      //@}
      /** @name Other */
      //@{

      boost::optional<double> autosizedControllerConvergenceTolerance() const;

      boost::optional<double> autosizedMaximumActuatedFlow() const;

      //@}

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ControllerWaterCoil");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_CONTROLLERWATERCOIL_IMPL_HPP
