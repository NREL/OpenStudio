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
