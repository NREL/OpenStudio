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

#include "SpaceTypesView.hpp"

#include "ModelObjectListView.hpp"
#include "OSItem.hpp"
#include "SpaceTypeInspectorView.hpp"

#include "../openstudio_lib/OSItem.hpp"

#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QStyleOption>
#include <QVBoxLayout>

#include <sstream>

#include <utilities/idd/IddEnums.hxx>


namespace openstudio {


  SpaceTypesView::SpaceTypesView(bool isIP,
    const openstudio::model::Model& model,
    QWidget * parent)
  : ModelSubTabView(new ModelObjectListView(IddObjectType::OS_SpaceType, model, true, parent),
                    new SpaceTypeInspectorView(isIP, model, parent),
                    true, // Note: "true" creates a GridView SubTabView
                    parent)
{
  ModelObjectListView* modelObjectListView = qobject_cast<ModelObjectListView*>(this->itemSelector());
  OS_ASSERT(modelObjectListView);
  modelObjectListView->setItemsDraggable(false);

  bool isConnected = false;

  isConnected = connect(itemSelector(), SIGNAL(selectionCleared()), inspectorView(), SIGNAL(selectionCleared()));
  OS_ASSERT(isConnected);

  isConnected = connect(inspectorView(), SIGNAL(dropZoneItemClicked(OSItem*)), this, SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);
}

} // openstudio

