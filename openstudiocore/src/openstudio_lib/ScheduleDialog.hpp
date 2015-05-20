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

#ifndef OPENSTUDIO_SCHEDULEDIALOG_HPP
#define OPENSTUDIO_SCHEDULEDIALOG_HPP

#include "../model/Model.hpp"
#include "../model/ScheduleTypeRegistry.hpp"
#include "../model/ScheduleTypeLimits.hpp"

#include "../shared_gui_components/OSDialog.hpp"

#include <set>

class QLabel;

class QComboBox;

namespace openstudio {

class ScheduleDialog : public OSDialog
{
  Q_OBJECT

public:

  ScheduleDialog(bool isIP, 
                 const model::Model & model,
                 QWidget * parent = nullptr);

  virtual ~ScheduleDialog() {}

  void setIsIP(bool isIP);

private slots:
  
  void onCurrentIndexChanged(int index);

private:

  virtual void createLayout() override;

  bool m_isIP;

  model::Model m_model;

  boost::optional<model::ScheduleTypeLimits> m_scheduleTypeLimits;

  QComboBox * m_scheduleTypeComboBox;

  QLabel * m_numericTypeLabel;

  QLabel * m_lowerLimitLabel;

  QLabel * m_upperLimitLabel;

protected slots:

  virtual void on_okButton(bool checked) override;

};

} // openstudio

#endif // OPENSTUDIO_SCHEDULEDIALOG_HPP
