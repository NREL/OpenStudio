/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LIGHTINGDESIGNDAY_HPP
#define MODEL_LIGHTINGDESIGNDAY_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class DateTime;
class Date;
class Time;

namespace model {

  namespace detail {

    class LightingDesignDay_Impl;

  }  // namespace detail

  /** LightingDesignDay is a ModelObject that wraps the OpenStudio IDD object 'OS_LightingDesignDay'. */
  class MODEL_API LightingDesignDay : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    LightingDesignDay(const std::string& cieSkyModel, const openstudio::Date& date, const Model& model);

    virtual ~LightingDesignDay() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    LightingDesignDay(const LightingDesignDay& other) = default;
    LightingDesignDay(LightingDesignDay&& other) = default;
    LightingDesignDay& operator=(const LightingDesignDay&) = default;
    LightingDesignDay& operator=(LightingDesignDay&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    static std::vector<std::string> validCIESkyModelValues();

    //@}
    /** @name Getters */
    //@{

    std::string cieSkyModel() const;

    int snowIndicator() const;

    bool isSnowIndicatorDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setCIESkyModel(const std::string& cIESkyModel);

    bool setSnowIndicator(int snowIndicator);

    void resetSnowIndicator();

    //@}

    openstudio::Date date() const;

    bool setDate(const openstudio::Date& date);

    std::vector<openstudio::Time> simulationTimes() const;

    std::vector<openstudio::DateTime> simulationDateTimes() const;

    bool addSimulationTime(const openstudio::Time& time);

    void clearSimulationTimes();

    // ensure that this object does not contain the date 2/29
    void ensureNoLeapDays();

   protected:
    /// @cond
    using ImplType = detail::LightingDesignDay_Impl;

    friend class detail::LightingDesignDay_Impl;
    friend class Model;
    friend class IdfObject;

    explicit LightingDesignDay(std::shared_ptr<detail::LightingDesignDay_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.LightingDesignDay");
  };

  /** \relates LightingDesignDay*/
  using OptionalLightingDesignDay = boost::optional<LightingDesignDay>;

  /** \relates LightingDesignDay*/
  using LightingDesignDayVector = std::vector<LightingDesignDay>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LIGHTINGDESIGNDAY_HPP
