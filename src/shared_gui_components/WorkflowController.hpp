/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef SHAREDGUICOMPONENTS_WORKFLOWCONTROLLER_HPP
#define SHAREDGUICOMPONENTS_WORKFLOWCONTROLLER_HPP

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
  class WorkflowStepController;
  class MeasureStepItem;
  class MeasureStepController;

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

  WorkflowStepController(BaseApp *t_baseApp);
};

// MeasureStepController controls a list of MeasureStepItems
class MeasureStepController : public WorkflowStepController
{
  Q_OBJECT

  public:

  MeasureStepController(MeasureType measureType, BaseApp *t_baseApp);

  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

  MeasureType measureType() const;

  std::vector<MeasureStep> measureSteps() const;

  void removeItemForStep(MeasureStep step);

  void moveUp(MeasureStep step);

  void moveDown(MeasureStep step);

  public slots:

  void addItemForDroppedMeasure(QDropEvent * event);

  private:

  void addItem(QSharedPointer<OSListItem> item);

  MeasureType m_measureType;
  BaseApp * m_app;

  std::vector<QSharedPointer<MeasureStepItem> > m_measureStepItems;
};

// MeasureStepItemDelegate views a MeasureStepItem and returns a MeasureStepView
class MeasureStepItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  explicit MeasureStepItemDelegate();

  QWidget * view(QSharedPointer<OSListItem> dataSource) override;

  private:

};

// Each MeasureStepItem represents a MeasureStep
class MeasureStepItem : public OSListItem
{
  Q_OBJECT

  public:

  MeasureStepItem(MeasureType measureType, MeasureStep step, BaseApp *t_baseApp);

  QString name() const;

  //QString displayName() const;

  MeasureType measureType() const;

  MeasureStep measureStep() const;

  QString description() const;

  QString modelerDescription() const;

  QString scriptFileName() const;

  OptionalBCLMeasure bclMeasure() const;

  // arguments includes the full list of arguments calculated for the given model along with any values specified in the OSW
  std::vector<measure::OSArgument> arguments() const;

  // Returns the name of the measure directory in question (not a path, just the last level)
  QString measureDirectory() const;

  bool hasIncompleteArguments() const;

  std::vector<measure::OSArgument> incompleteArguments() const;

  public slots:

  void remove();

  void moveUp();

  void moveDown();

  void setName(const QString & name);

  //void setDisplayName(const QString & displayName);

  void setDescription(const QString & description);

  void setArgument(const measure::OSArgument& argument);

  void setSelected(bool isSelected) override;

  signals:

  void nameChanged(const QString & name);

  //void displayNameChanged(const QString & displayName);

  void descriptionChanged();

  void argumentsChanged(bool isIncomplete);

  private:

  MeasureType m_measureType;
  MeasureStep m_step;
  BaseApp * m_app;

};

} // measuretab

} // openstudio

#endif // SHAREDGUICOMPONENTS_WORKFLOWCONTROLLER_HPP
