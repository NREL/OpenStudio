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
    class ThermalZone_Impl;

  }  // namespace detail

  /** This class implements an MRT weighting factor. */
  class MODEL_API MRTWeightingFactor
  {
   public:
    MRTWeightingFactor(const People& people, double mRTWeightingFactor);

    People people() const;
    double mRTWeightingFactor() const;

    bool operator==(const MRTWeightingFactor& other) const = default;
    bool operator!=(const MRTWeightingFactor& other) const;

   private:
    People m_people;
    double m_mRTWeightingFactor;
    REGISTER_LOGGER("openstudio.model.MRTWeightingFactor");
  };

  /** \relates MRTWeightingFactor */
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::MRTWeightingFactor& mRTWeightingFactor);

  /** ZoneMRTCalculation is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneMRTCalculation'. */
  class MODEL_API ZoneMRTCalculation : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

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

    std::vector<MRTWeightingFactor> mRTWeightingFactors() const;

    unsigned int numberofMRTWeightingFactors() const;

    boost::optional<unsigned> mRTWeightingFactorIndex(const People& people) const;

    boost::optional<MRTWeightingFactor> getMRTWeightingFactor(unsigned groupIndex) const;

    bool addMRTWeightingFactor(const MRTWeightingFactor& mRTWeightingFactor);

    bool addMRTWeightingFactor(const People& people, double mRTWeightingFactor);

    bool addMRTWeightingFactors(const std::vector<MRTWeightingFactor>& mRTWeightingFactors);

    void removeMRTWeightingFactor(int groupIndex);

    void removeAllMRTWeightingFactors();

    //@}
   protected:
    /// @cond
    using ImplType = detail::ZoneMRTCalculation_Impl;

    /*
     * This constructor shouldn't be used directly, ThermalZone::getZoneMRTCalculation should be preferred
     * as it will either instantiate a new object or return the existing one.
     * A ThermalZone should not have more than one ZoneMRTCalculation. If you try to instantiate a second one for a given
     * ThermalZone this constructor will throw.
     */
    explicit ZoneMRTCalculation(const ThermalZone& thermalZone);

    explicit ZoneMRTCalculation(std::shared_ptr<detail::ZoneMRTCalculation_Impl> impl);

    friend class detail::ZoneMRTCalculation_Impl;
    friend class detail::ThermalZone_Impl;
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
