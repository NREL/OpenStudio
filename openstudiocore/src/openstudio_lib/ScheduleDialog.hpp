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

#ifndef OPENSTUDIO_SCHEDULEDIALOG_HPP
#define OPENSTUDIO_SCHEDULEDIALOG_HPP

#include "../model/Model.hpp"
#include "../model/ScheduleTypeRegistry.hpp"

#include "../shared_gui_components/OSDialog.hpp"

class QComboBox;

class QLineEdit;

namespace openstudio {

class ScheduleDialog : public OSDialog
{
  Q_OBJECT

public:

  ScheduleDialog(bool isIP, 
                 const model::Model & model,
                 QWidget * parent = 0);

  virtual ~ScheduleDialog() {}

private:

  virtual void createLayout();

  void initScheduleRange();

  model::Model m_model;

  std::vector<model::ScheduleType> m_scheduleTypes;

  QComboBox * m_className;

  QComboBox * m_scheduleType;

  QLineEdit * m_scheduleUnits;

  QLineEdit * m_lowerLimit;

  QLineEdit * m_upperLimit;
  
  // ETH@20121004 - Could have combo box at the bottom that shows already existing ScheduleTypeLimits 
  //                that meet the selected criteria.

signals:

  void classNameCurrentIndexChanged(const QString & text);

  void scheduleTypeCurrentIndexChanged(const QString & text);

protected slots:

  virtual void on_okButton(bool checked);

  void on_classNameComboBox(const QString & text);

  void on_scheduleTypeComboBox(int index);

  void toggleUnits(bool displayIP);

};

} // openstudio

#endif // OPENSTUDIO_SCHEDULEDIALOG_HPP
