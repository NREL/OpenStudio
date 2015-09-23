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

#ifndef PATAPP_DESIGNALTERNATIVESVIEW_HPP
#define PATAPP_DESIGNALTERNATIVESVIEW_HPP

#include "PatMainTabView.hpp"
#include "../shared_gui_components/OSCollapsibleView.hpp"
#include <QAbstractButton>

class QPushButton;
class QLineEdit;
class QTextEdit;
class QComboBox;

namespace openstudio {

class OSListView;
class DarkGradientHeader;

namespace pat {

namespace altstab {

  class DesignAltsView;
  class DesignAltHeaderView;
  class DesignAltContentView;

}

// DesignAlternativesTabView is the main tab for viewing design alternatives in PAT
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

// Many of these classes are alternatives to the classes in \openstudiocore\src\shared_gui_components\VariableList.hpp
// If possible, we should share the classes in shared_gui_components to these
// These classes are only used on the design alternatives tab

// The widget on the right side of the main splitter
// Shows all the design alternatives
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

// AltsTabVariableGroupItemView displays a VariableGroupItem, e.g. all the Model or EnergyPlus MeasureGroups/Fixed Measures, the view is configured by a AltsTabVariableGroupItemDelegate
class AltsTabVariableGroupItemView : public OSCollapsibleView
{
  Q_OBJECT

  public:

  AltsTabVariableGroupItemView();

  virtual ~AltsTabVariableGroupItemView() {}

  DarkGradientHeader * variableGroupHeaderView;

  OSListView * variableGroupContentView;
};

// AltsTabVariableItemView displays a VariableItem (either a MeasureGroup or a fixed Measure)
// It is configured by the AltsTabVariableItemDelegate
class AltsTabVariableItemView : public OSCollapsibleView
{
  Q_OBJECT

  public:

  AltsTabVariableItemView();

  virtual ~AltsTabVariableItemView() {}

  QLabel * variableHeaderView;

  OSListView * variableContentView;
};


// AltsTabMeasureItemView displays a MeasureItem (individual measure)
// It is configured by AltsTabMeasureItemDelegate
class AltsTabMeasureItemView : public QAbstractButton
{
  Q_OBJECT

  public:

  AltsTabMeasureItemView();

  virtual ~AltsTabMeasureItemView() {}

  QLabel * label;

  public slots:

  void setHasEmphasis(bool hasEmphasis);

  protected:

  void paintEvent(QPaintEvent * e) override;
};

// DesignAltItemView views a DesignAltItem (particular combination of measures)
// It is configured by DesignAltItemDelegate
class DesignAltItemView : public OSCollapsibleView
{
  Q_OBJECT

public:

  DesignAltItemView(bool t_isBaseline, bool t_isAlternativeModel, bool t_alternateModelMeasureNeedsUpdate);

  virtual ~DesignAltItemView() {}

  DesignAltHeaderView * designAltHeaderView;

  DesignAltContentView * designAltContentView;

protected:

  void paintEvent(QPaintEvent * e);
};

class DesignAltHeaderView : public OSHeader
{
  Q_OBJECT

  public:

  DesignAltHeaderView(bool isBaseline, bool t_isAlternativeModel, bool t_alternateModelMeasureNeedsUpdate);

  virtual ~DesignAltHeaderView() {}

  QLineEdit * designAltNameEdit;

  QPushButton * removeButton;
};

class DesignAltContentView : public QWidget
{
  Q_OBJECT

  public:

  DesignAltContentView(bool isBaseline, bool t_isAlternativeModel, bool t_alternateModelMeasureNeedsUpdate);

  virtual ~DesignAltContentView() {}

  OSListView * perturbationListView;

  QTextEdit * descriptionTextEdit;

  OSListView * alternativeModelMeasureListView;

  QPushButton * addAlternativeModelMeasure;

  signals:

  void descriptionChanged(const QString & description);

  void addAlternativeModelMeasureClicked();

  private slots:

  void onDescriptionTextChanged();
};

class AlternativeModelMeasureItemView : public QWidget
{
  Q_OBJECT

  public:
   
  AlternativeModelMeasureItemView(const QString& uuid);
 
  QLineEdit* displayNameTextEdit;
  QTextEdit* descriptionTextEdit;
  QComboBox* taxonomyFirstLevelComboBox;
  QComboBox* taxonomySecondLevelComboBox;
  QLineEdit* capitalCostTextEdit;

  QPushButton * removeAlternativeModelMeasure;

  QString uuid() const;
  QString displayName() const;
  QString description() const;
  QString taxonomyTag() const;
  double capitalCost() const;

  void setDisplayName(const QString& displayName);
  void setDescription(const QString& description);
  void setTaxonomyTag(const QString& taxonomyTag);
  void setCapitalCost(double capitalCost);

  signals :

  void changed();

  void removed();

  private slots:

  void onFirstLevelTaxonomyTagChanged();

  private:

  QString m_uuid;

  void paintEvent(QPaintEvent * e) override;
};

} // altstab

} // pat

} // openstudio

#endif // PATAPP_DESIGNALTERNATIVESVIEW_HPP
