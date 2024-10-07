/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CONVERGENCELIMITS_HPP
#define MODEL_CONVERGENCELIMITS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ConvergenceLimits_Impl;

  }  // namespace detail

  /** ConvergenceLimits is a ModelObject that wraps the OpenStudio IDD object 'OS:ConvergenceLimits'. */
  class MODEL_API ConvergenceLimits : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ConvergenceLimits() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ConvergenceLimits(const ConvergenceLimits& other) = default;
    ConvergenceLimits(ConvergenceLimits&& other) = default;
    ConvergenceLimits& operator=(const ConvergenceLimits&) = default;
    ConvergenceLimits& operator=(ConvergenceLimits&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<int> minimumSystemTimestep() const;

    int maximumHVACIterations() const;

    bool isMaximumHVACIterationsDefaulted() const;

    int minimumPlantIterations() const;

    bool isMinimumPlantIterationsDefaulted() const;

    int maximumPlantIterations() const;

    bool isMaximumPlantIterationsDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setMinimumSystemTimestep(int minimumSystemTimestep);

    void resetMinimumSystemTimestep();

    bool setMaximumHVACIterations(int maximumHVACIterations);

    void resetMaximumHVACIterations();

    bool setMinimumPlantIterations(int minimumPlantIterations);

    void resetMinimumPlantIterations();

    bool setMaximumPlantIterations(int maximumPlantIterations);

    void resetMaximumPlantIterations();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ConvergenceLimits_Impl;

    explicit ConvergenceLimits(std::shared_ptr<detail::ConvergenceLimits_Impl> impl);

    friend class detail::ConvergenceLimits_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    explicit ConvergenceLimits(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ConvergenceLimits");
  };

  /** \relates ConvergenceLimits */
  using OptionalConvergenceLimits = boost::optional<ConvergenceLimits>;

  /** \relates ConvergenceLimits*/
  using ConvergenceLimitsVector = std::vector<ConvergenceLimits>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CONVERGENCELIMITS_HPP
