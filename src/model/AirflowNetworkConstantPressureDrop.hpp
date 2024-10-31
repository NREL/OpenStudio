/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKCONSTANTPRESSUREDROP_HPP
#define MODEL_AIRFLOWNETWORKCONSTANTPRESSUREDROP_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class AirflowNetworkConstantPressureDrop_Impl;

  }  // namespace detail

  /** AirflowNetworkConstantPressureDrop is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetwork:ConstantPressureDrop'. */
  class MODEL_API AirflowNetworkConstantPressureDrop : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    //explicit AirflowNetworkConstantPressureDrop(const Model& model);
    /** Construct a constant pressure drop object. */
    AirflowNetworkConstantPressureDrop(const Model& model, double pressureDrop);

    virtual ~AirflowNetworkConstantPressureDrop() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkConstantPressureDrop(const AirflowNetworkConstantPressureDrop& other) = default;
    AirflowNetworkConstantPressureDrop(AirflowNetworkConstantPressureDrop&& other) = default;
    AirflowNetworkConstantPressureDrop& operator=(const AirflowNetworkConstantPressureDrop&) = default;
    AirflowNetworkConstantPressureDrop& operator=(AirflowNetworkConstantPressureDrop&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{
    /** Returns the pressure drop across the component. */
    double pressureDrop() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the pressure drop across the component. */
    bool setPressureDrop(double pressureDrop);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkConstantPressureDrop_Impl;

    explicit AirflowNetworkConstantPressureDrop(std::shared_ptr<detail::AirflowNetworkConstantPressureDrop_Impl> impl);

    friend class detail::AirflowNetworkConstantPressureDrop_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkConstantPressureDrop");
  };

  /** \relates AirflowNetworkConstantPressureDrop*/
  using OptionalAirflowNetworkConstantPressureDrop = boost::optional<AirflowNetworkConstantPressureDrop>;

  /** \relates AirflowNetworkConstantPressureDrop*/
  using AirflowNetworkConstantPressureDropVector = std::vector<AirflowNetworkConstantPressureDrop>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKCONSTANTPRESSUREDROP_HPP
