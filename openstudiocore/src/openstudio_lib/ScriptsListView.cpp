/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "ScriptsListView.hpp"
#include "ModelObjectListView.hpp"
#include "ScriptItem.hpp"

#include "../utilities/core/PathHelpers.hpp"

#include "../model/Model_Impl.hpp"
#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QScrollArea>
#include <QDir>
#include <sstream>

namespace openstudio {

  ScriptsVectorController::ScriptsVectorController(const openstudio::path &t_path,
      const std::shared_ptr<QFileSystemWatcher> &t_fswatcher)
    : m_path(t_path), m_fswatcher(t_fswatcher)
  {
    connect(m_fswatcher.get(), &QFileSystemWatcher::directoryChanged, this, &ScriptsVectorController::directoryChanged);
    if (boost::filesystem::exists(t_path))
    {
      m_fswatcher->addPath(openstudio::toQString(t_path));
    }

    if (boost::filesystem::exists(t_path.parent_path()))
    {
      m_fswatcher->addPath(openstudio::toQString(t_path.parent_path()));
    }

    directoryChanged(openstudio::toQString(m_path));

    LOG(Debug, "Created ScriptVectorController " << m_path);
  }

  ScriptsVectorController::~ScriptsVectorController()
  {
    if (m_fswatcher){
      m_fswatcher->removePaths(m_fswatcher->files());
    }
  }

  void ScriptsVectorController::updateData()
  {
    directoryChanged(openstudio::toQString(m_path));
  }

  void ScriptsVectorController::directoryChanged(const QString &t_path)
  {
    StringVector extsToIgnore;
    extsToIgnore.push_back("osp");
    extsToIgnore.push_back("osp-journal");
    extsToIgnore.push_back("db");
    extsToIgnore.push_back("db-journal");
    StringVector::const_iterator extsToIgnoreBegin = extsToIgnore.begin();
    StringVector::const_iterator extsToIgnoreEnd = extsToIgnore.end();

    openstudio::path path = openstudio::toPath(t_path);

    if (!m_fswatcher->directories().contains(toQString(m_path))
        && boost::filesystem::exists(m_path))
    {
      m_fswatcher->addPath(openstudio::toQString(m_path));
    }

    if (!m_fswatcher->directories().contains(toQString(m_path.parent_path()))
        && boost::filesystem::exists(m_path.parent_path()))
    {
      m_fswatcher->addPath(openstudio::toQString(m_path.parent_path()));
    }

 
    if (path == m_path) {
      // ooh, it was us.
      QStringList files = QDir(openstudio::toQString(m_path)).entryList(QDir::Files, QDir::Name);

      std::vector<OSItemId> items;
      for (QStringList::const_iterator itr = files.begin();
           itr != files.end();
           ++itr)
      {
        openstudio::path filePath = openstudio::toPath(*itr);
        std::string ext = getFileExtension(filePath);
        if (std::find(extsToIgnoreBegin,extsToIgnoreEnd,ext) == extsToIgnoreEnd) {
          items.push_back(scriptToItemId(m_path / filePath));
        }
      }

      m_items = items;
      std::reverse(m_items.begin(), m_items.end());

      emit itemIds(m_items);
    } else {
      LOG(Debug, "No match for FS Path: " << openstudio::toString(path) << " " << openstudio::toString(m_path));
    }
  }

  std::vector<OSItemId> ScriptsVectorController::makeVector()
  {
    return m_items;
  }

ScriptsListView::ScriptsListView(const openstudio::path &t_path,
      bool addScrollArea,
      bool draggable,
      bool removeable,
      const std::shared_ptr<QFileSystemWatcher> &t_fswatcher,
      QWidget* parent )
  : OSItemList(new ScriptsVectorController(t_path, t_fswatcher), addScrollArea, parent)
{
  setItemsDraggable(draggable);
  setItemsRemoveable(removeable);
}

void ScriptsListView::updateData() {
  qobject_cast<ScriptsVectorController *>(vectorController())->updateData();
}

std::vector<ruleset::UserScriptInfo> ScriptsListView::userScripts() {
  std::vector<ruleset::UserScriptInfo> result;
  for (OSItem* item : items()) {
    ScriptItem* scriptItem = qobject_cast<ScriptItem*>(item);
    if (boost::optional<ruleset::UserScriptInfo> info = scriptItem->userScriptInfo()) {
      result.push_back(*info);
    }
  }
  return result;
}

} // openstudio

