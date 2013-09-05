/**********************************************************************
 * Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <openstudio_lib/InspectorView.hpp>
#include <openstudio_lib/ZoneChooserView.hpp>
#include <openstudio_lib/LoopChooserView.hpp>
#include <openstudio_lib/LibraryTabWidget.hpp>
#include <model_editor/InspectorGadget.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/AirLoopHVACZoneMixer.hpp>
#include <model/AirLoopHVACZoneMixer_Impl.hpp>
#include <model/WaterToAirComponent.hpp>
#include <model/WaterToAirComponent_Impl.hpp>
#include <model/StraightComponent.hpp>
#include <model/StraightComponent_Impl.hpp>
#include <model/CoilCoolingCooledBeam.hpp>
#include <model/CoilCoolingCooledBeam_Impl.hpp>
#include <model/CoilCoolingWater.hpp>
#include <model/CoilCoolingWater_Impl.hpp>
#include <model/CoilCoolingLowTempRadiantConstFlow.hpp>
#include <model/CoilCoolingLowTempRadiantConstFlow_Impl.hpp>
#include <model/CoilCoolingLowTempRadiantVarFlow.hpp>
#include <model/CoilCoolingLowTempRadiantVarFlow_Impl.hpp>
#include <model/CoilHeatingWater.hpp>
#include <model/CoilHeatingWater_Impl.hpp>
#include <model/CoilHeatingWaterBaseboard.hpp>
#include <model/CoilHeatingWaterBaseboard_Impl.hpp>
#include <model/CoilHeatingLowTempRadiantConstFlow.hpp>
#include <model/CoilHeatingLowTempRadiantConstFlow_Impl.hpp>
#include <model/CoilHeatingLowTempRadiantVarFlow.hpp>
#include <model/CoilHeatingLowTempRadiantVarFlow_Impl.hpp>
#include <model/ControllerWaterCoil.hpp>
#include <model/ControllerWaterCoil_Impl.hpp>
#include <model/ConnectorSplitter.hpp>
#include <model/ConnectorSplitter_Impl.hpp>
#include <model/ConnectorMixer.hpp>
#include <model/ConnectorMixer_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeCooledBeam.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeCooledBeam_Impl.hpp>
#include <model/AirTerminalSingleDuctVAVReheat.hpp>
#include <model/AirTerminalSingleDuctVAVReheat_Impl.hpp>
#include <model/ZoneHVACBaseboardConvectiveWater.hpp>
#include <model/ZoneHVACBaseboardConvectiveWater_Impl.hpp>
#include <model/ZoneHVACFourPipeFanCoil.hpp>
#include <model/ZoneHVACFourPipeFanCoil_Impl.hpp>
#include <model/ZoneHVACLowTempRadiantConstFlow.hpp>
#include <model/ZoneHVACLowTempRadiantConstFlow_Impl.hpp>
#include <model/ZoneHVACLowTempRadiantVarFlow.hpp>
#include <model/ZoneHVACLowTempRadiantVarFlow_Impl.hpp>
#include <model/ZoneHVACWaterToAirHeatPump.hpp>
#include <model/ZoneHVACWaterToAirHeatPump_Impl.hpp>
#include <model/ZoneHVACPackagedTerminalAirConditioner.hpp>
#include <model/ZoneHVACPackagedTerminalAirConditioner_Impl.hpp>
#include <model/ZoneHVACUnitHeater.hpp>
#include <model/ZoneHVACUnitHeater_Impl.hpp>
#include <utilities/core/Assert.hpp>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QApplication>
#include <QTimer>

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
      BOOST_ASSERT(isConnected);
  
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
      BOOST_ASSERT(isConnected);
  
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
      BOOST_ASSERT(isConnected);
  
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

AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView::AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             m_inspectorGadget, SIGNAL(toggleUnitsClicked(bool)));
  BOOST_ASSERT(isConnected);

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

AirTerminalSingleDuctVAVReheatInspectorView::AirTerminalSingleDuctVAVReheatInspectorView( QWidget * parent )
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

void AirTerminalSingleDuctVAVReheatInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
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

  if( boost::optional<model::AirTerminalSingleDuctVAVReheat> terminal = modelObject.optionalCast<model::AirTerminalSingleDuctVAVReheat>() )
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
  BOOST_ASSERT(isConnected);

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
  BOOST_ASSERT(isConnected);

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

} // openstudio

