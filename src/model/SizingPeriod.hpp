/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SIZINGPERIOD_HPP
#define MODEL_SIZINGPERIOD_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class SizingPeriod_Impl;
  }  // namespace detail

  class MODEL_API SizingPeriod : public ParentObject
  {
   public:
    virtual ~SizingPeriod() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SizingPeriod(const SizingPeriod& other) = default;
    SizingPeriod(SizingPeriod&& other) = default;
    SizingPeriod& operator=(const SizingPeriod&) = default;
    SizingPeriod& operator=(SizingPeriod&&) = default;

    // ensure that this object does not contain the date 2/29
    void ensureNoLeapDays();

   protected:
    using ImplType = detail::SizingPeriod_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    SizingPeriod(IddObjectType type, const Model& model);

    // constructor
    explicit SizingPeriod(std::shared_ptr<detail::SizingPeriod_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SizingPeriod");
  };

  /** \relates SizingPeriod */
  using OptionalSizingPeriod = boost::optional<SizingPeriod>;

  /** \relates SizingPeriod */
  using SizingPeriodVector = std::vector<SizingPeriod>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SIZINGPERIOD_HPP
