/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_AIRFLOWNETWORKEXTERNALNODE_HPP
#define MODEL_AIRFLOWNETWORKEXTERNALNODE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

class Curve;

namespace detail {

  class AirflowNetworkExternalNode_Impl;

} // detail

/** AirflowNetworkExternalNode is a ModelObject that wraps the OpenStudio IDD object 'OS:AirflowNetworkExternalNode'. */
class MODEL_API AirflowNetworkExternalNode : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirflowNetworkExternalNode(const Model& model);

  virtual ~AirflowNetworkExternalNode() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> windAngleTypeValues();

  /** @name Getters */
  //@{

  double externalNodeHeight() const;

  bool isExternalNodeHeightDefaulted() const;

  Curve windPressureCoefficientCurve() const;

  bool symmetricWindPressureCoefficientCurve() const;

  bool isSymmetricWindPressureCoefficientCurveDefaulted() const;

  std::string windAngleType() const;

  bool isWindAngleTypeDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  void setExternalNodeHeight(double externalNodeHeight);

  void resetExternalNodeHeight();

  // TODO: Check argument type. From object lists, some candidates are: WPCValue, UniVariateCurves, UniVariateTables.
  bool setWindPressureCoefficientCurve(const Curve& wPCValue);

  void setSymmetricWindPressureCoefficientCurve(bool symmetricWindPressureCoefficientCurve);

  void resetSymmetricWindPressureCoefficientCurve();

  bool setWindAngleType(const std::string& windAngleType);

  void resetWindAngleType();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirflowNetworkExternalNode_Impl ImplType;

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
typedef boost::optional<AirflowNetworkExternalNode> OptionalAirflowNetworkExternalNode;

/** \relates AirflowNetworkExternalNode*/
typedef std::vector<AirflowNetworkExternalNode> AirflowNetworkExternalNodeVector;

} // model
} // openstudio

#endif // MODEL_AIRFLOWNETWORKEXTERNALNODE_HPP

