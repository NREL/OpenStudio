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

#ifndef OPENSTUDIO_MODELOBJECTTREEITEMS_HPP
#define OPENSTUDIO_MODELOBJECTTREEITEMS_HPP

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include "../model/Space.hpp"
#include "OSItem.hpp"

#include <QObject>
#include <QTreeWidgetItem>
#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement

class QPushButton;
class QLabel;

namespace openstudio {

class OSItem;

namespace model {
  class ShadingSurfaceGroup;
  class Building;
  class BuildingStory;
  class ThermalZone;
  class SpaceType;
  class Space;
}

class ModelObjectTreeItem : public QObject, public QTreeWidgetItem, public Nano::Observer
{
  Q_OBJECT

  public:

    /// Constructed with a modelObject this tree item represents that object
    ModelObjectTreeItem(const openstudio::model::ModelObject& modelObject, bool isDefaulted, OSItemType type, QTreeWidgetItem* parent = nullptr );

    /// Constructed with no modelObject this tree item represents a container
    ModelObjectTreeItem(const std::string& name, const openstudio::model::Model& model, QTreeWidgetItem* parent = nullptr );

    virtual ~ModelObjectTreeItem();

    boost::optional<openstudio::Handle> handle() const;

    boost::optional<openstudio::model::ModelObject> modelObject() const;

    openstudio::model::Model model() const;

    std::string name() const;

    OSItem* item() const;

    std::vector<ModelObjectTreeItem*> children() const;

    std::vector<ModelObjectTreeItem*> recursiveChildren() const;

    void setStyle(int headerLevel, const QString& color);

    bool isDirty();

    void makeDirty();

  public slots:

    void refresh();

    void refreshTree();

    void change();

    void changeRelationship(int index, Handle newHandle, Handle oldHandle);

  protected:

    // make all child items 
    void makeChildren();

    // get any non-model object children that this item should have
    virtual std::vector<std::string> nonModelObjectChildren() const;

    // get any model object children that this item should have
    virtual std::vector<model::ModelObject> modelObjectChildren() const;

    // get any defaulted model object children that this item should have
    virtual std::vector<model::ModelObject> defaultedModelObjectChildren() const;

    // add a non-model object as a child
    virtual void addNonModelObjectChild(const std::string& child);

    // add a model object as a child
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted);

    // called after makeChildren or refresh
    virtual void finalize();

    static const OSItemType m_type;

    static OSItemType initializeOSItemType();

  private slots:

    void changeName();

  private:

    boost::optional<openstudio::Handle> m_handle;
    boost::optional<openstudio::model::ModelObject> m_modelObject;
    openstudio::model::Model m_model;
    std::string m_name;
    OSItem* m_item;
    bool m_dirty;
};

///////////////////// SiteShading ////////////////////////////////////////////////

class SiteShadingTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    SiteShadingTreeItem(const openstudio::model::Model& model, 
                        QTreeWidgetItem* parent = nullptr );
    virtual ~SiteShadingTreeItem() {}
    static std::string itemName();
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted) override;
};

///////////////////// ShadingSurfaceGroup ////////////////////////////////////////////////

class ShadingSurfaceGroupTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    ShadingSurfaceGroupTreeItem(const openstudio::model::ShadingSurfaceGroup& shadingSurfaceGroup, 
                                QTreeWidgetItem* parent = nullptr );
    virtual ~ShadingSurfaceGroupTreeItem() {}
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted) override;
};

///////////////////// Building ////////////////////////////////////////////////

class BuildingTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    BuildingTreeItem(const openstudio::model::Building& building, 
                     const openstudio::IddObjectType& sortByType,
                     QTreeWidgetItem* parent = nullptr );
    virtual ~BuildingTreeItem() {}
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted) override;
    virtual std::vector<std::string> nonModelObjectChildren() const override;
    virtual void addNonModelObjectChild(const std::string& child) override;
  private:
    openstudio::IddObjectType m_sortByType;
};

///////////////////// BuildingShading ////////////////////////////////////////////////

class BuildingShadingTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    BuildingShadingTreeItem(const openstudio::model::Model& model, 
                            QTreeWidgetItem* parent = nullptr );
    virtual ~BuildingShadingTreeItem() {}
    static std::string itemName();
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted) override;
};

///////////////////// BuildingStory ////////////////////////////////////////////////

class BuildingStoryTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    BuildingStoryTreeItem(const openstudio::model::BuildingStory& buildingStory, 
                          QTreeWidgetItem* parent = nullptr );
    virtual ~BuildingStoryTreeItem() {}
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted) override;
};

class NoBuildingStoryTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    NoBuildingStoryTreeItem(const openstudio::model::Model& model, 
                            QTreeWidgetItem* parent = nullptr );
    virtual ~NoBuildingStoryTreeItem() {}
    static std::string itemName();
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted) override;
    virtual void finalize() override;
};

///////////////////// ThermalZone ////////////////////////////////////////////////

class ThermalZoneTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    ThermalZoneTreeItem(const openstudio::model::ThermalZone& thermalZone, 
                        QTreeWidgetItem* parent = nullptr );
    virtual ~ThermalZoneTreeItem() {}
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted) override;
};

class NoThermalZoneTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    NoThermalZoneTreeItem(const openstudio::model::Model& model, 
                          QTreeWidgetItem* parent = nullptr );
    virtual ~NoThermalZoneTreeItem() {}
    static std::string itemName();
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted) override;
    virtual void finalize() override;
};

///////////////////// SpaceType ////////////////////////////////////////////////

class SpaceTypeTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    SpaceTypeTreeItem(const openstudio::model::SpaceType& spaceType, 
                      QTreeWidgetItem* parent = nullptr );
    virtual ~SpaceTypeTreeItem() {}
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual std::vector<model::ModelObject> defaultedModelObjectChildren() const override;
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted) override;
};

class NoSpaceTypeTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    NoSpaceTypeTreeItem(const openstudio::model::Model& model, 
                        QTreeWidgetItem* parent = nullptr );
    virtual ~NoSpaceTypeTreeItem() {}
    static std::string itemName();
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted) override;
    virtual void finalize() override;
};

///////////////////// Space ////////////////////////////////////////////////

class SpaceTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    SpaceTreeItem(const openstudio::model::Space& space, QTreeWidgetItem* parent = nullptr );
    virtual ~SpaceTreeItem() {}
  protected:
    virtual std::vector<std::string> nonModelObjectChildren() const override;
    virtual void addNonModelObjectChild(const std::string& child) override;
};

///////////////////// Roofs ////////////////////////////////////////////////

class RoofsTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    RoofsTreeItem(const openstudio::model::Space& space, QTreeWidgetItem* parent = nullptr );
    virtual ~RoofsTreeItem() {}
    static std::string itemName();
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted) override;
  private:
    openstudio::model::Space m_space;
};

///////////////////// Walls ////////////////////////////////////////////////

class WallsTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    WallsTreeItem(const openstudio::model::Space& space, QTreeWidgetItem* parent = nullptr );
    virtual ~WallsTreeItem() {}
    static std::string itemName();
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted) override;
  private:
    openstudio::model::Space m_space;
};

///////////////////// Floors ////////////////////////////////////////////////

class FloorsTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    FloorsTreeItem(const openstudio::model::Space& space, QTreeWidgetItem* parent = nullptr );
    virtual ~FloorsTreeItem() {}
    static std::string itemName();
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted) override;
  private:
    openstudio::model::Space m_space;
};

///////////////////// Surface ////////////////////////////////////////////////

class SurfaceTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    SurfaceTreeItem(const openstudio::model::Surface& surface, QTreeWidgetItem* parent = nullptr );
    virtual ~SurfaceTreeItem() {}
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
};

///////////////////// SpaceShading ////////////////////////////////////////////////

class SpaceShadingTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    SpaceShadingTreeItem(const openstudio::model::Space& space, QTreeWidgetItem* parent = nullptr );
    virtual ~SpaceShadingTreeItem() {}
    static std::string itemName();
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted) override;
  private:
    openstudio::model::Space m_space;
};

///////////////////// InteriorPartitions ////////////////////////////////////////////////

class InteriorPartitionsTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    InteriorPartitionsTreeItem(const openstudio::model::Space& space, QTreeWidgetItem* parent = nullptr );
    virtual ~InteriorPartitionsTreeItem() {}
    static std::string itemName();
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual void addModelObjectChild(const model::ModelObject& child, bool isDefaulted) override;
  private:
    openstudio::model::Space m_space;
};

///////////////////// InteriorPartitionSurfaceGroup ////////////////////////////////////////////////

class InteriorPartitionSurfaceGroupTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    InteriorPartitionSurfaceGroupTreeItem(const openstudio::model::InteriorPartitionSurfaceGroup& interiorPartitionSurfaceGroup, 
                                          QTreeWidgetItem* parent = nullptr );
    virtual ~InteriorPartitionSurfaceGroupTreeItem() {}
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
};

///////////////////// DaylightingObjects ////////////////////////////////////////////////

class DaylightingObjectsTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    DaylightingObjectsTreeItem(const openstudio::model::Space& space, 
                               QTreeWidgetItem* parent = nullptr );
    virtual ~DaylightingObjectsTreeItem() {}
    static std::string itemName();
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
  private:
    openstudio::model::Space m_space;
};


///////////////////// Loads ////////////////////////////////////////////////

class LoadsTreeItem : public ModelObjectTreeItem
{
  Q_OBJECT

  public:
    LoadsTreeItem(const openstudio::model::Space& space, QTreeWidgetItem* parent = nullptr );
    virtual ~LoadsTreeItem() {}
    static std::string itemName();
  protected:
    virtual std::vector<model::ModelObject> modelObjectChildren() const override;
    virtual std::vector<model::ModelObject> defaultedModelObjectChildren() const override;
  private:
    openstudio::model::Space m_space;
};


} // openstudio

#endif // OPENSTUDIO_MODELOBJECTTREEITEMS_HPP

