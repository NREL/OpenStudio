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

#include "ThermalZonesView.hpp"

#include "ModelObjectListView.hpp"
#include "ThermalZonesGridView.hpp"

#include <QPainter>
#include <QStackedWidget>
#include <QStyleOption>
#include <QTimer>
#include <QVBoxLayout>

#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

ThermalZonesView::ThermalZonesView(bool isIP,
  const model::Model & model,
  QWidget * parent)
  : ModelSubTabView(new ModelObjectListView(IddObjectType::OS_ThermalZone,model,true,parent),
                    new ThermalZoneView(isIP,model,parent),
                    true,
                    parent)
{
  bool isConnected = false;

  isConnected = connect(itemSelector(), SIGNAL(selectionCleared()), inspectorView(), SIGNAL(selectionCleared()));
  OS_ASSERT(isConnected);

  isConnected = connect(inspectorView(), SIGNAL(dropZoneItemClicked(OSItem*)), this, SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);
}

ThermalZoneView::ThermalZoneView(bool isIP,
  const model::Model & model,
  QWidget * parent )
  : ModelObjectInspectorView(model,true,parent),
    m_isIP(isIP)
{
  bool isConnected = false;

  m_thermalZonesGridView = new ThermalZonesGridView(this->m_isIP,this->m_model,this);
  this->stackedWidget()->addWidget(m_thermalZonesGridView);

  isConnected = connect(m_thermalZonesGridView, SIGNAL(dropZoneItemClicked(OSItem*)), this, SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(selectionCleared()), m_thermalZonesGridView, SIGNAL(selectionCleared()));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), this, SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_thermalZonesGridView, SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  refresh();
}

std::vector<model::ModelObject> ThermalZoneView::selectedObjects() const
{
  return m_thermalZonesGridView->selectedObjects();
}

void ThermalZoneView::onClearSelection()
{}

void ThermalZoneView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{}

void ThermalZoneView::onUpdate()
{}

void ThermalZoneView::refresh()
{}

void ThermalZoneView::toggleUnits(bool isIP)
{
  m_isIP = isIP;
}

} // openstudio
