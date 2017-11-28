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

  connect(m_fswatcher.get(), &QFileSystemWatcher::directoryChanged, this, &ScriptFolderListView::scriptListChanged);
  connect(m_fswatcher.get(), &QFileSystemWatcher::fileChanged, this, &ScriptFolderListView::scriptListChanged);
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
  auto folderTypeItem = new OSCollapsibleItem(collapsibleItemHeader, scriptsListView);

  LOG(Debug, "Adding scriptslistview: " << openstudio::toString(m_rootPath/folder));
  m_displayNames[m_rootPath / folder] = name;
  m_scriptsListViews[m_rootPath / folder] = scriptsListView;

  addCollapsibleItem(folderTypeItem);
}

void ScriptFolderListView::addScriptToFolder(const openstudio::path &t_path, const openstudio::path& folder_name)
{
  openstudio::path folder = m_rootPath / folder_name;
  openstudio::filesystem::create_directories(folder);
  openstudio::path filename = folder / t_path.filename();
  filename = iterateFileName(filename);
  openstudio::filesystem::copy_file(t_path, filename, openstudio::filesystem::copy_option::overwrite_if_exists);
  

  ScriptsListView *lv = m_scriptsListViews[folder];
  if (lv)
  {
    lv->updateData();
  }
}

void ScriptFolderListView::removeScript(const openstudio::path &t_path)
{
  openstudio::filesystem::remove(t_path);
}

void ScriptFolderListView::duplicateScript(const openstudio::path &t_path)
{
  openstudio::path filename = iterateFileName(t_path);
  openstudio::filesystem::copy_file(t_path, filename, openstudio::filesystem::copy_option::overwrite_if_exists);
}

void ScriptFolderListView::createEmptyScript(const openstudio::path &t_folder_name)
{
  openstudio::path filename = iterateFileName(m_rootPath / t_folder_name / openstudio::toPath("99_UserScript.rb"));

  // Scope for creating and closing file.
  {
    openstudio::filesystem::create_directories(m_rootPath / t_folder_name);
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
  auto it(m_displayNames.begin());
  auto itEnd(m_displayNames.end());
  for (; it != itEnd; ++it) {
    result.push_back(it->first);
  }
  return result;
}

openstudio::path ScriptFolderListView::selectedFolder() const {
  OSCollapsibleItem* selectedCollapsibleItem = this->selectedCollapsibleItem();

  if (selectedCollapsibleItem)
  {
    for (auto itr = m_scriptsListViews.begin();
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
  auto it = m_displayNames.find(folder);
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
    for (auto & listItem : listItems)
    {
      ScriptItem* scriptItem = qobject_cast<ScriptItem*>(listItem);
      if (scriptItem) {
        scriptItem->saveArgumentsToDb();
      }
    }
  }
}

std::vector<measure::OSMeasureInfo> ScriptFolderListView::folderUserScripts(
    const openstudio::path& folder) const
{
  auto it = m_scriptsListViews.find(folder);
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
  } while (!openstudio::filesystem::exists(p) && num > -1);

  if (!openstudio::filesystem::exists(p))
  {
    return p;
  } else {
    return last;
  }


}



} // openstudio

