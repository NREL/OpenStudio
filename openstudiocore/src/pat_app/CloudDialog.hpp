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

#ifndef PATAPP_CLOUDDIALOG_HPP
#define PATAPP_CLOUDDIALOG_HPP

#include "../shared_gui_components/OSDialog.hpp"

#include "../utilities/core/Optional.hpp"

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

class CloudDialog : public OSDialog
{
  Q_OBJECT

public:

  CloudDialog(QWidget * parent = nullptr);

  virtual ~CloudDialog();

protected slots:

  virtual void on_backButton(bool checked);

  virtual void on_cancelButton(bool checked);

  virtual void on_okButton(bool checked);

protected:

  void closeEvent(QCloseEvent * event);

  QCheckBox * m_iAcceptCheckBox; 

private:

  friend class AmazonProviderWidget;
  friend class BlankProviderWidget;
  friend class CloudProviderWidget;
  friend class VagrantProviderWidget;

  void createWidgets();

  boost::optional<CloudProviderWidget *> getCurrentCloudProviderWidget();

  void loadData();

  void saveData();
    
  QComboBox * m_cloudResourceComboBox;

  QStackedWidget * m_pageStackedWidget;

  QStackedWidget * m_loginStackedWidget;

  QStackedWidget * m_settingsStackedWidget;

  QVBoxLayout * m_leftLoginLayout; 

  QVBoxLayout * m_rightLoginLayout;

  QVBoxLayout * m_mainSettingsLayout;

  int m_blankPageIdx;

  int m_loginPageIdx;

  int m_settingsPageIdx;

  AmazonProviderWidget * m_amazonProviderWidget;

  BlankProviderWidget * m_blankProviderWidget;

  VagrantProviderWidget * m_vagrantProviderWidget;  

  QLabel * m_legalAgreement;

  static const int m_blankProviderIdx = 0;

  static const int m_vagrantProviderIdx = 1;

  static const int m_amazonProviderIdx = 2;

private slots:

  void iAcceptClicked(bool checked);

  void cloudResourceChanged(const QString & text);

};

class CloudProviderWidget : public QWidget
{
  Q_OBJECT

public:

  CloudProviderWidget(CloudDialog * cloudDialog);

  virtual ~CloudProviderWidget();

  virtual void loadData() = 0;

  virtual void saveData() = 0;

  QWidget * m_loginWidget;

  QWidget * m_leftSettingsWidget;

  QWidget * m_rightSettingsWidget;

protected:

  virtual void createWidgets();

  virtual void createLoginWidget() = 0;

  virtual void createSettingsWidget() = 0;

  QVBoxLayout * m_leftLoginLayout;

  //QVBoxLayout * m_rightLoginLayout;

  QVBoxLayout * m_leftSettingsLayout;

  QVBoxLayout * m_rightSettingsLayout;

  QCheckBox * m_waitCheckBox;

  QLineEdit * m_waitLineEdit;

  CloudDialog * m_cloudDialog;

protected slots:
    
  void waitClicked(bool checked);

};


class BlankProviderWidget : public CloudProviderWidget
{
  Q_OBJECT

public:

  BlankProviderWidget(CloudDialog * cloudDialog);

  virtual ~BlankProviderWidget();

  virtual void loadData();

  virtual void saveData();

private:

  virtual void createLoginWidget();

  virtual void createSettingsWidget();

};

class VagrantProviderWidget : public CloudProviderWidget
{
  Q_OBJECT

public:

  VagrantProviderWidget(CloudDialog * cloudDialog);

  virtual ~VagrantProviderWidget();

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

public:

  QLineEdit * m_serverUsernameLineEdit;

  QLineEdit * m_serverPasswordLineEdit;

  QLineEdit * m_serverDirLineEdit;

  QLineEdit * m_serverAddressIpLineEdit;

  QLineEdit * m_serverPortIpLineEdit;

  QLineEdit * m_workerDirLineEdit;

  QLineEdit * m_workerAddressIpLineEdit;

  QLineEdit * m_workerPortIpLineEdit;

};

class AmazonProviderWidget : public CloudProviderWidget
{
  Q_OBJECT

public:

  AmazonProviderWidget(CloudDialog * cloudDialog);

  virtual ~AmazonProviderWidget();
    
  virtual void loadData();

  virtual void saveData();

  QLineEdit * m_accessKeyLineEdit;

  QLineEdit * m_secretKeyLineEdit;

protected:

  virtual void createLoginWidget();

  virtual void createSettingsWidget();

private:

  QComboBox * m_regionComboBox;

  QComboBox * m_serverInstanceTypeComboBox;
  QLabel * m_serverNameLabel;
  QLabel * m_serverCpuCountLabel;

  QComboBox * m_workerInstanceTypeComboBox;
  QLabel * m_workerNameLabel;
  QLabel * m_workerCpuCountLabel;

  QLineEdit * m_numberOfWorkerInstancesLineEdit;

private slots:
    
  void on_serverInstanceTypeComboBox(const QString & text);

  void on_workerInstanceTypeComboBox(const QString & text);

};

} // openstudio

#endif // PATAPP_CLOUDDIALOG_HPP
