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

#ifndef MODEL_SPACEITEM_IMPL_HPP
#define MODEL_SPACEITEM_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  // forward declaration
  class Space;
  class SpaceItem;

  namespace detail {

    class MODEL_API SpaceItem_Impl : public ModelObject_Impl
    {

     public:
      // Construct completely new object.
      SpaceItem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // Construct from existing workspace object (typically when Model is being constructed
      // from Workspace).
      SpaceItem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // Clone copy constructor.
      SpaceItem_Impl(const SpaceItem_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SpaceItem_Impl() = default;

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      /// set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      /// Returns the parent Space.
      boost::optional<Space> space() const;

      /// Sets the parent Space.
      bool setSpace(const Space& space);

      void resetSpace();

     protected:
      // index of the space name
      virtual int spaceIndex() const = 0;

     private:
      REGISTER_LOGGER("openstudio.model.SpaceItem");

      boost::optional<ModelObject> spaceAsModelObject() const;

      bool setSpaceAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SPACEITEM_IMPL_HPP
