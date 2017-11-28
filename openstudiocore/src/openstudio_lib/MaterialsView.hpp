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

#ifndef OPENSTUDIO_MATERIALSVIEW_HPP
#define OPENSTUDIO_MATERIALSVIEW_HPP

#include "ModelSubTabView.hpp"
#include "ModelObjectInspectorView.hpp"

#include "../model/Model.hpp"

class QStackedWidget;

namespace openstudio {

class MaterialsView : public ModelSubTabView
{
  Q_OBJECT

  public:

    MaterialsView(bool isIP,
                  const openstudio::model::Model& model,
                  const QString& tabLabel,
                  bool hasSubTabs,
                  QWidget * parent = nullptr);

    virtual ~MaterialsView() {}

  private:

    static std::vector<std::pair<IddObjectType, std::string> > modelObjectTypesAndNames();

};

class MaterialsInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    MaterialsInspectorView(bool isIP,
                           const model::Model& model,
                           QWidget * parent = nullptr);

    virtual ~MaterialsInspectorView() {}

  protected:

    virtual void onClearSelection() override;

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;

    virtual void onUpdate() override;

  private:

    void showMaterialAirGapInspectorView(const openstudio::model::ModelObject & modelObject);
    void showMaterialAirWallInspectorView(const openstudio::model::ModelObject & modelObject);
    void showMaterialInfraredTransparentInspectorView(const openstudio::model::ModelObject & modelObject);
    void showMaterialInspectorView(const openstudio::model::ModelObject & modelObject);
    void showMaterialNoMassInspectorView(const openstudio::model::ModelObject & modelObject);
    void showMaterialRoofVegetationInspectorView(const openstudio::model::ModelObject & modelObject);
    void showWindowMaterialBlindInspectorView(const openstudio::model::ModelObject & modelObject);
    void showWindowMaterialDaylightRedirectionDeviceInspectorView(const openstudio::model::ModelObject & modelObject);
    void showWindowMaterialGasInspectorView(const openstudio::model::ModelObject & modelObject);
    void showWindowMaterialGasMixtureInspectorView(const openstudio::model::ModelObject & modelObject);
    void showWindowMaterialGlazingGroupThermochromicInspectorView(const openstudio::model::ModelObject & modelObject);
    void showWindowMaterialGlazingInspectorView(const openstudio::model::ModelObject & modelObject);
    void showWindowMaterialGlazingRefractionExtinctionMethodInspectorView(const openstudio::model::ModelObject & modelObject);
    void showWindowMaterialScreenInspectorView(const openstudio::model::ModelObject & modelObject);
    void showWindowMaterialShadeInspectorView(const openstudio::model::ModelObject & modelObject);
    void showWindowMaterialSimpleGlazingSystemInspectorView(const openstudio::model::ModelObject & modelObject);

    void showInspector(QWidget * widget);

    void showDefaultView();

    std::map<openstudio::IddObjectType, int> m_inspectorIndexMap;

    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP) override;

};

} // openstudio

#endif // OPENSTUDIO_MATERIALSVIEW_HPP
