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

  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

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

  virtual QGraphicsObject * view(QSharedPointer<OSListItem> dataSource) override;
};

class VRFSystemListDropZoneItem : public OSListItem
{
  Q_OBJECT

  public:

  VRFSystemListDropZoneItem(OSListController * listController = nullptr);

  ~VRFSystemListDropZoneItem() {}
};

class VRFSystemListItem : public OSListItem
{
  Q_OBJECT

  public:

  VRFSystemListItem(const model::AirConditionerVariableRefrigerantFlow & vrfSystem, OSListController * listController = nullptr);

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

