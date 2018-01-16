/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_METERCUSTOM_HPP
#define MODEL_METERCUSTOM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

//#include <memory>

namespace openstudio {
namespace model {

namespace detail {

  class MeterCustom_Impl;

} // detail

/** MeterCustom is a ModelObject that wraps the OpenStudio IDD object 'OS:Meter:Custom'. */
class MODEL_API MeterCustom : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  // Constructs a new MeterCustom object in the model.
  explicit MeterCustom(const Model& model);

  virtual ~MeterCustom() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> fuelTypeValues();

  /** @name Getters */
  //@{

  boost::optional<std::string> fuelType() const;

  // Return a vector of (Key, Var) pairs
  std::vector< std::pair<std::string, std::string> > keyVarGroups();

  // Return the number of (KeyName, OutputVariableorMeterName) groups
  unsigned numKeyVarGroups() const;


  // Lower Level functions
  /** Get the Key Name at index. Indexing starts at 0. */
  boost::optional<std::string> keyName(unsigned index) const;

  /** Get the Output Variable of Meter Name at index. Indexing starts at 0. */
  boost::optional<std::string> outputVariableorMeterName(unsigned index) const;

  //@}
  /** @name Setters */
  //@{

  bool setFuelType(const std::string& fuelType);

  void resetFuelType();

  // Add a new (Key, Var) group
  bool addKeyVarGroup(const std::string& keyName, const std::string& outputVariableorMeterName);

  // Remove the (Key, Var) group at given index
  bool removeKeyVarGroup(unsigned groupIndex);

  // Remove all the (Key, Var) groups
  void removeAllKeyVarGroups();


  // Lower level functions
  /** Set the Key Name at index. Indexing starts at 0. */
  bool setKeyName(unsigned index, const std::string& str);

  /** Set the Output Variable of Meter Name at index. Indexing starts at 0. */
  bool setOutputVariableorMeterName(unsigned index, const std::string& str);

  //@}
  /** @name Other */
  //@{

  //@}
  /** @name Type Casting */
  //@{


  //@}
 protected:
  /// @cond
  typedef detail::MeterCustom_Impl ImplType;

  explicit MeterCustom(std::shared_ptr<detail::MeterCustom_Impl> impl);

  friend class detail::MeterCustom_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  std::shared_ptr<detail::MeterCustom_Impl> m_impl;

  REGISTER_LOGGER("openstudio.model.MeterCustom");
};

/** \relates MeterCustom*/
typedef boost::optional<MeterCustom> OptionalMeterCustom;

/** \relates MeterCustom*/
typedef std::vector<MeterCustom> MeterCustomVector;

} // model
} // openstudio

#endif // MODEL_METERCUSTOM_HPP

