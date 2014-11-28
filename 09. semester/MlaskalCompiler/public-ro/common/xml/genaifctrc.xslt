<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text" version="1.0" encoding="UTF-8" indent="no"/>
	<xsl:template match="/">
		<xsl:text>/*</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GEN_AIFACTORY.CPP</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GENERATED, DO NOT MODIFY!!!!</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>*/</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>#include "ai_factory.hpp"</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>#include "abstract_instr.hpp"</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>namespace aifcr_int_ {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
			<xsl:call-template name="TAB"/>
			<xsl:text>static mlaskal::abstract_instr *aifcr_</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>(mlaskal::ICREGNUM out, mlaskal::ICREGNUM in1, mlaskal::ICREGNUM in2, mlaskal::ICREGNUM in3, mlaskal::ICREGNUM in4) {</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>return new ai::</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>(out, in1, in2, in3, in4);</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>}</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:text>}</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>namespace aifcl_int_ {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
			<xsl:call-template name="TAB"/>
			<xsl:text>static mlaskal::abstract_instr *aifcl_</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>(const mlaskal::abstract_instr *</xsl:text>
			<xsl:if test="./reg_out or ./operand or ./reg_in1">
				<xsl:text>oldai</xsl:text>
			</xsl:if>
			<xsl:text>) {</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>return new ai::</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>(</xsl:text>
			<xsl:if test="./reg_out">
				<xsl:text>oldai->get_out()</xsl:text>
			</xsl:if>
			<xsl:if test="./operand">
				<xsl:if test="./reg_out">
					<xsl:text>, </xsl:text>
				</xsl:if>
			</xsl:if>
			<xsl:choose>
				<xsl:when test="./operand='LITINT'">
					<xsl:text>oldai->get_litint()</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='LITREAL'">
					<xsl:text>oldai->get_litreal()</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='LITSTR'">
					<xsl:text>oldai->get_litstr()</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='ABSNUM'">
					<xsl:text>oldai->get_absnum()</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='GSTKOFFS' or ./operand='LSTKOFFS' or ./operand='SSTKOFFS'">
					<xsl:text>oldai->get_stkoffs()</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='RELJUMP'">
					<xsl:text>oldai->get_reljump()</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='ABSJUMP'">
					<xsl:text>oldai->get_absjump()</xsl:text>
				</xsl:when>
			</xsl:choose>
			<xsl:if test="./reg_in1">
				<xsl:if test="./operand or ./reg_out">
					<xsl:text>, </xsl:text>
				</xsl:if>
				<xsl:text>oldai->get_in1()</xsl:text>
			</xsl:if>
			<xsl:if test="./reg_in2">
				<xsl:text>, oldai->get_in2()</xsl:text>
			</xsl:if>
			<xsl:if test="./reg_in3">
				<xsl:text>, oldai->get_in3()</xsl:text>
			</xsl:if>
			<xsl:if test="./reg_in4">
				<xsl:text>, oldai->get_in4()</xsl:text>
			</xsl:if>
			<xsl:text>);</xsl:text>
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
    <xsl:text>ai_factory_creator_type ai_factory::creators_[ai::ICOP_DNOTUSE_MAX_]={</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
			<xsl:call-template name="TAB"/>
			<xsl:text>aifcr_int_::aifcr_</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>,</xsl:text>
			<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>ai_factory_cloner_type ai_factory::cloners_[ai::ICOP_DNOTUSE_MAX_]={</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
			<xsl:call-template name="TAB"/>
			<xsl:text>aifcl_int_::aifcl_</xsl:text>
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
