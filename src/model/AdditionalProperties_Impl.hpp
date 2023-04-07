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

#ifndef MODEL_ADDITIONALPROPERTIES_IMPL_HPP
#define MODEL_ADDITIONALPROPERTIES_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class AdditionalProperties;
  class ModelObject;

  namespace detail {

    class MODEL_API AdditionalProperties_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AdditionalProperties_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AdditionalProperties_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AdditionalProperties_Impl(const AdditionalProperties_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~AdditionalProperties_Impl() = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /// return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      /// set the parent, child may have to call non-const methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      /** Get the resources directly used by this ModelObject. */
      virtual std::vector<ResourceObject> resources() const override;

      /** @name Getters */
      //@{

      ModelObject modelObject() const;

      std::vector<std::string> featureNames() const;

      bool hasFeature(const std::string& name) const;

      boost::optional<std::string> getFeatureDataType(const std::string& name) const;

      boost::optional<std::string> getFeatureAsString(const std::string& name) const;

      boost::optional<double> getFeatureAsDouble(const std::string& name) const;

      boost::optional<int> getFeatureAsInteger(const std::string& name) const;

      boost::optional<bool> getFeatureAsBoolean(const std::string& name) const;

      std::vector<std::string> suggestedFeatureNames() const;

      //@}

      /** @name Setters */
      //@{

      bool setFeature(const std::string& name, const std::string& value);

      bool setFeature(const std::string& name, const char* value);

      bool setFeature(const std::string& name, double value);

      bool setFeature(const std::string& name, int value);

      bool setFeature(const std::string& name, bool value);

      bool resetFeature(const std::string& name);

      //@}

      void merge(const AdditionalProperties& other, bool overwrite = false);

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AdditionalProperties")

      boost::optional<ModelExtensibleGroup> getFeatureGroupByName(const std::string& name) const;

      bool setFeatureGroupDataTypeAndValue(const std::string& name, const std::string& dataType, const std::string& value);

      boost::optional<std::string> getFeatureStringAndCheckForType(const std::string& name, const std::string& expectedDataType) const;
    };

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif  //MODEL_ADDITIONALPROPERTIES_IMPL_HPP
