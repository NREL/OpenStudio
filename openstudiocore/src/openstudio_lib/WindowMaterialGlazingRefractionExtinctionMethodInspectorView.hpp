/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "../model/RefractionExtinctionGlazing.hpp"

namespace openstudio {

class OSLineEdit2;

class OSSwitch2;

class OSQuantityEdit2;

class StandardsInformationMaterialWidget;

class WindowMaterialGlazingRefractionExtinctionMethodInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    WindowMaterialGlazingRefractionExtinctionMethodInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = nullptr);

    virtual ~WindowMaterialGlazingRefractionExtinctionMethodInspectorView() {}

  protected:

    virtual void onClearSelection() override;

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;

    virtual void onUpdate() override;

  private:

    void createLayout();

    void attach(openstudio::model::RefractionExtinctionGlazing & RefractionExtinctionGlazing);

    void detach();

    void refresh();

    bool m_isIP;

    boost::optional<model::RefractionExtinctionGlazing> m_refractionExtinctionGlazing;

    OSLineEdit2 * m_nameEdit = nullptr;

    OSSwitch2 * m_solarDiffusing = nullptr;

    OSQuantityEdit2 * m_thickness = nullptr;

    OSQuantityEdit2 * m_solarIndexOfRefraction = nullptr;

    OSQuantityEdit2 * m_solarExtinctionCoefficient = nullptr;

    OSQuantityEdit2 * m_visibleIndexOfRefraction = nullptr;

    OSQuantityEdit2 * m_visibleExtinctionCoefficient = nullptr;

    OSQuantityEdit2 * m_infraredTransmittanceAtNormalIncidence = nullptr;

    OSQuantityEdit2 * m_infraredHemisphericalEmissivity = nullptr;

    OSQuantityEdit2 * m_conductivity = nullptr;

    OSQuantityEdit2 * m_dirtCorrectionFactorForSolarAndVisibleTransmittance = nullptr;

    StandardsInformationMaterialWidget * m_standardsInformationWidget = nullptr;

};

} // openstudio

#endif // OPENSTUDIO_WINDOWMATERIALGLAZINGREFRACTIONEXTINCTIONMETHODINSPECTORVIEW_HPP

