/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef BIMSERVER_PROJECTIMPORTER_HPP
#define BIMSERVER_PROJECTIMPORTER_HPP

#include "BIMserverAPI.hpp"
#include "BIMserverConnection.hpp"

#include "../model/Model.hpp"

#include <QDialog>
#include <QListWidget>
#include <QStatusBar>
#include <QSettings>
#include <QEventLoop>
#include <QCloseEvent>
#include <QKeyEvent>

namespace openstudio {
namespace bimserver {

    /// This shows a input dialog to gather project id for import
  class BIMSERVER_API ProjectImporter: public QDialog
  {
    Q_OBJECT

    public:

      /// Default constructor
      ProjectImporter(QWidget *parent);

      /// Start importing IFC workflow
      boost::optional<model::Model> run();

      /// Virtual destructor
      ~ProjectImporter();

      /// Reimplemented the close event and guide it to the run() function
      void closeEvent(QCloseEvent *event) override;

      /// Reimplemented the key press event of ESC and guide it to the run() function
      void keyPressEvent(QKeyEvent *event) override;

    signals:
      /// OSM String is retrieved.
      void finished();

    public slots:

      /// Takes projectList from BIMserverConnection and prints out projects
      void processProjectList(QStringList projectList);
      /// Takes ifc list from BIMserverConnection and prints out ifc list
      void processIFCList(QStringList ifcList);
      /// process success cases for createProject, checkInIFC, and login
      void processSucessCases(QString sucessCase);
      /// process all failure cases if BIMserver outputs an exception. Print it 
      void processFailureCases(QString failureCase);
      /// OSM string is retrieved
      void processOSMRetrieved(QString osmString);
      /// process if BIMserver is not connected.
      void processBIMserverErrors();

    private:

      BIMserverConnection *m_bimserverConnection;
      QSettings *m_settings;


      QString     m_proID;
      QString     m_ifcID;
      QListWidget *m_proList;
      QListWidget *m_ifcList;
      QStatusBar  *m_statusBar;
      QEventLoop  *m_waitForOSM;
      QString     m_OSM;

      QPushButton *m_okButton;
      QPushButton *m_loadButton;
      QPushButton *m_selectButton;

    private slots:

      void okButton_clicked();
      void newButton_clicked();
      void loadButton_clicked();
      void selectButton_clicked();
      void settingButton_clicked();
      void app_ended();

    };

} // bimserver
} // openstudio

#endif // BIMSERVER_PROJECTIMPORTER_HPP 
