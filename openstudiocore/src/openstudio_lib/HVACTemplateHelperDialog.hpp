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

#ifndef OPENSTUDIO_HVACTEMPLATEHELPERDIALOG_HPP
#define OPENSTUDIO_HVACTEMPLATEHELPERDIALOG_HPP

#include <QDialog>
#include "../model/Model.hpp"
#include "../model/ThermalZone.hpp"

class QButtonGroup;

namespace openstudio {

class HVACTemplateHelperDialog : public QDialog
{
  Q_OBJECT;

  public:

  HVACTemplateHelperDialog(const model::Model & model, QWidget * parent = 0);

  virtual ~HVACTemplateHelperDialog() {}

  std::vector<model::ThermalZone> selectedZones();

  private:

  model::Model m_model;

  std::vector<model::ThermalZone> m_zones;

  QButtonGroup * m_buttonGroup;
};

} // openstudio

#endif // OPENSTUDIO_HVACTEMPLATEHELPERDIALOG_HPP

