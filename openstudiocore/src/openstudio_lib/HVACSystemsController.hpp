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

namespace openstudio {

namespace model {

class HVACComponent;
class AirLoopHVAC;
class SetpointManagerScheduled;

}

class LoopView;
class HVACSystemsView;
class HVACControlsView;
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
class OSDropZone;
class FollowOATempSPMView;
class OAResetSPMView;
class AirLoopHVACUnitaryHeatPumpAirToAirControlView;
class NoControlsView;
class RefrigerationController;
class RefrigerationGridController;
class RefrigerationGridView;
class VRFController;

class HVACSystemsController : public QObject
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

  private slots:

  void update();

  void addToModel(AddToModelEnum addToModelEnum);

  void onAddSystemClicked();

  void onRemoveLoopClicked();

  void onShowTopologyClicked();

  void onShowControlsClicked();

  void onShowGridClicked();

  void onObjectAdded(const WorkspaceObject&);

  void onObjectRemoved(const WorkspaceObject&);

  void onObjectChanged();

  void onSystemComboBoxIndexChanged(int i);

  void toggleUnits(bool displayIP);

  private:

  std::vector<IddObjectType> systemComboBoxTypes() const;

  QPointer<HVACSystemsView> m_hvacSystemsView;

  std::shared_ptr<HVACLayoutController> m_hvacLayoutController;

  std::shared_ptr<HVACControlsController> m_hvacControlsController;

  std::shared_ptr<RefrigerationController> m_refrigerationController;

  std::shared_ptr<RefrigerationGridController> m_refrigerationGridController;

  std::shared_ptr<VRFController> m_vrfController;

  QString m_currentHandle;

  bool m_dirty;

  model::Model m_model;

  bool m_isIP;

  signals:
  
  void toggleUnitsClicked(bool displayIP);

};

class HVACControlsController : public QObject
{
  Q_OBJECT;

  public:

  HVACControlsController(HVACSystemsController * hvacSystemsController);

  virtual ~HVACControlsController();

  HVACControlsView * hvacControlsView() const;

  NoControlsView * noControlsView() const;

  boost::optional<model::AirLoopHVAC> airLoopHVAC() const;

  public slots:

  void updateLater();

  private slots:
  
  void update();

  void onEconomizerComboBoxIndexChanged(int index);

  void onVentilationCalcMethodComboBoxIndexChanged(int index);

  void onNightCycleComboBoxIndexChanged(int index);

  void onControlZoneComboBoxChanged(int index);

  void onUnitaryHeatPumpControlZoneChanged(int index);

  private:

  QPointer<HVACControlsView> m_hvacControlsView;

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

  bool m_dirty;
};

class HVACLayoutController : public QObject
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

  virtual ~SystemAvailabilityVectorController() {}

  boost::optional<model::AirLoopHVAC> airLoopHVAC();

  void attach(const model::ModelObject& modelObject);

  void detach();

  public slots:

  void reportItemsLater();

  void reportItems();

  protected:

  std::vector<OSItemId> makeVector();

  void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  void onDrop(const OSItemId& itemId);

  private:

  bool m_reportScheduled;
};

class SupplyAirTempScheduleVectorController : public ModelObjectVectorController
{
  Q_OBJECT;

  public:

  virtual ~SupplyAirTempScheduleVectorController() {}

  boost::optional<model::SetpointManagerScheduled> setpointManagerScheduled();

  void attach(const model::ModelObject& modelObject);

  void detach();

  public slots:

  void reportItemsLater();

  void reportItems();

  protected:

  std::vector<OSItemId> makeVector();

  void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  void onDrop(const OSItemId& itemId);

  private:

  bool m_reportScheduled;
};

} // openstudio

#endif // OPENSTUDIO_HVACSYSTEMSCONTROLLER_HPP

