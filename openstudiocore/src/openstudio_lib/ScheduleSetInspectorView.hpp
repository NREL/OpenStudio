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

#ifndef OPENSTUDIO_SCHEDULESETINSPECTORVIEW_HPP
#define OPENSTUDIO_SCHEDULESETINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"

#include "../model/DefaultScheduleSet.hpp"
#include "../model/Model.hpp"

namespace openstudio {

class OSDropZone;
class OSLineEdit;

class HoursOfOperationVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~HoursOfOperationVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class NumberOfPeopleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~NumberOfPeopleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class PeopleActivityScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~PeopleActivityScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class LightingScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~LightingScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class ElectricEquipmentScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~ElectricEquipmentScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class GasEquipmentScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~GasEquipmentScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class HotWaterEquipmentScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~HotWaterEquipmentScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class InfiltrationScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~InfiltrationScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class SteamEquipmentScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~SteamEquipmentScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class OtherEquipmentScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~OtherEquipmentScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);
};

class ScheduleSetInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:
    ScheduleSetInspectorView(const model::Model& model,
      QWidget * parent = 0);
    virtual ~ScheduleSetInspectorView() {}

  protected:
    virtual void onClearSelection();
    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);
    virtual void onUpdate();

  private:
    void attach(openstudio::model::DefaultScheduleSet& scheduleSet);
    void detach();
    void configDropZones();

    OSLineEdit * m_nameEdit;

    OSDropZone * m_hoursOfOperationDZ;
    OSDropZone * m_numberOfPeopleDZ;
    OSDropZone * m_peopleActivityScheduleDZ;
    OSDropZone * m_lightingScheduleDZ;
    OSDropZone * m_electricEquipmentScheduleDZ;
    OSDropZone * m_gasEquipmentScheduleDZ;
    OSDropZone * m_hotWaterEquipmentScheduleDZ;
    OSDropZone * m_infiltrationScheduleDZ;
    OSDropZone * m_ventilationPerPersonScheduleDZ;
    OSDropZone * m_ventilationPerFloorAreaScheduleDZ;

    HoursOfOperationVC * m_hoursOfOperationVC;
    NumberOfPeopleVC * m_numberOfPeopleVC;
    PeopleActivityScheduleVC * m_peopleActivityScheduleVC;
    LightingScheduleVC * m_lightingScheduleVC;
    ElectricEquipmentScheduleVC * m_electricEquipmentScheduleVC;
    GasEquipmentScheduleVC * m_gasEquipmentScheduleVC;
    HotWaterEquipmentScheduleVC * m_hotWaterEquipmentScheduleVC;
    InfiltrationScheduleVC * m_infiltrationScheduleVC;
    SteamEquipmentScheduleVC * m_steamEquipmentScheduleVC;
    OtherEquipmentScheduleVC * m_otherEquipmentScheduleVC;

    std::vector<ModelObjectVectorController *> m_vectorControllers;
    std::vector<OSDropZone *> m_dropZones;
};

} // openstudio

#endif // OPENSTUDIO_SCHEDULESETINSPECTORVIEW_HPP
