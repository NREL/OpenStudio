/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACE_IMPL_HPP
#define MODEL_EXTERNALINTERFACE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ExternalInterface_Impl is a ModelObject_Impl that is the implementation class for ExternalInterface.*/
    class MODEL_API ExternalInterface_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ExternalInterface_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExternalInterface_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ExternalInterface_Impl(const ExternalInterface_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ExternalInterface_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string nameofExternalInterface() const;

      //@}
      /** @name Setters */
      //@{

      bool setNameofExternalInterface(const std::string& nameofExternalInterface);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ExternalInterface");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACE_IMPL_HPP
