/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERFOLLOWSYSTEMNODETEMPERATURE_IMPL_HPP
#define MODEL_SETPOINTMANAGERFOLLOWSYSTEMNODETEMPERATURE_IMPL_HPP

#include "ModelAPI.hpp"
#include "SetpointManager_Impl.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    /** SetpointManagerFollowSystemNodeTemperature_Impl is a SetpointManager_Impl that is the implementation class for SetpointManagerFollowSystemNodeTemperature.*/
    class MODEL_API SetpointManagerFollowSystemNodeTemperature_Impl : public SetpointManager_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SetpointManagerFollowSystemNodeTemperature_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SetpointManagerFollowSystemNodeTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SetpointManagerFollowSystemNodeTemperature_Impl(const SetpointManagerFollowSystemNodeTemperature_Impl& other, Model_Impl* model,
                                                      bool keepHandle);

      virtual ~SetpointManagerFollowSystemNodeTemperature_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      // virtual bool addToNode(Node & node) override;

      virtual bool isAllowedOnPlantLoop() const override;

      //@}
      /** @name Getters */
      //@{

      virtual std::string controlVariable() const override;

      boost::optional<Node> referenceNode() const;

      std::string referenceTemperatureType() const;

      double offsetTemperatureDifference() const;

      double maximumLimitSetpointTemperature() const;

      double minimumLimitSetpointTemperature() const;

      virtual boost::optional<Node> setpointNode() const override;

      //@}
      /** @name Setters */
      //@{

      virtual bool setControlVariable(const std::string& controlVariable) override;

      bool setReferenceNode(const Node& node);

      void resetReferenceNode();

      bool setReferenceTemperatureType(const std::string& referenceTemperatureType);

      bool setOffsetTemperatureDifference(double offsetTemperatureDifference);

      bool setMaximumLimitSetpointTemperature(double maximumLimitSetpointTemperature);

      bool setMinimumLimitSetpointTemperature(double minimumLimitSetpointTemperature);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      virtual bool setSetpointNode(const Node& node) override;

      virtual void resetSetpointNode() override;

      REGISTER_LOGGER("openstudio.model.SetpointManagerFollowSystemNodeTemperature");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERFOLLOWSYSTEMNODETEMPERATURE_IMPL_HPP
