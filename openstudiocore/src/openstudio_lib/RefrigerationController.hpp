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

#ifndef OPENSTUDIO_REFRIGERATIONCONTROLLER_HPP
#define OPENSTUDIO_REFRIGERATIONCONTROLLER_HPP

#include <QObject>
#include <boost/optional.hpp>
#include "../shared_gui_components/OSListController.hpp"
#include "model/RefrigerationSystem.hpp"

class QGraphicsScene;
class QGraphicsView;
class QGraphicsObject;
class QWidget;

namespace openstudio {

class GridLayoutItem;
class RefrigerationSystemView;
class RefrigerationSystemListController;
class RefrigerationScene;
class RefrigerationSystemDetailView;
class OSItemId;
class RefrigerationView;
// TODO class RefrigerationGridView;

class RefrigerationController : public QObject
{
  Q_OBJECT

  public:

  RefrigerationController();

  virtual ~RefrigerationController();

  RefrigerationView * refrigerationView() const;

  // TODO RefrigerationGridView * refrigerationGridView() const;

  QSharedPointer<RefrigerationSystemListController> refrigerationSystemListController() const;

  static boost::optional<model::RefrigerationSystem> cascadeSystem(const model::RefrigerationCondenserCascade & condenser);
  static boost::optional<model::RefrigerationSystem> supplySystem(const model::RefrigerationCondenserCascade & condenser);

  void refreshRefrigerationSystemView(RefrigerationSystemView * systemView, boost::optional<model::RefrigerationSystem> & system);

  public slots:

  void zoomInOnSystem(const Handle & handle);

  void zoomInOnSystem(model::RefrigerationSystem & refrigerationSystem);

  void zoomOutToSystemGridView();

  private slots:

  void refresh();

  void refreshNow();

  void onCondenserViewDrop(const OSItemId & itemid);

  void onCompressorViewDrop(const OSItemId & itemid);

  void onSubCoolerViewDrop(const OSItemId & itemid);

  void onSHXViewDrop(const OSItemId & itemid);

  void onCasesViewDrop(const OSItemId & itemid);

  void onSecondaryViewDrop(const OSItemId & itemid);

  void removeCondenser(const OSItemId & itemid);

  void removeCompressor(const OSItemId & itemid);

  void removeCase(const OSItemId & itemid);

  void removeSubCooler(const OSItemId & itemid);

  void removeSubCoolerLiquidSuction(const OSItemId & itemid);

  void removeLoad(const Handle &);

  void inspectOSItem(const OSItemId & itemid);

  private:

  QPointer<RefrigerationView> m_refrigerationView;

  QPointer<GridLayoutItem> m_refrigerationSystemGridView;

  QPointer<RefrigerationSystemView> m_detailView;

  QSharedPointer<RefrigerationSystemListController> m_refrigerationSystemListController;

  QSharedPointer<QGraphicsScene> m_refrigerationGridScene;

  QPointer<QGraphicsScene> m_refrigerationScene;

  bool m_dirty;

  boost::optional<model::RefrigerationSystem> m_currentSystem;
};

class RefrigerationSystemListController : public OSListController
{
  Q_OBJECT

  public:

  RefrigerationSystemListController(RefrigerationController * refrigerationController);
  RefrigerationController * refrigerationController() const;

  QSharedPointer<OSListItem> itemAt(int i);
  int count();

  void reset();

  signals:

  void itemInsertedPrivate(int i);

  public slots:

  void addSystem(const OSItemId & itemid);
  void createNewSystem();
  void removeSystem(model::RefrigerationSystem & refrigerationSystem);

  private slots:

  void onModelObjectAdd(const WorkspaceObject& object, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle);

  private:

  std::vector<model::RefrigerationSystem> systems() const;
  int systemIndex(const model::RefrigerationSystem & system) const;
  QPointer<RefrigerationController> m_refrigerationController;
};

// A delegate to provide cells of the refigeration system grid
class RefrigerationSystemItemDelegate : public OSGraphicsItemDelegate
{
  Q_OBJECT;

  public:

  virtual ~RefrigerationSystemItemDelegate() {}

  virtual QGraphicsObject * view(QSharedPointer<OSListItem> dataSource);
};

class RefrigerationSystemListDropZoneItem : public OSListItem
{
  Q_OBJECT

  public:

  RefrigerationSystemListDropZoneItem(OSListController * listController = 0);

  ~RefrigerationSystemListDropZoneItem() {}
};

class RefrigerationSystemListItem : public OSListItem
{
  Q_OBJECT

  public:

  RefrigerationSystemListItem(const model::RefrigerationSystem & refrigerationSystem, OSListController * listController = 0);
  virtual ~RefrigerationSystemListItem() {}

  QString systemName() const;
  model::RefrigerationSystem system() const;

  public slots:

  void remove();
  void zoomInOnSystem();

  private:

  model::RefrigerationSystem m_refrigerationSystem;
};

} // openstudio

#endif // OPENSTUDIO_REFRIGERATIONCONTROLLER_HPP

