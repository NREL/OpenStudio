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

#ifndef MODEL_PORTLIST_IMPL_HPP
#define MODEL_PORTLIST_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {

namespace model {

class Node;

class ThermalZone;

class HVACComponent;

namespace detail {

/** PortList_Impl is a ModelObject_Impl that is the implementation class for PortList.*/
class MODEL_API PortList_Impl : public ModelObject_Impl {
 public:

  PortList_Impl(const IdfObject& idfObject,
                Model_Impl* model,
                bool keepHandle);

  PortList_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                Model_Impl* model,
                bool keepHandle);

  PortList_Impl(const PortList_Impl& other,
                Model_Impl* model,
                bool keepHandle);

  virtual ~PortList_Impl() {}

  unsigned port(unsigned portIndex);

  unsigned portIndex(unsigned port);

  unsigned nextPort();

  boost::optional<ModelObject> modelObject(unsigned portIndex);

  boost::optional<ModelObject> lastModelObject();

  std::vector<ModelObject> modelObjects();

  unsigned newPortAfterIndex(unsigned portIndex);

  unsigned portIndexForModelObject( ModelObject & modelObject, bool * ok = nullptr );

  unsigned nextPortIndex();

  void removePort(unsigned port);

  virtual const std::vector<std::string>& outputVariableNames() const override;

  virtual IddObjectType iddObjectType() const override;

  ThermalZone thermalZone() const;

  std::vector<openstudio::IdfObject> remove() override;

  unsigned airLoopHVACPort();

  unsigned airLoopHVACPortIndex();

  boost::optional<ModelObject> airLoopHVACModelObject();

  // Return the HVACComponent that the port list is associated with.
  HVACComponent hvacComponent() const;

  // Set the hvacComponent that the port list is associated with.
  bool setHVACComponent(const HVACComponent & hvacComponent);

  protected:

  private:

  REGISTER_LOGGER("openstudio.model.PortList");
};

} // detail

} // model
} // openstudio

#endif // MODEL_PORTLIST_IMPL_HPP

