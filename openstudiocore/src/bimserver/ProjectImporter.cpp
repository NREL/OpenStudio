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

#include "ProjectImporter.hpp"
#include "BIMserverConnection.hpp"

#include "../osversion/VersionTranslator.hpp"


#include <QFileDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QEventLoop>

#include <QLineEdit>
#include <QPushButton>

namespace openstudio {
namespace bimserver {

  ProjectImporter::ProjectImporter(QWidget *parent) :
    QDialog(parent) 
  {
    auto subLayout = new QGridLayout;
    auto mainLayout = new QVBoxLayout;

    QLabel *introLabel = new QLabel("Please select a project to import: ", this);
    m_proList = new QListWidget(this);
    m_ifcList = new QListWidget(this);
    m_okButton = new QPushButton(tr("Download OSM File"), this);
    m_okButton->setEnabled(false);
    QPushButton *newButton = new QPushButton(tr("New Project"), this);
    m_loadButton = new QPushButton(tr("Check in IFC File"), this);
    m_loadButton->setEnabled(false);
    m_selectButton = new QPushButton(tr(" > "), this);
    m_selectButton->setEnabled(false);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), this);
    QPushButton *settingButton = new QPushButton(tr("Setting"), this);
    m_statusBar = new QStatusBar(this);
    
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(okButton_clicked()));
    connect(newButton, SIGNAL(clicked()),this, SLOT(newButton_clicked()));
    connect(m_loadButton, SIGNAL(clicked()), this, SLOT(loadButton_clicked()));
    connect(m_selectButton, SIGNAL(clicked()), this, SLOT(selectButton_clicked()));
    connect(settingButton, SIGNAL(clicked()),this, SLOT(settingButton_clicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(app_ended()));

    subLayout->addWidget(m_proList, 1, 0, 1, 1);
    subLayout->addWidget(m_ifcList, 1, 2, 1, 1);

    subLayout->addWidget(introLabel, 0, 0, 1, 3);
    subLayout->addWidget(m_selectButton, 1, 1, 1, 1);
    subLayout->addWidget(newButton, 2, 0, 1, 1);
    subLayout->addWidget(m_loadButton, 2, 2, 1, 1);
    subLayout->addWidget(m_okButton, 3, 0, 1, 1);
    subLayout->addWidget(settingButton, 3, 1, 1, 1);
    subLayout->addWidget(cancelButton, 3, 2, 1, 1);

    mainLayout->addLayout(subLayout);

    mainLayout->addWidget(m_statusBar);

    setLayout(mainLayout);

    setWindowTitle("Import Project");

    m_waitForOSM = new QEventLoop(this);
    m_settings = new QSettings("OpenStudio", "BIMserverConnection");
    m_bimserverConnection = nullptr;
  }

  ProjectImporter::~ProjectImporter() 
  {
  }

  boost::optional<model::Model> ProjectImporter::run() 
  {
    if (m_settings->contains("addr") && m_settings->contains("port") && m_settings->contains("usrname") && m_settings->contains("psw")) {
      QString addr = m_settings->value("addr").toString();
      QString port = m_settings->value("port").toString();
      QString usrname = m_settings->value("usrname").toString();
      QString psw = m_settings->value("psw").toString();

      m_bimserverConnection = new BIMserverConnection(this, addr, port);

      connect(m_bimserverConnection, &BIMserverConnection::osmStringRetrieved, this, &ProjectImporter::processOSMRetrieved);
      connect(m_bimserverConnection, &BIMserverConnection::listAllProjects, this, &ProjectImporter::processProjectList);
      connect(m_bimserverConnection, &BIMserverConnection::listAllIFCRevisions, this, &ProjectImporter::processIFCList);
      connect(m_bimserverConnection, &BIMserverConnection::operationSucceeded, this, &ProjectImporter::processSucessCases);
      connect(m_bimserverConnection, &BIMserverConnection::errorOccured, this, &ProjectImporter::processFailureCases);
      connect(m_bimserverConnection, &BIMserverConnection::bimserverError, this, &ProjectImporter::processBIMserverErrors);
      connect(this, SIGNAL(finished()), m_waitForOSM, SLOT(quit()));

      m_bimserverConnection->login(usrname, psw);

    } else {
      settingButton_clicked();
    }

    //execute event loop
    m_waitForOSM->exec();

    //Reverse Translate from osmString.

    if (!m_OSM.isEmpty()) {
      std::stringstream stringStream(m_OSM.toStdString());

      openstudio::osversion::VersionTranslator vt;

      return vt.loadModel(stringStream);
    } else {
      return boost::none;
    }
  }

  void ProjectImporter::processProjectList(QStringList pList) 
  {

    m_proList->clear();

    foreach(QString itm, pList) {
      m_proList->addItem(itm);
    }

    m_selectButton->setEnabled(true);
  }

  void ProjectImporter::processIFCList(QStringList iList) 
  {

    m_ifcList->clear();

    foreach(QString itm, iList) {
      m_ifcList->addItem(itm);
    }

    m_loadButton->setEnabled(true);
    m_okButton->setEnabled(true);
  }

  void ProjectImporter::processSucessCases(QString sucessCase) 
  {
    if (sucessCase == "createProject") {
      m_statusBar->showMessage(tr("Project created, showing updated project list."), 2000);
      m_bimserverConnection->getAllProjects();

    } else if (sucessCase == "checkInIFC") {
      m_statusBar->showMessage(tr("IFC file loaded, showing updated IFC file list."), 2000);
      m_bimserverConnection->getIFCRevisionList(m_proID);

    } else if (sucessCase == "login") {
      this->show();
      m_statusBar->showMessage(tr("Login success!"), 2000);
      m_bimserverConnection->getAllProjects();
    }
  }

  void ProjectImporter::processFailureCases(QString failureCase) 
  {

    m_statusBar->showMessage(failureCase, 2000);
  } 

  void ProjectImporter::processBIMserverErrors() {
    this->hide();

    QMessageBox messageBox(this);
    messageBox.setText(tr("BIMserver disconnected")); 
    messageBox.setDetailedText(tr("BIMserver is not connected correctly. Please check if BIMserver is running and make sure your username and password are valid.\n"));
    messageBox.exec();

    settingButton_clicked();
  }

  void ProjectImporter::okButton_clicked() 
  {
    if (m_ifcList->currentItem()) {
      m_ifcID = m_ifcList->currentItem()->text().section(":", 0, 0);

      m_bimserverConnection->download(m_ifcID);

    } else {
      m_statusBar->showMessage(tr("Please select a IFC version before proceeding."), 2000);
    }
  }

  void ProjectImporter::selectButton_clicked() 
  {
    if (m_proList->currentItem()) {
      m_proID = m_proList->currentItem()->text().section(":", 0, 0);
      m_statusBar->showMessage(tr("Project selected, showing all versions of IFC files under it."), 2000);
      m_bimserverConnection->getIFCRevisionList(m_proID);

    } else {
      m_statusBar->showMessage(tr("Please select a project to see all the IFC versions under it."), 2000);
    }
  }

  void ProjectImporter::newButton_clicked() 
  {
    m_statusBar->showMessage(tr("Create a new project and upload it to the server."), 2000);
    QDialog newDialog(this);
    QGridLayout newLayout;
    newDialog.setWindowTitle(tr("New Project"));
    newDialog.setLayout(&newLayout);

    QLabel new_introLabel(tr("Please enter the project name: "), this);
    QLabel new_nameLabel(tr("Project Name:"), this);
    QLineEdit new_nameEdit;
    QPushButton new_okButton(tr("Create Project"), this);
    QPushButton new_cancelButton(tr("Cancel"), this);

    newLayout.addWidget(&new_introLabel,0,0,1,2);
    newLayout.addWidget(&new_nameLabel,1,0,1,1);
    newLayout.addWidget(&new_nameEdit,1,1,1,1);
    newLayout.addWidget(&new_okButton,2,0,1,1);
    newLayout.addWidget(&new_cancelButton,2,1,1,1);

    connect(&new_okButton, SIGNAL(clicked()), &newDialog, SLOT(accept()));
    connect(&new_cancelButton, SIGNAL(clicked()), &newDialog, SLOT(reject()));

    if (newDialog.exec()==QDialog::Accepted) {
      QString new_proString = new_nameEdit.text();
      m_bimserverConnection->createProject(new_proString);
      m_statusBar->showMessage("Project "+new_proString+" created, please upload IFC file now.", 2000);
    } else {
      m_statusBar->showMessage("Project creation terminated.", 2000);
    }
  }

  void ProjectImporter::loadButton_clicked() 
  {

    m_statusBar->showMessage(tr("Check in a new version IFC file for the selected project."), 2000);

    if (m_proList->currentItem()) {

      m_proID = m_proList->currentItem()->text().section(":", 0, 0);

      QString new_ifcString = QFileDialog::getOpenFileName(this,
        tr("Open IFC File"), ".",
        tr("IFC files (*.ifc)"));

    
      if (!new_ifcString.isEmpty()) {
        m_statusBar->showMessage("IFC File " + new_ifcString + " loaded.", 2000);
        m_bimserverConnection->checkInIFCFile(m_proID, new_ifcString);
      }
    } else {
      m_statusBar->showMessage(tr("Please select a project to check in a new IFC version."), 2000);
    }
  }

  void ProjectImporter::processOSMRetrieved(QString osmString) 
  {
    m_OSM = osmString;
    emit finished();
  }

  void ProjectImporter::settingButton_clicked() 
  {

    m_statusBar->showMessage(tr("Please specify the bimserver address/port and user credentials."), 2000);
    QDialog setDialog(this);
    auto setLayout = new QGridLayout;
    setDialog.setWindowTitle(tr("BIMserver Settings"));

    QLabel *set_introLabel = new QLabel(tr("Please enter the BIMserver information: "), this);
    QLabel *set_baddLabel = new QLabel(tr("BIMserver Address: http://"), this);
    auto set_baddEdit = new QLineEdit(this);
    set_baddEdit->setPlaceholderText("eg: 127.0.0.1");
    QLabel *set_bportLabel = new QLabel(tr("BIMserver Port:"), this);
    auto set_bportEdit = new QLineEdit(this);
    set_bportEdit->setPlaceholderText("eg: 8082");
    QLabel *set_unameLabel = new QLabel(tr("Username"), this);
    auto set_unameEdit = new QLineEdit(this);
    set_unameEdit->setPlaceholderText("eg: admin@bimserver.org");
    QLabel *set_upassLabel = new QLabel(tr("Password"), this);
    auto set_upassEdit = new QLineEdit(this);
    set_upassEdit->setPlaceholderText("eg: admin");
    set_upassEdit->setEchoMode(QLineEdit::Password);
    QPushButton *set_okButton = new QPushButton(tr("Okay"), this);
    QPushButton *set_cancelButton = new QPushButton(tr("Cancel"), this);

    if (m_settings->contains("addr")) {
      QString addr = m_settings->value("addr").toString();
      set_baddEdit->setText(addr);
    }

    if (m_settings->contains("port")) {
      QString port = m_settings->value("port").toString();
      set_bportEdit->setText(port);
    }

    if (m_settings->contains("usrname")) {
      QString usrname = m_settings->value("usrname").toString();
      set_unameEdit->setText(usrname);
    }

    if (m_settings->contains("psw")) {
      QString psw = m_settings->value("psw").toString();
      set_upassEdit->setText(psw);
    }

    setLayout->addWidget(set_introLabel,0,0,1,2);
    setLayout->addWidget(set_baddLabel,1,0,1,1);
    setLayout->addWidget(set_baddEdit,1,1,1,1);
    setLayout->addWidget(set_bportLabel,2,0,1,1);
    setLayout->addWidget(set_bportEdit,2,1,1,1);
    setLayout->addWidget(set_unameLabel,3,0,1,1);
    setLayout->addWidget(set_unameEdit,3,1,1,1);
    setLayout->addWidget(set_upassLabel,4,0,1,1);
    setLayout->addWidget(set_upassEdit,4,1,1,1);
    setLayout->addWidget(set_okButton,5,0,1,1);
    setLayout->addWidget(set_cancelButton,5,1,1,1);

    connect(set_okButton, SIGNAL(clicked()), &setDialog, SLOT(accept()));
    connect(set_cancelButton, SIGNAL(clicked()), &setDialog, SLOT(reject()));

    setDialog.setLayout(setLayout);

    if (setDialog.exec()==QDialog::Accepted) {
      QString address = set_baddEdit->text();
      QString port = set_bportEdit->text();
      QString usrname = set_unameEdit->text();
      QString psw = set_upassEdit->text();

      if (!address.isEmpty() && !port.isEmpty() && !usrname.isEmpty() && !psw.isEmpty())
      { 
        //in debug mode, this pointer is 0xCDCDCDCD, and is not null, creates an error
        //if (m_bimserverConnection != nullptr) {
        //  delete m_bimserverConnection;
        //}

        m_settings->setValue("addr", address);
        m_settings->setValue("port", port);
        m_settings->setValue("usrname", usrname);
        m_settings->setValue("psw", psw);

        m_bimserverConnection = new BIMserverConnection(this, address, port);

        connect(m_bimserverConnection, &BIMserverConnection::osmStringRetrieved, this, &ProjectImporter::processOSMRetrieved);
        connect(m_bimserverConnection, &BIMserverConnection::listAllProjects, this, &ProjectImporter::processProjectList);
        connect(m_bimserverConnection, &BIMserverConnection::listAllIFCRevisions, this, &ProjectImporter::processIFCList);
        connect(m_bimserverConnection, &BIMserverConnection::operationSucceeded, this, &ProjectImporter::processSucessCases);
        connect(m_bimserverConnection, &BIMserverConnection::errorOccured, this, &ProjectImporter::processFailureCases);
        connect(m_bimserverConnection, &BIMserverConnection::bimserverError, this, &ProjectImporter::processBIMserverErrors);
        connect(this, SIGNAL(finished()), m_waitForOSM, SLOT(quit()));

        m_bimserverConnection->login(usrname, psw);


        m_statusBar->showMessage("Settings configured.", 2000);
      }
      else {
        m_proList->clear();
        m_ifcList->clear();
        
        QMessageBox messageBox(this);
        messageBox.setText(tr("BIMserver not set up"));
        messageBox.setDetailedText(tr("Please provide valid BIMserver address, port, your username and password. You may ask your BIMserver manager for such information.\n"));
        messageBox.exec();

        settingButton_clicked();
      }

    } else {
      m_statusBar->showMessage("Settings configuration terminated.", 2000);
      
      if (this->isHidden()) {
        emit finished();
      }

    }
  }

  void ProjectImporter::app_ended() 
  {
    emit finished();
  }

  void ProjectImporter::closeEvent(QCloseEvent *event) {
    emit finished();
    event->accept();
  }

  void ProjectImporter::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {

      emit finished();
      event->accept();

    } else {
      event->ignore();
    }
  }

} // bimserver
} // openstudio
