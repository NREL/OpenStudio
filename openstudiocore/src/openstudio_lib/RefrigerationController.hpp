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

  QSharedPointer<OSListItem> itemAt(int i) override;
  int count() override;

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

// A delegate to provide cells of the refrigeration system grid
class RefrigerationSystemItemDelegate : public OSGraphicsItemDelegate
{
  Q_OBJECT;

  public:

  virtual ~RefrigerationSystemItemDelegate() {}

  virtual QGraphicsObject * view(QSharedPointer<OSListItem> dataSource) override;
};

class RefrigerationSystemListDropZoneItem : public OSListItem
{
  Q_OBJECT

  public:

  RefrigerationSystemListDropZoneItem(OSListController * listController = nullptr);

  ~RefrigerationSystemListDropZoneItem() {}
};

class RefrigerationSystemListItem : public OSListItem
{
  Q_OBJECT

  public:

  RefrigerationSystemListItem(const model::RefrigerationSystem & refrigerationSystem, OSListController * listController = nullptr);
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

