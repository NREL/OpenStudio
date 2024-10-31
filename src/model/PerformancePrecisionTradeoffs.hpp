/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PERFORMANCEPRECISIONTRADEOFFS_HPP
#define MODEL_PERFORMANCEPRECISIONTRADEOFFS_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class PerformancePrecisionTradeoffs_Impl;

  }  // namespace detail

  /** PerformancePrecisionTradeoffs is a ModelObject that wraps the OpenStudio IDD object 'OS:PerformancePrecisionTradeoffs'. */
  class MODEL_API PerformancePrecisionTradeoffs : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~PerformancePrecisionTradeoffs() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PerformancePrecisionTradeoffs(const PerformancePrecisionTradeoffs& other) = default;
    PerformancePrecisionTradeoffs(PerformancePrecisionTradeoffs&& other) = default;
    PerformancePrecisionTradeoffs& operator=(const PerformancePrecisionTradeoffs&) = default;
    PerformancePrecisionTradeoffs& operator=(PerformancePrecisionTradeoffs&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> zoneRadiantExchangeAlgorithmValues();
    static std::vector<std::string> validZoneRadiantExchangeAlgorithmValues();

    static std::vector<std::string> overrideModeValues();
    static std::vector<std::string> validOverrideModeValues();

    /** @name Getters */
    //@{

    bool useCoilDirectSolutions() const;
    bool isUseCoilDirectSolutionsDefaulted() const;

    std::string zoneRadiantExchangeAlgorithm() const;
    bool isZoneRadiantExchangeAlgorithmDefaulted() const;

    std::string overrideMode() const;
    bool isOverrideModeDefaulted() const;

    double maxZoneTempDiff() const;
    bool isMaxZoneTempDiffDefaulted() const;

    double maxAllowedDelTemp() const;
    bool isMaxAllowedDelTempDefaulted() const;

    bool useRepresentativeSurfacesforCalculations() const;
    bool isUseRepresentativeSurfacesforCalculationsDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setUseCoilDirectSolutions(bool useCoilDirectSolutions);
    void resetUseCoilDirectSolutions();

    bool setZoneRadiantExchangeAlgorithm(const std::string& zoneRadiantExchangeAlgorithm);
    void resetZoneRadiantExchangeAlgorithm();

    bool setOverrideMode(const std::string& overrideMode);
    void resetOverrideMode();

    bool setMaxZoneTempDiff(double maxZoneTempDiff);
    void resetMaxZoneTempDiff();

    bool setMaxAllowedDelTemp(double maxAllowedDelTemp);
    void resetMaxAllowedDelTemp();

    bool setUseRepresentativeSurfacesforCalculations(bool useRepresentativeSurfacesforCalculations);
    void resetUseRepresentativeSurfacesforCalculations();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    explicit PerformancePrecisionTradeoffs(Model& model);

    /// @cond
    using ImplType = detail::PerformancePrecisionTradeoffs_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::PerformancePrecisionTradeoffs_Impl;

    explicit PerformancePrecisionTradeoffs(std::shared_ptr<detail::PerformancePrecisionTradeoffs_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PerformancePrecisionTradeoffs");
  };

  /** \relates PerformancePrecisionTradeoffs*/
  using OptionalPerformancePrecisionTradeoffs = boost::optional<PerformancePrecisionTradeoffs>;

  /** \relates PerformancePrecisionTradeoffs*/
  using PerformancePrecisionTradeoffsVector = std::vector<PerformancePrecisionTradeoffs>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PERFORMANCEPRECISIONTRADEOFFS_HPP
