<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="xml" encoding="UTF-8" indent="yes" />

<xsl:template match="/">
<ptnet>
	<xsl:apply-templates select="/pnml/net/place" />
	<xsl:apply-templates select="/pnml/net/transition" />
	<xsl:apply-templates select="/pnml/net/arc" />
</ptnet>
</xsl:template>

<xsl:template match="/pnml/net/place">
<place>
	<xsl:attribute name="id"><xsl:value-of select="@id" /></xsl:attribute>
	<xsl:attribute name="name"><xsl:value-of select="name/value" /></xsl:attribute>
	<xsl:attribute name="x"><xsl:value-of select="graphics/position/@x" /></xsl:attribute>
	<xsl:attribute name="y"><xsl:value-of select="graphics/position/@y" /></xsl:attribute>
	<xsl:attribute name="tokens"><xsl:value-of select="initialMarking/value" /></xsl:attribute>
</place>
</xsl:template>

<xsl:template match="/pnml/net/transition">
<transition>
	<xsl:attribute name="id"><xsl:value-of select="@id" /></xsl:attribute>
	<xsl:attribute name="name"><xsl:value-of select="name/value" /></xsl:attribute>
	<xsl:attribute name="x"><xsl:value-of select="graphics/position/@x" /></xsl:attribute>
	<xsl:attribute name="y"><xsl:value-of select="graphics/position/@y" /></xsl:attribute>
	<xsl:attribute name="rotation"><xsl:value-of select="orientation/value" /></xsl:attribute>
</transition>
</xsl:template>

<xsl:template match="/pnml/net/arc">
<arc>
	<xsl:attribute name="id"><xsl:value-of select="@id" /></xsl:attribute>
	<xsl:attribute name="source"><xsl:value-of select="@source" /></xsl:attribute>
	<xsl:attribute name="target"><xsl:value-of select="@target" /></xsl:attribute>
	<xsl:attribute name="weight"><xsl:value-of select="inscription/value" /></xsl:attribute>
	<xsl:for-each select="arcpath[position() > 1 and position() &lt; last()]">
		<arcpath>
			<xsl:attribute name="x"><xsl:value-of select="@x"/></xsl:attribute>
			<xsl:attribute name="y"><xsl:value-of select="@y"/></xsl:attribute>
		</arcpath>
	</xsl:for-each>
</arc>
</xsl:template>

</xsl:stylesheet>
