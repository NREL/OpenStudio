/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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
