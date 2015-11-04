# File of parsing functions.

# takes namespace string like "standards::interface", and returns Array of individual 
# namespace strings. 
def ParseNamespaces(namespaceString)

  regexNamespace = Regexp.new(/(\w*)(::)?/)
  
  temp = namespaceString.split(regexNamespace)
  namespaceArray = Array.new()
  
  for i in 0..(temp.size()-1)
    namespaceMatch = regexNamespace.match(temp[i])
    if not namespaceMatch[1].empty?
      namespaceArray.push(namespaceMatch[1])
    end
  end
  
  return namespaceArray

end