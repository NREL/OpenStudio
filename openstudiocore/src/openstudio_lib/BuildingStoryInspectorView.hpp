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

#ifndef OPENSTUDIO_BUILDINGSTORYINSPECTORVIEW_HPP
#define OPENSTUDIO_BUILDINGSTORYINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"

#include "../model/BuildingStory.hpp"

class QLabel;
class QPushButton;
class QColor;

namespace openstudio {

class OSDropZone;
class OSLineEdit;
class OSQuantityEdit;
class RenderingColorWidget;

class BuildingStoryDefaultConstructionSetVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~BuildingStoryDefaultConstructionSetVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);
};

class BuildingStoryDefaultScheduleSetVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~BuildingStoryDefaultScheduleSetVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);
};

class BuildingStorySpacesVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~BuildingStorySpacesVectorController() {}
  
  virtual void attach(const model::ModelObject& modelObject);

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);
  
  virtual void onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);
  
  virtual void onRemoveItem(OSItem* item);

  virtual void onDrop(const OSItemId& itemId);
};

class BuildingStoryUnassignedSpacesVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~BuildingStoryUnassignedSpacesVectorController() {}

  virtual void attachModel(const model::Model& model);

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

  virtual void onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);
};


class BuildingStoryInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    BuildingStoryInspectorView(const openstudio::model::Model& model, QWidget * parent = 0 );
    
    virtual ~BuildingStoryInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void attach(openstudio::model::BuildingStory& buildingStory);

    void detach();

    OSLineEdit* m_nameEdit;
    //OSQuantityEdit* m_floorHeightEdit;
    //OSQuantityEdit* m_floorToFloorHeightEdit;
    BuildingStoryDefaultConstructionSetVectorController* m_defaultConstructionSetVectorController;
    OSDropZone* m_defaultConstructionSetDropZone;
    BuildingStoryDefaultScheduleSetVectorController* m_defaultScheduleSetVectorController;
    OSDropZone* m_defaultScheduleSetDropZone;
    RenderingColorWidget* m_renderingColorWidget;
    //BuildingStorySpacesVectorController* m_spacesVectorController;
    //OSDropZone* m_spacesDropZone;

  public slots:

    void toggleUnits(bool displayIP);
};


} // openstudio

#endif // OPENSTUDIO_BUILDINGSTORYINSPECTORVIEW_HPP

