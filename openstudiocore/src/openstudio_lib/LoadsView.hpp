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
              QWidget * parent = 0);

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
                       QWidget * parent = 0);

    virtual ~LoadsInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

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

    void toggleUnits(bool displayIP);

};


} // openstudio

#endif // OPENSTUDIO_LOADSVIEW_HPP
