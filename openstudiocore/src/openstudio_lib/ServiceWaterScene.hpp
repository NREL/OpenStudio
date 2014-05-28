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

#ifndef OPENSTUDIO_SERVICEWATERSCENE_HPP
#define OPENSTUDIO_SERVICEWATERSCENE_HPP

#include "GridScene.hpp"
#include "OSItem.hpp"
#include "../model/Model.hpp"
#include "../model/WaterUseConnections.hpp"

namespace openstudio {

namespace model {

class HVACComponent;

}

class ServiceWaterScene : public GridScene
{
  Q_OBJECT

  public:

  ServiceWaterScene(const model::Model & model);

  virtual ~ServiceWaterScene() {}

  model::Model model() const;

  public slots:

  void layout();

  private slots:

  void onAddedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr );

  void onRemovedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> );

  private:

  bool m_dirty;

  model::Model m_model;
};

class WaterUseConnectionsDetailScene : public GridScene
{
  Q_OBJECT

  public:

  WaterUseConnectionsDetailScene(const model::WaterUseConnections & waterUseConnections);

  virtual ~WaterUseConnectionsDetailScene() {}

  model::WaterUseConnections waterUseConnections() const;

  public slots:

  void layout();

  signals:

  void goToServiceWaterSceneClicked();

  private slots:

  void onAddedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr );

  void onRemovedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> );

  private:

  bool m_dirty;

  model::WaterUseConnections m_waterUseConnections;
};

} // openstudio

#endif // OPENSTUDIO_SERVICEWATERSCENE_HPP

