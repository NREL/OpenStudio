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

#include "MaterialsView.hpp"

#include "MaterialAirGapInspectorView.hpp"
#include "MaterialAirWallInspectorView.hpp"
#include "MaterialInfraredTransparentInspectorView.hpp"
#include "MaterialInspectorView.hpp"
#include "MaterialNoMassInspectorView.hpp"
#include "MaterialRoofVegetationInspectorView.hpp"
#include "ModelObjectTypeListView.hpp"
#include "WindowMaterialBlindInspectorView.hpp"
#include "WindowMaterialDaylightRedirectionDeviceInspectorView.hpp"
#include "WindowMaterialGasInspectorView.hpp"
#include "WindowMaterialGasMixtureInspectorView.hpp"
#include "WindowMaterialGlazingGroupThermochromicInspectorView.hpp"
#include "WindowMaterialGlazingInspectorView.hpp"
#include "WindowMaterialGlazingRefractionExtinctionMethodInspectorView.hpp"
#include "WindowMaterialScreenInspectorView.hpp"
#include "WindowMaterialShadeInspectorView.hpp"
#include "WindowMaterialSimpleGlazingSystemInspectorView.hpp"

#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <QStackedWidget>

namespace openstudio {


MaterialsView::MaterialsView(bool isIP,
                             const openstudio::model::Model& model,
                             const QString& tabLabel,
                             bool hasSubTabs,
                             QWidget * parent)
: ModelSubTabView(new ModelObjectTypeListView(MaterialsView::modelObjectTypesAndNames(), model, true, OSItemType::CollapsibleListHeader, parent),
  new MaterialsInspectorView(isIP, model, parent),
  false,
  parent)
{
  // ModelObjectTypeListView will call reportItems for each IddObjectType, this results in inspector being build for each IddObjecType then thrown away
  connect(this, &MaterialsView::toggleUnitsClicked, modelObjectInspectorView(), &ModelObjectInspectorView::toggleUnitsClicked);
}

std::vector<std::pair<IddObjectType, std::string> > MaterialsView::modelObjectTypesAndNames()
{
  std::vector<std::pair<IddObjectType, std::string> > result;
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_Material, "Materials"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_Material_NoMass, "No Mass Materials"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_Material_AirGap, "Air Gap Materials"));

  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_WindowMaterial_SimpleGlazingSystem, "Simple Glazing System Window Materials"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_WindowMaterial_Glazing, "Glazing Window Materials"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_WindowMaterial_Gas, "Gas Window Materials"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_WindowMaterial_GasMixture, "Gas Mixture Window Materials"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_WindowMaterial_Blind, "Blind Window Materials"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_WindowMaterial_DaylightRedirectionDevice, "Daylight Redirection Device Window Materials"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_WindowMaterial_Screen, "Screen Window Materials"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_WindowMaterial_Shade, "Shade Window Materials"));

  // Oddballs to be listed at the bottom of the list
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_Material_AirWall, "Air Wall Materials"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_Material_InfraredTransparent, "Infrared Transparent Materials"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_Material_RoofVegetation, "Roof Vegetation Materials"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod, "Refraction Extinction Method Glazing Window Materials"));
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic, "Glazing Group Thermochromic Window Materials"));
  
  return result;
}

MaterialsInspectorView::MaterialsInspectorView(bool isIP,
                                               const model::Model& model,
                                               QWidget * parent )
  : ModelObjectInspectorView(model, false, parent),
    m_isIP(isIP)
{
  //// Hack code to remove when tab active
  //QLabel * underConstructionLabel = new QLabel();
  //underConstructionLabel->setPixmap(QPixmap(":/images/coming_soon_building_summary.png"));
  //underConstructionLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  //int index = this->stackedWidget()->addWidget(underConstructionLabel);
  //OS_ASSERT(index == 0);

  //// index of hidden widget is 0
  ////QWidget* hiddenWidget = new QWidget();
  ////int index = this->stackedWidget()->addWidget(hiddenWidget);
  ////OS_ASSERT(index == 0);

  //// index of the default is 1
  //DefaultInspectorView* defaultInspectorView = new DefaultInspectorView(model, parent);
  //index = this->stackedWidget()->addWidget(defaultInspectorView);
  //OS_ASSERT(index == 1);

  ////StandardOpaqueMaterialInspectorView* standardOpaqueMaterialInspectorView = new StandardOpaqueMaterialInspectorView(model, parent);
  ////index = this->stackedWidget()->addWidget(standardOpaqueMaterialInspectorView);
  ////m_inspectorIndexMap[IddObjectType::OS_Material] = index;
}

void MaterialsInspectorView::onClearSelection()
{
  QWidget* widget = this->stackedWidget()->currentWidget();
  ModelObjectInspectorView* modelObjectInspectorView = qobject_cast<ModelObjectInspectorView*>(widget);
  if(modelObjectInspectorView){
    modelObjectInspectorView->clearSelection();
  }

  this->stackedWidget()->setCurrentIndex(0);
}

void MaterialsInspectorView::onUpdate()
{
}

void MaterialsInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  switch( modelObject.iddObjectType().value() )
  {
  case IddObjectType::OS_Material:
      this->showMaterialInspectorView(modelObject);
      break;
    case IddObjectType::OS_Material_AirGap:
      this->showMaterialAirGapInspectorView(modelObject);
      break;
    case IddObjectType::OS_Material_AirWall:
      this->showMaterialAirWallInspectorView(modelObject);
      break;
    case IddObjectType::OS_Material_InfraredTransparent:
      this->showMaterialInfraredTransparentInspectorView(modelObject);
      break;
    case IddObjectType::OS_Material_NoMass:
      this->showMaterialNoMassInspectorView(modelObject);
      break;
    case IddObjectType::OS_Material_RoofVegetation:
      this->showMaterialRoofVegetationInspectorView(modelObject);
      break;
    case IddObjectType::OS_WindowMaterial_Blind:
      this->showWindowMaterialBlindInspectorView(modelObject);
      break;
    case IddObjectType::OS_WindowMaterial_DaylightRedirectionDevice:
      this->showWindowMaterialDaylightRedirectionDeviceInspectorView(modelObject);
      break;
    case IddObjectType::OS_WindowMaterial_Gas:
      this->showWindowMaterialGasInspectorView(modelObject);
      break;
    case IddObjectType::OS_WindowMaterial_GasMixture:
      this->showWindowMaterialGasMixtureInspectorView(modelObject);
      break;
    case IddObjectType::OS_WindowMaterial_Glazing:
      this->showWindowMaterialGlazingInspectorView(modelObject);
      break;
    case IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod:
      this->showWindowMaterialGlazingRefractionExtinctionMethodInspectorView(modelObject);
      break;
    case IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic:
      this->showWindowMaterialGlazingGroupThermochromicInspectorView(modelObject);
      break;
    case IddObjectType::OS_WindowMaterial_Screen:
      this->showWindowMaterialScreenInspectorView(modelObject);
      break;
    case IddObjectType::OS_WindowMaterial_Shade:
      this->showWindowMaterialShadeInspectorView(modelObject);
      break;
    case IddObjectType::OS_WindowMaterial_SimpleGlazingSystem:
      this->showWindowMaterialSimpleGlazingSystemInspectorView(modelObject);
      break;      
    default:
      showDefaultView();      
  }
}

void MaterialsInspectorView::showMaterialAirGapInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new MaterialAirGapInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &MaterialAirGapInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showMaterialAirWallInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new MaterialAirWallInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &MaterialAirWallInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showMaterialInfraredTransparentInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new MaterialInfraredTransparentInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &MaterialInfraredTransparentInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showMaterialInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new MaterialInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &MaterialInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showMaterialNoMassInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new MaterialNoMassInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &MaterialNoMassInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showMaterialRoofVegetationInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new MaterialRoofVegetationInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &MaterialRoofVegetationInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialBlindInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new WindowMaterialBlindInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &WindowMaterialBlindInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialDaylightRedirectionDeviceInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new WindowMaterialDaylightRedirectionDeviceInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &WindowMaterialDaylightRedirectionDeviceInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialGasInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new WindowMaterialGasInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &WindowMaterialGasInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialGasMixtureInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new WindowMaterialGasMixtureInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &WindowMaterialGasMixtureInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialGlazingGroupThermochromicInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new WindowMaterialGlazingGroupThermochromicInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &WindowMaterialGlazingGroupThermochromicInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialGlazingInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new WindowMaterialGlazingInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &WindowMaterialGlazingInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialGlazingRefractionExtinctionMethodInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new WindowMaterialGlazingRefractionExtinctionMethodInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialScreenInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new WindowMaterialScreenInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &WindowMaterialScreenInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialShadeInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new WindowMaterialShadeInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &WindowMaterialShadeInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialSimpleGlazingSystemInspectorView(const openstudio::model::ModelObject & modelObject)
{
  auto view = new WindowMaterialSimpleGlazingSystemInspectorView(m_isIP, m_model);
  connect(this, &MaterialsInspectorView::toggleUnitsClicked, view, &WindowMaterialSimpleGlazingSystemInspectorView::toggleUnitsClicked);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showInspector(QWidget * widget)
{
  if( QWidget * _widget = this->stackedWidget()->currentWidget() )
  {
    this->stackedWidget()->removeWidget(_widget);

    delete _widget;
  }

  this->stackedWidget()->addWidget(widget);
}

void MaterialsInspectorView::showDefaultView()
{
  if( QWidget * widget = this->stackedWidget()->currentWidget() )
  {
    this->stackedWidget()->removeWidget(widget);

    delete widget;
  }
}

/****************** SLOTS ******************/

void MaterialsInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
