/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef SHAREDGUICOMPONENTS_VARIABLEVIEW_HPP
#define SHAREDGUICOMPONENTS_VARIABLEVIEW_HPP

#include "OSCollapsibleView.hpp"
#include "OSListView.hpp"
#include "HeaderViews.hpp"
#include <QAbstractButton>

class QLineEdit;
class QLabel;
class QPushButton;

namespace openstudio {

class MeasureType;

namespace measuretab {

class MeasuresView;
class VariableItemView;
class NewGroupDropZone;
class NewGroupView;
class VariableGroupContentView;
class MeasureDropZone;
class VariableHeaderView;
class VariableContentView;
class MeasureDropZone;
class MeasureItemButton;

} // measuretab

namespace measuretab {

// NewGroupView allows the user to add a new Variable (either for a MeasureGroup or a single fixed Measure)
// NewGroupView is instantiated underneath a VariableGroupContentView
class NewGroupView : public QWidget
{
  Q_OBJECT

  public:

  NewGroupView(bool t_fixed, bool t_fixedOnly);

  virtual ~NewGroupView() {}

  NewGroupDropZone * dropZone;
};

// RectangularDropZone styles a rectangular drop zone
class RectangularDropZone : public QWidget
{
  Q_OBJECT
  
  public:

  RectangularDropZone(bool t_fixed);

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
class NewGroupDropZone : public RectangularDropZone
{
  Q_OBJECT
  
  public:

  NewGroupDropZone(bool t_fixed);

  virtual ~NewGroupDropZone() {}
};

// VariableGroupItemView displays a VariableGroupItem, e.g. all the Model or EnergyPlus MeasureGroups/Fixed Measures
// It is configured by the VariableGroupItemDelegate
class VariableGroupItemView : public OSCollapsibleView
{
  Q_OBJECT

  public:

  VariableGroupItemView(bool t_fixed, MeasureType measureType);

  virtual ~VariableGroupItemView() {}

  LightGradientHeader * variableGroupHeader;

  VariableGroupContentView * variableGroupContentView;
};

class VariableGroupContentView : public QWidget
{
  Q_OBJECT

  public:

  VariableGroupContentView(bool t_fixedMeasuresOnly);

  virtual ~VariableGroupContentView() {}

  NewGroupView * newGroupView;
  NewGroupView * newFixedGroupView;

  OSListView * variableListView;
};

// VariableItemView displays a VariableItem (either a MeasureGroup or a fixed Measure)
// It is configured by the VariableItemDelegate
class VariableItemView : public OSCollapsibleView
{
  Q_OBJECT

public:

  VariableItemView(bool t_fixed);

  virtual ~VariableItemView() {}

  VariableHeaderView * variableHeaderView;

  VariableContentView * variableContentView;

protected:
  void paintEvent(QPaintEvent *);
};

class VariableHeaderView : public OSHeader
{
  Q_OBJECT

  public:

  VariableHeaderView(bool t_fixed);

  virtual ~VariableHeaderView() {}

  QLineEdit * variableNameEdit;

  QPushButton * removeButton;

  QPushButton * upButton;

  QPushButton * downButton;

  OSListView * measureListView;

  private:

  bool m_fixed;
};

class VariableContentView : public QWidget
{
  Q_OBJECT

  public:

  VariableContentView(bool t_fixed);

  virtual ~VariableContentView() {}

  OSListView * measureListView;

  MeasureDropZone * dropZone;

  protected:

  void paintEvent(QPaintEvent *) override;
};

// MeasureItemView displays a MeasureItem (individual measure)
// It is configured by MeasureItemDelegate
class MeasureItemView : public QWidget
{
  Q_OBJECT

public:

  MeasureItemView(bool t_fixed);

  virtual ~MeasureItemView() {}

  MeasureItemButton * measureItemButton;

  QPushButton * duplicateButton;

  QPushButton * removeButton;

protected:

  void paintEvent(QPaintEvent *) override;

};

class MeasureDropZone : public RectangularDropZone
{
  Q_OBJECT

  public:

  MeasureDropZone();

  virtual ~MeasureDropZone() {}
};

class MeasureItemButton : public QAbstractButton
{
  Q_OBJECT

  public:

  MeasureItemButton(bool t_fixed);

  virtual ~MeasureItemButton() {}

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
