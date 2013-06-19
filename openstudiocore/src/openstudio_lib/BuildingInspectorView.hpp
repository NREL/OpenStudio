/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_BUILDINGINSPECTORVIEW_H
#define OPENSTUDIO_BUILDINGINSPECTORVIEW_H

#include <openstudio_lib/ModelObjectInspectorView.hpp>
#include <openstudio_lib/ModelObjectVectorController.hpp>

class QLabel;
class QPushButton;
class QColor;

namespace openstudio {

class OSDropZone;
class OSLineEdit;
class OSComboBox;
class OSQuantityEdit;

namespace model {
  class Building;
}

class BuildingSpaceTypeVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~BuildingSpaceTypeVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);
};

class BuildingDefaultConstructionSetVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~BuildingDefaultConstructionSetVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);
};

class BuildingDefaultScheduleSetVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~BuildingDefaultScheduleSetVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);

  virtual std::vector<OSItemId> makeVector();

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);
};

class BuildingInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    BuildingInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0 );
    virtual ~BuildingInspectorView() {}

  signals:

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void attach(openstudio::model::Building& building);

    void detach();

    void refresh();

    OSLineEdit* m_nameEdit;
    OSComboBox* m_buildingTypeComboBox;
    BuildingSpaceTypeVectorController* m_spaceTypeVectorController;
    OSDropZone* m_spaceTypeDropZone;
    BuildingDefaultConstructionSetVectorController* m_defaultConstructionSetVectorController;
    OSDropZone* m_defaultConstructionSetDropZone;
    BuildingDefaultScheduleSetVectorController* m_defaultScheduleSetVectorController;
    OSDropZone* m_defaultScheduleSetDropZone;
    OSQuantityEdit* m_northAxisEdit;
    //OSQuantityEdit* m_floorToFloorHeightEdit;
    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);
};


} // openstudio

#endif // OPENSTUDIO_BUILDINGSTORYINSPECTORVIEW_H

