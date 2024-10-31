/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULETYPELIMITS_HPP
#define MODEL_SCHEDULETYPELIMITS_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
class Unit;

namespace model {

  namespace detail {

    class ScheduleTypeLimits_Impl;

  }  // namespace detail

  /** ScheduleTypeLimits is a ResourceObject that wraps the OpenStudio IDD object 'OS:ScheduleTypeLimits'.
 *  Once attached to an object of type ScheduleBase, it defines the units and bounds for that object's
 *  values. */
  class MODEL_API ScheduleTypeLimits : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ScheduleTypeLimits(const Model& model);

    virtual ~ScheduleTypeLimits() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleTypeLimits(const ScheduleTypeLimits& other) = default;
    ScheduleTypeLimits(ScheduleTypeLimits&& other) = default;
    ScheduleTypeLimits& operator=(const ScheduleTypeLimits&) = default;
    ScheduleTypeLimits& operator=(ScheduleTypeLimits&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> numericTypeValues();

    static std::vector<std::string> unitTypeValues();

    // std::string by val on purpose (boost::to_lower)
    static boost::optional<Unit> units(std::string unitType, bool returnIP = false);

    /** @name Getters */
    //@{

    boost::optional<double> lowerLimitValue() const;

    boost::optional<double> upperLimitValue() const;

    boost::optional<std::string> numericType() const;

    std::string unitType() const;

    bool isUnitTypeDefaulted() const;

    boost::optional<Unit> units(bool returnIP = false) const;

    //@}
    /** @name Setters */
    //@{

    // ETH@20120919 TODO: Have these setters fail if they are going to invalidate the current
    // use of a ScheduleBase object.

    bool setLowerLimitValue(double lowerLimitValue);

    void resetLowerLimitValue();

    bool setUpperLimitValue(double upperLimitValue);

    void resetUpperLimitValue();

    bool setNumericType(const std::string& numericType);

    void resetNumericType();

    bool setUnitType(const std::string& unitType);

    void resetUnitType();

    //@}
   protected:
    /// @cond
    using ImplType = detail::ScheduleTypeLimits_Impl;

    explicit ScheduleTypeLimits(std::shared_ptr<detail::ScheduleTypeLimits_Impl> impl);

    friend class detail::ScheduleTypeLimits_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleTypeLimits");
  };

  /** \relates ScheduleTypeLimits */
  using OptionalScheduleTypeLimits = boost::optional<ScheduleTypeLimits>;

  /** \relates ScheduleTypeLimits */
  using ScheduleTypeLimitsVector = std::vector<ScheduleTypeLimits>;

  /** Returns true if candidate is compatible with parentLimits, that is, if it is the same
 *  type and has bounds that are as least as strict as parentLimits's.
 *  \relates ScheduleTypeLimits */
  MODEL_API bool isCompatible(const ScheduleTypeLimits& parentLimits, const ScheduleTypeLimits& candidate);

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULETYPELIMITS_HPP
