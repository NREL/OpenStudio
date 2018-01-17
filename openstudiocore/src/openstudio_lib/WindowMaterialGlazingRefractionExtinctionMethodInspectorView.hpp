/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

