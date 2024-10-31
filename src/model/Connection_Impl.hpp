/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONNECTION_IMPL_HPP
#define MODEL_CONNECTION_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API Connection_Impl : public ModelObject_Impl
    {
     public:
      Connection_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Connection_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Connection_Impl(const Connection_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~Connection_Impl() override = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      boost::optional<ModelObject> sourceObject() const;

      boost::optional<unsigned> sourceObjectPort() const;

      boost::optional<ModelObject> targetObject() const;

      boost::optional<unsigned> targetObjectPort() const;

      bool setSourceObject(const ModelObject& object);

      bool setSourceObjectPort(unsigned port);

      bool setTargetObject(const ModelObject& object);

      bool setTargetObjectPort(unsigned port);

     private:
      REGISTER_LOGGER("openstudio.model.Connection");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONNECTION_IMPL_HPP
