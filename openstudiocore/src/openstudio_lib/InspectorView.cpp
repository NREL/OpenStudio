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

#include "OSDocument.hpp"
#include "OSAppBase.hpp"
#include "GridItem.hpp"
#include "MainRightColumnController.hpp"
#include "InspectorView.hpp"
#include "ZoneChooserView.hpp"
#include "LoopChooserView.hpp"
#include "LibraryTabWidget.hpp"
#include "../model_editor/InspectorGadget.hpp"
#include "../model/AirLoopHVACZoneSplitter.hpp"
#include "../model/AirLoopHVACZoneSplitter_Impl.hpp"
#include "../model/AirLoopHVACZoneMixer.hpp"
#include "../model/AirLoopHVACZoneMixer_Impl.hpp"
#include "../model/WaterToAirComponent.hpp"
#include "../model/WaterToAirComponent_Impl.hpp"
#include "../model/StraightComponent.hpp"
#include "../model/StraightComponent_Impl.hpp"
#include "../model/CoilCoolingCooledBeam.hpp"
#include "../model/CoilCoolingCooledBeam_Impl.hpp"
#include "../model/CoilCoolingWater.hpp"
#include "../model/CoilCoolingWater_Impl.hpp"
#include "../model/CoilCoolingLowTempRadiantConstFlow.hpp"
#include "../model/CoilCoolingLowTempRadiantConstFlow_Impl.hpp"
#include "../model/CoilCoolingLowTempRadiantVarFlow.hpp"
#include "../model/CoilCoolingLowTempRadiantVarFlow_Impl.hpp"
#include "../model/CoilHeatingWater.hpp"
#include "../model/CoilHeatingWater_Impl.hpp"
#include "../model/CoilHeatingWaterBaseboard.hpp"
#include "../model/CoilHeatingWaterBaseboard_Impl.hpp"
#include "../model/CoilHeatingLowTempRadiantConstFlow.hpp"
#include "../model/CoilHeatingLowTempRadiantConstFlow_Impl.hpp"
#include "../model/CoilHeatingLowTempRadiantVarFlow.hpp"
#include "../model/CoilHeatingLowTempRadiantVarFlow_Impl.hpp"
#include "../model/ControllerWaterCoil.hpp"
#include "../model/ControllerWaterCoil_Impl.hpp"
#include "../model/ConnectorSplitter.hpp"
#include "../model/ConnectorSplitter_Impl.hpp"
#include "../model/ConnectorMixer.hpp"
#include "../model/ConnectorMixer_Impl.hpp"
#include "../model/HVACComponent.hpp"
#include "../model/HVACComponent_Impl.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeCooledBeam.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeCooledBeam_Impl.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeReheat.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeReheat_Impl.hpp"
#include "../model/AirTerminalSingleDuctVAVReheat.hpp"
#include "../model/AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "../model/ZoneHVACBaseboardConvectiveWater.hpp"
#include "../model/ZoneHVACBaseboardConvectiveWater_Impl.hpp"
#include "../model/ZoneHVACFourPipeFanCoil.hpp"
#include "../model/ZoneHVACFourPipeFanCoil_Impl.hpp"
#include "../model/ZoneHVACLowTempRadiantConstFlow.hpp"
#include "../model/ZoneHVACLowTempRadiantConstFlow_Impl.hpp"
#include "../model/ZoneHVACLowTempRadiantVarFlow.hpp"
#include "../model/ZoneHVACLowTempRadiantVarFlow_Impl.hpp"
#include "../model/ZoneHVACWaterToAirHeatPump.hpp"
#include "../model/ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "../model/ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "../model/ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "../model/ZoneHVACUnitHeater.hpp"
#include "../model/ZoneHVACUnitHeater_Impl.hpp"
#include "../model/AirLoopHVACSupplyPlenum.hpp"
#include "../model/AirLoopHVACSupplyPlenum_Impl.hpp"
#include "../model/AirLoopHVACReturnPlenum.hpp"
#include "../model/AirLoopHVACReturnPlenum_Impl.hpp"
#include "../model/RefrigerationWalkIn.hpp"
#include "../model/RefrigerationWalkIn_Impl.hpp"
#include "../model/RefrigerationWalkInZoneBoundary.hpp"
#include "../model/RefrigerationWalkInZoneBoundary_Impl.hpp"
#include "../model/AirLoopHVACUnitarySystem.hpp"
#include "../model/AirLoopHVACUnitarySystem_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QApplication>
#include <QTimer>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QToolButton>
#include <QCheckBox>
#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QBitmap>

namespace openstudio {

InspectorView::InspectorView(QWidget* parent)
  : QWidget(parent),
    m_currentView(NULL)
{
  m_vLayout = new QVBoxLayout();

  m_vLayout->setContentsMargins(0,0,0,0);

  m_vLayout->setSpacing(0);

  setLayout(m_vLayout);
}

void InspectorView::update()
{
  if( m_currentView )
  {
    m_currentView->update();
  }
}

void InspectorView::layoutModelObject(openstudio::model::OptionalModelObject & modelObject, bool readOnly, bool displayIP)
{
  bool isConnected = false;

  if( QWidget * _parentWidget = parentWidget() )
  {
    _parentWidget->setUpdatesEnabled(false);
  }

  if( modelObject )
  {
    if( model::OptionalAirLoopHVACZoneSplitter splitter = modelObject->optionalCast<model::AirLoopHVACZoneSplitter>() )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new SplitterMixerInspectorView();
      
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(splitter.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect( m_currentView, SIGNAL(addZoneClicked(model::ThermalZone &)), this,  SIGNAL(addZoneClicked(model::ThermalZone &)) );

      connect( m_currentView, SIGNAL(removeZoneClicked(model::ThermalZone &)), this,  SIGNAL(removeZoneClicked(model::ThermalZone &)) );
    }
    else if( model::OptionalThermalZone zone = modelObject->optionalCast<model::ThermalZone>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ThermalZoneInspectorView();
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);

      isConnected = connect(m_currentView,SIGNAL(moveBranchForZoneSupplySelected(model::ThermalZone &,const Handle &)),
                      this,SIGNAL(moveBranchForZoneSupplySelected(model::ThermalZone &,const Handle &)));
      OS_ASSERT(isConnected);

      isConnected = connect(m_currentView,SIGNAL(moveBranchForZoneReturnSelected(model::ThermalZone &,const Handle &)),
                      this,SIGNAL(moveBranchForZoneReturnSelected(model::ThermalZone &,const Handle &)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(zone.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);
    }
    else if( model::OptionalAirLoopHVACZoneMixer mixer = modelObject->optionalCast<model::AirLoopHVACZoneMixer>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new SplitterMixerInspectorView();
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(mixer.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect( m_currentView, SIGNAL(addZoneClicked(model::ThermalZone &)), this,  SIGNAL(addZoneClicked(model::ThermalZone &)) );

      connect( m_currentView, SIGNAL(removeZoneClicked(model::ThermalZone &)), this,  SIGNAL(removeZoneClicked(model::ThermalZone &)) );
    }
    else if( boost::optional<model::AirLoopHVACUnitarySystem> component = 
             modelObject->optionalCast<model::AirLoopHVACUnitarySystem>()  )
    
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new AirLoopHVACUnitarySystemInspectorView();
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect( m_currentView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
      
      connect( m_currentView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
    }
    else if( boost::optional<model::WaterToAirComponent> component = modelObject->optionalCast<model::WaterToAirComponent>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new WaterToAirInspectorView();
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect( m_currentView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
      
      connect( m_currentView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
    }
    else if( boost::optional<model::AirTerminalSingleDuctConstantVolumeCooledBeam> component = modelObject->optionalCast<model::AirTerminalSingleDuctConstantVolumeCooledBeam>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView();
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect( m_currentView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
      
      connect( m_currentView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
    }
    else if( boost::optional<model::AirTerminalSingleDuctConstantVolumeReheat> component = modelObject->optionalCast<model::AirTerminalSingleDuctConstantVolumeReheat>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new AirTerminalSingleDuctConstantVolumeReheatInspectorView();
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect( m_currentView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
      
      connect( m_currentView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
    }
    else if( boost::optional<model::AirTerminalSingleDuctVAVReheat> component = modelObject->optionalCast<model::AirTerminalSingleDuctVAVReheat>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new AirTerminalSingleDuctVAVReheatInspectorView();
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect( m_currentView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
      
      connect( m_currentView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
    }
    else if( boost::optional<model::ZoneHVACBaseboardConvectiveWater> component = 
             modelObject->optionalCast<model::ZoneHVACBaseboardConvectiveWater>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACBaseboardConvectiveWaterInspectorView();
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect( m_currentView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)),
               this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );

      connect( m_currentView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
    }
    else if( boost::optional<model::ZoneHVACFourPipeFanCoil> component = 
             modelObject->optionalCast<model::ZoneHVACFourPipeFanCoil>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACFourPipeFanCoilInspectorView();
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect( m_currentView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)),
               this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
      
      connect( m_currentView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
    }
    else if( boost::optional<model::ZoneHVACLowTempRadiantConstFlow> component = 
             modelObject->optionalCast<model::ZoneHVACLowTempRadiantConstFlow>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACLowTempRadiantConstFlowInspectorView();
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect( m_currentView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)),
               this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
      
      connect( m_currentView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
    }
    
    else if( boost::optional<model::ZoneHVACLowTempRadiantVarFlow> component = 
             modelObject->optionalCast<model::ZoneHVACLowTempRadiantVarFlow>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACLowTempRadiantVarFlowInspectorView();
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect( m_currentView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)),
               this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
      
      connect( m_currentView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
    }
    else if( boost::optional<model::ZoneHVACWaterToAirHeatPump> component = 
             modelObject->optionalCast<model::ZoneHVACWaterToAirHeatPump>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACWaterToAirHeatPumpInspectorView();
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect( m_currentView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)),
               this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
      
      connect( m_currentView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
    }
    else if( boost::optional<model::ZoneHVACPackagedTerminalAirConditioner> component = 
             modelObject->optionalCast<model::ZoneHVACPackagedTerminalAirConditioner>()  )
    
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACPackagedTerminalAirConditionerInspectorView();
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect( m_currentView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
      
      connect( m_currentView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
    }
    else if( boost::optional<model::ZoneHVACUnitHeater> component = 
               modelObject->optionalCast<model::ZoneHVACUnitHeater>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACUnitHeaterInspectorView();
      
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect( m_currentView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)),
               this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
      
      connect( m_currentView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
               this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
    }
    else if( modelObject->optionalCast<model::ConnectorSplitter>() || 
             modelObject->optionalCast<model::ConnectorMixer>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new GenericInspectorView();
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);

      m_vLayout->addWidget(m_currentView);
    }
    else if( boost::optional<model::RefrigerationWalkIn> component = 
               modelObject->optionalCast<model::RefrigerationWalkIn>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new RefrigerationWalkinInspectorView();
      
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);
  
      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);
    }
    else
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new GenericInspectorView();
      isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                            m_currentView, SIGNAL(toggleUnitsClicked(bool)));
      OS_ASSERT(isConnected);

      m_currentView->layoutModelObject(modelObject.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);
    }
  }
  else
  {
    if( m_currentView )
    {
      delete m_currentView;
    }

    m_currentView = new GenericInspectorView();
    isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                          m_currentView, SIGNAL(toggleUnitsClicked(bool)));
    OS_ASSERT(isConnected);

    m_vLayout->addWidget(m_currentView);
  }

  if( m_currentView )
  {
    m_currentView->show();
  }

  if( QWidget * _parentWidget = parentWidget() )
  {
    _parentWidget->setUpdatesEnabled(true);
  }
}

void InspectorView:: toggleUnits(bool displayIP)
{
}

BaseInspectorView::BaseInspectorView(QWidget * parent)
{
  m_vLayout = new QVBoxLayout();

  setLayout(m_vLayout);

  m_vLayout->setContentsMargins(0,0,0,0);

  m_vLayout->setSpacing(0);

  //QWidget * tabBar = new QWidget(this);

  //tabBar->setFixedHeight(29);

  //tabBar->setObjectName("VBlueGradientWidget");

  //tabBar->setContentsMargins(0,0,0,0);

  //m_vLayout->addWidget(tabBar);

  m_libraryTabWidget = new LibraryTabWidget();

  m_vLayout->addWidget(m_libraryTabWidget);

  //m_vLayout->addStretch();
}

void BaseInspectorView::addMainView( QWidget * widget )
{
  m_vLayout->insertWidget(1,widget);
}

void BaseInspectorView:: toggleUnits(bool displayIP)
{
}

SplitterMixerInspectorView::SplitterMixerInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_zoneChooserView = new ZoneChooserView();

  m_libraryTabWidget->addTab( m_zoneChooserView,"","");
                              //":images/components_icon_pressed.png",
                              //":images/components_icon_off.png" );

  connect( m_zoneChooserView, SIGNAL(addZoneClicked(model::ThermalZone &)), this,  SIGNAL(addZoneClicked(model::ThermalZone &)) );
  
  connect( m_zoneChooserView, SIGNAL(removeZoneClicked(model::ThermalZone &)), this,  SIGNAL(removeZoneClicked(model::ThermalZone &)) );
}

void SplitterMixerInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
{
  m_zoneChooserView->layoutModelObject(modelObject);
}

GenericInspectorView::GenericInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             m_inspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_libraryTabWidget->addTab( m_inspectorGadget,"","");
                              //":images/components_icon_pressed.png",
                              //":images/components_icon_off.png" );
}

void GenericInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP )
{
  bool force=false;
  bool recursive=true;
  bool locked=readOnly;
  bool hideChildren=false;
  if( displayIP )
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::IP);
  }
  else
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::SI);
  }
  m_inspectorGadget->layoutModelObj(modelObject, force, recursive, locked, hideChildren);
}

NewPlenumDialog::NewPlenumDialog(QWidget * parent)
  : QDialog(parent)
{
  QVBoxLayout * mainVLayout = new QVBoxLayout();
  mainVLayout->setAlignment(Qt::AlignTop);
  mainVLayout->setContentsMargins(10,10,10,10);
  mainVLayout->setSpacing(10);
  setLayout(mainVLayout);

  setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
  setFixedWidth(400);

  QLabel * message = new QLabel();
  message->setWordWrap(true);
  message->setText("Choose an available zone to use as a plenum.  Only zones that are not conditioned by an air system or zone equipment are displayed."); 
  mainVLayout->addWidget(message);

  zoneChooser = new QComboBox();
  mainVLayout->addWidget(zoneChooser);

  mainVLayout->addSpacing(20);

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();
  model::Model model = doc->model();

  std::vector<model::ThermalZone> allZones = model.getModelObjects<model::ThermalZone>();
  std::sort(allZones.begin(),allZones.end(),WorkspaceObjectNameLess()); 

  for(std::vector<model::ThermalZone>::iterator it = allZones.begin();
      it != allZones.end();
      ++it)
  {
    if( (! it->isPlenum()) && it->equipment().empty() && (! it->airLoopHVAC()) )
    {
      zoneChooser->addItem(QString::fromStdString(it->name().get()),it->handle().toString());
    }
  }

  mainVLayout->addSpacing(10);

  bool bingo;
  QDialogButtonBox * buttonBox = new QDialogButtonBox();
  QPushButton * cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);
  bingo = connect(cancelButton,SIGNAL(clicked()),this,SLOT(onCancelClicked()));
  OS_ASSERT(bingo);
  QPushButton * applyButton = buttonBox->addButton(QDialogButtonBox::Apply);
  bingo = connect(applyButton,SIGNAL(clicked()),this,SLOT(onApplyClicked()));
  OS_ASSERT(bingo);
  mainVLayout->addWidget(buttonBox);
}

void NewPlenumDialog::onCancelClicked()
{
  reject();
}

void NewPlenumDialog::onApplyClicked()
{
  accept();
}

PlenumChooserView::PlenumChooserView(QWidget * parent)
{
  QVBoxLayout * mainVLayout = new QVBoxLayout();
  mainVLayout->setAlignment(Qt::AlignTop);
  setLayout(mainVLayout);
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(0);

  QFrame * supplyFrame = new QFrame();
  supplyFrame->setObjectName("IGRow");
  QVBoxLayout * supplyVLayout = new QVBoxLayout();
  supplyFrame->setLayout(supplyVLayout);
  mainVLayout->addWidget(supplyFrame);

  QLabel * supplyPlenumLabel = new QLabel("Select Supply Plenum");
  supplyVLayout->addWidget(supplyPlenumLabel);
  supplyPlenumChooser = new QComboBox();
  supplyPlenumChooser->setIconSize(QSize(25,25));
  supplyVLayout->addWidget(supplyPlenumChooser);

  newSupplyPlenumButton = new QToolButton();
  newSupplyPlenumButton->setText("New Supply Plenum");
  newSupplyPlenumButton->setIcon(QPixmap(":images/add.png"));
  newSupplyPlenumButton->setIconSize(QSize(35,35));
  newSupplyPlenumButton->setStyleSheet("QToolButton { font-size: 12px; }");
  newSupplyPlenumButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  supplyVLayout->addWidget(newSupplyPlenumButton);

  QFrame * returnFrame = new QFrame();
  returnFrame->setObjectName("IGRow");
  QVBoxLayout * returnVLayout = new QVBoxLayout();
  returnFrame->setLayout(returnVLayout);
  mainVLayout->addWidget(returnFrame);

  QLabel * returnPlenumLabel = new QLabel("Select Return Plenum");
  returnVLayout->addWidget(returnPlenumLabel);
  returnPlenumChooser = new QComboBox();
  returnPlenumChooser->setIconSize(QSize(25,25));
  returnVLayout->addWidget(returnPlenumChooser);

  newReturnPlenumButton = new QToolButton();
  newReturnPlenumButton->setText("New Return Plenum");
  newReturnPlenumButton->setIcon(QPixmap(":images/add.png"));
  newReturnPlenumButton->setIconSize(QSize(35,35));
  newReturnPlenumButton->setStyleSheet("QToolButton { font-size: 12px; }");
  newReturnPlenumButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  returnVLayout->addWidget(newReturnPlenumButton);
}

RefrigerationWalkinInspectorView::RefrigerationWalkinInspectorView(QWidget * parent)
  : BaseInspectorView(parent)
{
  m_walkinInspectorGadget = new InspectorGadget();
  bool bingo = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                       m_walkinInspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(bingo);
  m_libraryTabWidget->addTab( m_walkinInspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_zoneBoundaryWidget = new QWidget();
  m_libraryTabWidget->addTab( m_zoneBoundaryWidget,
                              ":images/walkin_right_tab_on.png",
                              ":images/walkin_right_tab_off.png" );
}

void RefrigerationWalkinInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
{
  m_modelObject = modelObject;

  bool force=false;
  bool recursive=true;
  bool locked=readOnly;
  bool hideChildren=false;

  m_displayIP = displayIP;

  if( displayIP )
  {
    m_walkinInspectorGadget->setUnitSystem(InspectorGadget::IP);
  }
  else
  {
    m_walkinInspectorGadget->setUnitSystem(InspectorGadget::SI);
  }

  m_walkinInspectorGadget->layoutModelObj(modelObject, force, recursive, locked, hideChildren);

  if( QLayout * t_layout = m_zoneBoundaryWidget->layout() )
  {
    delete t_layout;
  }
  QVBoxLayout * zoneBoundaryLayout = new QVBoxLayout();
  zoneBoundaryLayout->setContentsMargins(0,0,0,0);
  zoneBoundaryLayout->setSpacing(0);
  m_zoneBoundaryWidget->setLayout(zoneBoundaryLayout);
  boost::optional<model::RefrigerationWalkIn> walkin = modelObject.optionalCast<model::RefrigerationWalkIn>();
  OS_ASSERT(walkin);
  std::vector<model::RefrigerationWalkInZoneBoundary> zoneBoundaries = walkin->zoneBoundaries();
  for( std::vector<model::RefrigerationWalkInZoneBoundary>::iterator it = zoneBoundaries.begin();
       it != zoneBoundaries.end();
       ++it )
  {
    InspectorGadget * inspector = new InspectorGadget();
    bool bingo = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                    inspector, SIGNAL(toggleUnitsClicked(bool)));
    OS_ASSERT(bingo);
    if( displayIP )
    {
      inspector->setUnitSystem(InspectorGadget::IP);
    }
    else
    {
      inspector->setUnitSystem(InspectorGadget::SI);
    }
    inspector->layoutModelObj(*it, force, recursive, locked, hideChildren);
    m_zoneBoundaryWidget->layout()->addWidget(inspector);
  }
}

ThermalZoneInspectorView::ThermalZoneInspectorView(QWidget * parent)
{
  m_inspectorGadget = new InspectorGadget();
  bool bingo = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                       m_inspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(bingo);
  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_plenumChooser = new PlenumChooserView();
  m_libraryTabWidget->addTab( m_plenumChooser,
                              ":images/plenum_on.png",
                              ":images/plenum_off.png" );

  bingo = connect(m_plenumChooser->supplyPlenumChooser,SIGNAL(currentIndexChanged(int)),this,SLOT(onSupplyPlenumChooserChanged(int)));
  OS_ASSERT(bingo);

  bingo = connect(m_plenumChooser->returnPlenumChooser,SIGNAL(currentIndexChanged(int)),this,SLOT(onReturnPlenumChooserChanged(int)));
  OS_ASSERT(bingo);

  bingo = connect(m_plenumChooser->newSupplyPlenumButton,SIGNAL(clicked()),this,SLOT(onNewSupplyPlenumClicked()));
  OS_ASSERT(bingo);

  bingo = connect(m_plenumChooser->newReturnPlenumButton,SIGNAL(clicked()),this,SLOT(onNewReturnPlenumClicked()));
  OS_ASSERT(bingo);
}

void ThermalZoneInspectorView::onSupplyPlenumChooserChanged(int newIndex)
{
  OS_ASSERT(m_modelObject);
  boost::optional<model::ThermalZone> thermalZone;
  thermalZone = m_modelObject->optionalCast<model::ThermalZone>();
  OS_ASSERT(thermalZone);

  QString newPlenumString = m_plenumChooser->supplyPlenumChooser->itemData(newIndex).toString();
  Handle newPlenumHandle(newPlenumString);

  emit moveBranchForZoneSupplySelected(thermalZone.get(),newPlenumHandle);
}

void ThermalZoneInspectorView::onReturnPlenumChooserChanged(int newIndex)
{
  OS_ASSERT(m_modelObject);
  boost::optional<model::ThermalZone> thermalZone;
  thermalZone = m_modelObject->optionalCast<model::ThermalZone>();
  OS_ASSERT(thermalZone);

  QString newPlenumString = m_plenumChooser->returnPlenumChooser->itemData(newIndex).toString();
  Handle newPlenumHandle(newPlenumString);

  emit moveBranchForZoneReturnSelected(thermalZone.get(),newPlenumHandle);
}

void ThermalZoneInspectorView::onNewSupplyPlenumClicked()
{
  NewPlenumDialog dialog;
  int result = dialog.exec();

  if( result == QDialog::Accepted )
  {
    QComboBox * cb = dialog.zoneChooser;
    Handle newZoneHandle(cb->itemData(cb->currentIndex()).toString());
    if( ! newZoneHandle.isNull() )
    {
      OS_ASSERT(m_modelObject);
      boost::optional<model::ThermalZone> thermalZone;
      thermalZone = m_modelObject->optionalCast<model::ThermalZone>();
      OS_ASSERT(thermalZone);

      emit moveBranchForZoneSupplySelected(thermalZone.get(),newZoneHandle);
    }
  }
}

void ThermalZoneInspectorView::onNewReturnPlenumClicked()
{
  NewPlenumDialog dialog;
  int result = dialog.exec();

  if( result == QDialog::Accepted )
  {
    QComboBox * cb = dialog.zoneChooser;
    Handle newZoneHandle(cb->itemData(cb->currentIndex()).toString());
    if( ! newZoneHandle.isNull() )
    {
      OS_ASSERT(m_modelObject);
      boost::optional<model::ThermalZone> thermalZone;
      thermalZone = m_modelObject->optionalCast<model::ThermalZone>();
      OS_ASSERT(thermalZone);

      emit moveBranchForZoneReturnSelected(thermalZone.get(),newZoneHandle);
    }
  }
}

bool supplyPlenumSort(const model::AirLoopHVACSupplyPlenum & i, const model::AirLoopHVACSupplyPlenum & j)
{
  boost::optional<model::ThermalZone> iZone = i.thermalZone();
  boost::optional<model::ThermalZone> jZone = j.thermalZone();

  if( iZone && jZone )
  {
    return iZone->name().get() < jZone->name().get();
  }
  else
  {
    return i.name().get() < j.name().get();
  }
}

bool returnPlenumSort(const model::AirLoopHVACReturnPlenum & i, const model::AirLoopHVACReturnPlenum & j)
{
  boost::optional<model::ThermalZone> iZone = i.thermalZone();
  boost::optional<model::ThermalZone> jZone = j.thermalZone();

  if( iZone && jZone )
  {
    return iZone->name().get() < jZone->name().get();
  }
  else
  {
    return i.name().get() < j.name().get();
  }
}

void ThermalZoneInspectorView::update()
{
  OS_ASSERT(m_modelObject);
  boost::optional<model::ThermalZone> t_zone = m_modelObject->optionalCast<model::ThermalZone>();
  OS_ASSERT(t_zone);
  boost::optional<model::AirLoopHVAC> t_airLoopHVAC = t_zone->airLoopHVAC();
  OS_ASSERT(t_airLoopHVAC);

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();
  std::shared_ptr<MainRightColumnController> mrc = doc->mainRightColumnController(); 
  SystemItem * systemItem = mrc->systemItem(t_airLoopHVAC->handle());

  QPointF supplyPoints[4] = {
    QPointF(25,25),
    QPointF(75,40),
    QPointF(75,60),
    QPointF(25,75),
  };

  // Populate chooser with supply plenums in system
  QComboBox * supplyChooser = m_plenumChooser->supplyPlenumChooser;
  supplyChooser->blockSignals(true);
  supplyChooser->clear();
  
  std::vector<model::AirLoopHVACSupplyPlenum> supplyPlenums = subsetCastVector<model::AirLoopHVACSupplyPlenum>(t_airLoopHVAC->demandComponents());
  std::sort(supplyPlenums.begin(),supplyPlenums.end(),supplyPlenumSort);
  for( std::vector<model::AirLoopHVACSupplyPlenum>::iterator it = supplyPlenums.begin();
       it != supplyPlenums.end();
       ++it )
  {
    QBitmap bitmap(100,100);
    bitmap.clear();
    QPainter painter2(&bitmap);
    painter2.drawPolygon(supplyPoints,4);

    QPixmap supplyPixmap(100,100);
    supplyPixmap.setMask(bitmap);
    QPainter painter(&supplyPixmap);
    painter.setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
    painter.setBrush(QBrush(systemItem->plenumColor(it->handle()),Qt::SolidPattern));
    painter.drawPolygon(supplyPoints,4);

    boost::optional<model::ThermalZone> t_plenumZone = it->thermalZone();
    if( t_plenumZone )
    {
      supplyChooser->addItem(supplyPixmap,QString::fromStdString(t_plenumZone->name().get()),it->handle().toString());
    }
    else 
    {
      supplyChooser->addItem(supplyPixmap,QString::fromStdString(it->name().get()),it->handle().toString());
    }
  }
  supplyChooser->addItem("Ducted Supply - No Plenum","");

  // Set the current supply plenum
  std::vector<model::ModelObject> thisZoneSupplyPlenums = t_airLoopHVAC->demandComponents(t_airLoopHVAC->zoneSplitter(),t_zone.get(),model::AirLoopHVACSupplyPlenum::iddObjectType());
  if( thisZoneSupplyPlenums.empty() )
  {
    supplyChooser->setCurrentIndex(supplyChooser->findData(""));
  }
  else
  {
    supplyChooser->setCurrentIndex(supplyChooser->findData(thisZoneSupplyPlenums.front().handle().toString()));
  }
  supplyChooser->blockSignals(false);

  // Populate chooser with return plenums in system
  QComboBox * returnChooser = m_plenumChooser->returnPlenumChooser;
  returnChooser->blockSignals(true);
  returnChooser->clear();

  QPointF returnPoints[4] = {
    QPointF(25,40),
    QPointF(75,25),
    QPointF(75,75),
    QPointF(25,60),
  };

  std::vector<model::AirLoopHVACReturnPlenum> returnPlenums = subsetCastVector<model::AirLoopHVACReturnPlenum>(t_airLoopHVAC->demandComponents());
  std::sort(returnPlenums.begin(),returnPlenums.end(),returnPlenumSort);
  for( std::vector<model::AirLoopHVACReturnPlenum>::iterator it = returnPlenums.begin();
       it != returnPlenums.end();
       ++it )
  {
    QBitmap bitmap(100,100);
    bitmap.clear();
    QPainter painter2(&bitmap);
    painter2.drawPolygon(returnPoints,4);

    QPixmap returnPixmap(100,100);
    returnPixmap.setMask(bitmap);
    QPainter painter(&returnPixmap);
    painter.setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
    painter.setBrush(QBrush(systemItem->plenumColor(it->handle()),Qt::SolidPattern));
    painter.drawPolygon(returnPoints,4);

    boost::optional<model::ThermalZone> t_plenumZone = it->thermalZone();
    if( t_plenumZone )
    {
      returnChooser->addItem(returnPixmap,QString::fromStdString(t_plenumZone->name().get()),it->handle().toString());
    }
    else 
    {
      returnChooser->addItem(returnPixmap,QString::fromStdString(it->name().get()),it->handle().toString());
    }
  }
  returnChooser->addItem("Ducted Return - No Plenum","");

  // Set the current return plenum
  std::vector<model::ModelObject> thisZoneReturnPlenums = t_airLoopHVAC->demandComponents(t_zone.get(),t_airLoopHVAC->zoneMixer(),model::AirLoopHVACReturnPlenum::iddObjectType());
  if( thisZoneReturnPlenums.empty() )
  {
    returnChooser->setCurrentIndex(returnChooser->findData(""));
  }
  else
  {
    returnChooser->setCurrentIndex(returnChooser->findData(thisZoneReturnPlenums.front().handle().toString()));
  }
  returnChooser->blockSignals(false);
}

void ThermalZoneInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
{
  bool force=false;
  bool recursive=true;
  bool hideChildren=false;
  m_displayIP = displayIP;
  m_modelObject = modelObject;

  if( displayIP )
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::IP);
  }
  else
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::SI);
  }
  m_inspectorGadget->layoutModelObj(modelObject, force, recursive, readOnly, hideChildren);

  update();
}

WaterToAirInspectorView::WaterToAirInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             m_inspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_coilControllerInspectorGadget = new InspectorGadget();
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        m_coilControllerInspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_loopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_loopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->addTab( m_coilControllerInspectorGadget,
                              ":images/controller_icon_on.png",
                              ":images/controller_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(1);

  connect( m_loopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  
  connect( m_loopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
}

void WaterToAirInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
{
  m_modelObject = modelObject;

  bool force=false;
  bool recursive=true;
  bool locked=readOnly;
  bool hideChildren=false;

  m_displayIP = displayIP;

  if( displayIP )
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::IP);
  }
  else
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::SI);
  }
  m_inspectorGadget->layoutModelObj(modelObject, force, recursive, locked, hideChildren);

  boost::optional<model::ModelObject> mo = modelObject;

  m_loopChooserView->layoutModelObject(mo);

  this->layoutControllerObject();

  QObject * impl = modelObject.getImpl<model::detail::ModelObject_Impl>().get();

  this->connect(impl, SIGNAL(onChange()), SLOT(onWorkspaceObjectChanged()));
}

void WaterToAirInspectorView::onWorkspaceObjectChanged()
{
  QTimer::singleShot(0,this,SLOT(layoutControllerObject()));
}

void WaterToAirInspectorView::layoutControllerObject()
{
  m_coilControllerInspectorGadget->clear(true);

  if( m_modelObject )
  {
    if( boost::optional<model::CoilCoolingWater> coil = m_modelObject->optionalCast<model::CoilCoolingWater>() )
    {
      if( boost::optional<model::ControllerWaterCoil> controller = coil->controllerWaterCoil() )
      {
        if( m_displayIP )
        {
          m_coilControllerInspectorGadget->setUnitSystem(InspectorGadget::IP);
        }
        else
        {
          m_coilControllerInspectorGadget->setUnitSystem(InspectorGadget::SI);
        }

        m_coilControllerInspectorGadget->layoutModelObj(controller.get());
      }
    }

    if( boost::optional<model::CoilHeatingWater> coil = m_modelObject->optionalCast<model::CoilHeatingWater>() )
    {
      if( boost::optional<model::ControllerWaterCoil> controller = coil->controllerWaterCoil() )
      {
        if( m_displayIP )
        {
          m_coilControllerInspectorGadget->setUnitSystem(InspectorGadget::IP);
        }
        else
        {
          m_coilControllerInspectorGadget->setUnitSystem(InspectorGadget::SI);
        }

        m_coilControllerInspectorGadget->layoutModelObj(controller.get());
      }
    }
  }
}

AirTerminalInspectorView::AirTerminalInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             m_inspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_loopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_loopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(1);

  connect( m_loopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  
  connect( m_loopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
}

template <class T>
void AirTerminalInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
{
  m_modelObject = modelObject;

  bool force=false;
  bool recursive=true;
  bool locked=readOnly;
  bool hideChildren=false;
  if( displayIP )
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::IP);
  }
  else
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::SI);
  }
  m_inspectorGadget->layoutModelObj(modelObject, force, recursive, locked, hideChildren);

  bool waterCoil = false;

  if( boost::optional<T> terminal = modelObject.optionalCast<T>() )
  {
    if( boost::optional<model::HVACComponent> coil = terminal->reheatCoil() )
    {
      if( boost::optional<model::WaterToAirComponent> waterToAirCoil = coil->optionalCast<model::WaterToAirComponent>() )
      {
        boost::optional<model::ModelObject> mo = waterToAirCoil.get();

        m_loopChooserView->layoutModelObject(mo);

        waterCoil = true;
      }
    }
  }

  if( ! waterCoil )
  {
    boost::optional<model::ModelObject> mo;

    m_loopChooserView->layoutModelObject(mo);
  }
}

AirTerminalSingleDuctConstantVolumeReheatInspectorView::AirTerminalSingleDuctConstantVolumeReheatInspectorView( QWidget * parent )
  : AirTerminalInspectorView(parent)
{

}

void AirTerminalSingleDuctConstantVolumeReheatInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
{
  AirTerminalInspectorView::layoutModelObject<model::AirTerminalSingleDuctConstantVolumeReheat>(modelObject, readOnly, displayIP);
}

AirTerminalSingleDuctVAVReheatInspectorView::AirTerminalSingleDuctVAVReheatInspectorView( QWidget * parent )
  : AirTerminalInspectorView(parent)
{

}

void AirTerminalSingleDuctVAVReheatInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
{
  AirTerminalInspectorView::layoutModelObject<model::AirTerminalSingleDuctVAVReheat>(modelObject, readOnly, displayIP);
}

AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView::AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             m_inspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_coolingLoopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_coolingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(1);

  connect( m_coolingLoopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  
  connect( m_coolingLoopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
}

void AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
{
  m_modelObject = modelObject;

  bool force=false;
  bool recursive=true;
  bool locked=readOnly;
  bool hideChildren=false;
  if( displayIP )
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::IP);
  }
  else
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::SI);
  }
  m_inspectorGadget->layoutModelObj(modelObject, force, recursive, locked, hideChildren);

  bool coolCoil = false;

  if( boost::optional<model::AirTerminalSingleDuctConstantVolumeCooledBeam> terminal = modelObject.optionalCast<model::AirTerminalSingleDuctConstantVolumeCooledBeam>() )
  {
    if( boost::optional<model::HVACComponent> coil = terminal->coilCoolingCooledBeam() )
    {
        boost::optional<model::ModelObject> mo = coil.get();

        m_coolingLoopChooserView->layoutModelObject(mo);

        coolCoil = true;
     
    }
  }

  if( !coolCoil )
  {
    boost::optional<model::ModelObject> mo;

    m_coolingLoopChooserView->layoutModelObject(mo);
  }
}

ZoneHVACBaseboardConvectiveWaterInspectorView::ZoneHVACBaseboardConvectiveWaterInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             m_inspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_heatingLoopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_heatingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(0);

  connect( m_heatingLoopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  
  connect( m_heatingLoopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
}

void ZoneHVACBaseboardConvectiveWaterInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP )
{
  m_modelObject = modelObject;

  bool force=false;
  bool recursive=true;
  bool locked=readOnly;
  bool hideChildren=false;
  if( displayIP )
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::IP);
  }
  else
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::SI);
  }
  m_inspectorGadget->layoutModelObj(modelObject, force, recursive, locked, hideChildren);

  bool waterHeatingCoil = false;
  
  //check if the object is a zone baseboard
  boost::optional<model::ZoneHVACBaseboardConvectiveWater> baseboardConvtest = 
        modelObject.optionalCast<model::ZoneHVACBaseboardConvectiveWater>();
  
  if(baseboardConvtest){
    //if it is, check if it has a heating coil
    boost::optional<model::ModelObject> coilheatingbb = baseboardConvtest->heatingCoil();
    m_heatingLoopChooserView->layoutModelObject(coilheatingbb);
    waterHeatingCoil = true;
  }
 
  if( ! waterHeatingCoil )
  {
    boost::optional<model::ModelObject> moHeat;

    m_heatingLoopChooserView->layoutModelObject(moHeat);
  }
}

ZoneHVACFourPipeFanCoilInspectorView::ZoneHVACFourPipeFanCoilInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             m_inspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_heatingLoopChooserView = new LoopChooserView();

  m_coolingLoopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_heatingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->addTab( m_coolingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(0);

  connect( m_heatingLoopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  
  connect( m_heatingLoopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );

  connect( m_coolingLoopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  
  connect( m_coolingLoopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
}

void ZoneHVACFourPipeFanCoilInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP )
{
  m_modelObject = modelObject;

  bool force=false;
  bool recursive=true;
  bool locked=readOnly;
  bool hideChildren=false;
  if( displayIP )
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::IP);
  }
  else
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::SI);
  }
  m_inspectorGadget->layoutModelObj(modelObject, force, recursive, locked, hideChildren);

  bool waterHeatingCoil = false;
  bool waterCoolingCoil = false;

  if( boost::optional<model::ZoneHVACFourPipeFanCoil> fourPipe = 
        modelObject.optionalCast<model::ZoneHVACFourPipeFanCoil>() )
  {
    if( boost::optional<model::HVACComponent> coil = fourPipe->heatingCoil() )
    {
      if( boost::optional<model::WaterToAirComponent> waterToAirCoil = coil->optionalCast<model::WaterToAirComponent>() )
      {
        boost::optional<model::ModelObject> moHeat = waterToAirCoil.get();

        m_heatingLoopChooserView->layoutModelObject(moHeat);

        waterHeatingCoil = true;
      }
    }
    if( boost::optional<model::HVACComponent> coil = fourPipe->coolingCoil() )
    {
      if( boost::optional<model::WaterToAirComponent> waterToAirCoil = coil->optionalCast<model::WaterToAirComponent>() )
      {
        boost::optional<model::ModelObject> moCool = waterToAirCoil.get();

        m_coolingLoopChooserView->layoutModelObject(moCool);

        waterCoolingCoil = true;
      }
    }
  }

  if( ! waterHeatingCoil )
  {
    boost::optional<model::ModelObject> moHeat;

    m_heatingLoopChooserView->layoutModelObject(moHeat);
  }
  if( ! waterCoolingCoil )
  {
    boost::optional<model::ModelObject> moCool;

    m_coolingLoopChooserView->layoutModelObject(moCool);
  }
}

ZoneHVACLowTempRadiantConstFlowInspectorView::ZoneHVACLowTempRadiantConstFlowInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             m_inspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_heatingLoopChooserView = new LoopChooserView();
  m_coolingLoopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_heatingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->addTab( m_coolingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(0);

  connect( m_heatingLoopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  
  connect( m_heatingLoopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );

  connect( m_coolingLoopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  
  connect( m_coolingLoopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );


}

void ZoneHVACLowTempRadiantConstFlowInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP )
{
  
  m_modelObject = modelObject;

  bool force=false;
  bool recursive=true;
  bool locked=readOnly;
  bool hideChildren=false;
  if( displayIP )
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::IP);
  }
  else
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::SI);
  }
  m_inspectorGadget->layoutModelObj(modelObject, force, recursive, locked, hideChildren);

  bool waterHeatingCoil = false;
  bool waterCoolingCoil = false;

  if( boost::optional<model::ZoneHVACLowTempRadiantConstFlow> lowTempRadiant = 
        modelObject.optionalCast<model::ZoneHVACLowTempRadiantConstFlow>() )
  {
    if( boost::optional<model::HVACComponent> coil = lowTempRadiant->heatingCoil() )
    {
      if( boost::optional<model::HVACComponent> waterToAirCoil = coil->optionalCast<model::HVACComponent>() )
      {
        boost::optional<model::ModelObject> moHeat = waterToAirCoil.get();

        m_heatingLoopChooserView->layoutModelObject(moHeat);

        waterHeatingCoil = true;
      }
    }
    if( boost::optional<model::HVACComponent> coil = lowTempRadiant->coolingCoil() )
    {
      if( boost::optional<model::HVACComponent> waterToAirCoil = coil->optionalCast<model::HVACComponent>() )
      {
        boost::optional<model::ModelObject> moCool = waterToAirCoil.get();

        m_coolingLoopChooserView->layoutModelObject(moCool);

        waterCoolingCoil = true;
      }
    }
  }

  if( ! waterHeatingCoil )
  {
    boost::optional<model::ModelObject> moHeat;

    m_heatingLoopChooserView->layoutModelObject(moHeat);
  }
  if( ! waterCoolingCoil )
  {
    boost::optional<model::ModelObject> moCool;

    m_coolingLoopChooserView->layoutModelObject(moCool);
  }

}

ZoneHVACLowTempRadiantVarFlowInspectorView::ZoneHVACLowTempRadiantVarFlowInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             m_inspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_heatingLoopChooserView = new LoopChooserView();
  m_coolingLoopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_heatingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->addTab( m_coolingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(0);

  connect( m_heatingLoopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  
  connect( m_heatingLoopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );

  connect( m_coolingLoopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  
  connect( m_coolingLoopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );


}

void ZoneHVACLowTempRadiantVarFlowInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP )
{
  
  m_modelObject = modelObject;

  bool force=false;
  bool recursive=true;
  bool locked=readOnly;
  bool hideChildren=false;
  if( displayIP )
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::IP);
  }
  else
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::SI);
  }
  m_inspectorGadget->layoutModelObj(modelObject, force, recursive, locked, hideChildren);

  bool waterHeatingCoil = false;
  bool waterCoolingCoil = false;

  if( boost::optional<model::ZoneHVACLowTempRadiantVarFlow> lowTempRadiant = 
        modelObject.optionalCast<model::ZoneHVACLowTempRadiantVarFlow>() )
  {
    if( boost::optional<model::HVACComponent> coil = lowTempRadiant->heatingCoil() )
    {
      if( boost::optional<model::HVACComponent> waterToAirCoil = coil->optionalCast<model::HVACComponent>() )
      {
        boost::optional<model::ModelObject> moHeat = waterToAirCoil.get();

        m_heatingLoopChooserView->layoutModelObject(moHeat);

        waterHeatingCoil = true;
      }
    }
    if( boost::optional<model::HVACComponent> coil = lowTempRadiant->coolingCoil() )
    {
      if( boost::optional<model::HVACComponent> waterToAirCoil = coil->optionalCast<model::HVACComponent>() )
      {
        boost::optional<model::ModelObject> moCool = waterToAirCoil.get();

        m_coolingLoopChooserView->layoutModelObject(moCool);

        waterCoolingCoil = true;
      }
    }
  }

  if( ! waterHeatingCoil )
  {
    boost::optional<model::ModelObject> moHeat;

    m_heatingLoopChooserView->layoutModelObject(moHeat);
  }
  if( ! waterCoolingCoil )
  {
    boost::optional<model::ModelObject> moCool;

    m_coolingLoopChooserView->layoutModelObject(moCool);
  }

}

ZoneHVACWaterToAirHeatPumpInspectorView::ZoneHVACWaterToAirHeatPumpInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             m_inspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_heatingLoopChooserView = new LoopChooserView();

  m_coolingLoopChooserView = new LoopChooserView();

  m_supHeatingLoopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_heatingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->addTab( m_coolingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->addTab( m_supHeatingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(0);

  connect( m_heatingLoopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  
  connect( m_heatingLoopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );

  connect( m_coolingLoopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  
  connect( m_coolingLoopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );

  connect( m_supHeatingLoopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  
  connect( m_supHeatingLoopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
}

void ZoneHVACWaterToAirHeatPumpInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP )
{
  m_modelObject = modelObject;

  bool force=false;
  bool recursive=true;
  bool locked=readOnly;
  bool hideChildren=false;
  if( displayIP )
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::IP);
  }
  else
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::SI);
  }
  m_inspectorGadget->layoutModelObj(modelObject, force, recursive, locked, hideChildren);

  bool waterHeatingCoil = false;
  bool waterCoolingCoil = false;
  bool supplementalHC = false;

  if( boost::optional<model::ZoneHVACWaterToAirHeatPump> wtaHP = 
        modelObject.optionalCast<model::ZoneHVACWaterToAirHeatPump>() )
  {
    if( boost::optional<model::HVACComponent> coil = wtaHP->heatingCoil() )
    {
      if( boost::optional<model::WaterToAirComponent> waterToAirCoil = coil->optionalCast<model::WaterToAirComponent>() )
      {
        boost::optional<model::ModelObject> moHeat = waterToAirCoil.get();

        m_heatingLoopChooserView->layoutModelObject(moHeat);

        waterHeatingCoil = true;
      }
    }
    if( boost::optional<model::HVACComponent> coil = wtaHP->coolingCoil() )
    {
      if( boost::optional<model::WaterToAirComponent> waterToAirCoil = coil->optionalCast<model::WaterToAirComponent>() )
      {
        boost::optional<model::ModelObject> moCool = waterToAirCoil.get();

        m_coolingLoopChooserView->layoutModelObject(moCool);

        waterCoolingCoil = true;
      }
    }
    if( boost::optional<model::HVACComponent> coil = wtaHP->supplementalHeatingCoil() )
    {
      if( boost::optional<model::WaterToAirComponent> waterToAirCoil = coil->optionalCast<model::WaterToAirComponent>() )
      {
        boost::optional<model::ModelObject> moSupplementalHeat = waterToAirCoil.get();

        m_supHeatingLoopChooserView->layoutModelObject(moSupplementalHeat);

        supplementalHC = true;
      }
    }
  }

  if( ! waterHeatingCoil )
  {
    boost::optional<model::ModelObject> moHeat;

    m_heatingLoopChooserView->layoutModelObject(moHeat);
  }
  if( ! waterCoolingCoil )
  {
    boost::optional<model::ModelObject> moCool;

    m_coolingLoopChooserView->layoutModelObject(moCool);
  }

  if( ! supplementalHC )
  {
    boost::optional<model::ModelObject> moSupplementalHeat;

    m_supHeatingLoopChooserView->layoutModelObject(moSupplementalHeat);
  }
}

ZoneHVACPackagedTerminalAirConditionerInspectorView::ZoneHVACPackagedTerminalAirConditionerInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             m_inspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_loopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_loopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(1);

  connect( m_loopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  
  connect( m_loopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
}

void ZoneHVACPackagedTerminalAirConditionerInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
{
  m_modelObject = modelObject;

  bool force=false;
  bool recursive=true;
  bool locked=readOnly;
  bool hideChildren=false;
  if( displayIP )
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::IP);
  }
  else
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::SI);
  }
  m_inspectorGadget->layoutModelObj(modelObject, force, recursive, locked, hideChildren);

  bool waterCoil = false;

  if( boost::optional<model::ZoneHVACPackagedTerminalAirConditioner> ptac = 
        modelObject.optionalCast<model::ZoneHVACPackagedTerminalAirConditioner>() )
  {
    if( boost::optional<model::HVACComponent> coil = ptac->heatingCoil() )
    {
      if( boost::optional<model::WaterToAirComponent> waterToAirCoil = coil->optionalCast<model::WaterToAirComponent>() )
      {
        boost::optional<model::ModelObject> mo = waterToAirCoil.get();

        m_loopChooserView->layoutModelObject(mo);

        waterCoil = true;
      }
    }
  }

  if( ! waterCoil )
  {
    boost::optional<model::ModelObject> mo;

    m_loopChooserView->layoutModelObject(mo);
  }
}

ZoneHVACUnitHeaterInspectorView::ZoneHVACUnitHeaterInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();

  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             m_inspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_heatingLoopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_heatingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(0);

  connect( m_heatingLoopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  
  connect( m_heatingLoopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
}

void ZoneHVACUnitHeaterInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP )
{
  m_modelObject = modelObject;

  bool force=false;
  bool recursive=true;
  bool locked=readOnly;
  bool hideChildren=false;
  if( displayIP )
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::IP);
  }
  else
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::SI);
  }
  m_inspectorGadget->layoutModelObj(modelObject, force, recursive, locked, hideChildren);

  bool waterHeatingCoil = false;

  
  if( boost::optional<model::ZoneHVACUnitHeater> unitHeater = 
        modelObject.optionalCast<model::ZoneHVACUnitHeater>() )
  {
    if( boost::optional<model::HVACComponent> coil = unitHeater->heatingCoil() )
    {
      if( boost::optional<model::WaterToAirComponent> waterToAirCoil = coil->optionalCast<model::WaterToAirComponent>() )
      {
        boost::optional<model::ModelObject> moHeat = waterToAirCoil.get();

        m_heatingLoopChooserView->layoutModelObject(moHeat);

        waterHeatingCoil = true;
      }
    }
  }

  if( ! waterHeatingCoil )
  {
    boost::optional<model::ModelObject> moHeat;

    m_heatingLoopChooserView->layoutModelObject(moHeat);
  }
}

AirLoopHVACUnitarySystemInspectorView::AirLoopHVACUnitarySystemInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();

  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             m_inspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  m_heatingLoopChooserView = new LoopChooserView();
  m_coolingLoopChooserView = new LoopChooserView();
  m_secondaryLoopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_heatingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->addTab( m_coolingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->addTab( m_secondaryLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(0);

  connect( m_heatingLoopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  connect( m_heatingLoopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );

  connect( m_coolingLoopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  connect( m_coolingLoopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );

  connect( m_secondaryLoopChooserView, SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(addToLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
  connect( m_secondaryLoopChooserView, SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)), 
           this,  SIGNAL(removeFromLoopClicked(model::Loop &, boost::optional<model::HVACComponent> &)) );
}

void AirLoopHVACUnitarySystemInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP )
{
  m_modelObject = modelObject;

  bool force=false;
  bool recursive=true;
  bool locked=readOnly;
  bool hideChildren=false;
  if( displayIP )
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::IP);
  }
  else
  {
    m_inspectorGadget->setUnitSystem(InspectorGadget::SI);
  }
  m_inspectorGadget->layoutModelObj(modelObject, force, recursive, locked, hideChildren);

  bool waterHeatingCoil = false;
  bool waterCoolingCoil = false;
  bool waterSecondaryCoil = false;

  
  if( boost::optional<model::AirLoopHVACUnitarySystem> unitary = 
        modelObject.optionalCast<model::AirLoopHVACUnitarySystem>() )
  {
    if( boost::optional<model::HVACComponent> coil = unitary->heatingCoil() )
    {
      if( boost::optional<model::WaterToAirComponent> waterToAirCoil = coil->optionalCast<model::WaterToAirComponent>() )
      {
        boost::optional<model::ModelObject> mo = waterToAirCoil.get();
        m_heatingLoopChooserView->layoutModelObject(mo);
        waterHeatingCoil = true;
      }
    }

    if( boost::optional<model::HVACComponent> coil = unitary->coolingCoil() )
    {
      if( boost::optional<model::WaterToAirComponent> waterToAirCoil = coil->optionalCast<model::WaterToAirComponent>() )
      {
        boost::optional<model::ModelObject> mo = waterToAirCoil.get();
        m_coolingLoopChooserView->layoutModelObject(mo);
        waterCoolingCoil = true;
      }
    }

    if( boost::optional<model::HVACComponent> coil = unitary->supplementalHeatingCoil() )
    {
      if( boost::optional<model::WaterToAirComponent> waterToAirCoil = coil->optionalCast<model::WaterToAirComponent>() )
      {
        boost::optional<model::ModelObject> mo = waterToAirCoil.get();
        m_secondaryLoopChooserView->layoutModelObject(mo);
        waterSecondaryCoil = true;
      }
    }
  }

  if( ! waterHeatingCoil )
  {
    boost::optional<model::ModelObject> mo;
    m_heatingLoopChooserView->layoutModelObject(mo);
  }

  if( ! waterCoolingCoil )
  {
    boost::optional<model::ModelObject> mo;
    m_coolingLoopChooserView->layoutModelObject(mo);
  }

  if( ! waterSecondaryCoil )
  {
    boost::optional<model::ModelObject> mo;
    m_secondaryLoopChooserView->layoutModelObject(mo);
  }
}

} // openstudio

