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

#ifndef OPENSTUDIO_ZONECHOOSERVIEW_HPP
#define OPENSTUDIO_ZONECHOOSERVIEW_HPP

#include <QWidget>
#include "../model/ModelObject.hpp"
#include "../model/AirLoopHVACZoneSplitter.hpp"
#include "../model/AirLoopHVACZoneMixer.hpp"
#include "../model/ThermalZone.hpp"
#include "../utilities/core/String.hpp"

class QCheckBox;

class QVBoxLayout;

namespace openstudio {

class ZoneChooserItem;

class ZoneChooserView : public QWidget
{
  Q_OBJECT;

  public:

  ZoneChooserView(QWidget* parent = 0);

  virtual ~ZoneChooserView() {}

  ZoneChooserItem * zoneChooserItemForZone(std::string zoneName);

  void layoutView();

  public slots:

  void layoutModelObject(model::ModelObject & modelObject);

  signals:

  void addZoneClicked(model::ThermalZone &);

  void removeZoneClicked(model::ThermalZone &);

  protected:

  void paintEvent ( QPaintEvent * event );

  private:

  model::OptionalAirLoopHVACZoneSplitter m_splitter;

  model::OptionalAirLoopHVACZoneMixer m_mixer;

  QVBoxLayout * m_vLayout;

  std::vector<ZoneChooserItem *> m_zoneChooserItems;

};

class ZoneChooserItem : public QWidget
{
  Q_OBJECT;

  public:

  ZoneChooserItem(model::ThermalZone &, ZoneChooserView * parent = 0);

  virtual ~ZoneChooserItem() {}

  std::string zoneName();

  signals:

  void addZoneClicked(model::ThermalZone &);

  void removeZoneClicked(model::ThermalZone &);

  public slots:

  void setChecked( bool checked );

  private slots:

  void sendClickedSignal( bool checked );

  private:

  QCheckBox * m_checkBox;

  ZoneChooserView * m_zoneChooserView;

  model::ThermalZone m_thermalZone;
};

} // openstudio

#endif // OPENSTUDIO_ZONECHOOSERVIEW_HPP

