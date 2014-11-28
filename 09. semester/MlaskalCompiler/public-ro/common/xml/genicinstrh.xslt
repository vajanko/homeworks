<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text" version="1.0" encoding="UTF-8" indent="no"/>
	<xsl:template match="/">
		<xsl:text>/*</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GEN_ICINSTR.HPP</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GENERATED, DO NOT MODIFY!!!!</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>*/</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>#ifndef GEN_ICINSTR_HPP_</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>#define GEN_ICINSTR_HPP_</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<!--<xsl:text>namespace mlaskal {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>-->
		<xsl:text>namespace icinstr_int_ {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
			<xsl:call-template name="TAB"/>
			<xsl:text>class </xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text> : public mlaskal::ic_instr {</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>public:</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:value-of select="./name"/>
			<xsl:text>(ai::IC_OPCODE opc, mlaskal::ICRELNUM n, mlaskal::ICABSNUM in1, mlaskal::ICABSNUM in2, mlaskal::ICABSNUM in3, mlaskal::ICABSNUM in4, mlaskal::ICABSNUM out) : ic_instr(opc, n, in1, in2, in3, in4, out) { }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>			
			<xsl:text>};</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:text>} // namespace icinstr_int_</xsl:text>
		<!--<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>} // namespace mlaskal</xsl:text>-->
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
