/**********************************************************************
 *  Copyright (c) 2008-2012, Alliance for Sustainable Energy.
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

#ifndef RULESET_RUBYUSERSCRIPTARGUMENTGETTER_IMPL_HPP
#define RULESET_RUBYUSERSCRIPTARGUMENTGETTER_IMPL_HPP

#include <ruleset/RubyUserScriptArgumentGetter.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {

  /** Should only be instantiated in C++ Applications. For an example of how to use this, see
   *  XXXX. */
  template<typename RubyInterpreterType>
  class RubyUserScriptArgumentGetter_Impl : public RubyUserScriptArgumentGetter {
   public:
    RubyUserScriptArgumentGetter_Impl(const boost::shared_ptr<RubyInterpreterType>& rubyInterpreter)
      : m_rubyInterpreter(rubyInterpreter)
    {
      m_rubyInterpreter->template registerType<openstudio::BCLMeasure>("openstudio::BCLMeasure");
      m_rubyInterpreter->template registerType<openstudio::Workspace>("openstudio::Workspace");
      m_rubyInterpreter->template registerType<openstudio::model::Model>("openstudio::model::Model");
      m_rubyInterpreter->template registerType<openstudio::ruleset::OSArgument>("openstudio::ruleset::OSArgument");

      m_rubyInterpreter->evalString(argumentExtractorRubyFunction());
    }

    virtual ~RubyUserScriptArgumentGetter_Impl() {}

    std::vector<OSArgument> getArguments(const BCLMeasure& measure) {
      return m_rubyInterpreter->template execWithReturn<std::vector<OSArgument> >(
          "argumentExtractor", measure, model::OptionalModel(), OptionalWorkspace());
    }

    std::vector<OSArgument> getArguments(const BCLMeasure& measure, const Workspace& workspace) {
      if (model::OptionalModel model = workspace.optionalCast<model::Model>()) {
        return m_rubyInterpreter->template execWithReturn<std::vector<OSArgument> >(
              "argumentExtractor", measure, model::OptionalModel(model), OptionalWorkspace());
      }
      else {
        return m_rubyInterpreter->template execWithReturn<std::vector<OSArgument> >(
            "argumentExtractor", measure, model::OptionalModel(), OptionalWorkspace(workspace));
      }
    }

    std::vector<OSArgument> getArguments(const BCLMeasure& measure,
                                         const model::Model& model,
                                         const Workspace& workspace)
    {
      return m_rubyInterpreter->template execWithReturn<std::vector<OSArgument> >(
          "argumentExtractor", measure, model::OptionalModel(model), OptionalWorkspace(workspace));
    }

   private:
    boost::shared_ptr<RubyInterpreterType> m_rubyInterpreter;
  };

} // detail

} // ruleset
} // openstudio

#endif // RULESET_RUBYUSERSCRIPTARGUMENTGETTER_IMPL_HPP
