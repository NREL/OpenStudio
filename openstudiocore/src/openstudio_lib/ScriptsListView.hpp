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

#ifndef OPENSTUDIO_SCRIPTSLISTVIEW_HPP
#define OPENSTUDIO_SCRIPTSLISTVIEW_HPP

#include "SubTabView.hpp"
#include "ModelObjectInspectorView.hpp"
#include "OSVectorController.hpp"
#include "OSItemList.hpp"

#include "../ruleset/OSArgument.hpp"

#include "../model/Model.hpp"

#include "../utilities/idf/WorkspaceObject_Impl.hpp"

#include <QFileSystemWatcher>

class QLabel;

namespace openstudio {

class ScriptsVectorController : public OSVectorController {
  Q_OBJECT;

 public:
  ScriptsVectorController(
      const openstudio::path &t_path,
      const std::shared_ptr<QFileSystemWatcher> &t_fswatcher = std::shared_ptr<QFileSystemWatcher>(new QFileSystemWatcher()));

  virtual ~ScriptsVectorController();

  virtual std::vector<OSItemId> makeVector();

  void updateData();

 private slots:
  void directoryChanged(const QString &);

 private:
  REGISTER_LOGGER("ScriptsVectorController");

  openstudio::path m_path;
  std::shared_ptr<QFileSystemWatcher> m_fswatcher;
  std::vector<OSItemId> m_items;
};

class ScriptsListView : public OSItemList {
  Q_OBJECT;
 public:
  ScriptsListView(
      const openstudio::path &t_path,
      bool addScrollArea,
      bool draggable,
      bool removeable,
      const std::shared_ptr<QFileSystemWatcher> &fswatcher = std::shared_ptr<QFileSystemWatcher>(new QFileSystemWatcher()),
      QWidget* parent = 0);

  virtual ~ScriptsListView() {}

  void updateData();

  std::vector<ruleset::UserScriptInfo> userScripts();
};


} // openstudio

#endif // OPENSTUDIO_SCRIPTSLISTVIEW_HPP
