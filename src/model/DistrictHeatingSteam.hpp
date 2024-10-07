/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DISTRICTHEATINGSTEAM_HPP
#define MODEL_DISTRICTHEATINGSTEAM_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class DistrictHeatingSteam_Impl;

  }  // namespace detail

  /** DistrictHeatingSteam is a StraightComponent that wraps the OpenStudio IDD object 'OS:DistrictHeatingSteam'. */

  class MODEL_API DistrictHeatingSteam : public StraightComponent
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    // The capacityFractionSchedule is defaulted to alwaysOnContinuousSchedule
    explicit DistrictHeatingSteam(const Model& model);

    virtual ~DistrictHeatingSteam() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DistrictHeatingSteam(const DistrictHeatingSteam& other) = default;
    DistrictHeatingSteam(DistrictHeatingSteam&& other) = default;
    DistrictHeatingSteam& operator=(const DistrictHeatingSteam&) = default;
    DistrictHeatingSteam& operator=(DistrictHeatingSteam&&) = default;

    //@}

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityAutosized() const;

    Schedule capacityFractionSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setNominalCapacity(double nominalCapacity);

    void autosizeNominalCapacity();

    bool setCapacityFractionSchedule(Schedule& schedule);

    //@}
    /** @name Other */
    //@{

    //@}

    boost::optional<double> autosizedNominalCapacity() const;

   protected:
    /// @cond
    using ImplType = detail::DistrictHeatingSteam_Impl;

    explicit DistrictHeatingSteam(std::shared_ptr<detail::DistrictHeatingSteam_Impl> impl);

    friend class detail::DistrictHeatingSteam_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.DistrictHeatingSteam");
  };

  /** \relates DistrictHeatingSteam*/
  using OptionalDistrictHeatingSteam = boost::optional<DistrictHeatingSteam>;

  /** \relates DistrictHeatingSteam*/
  using DistrictHeatingSteamVector = std::vector<DistrictHeatingSteam>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DISTRICTHEATINGSTEAM_HPP
