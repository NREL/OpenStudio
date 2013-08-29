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

#ifndef OPENSTUDIO_CLOUDDIALOG_H
#define OPENSTUDIO_CLOUDDIALOG_H

#include <shared_gui_components/OSDialog.hpp>

class QCheckBox;
class QComboBox;
class QLineEdit;
class QStackedWidget;
class QVBoxLayout;

namespace openstudio{

class CloudDialog : public OSDialog
{
  Q_OBJECT

  public:

    CloudDialog(QWidget * parent = 0);

    virtual ~CloudDialog();

  protected slots:

    virtual void on_cancelButton(bool checked);

    virtual void on_okButton(bool checked);

  private:

    void createWidgets();

    void setLoginWidget(QWidget * widget);

    QCheckBox * m_iAcceptCheckBox; 
    
    QComboBox * m_cloudResourceComboBox;

    QStackedWidget * m_stackedWidget;

    QVBoxLayout * m_leftLayout;

  private slots:

    void iAcceptClicked(bool checked);

};


class CloudProvider : public QWidget
{
  Q_OBJECT

  public:

    CloudProvider(QWidget * parent = 0);

    virtual ~CloudProvider();

    virtual void loadData() = 0;
    virtual void saveData() = 0;

  protected:

    virtual void createWidgets();
    virtual void createLoginWidget() = 0;
    virtual void createSettingsWidget() = 0;

    QCheckBox * m_waitCheckBox;
    QLineEdit * m_waitineEdit;
    QVBoxLayout * m_loginLayout;
    QVBoxLayout * m_leftSettingLayout;
    QVBoxLayout * m_rightSettingsLayout;
    QWidget * m_loginWidget;
    QWidget * m_settingsWidget;

};

class Vagrant : public CloudProvider
{
  Q_OBJECT

  public:

    Vagrant(QWidget * parent = 0);

    virtual ~Vagrant();

    virtual void loadData();
    virtual void saveData();

  protected:

    virtual void createLoginWidget();
    virtual void createSettingsWidget();

  private slots:

    void serverDirButtonClicked(bool checked);
    void workerDirButtonClicked(bool checked);

  private:

    QCheckBox * m_runOnStartUpCheckBox;

    QLineEdit * m_serverUsernameLineEdit;
    QLineEdit * m_serverPasswordLineEdit;
    QLineEdit * m_workerUsernameLineEdit;
    QLineEdit * m_workerPasswordLineEdit;
    QLineEdit * m_serverDirLineEdit;
    QLineEdit * m_serverIpLineEdit;
    QLineEdit * m_serverIp2LineEdit;
    QLineEdit * m_workerDirLineEdit;
    QLineEdit * m_workerIpLineEdit;
    QLineEdit * m_workerIp2LineEdit;

};

class Amazon : public CloudProvider
{
  Q_OBJECT

  public:

    Amazon(QWidget * parent = 0);

    virtual ~Amazon();
    
    virtual void loadData();
    virtual void saveData();

  protected:

    virtual void createLoginWidget();
    virtual void createSettingsWidget();

private:

    QComboBox * m_regionComboBox;
    QComboBox * m_serverInstanceTypeComboBox;
    QComboBox * m_workerInstanceTypeComboBox;

    QLineEdit * m_accessKeyLineEdit;
    QLineEdit * m_secretKeyLineEdit;
    QLineEdit * m_selectPrivateKeyLineEdit;
    QLineEdit * m_numberOfWorkerInstancesLineEdit;
    QLineEdit * m_elasticStorageCapacityLineEdit;

};

} // openstudio

#endif // OPENSTUDIO_CLOUDDIALOG_H
