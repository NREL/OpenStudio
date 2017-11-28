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

#ifndef OPENSTUDIO_SPACELOADINSTANCESWIDGET_HPP
#define OPENSTUDIO_SPACELOADINSTANCESWIDGET_HPP

#include "ModelObjectVectorController.hpp"

#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement
#include "../model/Building.hpp"
#include "../model/DefaultScheduleSet.hpp"
#include "../model/Model.hpp"
#include "../model/Space.hpp"
#include "../model/SpaceLoadInstance.hpp"
#include "../model/SpaceType.hpp"

#include "../utilities/idf/WorkspaceObject_Impl.hpp"


#include <QWidget>

class QGridLayout;
class QLabel;
class QPushButton;
class QVBoxLayout;

namespace openstudio {

class OSDoubleEdit2;
class OSDropZone;
class OSIntegerEdit;
class OSLineEdit2;

class SpaceLoadInstanceDefinitionVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceLoadInstanceDefinitionVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
};

class SpaceLoadInstanceScheduleVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~SpaceLoadInstanceScheduleVectorController() {}
  virtual void attach(const model::ModelObject& modelObject) override;

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;

private:

  void attachOtherModelObjects(const model::SpaceLoadInstance& spaceLoadInstance);
  void attachOtherModelObjects(const model::Space& space);
  void attachOtherModelObjects(const model::SpaceType& spaceType);
  void attachOtherModelObjects(const model::BuildingStory& buildingStory);
  void attachOtherModelObjects(const model::Building& building);
  void attachOtherModelObjects(const model::DefaultScheduleSet& defaultScheduleSet);
};

class SpaceLoadInstanceActivityScheduleVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~SpaceLoadInstanceActivityScheduleVectorController() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
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
  OSDoubleEdit2* m_multiplierEdit;
  OSLineEdit2* m_nameEdit;
  QLabel* m_activityScheduleLabel;
  QPushButton* m_removeButton;

  SpaceLoadInstanceDefinitionVectorController* m_definitionVectorController;
  OSDropZone* m_definitionDropZone;

  SpaceLoadInstanceScheduleVectorController* m_scheduleVectorController;
  OSDropZone* m_scheduleDropZone;

  SpaceLoadInstanceActivityScheduleVectorController* m_activityScheduleVectorController;
  OSDropZone* m_activityScheduleDropZone;

  model::SpaceLoadInstance m_spaceLoadInstance;
  bool m_isDefault;

  boost::optional<model::SpaceLoadInstance> opt_spaceLoadInstance;

private:
  void createLayout(bool isDefault);
};

class NewSpaceLoadVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~NewSpaceLoadVectorController() {}

protected:
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class SpaceLoadInstancesWidget : public QWidget, public Nano::Observer
{
  Q_OBJECT

public:

  SpaceLoadInstancesWidget(QWidget* parent = nullptr);
  virtual ~SpaceLoadInstancesWidget() {}
  void attach(const model::Space& space);
  void attach(const model::SpaceType& spaceType);
  void detach();

private slots:

  void remove(SpaceLoadInstanceMiniView* spaceLoadInstanceMiniView);

  void onBuildingRelationshipChange(int index, Handle, Handle);

  void onSpaceTypeRelationshipChange(int index, Handle, Handle);

  void onSpaceRelationshipChange(int index, Handle, Handle);

  void objectAdded(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&);

  void objectRemoved(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&);

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

#endif // OPENSTUDIO_SPACELOADINSTANCESWIDGET_HPP

