/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ALFALFA_POINT_HPP
#define ALFALFA_POINT_HPP

#include "AlfalfaComponent.hpp"
#include "../utilities/core/Optional.hpp"
#include "../utilities/core/Logger.hpp"

#include "AlfalfaAPI.hpp"

#include <string>
#include <json/json.h>

namespace openstudio {
namespace alfalfa {
  namespace detail {
    class AlfalfaPoint_Impl;
  }
  class ALFALFA_API AlfalfaPoint
  {
   public:
    explicit AlfalfaPoint(const std::string& display_name);

    virtual ~AlfalfaPoint() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AlfalfaPoint(const AlfalfaPoint& other) = default;
    AlfalfaPoint(AlfalfaPoint&& other) = default;
    AlfalfaPoint& operator=(const AlfalfaPoint&) = default;
    AlfalfaPoint& operator=(AlfalfaPoint&&) = default;

    // AlfalfaPoint(const AlfalfaPoint& point);

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
    std::string id() const;

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
    bool isOptional() const;

    /**
     * Set whether a point is marked as optional or not.
     * If a point is marked as optional it will allow the model to run even if the point
     * cannot be instantiated.
     */
    void setOptional(bool optional);

    bool operator==(const AlfalfaPoint& rhs) const;

   private:
    std::shared_ptr<detail::AlfalfaPoint_Impl> m_impl;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
