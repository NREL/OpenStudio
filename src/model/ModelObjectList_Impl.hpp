/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MODELOBJECTLIST_IMPL_HPP
#define MODEL_MODELOBJECTLIST_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ModelObjectList_Impl is a ModelObject_Impl that is the implementation class for ModelObjectList.*/
    class MODEL_API ModelObjectList_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ModelObjectList_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ModelObjectList_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ModelObjectList_Impl(const ModelObjectList_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ModelObjectList_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      std::vector<IdfObject> remove() override;

      ModelObject clone(Model model) const override;

      unsigned size() const;

      bool addModelObject(const ModelObject& modelObject);

      bool hasModelObject(const ModelObject& modelObject) const;

      void removeModelObject(const ModelObject& modelObject);

      void removeAllModelObjects();

      std::vector<ModelObject> modelObjects() const;

      WorkspaceExtensibleGroup getGroupForModelObject(const ModelObject& modelObject);

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ModelObjectList");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MODELOBJECTLIST_IMPL_HPP
