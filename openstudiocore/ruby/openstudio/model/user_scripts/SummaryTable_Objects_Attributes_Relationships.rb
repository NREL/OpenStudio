########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

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
