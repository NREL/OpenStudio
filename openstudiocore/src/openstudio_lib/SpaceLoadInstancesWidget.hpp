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

#ifndef OPENSTUDIO_SPACELOADINSTANCESWIDGET_H
#define OPENSTUDIO_SPACELOADINSTANCESWIDGET_H

#include <openstudio_lib/ModelObjectVectorController.hpp>

#include <model/Model.hpp>
#include <model/Space.hpp>
#include <model/SpaceType.hpp>
#include <model/Building.hpp>
#include <model/DefaultScheduleSet.hpp>
#include <model/SpaceLoadInstance.hpp>

#include <QWidget>

#include <boost/shared_ptr.hpp>

class QPushButton;
class QVBoxLayout;

namespace openstudio {

class OSLineEdit;
class OSDoubleEdit;
class OSIntegerEdit;
class OSDropZone;

class SpaceLoadInstanceDefinitionVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceLoadInstanceDefinitionVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
};

class SpaceLoadInstanceScheduleVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceLoadInstanceScheduleVectorController() {}
  virtual void attach(const model::ModelObject& modelObject);

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);

private:

  void attachOtherModelObjects(const model::SpaceLoadInstance& spaceLoadInstance);
  void attachOtherModelObjects(const model::Space& space);
  void attachOtherModelObjects(const model::SpaceType& spaceType);
  void attachOtherModelObjects(const model::BuildingStory& buildingStory);
  void attachOtherModelObjects(const model::Building& building);
  void attachOtherModelObjects(const model::DefaultScheduleSet& defaultScheduleSet);
};

class SpaceLoadInstanceMiniView : public QWidget
{
  Q_OBJECT

public:
  SpaceLoadInstanceMiniView(const model::SpaceLoadInstance& spaceLoadInstance, bool isDefault);
  virtual ~SpaceLoadInstanceMiniView() {}
  model::SpaceLoadInstance spaceLoadInstance() const;
  bool isDefault() const;

signals:
  void removeClicked(SpaceLoadInstanceMiniView* spaceLoadInstanceMiniView);

private slots:
  void onRemoveClicked();

private:
  OSDoubleEdit* m_multiplierEdit;
  OSLineEdit* m_nameEdit;
  QPushButton* m_removeButton;
  SpaceLoadInstanceDefinitionVectorController* m_definitionVectorController;
  OSDropZone* m_definitionDropZone;
  SpaceLoadInstanceScheduleVectorController* m_scheduleVectorController;
  OSDropZone* m_scheduleDropZone;

  model::SpaceLoadInstance m_spaceLoadInstance;
  bool m_isDefault;
};

class NewSpaceLoadVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~NewSpaceLoadVectorController() {}

protected:
  virtual std::vector<OSItemId> makeVector();
  virtual void onDrop(const OSItemId& itemId);
};

class SpaceLoadInstancesWidget : public QWidget
{
  Q_OBJECT

public:

  SpaceLoadInstancesWidget(QWidget* parent = NULL);
  virtual ~SpaceLoadInstancesWidget() {}
  void attach(const model::Space& space);
  void attach(const model::SpaceType& spaceType);
  void detach();

private slots:

  void remove(SpaceLoadInstanceMiniView* spaceLoadInstanceMiniView);

  void onBuildingRelationshipChange(int index, Handle, Handle);

  void onSpaceTypeRelationshipChange(int index, Handle, Handle);

  void onSpaceRelationshipChange(int index, Handle, Handle);

  void objectAdded(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&);

  void objectRemoved(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&);
  
  void refresh();

private:

  void addSpaceLoads(const model::Space& space);

  void addSpaceTypeLoads(const model::SpaceType& spaceType, bool addAsDefaultLoads);

  void addSpaceLoadInstance(const model::SpaceLoadInstance& spaceLoadInstance, bool isDefault);

  QVBoxLayout* m_mainVLayout;

  NewSpaceLoadVectorController* m_newSpaceLoadVectorController;
  OSDropZone* m_newSpaceLoadDropZone;

  boost::optional<model::Space> m_space;
  boost::optional<model::SpaceType> m_spaceType;
  boost::optional<model::Model> m_model;

  bool m_dirty;
};

} // openstudio

#endif // OPENSTUDIO_SPACELOADINSTANCESWIDGET_H

