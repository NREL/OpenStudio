/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "InspectorView.hpp"

#include "GridItem.hpp"
#include "LibraryTabWidget.hpp"
#include "LoopChooserView.hpp"
#include "MainRightColumnController.hpp"
#include "ModelObjectItem.hpp"
#include "OSAppBase.hpp"
#include "OSItem.hpp"
#include "OSDocument.hpp"
#include "ZoneChooserView.hpp"

#include "../model/AirLoopHVACReturnPlenum.hpp"
#include "../model/AirLoopHVACReturnPlenum_Impl.hpp"
#include "../model/AirLoopHVACSupplyPlenum.hpp"
#include "../model/AirLoopHVACSupplyPlenum_Impl.hpp"
#include "../model/AirLoopHVACUnitarySystem.hpp"
#include "../model/AirLoopHVACUnitarySystem_Impl.hpp"
#include "../model/AirLoopHVACZoneMixer.hpp"
#include "../model/AirLoopHVACZoneMixer_Impl.hpp"
#include "../model/AirLoopHVACZoneSplitter.hpp"
#include "../model/AirLoopHVACZoneSplitter_Impl.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeCooledBeam.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeCooledBeam_Impl.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeReheat.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeReheat_Impl.hpp"
#include "../model/AirTerminalSingleDuctVAVReheat.hpp"
#include "../model/AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "../model/AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "../model/AirTerminalSingleDuctParallelPIUReheat_Impl.hpp"
#include "../model/AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "../model/AirTerminalSingleDuctSeriesPIUReheat_Impl.hpp"
#include "../model/CoilCoolingCooledBeam.hpp"
#include "../model/CoilCoolingCooledBeam_Impl.hpp"
#include "../model/CoilCoolingLowTempRadiantConstFlow.hpp"
#include "../model/CoilCoolingLowTempRadiantConstFlow_Impl.hpp"
#include "../model/CoilCoolingLowTempRadiantVarFlow.hpp"
#include "../model/CoilCoolingLowTempRadiantVarFlow_Impl.hpp"
#include "../model/CoilCoolingWater.hpp"
#include "../model/CoilCoolingWater_Impl.hpp"
#include "../model/CoilHeatingLowTempRadiantConstFlow.hpp"
#include "../model/CoilHeatingLowTempRadiantConstFlow_Impl.hpp"
#include "../model/CoilHeatingLowTempRadiantVarFlow.hpp"
#include "../model/CoilHeatingLowTempRadiantVarFlow_Impl.hpp"
#include "../model/CoilHeatingWater.hpp"
#include "../model/CoilHeatingWaterBaseboard.hpp"
#include "../model/CoilHeatingWaterBaseboard_Impl.hpp"
#include "../model/CoilHeatingWater_Impl.hpp"
#include "../model/ConnectorMixer.hpp"
#include "../model/ConnectorMixer_Impl.hpp"
#include "../model/ConnectorSplitter.hpp"
#include "../model/ConnectorSplitter_Impl.hpp"
#include "../model/ControllerWaterCoil.hpp"
#include "../model/ControllerWaterCoil_Impl.hpp"
#include "../model/HVACComponent.hpp"
#include "../model/HVACComponent_Impl.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeFourPipeInduction.hpp"
#include "../model/AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl.hpp"
#include "../model/AirTerminalSingleDuctVAVHeatAndCoolReheat.hpp"
#include "../model/AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/RefrigerationWalkIn.hpp"
#include "../model/RefrigerationWalkInZoneBoundary.hpp"
#include "../model/RefrigerationWalkInZoneBoundary_Impl.hpp"
#include "../model/RefrigerationWalkIn_Impl.hpp"
#include "../model/ScheduleRuleset.hpp"
#include "../model/ScheduleRuleset_Impl.hpp"
#include "../model/StraightComponent.hpp"
#include "../model/StraightComponent_Impl.hpp"
#include "../model/WaterToAirComponent.hpp"
#include "../model/WaterToAirComponent_Impl.hpp"
#include "../model/WaterToWaterComponent.hpp"
#include "../model/WaterToWaterComponent_Impl.hpp"
#include "../model/WaterHeaterHeatPump.hpp"
#include "../model/WaterHeaterHeatPump_Impl.hpp"
#include "../model/ZoneHVACBaseboardConvectiveWater.hpp"
#include "../model/ZoneHVACBaseboardConvectiveWater_Impl.hpp"
#include "../model/ZoneHVACBaseboardRadiantConvectiveWater.hpp"
#include "../model/ZoneHVACBaseboardRadiantConvectiveWater_Impl.hpp"
#include "../model/ZoneHVACFourPipeFanCoil.hpp"
#include "../model/ZoneHVACFourPipeFanCoil_Impl.hpp"
#include "../model/ZoneHVACLowTempRadiantConstFlow.hpp"
#include "../model/ZoneHVACLowTempRadiantConstFlow_Impl.hpp"
#include "../model/ZoneHVACLowTempRadiantVarFlow.hpp"
#include "../model/ZoneHVACLowTempRadiantVarFlow_Impl.hpp"
#include "../model/ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "../model/ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "../model/ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../model/ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "../model/ZoneHVACUnitHeater.hpp"
#include "../model/ZoneHVACUnitHeater_Impl.hpp"
#include "../model/ZoneHVACUnitVentilator.hpp"
#include "../model/ZoneHVACUnitVentilator_Impl.hpp"
#include "../model/ZoneHVACWaterToAirHeatPump.hpp"
#include "../model/ZoneHVACWaterToAirHeatPump_Impl.hpp"

#include "../model_editor/InspectorGadget.hpp"

#include "../utilities/core/Assert.hpp"

#include <QApplication>
#include <QBitmap>
#include <QButtonGroup>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>

namespace openstudio {

InspectorView::InspectorView(QWidget* parent)
  : QWidget(parent),
    m_currentView(nullptr)
{
  m_vLayout = new QVBoxLayout();

  m_vLayout->setContentsMargins(0,0,0,0);

  m_vLayout->setSpacing(0);

  setLayout(m_vLayout);
}

void InspectorView::enterEvent(QEvent * event)
{
  m_mouseOverInspectorView = true;

  QWidget::enterEvent(event);
}

void InspectorView::leaveEvent(QEvent * event)
{
  m_mouseOverInspectorView = false;

  QWidget::leaveEvent(event);
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
  m_modelObject = modelObject;

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

      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(splitter.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<SplitterMixerInspectorView *>(m_currentView), &SplitterMixerInspectorView::addZoneClicked, this, &InspectorView::addZoneClicked);

      connect(static_cast<SplitterMixerInspectorView *>(m_currentView), &SplitterMixerInspectorView::removeZoneClicked, this, &InspectorView::removeZoneClicked);
    }
    else if( model::OptionalThermalZone zone = modelObject->optionalCast<model::ThermalZone>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ThermalZoneInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      connect(static_cast<ThermalZoneInspectorView *>(m_currentView), &ThermalZoneInspectorView::moveBranchForZoneSupplySelected, this, &InspectorView::moveBranchForZoneSupplySelected);

      connect(static_cast<ThermalZoneInspectorView *>(m_currentView), &ThermalZoneInspectorView::moveBranchForZoneReturnSelected, this, &InspectorView::moveBranchForZoneReturnSelected);

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
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(mixer.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<SplitterMixerInspectorView *>(m_currentView), &SplitterMixerInspectorView::addZoneClicked, this, &InspectorView::addZoneClicked);

      connect(static_cast<SplitterMixerInspectorView *>(m_currentView), &SplitterMixerInspectorView::removeZoneClicked, this, &InspectorView::removeZoneClicked);
    }
    else if( boost::optional<model::AirTerminalSingleDuctConstantVolumeFourPipeInduction> component =
             modelObject->optionalCast<model::AirTerminalSingleDuctConstantVolumeFourPipeInduction>()  )

    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new AirTerminalSingleDuctConstantVolumeFourPipeInductionInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<AirLoopHVACUnitarySystemInspectorView *>(m_currentView), &AirLoopHVACUnitarySystemInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<AirLoopHVACUnitarySystemInspectorView *>(m_currentView), &AirLoopHVACUnitarySystemInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::AirLoopHVACUnitarySystem> component =
             modelObject->optionalCast<model::AirLoopHVACUnitarySystem>()  )

    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new AirLoopHVACUnitarySystemInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<AirLoopHVACUnitarySystemInspectorView *>(m_currentView), &AirLoopHVACUnitarySystemInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<AirLoopHVACUnitarySystemInspectorView *>(m_currentView), &AirLoopHVACUnitarySystemInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::WaterToAirComponent> component = modelObject->optionalCast<model::WaterToAirComponent>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new WaterToAirInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<WaterToAirInspectorView *>(m_currentView), &WaterToAirInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<WaterToAirInspectorView *>(m_currentView), &WaterToAirInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::AirTerminalSingleDuctConstantVolumeCooledBeam> component = modelObject->optionalCast<model::AirTerminalSingleDuctConstantVolumeCooledBeam>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView *>(m_currentView), &AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView *>(m_currentView), &AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::AirTerminalSingleDuctConstantVolumeReheat> component = modelObject->optionalCast<model::AirTerminalSingleDuctConstantVolumeReheat>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new AirTerminalSingleDuctConstantVolumeReheatInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<AirTerminalSingleDuctConstantVolumeReheatInspectorView *>(m_currentView), &AirTerminalSingleDuctConstantVolumeReheatInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<AirTerminalSingleDuctConstantVolumeReheatInspectorView *>(m_currentView), &AirTerminalSingleDuctConstantVolumeReheatInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::AirTerminalSingleDuctVAVReheat> component = modelObject->optionalCast<model::AirTerminalSingleDuctVAVReheat>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new AirTerminalSingleDuctVAVReheatInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<AirTerminalSingleDuctVAVReheatInspectorView *>(m_currentView), &AirTerminalSingleDuctVAVReheatInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<AirTerminalSingleDuctVAVReheatInspectorView *>(m_currentView), &AirTerminalSingleDuctVAVReheatInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( auto component = modelObject->optionalCast<model::AirTerminalSingleDuctParallelPIUReheat>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new AirTerminalSingleDuctParallelPIUReheatInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<AirTerminalSingleDuctParallelPIUReheatInspectorView *>(m_currentView), &AirTerminalSingleDuctParallelPIUReheatInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<AirTerminalSingleDuctParallelPIUReheatInspectorView *>(m_currentView), &AirTerminalSingleDuctParallelPIUReheatInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( auto component = modelObject->optionalCast<model::AirTerminalSingleDuctSeriesPIUReheat>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new AirTerminalSingleDuctSeriesPIUReheatInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<AirTerminalSingleDuctSeriesPIUReheatInspectorView *>(m_currentView), &AirTerminalSingleDuctSeriesPIUReheatInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<AirTerminalSingleDuctSeriesPIUReheatInspectorView *>(m_currentView), &AirTerminalSingleDuctSeriesPIUReheatInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::AirTerminalSingleDuctVAVHeatAndCoolReheat> component = modelObject->optionalCast<model::AirTerminalSingleDuctVAVHeatAndCoolReheat>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new AirTerminalSingleDuctVAVHeatAndCoolReheatInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<AirTerminalSingleDuctVAVHeatAndCoolReheatInspectorView *>(m_currentView), &AirTerminalSingleDuctVAVHeatAndCoolReheatInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<AirTerminalSingleDuctVAVHeatAndCoolReheatInspectorView *>(m_currentView), &AirTerminalSingleDuctVAVHeatAndCoolReheatInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::ZoneHVACBaseboardConvectiveWater> component =
             modelObject->optionalCast<model::ZoneHVACBaseboardConvectiveWater>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACBaseboardConvectiveWaterInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<ZoneHVACBaseboardConvectiveWaterInspectorView *>(m_currentView), &ZoneHVACBaseboardConvectiveWaterInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<ZoneHVACBaseboardConvectiveWaterInspectorView *>(m_currentView), &ZoneHVACBaseboardConvectiveWaterInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::ZoneHVACBaseboardRadiantConvectiveWater> component =
             modelObject->optionalCast<model::ZoneHVACBaseboardRadiantConvectiveWater>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACBaseboardRadiantConvectiveWaterInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<ZoneHVACBaseboardRadiantConvectiveWaterInspectorView *>(m_currentView), &ZoneHVACBaseboardRadiantConvectiveWaterInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<ZoneHVACBaseboardRadiantConvectiveWaterInspectorView *>(m_currentView), &ZoneHVACBaseboardRadiantConvectiveWaterInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::ZoneHVACFourPipeFanCoil> component =
             modelObject->optionalCast<model::ZoneHVACFourPipeFanCoil>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACFourPipeFanCoilInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<ZoneHVACFourPipeFanCoilInspectorView *>(m_currentView), &ZoneHVACFourPipeFanCoilInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<ZoneHVACFourPipeFanCoilInspectorView *>(m_currentView), &ZoneHVACFourPipeFanCoilInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::ZoneHVACLowTempRadiantConstFlow> component =
             modelObject->optionalCast<model::ZoneHVACLowTempRadiantConstFlow>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACLowTempRadiantConstFlowInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<ZoneHVACLowTempRadiantConstFlowInspectorView *>(m_currentView), &ZoneHVACLowTempRadiantConstFlowInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<ZoneHVACLowTempRadiantConstFlowInspectorView *>(m_currentView), &ZoneHVACLowTempRadiantConstFlowInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }

    else if( boost::optional<model::ZoneHVACLowTempRadiantVarFlow> component =
             modelObject->optionalCast<model::ZoneHVACLowTempRadiantVarFlow>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACLowTempRadiantVarFlowInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<ZoneHVACLowTempRadiantVarFlowInspectorView *>(m_currentView), &ZoneHVACLowTempRadiantVarFlowInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<ZoneHVACLowTempRadiantVarFlowInspectorView *>(m_currentView), &ZoneHVACLowTempRadiantVarFlowInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::ZoneHVACWaterToAirHeatPump> component =
             modelObject->optionalCast<model::ZoneHVACWaterToAirHeatPump>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACWaterToAirHeatPumpInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<ZoneHVACWaterToAirHeatPumpInspectorView *>(m_currentView), &ZoneHVACWaterToAirHeatPumpInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<ZoneHVACWaterToAirHeatPumpInspectorView *>(m_currentView), &ZoneHVACWaterToAirHeatPumpInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::ZoneHVACPackagedTerminalAirConditioner> component =
             modelObject->optionalCast<model::ZoneHVACPackagedTerminalAirConditioner>()  )

    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACPackagedTerminalAirConditionerInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<ZoneHVACPackagedTerminalAirConditionerInspectorView *>(m_currentView), &ZoneHVACPackagedTerminalAirConditionerInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<ZoneHVACPackagedTerminalAirConditionerInspectorView *>(m_currentView), &ZoneHVACPackagedTerminalAirConditionerInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::ZoneHVACPackagedTerminalHeatPump> component =
             modelObject->optionalCast<model::ZoneHVACPackagedTerminalHeatPump>()  )

    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACPackagedTerminalHeatPumpInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<ZoneHVACPackagedTerminalHeatPumpInspectorView *>(m_currentView), &ZoneHVACPackagedTerminalHeatPumpInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<ZoneHVACPackagedTerminalHeatPumpInspectorView *>(m_currentView), &ZoneHVACPackagedTerminalHeatPumpInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::WaterHeaterHeatPump> component =
             modelObject->optionalCast<model::WaterHeaterHeatPump>()  )

    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new WaterHeaterHeatPumpInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<WaterHeaterHeatPumpInspectorView *>(m_currentView), &WaterHeaterHeatPumpInspectorView::addToLoopClicked,
              this, &InspectorView::addToLoopClicked);

      connect(static_cast<WaterHeaterHeatPumpInspectorView *>(m_currentView), &WaterHeaterHeatPumpInspectorView::removeFromLoopClicked,
              this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::ZoneHVACUnitHeater> component =
               modelObject->optionalCast<model::ZoneHVACUnitHeater>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACUnitHeaterInspectorView();

      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<ZoneHVACUnitHeaterInspectorView *>(m_currentView), &ZoneHVACUnitHeaterInspectorView::addToLoopClicked,
        this, &InspectorView::addToLoopClicked);

      connect(static_cast<ZoneHVACUnitHeaterInspectorView *>(m_currentView), &ZoneHVACUnitHeaterInspectorView::removeFromLoopClicked,
        this, &InspectorView::removeFromLoopClicked);
    }
    else if( boost::optional<model::ZoneHVACUnitVentilator> component =
               modelObject->optionalCast<model::ZoneHVACUnitVentilator>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new ZoneHVACUnitVentilatorInspectorView();

      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);

      connect(static_cast<ZoneHVACUnitVentilatorInspectorView *>(m_currentView), &ZoneHVACUnitVentilatorInspectorView::addToLoopClicked,
        this, &InspectorView::addToLoopClicked);

      connect(static_cast<ZoneHVACUnitVentilatorInspectorView *>(m_currentView), &ZoneHVACUnitVentilatorInspectorView::removeFromLoopClicked,
        this, &InspectorView::removeFromLoopClicked);
    }
    else if( modelObject->optionalCast<model::ConnectorSplitter>() ||
             modelObject->optionalCast<model::ConnectorMixer>()  )
    {
      if( m_currentView )
      {
        delete m_currentView;
      }

      m_currentView = new GenericInspectorView();
      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

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

      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

      m_currentView->layoutModelObject(component.get(), readOnly, displayIP);

      m_vLayout->addWidget(m_currentView);
    }
    else if (boost::optional<model::ScheduleRuleset> component =
      modelObject->optionalCast<model::ScheduleRuleset>())
    {
      if (m_currentView)
      {
        delete m_currentView;
      }

      m_currentView = new ScheduleRulesetInspectorView();

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

      connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

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

    connect(this, &InspectorView::toggleUnitsClicked, m_currentView, &BaseInspectorView::toggleUnitsClicked);

    m_vLayout->addWidget(m_currentView);
  }

  if( m_currentView )
  {
    m_currentView->show();
    connect(m_currentView, &BaseInspectorView::removeButtonClicked, this, &InspectorView::removeButtonClicked);
    connect(m_currentView, &BaseInspectorView::workspaceObjectRemoved, this, &InspectorView::workspaceObjectRemoved);
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

  auto isConnected = connect(m_libraryTabWidget, SIGNAL(removeButtonClicked(bool)), this, SIGNAL(removeButtonClicked(bool)));
  OS_ASSERT(isConnected);

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

  connect(m_zoneChooserView, &ZoneChooserView::addZoneClicked, this, &SplitterMixerInspectorView::addZoneClicked);

  connect(m_zoneChooserView, &ZoneChooserView::removeZoneClicked, this, &SplitterMixerInspectorView::removeZoneClicked);
}

void SplitterMixerInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
{
  m_zoneChooserView->layoutModelObject(modelObject);
}

GenericInspectorView::GenericInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();

  connect(this, &GenericInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

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
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setAlignment(Qt::AlignTop);
  mainVLayout->setContentsMargins(10,10,10,10);
  mainVLayout->setSpacing(10);
  setLayout(mainVLayout);

  setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
  setFixedWidth(400);

  auto message = new QLabel();
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

  for(auto it = allZones.begin();
      it != allZones.end();
      ++it)
  {
    if( (! it->isPlenum()) && it->equipment().empty() && (! it->airLoopHVAC()) )
    {
      zoneChooser->addItem(QString::fromStdString(it->name().get()),toQString(it->handle()));
    }
  }

  mainVLayout->addSpacing(10);

  auto buttonBox = new QDialogButtonBox();
  QPushButton * cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);
  connect(cancelButton, &QPushButton::clicked, this, &NewPlenumDialog::onCancelClicked);
  QPushButton * applyButton = buttonBox->addButton(QDialogButtonBox::Apply);
  connect(applyButton, &QPushButton::clicked, this, &NewPlenumDialog::onApplyClicked);
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
  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setAlignment(Qt::AlignTop);
  setLayout(mainVLayout);
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setSpacing(0);

  auto supplyFrame = new QFrame();
  supplyFrame->setObjectName("IGRow");
  auto supplyVLayout = new QVBoxLayout();
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

  auto returnFrame = new QFrame();
  returnFrame->setObjectName("IGRow");
  auto returnVLayout = new QVBoxLayout();
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
  connect(this, &RefrigerationWalkinInspectorView::toggleUnitsClicked, m_walkinInspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_walkinInspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

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
  auto zoneBoundaryLayout = new QVBoxLayout();
  zoneBoundaryLayout->setContentsMargins(0,0,0,0);
  zoneBoundaryLayout->setSpacing(0);
  m_zoneBoundaryWidget->setLayout(zoneBoundaryLayout);
  boost::optional<model::RefrigerationWalkIn> walkin = modelObject.optionalCast<model::RefrigerationWalkIn>();
  OS_ASSERT(walkin);
  std::vector<model::RefrigerationWalkInZoneBoundary> zoneBoundaries = walkin->zoneBoundaries();
  for( auto it = zoneBoundaries.begin();
       it != zoneBoundaries.end();
       ++it )
  {
    auto inspector = new InspectorGadget();
    connect(this, &RefrigerationWalkinInspectorView::toggleUnitsClicked, inspector, &InspectorGadget::toggleUnitsClicked);
    connect(inspector, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);
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
  connect(this, &ThermalZoneInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);
  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_plenumChooser = new PlenumChooserView();
  m_libraryTabWidget->addTab( m_plenumChooser,
                              ":images/plenum_on.png",
                              ":images/plenum_off.png" );

  connect(m_plenumChooser->supplyPlenumChooser, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &ThermalZoneInspectorView::onSupplyPlenumChooserChanged);

  connect(m_plenumChooser->returnPlenumChooser, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
          this, &ThermalZoneInspectorView::onReturnPlenumChooserChanged);

  connect(m_plenumChooser->newSupplyPlenumButton, &QToolButton::clicked, this, &ThermalZoneInspectorView::onNewSupplyPlenumClicked);

  connect(m_plenumChooser->newReturnPlenumButton, &QToolButton::clicked, this, &ThermalZoneInspectorView::onNewReturnPlenumClicked);
}

void ThermalZoneInspectorView::onSupplyPlenumChooserChanged(int newIndex)
{
  OS_ASSERT(m_modelObject);
  boost::optional<model::ThermalZone> thermalZone;
  thermalZone = m_modelObject->optionalCast<model::ThermalZone>();
  OS_ASSERT(thermalZone);

  QString newPlenumString = m_plenumChooser->supplyPlenumChooser->itemData(newIndex).toString();
  Handle newPlenumHandle(toUUID(newPlenumString));

  emit moveBranchForZoneSupplySelected(thermalZone.get(),newPlenumHandle);
}

void ThermalZoneInspectorView::onReturnPlenumChooserChanged(int newIndex)
{
  OS_ASSERT(m_modelObject);
  boost::optional<model::ThermalZone> thermalZone;
  thermalZone = m_modelObject->optionalCast<model::ThermalZone>();
  OS_ASSERT(thermalZone);

  QString newPlenumString = m_plenumChooser->returnPlenumChooser->itemData(newIndex).toString();
  Handle newPlenumHandle(toUUID(newPlenumString));

  emit moveBranchForZoneReturnSelected(thermalZone.get(),newPlenumHandle);
}

void ThermalZoneInspectorView::onNewSupplyPlenumClicked()
{
  NewPlenumDialog dialog;
  int result = dialog.exec();

  if( result == QDialog::Accepted )
  {
    QComboBox * cb = dialog.zoneChooser;
    Handle newZoneHandle(toUUID(cb->itemData(cb->currentIndex()).toString()));
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
    Handle newZoneHandle(toUUID(cb->itemData(cb->currentIndex()).toString()));
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
  if ((! t_airLoopHVAC) || (t_airLoopHVAC->handle().isNull())) { return; }

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();
  std::shared_ptr<MainRightColumnController> mrc = doc->mainRightColumnController();
  SystemItem * systemItem = mrc->systemItem(t_airLoopHVAC->handle());
  // if there is t_airLoopHVAC but no systemItem then we are probably showing this view from the grid.
  // Don't do that.  But in just in case, return gracefully.
  if( ! systemItem ) { return; }

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
  for( auto it = supplyPlenums.begin();
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
      supplyChooser->addItem(supplyPixmap,QString::fromStdString(t_plenumZone->name().get()),toQString(it->handle()));
    }
    else
    {
      supplyChooser->addItem(supplyPixmap,QString::fromStdString(it->name().get()),toQString(it->handle()));
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
    supplyChooser->setCurrentIndex(supplyChooser->findData(toQString(thisZoneSupplyPlenums.front().handle())));
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
  for( auto it = returnPlenums.begin();
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
      returnChooser->addItem(returnPixmap,QString::fromStdString(t_plenumZone->name().get()), toQString(it->handle()));
    }
    else
    {
      returnChooser->addItem(returnPixmap,QString::fromStdString(it->name().get()), toQString(it->handle()));
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
    returnChooser->setCurrentIndex(returnChooser->findData(toQString(thisZoneReturnPlenums.front().handle())));
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
  connect(this, &WaterToAirInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

  m_coilControllerInspectorGadget = new InspectorGadget();
  connect(this, &WaterToAirInspectorView::toggleUnitsClicked, m_coilControllerInspectorGadget, &InspectorGadget::toggleUnitsClicked);

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

  connect(m_loopChooserView, &LoopChooserView::addToLoopClicked, this, &WaterToAirInspectorView::addToLoopClicked);

  connect(m_loopChooserView, &LoopChooserView::removeFromLoopClicked, this, &WaterToAirInspectorView::removeFromLoopClicked);
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

  model::detail::ModelObject_Impl * impl = modelObject.getImpl<model::detail::ModelObject_Impl>().get();

  impl->onChange.connect<WaterToAirInspectorView, &WaterToAirInspectorView::onWorkspaceObjectChanged>(this);
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
  connect(this, &AirTerminalInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

  m_loopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_loopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(0);

  connect(m_loopChooserView, &LoopChooserView::addToLoopClicked, this, &AirTerminalInspectorView::addToLoopClicked);

  connect(m_loopChooserView, &LoopChooserView::removeFromLoopClicked, this, &AirTerminalInspectorView::removeFromLoopClicked);
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

AirTerminalSingleDuctParallelPIUReheatInspectorView::AirTerminalSingleDuctParallelPIUReheatInspectorView( QWidget * parent )
  : AirTerminalInspectorView(parent)
{

}

void AirTerminalSingleDuctParallelPIUReheatInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
{
  AirTerminalInspectorView::layoutModelObject<model::AirTerminalSingleDuctParallelPIUReheat>(modelObject, readOnly, displayIP);
}

AirTerminalSingleDuctSeriesPIUReheatInspectorView::AirTerminalSingleDuctSeriesPIUReheatInspectorView( QWidget * parent )
  : AirTerminalInspectorView(parent)
{

}

void AirTerminalSingleDuctSeriesPIUReheatInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
{
  AirTerminalInspectorView::layoutModelObject<model::AirTerminalSingleDuctSeriesPIUReheat>(modelObject, readOnly, displayIP);
}

AirTerminalSingleDuctVAVHeatAndCoolReheatInspectorView::AirTerminalSingleDuctVAVHeatAndCoolReheatInspectorView( QWidget * parent )
  : AirTerminalInspectorView(parent)
{

}

void AirTerminalSingleDuctVAVHeatAndCoolReheatInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
{
  AirTerminalInspectorView::layoutModelObject<model::AirTerminalSingleDuctVAVHeatAndCoolReheat>(modelObject, readOnly, displayIP);
}

AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView::AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();
  connect(this, &AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

  m_coolingLoopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_coolingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(1);

  connect(m_coolingLoopChooserView, &LoopChooserView::addToLoopClicked, this, &AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView::addToLoopClicked);

  connect(m_coolingLoopChooserView, &LoopChooserView::removeFromLoopClicked, this, &AirTerminalSingleDuctConstantVolumeCooledBeamInspectorView::removeFromLoopClicked);
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
  connect(this, &ZoneHVACBaseboardConvectiveWaterInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

  m_heatingLoopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_heatingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(0);

  connect(m_heatingLoopChooserView, &LoopChooserView::addToLoopClicked, this, &ZoneHVACBaseboardConvectiveWaterInspectorView::addToLoopClicked);

  connect(m_heatingLoopChooserView, &LoopChooserView::removeFromLoopClicked, this, &ZoneHVACBaseboardConvectiveWaterInspectorView::removeFromLoopClicked);
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

ZoneHVACBaseboardRadiantConvectiveWaterInspectorView::ZoneHVACBaseboardRadiantConvectiveWaterInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();
  connect(this, &ZoneHVACBaseboardRadiantConvectiveWaterInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

  m_heatingLoopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_heatingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(0);

  connect(m_heatingLoopChooserView, &LoopChooserView::addToLoopClicked, this, &ZoneHVACBaseboardRadiantConvectiveWaterInspectorView::addToLoopClicked);

  connect(m_heatingLoopChooserView, &LoopChooserView::removeFromLoopClicked, this, &ZoneHVACBaseboardRadiantConvectiveWaterInspectorView::removeFromLoopClicked);
}

void ZoneHVACBaseboardRadiantConvectiveWaterInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP )
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
  boost::optional<model::ZoneHVACBaseboardRadiantConvectiveWater> baseboardConvtest =
        modelObject.optionalCast<model::ZoneHVACBaseboardRadiantConvectiveWater>();

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
  connect(this, &ZoneHVACFourPipeFanCoilInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

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

  connect(m_heatingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &ZoneHVACFourPipeFanCoilInspectorView::addToLoopClicked);

  connect(m_heatingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &ZoneHVACFourPipeFanCoilInspectorView::removeFromLoopClicked);

  connect(m_coolingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &ZoneHVACFourPipeFanCoilInspectorView::addToLoopClicked);

  connect(m_coolingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &ZoneHVACFourPipeFanCoilInspectorView::removeFromLoopClicked);
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
  connect(this, &ZoneHVACLowTempRadiantConstFlowInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

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

  connect(m_heatingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &ZoneHVACLowTempRadiantConstFlowInspectorView::addToLoopClicked);

  connect(m_heatingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &ZoneHVACLowTempRadiantConstFlowInspectorView::removeFromLoopClicked);

  connect(m_coolingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &ZoneHVACLowTempRadiantConstFlowInspectorView::addToLoopClicked);

  connect(m_coolingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &ZoneHVACLowTempRadiantConstFlowInspectorView::removeFromLoopClicked);


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
  connect(this, &ZoneHVACLowTempRadiantVarFlowInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

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

  connect(m_heatingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &ZoneHVACLowTempRadiantVarFlowInspectorView::addToLoopClicked);

  connect(m_heatingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &ZoneHVACLowTempRadiantVarFlowInspectorView::removeFromLoopClicked);

  connect(m_coolingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &ZoneHVACLowTempRadiantVarFlowInspectorView::addToLoopClicked);

  connect(m_coolingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &ZoneHVACLowTempRadiantVarFlowInspectorView::removeFromLoopClicked);

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
  connect(this, &ZoneHVACWaterToAirHeatPumpInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

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

  connect(m_heatingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &ZoneHVACWaterToAirHeatPumpInspectorView::addToLoopClicked);

  connect(m_heatingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &ZoneHVACWaterToAirHeatPumpInspectorView::removeFromLoopClicked);

  connect(m_coolingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &ZoneHVACWaterToAirHeatPumpInspectorView::addToLoopClicked);

  connect(m_coolingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &ZoneHVACWaterToAirHeatPumpInspectorView::removeFromLoopClicked);

  connect(m_supHeatingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &ZoneHVACWaterToAirHeatPumpInspectorView::addToLoopClicked);

  connect(m_supHeatingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &ZoneHVACWaterToAirHeatPumpInspectorView::removeFromLoopClicked);
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
  connect(this, &ZoneHVACPackagedTerminalAirConditionerInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

  m_loopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_loopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(1);

  connect(m_loopChooserView, &LoopChooserView::addToLoopClicked,
          this, &ZoneHVACPackagedTerminalAirConditionerInspectorView::addToLoopClicked);

  connect(m_loopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &ZoneHVACPackagedTerminalAirConditionerInspectorView::removeFromLoopClicked);
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

ZoneHVACPackagedTerminalHeatPumpInspectorView::ZoneHVACPackagedTerminalHeatPumpInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();
  connect(this, &ZoneHVACPackagedTerminalHeatPumpInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

  m_loopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_loopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(0);

  connect(m_loopChooserView, &LoopChooserView::addToLoopClicked,
          this, &ZoneHVACPackagedTerminalHeatPumpInspectorView::addToLoopClicked);

  connect(m_loopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &ZoneHVACPackagedTerminalHeatPumpInspectorView::removeFromLoopClicked);
}

void ZoneHVACPackagedTerminalHeatPumpInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
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

  if( boost::optional<model::ZoneHVACPackagedTerminalHeatPump> pthp =
        modelObject.optionalCast<model::ZoneHVACPackagedTerminalHeatPump>() )
  {
    if( boost::optional<model::HVACComponent> coil = pthp->supplementalHeatingCoil() )
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

WaterHeaterHeatPumpInspectorView::WaterHeaterHeatPumpInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();
  connect(this, &WaterHeaterHeatPumpInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

  m_loopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_loopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(0);

  connect(m_loopChooserView, &LoopChooserView::addToLoopClicked,
          this, &WaterHeaterHeatPumpInspectorView::addToLoopClicked);

  connect(m_loopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &WaterHeaterHeatPumpInspectorView::removeFromLoopClicked);
}

void WaterHeaterHeatPumpInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP)
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

  if( auto hpwh = modelObject.optionalCast<model::WaterHeaterHeatPump>() )
  {
    if( auto waterToWaterTank = hpwh->tank().optionalCast<model::WaterToWaterComponent>() )
    {
      auto tankMO = waterToWaterTank->optionalCast<model::ModelObject>();
      m_loopChooserView->layoutModelObject(tankMO);

      waterCoil = true;
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

  connect(this, &ZoneHVACUnitHeaterInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

  m_heatingLoopChooserView = new LoopChooserView();

  m_libraryTabWidget->addTab( m_inspectorGadget,
                              ":images/properties_icon_on.png",
                              ":images/properties_icon_off.png" );

  m_libraryTabWidget->addTab( m_heatingLoopChooserView,
                              ":images/link_icon_on.png",
                              ":images/link_icon_off.png" );

  m_libraryTabWidget->setCurrentIndex(0);

  connect(m_heatingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &ZoneHVACUnitHeaterInspectorView::addToLoopClicked);

  connect(m_heatingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &ZoneHVACUnitHeaterInspectorView::removeFromLoopClicked);
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

ZoneHVACUnitVentilatorInspectorView::ZoneHVACUnitVentilatorInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();

  connect(this, &ZoneHVACUnitVentilatorInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

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

  connect(m_heatingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &ZoneHVACUnitVentilatorInspectorView::addToLoopClicked);

  connect(m_heatingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &ZoneHVACUnitVentilatorInspectorView::removeFromLoopClicked);

  connect(m_coolingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &ZoneHVACUnitVentilatorInspectorView::addToLoopClicked);

  connect(m_coolingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &ZoneHVACUnitVentilatorInspectorView::removeFromLoopClicked);
}

void ZoneHVACUnitVentilatorInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP )
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


  if( boost::optional<model::ZoneHVACUnitVentilator> unitVentilator =
        modelObject.optionalCast<model::ZoneHVACUnitVentilator>() )
  {
    if( auto coil = unitVentilator->heatingCoil() ) {
      if( auto waterToAirCoil = coil->optionalCast<model::WaterToAirComponent>() ) {
        boost::optional<model::ModelObject> omo = waterToAirCoil.get();
        m_heatingLoopChooserView->layoutModelObject(omo);
        waterHeatingCoil = true;
      }
    }
    if( auto coil = unitVentilator->coolingCoil() ) {
      if( auto waterToAirCoil = coil->optionalCast<model::WaterToAirComponent>() ) {
        boost::optional<model::ModelObject> omo = waterToAirCoil.get();
        m_coolingLoopChooserView->layoutModelObject(omo);
        waterCoolingCoil = true;
      }
    }
  }

  if( ! waterHeatingCoil ) {
    boost::optional<model::ModelObject> omo;
    m_heatingLoopChooserView->layoutModelObject(omo);
  }
  if( ! waterCoolingCoil ) {
    boost::optional<model::ModelObject> omo;
    m_coolingLoopChooserView->layoutModelObject(omo);
  }
}

AirLoopHVACUnitarySystemInspectorView::AirLoopHVACUnitarySystemInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();

  connect(this, &AirLoopHVACUnitarySystemInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

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

  connect(m_heatingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &AirLoopHVACUnitarySystemInspectorView::addToLoopClicked);
  connect(m_heatingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &AirLoopHVACUnitarySystemInspectorView::removeFromLoopClicked);

  connect(m_coolingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &AirLoopHVACUnitarySystemInspectorView::addToLoopClicked);
  connect(m_coolingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &AirLoopHVACUnitarySystemInspectorView::removeFromLoopClicked);

  connect(m_secondaryLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &AirLoopHVACUnitarySystemInspectorView::addToLoopClicked);
  connect(m_secondaryLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &AirLoopHVACUnitarySystemInspectorView::removeFromLoopClicked);
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

AirTerminalSingleDuctConstantVolumeFourPipeInductionInspectorView::AirTerminalSingleDuctConstantVolumeFourPipeInductionInspectorView( QWidget * parent )
  : BaseInspectorView(parent)
{
  m_inspectorGadget = new InspectorGadget();

  connect(this, &AirTerminalSingleDuctConstantVolumeFourPipeInductionInspectorView::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);
  connect(m_inspectorGadget, &InspectorGadget::workspaceObjectRemoved, this, &BaseInspectorView::workspaceObjectRemoved);

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

  connect(m_heatingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &AirTerminalSingleDuctConstantVolumeFourPipeInductionInspectorView::addToLoopClicked);
  connect(m_heatingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &AirTerminalSingleDuctConstantVolumeFourPipeInductionInspectorView::removeFromLoopClicked);

  connect(m_coolingLoopChooserView, &LoopChooserView::addToLoopClicked,
          this, &AirTerminalSingleDuctConstantVolumeFourPipeInductionInspectorView::addToLoopClicked);
  connect(m_coolingLoopChooserView, &LoopChooserView::removeFromLoopClicked,
          this, &AirTerminalSingleDuctConstantVolumeFourPipeInductionInspectorView::removeFromLoopClicked);
}

void AirTerminalSingleDuctConstantVolumeFourPipeInductionInspectorView::layoutModelObject( model::ModelObject & modelObject, bool readOnly, bool displayIP )
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


  if( boost::optional<model::AirTerminalSingleDuctConstantVolumeFourPipeInduction> terminal =
        modelObject.optionalCast<model::AirTerminalSingleDuctConstantVolumeFourPipeInduction>() )
  {
    if( boost::optional<model::HVACComponent> coil = terminal->heatingCoil() )
    {
      if( boost::optional<model::WaterToAirComponent> waterToAirCoil = coil->optionalCast<model::WaterToAirComponent>() )
      {
        boost::optional<model::ModelObject> mo = waterToAirCoil.get();
        m_heatingLoopChooserView->layoutModelObject(mo);
        waterHeatingCoil = true;
      }
    }

    if( boost::optional<model::HVACComponent> coil = terminal->coolingCoil() )
    {
      if( boost::optional<model::WaterToAirComponent> waterToAirCoil = coil->optionalCast<model::WaterToAirComponent>() )
      {
        boost::optional<model::ModelObject> mo = waterToAirCoil.get();
        m_coolingLoopChooserView->layoutModelObject(mo);
        waterCoolingCoil = true;
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
}

ScheduleRulesetInspectorView::ScheduleRulesetInspectorView(QWidget * parent)
  : BaseInspectorView(parent)
{
}

void ScheduleRulesetInspectorView::layoutModelObject(model::ModelObject & modelObject, bool readOnly, bool displayIP)
{
  auto widget = new QWidget();

  auto mainLayout = new QVBoxLayout();
  mainLayout->setAlignment(Qt::AlignTop);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);
  widget->setLayout(mainLayout);

  auto label = new QLabel(modelObject.iddObject().type().valueDescription().c_str());
  label->setObjectName("IGHeader");
  label->setStyleSheet("font : bold");
  mainLayout->addWidget(label, 0, Qt::AlignTop);

  auto layout = new QVBoxLayout();
  layout->setAlignment(Qt::AlignTop);
  layout->setContentsMargins(10, 10, 10, 10);
  layout->setSpacing(6);

  label = new QLabel("Name");
  layout->addWidget(label, 0, Qt::AlignTop);

  QString text;
  auto name = modelObject.name();
  if (name) {
    text = name->c_str();
  }
  auto lineEdit = new QLineEdit(text);
  lineEdit->setReadOnly(true);
  layout->addWidget(lineEdit, 0, Qt::AlignTop);
  mainLayout->addLayout(layout);

  auto line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainLayout->addWidget(line);

  layout = new QVBoxLayout();
  layout->setAlignment(Qt::AlignTop);
  layout->setContentsMargins(10, 10, 10, 10);
  layout->setSpacing(6);

  label = new QLabel("Please use the Schedules tab to inspect this object.");
  label->setObjectName("IDFcomment");
  label->setWordWrap(true);
  layout->addWidget(label, 0, Qt::AlignTop);
  mainLayout->addLayout(layout);

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainLayout->addWidget(line);

  mainLayout->addStretch();

  m_libraryTabWidget->addTab(widget, "", "");
}

} // openstudio

