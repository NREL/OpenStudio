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

    Json::Value toJSON() const;
    std::string string() const;

    bool debug() const;
    bool isDebugDefaulted() const;
    bool setDebug(bool debug);
    void resetDebug();

    bool epjson() const;
    bool isEpjsonDefaulted() const;
    bool setEpjson(bool epjson);
    void resetEpjson();

    bool fast() const;
    bool isFastDefaulted() const;
    bool setFast(bool fast);
    void resetFast();

    bool preserveRunDir() const;
    bool isPreserveRunDirDefaulted() const;
    bool setPreserveRunDir(bool preserveRunDir);
    void resetPreserveRunDir();

    bool skipExpandObjects() const;
    bool isSkipExpandObjectsDefaulted() const;
    bool setSkipExpandObjects(bool skipExpandObjects);
    void resetSkipExpandObjects();

    bool skipEnergyPlusPreprocess() const;
    bool isSkipEnergyPlusPreprocessDefaulted() const;
    bool setSkipEnergyPlusPreprocess(bool skipEnergyPlusPreprocess);
    void resetSkipEnergyPlusPreprocess();

    bool cleanup() const;
    bool isCleanupDefaulted() const;
    bool setCleanup(bool cleanup);
    void resetCleanup();

    bool skipZipResults() const;
    bool isSkipZipResultsDefaulted() const;
    bool setSkipZipResults(bool skipZipResults);
    void resetSkipZipResults();

    boost::optional<CustomOutputAdapter> customOutputAdapter() const;
    bool setCustomOutputAdapter(const CustomOutputAdapter& adapter);
    void resetCustomOutputAdapter();

    ForwardTranslatorOptions forwardTranslatorOptions() const;
    bool setForwardTranslatorOptions(const ForwardTranslatorOptions& forwardTranslatorOptions);
    void resetForwardTranslatorOptions();

    /* Any non-defaulted value from other is brought over */
    void overrideValuesWith(const RunOptions& other);

    // Emitted on any change
    Nano::Signal<void()> onChange;

   protected:
    void onUpdate();

   private:
    // configure logging
    REGISTER_LOGGER("openstudio.RunOptions");

    static constexpr bool DEFAULT_DEBUG = false;
    static constexpr bool DEFAULT_EPJSON = false;
    static constexpr bool DEFAULT_FAST = false;
    static constexpr bool DEFAULT_PRESERVERUNDIR = false;
    static constexpr bool DEFAULT_SKIPEXPANDOBJECTS = false;
    static constexpr bool DEFAULT_SKIPENERGYPLUSPREPROCESS = false;
    static constexpr bool DEFAULT_CLEANUP = true;
    static constexpr bool DEFAULT_SKIPZIPRESULTS = false;

    bool m_debug = DEFAULT_DEBUG;
    bool m_is_debug_defaulted = true;

    bool m_epjson = DEFAULT_EPJSON;
    bool m_is_epjson_defaulted = true;

    bool m_fast = DEFAULT_FAST;
    bool m_is_fast_defaulted = true;

    bool m_preserveRunDir = DEFAULT_PRESERVERUNDIR;
    bool m_is_preserveRunDir_defaulted = true;

    bool m_skipExpandObjects = DEFAULT_SKIPEXPANDOBJECTS;
    bool m_is_skipExpandObjects_defaulted = true;

    bool m_skipEnergyPlusPreprocess = DEFAULT_SKIPENERGYPLUSPREPROCESS;
    bool m_is_skipEnergyPlusPreprocess_defaulted = true;

    // TODO this does absolutely nothing in the workflow-gem currently
    bool m_cleanup = DEFAULT_CLEANUP;
    bool m_is_cleanup_defaulted = true;

    bool m_skipZipResults = DEFAULT_SKIPZIPRESULTS;
    bool m_is_skipZipResults_defaulted = true;

    ForwardTranslatorOptions m_forwardTranslatorOptions;
    boost::optional<CustomOutputAdapter> m_customOutputAdapter;
  };

}  // namespace detail
}  // namespace openstudio

#endif  //UTILITIES_FILETYPES_RUNOPTIONS_IMPL_HPP
