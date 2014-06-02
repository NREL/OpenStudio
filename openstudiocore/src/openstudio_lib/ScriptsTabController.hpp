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

#ifndef OPENSTUDIO_SCRIPTSTABCONTROLLER_HPP
#define OPENSTUDIO_SCRIPTSTABCONTROLLER_HPP

#include "../utilities/core/Path.hpp"

#include "MainTabController.hpp"
#include "ScriptsTabView.hpp"
#include <QPointer>

namespace openstudio {

namespace measuretab {
class VariableListController;
class VariableItemDelegate;
class VariableGroupListController;
class VariableGroupItemDelegate;
class MeasureListController;
class VariableGroupItem;
class VariableItem;
}

class ScriptsTabController : public MainTabController
{
  Q_OBJECT

  public:

  ScriptsTabController();

  virtual ~ScriptsTabController();

  private:

  QPointer<ScriptsTabView> scriptsTabView;



  private:
  REGISTER_LOGGER("openstudio.measuretab.MeasuresTabController");

  QSharedPointer<measuretab::VariableGroupListController> m_variableGroupListController;
  QSharedPointer<measuretab::VariableGroupItemDelegate> m_variableGroupItemDelegate;

};

} // openstudio

#endif // OPENSTUDIO_SCRIPTSTABCONTROLLER_HPP
