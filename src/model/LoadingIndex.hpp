/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LOADINGINDEX_HPP
#define MODEL_LOADINGINDEX_HPP

#include <model/ModelAPI.hpp>
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class LoadingIndex_Impl;

  }  // namespace detail

  /** LoadingIndex is a ParentObject that wraps the OpenStudio IDD object 'OS:AirConditioner:VariableRefrigerantFlow:FluidTemperatureControl:LoadingIndex'. */
  class MODEL_API LoadingIndex : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit LoadingIndex(const Model& model, double compressorSpeed, const Curve& evaporativeCapacityMultiplierFunctionofTemperatureCurve,
                          const Curve& compressorPowerMultiplierFunctionofTemperatureCurve);

    virtual ~LoadingIndex() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    LoadingIndex(const LoadingIndex& other) = default;
    LoadingIndex(LoadingIndex&& other) = default;
    LoadingIndex& operator=(const LoadingIndex&) = default;
    LoadingIndex& operator=(LoadingIndex&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    double compressorSpeed() const;

    Curve evaporativeCapacityMultiplierFunctionofTemperatureCurve() const;

    Curve compressorPowerMultiplierFunctionofTemperatureCurve() const;

    //@}
    /** @name Setters */
    //@{

    bool setCompressorSpeed(double compressorSpeed);

    bool setEvaporativeCapacityMultiplierFunctionofTemperatureCurve(const Curve& curve);

    bool setCompressorPowerMultiplierFunctionofTemperatureCurve(const Curve& curve);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::LoadingIndex_Impl;

    explicit LoadingIndex(std::shared_ptr<detail::LoadingIndex_Impl> impl);

    friend class detail::LoadingIndex_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.LoadingIndex");
  };

  /** \relates LoadingIndex*/
  using OptionalLoadingIndex = boost::optional<LoadingIndex>;

  /** \relates LoadingIndex*/
  using LoadingIndexVector = std::vector<LoadingIndex>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LOADINGINDEX_HPP
