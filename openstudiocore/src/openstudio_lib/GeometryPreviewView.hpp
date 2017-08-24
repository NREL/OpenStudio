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

#ifndef OPENSTUDIO_GEOMETRYPREVIEWVIEW_HPP
#define OPENSTUDIO_GEOMETRYPREVIEWVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelSubTabView.hpp"

#include "../model/Model.hpp"

#include <QWidget>
#include <QWebEngineView>
#include <QProgressBar>

class QComboBox;
class QPushButton;

namespace openstudio {

class OSDocument;

class GeometryPreviewView : public QWidget
{
  Q_OBJECT

  public:

    GeometryPreviewView(bool isIP,
                      const openstudio::model::Model& model,
                      QWidget * parent = nullptr);

    virtual ~GeometryPreviewView();

  private:

};

// main widget

class PreviewWebView : public QWidget
{
  Q_OBJECT;

  public:
    PreviewWebView(bool isIP, const openstudio::model::Model& model, QWidget *t_parent = nullptr);
    virtual ~PreviewWebView();

  public slots:
    void onUnitSystemChange(bool t_isIP);

  private slots:
    void refreshClicked();

    // DLM: for debugging
    void 	onLoadFinished(bool ok);
    //void 	onLoadProgress(int progress);
    //void 	onLoadStarted();
    void 	onTranslateProgress(double percentage);
    void 	onJavaScriptFinished(const QVariant &v);
    void 	onRenderProcessTerminated(QWebEnginePage::RenderProcessTerminationStatus terminationStatus, int exitCode);
  private:
    REGISTER_LOGGER("openstudio::PreviewWebView");

    bool m_isIP;
    model::Model m_model;

    QProgressBar * m_progressBar;
    QPushButton * m_refreshBtn;

    QWebEngineView * m_view;
    std::shared_ptr<OSDocument> m_document;

    QString m_json;
};

} // openstudio

#endif // OPENSTUDIO_GEOMETRYPREVIEWVIEW_HPP
