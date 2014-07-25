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

#ifndef OPENSTUDIO_SCRIPTITEM_HPP
#define OPENSTUDIO_SCRIPTITEM_HPP

#include "OSItem.hpp"

#include "../ruleset/OSArgument.hpp"
#include "../runmanager/lib/RunManager.hpp"
#include "../runmanager/lib/RubyJobUtils.hpp"

#include "../utilities/core/Path.hpp"

#include <QWidget>

class QPushButton;
class QLabel;

namespace openstudio {

OSItemId scriptToItemId(const openstudio::path &t_path);

//QString modelToSourceId(const openstudio::model::Model& model);

class ScriptItem : public OSItem {
  Q_OBJECT

 public:

  ScriptItem(const openstudio::path &t_path,
             OSItemType type,
             QWidget * parent = 0);

  virtual ~ScriptItem() {}

  openstudio::path path() const;

  openstudio::path argsDbPath() const;

  bool isUserScript() const;

  void setIsUserScript(bool isUserScript);

  void refreshArgumentsFromScript(runmanager::RunManager t_rm);

  std::vector<ruleset::OSArgument> osArguments() const;

  boost::optional<ruleset::UserScriptInfo> userScriptInfo() const;

  /** Returns the resources path that corresponds to this script's path, if possible.
   *  Otherwise, returns the root scripts path. */
  openstudio::path resourcesPath() const;

  bool updateArgumentsFromDb();

  void saveArgumentsToDb();

  void deleteDb();

  bool removed() const;

  virtual bool equal(const openstudio::OSItem *) const;

 signals:

  void argChanged();

 public slots:

  void setOSArgument(const ruleset::OSArgument& arg);

 private slots:

  void onObjectChanged();

 private:
  bool m_removed;
  runmanager::ScriptInfo m_scriptInfo;

  REGISTER_LOGGER("openstudio.ScriptItem");

};



} // openstudio

#endif // OPENSTUDIO_SCRIPTITEM_HPP

