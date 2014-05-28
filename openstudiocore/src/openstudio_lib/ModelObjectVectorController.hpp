/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

