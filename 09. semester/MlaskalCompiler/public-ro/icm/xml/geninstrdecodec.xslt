<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text" version="1.0" encoding="UTF-8" indent="no"/>
	<xsl:template match="/">
		<xsl:text>/*</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GEN_INSTR_DECODE.CPP</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GENERATED, DO NOT MODIFY!!!!</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>*/</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>#include "icm_machine.hpp"</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>using namespace mlaskal;</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>namespace {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<!-- generate mops for instructions -->
		<xsl:for-each select="/icminstr/instrs/instr">
			<xsl:call-template name="gen_instr_code">
				<xsl:with-param name="instr" select="."/>
			</xsl:call-template>
		</xsl:for-each>
		<!-- generate array indexed by ICOP -->
		<xsl:call-template name="TAB"/>
		<xsl:text>instr_decode_::instr_code instr_decodes_int_[] = {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>inco_</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>,</xsl:text>
			<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>} // anonymous namespace</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>namespace mlaskal {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>namespace instr_decode_ {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>const carray&lt;instr_code&gt; instr_decodes(instr_decodes_int_);</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>} // namespace instr_decode_</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>} // namespace mlaskal</xsl:text>
		<xsl:call-template name="BR"/>
	</xsl:template>
	<xsl:template name="gen_instr_code">
		<xsl:param name="instr"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>instr_decode_::instr_mop inco_</xsl:text>
		<xsl:value-of select="$instr/name"/>
		<xsl:text>_int_[] = {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="$instr/mop_descr/mop">
			<xsl:call-template name="gen_instr_mop">
				<xsl:with-param name="mop" select="."/>
			</xsl:call-template>
		</xsl:for-each>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>const carray&lt;instr_decode_::instr_mop&gt; inco_</xsl:text>
		<xsl:value-of select="$instr/name"/>
		<xsl:text>(inco_</xsl:text>
		<xsl:value-of select="$instr/name"/>
		<xsl:text>_int_);</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
	</xsl:template>
	<xsl:template name="gen_instr_mop">
		<xsl:param name="mop"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>{ mops::MOPC_</xsl:text>
		<xsl:value-of select="$mop/mop_name"/>
		<xsl:text>,</xsl:text>
		<xsl:call-template name="TAB"/>
		<xsl:if test="string-length($mop/mop_name)&lt;=9">
			<xsl:call-template name="TAB"/>
		</xsl:if>
		<xsl:text>"</xsl:text>
		<xsl:value-of select="$mop/id"/>
		<xsl:text>",</xsl:text>
		<xsl:call-template name="TAB"/>
		<xsl:if test="string-length($mop/id)&lt;=4">
			<xsl:call-template name="TAB"/>
		</xsl:if>
		<xsl:choose>
			<xsl:when test="$mop/in1_id">
				<xsl:text>"</xsl:text>
				<xsl:value-of select="$mop/in1_id"/>
				<xsl:text>"</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>0</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>,</xsl:text>
		<xsl:call-template name="TAB"/>
		<xsl:if test="string-length($mop/in1_id)&lt;=4">
			<xsl:call-template name="TAB"/>
		</xsl:if>
		<xsl:choose>
			<xsl:when test="$mop/in2_id">
				<xsl:text>"</xsl:text>
				<xsl:value-of select="$mop/in2_id"/>
				<xsl:text>"</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>0</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>,</xsl:text>
		<xsl:call-template name="TAB"/>
		<xsl:if test="string-length($mop/in2_id)&lt;=4">
			<xsl:call-template name="TAB"/>
		</xsl:if>
		<xsl:choose>
			<xsl:when test="$mop/in3_id">
				<xsl:text>"</xsl:text>
				<xsl:value-of select="$mop/in3_id"/>
				<xsl:text>"</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>0</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:call-template name="TAB"/>
		<xsl:if test="string-length($mop/in3_id)&lt;=5">
			<xsl:call-template name="TAB"/>
		</xsl:if>
		<xsl:text> },</xsl:text>
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
