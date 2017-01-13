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

#ifndef OPENSTUDIO_SCRIPTITEM_HPP
#define OPENSTUDIO_SCRIPTITEM_HPP

#include "OSItem.hpp"

#include "../measure/OSArgument.hpp"
#include "../measure/OSMeasureInfoGetter.hpp"
//#include "../runmanager/lib/RunManager.hpp"
//#include "../runmanager/lib/RubyJobUtils.hpp"

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
             QWidget * parent = nullptr);

  virtual ~ScriptItem() {}

  openstudio::path path() const;

  openstudio::path argsDbPath() const;

  bool isUserScript() const;

  void setIsUserScript(bool isUserScript);

  //void refreshArgumentsFromScript(runmanager::RunManager t_rm);

  std::vector<measure::OSArgument> osArguments() const;

  boost::optional<measure::OSMeasureInfo> userScriptInfo() const;

  /** Returns the resources path that corresponds to this script's path, if possible.
   *  Otherwise, returns the root scripts path. */
  openstudio::path resourcesPath() const;

  bool updateArgumentsFromDb();

  void saveArgumentsToDb();

  void deleteDb();

  bool removed() const;

  virtual bool equal(const openstudio::OSItem *) const override;

 signals:

  void argChanged();

 public slots:

  void setOSArgument(const measure::OSArgument& arg);

 private slots:

  void onObjectChanged();

 private:
  bool m_removed;
  //runmanager::ScriptInfo m_scriptInfo;

  REGISTER_LOGGER("openstudio.ScriptItem");

};



} // openstudio

#endif // OPENSTUDIO_SCRIPTITEM_HPP

