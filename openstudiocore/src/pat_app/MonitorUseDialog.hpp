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

#ifndef PATAPP_MONITORUSEDIALOG_HPP
#define PATAPP_MONITORUSEDIALOG_HPP

#include "../shared_gui_components/OSDialog.hpp"

#include "../utilities/cloud/AWSProvider.hpp"
#include "../utilities/cloud/AWSProvider_Impl.hpp"

class QLabel;
class QLineEdit;
class QPushButton;

namespace openstudio{

namespace pat{

class MonitorUseDialog : public OSDialog
{
  Q_OBJECT

public:

  MonitorUseDialog(QWidget * parent = nullptr);

  virtual ~MonitorUseDialog();

protected:

  void showEvent(QShowEvent * event);

private:

  void createWidgets();
  void displayErrors();

  bool m_totalInstancesAvailable;
  bool m_estimatedChargesAvailable;

  QLabel * m_billingCharge;
  QLabel * m_timeRunning;
  QLabel * m_numInstances;
  QLabel * m_totalNumInstances;
  QPushButton * m_billingInfoButton;
  QPushButton * m_cloudStatus;
  QTimer * m_timer;

  AWSProvider m_awsProvider;

private slots:

  void updateData();
  void displayBillingMessage(bool checked);
  void on_cloudStatus(bool checked);
  void on_estimatedChargesAvailable();
  void on_totalInstancesAvailable();

};

} // pat

} // openstudio

#endif // PATAPP_MONITORUSEDIALOG_HPP


