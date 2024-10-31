/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONCONDENSERCASCADE_HPP
#define MODEL_REFRIGERATIONCONDENSERCASCADE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class RefrigerationSystem;

  namespace detail {

    class RefrigerationCondenserCascade_Impl;

  }  // namespace detail

  /** RefrigerationCondenserCascade is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:Condenser:Cascade'. */
  class MODEL_API RefrigerationCondenserCascade : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationCondenserCascade(const Model& model);

    virtual ~RefrigerationCondenserCascade() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationCondenserCascade(const RefrigerationCondenserCascade& other) = default;
    RefrigerationCondenserCascade(RefrigerationCondenserCascade&& other) = default;
    RefrigerationCondenserCascade& operator=(const RefrigerationCondenserCascade&) = default;
    RefrigerationCondenserCascade& operator=(RefrigerationCondenserCascade&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> condensingTemperatureControlTypeValues();

    /** @name Getters */
    //@{

    double ratedCondensingTemperature() const;

    double ratedApproachTemperatureDifference() const;

    bool isRatedApproachTemperatureDifferenceDefaulted() const;

    double ratedEffectiveTotalHeatRejectionRate() const;

    std::string condensingTemperatureControlType() const;

    bool isCondensingTemperatureControlTypeDefaulted() const;

    boost::optional<double> condenserRefrigerantOperatingChargeInventory() const;

    boost::optional<double> condensateReceiverRefrigerantInventory() const;

    boost::optional<double> condensatePipingRefrigerantInventory() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedCondensingTemperature(double ratedCondensingTemperature);

    bool setRatedApproachTemperatureDifference(double ratedApproachTemperatureDifference);

    void resetRatedApproachTemperatureDifference();

    bool setRatedEffectiveTotalHeatRejectionRate(double ratedEffectiveTotalHeatRejectionRate);

    bool setCondensingTemperatureControlType(const std::string& condensingTemperatureControlType);

    void resetCondensingTemperatureControlType();

    bool setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory);

    void resetCondenserRefrigerantOperatingChargeInventory();

    bool setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory);

    void resetCondensateReceiverRefrigerantInventory();

    bool setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory);

    void resetCondensatePipingRefrigerantInventory();

    //@}
    /** @name Other */
    //@{

    // Returns the parent RefrigerationSystem if any, that lists it in its TransferLoadList: this Cascade Condenser is **cooled** by this system
    boost::optional<RefrigerationSystem> system() const;

    // Remove from parent system in CondenserCascadeLoads if any
    void removeFromSystem();

    // Returns the parent RefrigerationSystem if any that lists it in 'Condenser Name':
    // this Cascade Condenser is **absorbing heat rejected by this system**
    boost::optional<RefrigerationSystem> heatRejectingSystem() const;

    void removeFromHeatRejectingSystem();

    //@}
   protected:
    /// @cond
    using ImplType = detail::RefrigerationCondenserCascade_Impl;

    explicit RefrigerationCondenserCascade(std::shared_ptr<detail::RefrigerationCondenserCascade_Impl> impl);

    friend class detail::RefrigerationCondenserCascade_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationCondenserCascade");
  };

  /** \relates RefrigerationCondenserCascade*/
  using OptionalRefrigerationCondenserCascade = boost::optional<RefrigerationCondenserCascade>;

  /** \relates RefrigerationCondenserCascade*/
  using RefrigerationCondenserCascadeVector = std::vector<RefrigerationCondenserCascade>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONCONDENSERCASCADE_HPP
