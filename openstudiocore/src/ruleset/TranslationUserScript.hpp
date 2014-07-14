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

#ifndef RULESET_TRANSLATIONUSERSCRIPT_HPP
#define RULESET_TRANSLATIONUSERSCRIPT_HPP

#include "RulesetAPI.hpp"
#include "UserScript.hpp"

#include "../utilities/core/Logger.hpp"

namespace openstudio {

class Workspace;

namespace model {
  class Model;
}

namespace ruleset {

/** TranslationUserScript is an abstract base class for UserScripts that operate on an OpenStudio
 *  Model and an OpenStudio Workspace. \deprecated */
class RULESET_API TranslationUserScript : public UserScript {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~TranslationUserScript();

  //@}
  /** @name Getters */
  //@{

  /** Returns the arguments for this script. In interactive applications, an OSRunner presents
   *  these arguments to the user to produce an OSArgumentMap of user_arguments that it then passes
   *  to this script's run method. The same basic steps should happen in applications with non-
   *  interactive scripts, but in that case an entity other than an OSRunner may be in charge of
   *  collecting user arguments. The base class implementation returns an empty vector. */
  virtual std::vector<OSArgument> arguments(const openstudio::Workspace& workspace) const;

  /** Returns the arguments for this script. In interactive applications, an OSRunner presents
   *  these arguments to the user to produce an OSArgumentMap of user_arguments that it then passes
   *  to this script's run method. The same basic steps should happen in applications with non-
   *  interactive scripts, but in that case an entity other than an OSRunner may be in charge of
   *  collecting user arguments. The base class implementation returns an empty vector. */
  virtual std::vector<OSArgument> arguments(const openstudio::model::Model& model) const;

  //@}
  /** @name Actions */
  //@{

  /** Run the script on the given workspace and model with the given runner and user_arguments. The
   *  base class implementation calls runner.prepareForUserScriptRun(*this) and should be called at
   *  the beginning of derived class implementations of this method. (In C++, that call looks like
   *  TranslationUserScript::run(workspace, model, runner, user_arguments). In Ruby that call looks
   *  like super(workspace, model, runner, user_arguments). */
  virtual bool run(openstudio::Workspace& workspace,
                   openstudio::model::Model& model,
                   OSRunner& runner,
                   const std::map<std::string, OSArgument>& user_arguments) const;

  //@}
  //
 protected:
  TranslationUserScript() {}

 private:
  REGISTER_LOGGER("openstudio.ruleset.TranslationUserScript");
};

} // ruleset
} // openstudio

#endif // RULESET_TRANSLATIONUSERSCRIPT_HPP
