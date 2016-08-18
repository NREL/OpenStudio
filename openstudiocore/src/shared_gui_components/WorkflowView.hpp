/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#ifndef SHAREDGUICOMPONENTS_VARIABLEVIEW_HPP
#define SHAREDGUICOMPONENTS_VARIABLEVIEW_HPP

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

#endif // SHAREDGUICOMPONENTS_VARIABLEVIEW_HPP
