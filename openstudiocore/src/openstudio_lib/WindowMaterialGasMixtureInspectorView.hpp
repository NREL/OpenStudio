/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef OPENSTUDIO_WINDOWMATERIALGASMIXTUREINSPECTORVIEW_HPP
#define OPENSTUDIO_WINDOWMATERIALGASMIXTUREINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "../model/GasMixture.hpp"

namespace openstudio {

class OSComboBox2;

class OSIntegerEdit2;

class OSLineEdit2;

class OSQuantityEdit2;

class StandardsInformationMaterialWidget;

class WindowMaterialGasMixtureInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    WindowMaterialGasMixtureInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = nullptr);

    virtual ~WindowMaterialGasMixtureInspectorView() {}

  protected:

    virtual void onClearSelection() override;

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;

    virtual void onUpdate() override;

  private:

    void createLayout();

    void attach(openstudio::model::GasMixture & GasMixture);

    void detach();

    void refresh();

    bool m_isIP;

    boost::optional<model::GasMixture> m_gasMixture;

    OSLineEdit2 * m_nameEdit = nullptr;

    OSComboBox2 * m_gas1Type = nullptr;
    OSComboBox2 * m_gas2Type = nullptr;
    OSComboBox2 * m_gas3Type = nullptr;
    OSComboBox2 * m_gas4Type = nullptr;

    OSLineEdit2 * m_2AEdit = nullptr;

    OSLineEdit2 * m_3AEdit = nullptr;

    OSLineEdit2 * m_4AEdit = nullptr;

    OSQuantityEdit2 * m_thickness = nullptr;

    OSIntegerEdit2 * m_numberOfGasesInMixture = nullptr;

    OSQuantityEdit2 * m_gas1Fraction = nullptr;

    OSQuantityEdit2 * m_gas2Fraction = nullptr;

    OSQuantityEdit2 * m_gas3Fraction = nullptr;

    OSQuantityEdit2 * m_gas4Fraction = nullptr;

    StandardsInformationMaterialWidget * m_standardsInformationWidget = nullptr;

};

} // openstudio

#endif // OPENSTUDIO_WINDOWMATERIALGASMIXTUREINSPECTORVIEW_HPP

