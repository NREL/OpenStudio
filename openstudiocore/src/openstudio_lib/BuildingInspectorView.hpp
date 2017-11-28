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

#ifndef OPENSTUDIO_BUILDINGINSPECTORVIEW_HPP
#define OPENSTUDIO_BUILDINGINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"

#include "../model/Building.hpp"

class QColor;
class QComboBox;
class QLabel;
class QPushButton;

namespace openstudio {

class OSComboBox2;
class OSDropZone;
class OSIntegerEdit2;
class OSLineEdit2;
class OSQuantityEdit2;
class OSQuantityEdit2;
class OSSwitch2;

class BuildingSpaceTypeVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~BuildingSpaceTypeVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;

  virtual std::vector<OSItemId> makeVector() override;

  virtual void onRemoveItem(OSItem* item) override;

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;

  virtual void onDrop(const OSItemId& itemId) override;
};

class BuildingDefaultConstructionSetVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~BuildingDefaultConstructionSetVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;

  virtual std::vector<OSItemId> makeVector() override;

  virtual void onRemoveItem(OSItem* item) override;

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;

  virtual void onDrop(const OSItemId& itemId) override;
};

class BuildingDefaultScheduleSetVectorController : public ModelObjectVectorController
{
  Q_OBJECT

public:

  virtual ~BuildingDefaultScheduleSetVectorController() {}

protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;

  virtual std::vector<OSItemId> makeVector() override;

  virtual void onRemoveItem(OSItem* item) override;

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;

  virtual void onDrop(const OSItemId& itemId) override;
};

class BuildingInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    BuildingInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = nullptr );

    virtual ~BuildingInspectorView() {}

  signals:

  protected:

    virtual void onClearSelection() override;

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;

    virtual void onUpdate() override;

  protected slots:

    void editStandardsBuildingType(const QString & text);

    void standardsBuildingTypeChanged(const QString & text);

  private:

    void attach(openstudio::model::Building& building);

    void detach();

    void populateStandardsBuildingTypes();

    boost::optional<openstudio::model::Building> m_building;
    BuildingDefaultConstructionSetVectorController* m_defaultConstructionSetVectorController = nullptr;
    BuildingDefaultScheduleSetVectorController* m_defaultScheduleSetVectorController = nullptr;
    BuildingSpaceTypeVectorController* m_spaceTypeVectorController = nullptr;
    OSDropZone* m_defaultConstructionSetDropZone = nullptr;
    OSDropZone* m_defaultScheduleSetDropZone = nullptr;
    OSDropZone* m_spaceTypeDropZone = nullptr;
    OSIntegerEdit2* m_numberAboveGroundStories = nullptr;
    OSIntegerEdit2* m_numberLivingUnits = nullptr;
    OSIntegerEdit2* m_numberStories = nullptr;
    OSLineEdit2* m_nameEdit = nullptr;
    OSQuantityEdit2* m_northAxisEdit = nullptr;
    OSQuantityEdit2 * m_floorToCeilingHeight = nullptr;
    OSQuantityEdit2 * m_floorToFloorHeight = nullptr;
    OSSwitch2* m_relocatable = nullptr;
    QComboBox* m_standardsBuildingTypeComboBox = nullptr;
    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP) override;
};


} // openstudio

#endif // OPENSTUDIO_BUILDINGSTORYINSPECTORVIEW_HPP

