/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

  virtual void on_backButton(bool checked) override;

  virtual void on_cancelButton(bool checked) override;

  virtual void on_okButton(bool checked) override;

protected:

  void closeEvent(QCloseEvent * event) override;

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

  virtual void loadData() override;

  virtual void saveData() override;

private:

  virtual void createLoginWidget() override;

  virtual void createSettingsWidget() override;

};

class VagrantProviderWidget : public CloudProviderWidget
{
  Q_OBJECT

public:

  VagrantProviderWidget(CloudDialog * cloudDialog);

  virtual ~VagrantProviderWidget();

  virtual void loadData() override;

  virtual void saveData() override;

protected:

  virtual void createLoginWidget() override;

  virtual void createSettingsWidget() override;

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
    
  virtual void loadData() override;

  virtual void saveData() override;

  QLineEdit * m_accessKeyLineEdit;

  QLineEdit * m_secretKeyLineEdit;

protected:

  virtual void createLoginWidget() override;

  virtual void createSettingsWidget() override;

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
