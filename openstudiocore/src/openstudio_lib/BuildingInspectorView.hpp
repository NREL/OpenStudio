/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_BUILDINGINSPECTORVIEW_HPP
#define OPENSTUDIO_BUILDINGINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"

#include "../model/Building.hpp"

class QColor;
class QComboBox;
class QLabel;
class QPushButton;

namespace openstudio {

class OSComboBox;
class OSDropZone;
class OSIntegerEdit2;
class OSLineEdit;
class OSQuantityEdit2;
class OSQuantityEdit;
class OSSwitch2;

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

  protected slots:

    void editStandardsBuildingType(const QString & text);
    void standardsBuildingTypeChanged(const QString & text);

  private:

    void attach(openstudio::model::Building& building);

    void detach();

    void populateStandardsBuildingTypes();

    boost::optional<openstudio::model::Building> m_building;

    OSLineEdit* m_nameEdit;
    QComboBox* m_standardsBuildingTypeComboBox;
    BuildingSpaceTypeVectorController* m_spaceTypeVectorController;
    OSDropZone* m_spaceTypeDropZone;
    BuildingDefaultConstructionSetVectorController* m_defaultConstructionSetVectorController;
    OSDropZone* m_defaultConstructionSetDropZone;
    BuildingDefaultScheduleSetVectorController* m_defaultScheduleSetVectorController;
    OSDropZone* m_defaultScheduleSetDropZone;
    OSQuantityEdit* m_northAxisEdit;
    OSIntegerEdit2* m_numberLivingUnits;
    OSIntegerEdit2* m_numberStories;
    OSIntegerEdit2* m_numberAboveGroundStories;
    OSSwitch2* m_relocatable;
    OSQuantityEdit2 * m_floorToCeilingHeight = nullptr;
    OSQuantityEdit2 * m_floorToFloorHeight = nullptr;
    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);
};


} // openstudio

#endif // OPENSTUDIO_BUILDINGSTORYINSPECTORVIEW_HPP

