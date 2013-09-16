/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_MONITORUSEDIALOG_H
#define OPENSTUDIO_MONITORUSEDIALOG_H

#include <shared_gui_components/OSDialog.hpp>

#include <utilities/core/Optional.hpp>

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QStackedWidget;
class QVBoxLayout;

namespace openstudio{

class AmazonProviderWidget;
class BlankProviderWidget;
class CloudProviderWidget;
class VagrantProviderWidget;

class MonitorUseDialog : public OSDialog
{
  Q_OBJECT

public:

  MonitorUseDialog(QWidget * parent = 0);

  virtual ~MonitorUseDialog();

private:

  void createWidgets();

  QLabel * m_billingCharge;
  QLabel * m_timeRunning;
  QLabel * m_numInstances;
  QLabel * m_totalNumInstances;
  QLabel * m_cloudStatus;

private slots:

  void updateData();

};

} // openstudio

#endif // OPENSTUDIO_MONITORUSEDIALOG_H


