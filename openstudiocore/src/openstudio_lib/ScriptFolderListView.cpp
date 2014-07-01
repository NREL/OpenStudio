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

#include "ScriptFolderListView.hpp"
#include "ScriptsListView.hpp"
#include "ScriptItem.hpp"
#include "ModelObjectItem.hpp"
#include "ModelObjectListView.hpp"
#include "OSCollapsibleItemHeader.hpp"
#include "OSCollapsibleItem.hpp"
#include "OSItem.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>

namespace openstudio {

ScriptFolderListView::ScriptFolderListView(const openstudio::path &folder,
                                           bool addScrollArea,
                                           bool draggable,
                                           bool removeable,
                                           OSItemType headerType,
                                           QWidget * parent)
  : OSCollapsibleItemList(addScrollArea, parent), m_rootPath(folder), m_headerType(headerType),
    m_draggable(draggable),
    m_removeable(removeable), 
    m_fswatcher(std::make_shared<QFileSystemWatcher>())
{ 
  setItemsDraggable(draggable);
  setItemsRemoveable(removeable);
  addScriptFolder(openstudio::toPath("post_energyplus"), "Post EnergyPlus Scripts");
  addScriptFolder(openstudio::toPath("idf_resources"), "IDF Script Resources");
  addScriptFolder(openstudio::toPath("idf_scripts"), "IDF Scripts");
  addScriptFolder(openstudio::toPath("model_resources"), "Model Script Resources");
  addScriptFolder(openstudio::toPath("model_scripts"), "Model Scripts");

  connect(m_fswatcher.get(), SIGNAL(directoryChanged(const QString &)),
      this, SIGNAL(scriptListChanged()));
  connect(m_fswatcher.get(), SIGNAL(fileChanged(const QString &)),
      this, SIGNAL(scriptListChanged()));

}

ScriptFolderListView::~ScriptFolderListView()
{
  if (m_fswatcher){
    m_fswatcher->removePaths(m_fswatcher->files());
  }
}


void ScriptFolderListView::addScriptFolder(const openstudio::path &folder, const std::string& name) {
  OSCollapsibleItemHeader* collapsibleItemHeader = new OSCollapsibleItemHeader(name, OSItemId("", "", false,""), m_headerType);
  ScriptsListView* scriptsListView = new ScriptsListView(m_rootPath / folder, false, m_draggable, m_removeable, m_fswatcher);
  OSCollapsibleItem* folderTypeItem = new OSCollapsibleItem(collapsibleItemHeader, scriptsListView);

  LOG(Debug, "Adding scriptslistview: " << openstudio::toString(m_rootPath/folder));
  m_displayNames[m_rootPath / folder] = name;
  m_scriptsListViews[m_rootPath / folder] = scriptsListView;

  addCollapsibleItem(folderTypeItem);
}

void ScriptFolderListView::addScriptToFolder(const openstudio::path &t_path, const openstudio::path& folder_name)
{
  openstudio::path folder = m_rootPath / folder_name;
  boost::filesystem::create_directories(folder);
  openstudio::path filename = folder / t_path.filename();
  filename = iterateFileName(filename);
  boost::filesystem::copy_file(t_path, filename, boost::filesystem::copy_option::overwrite_if_exists);
  

  ScriptsListView *lv = m_scriptsListViews[folder];
  if (lv)
  {
    lv->updateData();
  }
}

void ScriptFolderListView::removeScript(const openstudio::path &t_path)
{
  boost::filesystem::remove(t_path);
}

void ScriptFolderListView::duplicateScript(const openstudio::path &t_path)
{
  openstudio::path filename = iterateFileName(t_path);
  boost::filesystem::copy_file(t_path, filename, boost::filesystem::copy_option::overwrite_if_exists);
}

void ScriptFolderListView::createEmptyScript(const openstudio::path &t_folder_name)
{
  openstudio::path filename = iterateFileName(m_rootPath / t_folder_name / openstudio::toPath("99_UserScript.rb"));

  // Scope for creating and closing file.
  {
    boost::filesystem::create_directories(m_rootPath / t_folder_name);
    std::ofstream ofs(openstudio::toString(filename).c_str());
    ofs << "# Empty Script" << std::endl; 
  }

  ScriptsListView *lv = m_scriptsListViews[m_rootPath / t_folder_name];
  if (lv)
  {
    lv->updateData();
  }
}

openstudio::path ScriptFolderListView::rootPath() const {
  return m_rootPath;
}

std::vector<openstudio::path> ScriptFolderListView::folders() const {
  std::vector<openstudio::path> result;
  std::map<openstudio::path, std::string>::const_iterator it(m_displayNames.begin());
  std::map<openstudio::path, std::string>::const_iterator itEnd(m_displayNames.end());
  for (; it != itEnd; ++it) {
    result.push_back(it->first);
  }
  return result;
}

openstudio::path ScriptFolderListView::selectedFolder() const {
  OSCollapsibleItem* selectedCollapsibleItem = this->selectedCollapsibleItem();

  if (selectedCollapsibleItem)
  {
    for (std::map<openstudio::path, ScriptsListView *>::const_iterator itr = m_scriptsListViews.begin();
      itr != m_scriptsListViews.end();
      ++itr)
    {
      if (itr->second)
      {
        if (itr->second == selectedCollapsibleItem->itemList())
        {
          return itr->first.filename();
        }
      }
    }
  }
  throw std::runtime_error("No selected folder");
}

std::string ScriptFolderListView::folderName(const openstudio::path& folder) const {
  std::map<openstudio::path, std::string>::const_iterator it = m_displayNames.find(folder);
  if (it == m_displayNames.end()) {
    it = m_displayNames.find(m_rootPath / folder);
  }
  OS_ASSERT(it != m_displayNames.end());
  return it->second;
}

void ScriptFolderListView::saveOSArguments() {
  for (std::map<openstudio::path, ScriptsListView *>::const_iterator lit = m_scriptsListViews.begin(),
       litEnd = m_scriptsListViews.end(); lit != litEnd; ++lit)
  {
    std::vector<OSItem *> listItems = lit->second->items();
    for (std::vector<OSItem *>::iterator it = listItems.begin(), itEnd = listItems.end();
         it != itEnd; ++it)
    {
      ScriptItem* scriptItem = qobject_cast<ScriptItem*>(*it);
      if (scriptItem) {
        scriptItem->saveArgumentsToDb();
      }
    }
  }
}

std::vector<ruleset::UserScriptInfo> ScriptFolderListView::folderUserScripts(
    const openstudio::path& folder) const
{
  std::map<openstudio::path, ScriptsListView *>::const_iterator it = m_scriptsListViews.find(folder);
  if (it == m_scriptsListViews.end()) {
    it = m_scriptsListViews.find(m_rootPath / folder);
  }
  OS_ASSERT(it != m_scriptsListViews.end());
  return it->second->userScripts();
}

std::shared_ptr<QFileSystemWatcher> ScriptFolderListView::fsWatcher() const
{
  return m_fswatcher;
}

openstudio::path ScriptFolderListView::iterateFileName(const openstudio::path &t_path)
{
  struct BuildFileName
  {
    static openstudio::path doit(const openstudio::path &t_root,
        const std::string &t_stem,
        const std::string &t_extension,
        int iteration)
    {
      std::stringstream numportion;
      if (iteration > 0)
      {
        numportion << "-";
        if (iteration < 10)
        {
          numportion << "0";
        } 
        numportion << iteration;
      }
      return t_root / openstudio::toPath(t_stem + numportion.str() + t_extension);
    }
  };

  std::string stem = openstudio::toString(t_path.stem());

  if (boost::regex_match(openstudio::toString(stem), boost::regex(".*-[0-9][0-9]")))
  {
    stem = stem.substr(0, stem.size() - 3);
  }

  int num = 99;
  openstudio::path p;
  openstudio::path last;
  do
  {
    last = p;
    p = BuildFileName::doit(t_path.parent_path(), openstudio::toString(stem), openstudio::toString(t_path.extension()), num);
    --num;
  } while (!boost::filesystem::exists(p) && num > -1);

  if (!boost::filesystem::exists(p))
  {
    return p;
  } else {
    return last;
  }


}



} // openstudio

