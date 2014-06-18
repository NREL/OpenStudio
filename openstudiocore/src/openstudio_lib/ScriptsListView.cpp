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

#include "ScriptsListView.hpp"
#include "BuildingStoryInspectorView.hpp"
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
    connect(m_fswatcher.get(), SIGNAL(directoryChanged(const QString &)), this, SLOT(directoryChanged(const QString &)));

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

