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

#ifndef OPENSTUDIO_LOOPSCENE_HPP
#define OPENSTUDIO_LOOPSCENE_HPP

#include <QPainter>
#include <QRectF>
#include <boost/smart_ptr.hpp>
#include "../model/Loop.hpp"
#include "OSItem.hpp"
#include "GridScene.hpp"

namespace openstudio {

namespace model {
  class Splitter;

  class Mixer;
  
  class Node;

  class HVACComponent;

  class ModelObject;
}

class HorizontalBranchItem;

class VerticalBranchItem;

class HorizontalBranchGroupItem;

class DemandSideItem;

class SupplySideItem;

class OASystemItem;

class LoopScene : public GridScene
{

  Q_OBJECT

  public:
  
  LoopScene( model::Loop loop,
             QObject *parent = 0 );

  virtual ~LoopScene() {}

  model::Loop loop();

  public slots:

  void addedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr );

  void removedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr );
  
  void layout();

  //signals:
  //
  //void modelObjectSelected( model::OptionalModelObject &, bool readOnly );

  //void removeModelObjectClicked( model::ModelObject & );

  //void hvacComponentDropped(OSItemId itemid, model::HVACComponent & node);

  //void innerNodeClicked( model::ModelObject & );

  private:

  DemandSideItem * createDemandSide();

  SupplySideItem * createSupplySide();

  void initDefault();

  model::Loop m_loop;

  bool m_dirty;
};

} // openstudio

#endif // OPENSTUDIO_LOOPSCENE_HPP

