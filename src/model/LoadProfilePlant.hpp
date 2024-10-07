/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LOADPROFILEPLANT_HPP
#define MODEL_LOADPROFILEPLANT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class LoadProfilePlant_Impl;

  }  // namespace detail

  /** LoadProfilePlant is a StraightComponent that wraps the OpenStudio IDD object 'OS:LoadProfile:Plant'. */
  class MODEL_API LoadProfilePlant : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit LoadProfilePlant(const Model& model);

    explicit LoadProfilePlant(const Model& model, Schedule& loadSchedule, Schedule& flowRateFractionSchedule);

    virtual ~LoadProfilePlant() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    LoadProfilePlant(const LoadProfilePlant& other) = default;
    LoadProfilePlant(LoadProfilePlant&& other) = default;
    LoadProfilePlant& operator=(const LoadProfilePlant&) = default;
    LoadProfilePlant& operator=(LoadProfilePlant&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    Schedule loadSchedule() const;

    double peakFlowRate() const;

    Schedule flowRateFractionSchedule() const;

    std::string plantLoopFluidType() const;

    double degreeofSubCooling() const;

    double degreeofLoopSubCooling() const;

    //@}
    /** @name Setters */
    //@{

    bool setLoadSchedule(Schedule& schedule);

    bool setPeakFlowRate(double peakFlowRate);

    bool setFlowRateFractionSchedule(Schedule& schedule);

    bool setPlantLoopFluidType(const std::string& plantLoopFluidType);

    bool setDegreeofSubCooling(double degreeofSubCooling);

    bool setDegreeofLoopSubCooling(double degreeofLoopSubCooling);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::LoadProfilePlant_Impl;

    explicit LoadProfilePlant(std::shared_ptr<detail::LoadProfilePlant_Impl> impl);

    friend class detail::LoadProfilePlant_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.LoadProfilePlant");
  };

  /** \relates LoadProfilePlant*/
  using OptionalLoadProfilePlant = boost::optional<LoadProfilePlant>;

  /** \relates LoadProfilePlant*/
  using LoadProfilePlantVector = std::vector<LoadProfilePlant>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LOADPROFILEPLANT_HPP
