/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_WINDOWMATERIALSHADEINSPECTORVIEW_HPP
#define OPENSTUDIO_WINDOWMATERIALSHADEINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"

namespace openstudio {

namespace model {

  class Shade;

}

class OSLineEdit;

class OSQuantityEdit;

class StandardsInformationMaterialWidget;

class WindowMaterialShadeInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    WindowMaterialShadeInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0);

    virtual ~WindowMaterialShadeInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void createLayout();

    void attach(openstudio::model::Shade & Shade);

    void detach();

    void refresh();

    bool m_isIP;

    OSLineEdit * m_nameEdit = nullptr;

    OSQuantityEdit * m_solarTransmittance = nullptr;

    OSQuantityEdit * m_solarReflectance = nullptr;

    OSQuantityEdit * m_visibleTransmittance = nullptr;

    OSQuantityEdit * m_visibleReflectance = nullptr;

    OSQuantityEdit * m_thermalHemisphericalEmissivity = nullptr;

    OSQuantityEdit * m_thermalTransmittance = nullptr;

    OSQuantityEdit * m_thickness = nullptr;

    OSQuantityEdit * m_conductivity = nullptr;

    OSQuantityEdit * m_shadeToGlassDistance = nullptr;

    OSQuantityEdit * m_topOpeningMultiplier = nullptr;

    OSQuantityEdit * m_bottomOpeningMultiplier = nullptr;

    OSQuantityEdit * m_leftSideOpeningMultiplier = nullptr;

    OSQuantityEdit * m_rightSideOpeningMultiplier = nullptr;

    OSQuantityEdit * m_airflowPermeability = nullptr;

    StandardsInformationMaterialWidget * m_standardsInformationWidget = nullptr;

};

} // openstudio

#endif // OPENSTUDIO_WINDOWMATERIALSHADEINSPECTORVIEW_HPP

