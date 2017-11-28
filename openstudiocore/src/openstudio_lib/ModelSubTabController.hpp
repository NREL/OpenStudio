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

#ifndef OPENSTUDIO_MODELSUBTABCONTROLLER_HPP
#define OPENSTUDIO_MODELSUBTABCONTROLLER_HPP

#include "SubTabController.hpp"
#include "ModelSubTabView.hpp"

#include "../model/Model.hpp"

#include <QObject>

namespace openstudio {

class OSItem;
class OSItemId;
class SubTabView;

namespace model {
  class Component;
}

class ModelSubTabController : public SubTabController
{
  Q_OBJECT

  public:

    ModelSubTabController(ModelSubTabView* subTabView,
                     const model::Model & model);

    virtual ~ModelSubTabController() {}

  signals:

    void modelObjectSelected(model::OptionalModelObject & modelObject, bool readOnly);

    void dropZoneItemSelected(OSItem* item, bool readOnly);

    void dropZoneItemClicked(OSItem* item);

    void toggleUnitsClicked(bool displayIP);

  protected:

    openstudio::model::Model model() const;

    bool fromModel(const OSItemId& itemId) const;

    bool fromComponentLibrary(const OSItemId& itemId) const;

    boost::optional<model::ModelObject> getModelObject(const OSItemId& itemId) const;

    boost::optional<model::Component> getComponent(const OSItemId& itemId) const;

    virtual void onAddObject(const openstudio::IddObjectType& iddObjectType) = 0;

    virtual void onAddObject(const openstudio::model::ModelObject& modelObject) {}

    virtual void onCopyObject(const openstudio::model::ModelObject& modelObject) = 0;

    virtual void onRemoveObject(openstudio::model::ModelObject modelObject) = 0;

    virtual void onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId) = 0;

    virtual void onPurgeObjects(const openstudio::IddObjectType& iddObjectType) = 0;

    virtual void onDrop(const OSItemId& itemId) override = 0;

    virtual void onAddItem() override;

    virtual void onCopyItem() override;

    virtual void onRemoveItem(OSItem *item) override;

    virtual void onReplaceItem(OSItem *item, const OSItemId& replacementItemId) override;

    virtual void onPurgeItems() override;

  private slots:

  private:

    boost::optional<openstudio::model::ModelObject> selectedModelObject() const;

    openstudio::IddObjectType currentIddObjectType() const;

    openstudio::model::Model m_model;
};

} // openstudio

#endif // OPENSTUDIO_MODELSUBTABCONTROLLER_HPP
