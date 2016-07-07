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

#ifndef SHAREDGUICOMPONENTS_MEASUREITEM_HPP
#define SHAREDGUICOMPONENTS_MEASUREITEM_HPP

#include "OSListController.hpp"
#include "OSListView.hpp"
#include "BaseApp.hpp"

#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/filetypes/WorkflowStep.hpp"
#include "../utilities/core/Logger.hpp"
#include <QObject>
#include <QPointer>
#include <QSharedPointer>
#include <map>

namespace openstudio{
  
class MeasuresTabView;
class StartupView;

namespace measure {
  class OSArgument;
} 


namespace measuretab {


// MeasureItem represents a MeasureStep
  // DLM: probably neesd a MeasureStep and some sort of MeasureDefinition from the MeasureManager
class MeasureItem : public OSListItem
{
  Q_OBJECT

  public:

  MeasureItem(const MeasureStep & measureStep, BaseApp *t_app);

  QString name() const;

  QString description() const;

  QString modelerDescription() const;

  QString scriptFileName() const;

  std::vector<measure::OSArgument> arguments() const;

  //bool hasIncompleteArguments() const;

  //void setArgument(const ruleset::OSArgument& argument);

  MeasureStep measureStep() const;

  //OptionalBCLMeasure bclMeasure() const;

  //bool isAlternativeModelMeasure() const;

  public slots:

  void setName(const QString & name);

  void setDescription(const QString & description);

  //void duplicate();

  //void remove();

  void setSelected(bool isSelected) override;

  signals:

  //void nameChanged(const QString & name);

  void displayNameChanged(const QString & displayName);

  void descriptionChanged();

  void argumentsChanged(bool isIncomplete);

  private:
  BaseApp *m_app;
  MeasureStep m_measureStep;

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

#endif // SHAREDGUICOMPONENTS_MEASUREITEM_HPP
