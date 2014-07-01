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

#ifndef OPENSTUDIO_SCRIPTFOLDERLISTVIEW_HPP
#define OPENSTUDIO_SCRIPTFOLDERLISTVIEW_HPP

#include "OSCollapsibleItemList.hpp"

#include "../ruleset/OSArgument.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

#include "../utilities/core/Logger.hpp"

#include <QFileSystemWatcher>

#include <boost/optional.hpp>

class QVBoxLayout;

class QHBoxLayout;

namespace openstudio {

class ScriptsListView;

class ScriptFolderListView : public OSCollapsibleItemList {
  Q_OBJECT
 public:
  ScriptFolderListView(const openstudio::path &t_rootPath,
                       bool addScrollArea,
                       bool draggable,
                       bool removeable,
                       OSItemType headerType,
                       QWidget * parent = 0);

  virtual ~ScriptFolderListView();

  void addScriptFolder(const openstudio::path &t_path, const std::string& name);

  void addScriptToFolder(const openstudio::path &t_path, const openstudio::path& folder_name);
  void removeScript(const openstudio::path &t_path);

  void duplicateScript(const openstudio::path &t_path);
  void createEmptyScript(const openstudio::path &t_folder_name);

  openstudio::path rootPath() const;

  std::vector<openstudio::path> folders() const;

  openstudio::path selectedFolder() const;

  std::string folderName(const openstudio::path& folder) const;

  void saveOSArguments();

  std::vector<ruleset::UserScriptInfo> folderUserScripts(const openstudio::path& folder) const;

  std::shared_ptr<QFileSystemWatcher> fsWatcher() const;

 signals:
  void scriptListChanged();

 private:
  REGISTER_LOGGER("openstudio.ScriptFolderListView");

  openstudio::path iterateFileName(const openstudio::path &t_path);

  openstudio::path m_rootPath;
  OSItemType m_headerType;
  std::map<openstudio::path, std::string> m_displayNames;
  std::map<openstudio::path, ScriptsListView *> m_scriptsListViews;

  bool m_draggable;
  bool m_removeable;

  std::shared_ptr<QFileSystemWatcher> m_fswatcher;
};



} // openstudio

#endif // OPENSTUDIO_SCRIPTFOLDERLISTVIEW_HPP

