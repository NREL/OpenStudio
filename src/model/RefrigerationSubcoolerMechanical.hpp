/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONSUBCOOLERMECHANICAL_HPP
#define MODEL_REFRIGERATIONSUBCOOLERMECHANICAL_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class RefrigerationSystem;

  namespace detail {

    class RefrigerationSubcoolerMechanical_Impl;

  }  // namespace detail

  /** RefrigerationSubcoolerMechanical is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:Subcooler:Mechanical'. */
  class MODEL_API RefrigerationSubcoolerMechanical : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationSubcoolerMechanical(const Model& model);

    virtual ~RefrigerationSubcoolerMechanical() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationSubcoolerMechanical(const RefrigerationSubcoolerMechanical& other) = default;
    RefrigerationSubcoolerMechanical(RefrigerationSubcoolerMechanical&& other) = default;
    RefrigerationSubcoolerMechanical& operator=(const RefrigerationSubcoolerMechanical&) = default;
    RefrigerationSubcoolerMechanical& operator=(RefrigerationSubcoolerMechanical&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    // The system that provides the cooling capacity
    boost::optional<RefrigerationSystem> capacityProvidingSystem() const;

    boost::optional<double> outletControlTemperature() const;

    //@}
    /** @name Setters */
    //@{

    bool setCapacityProvidingSystem(const RefrigerationSystem& refrigerationSystem);

    void resetCapacityProvidingSystem();

    bool setOutletControlTemperature(double outletControlTemperature);

    void resetOutletControlTemperature();

    //@}
    /** @name Other */
    //@{

    // The parent RefrigerationSystem, for which condensate is cooled.
    // This is a convenience method to find any RefrigerationSystem that uses this subcooler
    boost::optional<RefrigerationSystem> system() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::RefrigerationSubcoolerMechanical_Impl;

    explicit RefrigerationSubcoolerMechanical(std::shared_ptr<detail::RefrigerationSubcoolerMechanical_Impl> impl);

    friend class detail::RefrigerationSubcoolerMechanical_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationSubcoolerMechanical");
  };

  /** \relates RefrigerationSubcoolerMechanical*/
  using OptionalRefrigerationSubcoolerMechanical = boost::optional<RefrigerationSubcoolerMechanical>;

  /** \relates RefrigerationSubcoolerMechanical*/
  using RefrigerationSubcoolerMechanicalVector = std::vector<RefrigerationSubcoolerMechanical>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONSUBCOOLERMECHANICAL_HPP
