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

#ifndef OPENSTUDIO_OSVECTORCONTROLLER_HPP
#define OPENSTUDIO_OSVECTORCONTROLLER_HPP

#include <QWidget>
#include "../model/ComponentData.hpp"
#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include <vector>

namespace openstudio {

class OSItem;
class OSItemId;

class OSVectorController : public QObject
{
  Q_OBJECT

public:

  // in the future, OSVectorController's will be managed by shared_ptr's on parent controller
  // for now, each controller must be parented by a widget
  OSVectorController();

  virtual ~OSVectorController() {}

public slots:

  void reportItems();

  void removeItem(OSItem* item);

  void replaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  void drop(const OSItemId& itemId);

  void makeNewItem();

signals:

  void itemIds(const std::vector<OSItemId>& itemIds);

  void selectedItemId(const OSItemId& itemId);

protected:

  virtual std::vector<OSItemId> makeVector() = 0;

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);

  virtual void onMakeNewItem();

};

} // openstudio

#endif // OPENSTUDIO_OSVECTORCONTROLLER_HPP

