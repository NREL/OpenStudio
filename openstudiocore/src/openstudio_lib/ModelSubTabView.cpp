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

#include "ModelSubTabView.hpp"

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectItem.hpp"
#include "ModelObjectListView.hpp"
#include "ModelObjectTypeListView.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSItemSelector.hpp"
#include "OSItemSelectorButtons.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <iostream>

#include <QButtonGroup>
#include <QCalendarWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGridLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QRadioButton>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QStyleOption>
#include <QTimer>
#include <QVBoxLayout>

namespace openstudio {

ModelSubTabView::ModelSubTabView(OSItemSelector* itemSelector,
  ModelObjectInspectorView* modelObjectInspectorView,
  bool showGridViewLayout,
  QWidget * parent)
  : SubTabView(itemSelector, modelObjectInspectorView, showGridViewLayout, parent),
    m_modelObjectInspectorView(modelObjectInspectorView)
{
  connect(this, &ModelSubTabView::dropZoneItemClicked, this, &ModelSubTabView::onDropZoneItemClicked);
  connect(modelObjectInspectorView, &ModelObjectInspectorView::dropZoneItemClicked, this, &ModelSubTabView::dropZoneItemClicked);
}

ModelObjectInspectorView* ModelSubTabView::modelObjectInspectorView()
{
  return m_modelObjectInspectorView;
}

void ModelSubTabView::onDropZoneItemClicked(OSItem* item)
{ 
  std::shared_ptr<OSDocument> currentDocument = OSAppBase::instance()->currentDocument();
  if (currentDocument){
    if (!item) {
      emit dropZoneItemSelected(item, false);
    }
    else {
      // Note: perhaps passing this here offers more flexibility in the future when determining readOnly
      emit dropZoneItemSelected(item, item->itemId().isDefaulted());
    }
  }
}

} // openstudio
