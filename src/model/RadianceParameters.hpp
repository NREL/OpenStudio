/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_RADIANCEPARAMETERS_HPP
#define MODEL_RADIANCEPARAMETERS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class RadianceParameters_Impl;

  }  // namespace detail

  /** RadianceParameters is a ModelObject that wraps the OpenStudio IDD object 'OS:RadianceParameters'. */
  class MODEL_API RadianceParameters : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~RadianceParameters() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RadianceParameters(const RadianceParameters& other) = default;
    RadianceParameters(RadianceParameters&& other) = default;
    RadianceParameters& operator=(const RadianceParameters&) = default;
    RadianceParameters& operator=(RadianceParameters&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> skyDiscretizationResolutionValues();

    /** @name Getters */
    //@{

    int accumulatedRaysperRecord() const;

    bool isAccumulatedRaysperRecordDefaulted() const;

    double directThreshold() const;

    bool isDirectThresholdDefaulted() const;

    double directCertainty() const;

    bool isDirectCertaintyDefaulted() const;

    double directJitter() const;

    bool isDirectJitterDefaulted() const;

    double directPretest() const;

    bool isDirectPretestDefaulted() const;

    int ambientBouncesVMX() const;

    bool isAmbientBouncesVMXDefaulted() const;

    int ambientBouncesDMX() const;

    bool isAmbientBouncesDMXDefaulted() const;

    int ambientDivisionsVMX() const;

    bool isAmbientDivisionsVMXDefaulted() const;

    int ambientDivisionsDMX() const;

    bool isAmbientDivisionsDMXDefaulted() const;

    int ambientSupersamples() const;

    bool isAmbientSupersamplesDefaulted() const;

    double limitWeightVMX() const;

    bool isLimitWeightVMXDefaulted() const;

    double limitWeightDMX() const;

    bool isLimitWeightDMXDefaulted() const;

    int klemsSamplingDensity() const;

    bool isKlemsSamplingDensityDefaulted() const;

    // Note JM 2018-12-12: It's a discrete choice field of numbers, perhaps we should return as an int?
    std::string skyDiscretizationResolution() const;

    bool isCoarseSettings();

    bool isFineSettings();

    //@}
    /** @name Setters */
    //@{

    bool setAccumulatedRaysperRecord(int accumulatedRaysperRecord);

    void resetAccumulatedRaysperRecord();

    bool setDirectThreshold(double directThreshold);

    void resetDirectThreshold();

    bool setDirectCertainty(double directCertainty);

    void resetDirectCertainty();

    bool setDirectJitter(double directJitter);

    void resetDirectJitter();

    bool setDirectPretest(double directPretest);

    void resetDirectPretest();

    bool setAmbientBouncesVMX(int ambientBouncesVMX);

    void resetAmbientBouncesVMX();

    bool setAmbientBouncesDMX(int ambientBouncesDMX);

    void resetAmbientBouncesDMX();

    bool setAmbientDivisionsVMX(int ambientDivisionsVMX);

    void resetAmbientDivisionsVMX();

    bool setAmbientDivisionsDMX(int ambientDivisionsDMX);

    void resetAmbientDivisionsDMX();

    bool setAmbientSupersamples(int ambientSupersamples);

    void resetAmbientSupersamples();

    bool setLimitWeightVMX(double limitWeightVMX);

    void resetLimitWeightVMX();

    bool setLimitWeightDMX(double limitWeightDMX);

    void resetLimitWeightDMX();

    bool setKlemsSamplingDensity(int klemsSamplingDensity);

    void resetKlemsSamplingDensity();

    bool setSkyDiscretizationResolution(const std::string& skyDiscretizationResolution);

    void resetSkyDiscretizationResolution();

    void applyCoarseSettings();

    void applyFineSettings();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::RadianceParameters_Impl;

    explicit RadianceParameters(std::shared_ptr<detail::RadianceParameters_Impl> impl);

    friend class detail::RadianceParameters_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit RadianceParameters(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RadianceParameters");
  };

  /** \relates RadianceParameters*/
  using OptionalRadianceParameters = boost::optional<RadianceParameters>;

  /** \relates RadianceParameters*/
  using RadianceParametersVector = std::vector<RadianceParameters>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_RADIANCEPARAMETERS_HPP
