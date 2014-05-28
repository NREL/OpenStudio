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

#ifndef PATAPP_DESIGNALTERNATIVESVIEW_HPP
#define PATAPP_DESIGNALTERNATIVESVIEW_HPP

#include "PatMainTabView.hpp"
#include "../shared_gui_components/OSCollapsibleView.hpp"
#include <QAbstractButton>

class QPushButton;
class QLineEdit;
class QTextEdit;

namespace openstudio {

class OSListView;
class DarkGradientHeader;

namespace pat {

namespace altstab {

  class DesignAltsView;

}


class DesignAlternativesTabView : public PatMainTabView
{
  Q_OBJECT

  public:

  DesignAlternativesTabView();
  virtual ~DesignAlternativesTabView() {}

  QPushButton * selectAllButton;

  QPushButton * clearSelectionsButton;

  OSListView * measuresListView;

  altstab::DesignAltsView * designAltsView;
};

namespace altstab {

// The thing on the right side of the main splitter
class DesignAltsView : public QWidget
{
  Q_OBJECT

  public:

  DesignAltsView();
  virtual ~DesignAltsView() {}

  OSListView * designAltsListView;

  QPushButton * createOneForEachSelectedMeasureButton;

  QPushButton * createOneWithSelectedMeasuresButton;

  QPushButton * createFromFileButton;
};

class VariableGroupItemView : public OSCollapsibleView
{
  Q_OBJECT

  public:

  VariableGroupItemView();
  virtual ~VariableGroupItemView() {}

  DarkGradientHeader * variableGroupHeaderView;

  OSListView * variableGroupContentView;
};

class VariableItemView : public OSCollapsibleView
{
  Q_OBJECT

  public:

  VariableItemView();
  virtual ~VariableItemView() {}

  QLabel * variableHeaderView;

  OSListView * variableContentView;
};

class MeasureItemView : public QAbstractButton
{
  Q_OBJECT

  public:

  MeasureItemView();
  virtual ~MeasureItemView() {}

  QLabel * label;

  public slots:

  void setHasEmphasis(bool hasEmphasis);

  protected:

  void paintEvent(QPaintEvent * e);
};

class DesignAltHeaderView : public OSHeader
{
  Q_OBJECT

  public:

  DesignAltHeaderView(bool isBaseline);
  virtual ~DesignAltHeaderView() {}

  QLineEdit * designAltNameEdit;
  QPushButton * removeButton;
};

class DesignAltContentView : public QWidget
{
  Q_OBJECT

  public:

  DesignAltContentView(bool isBaseline);
  virtual ~DesignAltContentView() {}

  OSListView * perturbationListView;

  QTextEdit * descriptionTextEdit;

  signals:

  void descriptionChanged(const QString & description);

  private slots:

  void onDescriptionTextChanged();
};

class DesignAltItemView : public OSCollapsibleView
{
  Q_OBJECT
  
  public:

  DesignAltItemView(bool t_isBaseline);
  virtual ~DesignAltItemView() {}

  DesignAltHeaderView * designAltHeaderView;

  DesignAltContentView * designAltContentView;

  protected:

  void paintEvent(QPaintEvent * e);
};

} // altstab

} // pat

} // openstudio

#endif // PATAPP_DESIGNALTERNATIVESVIEW_HPP
