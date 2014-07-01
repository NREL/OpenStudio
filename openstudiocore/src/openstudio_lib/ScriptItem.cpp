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

#include "ScriptItem.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "ScriptFolderListView.hpp"

#include "../runmanager/lib/RunManager.hpp"
#include "../runmanager/lib/RubyJobUtils.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/ApplicationPathHelpers.hpp"
#include "../utilities/core/Containers.hpp"

#include <QDir>
#include <QMessageBox>

#include <boost/filesystem.hpp>

namespace openstudio {

OSItemId scriptToItemId(const openstudio::path &t_path)
{
  return OSItemId(openstudio::toQString(t_path), openstudio::toQString(t_path.parent_path().filename()), false, "");
}

ScriptItem::ScriptItem(const openstudio::path& path,
                       OSItemType type,
                       QWidget * parent)
  : OSItem(scriptToItemId(path), type, parent),
    m_removed(false),
    m_scriptInfo(path, true, false)
{
  setText(openstudio::toQString(path.filename()));
  setLeftPixmap(QPixmap(":/images/icon_scripts.png"));
  if (boost::regex_search(toString(itemId().sourceId()),boost::regex("resource"))) {
    m_scriptInfo.isUserScript = false;
  }
  else {
    try {
      m_scriptInfo = runmanager::RubyJobBuilder::updateArgumentsFromDb(m_scriptInfo);
    } catch (const runmanager::ScriptDetectionError &e) {
      // Nothing to display here in the constructor
      m_scriptInfo = e.scriptInfo;
    }
  }


  std::shared_ptr<OSDocument> osDoc = OSAppBase::instance()->currentDocument();
  connect(this,SIGNAL(argChanged()),osDoc.get(),SLOT(markAsModified()));
}

openstudio::path ScriptItem::path() const {
  return m_scriptInfo.scriptPath;
}

openstudio::path ScriptItem::argsDbPath() const {
  return m_scriptInfo.dbPath;
}

bool ScriptItem::isUserScript() const {
  return m_scriptInfo.isUserScript;
}

void ScriptItem::setIsUserScript(bool isUserScript) {
  m_scriptInfo.isUserScript = isUserScript;
}

void ScriptItem::refreshArgumentsFromScript(runmanager::RunManager t_rm)
{
  if (t_rm.getConfigOptions().getTools().getAllByName("ruby").tools().size() == 0)
  {
    QMessageBox::critical(this,
        "Ruby Not Installed",
        "Please install and locate ruby to use user scripts in OpenStudio.",
        QMessageBox::Ok);
  } else {
    try {
      m_scriptInfo = runmanager::RubyJobBuilder::refreshArgumentsFromScript(t_rm, m_scriptInfo);
    } catch (const runmanager::ScriptDetectionError &e) {
      m_scriptInfo = e.scriptInfo;

      QMessageBox::information(this,
          "Could not retrieve user script arguments",
          e.what(),
          QMessageBox::Ok);
    }
  }
}


std::vector<ruleset::OSArgument> ScriptItem::osArguments() const {
  ruleset::OSArgumentVector result;
  for (std::map<std::string,ruleset::OSArgument>::const_iterator itr = m_scriptInfo.arguments.begin(),
       itrEnd = m_scriptInfo.arguments.end(); itr != itrEnd; ++itr)
  {
    result.push_back(itr->second);
  }
  return result;
}

boost::optional<ruleset::UserScriptInfo> ScriptItem::userScriptInfo() const {
  if (isUserScript()) {
    return ruleset::UserScriptInfo(path(),osArguments());
  }
  return boost::none;
}

openstudio::path ScriptItem::resourcesPath() const {
/*
  const ScriptFolderListView* scriptFolders =
      OSAppBase::instance()->currentDocument()->scriptFolderListView();
  QString folderName = itemId().sourceId();
  openstudio::path result;
  if (folderName == "post_energyplus") {
    result = scriptFolders->rootPath();
  }
  else if ((folderName == "idf_resources") || (folderName == "idf_scripts")) {
    result = scriptFolders->rootPath() / toPath("idf_resources");
  }
  else {
    result = scriptFolders->rootPath() / toPath("model_resources");
  }
  return result;
*/
  return openstudio::path();
}

void ScriptItem::setOSArgument(const ruleset::OSArgument& arg)
{
  m_scriptInfo.arguments[arg.name()] = arg;
  m_scriptInfo.argsChanged = true;
  emit argChanged();
}

void ScriptItem::saveArgumentsToDb() {
  try {
    m_scriptInfo = runmanager::RubyJobBuilder::saveArgumentsToDb(m_scriptInfo);
  } catch (const runmanager::ScriptDetectionError &e) {
    QMessageBox::information(this,
        "User Script Argument Values not Saved",
        e.what(),
        QMessageBox::Ok);
    m_scriptInfo = e.scriptInfo;
  }
}


void ScriptItem::deleteDb() {
  m_removed = true;
  if (boost::filesystem::exists(argsDbPath())) {
    boost::filesystem::remove(argsDbPath());
    boost::filesystem::remove(toPath(toString((argsDbPath())) + "-journal"));
  }
}

bool ScriptItem::removed() const {
  return m_removed;
}

bool ScriptItem::equal(const openstudio::OSItem* otherItem) const
{
  if (otherItem) {
    return otherItem->itemId() == itemId();
  }
  return false;

}

void ScriptItem::onObjectChanged()
{
  this->setText(openstudio::toQString(m_scriptInfo.scriptPath.filename()));
}


bool ScriptItem::updateArgumentsFromDb()
{
  try {
    m_scriptInfo = runmanager::RubyJobBuilder::updateArgumentsFromDb(m_scriptInfo);
  } catch (const runmanager::ScriptDetectionError &e) {
    m_scriptInfo = e.scriptInfo;

    QMessageBox::information(this,
        "Could not retrieve user script arguments",
        e.what(),
        QMessageBox::Ok);
  }
  return m_scriptInfo.detectionSucceeded;
}





} // openstudio

