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

#include "GeometryEditorView.hpp"
#include "GeometryPreviewView.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelMerger.hpp"
#include "../model/ThreeJSReverseTranslator.hpp"
#include "../model/FloorplanJSForwardTranslator.hpp"
#include "../model/FloorplanJSForwardTranslator.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/PlanarSurfaceGroup.hpp"
#include "../model/PlanarSurfaceGroup_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/ShadingSurfaceGroup_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Checksum.hpp"
#include "../utilities/geometry/FloorplanJS.hpp"
#include "../utilities/geometry/ThreeJS.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <QDialog>
#include <QComboBox>
#include <QMessageBox>
#include <QTimer>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWebEnginePage>
#include <QWebEngineSettings>

namespace openstudio {

GeometryEditorView::GeometryEditorView(bool isIP,
                                     const openstudio::model::Model& model,
                                     QWidget * parent)
: QWidget(parent)
{
  // TODO: DLM impliment units switching
  //connect(this, &GeometryEditorView::toggleUnitsClicked, modelObjectInspectorView(), &ModelObjectInspectorView::toggleUnitsClicked);

  QVBoxLayout *layout = new QVBoxLayout;
  
  EditorWebView* webView = new EditorWebView(model, this);
  layout->addWidget(webView);

  setLayout(layout);
}

GeometryEditorView::~GeometryEditorView()
{

}

EditorWebView::EditorWebView(const openstudio::model::Model& model, QWidget *t_parent)
  : QWidget(t_parent),
    m_model(model),
    m_isIP(true),
    m_geometryEditorStarted(false),
    m_geometryEditorLoaded(false),
    m_javascriptRunning(false),
    m_geometrySourceComboBox(new QComboBox()),
    m_newImportGeometry(new QPushButton("New")),
    m_progressBar(new QProgressBar()),
    m_refreshBtn(new QPushButton("Refresh")),
    m_previewBtn(new QPushButton("Preview")),
    m_mergeBtn(new QPushButton("Merge"))
{
  openstudio::OSAppBase * app = OSAppBase::instance();
  OS_ASSERT(app);
  m_document = app->currentDocument();
  OS_ASSERT(m_document);

  auto mainLayout = new QVBoxLayout;
  setLayout(mainLayout);

  connect(m_geometrySourceComboBox, &QComboBox::currentTextChanged, this, &EditorWebView::geometrySourceChanged);
  connect(m_newImportGeometry, &QPushButton::clicked, this, &EditorWebView::newImportClicked);
  connect(m_refreshBtn, &QPushButton::clicked, this, &EditorWebView::refreshClicked);
  connect(m_previewBtn, &QPushButton::clicked, this, &EditorWebView::previewClicked);
  connect(m_mergeBtn, &QPushButton::clicked, this, &EditorWebView::mergeClicked);

  auto hLayout = new QHBoxLayout(this);
  mainLayout->addLayout(hLayout);

  m_geometrySourceComboBox->addItem("Floorplan");
  m_geometrySourceComboBox->setCurrentIndex(0);
  hLayout->addWidget(m_geometrySourceComboBox);

  hLayout->addWidget(m_newImportGeometry);

  hLayout->addStretch();

  hLayout->addWidget(m_progressBar, 0, Qt::AlignVCenter);
  m_progressBar->setMinimum(0);
  m_progressBar->setMaximum(100);
  m_progressBar->setValue(0);
  m_progressBar->setVisible(false); // make visible when load first page

  hLayout->addWidget(m_refreshBtn, 0, Qt::AlignVCenter);
  m_refreshBtn->setVisible(true);
  m_refreshBtn->setEnabled(false);

  hLayout->addWidget(m_previewBtn, 0, Qt::AlignVCenter);
  m_previewBtn->setVisible(true);
  m_previewBtn->setEnabled(false);

  hLayout->addWidget(m_mergeBtn, 0, Qt::AlignVCenter);
  m_mergeBtn->setVisible(true);
  m_mergeBtn->setEnabled(false);

  m_view = new QWebEngineView(this);
  m_view->settings()->setAttribute(QWebEngineSettings::WebAttribute::LocalContentCanAccessRemoteUrls, true);
  m_view->settings()->setAttribute(QWebEngineSettings::WebAttribute::SpatialNavigationEnabled, true);

  connect(m_view, &QWebEngineView::loadFinished, this, &EditorWebView::onLoadFinished);
  connect(m_view, &QWebEngineView::loadProgress, this, &EditorWebView::onLoadProgress);
  connect(m_view, &QWebEngineView::loadStarted, this, &EditorWebView::onLoadStarted);
  connect(m_view, &QWebEngineView::renderProcessTerminated, this, &EditorWebView::onRenderProcessTerminated);
  
  // Qt 5.8 and higher
  //m_view->setAttribute(QWebEngineSettings::WebAttribute::AllowRunningInsecureContent, true);

  //m_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_view->setContextMenuPolicy(Qt::NoContextMenu);

  //mainLayout->addWidget(m_view, 10, Qt::AlignTop);
  mainLayout->addWidget(m_view);

  connect(m_document.get(), &OSDocument::modelSaving, this, &EditorWebView::saveClickedBlocking);

  m_checkForUpdateTimer = new QTimer(this);
  connect(m_checkForUpdateTimer, SIGNAL(timeout()), this, SLOT(checkForUpdate()));

  openstudio::path p = floorplanPath();
  if (exists(p)){
    openstudio::filesystem::ifstream ifs(p);
    OS_ASSERT(ifs.is_open());
    std::string contents((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    ifs.close();
    m_floorplan = FloorplanJS::load(contents);

    if (m_floorplan)
    {
      // floorplan loaded correctly

      // update with current model content
      model::FloorplanJSForwardTranslator ft;
      m_floorplan = ft.updateFloorplanJSResources(*m_floorplan, m_model);
      
      // start the editor
      m_newImportGeometry->setEnabled(false);
      m_geometryEditorStarted = true;
      m_geometryEditorLoaded = false;
      m_view->load(QUrl("qrc:///library/embeddable_geometry_editor.html"));
    } else {
      m_newImportGeometry->setEnabled(false);
      m_view->setHtml(QString("Failed to open existing floorplan."));
    }

  } else {
    if ((model.getConcreteModelObjects<model::Space>().size() > 0) || (model.getConcreteModelObjects<model::ShadingSurfaceGroup>().size() > 0) || (model.getConcreteModelObjects<model::BuildingStory>().size() > 0)){
      m_newImportGeometry->setEnabled(false);
      m_view->setHtml(QString("Model has existing geometry, floorplan editor disabled."));
    } else{
      m_view->setHtml(QString("Create new floorplan to begin."));
    }
    
  }
}

EditorWebView::~EditorWebView()
{
  saveClickedBlocking("");
  delete m_view;
}

void EditorWebView::geometrySourceChanged(const QString& text)
{
  if (text == "Floorplan"){
    m_newImportGeometry->setText("New");
  }
}

void EditorWebView::newImportClicked()
{
  if (m_geometrySourceComboBox->currentText() == "Floorplan"){
    m_newImportGeometry->setEnabled(false);

    m_geometryEditorStarted = true;
    m_geometryEditorLoaded = false;

    m_view->load(QUrl("qrc:///library/embeddable_geometry_editor.html"));

    onChanged();
  }
}


void EditorWebView::refreshClicked()
{
  m_view->triggerPageAction(QWebEnginePage::ReloadAndBypassCache);
}

void EditorWebView::saveClickedBlocking(const openstudio::path&)
{
  if (m_geometryEditorLoaded && !m_javascriptRunning){
    m_javascriptRunning = true;
    m_document->disable();
    QString javascript = QString("JSON.stringify(api.doExport());");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_export = v; this->saveExport(); m_javascriptRunning = false; });
    while (m_javascriptRunning){
      OSAppBase::instance()->processEvents();
    }
    m_document->enable();
  }
}

void EditorWebView::previewClicked()
{
  if (m_geometryEditorLoaded && !m_javascriptRunning){
    m_previewBtn->setEnabled(false);

    m_javascriptRunning = true;
    m_document->disable();
    QString javascript = QString("JSON.stringify(api.doExport());");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_export = v; QTimer::singleShot(0, this, &EditorWebView::previewExport); m_javascriptRunning = false; });

    // re-enable document in previewExport
  }
}

void EditorWebView::mergeClicked()
{
  if (m_geometryEditorLoaded && !m_javascriptRunning){
    m_mergeBtn->setEnabled(false);

    m_javascriptRunning = true;
    m_document->disable();
    QString javascript = QString("JSON.stringify(api.doExport());");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_export = v; QTimer::singleShot(0, this, &EditorWebView::mergeExport); m_javascriptRunning = false; });

     // re-enable document in mergeExport
  }
}

void EditorWebView::translateExport()
{
  std::string json = m_export.value<QString>().toStdString();

  model::ThreeJSReverseTranslator rt;
  boost::optional<model::Model> model;
  boost::optional<FloorplanJS> floorplan = FloorplanJS::load(json);
  if (floorplan){
    ThreeScene scene = floorplan->toThreeScene(true);
    model = rt.modelFromThreeJS(scene);
  } else{
    // DLM: this is an error, the editor produced a JSON we can't read
  }
  
  if (model){
    m_exportModel = *model;
    m_exportModelHandleMapping = rt.handleMapping();
  } else{
    // DLM: this is an error, either floorplan was empty or could not be translated
    m_exportModel = model::Model();
    m_exportModelHandleMapping.clear();
  }
  
}

void EditorWebView::startEditor()
{
   m_document->disable();

  // set config
  {
    OS_ASSERT(!m_javascriptRunning);

    m_javascriptRunning = true;

    std::string json = "{";
    if (m_floorplan){
      json += "showMapDialogOnStart: false";
    } else{
      json += "showMapDialogOnStart: true";
    }
    json += "}";

    QString javascript = QString("window.api.setConfig(") + QString::fromStdString(json) + QString(");");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_javascriptRunning = false; });
    while (m_javascriptRunning){
      OSAppBase::instance()->processEvents();
    }
  }

  // start the app
  {
    OS_ASSERT(!m_javascriptRunning);

    m_javascriptRunning = true;

    QString javascript = QString("window.api.init();");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_javascriptRunning = false; });
    while (m_javascriptRunning){
      OSAppBase::instance()->processEvents();
    }
  }

  // create library from current model
  {
    OS_ASSERT(!m_javascriptRunning);

    if (m_floorplan){

      // import the current floorplan
      m_javascriptRunning = true;

      std::string json = m_floorplan->toJSON(false);

      QString javascript = QString("window.api.doImport(JSON.stringify(") + QString::fromStdString(json) + QString("));");
      m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_javascriptRunning = false; });
      while (m_javascriptRunning){
        OSAppBase::instance()->processEvents();
      }

    } else{

      // import current model content as library
      FloorplanJS floorplan;
      model::FloorplanJSForwardTranslator ft;
      floorplan = ft.updateFloorplanJSResources(floorplan, m_model);

      m_javascriptRunning = true;

      std::string json = floorplan.toJSON(false);

      QString javascript = QString("window.api.importLibrary(JSON.stringify(") + QString::fromStdString(json) + QString("));");
      m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_javascriptRunning = false; });
      while (m_javascriptRunning){
        OSAppBase::instance()->processEvents();
      }
    }
  }

  // start checking for updates
  m_versionNumber = 0;
  m_checkForUpdateTimer->start(1000);

  m_document->enable();
}

void EditorWebView::saveExport()
{
  if (!m_export.isNull()){

    std::string contents = m_export.value<QString>().toStdString();

    // DLM: should we compare checksums and only 
    openstudio::path out = floorplanPath();
    if (!out.empty()){
      if (checksum(contents) != checksum(out)){

        // DLM: what if this fails?
        m_floorplan = FloorplanJS::load(contents);

        openstudio::filesystem::ofstream file(out);
        OS_ASSERT(file.is_open());
        file << contents;
        file.close();

        onChanged();
      }
    }
  }
}

void EditorWebView::previewExport()
{
  // translate the exported floorplan
  translateExport();

  // merge export model into clone of m_model
  bool keepHandles = true;
  model::Model temp = m_model.clone(keepHandles).cast<model::Model>();
  model::ModelMerger mm;
  mm.mergeModels(temp, m_exportModel, m_exportModelHandleMapping);

  // do not update floorplan since this is not a real merge

  // save the exported floorplan
  saveExport();

  PreviewWebView* webView = new PreviewWebView(temp);
  QLayout* layout = new QVBoxLayout();
  layout->addWidget(webView);

  QDialog* dialog = new QDialog(this, Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
  dialog->setModal(true);
  dialog->setWindowTitle("Geometry Preview");
  dialog->setLayout(layout);
  dialog->open();

  m_document->enable();
  m_previewBtn->setEnabled(true);
}

void EditorWebView::mergeExport()
{
  // translate the exported floorplan
  translateExport();

  // merge export model into m_model
  model::ModelMerger mm;
  mm.mergeModels(m_model, m_exportModel, m_exportModelHandleMapping);

  // make sure handles get updated in floorplan and the exported string
  model::FloorplanJSForwardTranslator ft;
  m_floorplan = ft.updateFloorplanJSResources(*m_floorplan, m_model);
  m_export = QString::fromStdString(m_floorplan->toJSON());

  // save the exported floorplan
  saveExport();

  m_document->enable();
  m_mergeBtn->setEnabled(true);
}

void EditorWebView::checkForUpdate()
{
  if (!m_javascriptRunning){
    m_javascriptRunning = true;
    
    // DLM: don't disable and enable the tabs, too distracting
    //m_document->disable();

    unsigned currentVersionNumber = m_versionNumber;

    QString javascript = QString("window.versionNumber;");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_versionNumber = v.toUInt();  m_javascriptRunning = false; });
    while (m_javascriptRunning){
      // DLM: instead ignore user events
      OSAppBase::instance()->processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    //m_document->enable();

    if (currentVersionNumber != m_versionNumber){
      onChanged();
    }
  }
}

void EditorWebView::onChanged()
{
  m_document->markAsModified();
}

void EditorWebView::onUnitSystemChange(bool t_isIP) 
{
  LOG(Debug, "onUnitSystemChange " << t_isIP << " reloading results");
  m_isIP = t_isIP;
}

void EditorWebView::onLoadFinished(bool ok)
{
  OS_ASSERT(!m_javascriptRunning);

  QString title = m_view->title();
  if (ok){
    if (m_geometryEditorStarted){

      // can't call javascript now, page is still loading
      QTimer::singleShot(0, this, &EditorWebView::startEditor);

      m_geometryEditorLoaded = true;
      m_refreshBtn->setEnabled(true);
      m_previewBtn->setEnabled(true);
      m_mergeBtn->setEnabled(true);
    }

    m_progressBar->setStyleSheet("");
    m_progressBar->setFormat("");
    m_progressBar->setVisible(false);
    m_progressBar->setTextVisible(false);
  } else{
    m_progressBar->setStyleSheet("QProgressBar::chunk {background-color: #FF0000;}");
    m_progressBar->setFormat("Error");
    m_progressBar->setVisible(true);
    m_progressBar->setTextVisible(true);
  }
}

void EditorWebView::onLoadProgress(int progress)
{
  m_progressBar->setStyleSheet("");
  m_progressBar->setFormat("");
  m_progressBar->setTextVisible(false);
  m_progressBar->setValue(progress);
}

void EditorWebView::onLoadStarted()
{
  m_progressBar->setStyleSheet("");
  m_progressBar->setFormat("");
  m_progressBar->setVisible(true);
  m_progressBar->setTextVisible(false);
}

void EditorWebView::onRenderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus terminationStatus, int exitCode)
{
  m_progressBar->setStyleSheet("QProgressBar::chunk {background-color: #FF0000;}");
  m_progressBar->setFormat("Error");
  m_progressBar->setTextVisible(true);
}

openstudio::path EditorWebView::floorplanPath() const
{
  return toPath(m_document->modelTempDir()) / toPath("resources/floorplan.json");
}

} // openstudio
