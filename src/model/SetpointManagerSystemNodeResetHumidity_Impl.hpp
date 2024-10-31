/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSYSTEMNODERESETHUMIDITY_IMPL_HPP
#define MODEL_SETPOINTMANAGERSYSTEMNODERESETHUMIDITY_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** SetpointManagerSystemNodeResetHumidity_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerSystemNodeResetHumidity.*/
    class MODEL_API SetpointManagerSystemNodeResetHumidity_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerSystemNodeResetHumidity_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerSystemNodeResetHumidity_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerSystemNodeResetHumidity_Impl(const SetpointManagerSystemNodeResetHumidity_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SetpointManagerSystemNodeResetHumidity_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual bool isAllowedOnPlantLoop() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::string controlVariable() const override;

      virtual bool setControlVariable(const std::string& controlVariable) override;

      virtual boost::optional<Node> setpointNode() const override;

      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;

      //@}
      /** @name Getters */
      //@{

      double setpointatLowReferenceHumidityRatio() const;

      double setpointatHighReferenceHumidityRatio() const;

      double lowReferenceHumidityRatio() const;

      double highReferenceHumidityRatio() const;

      boost::optional<Node> referenceNode() const;

      //@}
      /** @name Setters */
      //@{

      bool setSetpointatLowReferenceHumidityRatio(double setpointatLowReferenceHumidityRatio);

      bool setSetpointatHighReferenceHumidityRatio(double setpointatHighReferenceHumidityRatio);

      bool setLowReferenceHumidityRatio(double lowReferenceHumidityRatio);

      bool setHighReferenceHumidityRatio(double highReferenceHumidityRatio);

      bool setReferenceNode(const Node& node);
      void resetReferenceNode();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SetpointManagerSystemNodeResetHumidity");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSYSTEMNODERESETHUMIDITY_IMPL_HPP
