/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKEXTERNALNODE_HPP
#define MODEL_AIRFLOWNETWORKEXTERNALNODE_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkNode.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class AirflowNetworkExternalNode_Impl;

  }  // namespace detail

  /** AirflowNetworkExternalNode is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkExternalNode'. */
  class MODEL_API AirflowNetworkExternalNode : public AirflowNetworkNode
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    /** Construct an external node with defaulted values. */
    explicit AirflowNetworkExternalNode(const Model& model);
    /** Construct an external node with a specified wind pressure curve. */
    explicit AirflowNetworkExternalNode(const Model& model, const Curve& curve);

    virtual ~AirflowNetworkExternalNode() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkExternalNode(const AirflowNetworkExternalNode& other) = default;
    AirflowNetworkExternalNode(AirflowNetworkExternalNode&& other) = default;
    AirflowNetworkExternalNode& operator=(const AirflowNetworkExternalNode&) = default;
    AirflowNetworkExternalNode& operator=(AirflowNetworkExternalNode&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> windAngleTypeValues();

    /** @name Getters */
    //@{
    /** Returns the external node height. */
    double externalNodeHeight() const;
    /** Returns true if the external node height is defaulted. */
    bool isExternalNodeHeightDefaulted() const;
    /** Returns the wind pressure coefficient curve for this external node. */
    Curve windPressureCoefficientCurve() const;
    /** Returns true if the wind pressure curve is to be treated as symmetric. */
    bool symmetricWindPressureCoefficientCurve() const;
    /** Returns true if the wind pressure curve symmetry is defaulted. */
    bool isSymmetricWindPressureCoefficientCurveDefaulted() const;
    /** Returns the wind angle type, "Absolute" or "Relative". */
    std::string windAngleType() const;
    /** Returns true if the wind angle type is defaulted ("Absolute). */
    bool isWindAngleTypeDefaulted() const;

    //@}
    /** @name Setters */
    //@{
    /** Sets the external node height. */
    void setExternalNodeHeight(double externalNodeHeight);
    /** Resets the external node height. */
    void resetExternalNodeHeight();

    /** Sets the wind pressure coefficient curve. */
    bool setWindPressureCoefficientCurve(const Curve& wPCValue);
    /** Sets the wind pressure curve symmetry. */
    void setSymmetricWindPressureCoefficientCurve(bool symmetricWindPressureCoefficientCurve);
    /** Resets the wind pressure curve symmetry. */
    void resetSymmetricWindPressureCoefficientCurve();
    /** Sets the wind angle type. */
    bool setWindAngleType(const std::string& windAngleType);
    /** Resets the wind angle type. */
    void resetWindAngleType();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkExternalNode_Impl;

    explicit AirflowNetworkExternalNode(std::shared_ptr<detail::AirflowNetworkExternalNode_Impl> impl);

    friend class detail::AirflowNetworkExternalNode_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkExternalNode");
  };

  /** \relates AirflowNetworkExternalNode*/
  using OptionalAirflowNetworkExternalNode = boost::optional<AirflowNetworkExternalNode>;

  /** \relates AirflowNetworkExternalNode*/
  using AirflowNetworkExternalNodeVector = std::vector<AirflowNetworkExternalNode>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKEXTERNALNODE_HPP
