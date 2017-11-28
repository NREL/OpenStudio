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

#ifndef OPENSTUDIO_GEOMETRYEDITORVIEW_HPP
#define OPENSTUDIO_GEOMETRYEDITORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelSubTabView.hpp"

#include "../model/Model.hpp"

#include "../utilities/geometry/FloorplanJS.hpp"

#include <QWidget>
#include <QDialog>
#include <QWebEngineView>
#include <QProgressBar>

class QComboBox;
class QPushButton;
class QTimer;

namespace openstudio {

class GeometryEditorView : public QWidget
{
  Q_OBJECT

  public:

    GeometryEditorView(bool isIP,
                      const openstudio::model::Model& model,
                      QWidget * parent = nullptr);

    virtual ~GeometryEditorView();

  private:

};

// debug widget
class DebugWebView : public QDialog
{
  Q_OBJECT;

  public:
    DebugWebView(const QString& debugPort, QWidget * parent = nullptr);
    virtual ~DebugWebView();

  private:

    QWebEngineView * m_view;
};

// main widget

class EditorWebView : public QWidget
{
  Q_OBJECT;

  public:
    EditorWebView(bool isIP, const openstudio::model::Model& model, QWidget *t_parent = nullptr);
    virtual ~EditorWebView();

  public slots:
    void onUnitSystemChange(bool t_isIP);

  private slots:
    void geometrySourceChanged(const QString& text);
    void newImportClicked();
    void refreshClicked();
    void saveClickedBlocking(const openstudio::path&);
    void previewClicked();
    void mergeClicked();
    void debugClicked();
    void startEditor();
    void doExport();
    void saveExport();
    void translateExport();
    void previewExport();
    void mergeExport();
    void checkForUpdate();
    void onChanged();

    void 	onLoadFinished(bool ok);
    void 	onLoadProgress(int progress);
    void 	onLoadStarted();
    void 	onRenderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus terminationStatus, int exitCode);

  private:
    REGISTER_LOGGER("openstudio::EditorWebView");

    openstudio::path floorplanPath() const;

    bool m_isIP;
    bool m_geometryEditorStarted;
    bool m_geometryEditorLoaded;
    bool m_javascriptRunning;
    boost::optional<FloorplanJS> m_floorplan;
    model::Model m_model;
    QVariant m_export;
    unsigned m_versionNumber;
    model::Model m_exportModel;
    std::map<UUID, UUID> m_exportModelHandleMapping;
    QTimer* m_checkForUpdateTimer;
    QString m_debugPort;

    QComboBox * m_geometrySourceComboBox;
    QPushButton * m_newImportGeometry;
    QProgressBar * m_progressBar;
    QPushButton * m_refreshBtn;
    QPushButton * m_previewBtn;
    QPushButton * m_mergeBtn;
    QPushButton * m_debugBtn;

    QWebEngineView * m_view;
    std::shared_ptr<OSDocument> m_document;
};

} // openstudio

#endif // OPENSTUDIO_GEOMETRYEDITORVIEW_HPP
