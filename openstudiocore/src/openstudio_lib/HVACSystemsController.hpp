/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef OPENSTUDIO_HVACSYSTEMSCONTROLLER_HPP
#define OPENSTUDIO_HVACSYSTEMSCONTROLLER_HPP

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include "../model/Loop.hpp"
#include "OSDropZone.hpp"
#include "ModelObjectItem.hpp"
#include "ModelObjectVectorController.hpp"
#include <boost/smart_ptr.hpp>
#include "SOConstants.hpp"
#include "../shared_gui_components/OSQObjectController.hpp"
#include "OSItem.hpp"
#include "../shared_gui_components/OSComboBox.hpp"
#include <QAbstractListModel>
#include <QPointer>
#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement
#include "../model_editor/QMetaTypes.hpp"

class QMutex;

namespace openstudio {

namespace model {

class HVACComponent;
class AirLoopHVAC;
class SetpointManagerScheduled;
class PlantLoop;
class AvailabilityManagerAssignmentList;

}

class LoopView;
class HVACSystemsView;
class HVACAirLoopControlsView;
class HVACPlantLoopControlsView;
class LoopListModel;
class HVACGraphicsView;
class HVACLayoutController;
class HVACControlsController;
class MechanicalVentilationView;
class SingleZoneReheatSPMView;
class NoSupplyAirTempControlView;
class NoMechanicalVentilationView;
class ScheduledSPMView;
class SystemAvailabilityVectorController;
class AvailabilityManagerObjectVectorController;
class OSDropZone;
class FollowOATempSPMView;
class OAResetSPMView;
class AirLoopHVACUnitaryHeatPumpAirToAirControlView;
class NoControlsView;
class RefrigerationController;
class RefrigerationGridController;
class RefrigerationGridView;
class VRFController;

class HVACSystemsController : public QObject, public Nano::Observer
{
  Q_OBJECT

  public:

  enum SceneType {TOPOLOGY, CONTROLS};

  HVACSystemsController(bool isIP, const model::Model & model);

  virtual ~HVACSystemsController();

  HVACSystemsView * hvacSystemsView() const;

  std::shared_ptr<HVACLayoutController> hvacLayoutController() const;

  std::shared_ptr<HVACControlsController> hvacControlsController() const;

  model::Model model() const;

  // Indicates what object or system to display
  // This handle may correspond to a Loop, a ModelObject, such as a WaterUseConnection object,
  // or it may be NULL when displaying the service hot water overview.
  QString currentHandle() const;

  void setCurrentHandle(const QString & handle);

  boost::optional<model::Loop> currentLoop() const;

  void clearSceneSelection();

  public slots:

  void updateLater();

  // void dropZoneItemSelected(OSItem* item);

  private slots:

  void update();

  // Trigger repopulation of the SystemComboBox only
  // Will be connected to Plant/AirLoop name IdfObject name change and also called by (full) update
  void repopulateSystemComboBox();

  void addToModel(AddToModelEnum addToModelEnum);

  void onAddSystemClicked();

  void onCopySystemClicked();

  void onRemoveLoopClicked();

  void onShowTopologyClicked();

  void onShowControlsClicked();

  void onShowGridClicked();

  void onObjectAdded(const WorkspaceObject&, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);

  void onObjectRemoved(const WorkspaceObject&, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);

  void onObjectChanged();

  void onSystemComboBoxIndexChanged(int i);

  void toggleUnits(bool displayIP);

  private:

  REGISTER_LOGGER("openstudio.openstudio_lib.HVACSystemsController");

  std::vector<IddObjectType> systemComboBoxTypes() const;

  QPointer<HVACSystemsView> m_hvacSystemsView;

  std::shared_ptr<HVACLayoutController> m_hvacLayoutController;

  std::shared_ptr<HVACControlsController> m_hvacControlsController;

  std::shared_ptr<RefrigerationController> m_refrigerationController;

  std::shared_ptr<RefrigerationGridController> m_refrigerationGridController;

  std::shared_ptr<VRFController> m_vrfController;

  QString m_currentHandle;

  bool m_dirty;

  QMutex * m_updateMutex;

  model::Model m_model;

  bool m_isIP;

  signals:

  void toggleUnitsClicked(bool displayIP);

};

class HVACControlsController : public QObject, public Nano::Observer
{
  Q_OBJECT;

  public:

  HVACControlsController(HVACSystemsController * hvacSystemsController);

  virtual ~HVACControlsController();

  HVACAirLoopControlsView * hvacAirLoopControlsView() const;

  HVACPlantLoopControlsView * hvacPlantLoopControlsView() const;

  NoControlsView * noControlsView() const;

  boost::optional<model::AirLoopHVAC> airLoopHVAC() const;

  boost::optional<model::PlantLoop> plantLoop() const;

  public slots:

  void updateLater();

  private slots:

  void update();

  void onEconomizerComboBoxIndexChanged(int index);

  void onVentilationCalcMethodComboBoxIndexChanged(int index);

  void onNightCycleComboBoxIndexChanged(int index);

  void onControlZoneComboBoxChanged(int index);

  void onUnitaryHeatPumpControlZoneChanged(int index);

  // void onDropZoneItemClicked(OSItem* item);

  private:

  QPointer<HVACAirLoopControlsView> m_hvacAirLoopControlsView;

  QPointer<HVACPlantLoopControlsView> m_hvacPlantLoopControlsView;

  QPointer<NoControlsView> m_noControlsView;

  QPointer<MechanicalVentilationView> m_mechanicalVentilationView;

  QPointer<SingleZoneReheatSPMView> m_singleZoneReheatSPMView;

  QPointer<ScheduledSPMView> m_scheduledSPMView;

  QPointer<NoSupplyAirTempControlView> m_noSupplyAirTempControlView;

  QPointer<NoMechanicalVentilationView> m_noMechanicalVentilationView;

  QPointer<OSDropZone> m_systemAvailabilityDropZone;

  QPointer<OSDropZone> m_supplyAirTempScheduleDropZone;

  QPointer<FollowOATempSPMView> m_followOATempSPMView;

  QPointer<OAResetSPMView> m_oaResetSPMView;

  QPointer<AirLoopHVACUnitaryHeatPumpAirToAirControlView> m_airLoopHVACUnitaryHeatPumpAirToAirControlView;

  QPointer<HVACSystemsController> m_hvacSystemsController;

  QPointer<OSDropZone> m_availabilityManagerDropZone;

  bool m_dirty;
};

class HVACLayoutController : public QObject, public Nano::Observer
{
  Q_OBJECT;

  public:

  HVACLayoutController(HVACSystemsController * hvacSystemsController);

  virtual ~HVACLayoutController();

  std::shared_ptr<RefrigerationGridController> refrigerationGridController() const;

  HVACGraphicsView * hvacGraphicsView() const;

  void clearSceneSelection();

  public slots:

  void updateLater();

  private slots:

  void update();

  void onModelObjectSelected(model::OptionalModelObject & modelObject, bool readOnly);

  void addLibraryObjectToModelNode(OSItemId itemid, model::HVACComponent & comp);

  void removeModelObject(model::ModelObject & modelObject);

  void goToOtherLoop( model::ModelObject & modelObject );

  void addLibraryObjectToTopLevel(OSItemId itemid);

  void goToServiceWaterScene();

  private:

  QPointer<HVACGraphicsView> m_hvacGraphicsView;

  QPointer<HVACSystemsController> m_hvacSystemsController;

  std::shared_ptr<RefrigerationGridController> m_refrigerationGridController;

  bool m_dirty;
};

class SystemAvailabilityVectorController : public ModelObjectVectorController
{
  Q_OBJECT;

  public:

  SystemAvailabilityVectorController();

  virtual ~SystemAvailabilityVectorController();

  boost::optional<model::AirLoopHVAC> airLoopHVAC();

  void attach(const model::ModelObject& modelObject) override;

  void detach();

  public slots:

  void reportItemsLater();

  void reportItems();

  void onDropZoneItemClicked(OSItem* item);

  protected:

  std::vector<OSItemId> makeVector() override;

  void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;

  void onDrop(const OSItemId& itemId) override;

  private:

  bool m_reportScheduled;

  QMutex * m_reportItemsMutex;
};

class SupplyAirTempScheduleVectorController : public ModelObjectVectorController
{
  Q_OBJECT;

  public:

  SupplyAirTempScheduleVectorController();

  virtual ~SupplyAirTempScheduleVectorController();

  boost::optional<model::SetpointManagerScheduled> setpointManagerScheduled();

  void attach(const model::ModelObject& modelObject) override;

  void detach();

  public slots:

  void reportItemsLater();

  void reportItems();

  void onDropZoneItemClicked(OSItem* item);

  protected:

  std::vector<OSItemId> makeVector() override;

  void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;

  void onDrop(const OSItemId& itemId) override;

  private:

  bool m_reportScheduled;

  QMutex * m_reportItemsMutex;
};


class AvailabilityManagerObjectVectorController : public ModelObjectVectorController
{
  Q_OBJECT;

 public:

  AvailabilityManagerObjectVectorController();

  virtual ~AvailabilityManagerObjectVectorController();

  void attach(const model::ModelObject& modelObject) override;

  void detach();

  boost::optional<model::PlantLoop> currentLoop();

  boost::optional<model::AvailabilityManagerAssignmentList> avmList();

 public slots:

  void reportItemsLater();

  void reportItems();

  void onDropZoneItemClicked(OSItem* item);


 protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;

  virtual void onDataChange(const model::ModelObject& modelObject) override;

  virtual std::vector<OSItemId> makeVector() override;

  virtual void onRemoveItem(OSItem* item) override;

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;

  virtual void onDrop(const OSItemId& itemId) override;


 private:

  bool m_reportScheduled;

  QMutex * m_reportItemsMutex;

};


} // openstudio

#endif // OPENSTUDIO_HVACSYSTEMSCONTROLLER_HPP

