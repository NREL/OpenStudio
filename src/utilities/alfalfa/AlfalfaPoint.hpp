/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ALFALFA_POINT_HPP
#define ALFALFA_POINT_HPP

#include "AlfalfaComponent.hpp"
#include "utilities/core/Optional.hpp"
#include "utilities/core/Logger.hpp"

#include <string>
#include <json/json.h>

namespace openstudio {
namespace alfalfa {
  namespace detail {
    class AlfalfaPoint_Impl;
  }
  class AlfalfaPoint
  {
   public:
    AlfalfaPoint();

    AlfalfaPoint(const std::string& display_name);

    /**
     * Set input of point. This component will be written to by the Alfalfa API.
     */
    void setInput(const AlfalfaComponent& component);

    /**
     * Get input component of point.
     */
    boost::optional<AlfalfaComponent> input() const;

    /**
     * Set output component of point. This component will be read to report a value to the Alfalfa API.
     */
    void setOutput(const AlfalfaComponent& component);

    /**
     * Get output component of point.
     */
    boost::optional<AlfalfaComponent> output() const;

    /**
     * Set the units of the point.
     */
    void setUnits(const std::string& units);

    /**
     * Get the units of the point.
     */
    boost::optional<std::string> units() const;

    Json::Value toJSON() const;

    /**
     * Get id of point. By default this will be a version of the display name with spaces removed.
     */
    boost::optional<std::string> id() const;

    /**
     * Set id of point. This is the component which will uniquely identify the point in the API.
     */
    void setId(const std::string& id);

    /**
     * Get display name of point.
     */
    std::string displayName() const;

    /**
     * Set display name of point.
     */
    void setDisplayName(const std::string& display_name);

    /**
     * Get whether a point is marked as optional or not.
     */
    bool optional() const;

    /**
     * Set whether a point is marked as optional or not.
     * If a point is marked as optional it will allow the model to run even if the point
     * cannot be instantiated.
     */
    void setOptional(bool optional);

    bool operator==(const AlfalfaPoint& rhs) const;

   private:
    std::shared_ptr<detail::AlfalfaPoint_Impl> m_impl;

    std::string toIdString(const std::string& str) const;

    bool isValidId(const std::string& id) const;

    inline static const std::string ID_VALID_CHARS_MSG = "IDs can only contain letters, numbers, and the following special characters _-[]():";

    // configure logging
    REGISTER_LOGGER("openstudio.AlfalfaPoint")
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
