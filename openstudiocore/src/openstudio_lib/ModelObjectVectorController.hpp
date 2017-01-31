/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef OPENSTUDIO_MODELOBJECTVECTORCONTROLLER_HPP
#define OPENSTUDIO_MODELOBJECTVECTORCONTROLLER_HPP

#include "OSVectorController.hpp"
#include "../model/ModelObject.hpp"
#include "../model/Component.hpp"
#include <vector>

namespace openstudio {

class ModelObjectVectorController : public OSVectorController
{
  Q_OBJECT

public:

  virtual ~ModelObjectVectorController() {}

  virtual void attach(const model::ModelObject& modelObject);

  virtual void attachModel(const model::Model& model);

  void detach();

protected slots:

  void objectAdded(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

  void objectRemoved(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

  void changeRelationship(int index, Handle newHandle, Handle oldHandle);

  void dataChange();

  void change();

protected:

  void attachOtherModelObject(const model::ModelObject& modelObject);

  void detachOtherModelObject(const model::ModelObject& modelObject);

  void detachOtherModelObjects();

  virtual void onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

  virtual void onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

  virtual void onChangeRelationship(const openstudio::model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual void onDataChange(const openstudio::model::ModelObject& modelObject);

  virtual void onChange(const openstudio::model::ModelObject& modelObject);

  bool fromModel(const OSItemId& itemId) const;

  bool fromComponentLibrary(const OSItemId& itemId) const;

  boost::optional<model::ModelObject> getModelObject(const OSItemId& itemId) const;

  boost::optional<model::Component> getComponent(const OSItemId& itemId) const;

  template <class ModelObjectType>
  boost::optional<ModelObjectType> addToModel(const OSItemId& itemId) {

    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<ModelObjectType>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        return modelObject->cast<ModelObjectType>();
      }
    }else{
      boost::optional<model::Component> component = this->getComponent(itemId);
      if (component){
        if (component->primaryObject().optionalCast<ModelObjectType>()){
          boost::optional<model::ComponentData> componentData = m_modelObject->model().insertComponent(*component);
          if (componentData){
            modelObject = componentData->primaryComponentObject();
            return modelObject->cast<ModelObjectType>();
          }
        }
      }
    }
    return boost::none;
  }

  boost::optional<model::ModelObject> m_modelObject;
  boost::optional<model::Model> m_model;
  std::vector<model::ModelObject> m_otherModelObjects;
};

} // openstudio

#endif // OPENSTUDIO_MODELOBJECTVECTORCONTROLLER_HPP

