/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGEROUTDOORAIRPRETREAT_IMPL_HPP
#define MODEL_SETPOINTMANAGEROUTDOORAIRPRETREAT_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** SetpointManagerOutdoorAirPretreat_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerOutdoorAirPretreat.*/
    class MODEL_API SetpointManagerOutdoorAirPretreat_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerOutdoorAirPretreat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerOutdoorAirPretreat_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerOutdoorAirPretreat_Impl(const SetpointManagerOutdoorAirPretreat_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SetpointManagerOutdoorAirPretreat_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      virtual std::string controlVariable() const override;

      double minimumSetpointTemperature() const;

      bool isMinimumSetpointTemperatureDefaulted() const;

      double maximumSetpointTemperature() const;

      bool isMaximumSetpointTemperatureDefaulted() const;

      double minimumSetpointHumidityRatio() const;

      bool isMinimumSetpointHumidityRatioDefaulted() const;

      double maximumSetpointHumidityRatio() const;

      bool isMaximumSetpointHumidityRatioDefaulted() const;

      boost::optional<Node> referenceSetpointNode() const;

      boost::optional<Node> mixedAirStreamNode() const;

      boost::optional<Node> outdoorAirStreamNode() const;

      boost::optional<Node> returnAirStreamNode() const;

      virtual boost::optional<Node> setpointNode() const override;

      //@}
      /** @name Setters */
      //@{

      virtual bool setControlVariable(const std::string& controlVariable) override;

      void resetControlVariable();

      bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

      void resetMinimumSetpointTemperature();

      bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

      void resetMaximumSetpointTemperature();

      bool setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio);

      void resetMinimumSetpointHumidityRatio();

      bool setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio);

      void resetMaximumSetpointHumidityRatio();

      bool setReferenceSetpointNode(const Node& node);

      void resetReferenceSetpointNode();

      bool setMixedAirStreamNode(const Node& node);

      void resetMixedAirStreamNode();

      bool setOutdoorAirStreamNode(const Node& node);

      void resetOutdoorAirStreamNode();

      bool setReturnAirStreamNode(const Node& node);

      void resetReturnAirStreamNode();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;

      REGISTER_LOGGER("openstudio.model.SetpointManagerOutdoorAirPretreat");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGEROUTDOORAIRPRETREAT_IMPL_HPP
