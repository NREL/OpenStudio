/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DESIGNSPECIFICATIONOUTDOORAIR_HPP
#define MODEL_DESIGNSPECIFICATIONOUTDOORAIR_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  class Schedule;

  namespace detail {

    class DesignSpecificationOutdoorAir_Impl;

  }  // namespace detail

  /** DesignSpecificationOutdoorAir is a ModelObject that wraps the OpenStudio IDD
 *  object 'OS:DesignSpecification:OutdoorAir'. */
  class MODEL_API DesignSpecificationOutdoorAir : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DesignSpecificationOutdoorAir(const Model& model);

    virtual ~DesignSpecificationOutdoorAir() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DesignSpecificationOutdoorAir(const DesignSpecificationOutdoorAir& other) = default;
    DesignSpecificationOutdoorAir(DesignSpecificationOutdoorAir&& other) = default;
    DesignSpecificationOutdoorAir& operator=(const DesignSpecificationOutdoorAir&) = default;
    DesignSpecificationOutdoorAir& operator=(DesignSpecificationOutdoorAir&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> outdoorAirMethodValues();

    /** \deprecated */
    static std::vector<std::string> validOutdoorAirMethodValues();

    /** @name Getters */
    //@{

    std::string outdoorAirMethod() const;

    bool isOutdoorAirMethodDefaulted() const;

    double outdoorAirFlowperPerson() const;

    bool isOutdoorAirFlowperPersonDefaulted() const;

    double outdoorAirFlowperFloorArea() const;

    bool isOutdoorAirFlowperFloorAreaDefaulted() const;

    double outdoorAirFlowRate() const;

    bool isOutdoorAirFlowRateDefaulted() const;

    double outdoorAirFlowAirChangesperHour() const;

    bool isOutdoorAirFlowAirChangesperHourDefaulted() const;

    /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Outdoor Air Schedule Name" **/
    boost::optional<Schedule> outdoorAirFlowRateFractionSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setOutdoorAirMethod(const std::string& outdoorAirMethod);

    void resetOutdoorAirMethod();

    bool setOutdoorAirFlowperPerson(double outdoorAirFlowperPerson);

    void resetOutdoorAirFlowperPerson();

    bool setOutdoorAirFlowperFloorArea(double outdoorAirFlowperFloorArea);

    void resetOutdoorAirFlowperFloorArea();

    bool setOutdoorAirFlowRate(double outdoorAirFlowRate);

    void resetOutdoorAirFlowRate();

    bool setOutdoorAirFlowAirChangesperHour(double outdoorAirFlowAirChangesperHour);

    void resetOutdoorAirFlowAirChangesperHour();

    /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Outdoor Air Schedule Name" **/
    bool setOutdoorAirFlowRateFractionSchedule(Schedule& schedule);

    void resetOutdoorAirFlowRateFractionSchedule();

    //@}
   protected:
    /// @cond
    using ImplType = detail::DesignSpecificationOutdoorAir_Impl;

    explicit DesignSpecificationOutdoorAir(std::shared_ptr<detail::DesignSpecificationOutdoorAir_Impl> impl);

    friend class detail::DesignSpecificationOutdoorAir_Impl;

    friend class Model;

    friend class IdfObject;

    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.DesignSpecificationOutdoorAir");
  };

  /** \relates DesignSpecificationOutdoorAir*/
  using OptionalDesignSpecificationOutdoorAir = boost::optional<DesignSpecificationOutdoorAir>;

  /** \relates DesignSpecificationOutdoorAir*/
  using DesignSpecificationOutdoorAirVector = std::vector<DesignSpecificationOutdoorAir>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DESIGNSPECIFICATIONOUTDOORAIR_HPP
