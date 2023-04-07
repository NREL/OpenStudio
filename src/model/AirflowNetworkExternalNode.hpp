/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    virtual ~AirflowNetworkExternalNode() = default;
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
