/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYINCIDENTSOLARMULTIPLIER_HPP
#define MODEL_SURFACEPROPERTYINCIDENTSOLARMULTIPLIER_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class SubSurface;
  class Schedule;

  namespace detail {

    class SurfacePropertyIncidentSolarMultiplier_Impl;

  }  // namespace detail

  /** SurfacePropertyIncidentSolarMultiplier is a ModelObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:IncidentSolarMultiplier'. */
  class MODEL_API SurfacePropertyIncidentSolarMultiplier : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SurfacePropertyIncidentSolarMultiplier(const SubSurface& subSurface);

    virtual ~SurfacePropertyIncidentSolarMultiplier() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SurfacePropertyIncidentSolarMultiplier(const SurfacePropertyIncidentSolarMultiplier& other) = default;
    SurfacePropertyIncidentSolarMultiplier(SurfacePropertyIncidentSolarMultiplier&& other) = default;
    SurfacePropertyIncidentSolarMultiplier& operator=(const SurfacePropertyIncidentSolarMultiplier&) = default;
    SurfacePropertyIncidentSolarMultiplier& operator=(SurfacePropertyIncidentSolarMultiplier&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    SubSurface subSurface() const;

    double incidentSolarMultiplier() const;

    boost::optional<Schedule> incidentSolarMultiplierSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setSubSurface(const SubSurface& subSurface);

    bool setIncidentSolarMultiplier(double incidentSolarMultiplier);

    bool setIncidentSolarMultiplierSchedule(Schedule& schedule);

    void resetIncidentSolarMultiplierSchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SurfacePropertyIncidentSolarMultiplier_Impl;

    explicit SurfacePropertyIncidentSolarMultiplier(std::shared_ptr<detail::SurfacePropertyIncidentSolarMultiplier_Impl> impl);

    friend class detail::SurfacePropertyIncidentSolarMultiplier_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SurfacePropertyIncidentSolarMultiplier");
  };

  /** \relates SurfacePropertyIncidentSolarMultiplier*/
  using OptionalSurfacePropertyIncidentSolarMultiplier = boost::optional<SurfacePropertyIncidentSolarMultiplier>;

  /** \relates SurfacePropertyIncidentSolarMultiplier*/
  using SurfacePropertyIncidentSolarMultiplierVector = std::vector<SurfacePropertyIncidentSolarMultiplier>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYINCIDENTSOLARMULTIPLIER_HPP
