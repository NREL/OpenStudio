######################################################################
#  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

require 'rexml/document'
require 'openstudio'

class SDDWriter

  attr_accessor :document, :root, :flatFormat, :printAttributes

  def initialize(flatFormat=false,printAttributes=false)
  
    @document = REXML::Document.new
    @root = document.add_element("sdd")
    @flatFormat = flatFormat
    @printAttributes = printAttributes
  
  end
  
  def addToDocument(node,modelObject)
  
    if @flatFormat
    
      result = node.add_element("ModelObject")
      
      result.add_attribute("Type",modelObject.iddObject.type.valueDescription)
      result.add_attribute("UUID",modelObject.handle.to_s)
      
      index = 0
      name = modelObject.name
      if not name.empty?
        index += 1
        result.add_attribute("Name",name.get)
      end
      
      for i in index..(modelObject.numFields - 1)
        tnode = result.add_element("Field")
        tnode.add_attribute("index",i.to_s)
        tnode.add_attribute("name",modelObject.iddObject.getField(i).get.name)
        target = modelObject.getTarget(i)
        if not target.empty?
          tnode.add_attribute("uuid")
          tnode.add_text(target.get.handle.to_s)
        else
          s = modelObject.getString(i,false,true)
          if not s.empty?
            tnode.add_text(s.get)
          end
        end
      end
      
      if @printAttributes
      
        tnode = result.add_element("Attributes")
      
        addAttributes(tnode,modelObject)
        
        tnode = result.add_element("Relationships")
        
        addRelationships(tnode,modelObject)
      
      end
   
    else
    
      result = node.add_element(modelObject.iddObject.type.valueDescription)
          
      addAttributes(result,modelObject)
    
      resources = modelObject.getRelationship("resources").get
      resourceObjects = resources.relatedModelObjects
      if resourceObjects.size > 0

        resourcesNode = result.add_element("Resources")
        resourceObjects.each { |resourceObject|
          addToDocument(resourcesNode,resourceObject)
        }

      end

      parentObject = modelObject.to_ParentObject
      if not parentObject.empty?
        children = parentObject.get.getRelationship("children").get
        childObjects = children.relatedModelObjects
        if childObjects.size > 0

          childrenNode = result.add_element("Children")
          childObjects.each { |childObject|
            addToDocument(childrenNode,childObject)
          }

        end
      end
      
      if not modelObject.to_Loop.empty?
      
        loop = modelObject.to_Loop.get
      
        # print supply components
        
        components = loop.supplyComponents
        if not components.empty?
          tnode = result.add_element("SupplyComponents")
          components.each { |component|
            addToDocument(tnode,component)
          }
        end
        
        # print demand components
        
        components = loop.demandComponents
        if not components.empty?
          tnode = result.add_element("DemandComponents")
          components.each { |component|
            addToDocument(tnode,component)
          }
        end        
      
      end
      
    end      
    
    return result
  
  end
  
  def addAttributes(node,modelObject)
  
    attributes = modelObject.attributeNames
    attributes.each { |attribute| 
      attributeNode = node.add_element(attribute)
      
      begin
        if modelObject.isSettableAttribute(attribute)
          attributeNode.add_attribute("settable")
        end
      
        attributeObject = modelObject.getAttribute(attribute)
        if not attributeObject.empty?
          attributeObject = attributeObject.get

          vt = attributeObject.valueType
          if vt == "Boolean".to_AttributeValueType
            val = attributeObject.valueAsBoolean
            if val
              attributeNode.add_text("true")
            else
              attributeNode.add_text("false")          
            end
          elsif vt == "Double".to_AttributeValueType
            attributeNode.add_text(attributeObject.valueAsDouble.to_s)
          elsif vt == "Integer".to_AttributeValueType
            attributeNode.add_text(attributeObject.valueAsInteger.to_s)        
          elsif vt == "Unsigned".to_AttributeValueType
            attributeNode.add_text(attributeObject.valueAsUnsigned.to_s)
          elsif vt == "String".to_AttributeValueType
            attributeNode.add_text(attributeObject.valueAsString)                
          end

          units = attributeObject.units
          if not units.empty?
            el = attributeNode.add_attribute("units",units.get)
          end
        end
      rescue
        next
      end
    }    
  
  end
  
  def addRelationships(node, modelObject)
  
    relationships = modelObject.relationshipNames
    relationships.each { |relationship|
    
      relationshipNode = node.add_element(relationship)
      
      begin
        
        relationshipObject = modelObject.getRelationship(relationship).get
        
        if relationshipObject.isSettable
          relationshipNode.add_attribute("settable")
        end
        
        objects = OpenStudio::Model::ModelObjectVector.new
        if relationshipObject.isSingular
          object = relationshipObject.relatedModelObject
          if not object.empty?
            objects = OpenStudio::Model::ModelObjectVector.new(1,object.get)
          end
        else
          objects = relationshipObject.relatedModelObjects
        end
        
        objects.each { |object|
          tnode = relationshipNode.add_element("ModelObject")
          tnode.add_attribute("Type",object.iddObject.type.valueDescription)
          tnode.add_attribute("UUID",object.handle.to_s)
          
          name = object.name
          if not name.empty?
            tnode.add_attribute("Name",name.get)
          end
        }
        
      rescue
        next
      end
    
    }
  
  end

end