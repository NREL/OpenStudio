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

#ifndef SHAREDGUICOMPONENTS_WORKFLOWVIEW_HPP
#define SHAREDGUICOMPONENTS_WORKFLOWVIEW_HPP

#include "OSCollapsibleView.hpp"
#include "OSListView.hpp"
#include "HeaderViews.hpp"
#include "WorkflowView.hpp"
#include <QAbstractButton>

class QLineEdit;
class QLabel;
class QPushButton;

namespace openstudio {

class MeasureType;

namespace measuretab {
  
  class WorkflowSectionContentView;
  class WorkflowStepButton;

} // measuretab

namespace measuretab {

// RectangularDropZone styles a rectangular drop zone
class RectangularDropZone : public QWidget
{
  Q_OBJECT
  
  public:

  RectangularDropZone();

  virtual ~RectangularDropZone() {}

  void setAcceptedMimeType(const QString & type);

  QLabel * nameLabel;

  signals:

  void dataDropped(QDropEvent * event);

  protected:

  void paintEvent(QPaintEvent *) override;

  void dropEvent(QDropEvent * event) override;

  void dragEnterEvent(QDragEnterEvent * event) override;

  private:

  QString m_acceptedMimeType;
};

// Accepts drop of a measure and emits signal which is connected by VariableGroupItemDelegate
class NewMeasureDropZone : public RectangularDropZone
{
  Q_OBJECT
  
  public:

  NewMeasureDropZone();

  virtual ~NewMeasureDropZone() {}
};

// WorkflowSectionView displays a WorkflowSectionItem, e.g. all the Model or EnergyPlus Measures
class WorkflowSectionView : public OSCollapsibleView
{
  Q_OBJECT

  public:

  WorkflowSectionView(MeasureType measureType);

  virtual ~WorkflowSectionView() {}

  LightGradientHeader * header;

  WorkflowSectionContentView * content;
};

class WorkflowSectionContentView : public QWidget
{
  Q_OBJECT

  public:

  WorkflowSectionContentView();

  virtual ~WorkflowSectionContentView() {}

  OSListView * workflowStepsView;

  NewMeasureDropZone * newMeasureDropZone;

};

// WorkflowStepView displays a WorkflowStepItem
class WorkflowStepView : public QWidget
{
  Q_OBJECT

public:

  WorkflowStepView();

  virtual ~WorkflowStepView() {}

  WorkflowStepButton * workflowStepButton;

  QPushButton * duplicateButton;

  QPushButton * removeButton;

  QPushButton * upButton;

  QPushButton * downButton;

protected:

  void paintEvent(QPaintEvent *) override;

};

class WorkflowStepButton : public QAbstractButton
{
  Q_OBJECT

  public:

  WorkflowStepButton();

  virtual ~WorkflowStepButton() {}

  QLabel * nameLabel;

  QLabel * cautionLabel;

  public slots:

  void setHasEmphasis(bool hasEmphasis);

  protected:

  void paintEvent(QPaintEvent * e) override;
};


} // measuretab

} // openstudio

#endif // SHAREDGUICOMPONENTS_WORKFLOWVIEW_HPP
