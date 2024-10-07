/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONSTRUCTIONAIRBOUNDARY_HPP
#define MODEL_CONSTRUCTIONAIRBOUNDARY_HPP

#include <model/ModelAPI.hpp>
#include "ConstructionBase.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  // TODO: Check the following class names against object getters and setters.
  class Schedule;

  namespace detail {

    class ConstructionAirBoundary_Impl;

  }  // namespace detail

  /** ConstructionAirBoundary is a ConstructionBase that wraps the OpenStudio IDD object 'OS:Construction:AirBoundary'. */
  class MODEL_API ConstructionAirBoundary : public ConstructionBase
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ConstructionAirBoundary(const Model& model);

    virtual ~ConstructionAirBoundary() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ConstructionAirBoundary(const ConstructionAirBoundary& other) = default;
    ConstructionAirBoundary(ConstructionAirBoundary&& other) = default;
    ConstructionAirBoundary& operator=(const ConstructionAirBoundary&) = default;
    ConstructionAirBoundary& operator=(ConstructionAirBoundary&&) = default;

    //@}

    static IddObjectType iddObjectType();

    OS_DEPRECATED(3, 2, 0) static std::vector<std::string> solarAndDaylightingMethodValues();

    OS_DEPRECATED(3, 2, 0) static std::vector<std::string> radiantExchangeMethodValues();

    static std::vector<std::string> airExchangeMethodValues();

    /** @name Getters */
    //@{

    // cppcheck-suppress functionStatic
    OS_DEPRECATED(3, 2, 0) std::string solarAndDaylightingMethod() const;

    // cppcheck-suppress functionStatic
    OS_DEPRECATED(3, 2, 0) bool isSolarAndDaylightingMethodDefaulted() const;

    // cppcheck-suppress functionStatic
    OS_DEPRECATED(3, 2, 0) std::string radiantExchangeMethod() const;

    // cppcheck-suppress functionStatic
    OS_DEPRECATED(3, 2, 0) bool isRadiantExchangeMethodDefaulted() const;

    std::string airExchangeMethod() const;

    bool isAirExchangeMethodDefaulted() const;

    double simpleMixingAirChangesPerHour() const;

    bool isSimpleMixingAirChangesPerHourDefaulted() const;

    boost::optional<Schedule> simpleMixingSchedule() const;

    //@}
    /** @name Setters */
    //@{

    // cppcheck-suppress functionStatic
    OS_DEPRECATED(3, 2, 0) bool setSolarAndDaylightingMethod(const std::string& solarAndDaylightingMethod);

    // cppcheck-suppress functionStatic
    OS_DEPRECATED(3, 2, 0) void resetSolarAndDaylightingMethod();

    // cppcheck-suppress functionStatic
    OS_DEPRECATED(3, 2, 0) bool setRadiantExchangeMethod(const std::string& radiantExchangeMethod);

    // cppcheck-suppress functionStatic
    OS_DEPRECATED(3, 2, 0) void resetRadiantExchangeMethod();

    bool setAirExchangeMethod(const std::string& airExchangeMethod);

    void resetAirExchangeMethod();

    bool setSimpleMixingAirChangesPerHour(double simpleMixingAirChangesPerHour);

    void resetSimpleMixingAirChangesPerHour();

    bool setSimpleMixingSchedule(Schedule& schedule);

    void resetSimpleMixingSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ConstructionAirBoundary_Impl;

    explicit ConstructionAirBoundary(std::shared_ptr<detail::ConstructionAirBoundary_Impl> impl);

    friend class detail::ConstructionAirBoundary_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ConstructionAirBoundary");
  };

  /** \relates ConstructionAirBoundary*/
  using OptionalConstructionAirBoundary = boost::optional<ConstructionAirBoundary>;

  /** \relates ConstructionAirBoundary*/
  using ConstructionAirBoundaryVector = std::vector<ConstructionAirBoundary>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONSTRUCTIONAIRBOUNDARY_HPP
