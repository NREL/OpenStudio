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

#ifndef OPENSTUDIO_INSPECTORCONTROLLER_HPP
#define OPENSTUDIO_INSPECTORCONTROLLER_HPP

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include <boost/smart_ptr.hpp>
#include <QObject>

namespace openstudio {

namespace model {

class ThermalZone;

class Loop;

class WaterToAirComponent;

class HVACComponent;

}

class InspectorView;

class InspectorController : public QObject
{
  Q_OBJECT

  public:

  InspectorController();

  virtual ~InspectorController();

  InspectorView * inspectorView();
  
  signals:
  
  void toggleUnitsClicked(bool displayIP);

  public slots:

  void layoutModelObject( model::OptionalModelObject &, bool readOnly );

  void addBranchForZone(model::ThermalZone &);
  void removeBranchForZone(model::ThermalZone &);
  void moveBranchForZoneSupply(model::ThermalZone & zone, const Handle & newPlenumHandle);
  void moveBranchForZoneReturn(model::ThermalZone & zone, const Handle & newPlenumHandle);

  void addToLoop(model::Loop &, boost::optional<model::HVACComponent> &);
  void removeFromLoop(model::Loop &, boost::optional<model::HVACComponent> &);

  void toggleUnits(bool displayIP);

  private slots:

  void onViewDestroyed(QObject * object = 0);

  private:

  InspectorView * m_inspectorView;

  boost::optional<model::ModelObject> m_modelObject;
};

} // openstudio

#endif // OPENSTUDIO_INSPECTORCONTROLLER_HPP

