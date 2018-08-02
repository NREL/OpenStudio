/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "GeometryEditorView.hpp"
#include "GeometryPreviewView.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "MainWindow.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelMerger.hpp"
#include "../model/ThreeJSReverseTranslator.hpp"
#include "../model/ThreeJSForwardTranslator.hpp"
#include "../model/FloorplanJSForwardTranslator.hpp"
#include "../model/FloorplanJSForwardTranslator.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/PlanarSurfaceGroup.hpp"
#include "../model/PlanarSurfaceGroup_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/ShadingSurfaceGroup_Impl.hpp"
#include "../model/Site.hpp"
#include "../model/Site_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"

#include "../gbxml/ReverseTranslator.hpp"
#include "../energyplus/ReverseTranslator.hpp"
#include "../osversion/VersionTranslator.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Checksum.hpp"
#include "../utilities/bcl/RemoteBCL.hpp"
#include "../utilities/geometry/FloorplanJS.hpp"
#include "../utilities/geometry/ThreeJS.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <QFile>
#include <QDialog>
#include <QTcpServer>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <QTemporaryDir>
#include <QProcess>
#include <QProcessEnvironment>

int CHECKFORUPDATEMSEC = 5000;

namespace openstudio {

QUrl getEmbeddedFileUrl(const QString& filename)
{
  QUrl result(QString("qrc:///library/") + filename);

  QString appDir = QCoreApplication::applicationDirPath();
  QFileInfo fileInfo(appDir + QString("/") + filename);
  if (fileInfo.exists() && fileInfo.isFile()) {
    result = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
  }

  return result;
}

GeometryEditorView::GeometryEditorView(bool isIP,
                                     const openstudio::model::Model& model,
                                     QWidget * parent)
: QWidget(parent)
{
  QVBoxLayout *layout = new QVBoxLayout;

  EditorWebView* webView = new EditorWebView(isIP, model, this);
  layout->addWidget(webView);

  setLayout(layout);
}

GeometryEditorView::~GeometryEditorView()
{

}

DebugWebView::DebugWebView(const QString& debugPort, QWidget * parent)
  : QDialog(parent)
{
  auto mainLayout = new QVBoxLayout;
  setLayout(mainLayout);

  m_view = new QWebEngineView(this);
  m_view->settings()->setAttribute(QWebEngineSettings::WebAttribute::LocalContentCanAccessRemoteUrls, true);
  m_view->settings()->setAttribute(QWebEngineSettings::WebAttribute::SpatialNavigationEnabled, true);

  //mainLayout->addWidget(m_view, 10, Qt::AlignTop);
  mainLayout->addWidget(m_view);

  QString urlString = "http://127.0.0.1:" + debugPort;
  m_view->load(QUrl(urlString));
}

DebugWebView::~DebugWebView()
{

}


BaseEditor::BaseEditor(bool isIP, const openstudio::model::Model& model, QWebEngineView * view, QWidget *t_parent)
  : QObject(t_parent),
    m_editorLoaded(false),
    m_javascriptRunning(false),
    m_versionNumber(0),
    m_isIP(isIP),
    m_model(model),
    m_view(view)
{
  m_checkForUpdateTimer = new QTimer(this);
  connect(m_checkForUpdateTimer, SIGNAL(timeout()), this, SLOT(checkForUpdate()));

  openstudio::OSAppBase * app = OSAppBase::instance();
  OS_ASSERT(app);
  m_document = app->currentDocument();
  OS_ASSERT(m_document);
}

BaseEditor::~BaseEditor()
{}

bool BaseEditor::editorLoaded() const
{
  return m_editorLoaded;
}

bool BaseEditor::javascriptRunning() const
{
  return m_javascriptRunning;
}

bool BaseEditor::blockUpdateTimerSignals(bool block){
  return m_checkForUpdateTimer->blockSignals(block);
}

model::Model BaseEditor::exportModel() const
{
  return m_exportModel;
}

std::map<UUID, UUID> BaseEditor::exportModelHandleMapping() const
{
  return m_exportModelHandleMapping;
}

void BaseEditor::onChanged()
{
  m_document->markAsModified();
}

FloorspaceEditor::FloorspaceEditor(const openstudio::path& floorplanPath, bool isIP, const openstudio::model::Model& model, QWebEngineView * view, QWidget *t_parent)
  : BaseEditor(isIP, model, view, t_parent),
  m_floorplanPath(floorplanPath)
{
  m_document->disable();

  if (exists(m_floorplanPath)){

    openstudio::filesystem::ifstream ifs(m_floorplanPath);
    OS_ASSERT(ifs.is_open());
    std::string contents((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    ifs.close();
    m_floorplan = FloorplanJS::load(contents);

    if (m_floorplan)
    {
      // floorplan loaded correctly

      // update with current model content
      // at this point you may have removed objects in the app, so tell updateFloorplanJS to remove missing objects
      model::FloorplanJSForwardTranslator ft;
      m_floorplan = ft.updateFloorplanJS(*m_floorplan, m_model, true);

      QString errorsAndWarnings;
      for (const auto& error : ft.errors()){
        errorsAndWarnings += QString::fromStdString(error.logMessage() + "\n");
      }
      for (const auto& warning : ft.warnings()){
        errorsAndWarnings += QString::fromStdString(warning.logMessage() + "\n");
      }
      if (!errorsAndWarnings.isEmpty()){
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Updating Floorplan", errorsAndWarnings);
      }

      //std::string contents2 = m_floorplan->toJSON();

      // start loading the editor, will trigger EditorWebView::onLoadFinished when done
      m_view->load(getEmbeddedFileUrl("embeddable_geometry_editor.html"));
    } else {
      m_view->setHtml(QString("Failed to open existing floorplan."));
    }

  } else {

    // new floorplan, will trigger EditorWebView::onLoadFinished when done
    m_view->load(getEmbeddedFileUrl("embeddable_geometry_editor.html"));
  }

  m_document->enable();
}

FloorspaceEditor::~FloorspaceEditor()
{}

void FloorspaceEditor::loadEditor()
{
  // set config
  {
    OS_ASSERT(!m_javascriptRunning);

    m_javascriptRunning = true;

    Json::Value config(Json::objectValue);
    config["showImportExport"] = false;

    if (m_floorplan){
      config["showMapDialogOnStart"] = false;
    } else{
      config["showMapDialogOnStart"] = true;
    }

    // DLM: need a better check here
    RemoteBCL remoteBCL;
    if (remoteBCL.isOnline()){
      config["online"] = true;
    } else{
      config["online"] = false;
    }

    if (m_isIP){
      config["units"] = "ip";
      config["initialGridSize"] = 15;
    }else{
      config["units"] = "si";
      config["initialGridSize"] = 5;
    }

    boost::optional<model::Site> site = m_model.getOptionalUniqueModelObject<model::Site>();
    if (site){
      double latitude = site->latitude();
      double longitude = site->longitude();
      if ((latitude != 0) && (longitude != 0)){
        Json::Value defaultLocation(Json::objectValue);
        defaultLocation["latitude"] = latitude;
        defaultLocation["longitude"] = longitude;
        config["defaultLocation"] = defaultLocation;
      }
    }

    config["initialNorthAxis"] = m_model.getUniqueModelObject<model::Building>().northAxis();

    Json::FastWriter writer;
    std::string json = writer.write(config);

    QString javascript = QString("window.api.setConfig(") + QString::fromStdString(json) + QString(");");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_javascriptRunning = false; });
    while (m_javascriptRunning){
      OSAppBase::instance()->processEvents(QEventLoop::ExcludeUserInputEvents, 200);
    }
  }

  // start the app
  {
    OS_ASSERT(!m_javascriptRunning);

    m_javascriptRunning = true;

    QString javascript = QString("window.api.init();");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_javascriptRunning = false; });
    while (m_javascriptRunning){
      OSAppBase::instance()->processEvents(QEventLoop::ExcludeUserInputEvents, 200);
    }
  }

  // customize css
  {
    OS_ASSERT(!m_javascriptRunning);

    m_javascriptRunning = true;

    QFile cssFile(":/library/geometry_editor.css");
    bool test = cssFile.open(QFile::ReadOnly | QFile::Text);
    OS_ASSERT(test);
    QTextStream cssStream(&cssFile);
    QString css = cssStream.readAll();
    cssFile.close();

    QString javascript = "const rules = `\n";
    javascript += css;
    javascript += "`;\n\
const style = document.createElement('style')\n\
style.type = 'text/css';\n\
style.innerHTML = rules;\n\
document.head.appendChild(style);\n";

    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_javascriptRunning = false; });
    while (m_javascriptRunning){
      OSAppBase::instance()->processEvents(QEventLoop::ExcludeUserInputEvents, 200);
    }
  }

  // create library from current model
  {
    OS_ASSERT(!m_javascriptRunning);

    if (m_floorplan){

      // import the current floorplan
      m_javascriptRunning = true;

      std::string json = m_floorplan->toJSON(false);

      QString javascript = QString("window.api.openFloorplan(JSON.stringify(") + QString::fromStdString(json) + QString("), { noReloadGrid: false });");
      m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_javascriptRunning = false; });
      while (m_javascriptRunning){
        OSAppBase::instance()->processEvents(QEventLoop::ExcludeUserInputEvents, 200);
      }

    } else{

      // import current model content as library
      FloorplanJS floorplan;
      floorplan.setUnits("si");

      model::FloorplanJSForwardTranslator ft;
      floorplan = ft.updateFloorplanJS(floorplan, m_model, false);

      QString errorsAndWarnings;
      for (const auto& error : ft.errors()){
        errorsAndWarnings += QString::fromStdString(error.logMessage() + "\n");
      }
      for (const auto& warning : ft.warnings()){
        errorsAndWarnings += QString::fromStdString(warning.logMessage() + "\n");
      }
      if (!errorsAndWarnings.isEmpty()){
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), QString("Updating Floorplan"), errorsAndWarnings);
      }

      m_javascriptRunning = true;

      std::string json = floorplan.toJSON(false);
      // DLM: temp
      //Json::Value value;
      //Json::Reader reader;
      //bool parsingSuccessful = reader.parse(json, value);
      //value.removeMember("stories");
      //Json::FastWriter writer;
      //json = writer.write(value);

      QString javascript = QString("window.api.importLibrary(JSON.stringify(") + QString::fromStdString(json) + QString("));");
      m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_javascriptRunning = false; });
      while (m_javascriptRunning){
        OSAppBase::instance()->processEvents(QEventLoop::ExcludeUserInputEvents, 200);
      }
    }
  }

  m_editorLoaded = true;

  // start checking for updates
  m_versionNumber = 0;
  m_checkForUpdateTimer->start(CHECKFORUPDATEMSEC);
}

void FloorspaceEditor::doExport()
{
  if (m_editorLoaded && !m_javascriptRunning){
    m_javascriptRunning = true;
    m_document->disable();
    QString javascript = QString("JSON.stringify(window.api.exportFloorplan());");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {
      m_export = v;
      m_javascriptRunning = false;
    });
    while (m_javascriptRunning){
      OSAppBase::instance()->processEvents(QEventLoop::ExcludeUserInputEvents, 200);
    }
    m_document->enable();

    // DLM: what if this fails?
    std::string contents = m_export.value<QString>().toStdString();
    m_floorplan = FloorplanJS::load(contents);

    if (!m_floorplan){
      // DLM: This is an error
      bool t = false;
    }

  } else{
      // DLM: This is an error
      bool t = false;
  }
}

void FloorspaceEditor::saveExport()
{
  if (!m_export.isNull()){

    std::string contents = m_export.value<QString>().toStdString();

    // DLM: should we compare checksums and only
    openstudio::path out = m_floorplanPath;
    if (!out.empty()){
      if (checksum(contents) != checksum(out)){

        openstudio::filesystem::ofstream file(out);
        OS_ASSERT(file.is_open());
        file << contents;
        file.close();

        onChanged();
      }
    }
  }
}

void FloorspaceEditor::translateExport()
{
  model::ThreeJSReverseTranslator rt;
  boost::optional<model::Model> model;
  if (m_floorplan){
    ThreeScene scene = m_floorplan->toThreeScene(true);
    model = rt.modelFromThreeJS(scene);

    if (model){
      // set north axis
      model->getUniqueModelObject<model::Building>().setNorthAxis(m_floorplan->northAxis());

      // TODO: synchronize latitude and longitude
    }
  } else{
    // DLM: this is an error, the editor produced a JSON we can't read
  }

  QString errorsAndWarnings;
  for (const auto& error : rt.errors()){
    errorsAndWarnings += QString::fromStdString(error.logMessage() + "\n");
  }
  for (const auto& warning : rt.warnings()){
    errorsAndWarnings += QString::fromStdString(warning.logMessage() + "\n");
  }
  if (!errorsAndWarnings.isEmpty()){
    QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Creating Model From Floorplan", errorsAndWarnings);
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

void FloorspaceEditor::updateModel(const openstudio::model::Model& model)
{
  if (m_floorplan){

    m_model = model;

    // make sure handles get updated in floorplan and the exported string
    model::FloorplanJSForwardTranslator ft;
    m_floorplan = ft.updateFloorplanJS(*m_floorplan, m_model, false);

    QString errorsAndWarnings;
    for (const auto& error : ft.errors()){
      errorsAndWarnings += QString::fromStdString(error.logMessage() + "\n");
    }
    for (const auto& warning : ft.warnings()){
      errorsAndWarnings += QString::fromStdString(warning.logMessage() + "\n");
    }
    if (!errorsAndWarnings.isEmpty()){
      QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Updating Floorplan", errorsAndWarnings);
    }

    OS_ASSERT(!m_javascriptRunning);
    m_javascriptRunning = true;

    // import updated floorplan back into editor
    OS_ASSERT(m_floorplan);
    std::string json = m_floorplan->toJSON(false);

    QString javascript = QString("window.api.openFloorplan(JSON.stringify(") + QString::fromStdString(json) + QString("), { noReloadGrid: true });");
    //QString javascript = QString("window.api.importLibrary(JSON.stringify(") + QString::fromStdString(json) + QString("));");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_javascriptRunning = false; });
    while (m_javascriptRunning){
      OSAppBase::instance()->processEvents(QEventLoop::ExcludeUserInputEvents, 200);
    }
  }
}

void FloorspaceEditor::checkForUpdate()
{
  if (!m_javascriptRunning){
    m_javascriptRunning = true;

    // DLM: don't disable and enable the tabs, too distracting
    //m_document->disable();

    unsigned currentVersionNumber = m_versionNumber;

    QString javascript = QString("window.versionNumber;");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_versionNumber = v.toUInt();  m_javascriptRunning = false; });

    // DLM: the javascript engine appears to stop when a file dialog is launched by the editor (e.g. to import an image)
    int processEventsTime = 200;
    boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();
    while (m_javascriptRunning){
      // if the javascript is taking too long to evaluate, bail out here
      // DLM: multiply elapsed time by two, we want to make sure we have half of the time between update checks to process other events
      int elapsed = 2 * (boost::posix_time::microsec_clock::universal_time() - start).total_milliseconds();
      if (elapsed > CHECKFORUPDATEMSEC){
        break;
      }
      // DLM: instead ignore user events
      // DLM: this is a problem during file dialogs launched by the editor
      OSAppBase::instance()->processEvents(QEventLoop::ExcludeUserInputEvents, processEventsTime);
    }

    //m_document->enable();

    if (m_javascriptRunning){
      // DLM: do not set this to false, we will require the javascript to complete
      //m_javascriptRunning = false;
    } else{
      if (currentVersionNumber != m_versionNumber){
        onChanged();
      }
    }
  }
}


GbXmlEditor::GbXmlEditor(const openstudio::path& gbXmlPath, bool isIP, const openstudio::model::Model& model, QWebEngineView * view, QWidget *t_parent)
  : BaseEditor(isIP, model, view, t_parent),
  m_gbXmlPath(gbXmlPath)
{
  m_document->disable();

  if (exists(m_gbXmlPath)){
    openstudio::filesystem::ifstream ifs(m_gbXmlPath);
    OS_ASSERT(ifs.is_open());
    std::string contents((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    m_gbXML = toQString(contents).simplified().replace(QString("\""), QString("\\\""));
    ifs.close();
  }

  // start loading the editor, will trigger EditorWebView::onLoadFinished when done
  m_view->load(getEmbeddedFileUrl("embeddable_gbxml_editor.html"));
  //m_view->load(QUrl("file:///E:/openstudio2/openstudiocore/src/openstudio_lib/library/embeddable_gbxml_editor.html"));

  m_document->enable();
}

GbXmlEditor::~GbXmlEditor()
{}

void GbXmlEditor::loadEditor()
{
  {
    OS_ASSERT(!m_javascriptRunning);
    // call init and animate
    QString javascript = QString("init();\n animate();");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_javascriptRunning = false; });
    while (m_javascriptRunning){
      OSAppBase::instance()->processEvents(QEventLoop::ExcludeUserInputEvents, 200);
    }
  }

  if (!m_gbXML.isEmpty()){
    OS_ASSERT(!m_javascriptRunning);

    m_javascriptRunning = true;

    QString javascript = QString("setGbXml(\"") + m_gbXML + QString("\");");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_javascriptRunning = false; });
    while (m_javascriptRunning){
      OSAppBase::instance()->processEvents(QEventLoop::ExcludeUserInputEvents, 200);
    }
  }

  m_editorLoaded = true;

  // start checking for updates
  //m_versionNumber = 0;
  //m_checkForUpdateTimer->start(CHECKFORUPDATEMSEC);

}

void GbXmlEditor::doExport()
{
  // no-op since we aren't editing anything
}

void GbXmlEditor::saveExport()
{
  // no-op since we aren't editing anything
}

void GbXmlEditor::translateExport()
{

  gbxml::ReverseTranslator rt;
  boost::optional<model::Model> model;

  model = rt.loadModel(m_gbXmlPath);

  QString errorsAndWarnings;
  for (const auto& error : rt.errors()){
    errorsAndWarnings += QString::fromStdString(error.logMessage() + "\n");
  }
  for (const auto& warning : rt.warnings()){
    errorsAndWarnings += QString::fromStdString(warning.logMessage() + "\n");
  }

  if (model){
    m_exportModel = *model;
    model::ModelMerger mm;
    m_exportModelHandleMapping = mm.suggestHandleMapping(m_model, m_exportModel);
  } else{
    // DLM: this is an error, either gbxml was empty or could not be translated
    errorsAndWarnings += QString("Model could not be loaded\n");
    m_exportModel = model::Model();
    m_exportModelHandleMapping.clear();
  }

  if (!errorsAndWarnings.isEmpty()){
    QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Creating Model From gbXML", errorsAndWarnings);
  }
}

void GbXmlEditor::updateModel(const openstudio::model::Model& model)
{
  // no-op for now
}

void GbXmlEditor::checkForUpdate()
{
  // no-op since we aren't editing anything
}

IdfEditor::IdfEditor(const openstudio::path& idfPath, bool forceConvert, bool isIP, const openstudio::model::Model& model, QWebEngineView * view, QWidget *t_parent)
  : BaseEditor(isIP, model, view, t_parent),
  m_idfPath(idfPath)
{
  m_document->disable();

  QFileInfo fi(toQString(m_idfPath));
  QString jdfName = fi.fileName();
  jdfName.replace(".idf", ".epJSON");
  openstudio::path jdfPath = toPath(fi.absolutePath()) / toPath(jdfName);

  QString qJdfPath = toQString(jdfPath);
  if (QFile::exists(qJdfPath)) {
    QFileInfo fi2(qJdfPath);

    if ((forceConvert) || (fi2.lastModified() < fi.lastModified())){
      QFile::remove(qJdfPath);
    }
  }

  if (exists(m_idfPath)){

    if (!exists(jdfPath)){

      QTemporaryDir tempDir( QString ( QDir::tempPath() + "/XXXXXX" ) );
      tempDir.setAutoRemove(true);
      if (tempDir.isValid()) {

        openstudio::path jdfTempPath = toPath(tempDir.path()) / toPath(jdfName);

        QProcess runner;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

        QString energyPlusExePath = toQString(getEnergyPlusExecutable());

        runner.setProcessEnvironment(env);
        runner.setWorkingDirectory(tempDir.path());

        QStringList arguments;
        arguments << QString("-c") << fi.absoluteFilePath();

        //m_document->disableTabsDuringRun();

        runner.start(energyPlusExePath, arguments);
        runner.waitForFinished();
        if (runner.state()){
          // if still running just kill it
          runner.kill();
        }
        int result = runner.exitCode();
        QString error(runner.readAllStandardError());
        QString output(runner.readAllStandardOutput());

        //m_document->enableTabsAfterRun();

        if (exists(jdfTempPath)) {
          QString qJdfTempPath = toQString(jdfTempPath);
          QFile::copy(qJdfTempPath, qJdfPath);
        }
      }
    }

    if (exists(jdfPath)){
      openstudio::filesystem::ifstream ifs(jdfPath);
      OS_ASSERT(ifs.is_open());
      std::string contents((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
      m_jdf = toQString(contents).simplified(); // .replace(QString("\""), QString("\\\""));
      ifs.close();
    }
  }

  // start loading the editor, will trigger EditorWebView::onLoadFinished when done
  m_view->load(getEmbeddedFileUrl("embeddable_idf_editor.html"));
  //m_view->load(QUrl("file:///E:/openstudio2/openstudiocore/src/openstudio_lib/library/embeddable_idf_editor.html"));

  m_document->enable();
}

IdfEditor::~IdfEditor()
{}

void IdfEditor::loadEditor()
{

  if (m_jdf.isEmpty() || m_jdf.isNull() || m_jdf == "null"){

    OS_ASSERT(!m_javascriptRunning);

    m_javascriptRunning = true;

    QString javascript = QString("setMessage(\"Failed to convert IDF to JSON format\");");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_javascriptRunning = false; });
    while (m_javascriptRunning){
      OSAppBase::instance()->processEvents(QEventLoop::ExcludeUserInputEvents, 200);
    }

  } else {
    OS_ASSERT(!m_javascriptRunning);

    m_javascriptRunning = true;

    QString javascript = QString("setJdf(JSON.stringify(") + m_jdf + QString("));");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_javascriptRunning = false; });
    while (m_javascriptRunning){
      OSAppBase::instance()->processEvents(QEventLoop::ExcludeUserInputEvents, 200);
    }
  }

  m_editorLoaded = true;

  // start checking for updates
  //m_versionNumber = 0;
  //m_checkForUpdateTimer->start(CHECKFORUPDATEMSEC);

}

void IdfEditor::doExport()
{
  // no-op since we aren't editing anything
}

void IdfEditor::saveExport()
{
  // no-op since we aren't editing anything
}

void IdfEditor::translateExport()
{

  energyplus::ReverseTranslator rt;
  boost::optional<model::Model> model;

  model = rt.loadModel(m_idfPath);

  QString errorsAndWarnings;
  for (const auto& error : rt.errors()){
    errorsAndWarnings += QString::fromStdString(error.logMessage() + "\n");
  }
  for (const auto& warning : rt.warnings()){
    errorsAndWarnings += QString::fromStdString(warning.logMessage() + "\n");
  }

  if (model){
    m_exportModel = *model;
    model::ModelMerger mm;
    m_exportModelHandleMapping = mm.suggestHandleMapping(m_model, m_exportModel);
  } else{
    // DLM: this is an error, either idf was empty or could not be translated
    errorsAndWarnings += QString("Model could not be loaded\n");
    m_exportModel = model::Model();
    m_exportModelHandleMapping.clear();
  }

  if (!errorsAndWarnings.isEmpty()){
    QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Creating Model From IDF", errorsAndWarnings);
  }
}

void IdfEditor::updateModel(const openstudio::model::Model& model)
{
  // no-op for now
}

void IdfEditor::checkForUpdate()
{
  // no-op since we aren't editing anything
}

OsmEditor::OsmEditor(const openstudio::path& osmPath, bool isIP, const openstudio::model::Model& model, QWebEngineView * view, QWidget *t_parent)
  : BaseEditor(isIP, model, view, t_parent),
    m_osmPath(osmPath)
{
  m_document->disable();

  if (exists(osmPath)){
    osversion::VersionTranslator vt;
    boost::optional<model::Model> optModel = vt.loadModel(osmPath);

    QString errorsAndWarnings;
    for (const auto& error : vt.errors()){
      errorsAndWarnings += QString::fromStdString(error.logMessage() + "\n");
    }
    for (const auto& warning : vt.warnings()){
      errorsAndWarnings += QString::fromStdString(warning.logMessage() + "\n");
    }

    if (optModel){
      m_exportModel = *optModel;
      model::ModelMerger mm;
      m_exportModelHandleMapping = mm.suggestHandleMapping(m_model, m_exportModel);
    } else{
      errorsAndWarnings += QString("Model could not be loaded\n");
      m_exportModel = model::Model();
      m_exportModelHandleMapping.clear();
    }

    if (!errorsAndWarnings.isEmpty()){
      QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Creating Model From OSM", errorsAndWarnings);
    }
  }

  // start loading the editor, will trigger EditorWebView::onLoadFinished when done
  m_view->load(getEmbeddedFileUrl("geometry_preview.html"));

  m_document->enable();
}

OsmEditor::~OsmEditor()
{}

void OsmEditor::loadEditor()
{

  {
    OS_ASSERT(!m_javascriptRunning);

    model::ThreeJSForwardTranslator ft;
    ThreeScene scene = ft.modelToThreeJS(m_exportModel, true); // triangulated
    std::string json = scene.toJSON(false); // no pretty print

    // call init and animate
    QString javascript = QString("init(") + toQString(json) + QString(");\n animate();\n initDatGui();");
    m_view->page()->runJavaScript(javascript, [this](const QVariant &v) {m_javascriptRunning = false; });
    while (m_javascriptRunning){
      OSAppBase::instance()->processEvents(QEventLoop::ExcludeUserInputEvents, 200);
    }
  }

  m_editorLoaded = true;

  // start checking for updates
  //m_versionNumber = 0;
  //m_checkForUpdateTimer->start(CHECKFORUPDATEMSEC);

}

void OsmEditor::doExport()
{
  // no-op since we aren't editing anything
}

void OsmEditor::saveExport()
{
  // no-op since we aren't editing anything
}

void OsmEditor::translateExport()
{
  // happens in ctor
}

void OsmEditor::updateModel(const openstudio::model::Model& model)
{
  // no-op for now
}

void OsmEditor::checkForUpdate()
{
  // no-op since we aren't editing anything
}


EditorWebView::EditorWebView(bool isIP, const openstudio::model::Model& model, QWidget *t_parent)
  : QWidget(t_parent),
    m_model(model),
    m_baseEditor(nullptr),
    m_isIP(isIP),
    m_geometrySourceComboBox(new QComboBox()),
    m_newImportGeometry(new QPushButton("New")),
    m_progressBar(new QProgressBar()),
    m_refreshBtn(new QPushButton("Refresh")),
    m_previewBtn(new QPushButton("Preview")),
    m_mergeBtn(new QPushButton("Merge")),
    m_debugBtn(new QPushButton("Debug"))
{
  openstudio::OSAppBase * app = OSAppBase::instance();
  OS_ASSERT(app);
  m_document = app->currentDocument();
  OS_ASSERT(m_document);

  // find available port for debugging
  m_debugPort = QString(qgetenv("QTWEBENGINE_REMOTE_DEBUGGING"));

  auto mainLayout = new QVBoxLayout;
  setLayout(mainLayout);

  connect(m_document.get(), &OSDocument::toggleUnitsClicked, this, &EditorWebView::onUnitSystemChange);
  connect(m_geometrySourceComboBox, &QComboBox::currentTextChanged, this, &EditorWebView::geometrySourceChanged);
  connect(m_newImportGeometry, &QPushButton::clicked, this, &EditorWebView::newImportClicked);
  connect(m_refreshBtn, &QPushButton::clicked, this, &EditorWebView::refreshClicked);
  connect(m_previewBtn, &QPushButton::clicked, this, &EditorWebView::previewClicked);
  connect(m_mergeBtn, &QPushButton::clicked, this, &EditorWebView::mergeClicked);
  connect(m_debugBtn, &QPushButton::clicked, this, &EditorWebView::debugClicked);

  auto hLayout = new QHBoxLayout(this);
  mainLayout->addLayout(hLayout);

  m_geometrySourceComboBox->addItem("Floorplan");
  m_geometrySourceComboBox->addItem("gbXML");
  m_geometrySourceComboBox->addItem("IDF");
  m_geometrySourceComboBox->addItem("OSM");
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

  hLayout->addWidget(m_debugBtn, 0, Qt::AlignVCenter);
  if (m_debugPort.isEmpty()){
    m_debugBtn->setVisible(false);
    m_debugBtn->setEnabled(false);
  } else{
    m_debugBtn->setVisible(true);
    m_debugBtn->setEnabled(false);
  }

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

  // check if floorplan exists
  openstudio::path p = floorplanPath();
  if (exists(p)){
    m_geometrySourceComboBox->setCurrentText("Floorplan");
    m_geometrySourceComboBox->setEnabled(false);
    m_newImportGeometry->setEnabled(false);

    m_baseEditor = new FloorspaceEditor(p, m_isIP, m_model, m_view, this);

    // editor will be started when page load finishes
    return;
  }

  // check if gbXml exists
   p = gbXmlPath();
  if (exists(p)){
    m_geometrySourceComboBox->setCurrentText("gbXML");
    m_geometrySourceComboBox->setEnabled(false);
    m_newImportGeometry->setEnabled(true);

    m_baseEditor = new GbXmlEditor(p, m_isIP, m_model, m_view, this);

    // editor will be started when page load finishes
    return;
  }

  // check if idf exists
   p = idfPath();
  if (exists(p)){
    m_geometrySourceComboBox->setCurrentText("IDF");
    m_geometrySourceComboBox->setEnabled(false);
    m_newImportGeometry->setEnabled(true);

    m_baseEditor = new IdfEditor(p, false, m_isIP, m_model, m_view, this);

    // editor will be started when page load finishes
    return;
  }

  // check if osm exists
  p = osmPath();
  if (exists(p)){
    m_geometrySourceComboBox->setCurrentText("OSM");
    m_geometrySourceComboBox->setEnabled(false);
    m_newImportGeometry->setEnabled(true);

    m_baseEditor = new OsmEditor(p, m_isIP, m_model, m_view, this);

    // editor will be started when page load finishes
    return;
  }

  // no files found
  if ((model.getConcreteModelObjects<model::Surface>().size() > 0) || (model.getConcreteModelObjects<model::SubSurface>().size() > 0) || (model.getConcreteModelObjects<model::ShadingSurface>().size() > 0)){
    m_newImportGeometry->setEnabled(false);
    m_view->load(getEmbeddedFileUrl("geometry_editor_start.html"));
  } else{
    m_view->load(getEmbeddedFileUrl("geometry_editor_start.html"));
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
  }else if (text == "gbXML"){
    m_newImportGeometry->setText("Import");
  }else if (text == "IDF"){
    m_newImportGeometry->setText("Import");
  }else if (text == "OSM"){
    m_newImportGeometry->setText("Import");
  }
}

void EditorWebView::newImportClicked()
{
  if (m_baseEditor) {
    delete m_baseEditor;
  }

  if (m_geometrySourceComboBox->currentText() == "Floorplan"){
    m_geometrySourceComboBox->setEnabled(false);
    m_newImportGeometry->setEnabled(false);

    m_baseEditor = new FloorspaceEditor(floorplanPath(), m_isIP, m_model, m_view, this);

    onChanged();

    // editor will be started when page load finishes
    return;
  }

  if (m_geometrySourceComboBox->currentText() == "gbXML"){

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                m_document->savePath(),
                                                tr("gbXML (*.xml *.gbxml)"));
    if (fileName.isEmpty()) {
      // canceled
      return;
    }

    openstudio::path op = gbXmlPath();
    QString p = toQString(op);
    if (QFile::exists(p)) {
      QFile::remove(p);
    }
    QFile::copy(fileName, p);

    m_geometrySourceComboBox->setEnabled(false);
    m_newImportGeometry->setEnabled(true);

    m_baseEditor = new GbXmlEditor(op, m_isIP, m_model, m_view, this);

    onChanged();

    // editor will be started when page load finishes
    return;
  }

  if (m_geometrySourceComboBox->currentText() == "IDF"){

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                m_document->savePath(),
                                                tr("IDF (*.idf)"));
    if (fileName.isEmpty()) {
      // canceled
      return;
    }

    openstudio::path op = idfPath();
    QString p = toQString(op);
    if (QFile::exists(p)) {
      QFile::remove(p);
    }
    QFile::copy(fileName, p);

    m_geometrySourceComboBox->setEnabled(false);
    m_newImportGeometry->setEnabled(true);

    m_baseEditor = new IdfEditor(op, true, m_isIP, m_model, m_view, this);

    onChanged();

    // editor will be started when page load finishes
    return;
  }

  if (m_geometrySourceComboBox->currentText() == "OSM"){

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                m_document->savePath(),
                                                tr("OSM (*.osm)"));
    if (fileName.isEmpty()) {
      // canceled
      return;
    }

    openstudio::path op = osmPath();
    QString p = toQString(op);
    if (QFile::exists(p)) {
      QFile::remove(p);
    }
    QFile::copy(fileName, p);

    m_geometrySourceComboBox->setEnabled(false);
    m_newImportGeometry->setEnabled(true);

    m_baseEditor = new OsmEditor(op, m_isIP, m_model, m_view, this);

    onChanged();

    // editor will be started when page load finishes
    return;
  }
}


void EditorWebView::refreshClicked()
{
  m_view->triggerPageAction(QWebEnginePage::ReloadAndBypassCache);
}

void EditorWebView::saveClickedBlocking(const openstudio::path&)
{
  if (m_baseEditor && m_baseEditor->editorLoaded()){
    m_baseEditor->doExport();
    m_baseEditor->saveExport();
  }
}

void EditorWebView::previewClicked()
{
  if (m_baseEditor && m_baseEditor->editorLoaded()){
    m_baseEditor->doExport();
    previewExport();
  }

}

void EditorWebView::mergeClicked()
{
  if (m_baseEditor && m_baseEditor->editorLoaded()){
    m_baseEditor->doExport();
    mergeExport();
  }
}

void EditorWebView::debugClicked()
{
  DebugWebView debugWebView(m_debugPort, m_document->mainWindow());
  debugWebView.exec();
}

void EditorWebView::previewExport()
{
  if (m_baseEditor && m_baseEditor->editorLoaded()){

    // do the export
    m_baseEditor->doExport();

    // translate the exported floorplan
    m_baseEditor->translateExport();

    // merge export model into clone of m_model
    bool keepHandles = true;
    model::Model temp = m_model.clone(keepHandles).cast<model::Model>();
    model::ModelMerger mm;
    mm.mergeModels(temp, m_baseEditor->exportModel(), m_baseEditor->exportModelHandleMapping());

    QString errorsAndWarnings;
    for (const auto& error : mm.errors()){
      errorsAndWarnings += QString::fromStdString(error.logMessage() + "\n");
    }
    for (const auto& warning : mm.warnings()){
      errorsAndWarnings += QString::fromStdString(warning.logMessage() + "\n");
    }
    if (!errorsAndWarnings.isEmpty()){
      QMessageBox::warning(this, "Merging Models", errorsAndWarnings);
    }

    // do not update floorplan since this is not a real merge

    // save the exported floorplan
    m_baseEditor->saveExport();

    bool signalsBlocked = m_baseEditor->blockUpdateTimerSignals(true);

    PreviewWebView* webView = new PreviewWebView(m_isIP, temp);
    QLayout* layout = new QVBoxLayout();
    layout->addWidget(webView);

    // show preview in blocking dialog
    QDialog dialog(this, Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    dialog.setModal(true);
    dialog.setWindowTitle("Geometry Preview");
    dialog.setLayout(layout);
    dialog.exec();

    delete webView;
    delete layout;

    m_baseEditor->blockUpdateTimerSignals(signalsBlocked);

    m_document->enable();
    m_previewBtn->setEnabled(true);
  }
}

void EditorWebView::mergeExport()
{
  if (m_baseEditor && m_baseEditor->editorLoaded()){

    // do the export
    m_baseEditor->doExport();

    // translate the exported floorplan
    m_baseEditor->translateExport();

    // merge export model into m_model
    model::ModelMerger mm;
    mm.mergeModels(m_model, m_baseEditor->exportModel(), m_baseEditor->exportModelHandleMapping());

    QString errorsAndWarnings;
    for (const auto& error : mm.errors()){
      errorsAndWarnings += QString::fromStdString(error.logMessage() + "\n");
    }
    for (const auto& warning : mm.warnings()){
      errorsAndWarnings += QString::fromStdString(warning.logMessage() + "\n");
    }
    if (!errorsAndWarnings.isEmpty()){
      QMessageBox::warning(this, "Merging Models", errorsAndWarnings);
    } else{
      // DLM: print out a better report
      QMessageBox::information(this, "Merging Models", "Models Merged");
    }

    // update the editor with merged model (potentially has new handles)
    m_baseEditor->updateModel(m_model);

    // DLM: call doExport again just to be sure we get the freshest content
    m_baseEditor->doExport();

    // save the exported floorplan
    m_baseEditor->saveExport();

    m_document->enable();
    m_mergeBtn->setEnabled(true);
  }
}

void EditorWebView::onChanged()
{
  m_document->markAsModified();
}

void EditorWebView::onUnitSystemChange(bool t_isIP)
{
  if (m_baseEditor){
    QMessageBox::warning(this, "Units Change", "Changing unit system for existing floorplan is not currently supported.  Reload tab to change units.");
  }else{
    m_isIP = t_isIP;
  }
}

void EditorWebView::onLoadFinished(bool ok)
{

  QString title = m_view->title();
  if (ok){
    if (m_baseEditor){

      // can't call javascript now, page is still loading
      QTimer::singleShot(0, m_baseEditor, &BaseEditor::loadEditor);

      m_refreshBtn->setEnabled(true);
      m_previewBtn->setEnabled(true);
      m_mergeBtn->setEnabled(true);
      m_debugBtn->setEnabled(true);
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

openstudio::path EditorWebView::gbXmlPath() const
{
  return toPath(m_document->modelTempDir()) / toPath("resources/gbXML.xml");
}

openstudio::path EditorWebView::idfPath() const
{
  return toPath(m_document->modelTempDir()) / toPath("resources/geometry.idf");
}

openstudio::path EditorWebView::osmPath() const
{
  return toPath(m_document->modelTempDir()) / toPath("resources/geometry.osm");
}

} // openstudio
