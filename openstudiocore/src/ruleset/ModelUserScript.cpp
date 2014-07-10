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

#include "ModelUserScript.hpp"

#include "OSArgument.hpp"
#include "OSRunner.hpp"

namespace openstudio {
namespace ruleset {

ModelUserScript::~ModelUserScript()
{}

std::vector<UserScriptArgument> ModelUserScript::arguments(const openstudio::model::Model& model) const {
  return UserScriptArgumentVector();
}

bool ModelUserScript::run(openstudio::model::Model& model,
                          OSRunner& runner,
                          const std::map<std::string, OSArgument>& user_arguments) const
{
  runner.prepareForUserScriptRun(*this);
  return true;
}

} // ruleset
} // openstudio
