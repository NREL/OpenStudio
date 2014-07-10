/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef MODEL_LOOP_IMPL_HPP
#define MODEL_LOOP_IMPL_HPP

#include "ParentObject_Impl.hpp"

namespace openstudio {

namespace model {

class Node;
class HVACComponent;
class Splitter;
class Mixer;

namespace detail {

  class Model_Impl;

  class MODEL_API Loop_Impl : public ParentObject_Impl {

    Q_OBJECT;
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> supplyInletNode READ supplyInletNodeAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> supplyOutletNode READ supplyOutletNodeAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> demandInletNode READ demandInletNodeAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> demandOutletNode READ demandOutletNodeAsModelObject);

  public:

    Loop_Impl(IddObjectType type, Model_Impl* model);

    Loop_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    Loop_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
              Model_Impl* model,
              bool keepHandle);

    Loop_Impl(const Loop_Impl& other, Model_Impl* model, bool keepHandles);

    virtual ~Loop_Impl() {}

    virtual Node supplyInletNode() const;

    virtual Node supplyOutletNode() const;

    virtual Node demandInletNode() const;

    virtual Node demandOutletNode() const;

    virtual std::vector<ModelObject> children() const;

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual std::vector<ModelObject> supplyComponents( HVACComponent inletComp,
                                                       HVACComponent outletComp,
                                                       openstudio::IddObjectType type = IddObjectType::Catchall) const;

    virtual std::vector<ModelObject> demandComponents( HVACComponent inletComp,
                                                       HVACComponent outletComp,
                                                       openstudio::IddObjectType type = IddObjectType::Catchall);

    virtual std::vector<ModelObject> supplyComponents(openstudio::IddObjectType type=IddObjectType::Catchall) const;

    virtual std::vector<ModelObject> demandComponents(openstudio::IddObjectType type=IddObjectType::Catchall);

    virtual std::vector<ModelObject> components(openstudio::IddObjectType type = IddObjectType::Catchall);

    std::vector<ModelObject> components(HVACComponent inletComp,
                                        HVACComponent outletComp,
                                        openstudio::IddObjectType type = IddObjectType::Catchall);

    virtual boost::optional<ModelObject> component(openstudio::Handle handle);

    virtual boost::optional<ModelObject> demandComponent(openstudio::Handle handle);

    virtual boost::optional<ModelObject> supplyComponent(openstudio::Handle handle) const;

    virtual std::vector<ModelObject> supplyComponents(std::vector<HVACComponent> inletComps,
                                                      std::vector<HVACComponent> outletComps,
                                                      openstudio::IddObjectType type
                                                      ) const;

    virtual std::vector<ModelObject> demandComponents(std::vector<HVACComponent> inletComps,
                                                      std::vector<HVACComponent> outletComps,
                                                      openstudio::IddObjectType type
                                                      );

    virtual std::vector<openstudio::IdfObject> remove();

    virtual ModelObject clone(Model model) const;

    virtual Splitter demandSplitter() = 0;

    virtual Mixer demandMixer() = 0;

  private:

    REGISTER_LOGGER("openstudio.model.Loop");

    // TODO: Make these const.
    boost::optional<ModelObject> supplyInletNodeAsModelObject();
    boost::optional<ModelObject> supplyOutletNodeAsModelObject();
    boost::optional<ModelObject> demandInletNodeAsModelObject();
    boost::optional<ModelObject> demandOutletNodeAsModelObject();

  };

} // detail

} // model

} // openstudio

#endif

