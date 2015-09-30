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

class SummaryTableObjectsAttributesRelationships < OpenStudio::Ruleset::ModelUserScript

  def name
    return "Summary Table: Objects, Attributes, and Relationships"
  end
  
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new
    
    csvPathArg = OpenStudio::Ruleset::OSArgument::makePathArgument("csvPath",false,"csv")
    csvPathArg.setDefaultValue(OpenStudio::Path.new)
    result.push(csvPathArg)
    
    overwriteArg = OpenStudio::Ruleset::OSArgument::makeBoolArgument("overwrite")
    overwriteArg.setDefaultValue(true)
    result.push(overwriteArg)
    
    return result;
  end
  
  def run(model,runner,arguments) 
  
    csvPath = arguments["csvPath"]
    overwrite = arguments["overwrite"]
    
    table = OpenStudio::Table.new
    opts = OpenStudio::TableLoadOptions.new(false,false,false)
    row = []
    
    # Table Header
    row.push("Object Type")
    row.push("Number of Objects")
    row.push("Attributes")
    row.push("Settable")
    row.push("Initialized Count")
    row.push("Relationships")
    row.push("Settable")
    row.push("Initialized Count")
    table.appendRow(row,opts)
    row = []
    
    # Get Objects Sorted by Type
    wmodel = model.clone          # do not touch order of actual model
    wmodel.order.setOrderByIddEnum
    objects = model.modelObjects(true)
    
    # Populate Rows
    i = 0
    n = objects.size
    typeStart = 0
    while i < n
    
      # Bound Object Type
      typeCount = 1
      while (i < n-1) and (objects[i+1].iddObject == objects[i].iddObject)
        typeCount += 1
        i += 1
      end
      
      puts "Gathering attribute and relationship information on " + objects[typeStart].iddObject.name + "."
      
      # Add Object Type Information to the First Row
      row.push(objects[typeStart].iddObject.type.valueDescription)
      row.push(typeCount.to_s)
      
      # Gather Attribute Information
      attributeInfo = []
      objects[typeStart].attributeNames.each { |attribute|
        settable = false
        setCount = 0
        for j in typeStart..i
          settable = objects[j].isSettableAttribute(attribute) if j == typeStart
          if not objects[j].getAttribute(attribute).empty?
            setCount += 1
          end
        end
        attributeInfo.push([attribute, settable, setCount])
      }
      
      # Gather Relationship Information
      relationshipInfo = []
      objects[typeStart].relationshipNames.each { |relationship|
        settable = false
        setCount = 0
        for j in typeStart..i
          r = objects[j].getRelationship(relationship).get
          settable = r.isSettable if j == typeStart
          if r.isSingular
            if not r.relatedModelObject.empty?
              setCount += 1
            end
          else
            if not r.relatedModelObjects.empty?
              setCount += 1
            end
          end
        end
        relationshipInfo.push([relationship, settable, setCount])
      }
      
      # Add Rows to Table
      nr = [attributeInfo.size,relationshipInfo.size].max
      for j in 0..(nr-1)
        if j < attributeInfo.size
          row.push(attributeInfo[j][0])
          row.push(attributeInfo[j][1].to_s)
          row.push(attributeInfo[j][2].to_s)
        else
          row.push("")
          row.push("")
          row.push("")
        end
        
        if j < relationshipInfo.size
          row.push(relationshipInfo[j][0])
          row.push(relationshipInfo[j][1].to_s)
          row.push(relationshipInfo[j][2].to_s)
        else
          row.push("")
          row.push("")
          row.push("")          
        end
        
        table.appendRow(row,opts)
        row = []
        row.push("")
        row.push("")
      end
      
      i += 1
      typeStart = i
    end
    
    
    # Save or Print
    
    csvPath.hasValue ? csvPath = csvPath.valueAsPath : csvPath = csvPath.defaultValueAsPath
    overwrite.hasValue ? overwrite = overwrite.valueAsBool : overwrite = overwrite.defaultValueAsBool
    
    saved = false
    if not csvPath.empty
      puts "Saving summary table to " + csvPath.to_s + "."
      saved = table.save(csvPath,overwrite)
      puts "Unable to save table. Outputting to screen." if not saved
    else
      puts "No csv file path specified. Outputting to screen."
    end
    
    if not saved
      puts table.print("CSV".to_TableFormat)
    end
    
  end

end

SummaryTableObjectsAttributesRelationships.new.registerWithSketchUp
