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

#ifndef OPENSTUDIO_THERMALZONESVIEW_HPP
#define OPENSTUDIO_THERMALZONESVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelSubTabView.hpp"

#include "../model/ThermalZone.hpp"

namespace openstudio {

  class ThermalZonesGridView;

class ThermalZonesView : public ModelSubTabView
{
  Q_OBJECT

public:

  ThermalZonesView(bool isIP, const model::Model & model, QWidget * parent = 0);

  virtual ~ThermalZonesView() {}

};

class ThermalZoneView : public ModelObjectInspectorView
{

  Q_OBJECT

public:

  ThermalZoneView(bool isIP, const model::Model & model, QWidget * parent = 0);

  virtual ~ThermalZoneView() {}

  virtual bool supportsMultipleObjectSelection() const { return true; }
  virtual std::vector<model::ModelObject> selectedObjects() const;

public slots:

  void refresh();

signals:

  void modelObjectSelected(model::OptionalModelObject & modelObject, bool readOnly);

  void dropZoneItemSelected(OSItem* item, bool readOnly);

protected:

  void onClearSelection();

  void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

  void onUpdate();

private slots:

  void toggleUnits(bool);

private:

  ThermalZonesGridView * m_thermalZonesGridView = nullptr;

  bool m_isIP;

};

} // openstudio

#endif // OPENSTUDIO_THERMALZONESVIEW_HPP
