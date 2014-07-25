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
                  QWidget * parent = 0);

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
                           QWidget * parent = 0);

    virtual ~MaterialsInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void showMaterialAirGapInspectorView(const openstudio::model::ModelObject & modelObject);
    void showMaterialAirWallInspectorView(const openstudio::model::ModelObject & modelObject);
    void showMaterialInfraredTransparentInspectorView(const openstudio::model::ModelObject & modelObject);
    void showMaterialInspectorView(const openstudio::model::ModelObject & modelObject);
    void showMaterialNoMassInspectorView(const openstudio::model::ModelObject & modelObject);
    void showMaterialRoofVegetationInspectorView(const openstudio::model::ModelObject & modelObject);
    void showWindowMaterialBlindInspectorView(const openstudio::model::ModelObject & modelObject);
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

    void toggleUnits(bool displayIP);

};

} // openstudio

#endif // OPENSTUDIO_MATERIALSVIEW_HPP
