/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEAIRHEATBALANCEALGORITHM_HPP
#define MODEL_ZONEAIRHEATBALANCEALGORITHM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class ZoneAirHeatBalanceAlgorithm_Impl;

  }  // namespace detail

  /** ZoneAirHeatBalanceAlgorithm is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneAirHeatBalanceAlgorithm'. */
  class MODEL_API ZoneAirHeatBalanceAlgorithm : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ZoneAirHeatBalanceAlgorithm() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneAirHeatBalanceAlgorithm(const ZoneAirHeatBalanceAlgorithm& other) = default;
    ZoneAirHeatBalanceAlgorithm(ZoneAirHeatBalanceAlgorithm&& other) = default;
    ZoneAirHeatBalanceAlgorithm& operator=(const ZoneAirHeatBalanceAlgorithm&) = default;
    ZoneAirHeatBalanceAlgorithm& operator=(ZoneAirHeatBalanceAlgorithm&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validAlgorithmValues();

    /** @name Getters */
    //@{

    std::string algorithm() const;

    bool isAlgorithmDefaulted() const;

    bool doSpaceHeatBalanceforSizing() const;

    bool isDoSpaceHeatBalanceforSizingDefaulted() const;

    bool doSpaceHeatBalanceforSimulation() const;

    bool isDoSpaceHeatBalanceforSimulationDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAlgorithm(const std::string& algorithm);

    void resetAlgorithm();

    bool setDoSpaceHeatBalanceforSizing(bool doSpaceHeatBalanceforSizing);

    void resetDoSpaceHeatBalanceforSizing();

    bool setDoSpaceHeatBalanceforSimulation(bool doSpaceHeatBalanceforSimulation);

    void resetDoSpaceHeatBalanceforSimulation();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneAirHeatBalanceAlgorithm_Impl;

    explicit ZoneAirHeatBalanceAlgorithm(std::shared_ptr<detail::ZoneAirHeatBalanceAlgorithm_Impl> impl);

    friend class detail::ZoneAirHeatBalanceAlgorithm_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit ZoneAirHeatBalanceAlgorithm(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneAirHeatBalanceAlgorithm");
  };

  /** \relates ZoneAirHeatBalanceAlgorithm*/
  using OptionalZoneAirHeatBalanceAlgorithm = boost::optional<ZoneAirHeatBalanceAlgorithm>;

  /** \relates ZoneAirHeatBalanceAlgorithm*/
  using ZoneAirHeatBalanceAlgorithmVector = std::vector<ZoneAirHeatBalanceAlgorithm>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEAIRHEATBALANCEALGORITHM_HPP
