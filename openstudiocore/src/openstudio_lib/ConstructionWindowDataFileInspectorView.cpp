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

#include "ConstructionWindowDataFileInspectorView.hpp"

#include "../shared_gui_components/OSLineEdit.hpp"

#include "../model/WindowDataFile.hpp"
#include "../model/WindowDataFile_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// ConstructionVC

ConstructionWindowDataFileInspectorView::ConstructionWindowDataFileInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, parent),
    m_isIP(isIP),
    m_nameEdit(nullptr),
    m_urlEdit(nullptr)
{
  createLayout();
}

void ConstructionWindowDataFileInspectorView::createLayout()
{
  auto visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  auto mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7, 7, 7, 7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  // URL

  QLabel * label = new QLabel("URL: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, 2, 0);

  m_urlEdit = new OSLineEdit2();
  mainGridLayout->addWidget(m_urlEdit, 3, 0, 1, 3);

  // Stretch

  mainGridLayout->setRowStretch(100, 100);

  mainGridLayout->setColumnStretch(100, 100);
}

void ConstructionWindowDataFileInspectorView::onClearSelection()
{
  detach();

  this->stackedWidget()->setCurrentIndex(0);
}

void ConstructionWindowDataFileInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::WindowDataFile windowDataFile = modelObject.cast<model::WindowDataFile>();
  attach(windowDataFile);

  this->stackedWidget()->setCurrentIndex(1);
}

void ConstructionWindowDataFileInspectorView::onUpdate()
{
}

void ConstructionWindowDataFileInspectorView::attach(openstudio::model::WindowDataFile & windowDataFile)
{
  m_windowDataFile = windowDataFile;
  // m_nameEdit->bind(windowDataFile,"name");
  m_nameEdit->bind(
    *m_windowDataFile,
    OptionalStringGetter(std::bind(&model::WindowDataFile::name, m_windowDataFile.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::WindowDataFile::setName, m_windowDataFile.get_ptr(),std::placeholders::_1))
  );

  // m_urlEdit->bind(windowDataFile,"url");
  // Replaced to upgrade to OSLineEdit2, but fully removed at the suggestion of @danmacumber

  // m_urlEdit->bind(
  //   *m_windowDataFile,
  //   OptionalStringGetter(std::bind(&model::WindowDataFile::getURL, m_windowDataFile.get_ptr(),true)),
  //   boost::optional<StringSetter>(std::bind(&model::WindowDataFile::setURL, m_windowDataFile.get_ptr(),std::placeholders::_1))
  // );

}

void ConstructionWindowDataFileInspectorView::detach()
{
  m_urlEdit->unbind();
  m_windowDataFile = boost::none;
}

} // openstudio

