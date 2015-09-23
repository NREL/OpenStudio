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

#ifndef SHAREDGUICOMPONENTS_VARIABLELIST_HPP
#define SHAREDGUICOMPONENTS_VARIABLELIST_HPP

#include "OSListController.hpp"
#include "OSListView.hpp"
#include "BaseApp.hpp"
#include "../analysis/MeasureGroup.hpp"
#include "../analysis/RubyMeasure.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/Logger.hpp"
#include <QObject>
#include <QPointer>
#include <QSharedPointer>
#include <map>

namespace openstudio{

namespace analysisdriver {
class SimpleProject;
}

  
class MeasuresTabView;
class StartupView;

namespace measuretab {

class VariableListController;
class VariableItemDelegate;
class VariableGroupListController;
class VariableGroupItemDelegate;
class MeasureListController;
class VariableGroupItem;
class VariableItem;

} // measuretab


namespace measuretab {

// the classes here are used in the measures tab of PAT as well as the measures tab of the App
//
// alternatives classes to these classes used on the design alternatives tab in PAT
// are defined in \openstudiocore\src\pat_app\DesignAlternativesTabController.hpp
  
// VariableGroupListController controls a list of VariableGroupItems
// Each VariableGroupItem represents a list of measures grouped by type
class VariableGroupListController : public OSListController
{
  Q_OBJECT

  public:

  VariableGroupListController(bool filterFixed, BaseApp *t_baseApp);

  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

  private:

  void addItem(QSharedPointer<OSListItem> item);

  std::vector<QSharedPointer<VariableGroupItem> > m_variableGroupItems;
};

// Each VariableGroupItem represents a list of measures grouped by type, e.g. all the Model Measures
class VariableGroupItem : public OSListItem
{
  Q_OBJECT

  public:

  VariableGroupItem(MeasureType measureType, const QString & label, bool filterFixed, BaseApp *t_baseApp);

  QSharedPointer<VariableListController> variableListController() const;

  QString label() const;

  private:

  QSharedPointer<VariableListController> m_variableListController;

  QString m_label;  
};

// VariableGroupItemDelegate views a VariableGroupItem and returns a VariableGroupItemView
class VariableGroupItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  explicit VariableGroupItemDelegate(bool t_fixedMeasuresOnly);

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;

  private:

  bool m_fixedMeasuresOnly;
};

// VariableListController controls a list of VariableItems
// It can add, remove, or change the order of MeasureGroups (which correspond to analysis::Variables)
class VariableListController : public OSListController
{
  Q_OBJECT

  public:

  VariableListController(MeasureType measureType, bool designAlternatives, BaseApp *t_baseApp);

  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

  void removeItemForVariable(analysis::MeasureGroup variable);

  void moveUp(analysis::MeasureGroup variable);

  void moveDown(analysis::MeasureGroup variable);

  MeasureType measureType() const;

  public slots:

  void addItemForDroppedMeasure(QDropEvent * event);

  void addFixedItemForDroppedMeasure(QDropEvent *event);

  private:

  REGISTER_LOGGER("openstudio.measuretab.VariableListController");

  void addItemForDroppedMeasureImpl(QDropEvent * event, bool t_fixed);

  BaseApp *m_app;
  MeasureType m_measureType;
  bool m_filterFixed;

  std::vector<analysis::MeasureGroup> variables() const;
};

// VariableItem represents a MeasureGroup (which corresponds to analysis::Variable)
class VariableItem : public OSListItem
{
  Q_OBJECT

  public:

  VariableItem(const analysis::MeasureGroup & variable, MeasureType measureType, BaseApp *t_app);

  QSharedPointer<MeasureListController> measureListController() const { return m_measureListController; }

  QString name() const;

  QString displayName() const;

  analysis::MeasureGroup variable() const { return m_variable; }

  MeasureType measureType() const { return m_measureType; }

  bool isFixedMeasure();

  public slots:

  void remove();

  void moveUp();

  void moveDown();

  void setName(const QString & name);

  void setDisplayName(const QString & displayName);

  bool isAlternativeModelVariable() const;

  private:
  REGISTER_LOGGER("openstudio.measuretab.VariableItem");

  BaseApp *m_app;

  QSharedPointer<MeasureListController> m_measureListController; 

  analysis::MeasureGroup m_variable;

  MeasureType m_measureType;
};

// VariableItemDelegate views a VariableItem and returns a VariableItemView
class VariableItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;
};

// VariableListController controls a list of MeasureItems which come from a VariableItem
class MeasureListController : public OSListController
{
  Q_OBJECT

  public:

  MeasureListController(VariableItem * variableItem, BaseApp *t_app);

  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

  void addItemForDuplicateMeasure(const analysis::Measure& measure);

  void removeItemForMeasure(const analysis::Measure & measure);

  public slots:

  void addItemForDroppedMeasure(QDropEvent * event);

  private:
  REGISTER_LOGGER("openstudio.measuretab.MeasureListController");

  BaseApp *m_app;

  std::vector<analysis::RubyMeasure> measures() const;

  QPointer<VariableItem> m_variableItem;
};

// MeasureItem represents an analysis::RubyMeasure)
class MeasureItem : public OSListItem
{
  Q_OBJECT

  public:

  MeasureItem(const analysis::RubyMeasure & measure, BaseApp *t_app);

  QString name() const;

  QString displayName() const;

  QString description() const;

  QString modelerDescription() const;

  QString scriptFileName() const;

  std::vector<ruleset::OSArgument> arguments() const;

  bool hasIncompleteArguments() const;

  void setArgument(const ruleset::OSArgument& argument);

  analysis::RubyMeasure measure() const;

  OptionalBCLMeasure bclMeasure() const;

  bool isAlternativeModelMeasure() const;

  public slots:

  void setName(const QString & name);

  void setDisplayName(const QString & displayName);

  void setDescription(const QString & description);

  void duplicate();

  void remove();

  void setSelected(bool isSelected) override;

  signals:

  //void nameChanged(const QString & name);

  void displayNameChanged(const QString & displayName);

  void descriptionChanged();

  void argumentsChanged(bool isIncomplete);

  private:
  BaseApp *m_app;
  analysis::RubyMeasure m_measure;

  REGISTER_LOGGER("openstudio.measuretab.MeasureItem");
};

// MeasureItemDelegate views a MeasureItem and returns a MeasureItemView
class MeasureItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  explicit MeasureItemDelegate(bool t_fixed);

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;

  private:

  MeasureItemDelegate();

  bool m_fixed;
};

} // measuretab

} // openstudio

#endif // SHAREDGUICOMPONENTS_VARIABLELIST_HPP
