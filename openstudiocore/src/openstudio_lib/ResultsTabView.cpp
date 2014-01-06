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

#include <openstudio_lib/ResultsTabView.hpp>

#include <openstudio_lib/OSDocument.hpp>

#include "OSAppBase.hpp"

#include <QBoxLayout>
#include <QComboBox>
#include <QDomDocument>
#include <QLabel>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QString>
#include <QWebView>

#include <runmanager/lib/FileInfo.hpp>
#include <runmanager/lib/JobStatusWidget.hpp>
#include <runmanager/lib/RunManager.hpp>

#include <utilities/core/ApplicationPathHelpers.hpp>
#include <utilities/core/Assert.hpp>

#include <boost/filesystem.hpp>

namespace openstudio {

ResultsTabView::ResultsTabView(const QString & tabLabel,
                               bool hasSubTab,
                               QWidget * parent)
                               : MainTabView(tabLabel,hasSubTab,parent),
                                 m_resultsView(new ResultsView())
{
  addTabWidget(m_resultsView);
  m_resultsView->setAutoFillBackground(false);

  bool isConnected = false;
  isConnected = connect(this, SIGNAL(treeChanged(const openstudio::UUID &)),
    m_resultsView, SLOT(treeChanged(const openstudio::UUID &)));
  OS_ASSERT(isConnected);

}

void ResultsTabView::searchForExistingResults(const openstudio::path &t_runDir)
{
  LOG(Debug, "searchForExistingResults " << openstudio::toString(t_runDir));
  m_resultsView->searchForExistingResults(t_runDir);
}

void ResultsTabView::onUnitSystemChange(bool t_isIP) 
{
  LOG(Debug, "onUnitSystemChange " << t_isIP << " reloading results");
  m_resultsView->onUnitSystemChange(t_isIP);
}

void ResultsTabView::resultsGenerated(const openstudio::path &t_sqlFilePath, const openstudio::path &t_radianceResultsPath)
{
  LOG(Debug, "resultsGenerated " << openstudio::toString(t_sqlFilePath) << " " << openstudio::toString(t_radianceResultsPath));
  m_resultsView->resultsGenerated(t_sqlFilePath, t_radianceResultsPath);
}

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

ResultsView::ResultsView(QWidget *t_parent)
  : QWidget(t_parent),
    m_isIP(true),
    m_openResultsViewerBtn(new QPushButton("Open ResultsViewer\nfor Detailed Reports"))
{
  bool isConnected = false;

  QVBoxLayout * mainLayout = new QVBoxLayout;
  setLayout(mainLayout);

  QHBoxLayout * hLayout = 0;

  QWidget * widget = 0;

  isConnected = connect(m_openResultsViewerBtn, SIGNAL(clicked()),
      this, SLOT(openResultsViewerClicked()));
  OS_ASSERT(isConnected);
  
  hLayout = new QHBoxLayout(this);
  mainLayout->addLayout(hLayout);

  m_reportLabel = new QLabel("Reports: ",this);
  m_reportLabel->setObjectName("H2");
  m_reportLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
  hLayout->addWidget(m_reportLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);

  m_comboBox = new QComboBox(this);
  isConnected = connect(m_comboBox, SIGNAL(currentIndexChanged( int )),
    this, SLOT(comboBoxChanged( int )));
  OS_ASSERT(isConnected);
  hLayout->addWidget(m_comboBox, 0, Qt::AlignLeft | Qt::AlignVCenter);

  hLayout->addStretch();

  hLayout->addWidget(m_openResultsViewerBtn, 0, Qt::AlignVCenter);

  m_view = new QWebView(this);
  m_view->setContextMenuPolicy(Qt::NoContextMenu);
  m_view->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  mainLayout->addWidget(m_view, 0, Qt::AlignTop);

//#if _DEBUG || (__GNUC__ && !NDEBUG)
//  m_view->page()->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
//  QWebInspector *inspector = new QWebInspector;
//  inspector->setPage(m_view->page());
//  inspector->setVisible(true);
//#endif

  mainLayout->addStretch();
}

void ResultsView::openResultsViewerClicked()
{
  LOG(Debug, "openResultsViewerClicked");

#ifdef Q_OS_MAC
  openstudio::path resultsviewer
    = openstudio::getApplicationRunDirectory() / openstudio::toPath("../../../ResultsViewer.app/Contents/MacOS/ResultsViewer");
#else
  openstudio::path resultsviewer
    = openstudio::getApplicationRunDirectory() / openstudio::toPath("ResultsViewer");
#endif

  QStringList args;

  // instruct ResultsViewer to make its own copies of the sql files passed in and to clean them up
  // when done
  args.push_back("--maketempcopies"); 

  if (!m_sqlFilePath.empty())
  {
    args.push_back(openstudio::toQString(m_sqlFilePath));
  }

  if (!m_radianceResultsPath.empty())
  {
    args.push_back(openstudio::toQString(m_radianceResultsPath));
  }

  if (!QProcess::startDetached(openstudio::toQString(resultsviewer), args))
  {
    QMessageBox::critical(this, "Unable to launch ResultsViewer", "ResultsViewer was not found in the expected location:\n" + openstudio::toQString(resultsviewer));
  }
}

void ResultsView::onUnitSystemChange(bool t_isIP) 
{
  LOG(Debug, "onUnitSystemChange " << t_isIP << " reloading results");
  m_isIP = t_isIP;
  resultsGenerated(m_sqlFilePath, m_radianceResultsPath);
}

void ResultsView::searchForExistingResults(const openstudio::path &t_runDir)
{
  LOG(Debug, "Looking for existing results in: " << openstudio::toString(t_runDir));

  std::vector<openstudio::path> eplusout;
  std::vector<openstudio::path> radout;
  std::vector<openstudio::path> reports;

  for ( boost::filesystem::basic_recursive_directory_iterator<openstudio::path> end, dir(t_runDir); 
        dir != end; 
        ++dir ) 
  {
    openstudio::path p = *dir;
    if (openstudio::toString(p.filename()) == "eplusout.sql")
    {
      eplusout.push_back(p);
    } else if (openstudio::toString(p.filename()) == "radout.sql") {
      radout.push_back(p);
    } else if (openstudio::toString(p.filename()) == "report.html") {
      reports.push_back(p);
    } else if (openstudio::toString(p.filename()) == "eplusout.html") {
      reports.push_back(p);
    }
  }

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

openstudio::runmanager::RunManager ResultsView::runManager()
{
  return OSAppBase::instance()->project()->runManager();
}

void ResultsView::treeChanged(const openstudio::UUID &t_uuid)
{
  try {
    openstudio::runmanager::Job j = runManager().getJob(t_uuid);
    while (j.parent())
    {
      j = j.parent().get();
    }

    openstudio::runmanager::TreeStatusEnum status = j.treeStatus();

    QString fullPathString;
    openstudio::path path;

    if (status == openstudio::runmanager::TreeStatusEnum::Finished)
    {
      try {
        openstudio::runmanager::Files f = j.treeAllFiles().getAllByFilename("report.html");
        std::vector<openstudio::runmanager::FileInfo> t_files = f.files();
        std::vector<openstudio::path> reports;
        Q_FOREACH(openstudio::runmanager::FileInfo file, t_files){
          reports.push_back(file.fullPath);
        }
        f = j.treeAllFiles().getAllByFilename("eplusout.html");
        t_files = f.files();
        Q_FOREACH(openstudio::runmanager::FileInfo file, t_files){
          reports.push_back(file.fullPath);
        }
        populateComboBox(reports);
      } catch (const std::exception &e) {
        LOG(Debug, "Tree finished, error getting html file: " << e.what());
      } catch (...) {
        LOG(Debug, "Tree finished, error getting html file");
        // no html file exists
      }
    } 
  } catch (const std::exception &e) {
    LOG(Debug, "Tree finished, error getting status: " << e.what());
  } catch (...) {
    LOG(Debug, "Tree finished, error getting status");
    // no html file exists
  }
}

void ResultsView::populateComboBox(std::vector<openstudio::path> reports)
{
  QString num;
  QString fullPathString;
  openstudio::path path;

  m_comboBox->clear();
  Q_FOREACH(openstudio::path report, reports){
    fullPathString = toQString(report.string());
    QFile file(fullPathString);
    fullPathString.prepend("file:///");
    if (file.open(QFile::ReadOnly)){
      QDomDocument doc;
      doc.setContent(&file);
      file.close();
      QString string = doc.toString();
      int startingIndex = string.indexOf("<title>");
      int endingIndex = string.indexOf("</title>");
      if((startingIndex == -1) | (endingIndex == -1) | (startingIndex >= endingIndex)){
        m_comboBox->addItem(num.setNum(m_comboBox->count() + 1),fullPathString);
      } else {
        // length of "<title>" = 7
        QString title = string.mid(startingIndex+7, endingIndex-startingIndex-7);
        m_comboBox->addItem(title,fullPathString);
      }
    }
  }
  if(m_comboBox->count()){
    m_comboBox->setCurrentIndex(0);
    int width = m_comboBox->minimumSizeHint().width();
    m_comboBox->setMinimumWidth(width + 20);
  }
}

void ResultsView::comboBoxChanged(int index)
{
  QString filename = m_comboBox->itemData(index).toString();
  m_view->load(QUrl(filename));
}

} // openstudio
