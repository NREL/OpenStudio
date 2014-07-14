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

#ifndef OPENSTUDIO_GRIDSCENE_HPP
#define OPENSTUDIO_GRIDSCENE_HPP

#include <QGraphicsScene>
#include "../model/ModelObject.hpp"
#include "OSItem.hpp"

namespace openstudio {

namespace model {
  class HVACComponent;
}

class GridScene : public QGraphicsScene
{
  Q_OBJECT

  public:
  
  GridScene( QObject *parent = 0 );

  virtual ~GridScene() {}

  signals:
  
  void modelObjectSelected( model::OptionalModelObject &, bool readOnly );

  void removeModelObjectClicked( model::ModelObject & );

  void hvacComponentDropped(OSItemId itemid, model::HVACComponent & node);

  void hvacComponentDropped(OSItemId itemid);

  void innerNodeClicked( model::ModelObject & );

  protected:

  QRectF getCell(int x, int y);

  QRectF getCell(QPointF point);
};

} // openstudio

#endif // OPENSTUDIO_GRIDSCENE_HPP

