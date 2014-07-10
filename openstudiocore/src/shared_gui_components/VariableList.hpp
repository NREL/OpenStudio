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

class VariableGroupListController : public OSListController
{
  Q_OBJECT

  public:

  VariableGroupListController(bool filterFixed, BaseApp *t_baseApp);

  void addItem(QSharedPointer<OSListItem> item);

  QSharedPointer<OSListItem> itemAt(int i);

  int count();

  private:

  std::vector<QSharedPointer<VariableGroupItem> > m_variableGroupItems;
};

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

class VariableGroupItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  VariableGroupItemDelegate(bool t_fixedMeasuresOnly);
  QWidget * view(QSharedPointer<OSListItem> dataSource);

  private:

  bool m_fixedMeasuresOnly;
};

class VariableListController : public OSListController
{
  Q_OBJECT

  public:

  VariableListController(MeasureType measureType, bool designAlternatives, BaseApp *t_baseApp);

  QSharedPointer<OSListItem> itemAt(int i);

  int count();

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

class VariableItem : public OSListItem
{
  Q_OBJECT

  public:

  VariableItem(const analysis::MeasureGroup & variable, MeasureType measureType, BaseApp *t_app);

  QSharedPointer<MeasureListController> measureListController() const { return m_measureListController; }

  QString name() const;

  analysis::MeasureGroup variable() const { return m_variable; }

  MeasureType measureType() const { return m_measureType; }

  bool isFixedMeasure();

  public slots:

  void remove();

  void moveUp();

  void moveDown();

  void setName(const QString & name);

  private:
  REGISTER_LOGGER("openstudio.measuretab.VariableItem");

  BaseApp *m_app;

  QSharedPointer<MeasureListController> m_measureListController; 

  analysis::MeasureGroup m_variable;

  MeasureType m_measureType;
};

class VariableItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  QWidget * view(QSharedPointer<OSListItem> dataSource);
};

class MeasureListController : public OSListController
{
  Q_OBJECT

  public:

  MeasureListController(VariableItem * variableItem, BaseApp *t_app);

  QSharedPointer<OSListItem> itemAt(int i);

  int count();

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

class MeasureItem : public OSListItem
{
  Q_OBJECT

  public:

  MeasureItem(const analysis::RubyMeasure & measure, BaseApp *t_app);

  QString name() const;

  QString description() const;

  QString modelerDescription() const;

  QString scriptFileName() const;

  std::vector<ruleset::OSArgument> arguments() const;

  bool hasIncompleteArguments() const;

  void setArgument(const ruleset::OSArgument& argument);

  analysis::RubyMeasure measure() const;

  bool isFixedMeasure() const;

  public slots:

  void setName(const QString & name);

  void setDescription(const QString & description);

  void duplicate();

  void remove();

  void setSelected(bool isSelected);

  signals:

  void nameChanged(const QString & name);

  void descriptionChanged();

  void argumentsChanged(bool isIncomplete);

  private:
  BaseApp *m_app;
  analysis::RubyMeasure m_measure;

  REGISTER_LOGGER("openstudio.measuretab.MeasureItem");
};

class MeasureItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  MeasureItemDelegate(bool t_fixed) : m_fixed(t_fixed) {}

  QWidget * view(QSharedPointer<OSListItem> dataSource);

  private:

  bool m_fixed;
};

} // measuretab

} // openstudio

#endif // SHAREDGUICOMPONENTS_VARIABLELIST_HPP
