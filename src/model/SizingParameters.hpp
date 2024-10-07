/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SIZINGPARAMETERS_HPP
#define MODEL_SIZINGPARAMETERS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class SizingParameters_Impl;

  }  // namespace detail

  /** SizingParameters is a ModelObject that wraps the OpenStudio IDD object 'OS:Sizing:Parameters'. */
  class MODEL_API SizingParameters : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~SizingParameters() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SizingParameters(const SizingParameters& other) = default;
    SizingParameters(SizingParameters&& other) = default;
    SizingParameters& operator=(const SizingParameters&) = default;
    SizingParameters& operator=(SizingParameters&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    double heatingSizingFactor() const;

    bool isHeatingSizingFactorDefaulted() const;

    double coolingSizingFactor() const;

    bool isCoolingSizingFactorDefaulted() const;

    boost::optional<int> timestepsinAveragingWindow() const;

    //@}
    /** @name Setters */
    //@{

    bool setHeatingSizingFactor(double heatingSizingFactor);

    void resetHeatingSizingFactor();

    bool setCoolingSizingFactor(double coolingSizingFactor);

    void resetCoolingSizingFactor();

    bool setTimestepsinAveragingWindow(int timestepsinAveragingWindow);

    void resetTimestepsinAveragingWindow();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SizingParameters_Impl;

    explicit SizingParameters(std::shared_ptr<detail::SizingParameters_Impl> impl);

    friend class detail::SizingParameters_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit SizingParameters(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SizingParameters");
  };

  /** \relates SizingParameters*/
  using OptionalSizingParameters = boost::optional<SizingParameters>;

  /** \relates SizingParameters*/
  using SizingParametersVector = std::vector<SizingParameters>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SIZINGPARAMETERS_HPP
