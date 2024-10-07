/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATBALANCEALGORITHM_HPP
#define MODEL_HEATBALANCEALGORITHM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class HeatBalanceAlgorithm_Impl;

  }  // namespace detail

  /** HeatBalanceAlgorithm derives from ModelObject and is an interface to the OpenStudio IDD object named "HeatBalanceAlgorithm".
 *
 *  HeatBalanceAlgorithm is a unique object that specifies the global heat and moisture algorithms
 *  used across building construction calculations. HeatBalanceAlgorithm does not have a public constructor because it is a unique ModelObject.
 *  To get the HeatBalanceAlgorithm object for a Model or create one if it does not yet exist use model.getUniqueObject<HeatBalanceAlgorithm>().
 *  To get the HeatBalanceAlgorithm object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<HeatBalanceAlgorithm>().
 */
  class MODEL_API HeatBalanceAlgorithm : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~HeatBalanceAlgorithm() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HeatBalanceAlgorithm(const HeatBalanceAlgorithm& other) = default;
    HeatBalanceAlgorithm(HeatBalanceAlgorithm&& other) = default;
    HeatBalanceAlgorithm& operator=(const HeatBalanceAlgorithm&) = default;
    HeatBalanceAlgorithm& operator=(HeatBalanceAlgorithm&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //static std::vector<std::string> validAlgorithmValues(); TODO delete
    static std::vector<std::string> algorithmValues();

    //@}
    /** @name Getters */
    //@{

    std::string algorithm() const;

    bool isAlgorithmDefaulted() const;

    double surfaceTemperatureUpperLimit() const;

    bool isSurfaceTemperatureUpperLimitDefaulted() const;

    double minimumSurfaceConvectionHeatTransferCoefficientValue() const;

    bool isMinimumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const;

    double maximumSurfaceConvectionHeatTransferCoefficientValue() const;

    bool isMaximumSurfaceConvectionHeatTransferCoefficientValueDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAlgorithm(const std::string& algorithm);

    void resetAlgorithm();

    bool setSurfaceTemperatureUpperLimit(double surfaceTemperatureUpperLimit);

    void resetSurfaceTemperatureUpperLimit();

    bool setMinimumSurfaceConvectionHeatTransferCoefficientValue(double minimumSurfaceConvectionHeatTransferCoefficientValue);

    void resetMinimumSurfaceConvectionHeatTransferCoefficientValue();

    bool setMaximumSurfaceConvectionHeatTransferCoefficientValue(double maximumSurfaceConvectionHeatTransferCoefficientValue);

    void resetMaximumSurfaceConvectionHeatTransferCoefficientValue();

    //@}

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new HeatBalanceAlgorithm object in the model.
    explicit HeatBalanceAlgorithm(const Model& model);

    //@}

    /// @cond

    using ImplType = detail::HeatBalanceAlgorithm_Impl;

    friend class detail::HeatBalanceAlgorithm_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit HeatBalanceAlgorithm(Model& model);
    // constructor
    explicit HeatBalanceAlgorithm(std::shared_ptr<detail::HeatBalanceAlgorithm_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.HeatBalanceAlgorithm");

    /// @endcond
  };

  /** \relates HeatBalanceAlgorithm*/
  using OptionalHeatBalanceAlgorithm = boost::optional<HeatBalanceAlgorithm>;

  /** \relates HeatBalanceAlgorithm*/
  using HeatBalanceAlgorithmVector = std::vector<HeatBalanceAlgorithm>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATBALANCEALGORITHM_HPP
