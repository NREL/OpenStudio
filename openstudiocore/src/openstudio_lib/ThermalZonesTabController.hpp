/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_THERMALZONESTABCONTROLLER_HPP
#define OPENSTUDIO_THERMALZONESTABCONTROLLER_HPP

#include "MainTabController.hpp"
#include "../model/Model.hpp"

namespace openstudio {

class ThermalZonesController;
class ThermalZonesView;

class ThermalZonesTabController : public MainTabController
{
  Q_OBJECT

public:

  ThermalZonesTabController(bool isIP, const model::Model& model);

  virtual ~ThermalZonesTabController() {}

signals:

  void toggleUnitsClicked(bool);

private slots:

  //void onSelectItem(OSItem *item);
  void addObject();
  //void onCopyObject(const openstudio::model::ModelObject& modelObject);
  void removeSelectedObjects();
  //void onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId);
  //void onPurgeObjects(const openstudio::IddObjectType& iddObjectType);
  //void onDrop(const OSItemId& itemId);
  //void onInspectItem(OSItem* item);

private:

  ThermalZonesView * m_thermalZonesView;

  std::shared_ptr<ThermalZonesController> m_thermalZonesController;

  model::Model m_model;
};

} // openstudio

#endif // OPENSTUDIO_THERMALZONESTABCONTROLLER_HPP
