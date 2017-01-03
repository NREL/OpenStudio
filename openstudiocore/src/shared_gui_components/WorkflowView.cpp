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

#include "WorkflowView.hpp"
#include "OSViewSwitcher.hpp"
#include "OSListView.hpp"
#include "HeaderViews.hpp"
#include "Buttons.hpp"
#include "LocalLibraryController.hpp"
#include <QLabel>
#include <QLineEdit>
#include <QMimeData>
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QDragEnterEvent>

namespace openstudio{
  
namespace measuretab {

RectangularDropZone::RectangularDropZone()
  : QWidget()
{
  setAcceptDrops(true);

  setFixedHeight(30);

  setStyleSheet("openstudio--measuretab--RectangularDropZone {background: #F4F4F4; border: 2px dashed #A5A5A5;}");

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(5,5,5,5);
  mainHLayout->setSpacing(5);
  setLayout(mainHLayout);

  nameLabel = new QLabel();
  mainHLayout->addWidget(nameLabel);
}

void RectangularDropZone::setAcceptedMimeType(const QString & type)
{
  m_acceptedMimeType = type;
}

void RectangularDropZone::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void RectangularDropZone::dropEvent(QDropEvent * event)
{
  emit dataDropped(event);
}

void RectangularDropZone::dragEnterEvent(QDragEnterEvent * event)
{
  if(event->mimeData()->hasFormat(m_acceptedMimeType))
  {
    event->accept();
  }
}

NewMeasureDropZone::NewMeasureDropZone()
  : RectangularDropZone()
{
  nameLabel->setText("<i>Drop Measure From Library to Create a New Always Run Measure</i>");
  nameLabel->setStyleSheet("QLabel {color: #7D7D7D; }");
}

WorkflowSectionView::WorkflowSectionView(MeasureType measureType)
  : OSCollapsibleView()
{
  header = new LightGradientHeader();
  header->setMeasureType(measureType);
  setHeader(header);

  content = new WorkflowSectionContentView();
  setContent(content);

  setExpanded(true);
}

WorkflowSectionContentView::WorkflowSectionContentView()
  : QWidget(),
    workflowStepsView(nullptr),
    newMeasureDropZone(nullptr)
{
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(0,10,0,10);
  mainVLayout->setSpacing(0);
  setLayout(mainVLayout);

  // Workflow Steps

  workflowStepsView = new OSListView(false);
  mainVLayout->addWidget(workflowStepsView);

  newMeasureDropZone = new NewMeasureDropZone();
  mainVLayout->addWidget(newMeasureDropZone);
} 

WorkflowStepView::WorkflowStepView()
  : QWidget()
{
  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(0,0,0,0);
  mainHLayout->setSpacing(5);
  setLayout(mainHLayout);

  workflowStepButton = new WorkflowStepButton();
  mainHLayout->addWidget(workflowStepButton);

  duplicateButton = new SofterDuplicateButton();
  mainHLayout->addWidget(duplicateButton);

  removeButton = new SofterRemoveButton();
  mainHLayout->addWidget(removeButton);

  mainHLayout->addSpacing(50);

  downButton = new DownButton();
  mainHLayout->addWidget(downButton);

  upButton = new UpButton();
  mainHLayout->addWidget(upButton);

  duplicateButton->setVisible(false);
  removeButton->setVisible(true);
}

void WorkflowStepView::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

WorkflowStepButton::WorkflowStepButton()
  : QAbstractButton()
{
  setFixedHeight(25);

  setHasEmphasis(false);

  auto mainHLayout = new QHBoxLayout();
  mainHLayout->setContentsMargins(5,5,5,5);
  mainHLayout->setSpacing(5);
  setLayout(mainHLayout);

  nameLabel = new QLabel();
  nameLabel->setStyleSheet("font:italic");

  mainHLayout->addWidget(nameLabel);

  mainHLayout->addStretch();

  cautionLabel = new QLabel();
  cautionLabel->setPixmap(QPixmap(":/shared_gui_components/images/warning_icon.png"));
  mainHLayout->addWidget(cautionLabel);
  cautionLabel->setVisible(false);
}

void WorkflowStepButton::setHasEmphasis(bool hasEmphasis)
{
  if( hasEmphasis )
  {
    setStyleSheet("openstudio--measuretab--WorkflowStepButton { background: #FECD60; border: 2px solid #EE641A; }");
  }
  else
  {
    setStyleSheet("openstudio--measuretab--WorkflowStepButton {background: #E0E0E0; }");
  }
}

void WorkflowStepButton::paintEvent(QPaintEvent * e)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


} // measuretab

} // openstudio

