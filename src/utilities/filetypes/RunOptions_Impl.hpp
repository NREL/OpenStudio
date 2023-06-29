/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_RUNOPTIONS_IMPL_HPP
#define UTILITIES_FILETYPES_RUNOPTIONS_IMPL_HPP

#include "ForwardTranslatorOptions.hpp"

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../data/Variant.hpp"

#include <json/json.h>

#include <nano/nano_signal_slot.hpp>

namespace openstudio {
class CustomOutputAdapter;

namespace detail {

  class UTILITIES_API RunOptions_Impl
  {
   public:
    RunOptions_Impl() = default;

    std::string string() const;

    bool debug() const;
    bool setDebug(bool debug);
    void resetDebug();

    bool epjson() const;
    bool setEpjson(bool epjson);
    void resetEpjson();

    bool fast() const;
    bool setFast(bool fast);
    void resetFast();

    bool preserveRunDir() const;
    bool setPreserveRunDir(bool preserve);
    void resetPreserveRunDir();

    bool skipExpandObjects() const;
    bool setSkipExpandObjects(bool skip);
    void resetSkipExpandObjects();

    bool skipEnergyPlusPreprocess() const;
    bool setSkipEnergyPlusPreprocess(bool skip);
    void resetSkipEnergyPlusPreprocess();

    bool cleanup() const;
    bool setCleanup(bool cleanup);
    void resetCleanup();

    boost::optional<CustomOutputAdapter> customOutputAdapter() const;
    bool setCustomOutputAdapter(const CustomOutputAdapter& adapter);
    void resetCustomOutputAdapter();

    ForwardTranslatorOptions forwardTranslatorOptions() const;
    bool setForwardTranslatorOptions(const ForwardTranslatorOptions& forwardTranslatorOptions);
    void resetForwardTranslatorOptions();

    // Emitted on any change
    Nano::Signal<void()> onChange;

   protected:
    void onUpdate();

   private:
    // configure logging
    REGISTER_LOGGER("openstudio.RunOptions");

    bool m_debug = false;
    bool m_epjson = false;
    bool m_fast = false;
    bool m_preserveRunDir = false;
    bool m_skipExpandObjects = false;
    bool m_skipEnergyPlusPreprocess = false;
    bool m_cleanup = true;  // TODO this does absolutely nothing in the workflow-gem currently
    ForwardTranslatorOptions m_forwardTranslatorOptions;
    boost::optional<CustomOutputAdapter> m_customOutputAdapter;
  };

}  // namespace detail
}  // namespace openstudio

#endif  //UTILITIES_FILETYPES_RUNOPTIONS_IMPL_HPP
