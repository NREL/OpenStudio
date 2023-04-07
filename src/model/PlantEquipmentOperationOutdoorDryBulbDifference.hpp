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

#ifndef MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULBDIFFERENCE_HPP
#define MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULBDIFFERENCE_HPP

#include "ModelAPI.hpp"
#include "PlantEquipmentOperationRangeBasedScheme.hpp"

namespace openstudio {
namespace model {

  class Node;

  namespace detail {

    class PlantEquipmentOperationOutdoorDryBulbDifference_Impl;

  }  // namespace detail

  /** PlantEquipmentOperationOutdoorDryBulbDifference is a PlantEquipmentOperationRangeBasedScheme that wraps the OpenStudio IDD object 'OS:PlantEquipmentOperation:OutdoorDryBulbDifference'. */
  class MODEL_API PlantEquipmentOperationOutdoorDryBulbDifference : public PlantEquipmentOperationRangeBasedScheme
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit PlantEquipmentOperationOutdoorDryBulbDifference(const Model& model);

    virtual ~PlantEquipmentOperationOutdoorDryBulbDifference() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PlantEquipmentOperationOutdoorDryBulbDifference(const PlantEquipmentOperationOutdoorDryBulbDifference& other) = default;
    PlantEquipmentOperationOutdoorDryBulbDifference(PlantEquipmentOperationOutdoorDryBulbDifference&& other) = default;
    PlantEquipmentOperationOutdoorDryBulbDifference& operator=(const PlantEquipmentOperationOutdoorDryBulbDifference&) = default;
    PlantEquipmentOperationOutdoorDryBulbDifference& operator=(PlantEquipmentOperationOutdoorDryBulbDifference&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<Node> referenceTemperatureNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceTemperatureNode(const Node& node);

    void resetReferenceTemperatureNode();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::PlantEquipmentOperationOutdoorDryBulbDifference_Impl;

    explicit PlantEquipmentOperationOutdoorDryBulbDifference(std::shared_ptr<detail::PlantEquipmentOperationOutdoorDryBulbDifference_Impl> impl);

    friend class detail::PlantEquipmentOperationOutdoorDryBulbDifference_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.PlantEquipmentOperationOutdoorDryBulbDifference");
  };

  /** \relates PlantEquipmentOperationOutdoorDryBulbDifference*/
  using OptionalPlantEquipmentOperationOutdoorDryBulbDifference = boost::optional<PlantEquipmentOperationOutdoorDryBulbDifference>;

  /** \relates PlantEquipmentOperationOutdoorDryBulbDifference*/
  using PlantEquipmentOperationOutdoorDryBulbDifferenceVector = std::vector<PlantEquipmentOperationOutdoorDryBulbDifference>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTEQUIPMENTOPERATIONOUTDOORDRYBULBDIFFERENCE_HPP
