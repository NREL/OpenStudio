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

#ifndef MODEL_SIZINGPLANT_IMPL_HPP
#define MODEL_SIZINGPLANT_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {

class PlantLoop;

namespace detail {

  /** SizingPlant_Impl is a ModelObject_Impl that is the implementation class for SizingPlant.*/
  class MODEL_API SizingPlant_Impl : public ModelObject_Impl {
    Q_OBJECT;

    Q_PROPERTY(std::string loopType READ loopType WRITE setLoopType);
    Q_PROPERTY(std::vector<std::string> loopTypeValues READ loopTypeValues);

    Q_PROPERTY(double designLoopExitTemperature READ designLoopExitTemperature WRITE setDesignLoopExitTemperature);
    Q_PROPERTY(openstudio::Quantity designLoopExitTemperature_SI READ designLoopExitTemperature_SI WRITE setDesignLoopExitTemperature);
    Q_PROPERTY(openstudio::Quantity designLoopExitTemperature_IP READ designLoopExitTemperature_IP WRITE setDesignLoopExitTemperature);

    Q_PROPERTY(double loopDesignTemperatureDifference READ loopDesignTemperatureDifference WRITE setLoopDesignTemperatureDifference);
    Q_PROPERTY(openstudio::Quantity loopDesignTemperatureDifference_SI READ loopDesignTemperatureDifference_SI WRITE setLoopDesignTemperatureDifference);
    Q_PROPERTY(openstudio::Quantity loopDesignTemperatureDifference_IP READ loopDesignTemperatureDifference_IP WRITE setLoopDesignTemperatureDifference);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> plantLoop READ plantLoopAsModelObject WRITE setPlantLoopAsModelObject);

    public:

    SizingPlant_Impl(const IdfObject& idfObject,
                     Model_Impl* model,
                     bool keepHandle);

    SizingPlant_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    SizingPlant_Impl(const SizingPlant_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    virtual ~SizingPlant_Impl() {}

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    PlantLoop plantLoop() const;

    std::string loopType() const;

    double designLoopExitTemperature() const;

    Quantity getDesignLoopExitTemperature(bool returnIP=false) const;

    double loopDesignTemperatureDifference() const;

    Quantity getLoopDesignTemperatureDifference(bool returnIP=false) const;

    bool setPlantLoop(const PlantLoop& plantLoop);

    bool setLoopType(std::string loopType);

    void setDesignLoopExitTemperature(double designLoopExitTemperature);

    bool setDesignLoopExitTemperature(const Quantity& designLoopExitTemperature);

    bool setLoopDesignTemperatureDifference(double loopDesignTemperatureDifference);

    bool setLoopDesignTemperatureDifference(const Quantity& loopDesignTemperatureDifference);

    protected:

    private:

    REGISTER_LOGGER("openstudio.model.SizingPlant");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<PlantLoop> optionalPlantLoop() const;

    std::vector<std::string> loopTypeValues() const;
    openstudio::Quantity designLoopExitTemperature_SI() const;
    openstudio::Quantity designLoopExitTemperature_IP() const;
    openstudio::Quantity loopDesignTemperatureDifference_SI() const;
    openstudio::Quantity loopDesignTemperatureDifference_IP() const;

    boost::optional<ModelObject> plantLoopAsModelObject() const;

    bool setPlantLoopAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SIZINGPLANT_IMPL_HPP

