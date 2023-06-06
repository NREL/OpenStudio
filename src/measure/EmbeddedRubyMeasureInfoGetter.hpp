/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
