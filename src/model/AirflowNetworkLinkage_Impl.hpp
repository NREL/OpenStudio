/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKLINKAGE_IMPL_HPP
#define MODEL_AIRFLOWNETWORKLINKAGE_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API AirflowNetworkLinkage_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirflowNetworkLinkage_Impl(IddObjectType type, Model_Impl* model);

      AirflowNetworkLinkage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirflowNetworkLinkage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirflowNetworkLinkage_Impl(const AirflowNetworkLinkage_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~AirflowNetworkLinkage_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      //@}
      /** @name Getters */
      //@{

      //@}
      /** @name Setters */
      //@{

      //@}
      /** @name Other */
      //@{

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.AirflowNetworkLinkage");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKLINKAGE_IMPL_HPP
