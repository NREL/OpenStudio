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

#ifndef OPENSTUDIO_LOADSVIEW_HPP
#define OPENSTUDIO_LOADSVIEW_HPP


#include "ModelSubTabView.hpp"
#include "ModelObjectInspectorView.hpp"

#include "../model/Model.hpp"

#include "../utilities/idf/WorkspaceObject_Impl.hpp"

class QStackedWidget;

namespace openstudio {

class LoadsView : public ModelSubTabView
{
  Q_OBJECT

  public:

    LoadsView(bool isIP,
              const openstudio::model::Model& model, 
              QWidget * parent = nullptr);

    virtual ~LoadsView() {}

  private:

    static std::vector<std::pair<IddObjectType, std::string> > modelObjectTypesAndNames();

  public slots:

    void toggleUnits(bool displayIP);
};

class LoadsInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    LoadsInspectorView(bool isIP,
                       const model::Model& model,
                       QWidget * parent = nullptr);

    virtual ~LoadsInspectorView() {}

  protected:

    virtual void onClearSelection() override;

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;

    virtual void onUpdate() override;

    void showPeopleDefinitionsInspector(const openstudio::model::ModelObject& modelObject);

    void showInternalMassDefinitionsInspector(const openstudio::model::ModelObject& modelObject);

    void showLightsDefinitionsInspector(const openstudio::model::ModelObject& modelObject);

    void showLuminaireDefinitionsInspector(const openstudio::model::ModelObject& modelObject);

    void showElectricEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject);

    void showGasEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject);

    void showSteamEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject);

    void showOtherEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject);

    void showWaterUseEquipmentDefinitionsInspector(const openstudio::model::ModelObject& modelObject);

    void showInspector(QWidget * widget);

    void showDefaultView();

  private:
    
    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP) override;

};


} // openstudio

#endif // OPENSTUDIO_LOADSVIEW_HPP
