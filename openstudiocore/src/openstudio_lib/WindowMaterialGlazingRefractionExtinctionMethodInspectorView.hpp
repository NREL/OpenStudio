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

#ifndef OPENSTUDIO_WINDOWMATERIALGLAZINGREFRACTIONEXTINCTIONMETHODINSPECTORVIEW_HPP
#define OPENSTUDIO_WINDOWMATERIALGLAZINGREFRACTIONEXTINCTIONMETHODINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"

namespace openstudio {

namespace model {

  class RefractionExtinctionGlazing;

}

class OSLineEdit;

class OSSwitch;

class OSQuantityEdit;

class StandardsInformationMaterialWidget;

class WindowMaterialGlazingRefractionExtinctionMethodInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    WindowMaterialGlazingRefractionExtinctionMethodInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0);

    virtual ~WindowMaterialGlazingRefractionExtinctionMethodInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void createLayout();

    void attach(openstudio::model::RefractionExtinctionGlazing & RefractionExtinctionGlazing);

    void detach();

    void refresh();

    bool m_isIP;

    OSLineEdit * m_nameEdit = nullptr;

    OSSwitch * m_solarDiffusing = nullptr;

    OSQuantityEdit * m_thickness = nullptr;

    OSQuantityEdit * m_solarIndexOfRefraction = nullptr;

    OSQuantityEdit * m_solarExtinctionCoefficient = nullptr;

    OSQuantityEdit * m_visibleIndexOfRefraction = nullptr;

    OSQuantityEdit * m_visibleExtinctionCoefficient = nullptr;

    OSQuantityEdit * m_infraredTransmittanceAtNormalIncidence = nullptr;

    OSQuantityEdit * m_infraredHemisphericalEmissivity = nullptr;

    OSQuantityEdit * m_conductivity = nullptr;

    OSQuantityEdit * m_dirtCorrectionFactorForSolarAndVisibleTransmittance = nullptr;

    StandardsInformationMaterialWidget * m_standardsInformationWidget = nullptr;

};

} // openstudio

#endif // OPENSTUDIO_WINDOWMATERIALGLAZINGREFRACTIONEXTINCTIONMETHODINSPECTORVIEW_HPP

