/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MEASURE_EMBEDDEDRUBYMEASUREINFOGETTER_HPP
#define MEASURE_EMBEDDEDRUBYMEASUREINFOGETTER_HPP

#include "OSMeasureInfoGetter.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"

namespace openstudio {
namespace measure {

  /** Should only be instantiated in C++ Applications. For an example of how to use this, see
   *  src/measure/test/EmbeddedRuby_GTest.cpp. */
  template <typename RubyInterpreterType>
  class EmbeddedRubyMeasureInfoGetter : public OSMeasureInfoGetter
  {
   public:
    EmbeddedRubyMeasureInfoGetter(const std::shared_ptr<RubyInterpreterType>& rubyInterpreter) : m_rubyInterpreter(rubyInterpreter) {
      m_rubyInterpreter->template registerType<openstudio::BCLMeasure>("openstudio::BCLMeasure");
      m_rubyInterpreter->template registerType<openstudio::Workspace>("openstudio::Workspace");
      m_rubyInterpreter->template registerType<openstudio::model::Model>("openstudio::model::Model");
      //m_rubyInterpreter->template registerType<openstudio::measure::OSArgument>("openstudio::measure::OSArgument");
      m_rubyInterpreter->template registerType<openstudio::measure::OSMeasureInfo>("openstudio::measure::OSMeasureInfo");

      m_rubyInterpreter->evalString(infoExtractorRubyFunction());
    }

    virtual ~EmbeddedRubyMeasureInfoGetter() = default;

    OSMeasureInfo getInfo(const BCLMeasure& measure) override {
      return m_rubyInterpreter->template execWithReturn<OSMeasureInfo>("infoExtractor", measure, model::OptionalModel(), OptionalWorkspace());
    }

    OSMeasureInfo getInfo(const BCLMeasure& measure, const Workspace& workspace) override {
      if (model::OptionalModel model = workspace.optionalCast<model::Model>()) {
        return m_rubyInterpreter->template execWithReturn<OSMeasureInfo>("infoExtractor", measure, model::OptionalModel(model), OptionalWorkspace());
      } else {
        return m_rubyInterpreter->template execWithReturn<OSMeasureInfo>("infoExtractor", measure, model::OptionalModel(),
                                                                         OptionalWorkspace(workspace));
      }
    }

    OSMeasureInfo getInfo(const BCLMeasure& measure, const model::Model& model, const Workspace& workspace) override {
      return m_rubyInterpreter->template execWithReturn<OSMeasureInfo>("infoExtractor", measure, model::OptionalModel(model),
                                                                       OptionalWorkspace(workspace));
    }

   private:
    std::shared_ptr<RubyInterpreterType> m_rubyInterpreter;
  };

}  // namespace measure
}  // namespace openstudio

#endif  // MEASURE_EMBEDDEDRUBYMEASUREINFOGETTER_HPP
