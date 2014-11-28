<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text" version="1.0" encoding="UTF-8" indent="no"/>
	<xsl:template match="/">
		<xsl:text>/*</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GEN_ICINSTRTAB.CPP</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GENERATED, DO NOT MODIFY!!!!</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>*/</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>#include "ic_instrtab.hpp"</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>namespace ai {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>mlaskal::ic_instrtab_type ic_instrtab[ai::ICOP_DNOTUSE_MAX_] = {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
			<xsl:call-template name="TAB"/>
			<xsl:text>{ ai::ICOP_</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>, "</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>"</xsl:text>
			<xsl:call-template name="TAB"/>
			<xsl:text>, </xsl:text>
			<xsl:choose>
				<xsl:when test="./operand">
					<xsl:text>mlaskal::ICSPT_</xsl:text>
					<xsl:value-of select="./operand"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>mlaskal::ICSPT_NONE</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text>, </xsl:text>
			<xsl:call-template name="prt_ptype">
				<xsl:with-param name="ptype" select="./reg_in1/type"/>
			</xsl:call-template>
			<xsl:text>, </xsl:text>
			<xsl:call-template name="prt_ptype">
				<xsl:with-param name="ptype" select="./reg_in2/type"/>
			</xsl:call-template>
			<xsl:text>, </xsl:text>
			<xsl:call-template name="prt_ptype">
				<xsl:with-param name="ptype" select="./reg_in3/type"/>
			</xsl:call-template>
			<xsl:text>, </xsl:text>
			<xsl:call-template name="prt_ptype">
				<xsl:with-param name="ptype" select="./reg_in4/type"/>
			</xsl:call-template>
			<xsl:text>, </xsl:text>
			<xsl:call-template name="prt_ptype">
				<xsl:with-param name="ptype" select="./reg_out/type"/>
			</xsl:call-template>
			<xsl:text> }, </xsl:text>
			<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>}</xsl:text>
		<xsl:call-template name="BR"/>
	</xsl:template>
	<xsl:template name="prt_ptype">
		<xsl:param name="ptype"/>
		<xsl:choose>
			<xsl:when test="$ptype">
				<xsl:text>mlaskal::PTYPE_</xsl:text>
				<xsl:choose>
					<xsl:when test="$ptype='N'">
						<xsl:text>UNDEF</xsl:text>
					</xsl:when>
					<xsl:when test="$ptype='B'">
						<xsl:text>BOOL</xsl:text>
					</xsl:when>
					<xsl:when test="$ptype='I'">
						<xsl:text>INTEGER</xsl:text>
					</xsl:when>
					<xsl:when test="$ptype='S'">
						<xsl:text>STRING</xsl:text>
					</xsl:when>
					<xsl:when test="$ptype='A'">
						<xsl:text>ANY</xsl:text>
					</xsl:when>
					<xsl:when test="$ptype='P'">
						<xsl:text>ADDR</xsl:text>
					</xsl:when>
					<xsl:when test="$ptype='R'">
						<xsl:text>REAL</xsl:text>
					</xsl:when>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>mlaskal::PTYPE_UNDEF</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
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
