/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERMIXEDAIR_IMPL_HPP
#define MODEL_SETPOINTMANAGERMIXEDAIR_IMPL_HPP

#include "SetpointManager_Impl.hpp"
#include <boost/optional.hpp>

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API SetpointManagerMixedAir_Impl : public SetpointManager_Impl
    {

     public:
      SetpointManagerMixedAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerMixedAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerMixedAir_Impl(const SetpointManagerMixedAir_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~SetpointManagerMixedAir_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual bool addToNode(Node& node) override;

      virtual std::string controlVariable() const override;

      virtual bool setControlVariable(const std::string& controlVariable) override;

      boost::optional<Node> referenceSetpointNode();

      bool setReferenceSetpointNode(Node& node);

      void resetReferenceSetpointNode();

      boost::optional<Node> fanInletNode();

      bool setFanInletNode(Node& node);

      void resetFanInletNode();

      boost::optional<Node> fanOutletNode();

      bool setFanOutletNode(Node& node);

      void resetFanOutletNode();

      virtual boost::optional<Node> setpointNode() const override;

     private:
      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;

      REGISTER_LOGGER("openstudio.model.SetpointManagerMixedAir");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERMIXEDAIR_IMPL_HPP
