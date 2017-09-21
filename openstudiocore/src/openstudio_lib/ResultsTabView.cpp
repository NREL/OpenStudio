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

#include "ResultsTabView.hpp"
#include "OSDocument.hpp"
#include "OSAppBase.hpp"

#include <QFile>
#include <QBoxLayout>
#include <QComboBox>
#include <QDesktopWidget>
#include <QDomDocument>
#include <QLabel>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QString>
#include <QRegExp>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include "../utilities/core/Assert.hpp"

namespace openstudio {

ResultsTabView::ResultsTabView(const QString & tabLabel,
  TabType tabType,
  QWidget * parent)
  : MainTabView(tabLabel, tabType, parent),
  m_resultsView(new ResultsView(parent))
{
  addTabWidget(m_resultsView);
  m_resultsView->setAutoFillBackground(false);

  auto savePath = OSAppBase::instance()->currentDocument()->savePath();
  if( ! savePath.isEmpty() ) {
    openstudio::path runPath = toPath(savePath).parent_path() / toPath(savePath).stem() / openstudio::toPath("run");
    openstudio::path reportsPath = toPath(savePath).parent_path() / toPath(savePath).stem() / openstudio::toPath("reports");
    m_resultsView->searchForExistingResults(runPath, reportsPath);
  }
}

void ResultsTabView::onUnitSystemChange(bool t_isIP) 
{
  LOG(Debug, "onUnitSystemChange " << t_isIP << " reloading results");
  m_resultsView->onUnitSystemChange(t_isIP);
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

ResultsView::ResultsView(QWidget *t_parent)
  : QWidget(t_parent),
    m_isIP(true),
    m_progressBar(new QProgressBar()),
    m_refreshBtn(new QPushButton("Refresh")),
    m_openDViewBtn(new QPushButton("Open DView for\nDetailed Reports"))
{

  auto mainLayout = new QVBoxLayout;
  setLayout(mainLayout);

  connect(m_refreshBtn, &QPushButton::clicked, this, &ResultsView::refreshClicked);

  connect(m_openDViewBtn, &QPushButton::clicked, this, &ResultsView::openDViewClicked);
  
  auto hLayout = new QHBoxLayout(this);
  mainLayout->addLayout(hLayout);

  m_reportLabel = new QLabel("Reports: ",this);
  m_reportLabel->setObjectName("H2");
  m_reportLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  hLayout->addWidget(m_reportLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);

  m_comboBox = new QComboBox(this);
  connect(m_comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ResultsView::comboBoxChanged);
  hLayout->addWidget(m_comboBox, 0, Qt::AlignLeft | Qt::AlignVCenter);

  hLayout->addStretch();

  hLayout->addWidget(m_progressBar, 0, Qt::AlignVCenter);
  m_progressBar->setMinimum(0);
  m_progressBar->setMaximum(100);
  m_progressBar->setValue(0);
  m_progressBar->setVisible(false); // make visible when load first page

  hLayout->addWidget(m_refreshBtn, 0, Qt::AlignVCenter);
  m_refreshBtn->setVisible(true);

  hLayout->addWidget(m_openDViewBtn, 0, Qt::AlignVCenter);

  m_view = new QWebEngineView(this);
  m_view->settings()->setAttribute(QWebEngineSettings::WebAttribute::LocalContentCanAccessRemoteUrls, true);
  m_view->settings()->setAttribute(QWebEngineSettings::WebAttribute::SpatialNavigationEnabled, true);

  connect(m_view, &QWebEngineView::loadFinished, this, &ResultsView::onLoadFinished);
  connect(m_view, &QWebEngineView::loadProgress, this, &ResultsView::onLoadProgress);
  connect(m_view, &QWebEngineView::loadStarted, this, &ResultsView::onLoadStarted);
  connect(m_view, &QWebEngineView::renderProcessTerminated, this, &ResultsView::onRenderProcessTerminated);
  
  // Qt 5.8 and higher
  //m_view->setAttribute(QWebEngineSettings::WebAttribute::AllowRunningInsecureContent, true);

  //m_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_view->setContextMenuPolicy(Qt::NoContextMenu);

  //mainLayout->addWidget(m_view, 10, Qt::AlignTop);
  mainLayout->addWidget(m_view);
  
}

ResultsView::~ResultsView()
{
  delete m_view;
}

void ResultsView::refreshClicked()
{
  m_view->triggerPageAction(QWebEnginePage::ReloadAndBypassCache);
}

void ResultsView::openDViewClicked()
{
  LOG(Debug, "openDViewClicked");

#ifdef Q_OS_MAC
  openstudio::path dview
    = openstudio::toPath(QCoreApplication::applicationDirPath()) / openstudio::toPath("../../../DView.app/Contents/MacOS/DView");
#else
  openstudio::path dview
    = openstudio::toPath(QCoreApplication::applicationDirPath()) / openstudio::toPath("DView");
#endif

  QStringList args;

  if (!m_sqlFilePath.empty())
  {
    args.push_back(openstudio::toQString(m_sqlFilePath));
  }

  if (!m_radianceResultsPath.empty())
  {
    args.push_back(openstudio::toQString(m_radianceResultsPath));
  }

  if (!QProcess::startDetached(openstudio::toQString(dview), args))
  {
    QMessageBox::critical(this, "Unable to launch DView", "DView was not found in the expected location:\n" + openstudio::toQString(dview));
  }
}

void ResultsView::onUnitSystemChange(bool t_isIP) 
{
  LOG(Debug, "onUnitSystemChange " << t_isIP << " reloading results");
  m_isIP = t_isIP;
  resultsGenerated(m_sqlFilePath, m_radianceResultsPath);
}

// need to sort paths by number so 8-UserScript-0, shows up before 11-UserScript-0
struct ResultsPathSorter
{
  bool operator()(const openstudio::path& left, const openstudio::path& right){
    openstudio::path leftParent = left.parent_path().stem();
    openstudio::path rightParent = right.parent_path().stem();

    QRegExp regexp("^(\\d)+.*");

    boost::optional<int> leftInt;
    if (regexp.exactMatch(toQString(leftParent))){
      QStringList leftParts = regexp.capturedTexts();
      OS_ASSERT(leftParts.size() == 2);
      leftInt = leftParts[1].toInt();
    }

    boost::optional<int> rightInt;
    if (regexp.exactMatch(toQString(rightParent))){
      QStringList rightParts = regexp.capturedTexts();
      OS_ASSERT(rightParts.size() == 2);
      rightInt = rightParts[1].toInt();
    }

    if (leftInt && rightInt){
      return leftInt.get() < rightInt.get();
    }else if (leftInt){
      return true;
    }else if (rightInt){
      return false;
    }

    return (left < right);
  }
};

void ResultsView::searchForExistingResults(const openstudio::path &t_runDir, const openstudio::path &t_reportsDir)
{
  LOG(Debug, "Looking for existing results in: " << openstudio::toString(t_runDir));

  std::vector<openstudio::path> eplusout;
  std::vector<openstudio::path> radout;
  std::vector<openstudio::path> reports;

  for ( openstudio::filesystem::recursive_directory_iterator end, dir(t_runDir); 
        dir != end; 
        ++dir ) 
  {
    openstudio::path p = *dir;
    if        (openstudio::toString(p.filename()) == "eplusout.sql") {
      eplusout.push_back(p);
    } else if (openstudio::toString(p.filename()) == "radout.sql") {
      radout.push_back(p);
    } else if (openstudio::toString(p.filename()) == "report.html") {
      //reports.push_back(p);
    } else if (openstudio::toString(p.filename()) == "eplustbl.htm") {
      //reports.push_back(p);
    }
  }

  LOG(Debug, "Looking for existing results in: " << openstudio::toString(t_reportsDir));

  if( openstudio::filesystem::exists(t_reportsDir) ) {
    for ( openstudio::filesystem::directory_iterator end, dir(t_reportsDir); 
          dir != end; 
          ++dir ) 
    {
      openstudio::path p = *dir;
      if (openstudio::toString(p.extension()) == ".html" || openstudio::toString(p.extension()) == ".htm") {
        reports.push_back(p);
      }
    }
  }

  // sort paths as directory iterator order is undefined
  std::sort(eplusout.begin(), eplusout.end(), ResultsPathSorter());
  std::sort(radout.begin(), radout.end(), ResultsPathSorter());
  std::sort(reports.begin(), reports.end(), ResultsPathSorter());

  openstudio::path eplus = eplusout.empty()?openstudio::path():eplusout.back();
  openstudio::path rad = radout.empty()?openstudio::path():radout.back();

  resultsGenerated(eplus, rad);

  populateComboBox(reports);
}

void ResultsView::resultsGenerated(const openstudio::path &t_path, const openstudio::path &t_radianceResultsPath)
{
  LOG(Debug, "resultsGenerated " << openstudio::toString(t_path) << " " << openstudio::toString(t_radianceResultsPath));

  m_sqlFilePath = t_path;
  m_radianceResultsPath = t_radianceResultsPath;
}

//openstudio::runmanager::RunManager ResultsView::runManager()
//{
//  return OSAppBase::instance()->project()->runManager();
//}

void ResultsView::treeChanged(const openstudio::UUID &t_uuid)
{
  //try {
  //  openstudio::runmanager::Job j = runManager().getJob(t_uuid);
  //  while (j.parent())
  //  {
  //    j = j.parent().get();
  //  }

  //  openstudio::runmanager::TreeStatusEnum status = j.treeStatus();

  //  QString fullPathString;
  //  openstudio::path path;

  //  if (status == openstudio::runmanager::TreeStatusEnum::Finished)
  //  {
  //    try {
  //      openstudio::runmanager::Files f = j.treeAllFiles().getAllByFilename("report.html");
  //      std::vector<openstudio::runmanager::FileInfo> t_files = f.files();
  //      std::vector<openstudio::path> reports;
  //      for (const openstudio::runmanager::FileInfo& file : t_files) {
  //        reports.push_back(file.fullPath);
  //      }
  //      f = j.treeAllFiles().getAllByFilename("eplustbl.htm");
  //      t_files = f.files();
  //      for (const openstudio::runmanager::FileInfo& file : t_files) {
  //        reports.push_back(file.fullPath);
  //      }
  //      populateComboBox(reports);
  //    } catch (const std::exception &e) {
  //      LOG(Debug, "Tree finished, error getting html file: " << e.what());
  //    } catch (...) {
  //      LOG(Debug, "Tree finished, error getting html file");
  //      // no html file exists
  //    }
  //  } 
  //} catch (const std::exception &e) {
  //  LOG(Debug, "Tree finished, error getting status: " << e.what());
  //} catch (...) {
  //  LOG(Debug, "Tree finished, error getting status");
  //  // no html file exists
  //}
}

void ResultsView::populateComboBox(std::vector<openstudio::path> reports)
{
  unsigned num = 0;
  QString fullPathString;
  openstudio::path path;

  m_comboBox->clear();
  for (const openstudio::path& report : reports) {

    fullPathString = toQString(report.string());
    QFile file(fullPathString);
    fullPathString.prepend("file:///");

    if (openstudio::toString(report.filename()) == "eplustbl.html" || openstudio::toString(report.filename()) == "eplustbl.htm"){
      
      m_comboBox->addItem("EnergyPlus Results",fullPathString);

    }else{
      
      ++num;

      if (file.open(QFile::ReadOnly)){
        QDomDocument doc;
        doc.setContent(&file);
        file.close();
        QString string = doc.toString();
        int startingIndex = string.indexOf("<title>");
        int endingIndex = string.indexOf("</title>");
        if((startingIndex == -1) | (endingIndex == -1) | (startingIndex >= endingIndex)){
          m_comboBox->addItem(QString("Custom Report ") + QString::number(num), fullPathString);
        } else {
          // length of "<title>" = 7
          QString title = string.mid(startingIndex+7, endingIndex-startingIndex-7);
          m_comboBox->addItem(title,fullPathString);
        }
      }
    }
  }
  if(m_comboBox->count()){
    m_comboBox->setCurrentIndex(0);
    for (int i = 0; i < m_comboBox->count(); ++i){
      if (m_comboBox->itemText(i) == QString("OpenStudio Results")){
        m_comboBox->setCurrentIndex(i);
        break;
      }
    }
    int width = m_comboBox->minimumSizeHint().width();
    m_comboBox->setMinimumWidth(width + 20);
  }
}

void ResultsView::comboBoxChanged(int index)
{
  QString filename = m_comboBox->itemData(index).toString();

  // DLM: setting html here causes a flicker, wish there was a better way to clear the current page
  //m_view->setHtml("");

  // DLM: there is a 2MB limit on content set in setHtml
  //QFile file(filename);
  //QString content;
  //if(file.open(QIODevice::ReadOnly)) {
  //  QTextStream in(&file);
  //  in.setCodec("UTF-8");
  //  content = in.readAll();
  //  file.close();
  //}
  //m_view->setHtml(content);

  m_progressBar->setVisible(true);
  m_progressBar->setStyleSheet("");
  m_progressBar->setFormat("");
  m_progressBar->setTextVisible(false);

  m_view->load(QUrl(filename));
}

void 	ResultsView::onLoadFinished(bool ok)
{
  QString title = m_view->title();
  if (ok){
    m_progressBar->setStyleSheet("");
    m_progressBar->setFormat("");
    m_progressBar->setTextVisible(false);
  } else{
    m_progressBar->setStyleSheet("QProgressBar::chunk {background-color: #FF0000;}");
    m_progressBar->setFormat("Error");
    m_progressBar->setTextVisible(true);
  }
}

void 	ResultsView::onLoadProgress(int progress)
{
  m_progressBar->setStyleSheet("");
  m_progressBar->setFormat("");
  m_progressBar->setTextVisible(false);
  m_progressBar->setValue(progress);
}

void 	ResultsView::onLoadStarted()
{
  m_progressBar->setStyleSheet("");
  m_progressBar->setFormat("");
  m_progressBar->setTextVisible(false);
}

void 	ResultsView::onRenderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus terminationStatus, int exitCode)
{
  m_progressBar->setStyleSheet("QProgressBar::chunk {background-color: #FF0000;}");
  m_progressBar->setFormat("Error");
  m_progressBar->setTextVisible(true);
}


} // openstudio
