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

#ifndef OPENSTUDIO_VRFCONTROLLER_HPP
#define OPENSTUDIO_VRFCONTROLLER_HPP

#include <QObject>
#include <QSharedPointer>
#include <boost/optional.hpp>
#include "../model/AirConditionerVariableRefrigerantFlow.hpp"
#include "../shared_gui_components/OSListController.hpp"

class QGraphicsScene;
class QGraphicsView;

namespace openstudio {

class OSItemId;
class VRFSystemListController;
class GridLayoutItem;
class VRFView;
class VRFSystemView;

class VRFController : public QObject
{
  Q_OBJECT

  public:

  VRFController();

  virtual ~VRFController();

  VRFView * vrfView() const;

  QSharedPointer<VRFSystemListController> vrfSystemListController() const;

  public slots:

  void zoomInOnSystem(model::AirConditionerVariableRefrigerantFlow & system);

  void zoomOutToSystemGridView();

  private slots:

  void refresh();

  void refreshNow();

  void inspectOSItem(const OSItemId & itemid);

  void onVRFSystemViewDrop(const OSItemId & itemid);

  void onVRFSystemViewZoneDrop(const OSItemId & itemid);

  void onVRFTerminalViewDrop(const OSItemId & terminalId, const OSItemId & thermalZoneId);

  void onRemoveZoneClicked(const OSItemId & terminalId);

  void onRemoveTerminalClicked(const OSItemId & terminalId);

  private:

  QPointer<VRFView> m_vrfView;

  QPointer<GridLayoutItem> m_vrfSystemGridView;

  QPointer<VRFSystemView> m_detailView;

  QSharedPointer<QGraphicsScene> m_vrfGridScene;

  QSharedPointer<QGraphicsScene> m_detailScene;

  QSharedPointer<VRFSystemListController> m_vrfSystemListController;

  boost::optional<model::AirConditionerVariableRefrigerantFlow> m_currentSystem;

  bool m_dirty;
};

class VRFSystemListController : public OSListController
{
  Q_OBJECT

  public:

  VRFSystemListController(VRFController * vrfController);

  VRFController * vrfController() const;

  QSharedPointer<OSListItem> itemAt(int i);

  int count();

  void reset();

  public slots:

  void addSystem(const OSItemId & itemid);

  void createNewSystem();

  void removeSystem(model::AirConditionerVariableRefrigerantFlow & system);

  private:

  std::vector<model::AirConditionerVariableRefrigerantFlow> systems() const;

  int systemIndex(const model::AirConditionerVariableRefrigerantFlow & system) const;

  QPointer<VRFController> m_vrfController;
};

// A delegate to provide cells of the vrf system grid
class VRFSystemItemDelegate : public OSGraphicsItemDelegate
{
  Q_OBJECT;

  public:

  virtual ~VRFSystemItemDelegate() {}

  virtual QGraphicsObject * view(QSharedPointer<OSListItem> dataSource);
};

class VRFSystemListDropZoneItem : public OSListItem
{
  Q_OBJECT

  public:

  VRFSystemListDropZoneItem(OSListController * listController = 0);

  ~VRFSystemListDropZoneItem() {}
};

class VRFSystemListItem : public OSListItem
{
  Q_OBJECT

  public:

  VRFSystemListItem(const model::AirConditionerVariableRefrigerantFlow & vrfSystem, OSListController * listController = 0);

  virtual ~VRFSystemListItem() {}

  QString systemName() const;

  int numberOfConnectedZones() const;

  int numberOfConnectedTerminals() const;

  public slots:

  void remove();

  void zoomInOnSystem();

  private:

  model::AirConditionerVariableRefrigerantFlow m_vrfSystem;
};

} // openstudio

#endif // OPENSTUDIO_VRFCONTROLLER_HPP

