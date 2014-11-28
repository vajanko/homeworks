<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text" version="1.0" encoding="UTF-8" indent="no"/>
	<xsl:template match="/">
		<xsl:text>/*</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GEN_ICFACTORY.CPP</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GENERATED, DO NOT MODIFY!!!!</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>*/</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>#include "common_types.hpp"</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>#include "ic_opcodes.hpp"</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>#include "ic_factory.hpp"</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>#include "ic_instr.hpp"</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<!--<xsl:text>namespace mlaskal {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>-->
		<xsl:text>namespace icfcr_int_ {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
			<xsl:call-template name="TAB"/>
			<xsl:text>static mlaskal::ic_instr *icfcr_</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>(ai::IC_OPCODE opc, mlaskal::ICRELNUM n, mlaskal::ICREGNUM in1, mlaskal::ICREGNUM in2, mlaskal::ICREGNUM in3, mlaskal::ICREGNUM in4, mlaskal::ICREGNUM out) {</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>return new icinstr_int_::</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>(opc, n, in1, in2, in3, in4, out);</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>}</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:text>}</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
    <xsl:text>namespace mlaskal {</xsl:text>
    <xsl:call-template name="BR"/>
    <xsl:call-template name="BR"/>
    <xsl:text>ic_factory_creator_type ic_factory::creators_[ai::ICOP_DNOTUSE_MAX_]={</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
			<xsl:call-template name="TAB"/>
			<xsl:text>icfcr_int_::icfcr_</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>,</xsl:text>
			<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>}</xsl:text>
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
