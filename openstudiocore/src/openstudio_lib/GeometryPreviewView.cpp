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

#include "GeometryPreviewView.hpp"

#include "../model/Model_Impl.hpp"
#include "../model/ThreeJS.hpp"

#include "../utilities/core/Assert.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <QWebEngineScriptCollection>

namespace openstudio {

GeometryPreviewView::GeometryPreviewView(bool isIP,
                                         const openstudio::model::Model& model,
                                         QWidget * parent)
: QWidget(parent)
{
  // TODO: DLM impliment units switching
  //connect(this, &GeometryPreviewView::toggleUnitsClicked, modelObjectInspectorView(), &ModelObjectInspectorView::toggleUnitsClicked);

  QVBoxLayout *layout = new QVBoxLayout;

  PreviewWebView* webView = new PreviewWebView(model, this);
  layout->addWidget(webView);

  setLayout(layout);
}

GeometryPreviewView::~GeometryPreviewView()
{

}

PreviewWebView::PreviewWebView(const model::Model& model, QWidget *t_parent)
  : QWidget(t_parent),
    m_model(model),
    m_isIP(true),
    m_progressBar(new QProgressBar()),
    m_refreshBtn(new QPushButton("Refresh"))
{

  auto mainLayout = new QVBoxLayout;
  setLayout(mainLayout);

  connect(m_refreshBtn, &QPushButton::clicked, this, &PreviewWebView::refreshClicked);

  auto hLayout = new QHBoxLayout(this);
  mainLayout->addLayout(hLayout);

  hLayout->addStretch();

  hLayout->addWidget(m_progressBar, 0, Qt::AlignVCenter);
  m_progressBar->setMinimum(0);
  m_progressBar->setMaximum(100);
  m_progressBar->setValue(0);
  m_progressBar->setVisible(false); // make visible when load first page

  hLayout->addWidget(m_refreshBtn, 0, Qt::AlignVCenter);
  m_refreshBtn->setVisible(true);

  m_view = new QWebEngineView(this);
  m_view->settings()->setAttribute(QWebEngineSettings::WebAttribute::LocalContentCanAccessRemoteUrls, true);
  m_view->settings()->setAttribute(QWebEngineSettings::WebAttribute::SpatialNavigationEnabled, true);

  connect(m_view, &QWebEngineView::loadFinished, this, &PreviewWebView::onLoadFinished);
  connect(m_view, &QWebEngineView::loadProgress, this, &PreviewWebView::onLoadProgress);
  connect(m_view, &QWebEngineView::loadStarted, this, &PreviewWebView::onLoadStarted);
  connect(m_view, &QWebEngineView::renderProcessTerminated, this, &PreviewWebView::onRenderProcessTerminated);
  
  // Qt 5.8 and higher
  //m_view->setAttribute(QWebEngineSettings::WebAttribute::AllowRunningInsecureContent, true);

  //m_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  m_view->setContextMenuPolicy(Qt::NoContextMenu);

  //mainLayout->addWidget(m_view, 10, Qt::AlignTop);
  mainLayout->addWidget(m_view);

  m_view->load(QUrl("qrc:///library/geometry_preview.html"));
}

PreviewWebView::~PreviewWebView()
{
  delete m_view;
}

void PreviewWebView::refreshClicked()
{
  m_view->triggerPageAction(QWebEnginePage::ReloadAndBypassCache);
}

void PreviewWebView::onUnitSystemChange(bool t_isIP) 
{
  LOG(Debug, "onUnitSystemChange " << t_isIP << " reloading results");
  m_isIP = t_isIP;
}

void callWithResult(const QString& result)
{
  QString x = result;
}

void PreviewWebView::onLoadFinished(bool ok)
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
  
  ThreeScene scene = modelToThreeJS(m_model, true); // triangulated
  //ThreeScene scene = modelToThreeJS(m_model, false); // non-triangulated
  std::string json = scene.toJSON(false); // no pretty print

  std::stringstream ss;
  ss << m_model;
  std::string s = ss.str();

  // call init and animate
  QString javascript = QString("init(") + QString::fromStdString(json) + QString(");\n animate();\n initDatGui();");
  m_view->page()->runJavaScript(javascript);

  //javascript = QString("os_data.metadata.version");
  //m_view->page()->runJavaScript(javascript, [](const QVariant &v) { callWithResult(v.toString()); });

}

void PreviewWebView::onLoadProgress(int progress)
{
  m_progressBar->setStyleSheet("");
  m_progressBar->setFormat("");
  m_progressBar->setTextVisible(false);
  m_progressBar->setValue(progress);
}

void PreviewWebView::onLoadStarted()
{
  m_progressBar->setStyleSheet("");
  m_progressBar->setFormat("");
  m_progressBar->setTextVisible(false);
}

void PreviewWebView::onRenderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus terminationStatus, int exitCode)
{
  m_progressBar->setStyleSheet("QProgressBar::chunk {background-color: #FF0000;}");
  m_progressBar->setFormat("Error");
  m_progressBar->setTextVisible(true);
}


} // openstudio
