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

#ifndef SHAREDGUICOMPONENTS_VARIABLELIST_HPP
#define SHAREDGUICOMPONENTS_VARIABLELIST_HPP

#include "OSListController.hpp"
#include "OSListView.hpp"
#include "BaseApp.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/filetypes/WorkflowStep.hpp"
#include <QObject>
#include <QPointer>
#include <QSharedPointer>
#include <map>

namespace openstudio{

namespace measuretab {
  
  class WorkflowSectionItem;
  class WorkflowStepItem;
  class WorkflowStepController;

} // measuretab

namespace measure {
  
  class OSArgument;

}

namespace measuretab {

// the classes here are used in the measures tab of the App

// WorkflowController controls a list of WorkflowSectionItems
// Each WorkflowSectionItem represents a set of measures grouped by type, e.g. all the model measures
class WorkflowController : public OSListController
{
  Q_OBJECT

  public:

  WorkflowController(BaseApp *t_baseApp);

  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

  private:

  void addItem(QSharedPointer<OSListItem> item);

  std::vector<QSharedPointer<WorkflowSectionItem> > m_workflowSectionItems;
};

// WorkflowSectionItemDelegate views a WorkflowSectionItem and returns a WorkflowSectionView
class WorkflowSectionItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  explicit WorkflowSectionItemDelegate();

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;

  private:

};

// Each WorkflowSectionItem represents a set of measures grouped by type, e.g. all the model measures
// DLM: in the future this can also be used to show other types of jobs, e.g. ModelToIdf
class WorkflowSectionItem : public OSListItem
{
  Q_OBJECT

  public:

  WorkflowSectionItem(MeasureType measureType, const QString& label, BaseApp *t_baseApp);

  QString label() const;

  boost::optional<MeasureType> measureType() const;

  QSharedPointer<WorkflowStepController> workflowStepController() const;

  private:
  
  QString m_label;
  boost::optional<MeasureType> m_measureType;

  QSharedPointer<WorkflowStepController> m_workflowStepController;

};

// WorkflowStepController controls a list of WorkflowStepItems
class WorkflowStepController : public OSListController
{
  Q_OBJECT

  public:

  WorkflowStepController(MeasureType measureType, BaseApp *t_baseApp);

  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

  boost::optional<MeasureType> measureType() const;

  void removeItemForStep(WorkflowStep step);

  void moveUp(WorkflowStep step);

  void moveDown(WorkflowStep step);

  public slots:

  void addItemForDroppedMeasure(QDropEvent * event);

  private:

  void addItem(QSharedPointer<OSListItem> item);

  boost::optional<MeasureType> m_measureType;
  BaseApp * m_app;

  std::vector<QSharedPointer<WorkflowStepItem> > m_workflowStepItems;
};

// WorkflowStepItemDelegate views a WorkflowStepItem and returns a WorkflowStepView
class WorkflowStepItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  explicit WorkflowStepItemDelegate();

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;

  private:

};

// Each WorkflowStepItem represents a WorkflowStep
class WorkflowStepItem : public OSListItem
{
  Q_OBJECT

  public:

  WorkflowStepItem(WorkflowStep step, BaseApp *t_baseApp);

  QString name() const;

  QString displayName() const;

  boost::optional<MeasureType> measureType() const;

  QString description() const;

  QString modelerDescription() const;

  QString scriptFileName() const;

  OptionalBCLMeasure bclMeasure() const;

  std::vector<measure::OSArgument> arguments() const;

  bool hasIncompleteArguments() const;

  public slots:

  void remove();

  void moveUp();

  void moveDown();

  void setName(const QString & name);

  void setDisplayName(const QString & displayName);

  void setArgument(const measure::OSArgument& argument);

  private:

  WorkflowStep m_step;
  BaseApp * m_app;

};

} // measuretab

} // openstudio

#endif // SHAREDGUICOMPONENTS_VARIABLELIST_HPP
