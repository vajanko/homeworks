<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text" version="1.0" encoding="UTF-8" indent="no"/>
	<xsl:template match="/">
		<xsl:text>/*</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GEN_MOPCODES.HPP</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GENERATED, DO NOT MODIFY!!!!</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>*/</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>namespace mlaskal {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>namespace mops {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<!-- generate MOP enum -->
		<xsl:call-template name="TAB"/>
		<xsl:text>enum MOP_MOPCODE {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/mops/mop">
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>MOPC_</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>,</xsl:text>
			<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>MOPC_DNOTUSE_MAX_</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<!-- generate unit types enum -->
		<xsl:call-template name="TAB"/>
		<xsl:text>enum UNIT_TYPE {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/unit_types/unit_type">
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>UT_</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>,</xsl:text>
			<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>} // namespace mops</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>} // namespace mlaskal</xsl:text>
		<xsl:call-template name="BR"/>
	</xsl:template>
	<!-- Generates the newline -->
	<xsl:template name="BR">
		<xsl:value-of select="&quot;&#xA;&quot;"/>
	</xsl:template>
	<!-- Generates the tabelator -->
	<xsl:template name="TAB">
		<xsl:value-of select="&quot;&#x9;&quot;"/>
	</xsl:template>
</xsl:stylesheet>
