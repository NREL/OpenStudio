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

#ifndef OPENSTUDIO_SUBTABCONTROLLER_HPP
#define OPENSTUDIO_SUBTABCONTROLLER_HPP

#include "../shared_gui_components/OSQObjectController.hpp"

#include "../model/Model.hpp"

#include <QObject>

namespace openstudio {

class OSItem;
class OSItemId;
class SubTabView;

class SubTabController : public OSQObjectController
{
  Q_OBJECT

  public:

    SubTabController(SubTabView* subTabView);
    virtual ~SubTabController() {}

    SubTabView* subTabView();
    const SubTabView* subTabView() const;

  protected:

    virtual void onSelectItem(OSItem *item);

    virtual void onClearSelection();

    virtual void onAddItem() = 0;

    virtual void onCopyItem() = 0;

    virtual void onRemoveItem(OSItem *item) = 0;

    virtual void onReplaceItem(OSItem *item, const OSItemId& replacementItemId) = 0;

    virtual void onPurgeItems() = 0;

    virtual void onDrop(const OSItemId& itemId) = 0;

    virtual void onInspectItem(OSItem* item) = 0;

    SubTabView* m_subTabView;

signals:

    void downloadComponentsClicked();

    void openLibDlgClicked();

  public slots:

    void removeItem(OSItem* item);

  protected slots:

    void selectItem(OSItem* item);

  private slots:
  
    void replaceItem(OSItem* item, const OSItemId& replacementItemId);

    void clearSelection();

    void handleDrop(const OSItemId&);
  
    void doAdd();

    void doCopy();

    void doRemove();

    void doPurge();

    void inspectItem(OSItem* item);

  private:

    boost::optional<openstudio::model::ModelObject> selectedModelObject() const;

    openstudio::IddObjectType currentIddObjectType() const;

    openstudio::model::Model m_model;
};

} // openstudio

#endif // OPENSTUDIO_SUBTABCONTROLLER_HPP
