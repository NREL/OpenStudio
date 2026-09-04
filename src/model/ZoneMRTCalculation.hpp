/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEMRTCALCULATION_HPP
#define MODEL_ZONEMRTCALCULATION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "People.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;

  namespace detail {

    class ZoneMRTCalculation_Impl;

  }  // namespace detail

  /** This class implements an MRT weighting factor. */
  class MODEL_API MRTWeightingFactor
  {
   public:
    MRTWeightingFactor(const People& people, double mrtWeightingFactor);

    People people() const;
    double mrtWeightingFactor() const;

   private:
    People m_people;
    double m_mrtWeightingFactor;
    REGISTER_LOGGER("openstudio.model.MRTWeightingFactor");
  };

  /** \relates MRTWeightingFactor */
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::MRTWeightingFactor& mrtWeightingFactor);

  /** ZoneMRTCalculation is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneMRTCalculation'. */
  class MODEL_API ZoneMRTCalculation : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /*
     * This constructor shouldn't be used, ThermalZone::getZoneMRTCalculation should be preferred
     * as it will either instantiate a new object or return the existing one.
     * A ThermalZone should not have more than one ZoneMRTCalculation. If you try to instantiate a second one for a given
     * ThermalZone this constructor will throw.
     */
    explicit ZoneMRTCalculation(const ThermalZone& thermalZone);

    virtual ~ZoneMRTCalculation() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ZoneMRTCalculation(const ZoneMRTCalculation& other) = default;
    ZoneMRTCalculation(ZoneMRTCalculation&& other) = default;
    ZoneMRTCalculation& operator=(const ZoneMRTCalculation&) = default;
    ZoneMRTCalculation& operator=(ZoneMRTCalculation&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ThermalZone thermalZone() const;

    //@}
    /** @name Setters */
    //@{

    //@}
    /** @name Other */
    //@{

    std::vector<MRTWeightingFactor> mrtWeightingFactors() const;

    unsigned int numberofMRTWeightingFactors() const;

    boost::optional<unsigned> mrtWeightingFactorIndex(const People& people) const;

    boost::optional<MRTWeightingFactor> getMRTWeightingFactor(unsigned groupIndex) const;

    bool addMRTWeightingFactor(const MRTWeightingFactor& mrtWeightingFactor);

    bool addMRTWeightingFactor(const People& people, double mrtWeightingFactor);

    bool addMRTWeightingFactors(const std::vector<MRTWeightingFactor>& mrtWeightingFactors);

    void removeMRTWeightingFactor(int groupIndex);

    void removeAllMRTWeightingFactors();

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneMRTCalculation_Impl;

    explicit ZoneMRTCalculation(std::shared_ptr<detail::ZoneMRTCalculation_Impl> impl);

    friend class detail::ZoneMRTCalculation_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ZoneMRTCalculation");
  };

  /** \relates ZoneMRTCalculation*/
  using OptionalZoneMRTCalculation = boost::optional<ZoneMRTCalculation>;

  /** \relates ZoneMRTCalculation*/
  using ZoneMRTCalculationVector = std::vector<ZoneMRTCalculation>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEMRTCALCULATION_HPP