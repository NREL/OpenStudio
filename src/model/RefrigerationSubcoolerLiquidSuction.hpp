/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONSUBCOOLERLIQUIDSUCTION_HPP
#define MODEL_REFRIGERATIONSUBCOOLERLIQUIDSUCTION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class RefrigerationSystem;

  namespace detail {

    class RefrigerationSubcoolerLiquidSuction_Impl;

  }  // namespace detail

  /** RefrigerationSubcoolerLiquidSuction is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:Subcooler:LiquidSuction'. */
  class MODEL_API RefrigerationSubcoolerLiquidSuction : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationSubcoolerLiquidSuction(const Model& model);

    virtual ~RefrigerationSubcoolerLiquidSuction() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationSubcoolerLiquidSuction(const RefrigerationSubcoolerLiquidSuction& other) = default;
    RefrigerationSubcoolerLiquidSuction(RefrigerationSubcoolerLiquidSuction&& other) = default;
    RefrigerationSubcoolerLiquidSuction& operator=(const RefrigerationSubcoolerLiquidSuction&) = default;
    RefrigerationSubcoolerLiquidSuction& operator=(RefrigerationSubcoolerLiquidSuction&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<double> liquidSuctionDesignSubcoolingTemperatureDifference() const;

    boost::optional<double> designLiquidInletTemperature() const;

    boost::optional<double> designVaporInletTemperature() const;

    //@}
    /** @name Setters */
    //@{

    bool setLiquidSuctionDesignSubcoolingTemperatureDifference(double liquidSuctionDesignSubcoolingTemperatureDifference);

    void resetLiquidSuctionDesignSubcoolingTemperatureDifference();

    bool setDesignLiquidInletTemperature(double designLiquidInletTemperature);

    void resetDesignLiquidInletTemperature();

    bool setDesignVaporInletTemperature(double designVaporInletTemperature);

    void resetDesignVaporInletTemperature();

    //@}
    /** @name Other */
    //@{

    // The parent RefrigerationSystem, for which condensate is cooled.
    // This is a convenience method to find any RefrigerationSystem that uses this subcooler
    boost::optional<RefrigerationSystem> system() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::RefrigerationSubcoolerLiquidSuction_Impl;

    explicit RefrigerationSubcoolerLiquidSuction(std::shared_ptr<detail::RefrigerationSubcoolerLiquidSuction_Impl> impl);

    friend class detail::RefrigerationSubcoolerLiquidSuction_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationSubcoolerLiquidSuction");
  };

  /** \relates RefrigerationSubcoolerLiquidSuction*/
  using OptionalRefrigerationSubcoolerLiquidSuction = boost::optional<RefrigerationSubcoolerLiquidSuction>;

  /** \relates RefrigerationSubcoolerLiquidSuction*/
  using RefrigerationSubcoolerLiquidSuctionVector = std::vector<RefrigerationSubcoolerLiquidSuction>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONSUBCOOLERLIQUIDSUCTION_HPP
