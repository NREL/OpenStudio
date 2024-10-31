/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PHOTOVOLTAICPERFORMANCESIMPLE_HPP
#define MODEL_PHOTOVOLTAICPERFORMANCESIMPLE_HPP

#include "ModelAPI.hpp"
#include "PhotovoltaicPerformance.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class PhotovoltaicPerformanceSimple_Impl;

  }  // namespace detail

  /** PhotovoltaicPerformanceSimple is a PhotovoltaicPerformance that wraps the OpenStudio IDD object 'OS:PhotovoltaicPerformance:Simple'. */
  class MODEL_API PhotovoltaicPerformanceSimple : public PhotovoltaicPerformance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PhotovoltaicPerformanceSimple(const Model& model);

    virtual ~PhotovoltaicPerformanceSimple() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PhotovoltaicPerformanceSimple(const PhotovoltaicPerformanceSimple& other) = default;
    PhotovoltaicPerformanceSimple(PhotovoltaicPerformanceSimple&& other) = default;
    PhotovoltaicPerformanceSimple& operator=(const PhotovoltaicPerformanceSimple&) = default;
    PhotovoltaicPerformanceSimple& operator=(PhotovoltaicPerformanceSimple&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double fractionOfSurfaceAreaWithActiveSolarCells() const;

    bool isfractionOfSurfaceAreaWithActiveSolarCellsDefaulted() const;

    std::string conversionEfficiencyInputMode() const;

    boost::optional<double> fixedEfficiency() const;

    boost::optional<Schedule> efficiencySchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setFractionOfSurfaceAreaWithActiveSolarCells(double fractionOfSurfaceAreaWithActiveSolarCells);

    void resetFractionOfSurfaceAreaWithActiveSolarCells();

    bool setFixedEfficiency(double fixedEfficiency);

    void resetFixedEfficiency();

    bool setEfficiencySchedule(Schedule& schedule);

    void resetEfficiencySchedule();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::PhotovoltaicPerformanceSimple_Impl;

    explicit PhotovoltaicPerformanceSimple(std::shared_ptr<detail::PhotovoltaicPerformanceSimple_Impl> impl);

    friend class detail::PhotovoltaicPerformanceSimple_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PhotovoltaicPerformanceSimple");
  };

  /** \relates PhotovoltaicPerformanceSimple*/
  using OptionalPhotovoltaicPerformanceSimple = boost::optional<PhotovoltaicPerformanceSimple>;

  /** \relates PhotovoltaicPerformanceSimple*/
  using PhotovoltaicPerformanceSimpleVector = std::vector<PhotovoltaicPerformanceSimple>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PHOTOVOLTAICPERFORMANCESIMPLE_HPP
