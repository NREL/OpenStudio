/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSYSTEMNODERESETTEMPERATURE_IMPL_HPP
#define MODEL_SETPOINTMANAGERSYSTEMNODERESETTEMPERATURE_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** SetpointManagerSystemNodeResetTemperature_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerSystemNodeResetTemperature.*/
    class MODEL_API SetpointManagerSystemNodeResetTemperature_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerSystemNodeResetTemperature_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerSystemNodeResetTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerSystemNodeResetTemperature_Impl(const SetpointManagerSystemNodeResetTemperature_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SetpointManagerSystemNodeResetTemperature_Impl() override = default;

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

      double setpointatLowReferenceTemperature() const;

      double setpointatHighReferenceTemperature() const;

      double lowReferenceTemperature() const;

      double highReferenceTemperature() const;

      boost::optional<Node> referenceNode() const;

      //@}
      /** @name Setters */
      //@{

      bool setSetpointatLowReferenceTemperature(double setpointatLowReferenceTemperature);

      bool setSetpointatHighReferenceTemperature(double setpointatHighReferenceTemperature);

      bool setLowReferenceTemperature(double lowReferenceTemperature);

      bool setHighReferenceTemperature(double highReferenceTemperature);

      bool setReferenceNode(const Node& node);

      void resetReferenceNode();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SetpointManagerSystemNodeResetTemperature");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSYSTEMNODERESETTEMPERATURE_IMPL_HPP
