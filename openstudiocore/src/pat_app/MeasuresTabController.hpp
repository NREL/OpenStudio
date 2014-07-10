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

#ifndef PATAPP_MEASURESTABCONTROLLER_HPP
#define PATAPP_MEASURESTABCONTROLLER_HPP

#include "../shared_gui_components/OSListController.hpp"
#include "../shared_gui_components/OSListView.hpp"
#include "../analysis/MeasureGroup.hpp"
#include "../analysis/RubyMeasure.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include <QObject>
#include <QPointer>
#include <QSharedPointer>
#include <map>

namespace openstudio{

namespace analysisdriver {
class SimpleProject;
}

namespace measuretab {

class VariableListController;
class VariableItemDelegate;
class VariableGroupListController;
class VariableGroupItemDelegate;
class MeasureListController;
class VariableGroupItem;
class VariableItem;

} // measuretab
  
namespace pat {

class MeasuresTabView;
class StartupView;


class MeasuresTabController : public QObject
{
  Q_OBJECT

  public:

  MeasuresTabController();

  QPointer<MeasuresTabView> measuresTabView;

  virtual ~MeasuresTabController();

  void refreshAllViews();

  private slots:

  void selectBaseline();

  private:
  REGISTER_LOGGER("openstudio.pat.measuretab.MeasuresTabController");

  QSharedPointer<measuretab::VariableGroupListController> m_variableGroupListController;
  QSharedPointer<measuretab::VariableGroupItemDelegate> m_variableGroupItemDelegate;

  //std::map<MeasureType,QString> m_groups;
};


} // pat

} // openstudio

#endif // PATAPP_MEASURESTABCONTROLLER_HPP
