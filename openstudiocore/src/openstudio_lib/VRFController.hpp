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

#ifndef OPENSTUDIO_VRFCONTROLLER_H
#define OPENSTUDIO_VRFCONTROLLER_H

#include <QObject>
#include <QSharedPointer>
#include <boost/optional.hpp>
#include "../model/AirConditionerVariableRefrigerantFlow.hpp"
#include "../shared_gui_components/OSListController.hpp"

namespace openstudio {

class OSItemId;
class VRFSystemListController;
class VRFView;

class VRFController : public QObject
{
  Q_OBJECT

  public:

  VRFController();

  virtual ~VRFController();

  VRFView * vrfView() const;

  QSharedPointer<VRFSystemListController> vrfSystemListController() const;

  private:

  QPointer<VRFView> m_vrfView;

  QSharedPointer<VRFSystemListController> m_vrfSystemListController;
};

class VRFSystemListController : public OSListController
{
  Q_OBJECT

  public:

  VRFSystemListController(RefrigerationController * refrigerationController);

  VRFController * vrfController() const;

  QSharedPointer<OSListItem> itemAt(int i);

  int count();

  public slots:

  void addSystem(const OSItemId & itemid);

  void createNewSystem();

  void removeSystem(model::AirConditionerVariableRefrigerantFlow & system);

  private:

  std::vector<model::AirConditionerVariableRefrigerantFlow> systems() const;

  int systemIndex(const model::RefrigerationSystem & system) const;

  QPointer<AirConditionerVariableRefrigerantFlow> m_refrigerationController;
};

// A delegate to provide cells of the vrf system grid
class VRFSystemItemDelegate : public OSGraphicsItemDelegate
{
  Q_OBJECT;

  public:

  virtual ~VRFSystemItemDelegate() {}

  virtual QGraphicsObject * view(QSharedPointer<OSListItem> dataSource);
};

//class RefrigerationSystemListDropZoneItem : public OSListItem
//{
//  Q_OBJECT
//
//  public:
//
//  RefrigerationSystemListDropZoneItem(OSListController * listController = 0);
//
//  ~RefrigerationSystemListDropZoneItem() {}
//};

class VRFSystemListItem : public OSListItem
{
  Q_OBJECT

  public:

  VRFSystemListItem(const model::AirConditionerVariableRefrigerantFlow & vrfSystem, OSListController * listController = 0);

  virtual ~VRFSystemListItem() {}

  QString systemName() const;

  public slots:

  void remove();

  void zoomInOnSystem();

  private:

  model::AirConditionerVariableRefrigerantFlow m_vrfSystem;
};

} // openstudio

#endif // OPENSTUDIO_VRFCONTROLLER_H

