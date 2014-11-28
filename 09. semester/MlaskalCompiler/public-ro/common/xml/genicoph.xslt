<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text" version="1.0" encoding="UTF-8" indent="no"/>
	<xsl:template match="/">
		<xsl:text>/*</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GEN_ICOPCODES.HPP</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GENERATED, DO NOT MODIFY!!!!</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>*/</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>#ifndef GEN_ICOPCODES_HPP_</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>#define GEN_ICOPCODES_HPP_</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>namespace ai {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>enum IC_OPCODE {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>ICOP_</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>,</xsl:text>
			<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>ICOP_DNOTUSE_MAX_		/* the last member of enumeration */</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>} // namespace ai</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>#endif</xsl:text>
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
