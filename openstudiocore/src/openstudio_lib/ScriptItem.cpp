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

#include "ScriptItem.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"

//#include "../runmanager/lib/RunManager.hpp"
//#include "../runmanager/lib/RubyJobUtils.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"

#include <QDir>
#include <QMessageBox>



namespace openstudio {

OSItemId scriptToItemId(const openstudio::path &t_path)
{
  return OSItemId(openstudio::toQString(t_path), openstudio::toQString(t_path.parent_path().filename()), false, "");
}

ScriptItem::ScriptItem(const openstudio::path& path,
                       OSItemType type,
                       QWidget * parent)
  : OSItem(scriptToItemId(path), type, parent),
    m_removed(false)
    //m_scriptInfo(path, true, false)
{
  setText(openstudio::toQString(path.filename()));
  setLeftPixmap(QPixmap(":/images/icon_scripts.png"));
  //if (boost::regex_search(toString(itemId().sourceId()),boost::regex("resource"))) {
  //  m_scriptInfo.isUserScript = false;
  //}
  //else {
  //  try {
  //    m_scriptInfo = runmanager::RubyJobBuilder::updateArgumentsFromDb(m_scriptInfo);
  //  } catch (const runmanager::ScriptDetectionError &e) {
  //    // Nothing to display here in the constructor
  //    m_scriptInfo = e.scriptInfo;
  //  }
  //}


  //std::shared_ptr<OSDocument> osDoc = OSAppBase::instance()->currentDocument();
  //connect(this, &ScriptItem::argChanged, osDoc.get(), &OSDocument::markAsModified);
}

openstudio::path ScriptItem::path() const {
  //return m_scriptInfo.scriptPath;
  return openstudio::path();
}

openstudio::path ScriptItem::argsDbPath() const {
  //return m_scriptInfo.dbPath;
  return openstudio::path();
}

bool ScriptItem::isUserScript() const {
  //return m_scriptInfo.isUserScript;
  return false;
}

void ScriptItem::setIsUserScript(bool isUserScript) {
  //m_scriptInfo.isUserScript = isUserScript;
}

//void ScriptItem::refreshArgumentsFromScript(runmanager::RunManager t_rm)
//{
//  if (t_rm.getConfigOptions().getTools().getAllByName("ruby").tools().size() == 0)
//  {
//    QMessageBox::critical(this,
//        "Ruby Not Installed",
//        "Please install and locate ruby to use user scripts in OpenStudio.",
//        QMessageBox::Ok);
//  } else {
//    try {
//      m_scriptInfo = runmanager::RubyJobBuilder::refreshArgumentsFromScript(t_rm, m_scriptInfo);
//    } catch (const runmanager::ScriptDetectionError &e) {
//      m_scriptInfo = e.scriptInfo;
//
//      QMessageBox::information(this,
//          "Could not retrieve user script arguments",
//          e.what(),
//          QMessageBox::Ok);
//    }
//  }
//}


std::vector<measure::OSArgument> ScriptItem::osArguments() const {
  measure::OSArgumentVector result;
  //for (const auto & elem : m_scriptInfo.arguments)
  //{
  //  result.push_back(elem.second);
  //}
  return result;
}

boost::optional<measure::OSMeasureInfo> ScriptItem::userScriptInfo() const {
  if (isUserScript()) {
    OS_ASSERT(false);
    //return measure::OSMeasureInfo(path(),osArguments());
  }
  return boost::none;
}

openstudio::path ScriptItem::resourcesPath() const {
  return openstudio::path();
}

void ScriptItem::setOSArgument(const measure::OSArgument& arg)
{
  //m_scriptInfo.arguments[arg.name()] = arg;
  //m_scriptInfo.argsChanged = true;
  //emit argChanged();
}

void ScriptItem::saveArgumentsToDb() {
  //try {
  //  m_scriptInfo = runmanager::RubyJobBuilder::saveArgumentsToDb(m_scriptInfo);
  //} catch (const runmanager::ScriptDetectionError &e) {
  //  QMessageBox::information(this,
  //      "User Script Argument Values not Saved",
  //      e.what(),
  //      QMessageBox::Ok);
  //  m_scriptInfo = e.scriptInfo;
  //}
}


void ScriptItem::deleteDb() {
  m_removed = true;
  if (openstudio::filesystem::exists(argsDbPath())) {
    openstudio::filesystem::remove(argsDbPath());
    openstudio::filesystem::remove(toPath(toString((argsDbPath())) + "-journal"));
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
  //this->setText(openstudio::toQString(m_scriptInfo.scriptPath.filename()));
}


bool ScriptItem::updateArgumentsFromDb()
{
  //try {
  //  m_scriptInfo = runmanager::RubyJobBuilder::updateArgumentsFromDb(m_scriptInfo);
  //} catch (const runmanager::ScriptDetectionError &e) {
  //  m_scriptInfo = e.scriptInfo;

  //  QMessageBox::information(this,
  //      "Could not retrieve user script arguments",
  //      e.what(),
  //      QMessageBox::Ok);
  //}
  //return m_scriptInfo.detectionSucceeded;
  return false;
}





} // openstudio

