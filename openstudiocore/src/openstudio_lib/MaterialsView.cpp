/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <openstudio_lib/MaterialsView.hpp>

#include <openstudio_lib/MaterialAirGapInspectorView.hpp>
#include <openstudio_lib/MaterialAirWallInspectorView.hpp>
#include <openstudio_lib/MaterialInfraredTransparentInspectorView.hpp>
#include <openstudio_lib/MaterialInspectorView.hpp>
#include <openstudio_lib/MaterialNoMassInspectorView.hpp>
#include <openstudio_lib/MaterialRoofVegetationInspectorView.hpp>
#include <openstudio_lib/ModelObjectTypeListView.hpp>
#include <openstudio_lib/WindowMaterialBlindInspectorView.hpp>
#include <openstudio_lib/WindowMaterialGasInspectorView.hpp>
#include <openstudio_lib/WindowMaterialGasMixtureInspectorView.hpp>
#include <openstudio_lib/WindowMaterialGlazingGroupThermochromicInspectorView.hpp>
#include <openstudio_lib/WindowMaterialGlazingInspectorView.hpp>
#include <openstudio_lib/WindowMaterialGlazingRefractionExtinctionMethodInspectorView.hpp>
#include <openstudio_lib/WindowMaterialScreenInspectorView.hpp>
#include <openstudio_lib/WindowMaterialShadeInspectorView.hpp>
#include <openstudio_lib/WindowMaterialSimpleGlazingSystemInspectorView.hpp>

#include <model/Model_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <QStackedWidget>

namespace openstudio {


MaterialsView::MaterialsView(bool isIP,
                             const openstudio::model::Model& model,
                             const QString& tabLabel,
                             bool hasSubTabs,
                             QWidget * parent)
: ModelSubTabView(new ModelObjectTypeListView(MaterialsView::modelObjectTypesAndNames(), model, true, OSItem::COLLAPSIBLE_LIST_HEADER, parent),
  new MaterialsInspectorView(isIP, model, parent),
  parent)
{
  bool isConnected = connect(this,SIGNAL(toggleUnitsClicked(bool)),
                             modelObjectInspectorView(),SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);
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
  if( modelObject.iddObjectType() == IddObjectType::OS_Material)
  {
    this->showMaterialInspectorView(modelObject);
  }
  else if( modelObject.iddObjectType() == IddObjectType::OS_Material_AirGap)
  {
    this->showMaterialAirGapInspectorView(modelObject);
  }
  else if( modelObject.iddObjectType() == IddObjectType::OS_Material_AirWall)
  {
    this->showMaterialAirWallInspectorView(modelObject);
  }
  else if( modelObject.iddObjectType() == IddObjectType::OS_Material_InfraredTransparent)
  {
    this->showMaterialInfraredTransparentInspectorView(modelObject);
  }
  else if( modelObject.iddObjectType() == IddObjectType::OS_Material_NoMass)
  {
    this->showMaterialNoMassInspectorView(modelObject);
  }
  else if( modelObject.iddObjectType() == IddObjectType::OS_Material_RoofVegetation)
  {
    this->showMaterialRoofVegetationInspectorView(modelObject);
  }
  else if( modelObject.iddObjectType() == IddObjectType::OS_WindowMaterial_Blind)
  {
    this->showWindowMaterialBlindInspectorView(modelObject);
  }
  else if( modelObject.iddObjectType() == IddObjectType::OS_WindowMaterial_Gas)
  {
    this->showWindowMaterialGasInspectorView(modelObject);
  }
  else if( modelObject.iddObjectType() == IddObjectType::OS_WindowMaterial_GasMixture)
  {
    this->showWindowMaterialGasMixtureInspectorView(modelObject);
  }
  else if( modelObject.iddObjectType() == IddObjectType::OS_WindowMaterial_Glazing)
  {
    this->showWindowMaterialGlazingInspectorView(modelObject);
  }
  else if( modelObject.iddObjectType() == IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod)
  {
    this->showWindowMaterialGlazingRefractionExtinctionMethodInspectorView(modelObject);
  }
  else if( modelObject.iddObjectType() == IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic)
  {
    this->showWindowMaterialGlazingGroupThermochromicInspectorView(modelObject);
  }
  else if( modelObject.iddObjectType() == IddObjectType::OS_WindowMaterial_Screen)
  {
    this->showWindowMaterialScreenInspectorView(modelObject);
  }
  else if( modelObject.iddObjectType() == IddObjectType::OS_WindowMaterial_Shade)
  {
    this->showWindowMaterialShadeInspectorView(modelObject);
  }
  else if( modelObject.iddObjectType() == IddObjectType::OS_WindowMaterial_SimpleGlazingSystem)
  {
    this->showWindowMaterialSimpleGlazingSystemInspectorView(modelObject);
  }
  else
  { 
    showDefaultView();      
  }
}

void MaterialsInspectorView::showMaterialAirGapInspectorView(const openstudio::model::ModelObject & modelObject)
{
  MaterialAirGapInspectorView * view = new MaterialAirGapInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             view, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showMaterialAirWallInspectorView(const openstudio::model::ModelObject & modelObject)
{
  MaterialAirWallInspectorView * view = new MaterialAirWallInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             view, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showMaterialInfraredTransparentInspectorView(const openstudio::model::ModelObject & modelObject)
{
  MaterialInfraredTransparentInspectorView * view = new MaterialInfraredTransparentInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             view, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showMaterialInspectorView(const openstudio::model::ModelObject & modelObject)
{
  MaterialInspectorView * view = new MaterialInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             view, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showMaterialNoMassInspectorView(const openstudio::model::ModelObject & modelObject)
{
  MaterialNoMassInspectorView * view = new MaterialNoMassInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             view, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showMaterialRoofVegetationInspectorView(const openstudio::model::ModelObject & modelObject)
{
  MaterialRoofVegetationInspectorView * view = new MaterialRoofVegetationInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             view, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialBlindInspectorView(const openstudio::model::ModelObject & modelObject)
{
  WindowMaterialBlindInspectorView * view = new WindowMaterialBlindInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             view, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialGasInspectorView(const openstudio::model::ModelObject & modelObject)
{
  WindowMaterialGasInspectorView * view = new WindowMaterialGasInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             view, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialGasMixtureInspectorView(const openstudio::model::ModelObject & modelObject)
{
  WindowMaterialGasMixtureInspectorView * view = new WindowMaterialGasMixtureInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             view, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialGlazingGroupThermochromicInspectorView(const openstudio::model::ModelObject & modelObject)
{
  WindowMaterialGlazingGroupThermochromicInspectorView * view = new WindowMaterialGlazingGroupThermochromicInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             view, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialGlazingInspectorView(const openstudio::model::ModelObject & modelObject)
{
  WindowMaterialGlazingInspectorView * view = new WindowMaterialGlazingInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             view, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialGlazingRefractionExtinctionMethodInspectorView(const openstudio::model::ModelObject & modelObject)
{
  WindowMaterialGlazingRefractionExtinctionMethodInspectorView * view = new WindowMaterialGlazingRefractionExtinctionMethodInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             view, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialScreenInspectorView(const openstudio::model::ModelObject & modelObject)
{
  WindowMaterialScreenInspectorView * view = new WindowMaterialScreenInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             view, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialShadeInspectorView(const openstudio::model::ModelObject & modelObject)
{
  WindowMaterialShadeInspectorView * view = new WindowMaterialShadeInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             view, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  view->selectModelObject(modelObject);

  this->showInspector(view);
}

void MaterialsInspectorView::showWindowMaterialSimpleGlazingSystemInspectorView(const openstudio::model::ModelObject & modelObject)
{
  WindowMaterialSimpleGlazingSystemInspectorView * view = new WindowMaterialSimpleGlazingSystemInspectorView(m_isIP, m_model);
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             view, 
                             SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

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
