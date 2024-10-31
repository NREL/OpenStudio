/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONTROLLERWATERCOIL_HPP
#define MODEL_CONTROLLERWATERCOIL_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class ControllerWaterCoil_Impl;
    class CoilCoolingWater_Impl;
    class CoilHeatingWater_Impl;
    class CoilSystemCoolingWaterHeatExchangerAssisted_Impl;

  }  // namespace detail

  /** ControllerWaterCoil is a HVACComponent that wraps the OpenStudio IDD object
 *  'OS:Controller:WaterCoil'. */
  class MODEL_API ControllerWaterCoil : public HVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ControllerWaterCoil() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ControllerWaterCoil(const ControllerWaterCoil& other) = default;
    ControllerWaterCoil(ControllerWaterCoil&& other) = default;
    ControllerWaterCoil& operator=(const ControllerWaterCoil&) = default;
    ControllerWaterCoil& operator=(ControllerWaterCoil&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    static std::vector<std::string> validControlVariableValues();

    static std::vector<std::string> validActionValues();

    static std::vector<std::string> validActuatorVariableValues();

    //@}
    /** @name Getters */
    //@{

    boost::optional<HVACComponent> waterCoil() const;

    boost::optional<std::string> controlVariable() const;  // Shouldn't return an optional, has a default
    bool isControlVariableDefaulted() const;

    boost::optional<std::string> action() const;  // Has no default and is required...

    boost::optional<std::string> actuatorVariable() const;  // Shouldn't return an optional, has a default
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
    /// @cond
    using ImplType = detail::ControllerWaterCoil_Impl;

    friend class detail::ControllerWaterCoil_Impl;

    friend class Model;

    friend class openstudio::IdfObject;

    // Classes that need to instantiate it
    friend class openstudio::model::detail::CoilCoolingWater_Impl;
    friend class openstudio::model::detail::CoilHeatingWater_Impl;
    friend class openstudio::model::detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl;

    explicit ControllerWaterCoil(const Model& model);

    explicit ControllerWaterCoil(std::shared_ptr<detail::ControllerWaterCoil_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ControllerWaterCoil");
  };

  /** \relates ControllerWaterCoil*/
  using OptionalControllerWaterCoil = boost::optional<ControllerWaterCoil>;

  /** \relates ControllerWaterCoil*/
  using ControllerWaterCoilVector = std::vector<ControllerWaterCoil>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_CONTROLLERWATERCOIL_HPP
