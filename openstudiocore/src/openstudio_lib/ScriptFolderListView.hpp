/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef OPENSTUDIO_SCRIPTFOLDERLISTVIEW_HPP
#define OPENSTUDIO_SCRIPTFOLDERLISTVIEW_HPP

#include "OSCollapsibleItemList.hpp"

#include "../measure/OSArgument.hpp"
#include "../measure/OSMeasureInfoGetter.hpp"

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
                       QWidget * parent = nullptr);

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

  std::vector<measure::OSMeasureInfo> folderUserScripts(const openstudio::path& folder) const;

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

