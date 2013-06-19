<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:fn="http://www.w3.org/2005/xpath-functions">
  
<xsl:output method="html" doctype-public="-//W3C//DTD XHTML 1.0 Transitional//EN" doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd" />

<xsl:template match="/">

  <html>
    <head xmlns="http://www.w3.org/1999/xhtml">
      <title>Prototype Data Dictionary</title>
    </head>
    <body>
      <xsl:for-each select="DataDictionary/element()">
      
        <h1><xsl:value-of select="name()"/></h1>
        <h2>Entries</h2>
        
          <xsl:for-each select="Term">
          <xsl:sort select="name"/>
          <p>
          <b><xsl:value-of select="name"/><xsl:text>. </xsl:text></b><xsl:value-of select="definition"/><xsl:text>. </xsl:text>
          <b><xsl:value-of select="type/element()/name()"/></b>
          <xsl:if test="type/*/element(min)">, min = <xsl:value-of select="type/*/min"/></xsl:if>
          <xsl:if test="type/*/element(max)">, max = <xsl:value-of select="type/*/max"/></xsl:if>
          <xsl:if test="type/*/element(units)">, units = <xsl:value-of select="type/*/units"/></xsl:if><xsl:text>. </xsl:text>
          <xsl:for-each select="relationships/element()">
            <i><xsl:value-of select="name()"/>:</i> 
            <xsl:for-each select="name">
              <xsl:text> </xsl:text><xsl:value-of select="."/> 
            </xsl:for-each>
          <xsl:text>. </xsl:text>
          </xsl:for-each>
          </p>
          </xsl:for-each> 
          
        <xsl:if test="count(Term/type/enumeration/value)>0">
        <h2>Enumerations</h2>
        </xsl:if>
        
        <xsl:for-each select="Term">
        <xsl:sort select="name"/>
        <xsl:if test="type/enumeration/element(value)">
          <p>
          <b><xsl:value-of select="name"/>:</b> 
          <xsl:for-each select="type/enumeration/value">
          <xsl:text> </xsl:text><xsl:value-of select="."/>;
          </xsl:for-each>
          </p>
        </xsl:if>
        </xsl:for-each>
        
      </xsl:for-each>
    </body>
  </html>
</xsl:template>
</xsl:stylesheet>
