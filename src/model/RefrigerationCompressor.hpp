/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONCOMPRESSOR_HPP
#define MODEL_REFRIGERATIONCOMPRESSOR_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class RefrigerationSystem;
  class CurveBicubic;

  namespace detail {

    class RefrigerationCompressor_Impl;

  }  // namespace detail

  /** RefrigerationCompressor is a ParentObject that wraps the OpenStudio IDD object 'OS:Refrigeration:Compressor'. */
  class MODEL_API RefrigerationCompressor : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationCompressor(const Model& model);

    virtual ~RefrigerationCompressor() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationCompressor(const RefrigerationCompressor& other) = default;
    RefrigerationCompressor(RefrigerationCompressor&& other) = default;
    RefrigerationCompressor& operator=(const RefrigerationCompressor&) = default;
    RefrigerationCompressor& operator=(RefrigerationCompressor&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> modeofOperationValues();

    /** @name Getters */
    //@{

    CurveBicubic refrigerationCompressorPowerCurve() const;

    CurveBicubic refrigerationCompressorCapacityCurve() const;

    boost::optional<double> ratedSuperheat() const;

    boost::optional<double> ratedReturnGasTemperature() const;

    boost::optional<double> ratedLiquidTemperature() const;

    boost::optional<double> ratedSubcooling() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    std::string modeofOperation() const;

    // bool isModeofOperationDefaulted() const;

    boost::optional<CurveBicubic> transcriticalCompressorPowerCurve() const;

    boost::optional<CurveBicubic> transcriticalCompressorCapacityCurve() const;

    // Returns the parent RefrigerationSystem if any
    boost::optional<RefrigerationSystem> system() const;

    //@}
    /** @name Setters */
    //@{

    bool setRefrigerationCompressorPowerCurve(const CurveBicubic& curveBicubic);

    bool setRefrigerationCompressorCapacityCurve(const CurveBicubic& curveBicubic);

    bool setRatedSuperheat(double ratedSuperheat);

    void resetRatedSuperheat();

    bool setRatedReturnGasTemperature(double ratedReturnGasTemperature);

    void resetRatedReturnGasTemperature();

    bool setRatedLiquidTemperature(double ratedLiquidTemperature);

    void resetRatedLiquidTemperature();

    bool setRatedSubcooling(double ratedSubcooling);

    void resetRatedSubcooling();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    // bool setModeofOperation(const std::string& modeofOperation);

    // void resetModeofOperation();

    bool setTranscriticalCompressorPowerCurve(const CurveBicubic& curveBicubic);

    void resetTranscriticalCompressorPowerCurve();

    bool setTranscriticalCompressorCapacityCurve(const CurveBicubic& curveBicubic);

    void resetTranscriticalCompressorCapacityCurve();

    // Remove from parentt system if any
    void removeFromSystem();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::RefrigerationCompressor_Impl;

    explicit RefrigerationCompressor(std::shared_ptr<detail::RefrigerationCompressor_Impl> impl);

    friend class detail::RefrigerationCompressor_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationCompressor");
  };

  /** \relates RefrigerationCompressor*/
  using OptionalRefrigerationCompressor = boost::optional<RefrigerationCompressor>;

  /** \relates RefrigerationCompressor*/
  using RefrigerationCompressorVector = std::vector<RefrigerationCompressor>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONCOMPRESSOR_HPP
