#ifndef litesql_xmlparser_hpp
#define litesql_xmlparser_hpp

#include <string>

#include "expat.h"

namespace xml {
/** abstract base class for SAX2 Parser based on libxml2 */
class XmlParser {

public:
   virtual ~XmlParser();
   bool parseFile(const std::string& filename);
   
   static const XML_Char* xmlGetAttrValue(const XML_Char** attrs,const XML_Char* key);

protected:
   virtual void onStartElement(	const XML_Char *fullname,
                                 const XML_Char **atts)	= 0;
   virtual void onEndElement(	const XML_Char *fullname)   = 0;

private:
   XML_Parser saxHandler;

   friend void XMLParser_xmlSAX2StartElement		(void *ctx,
						 const XML_Char *fullname,
						 const XML_Char **atts);

   friend void XMLParser_xmlSAX2EndElement(void *ctx,const XML_Char *name);
};
}
#endif

