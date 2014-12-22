<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text" version="1.0" encoding="UTF-8" indent="no"/>
	<xsl:template match="/">
		<xsl:text>/*</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GEN_MACHINES.CPP</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GENERATED, DO NOT MODIFY!!!!</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>*/</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>#include &lt;iostream&gt;</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>#include "icm_machine.hpp"</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>using namespace std;</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>using namespace mlaskal;</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>namespace {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<!-- generate units -->
		<xsl:for-each select="/icminstr/units/unit">
			<xsl:call-template name="gen_unit">
				<xsl:with-param name="unit" select="."/>
			</xsl:call-template>
		</xsl:for-each>
		<!-- generate machine classes-->
		<xsl:for-each select="/icminstr/machines/machine">
			<xsl:call-template name="gen_machine_class">
				<xsl:with-param name="machine" select="."/>
			</xsl:call-template>
		</xsl:for-each>
		<!-- generate units -->
		<xsl:for-each select="/icminstr/machines/machine">
			<xsl:call-template name="gen_machine_units">
				<xsl:with-param name="machine" select="."/>
			</xsl:call-template>
		</xsl:for-each>
		<!-- generate creators-->
		<xsl:for-each select="/icminstr/machines/machine">
			<xsl:call-template name="gen_machine_creator">
				<xsl:with-param name="machine" select="."/>
			</xsl:call-template>
		</xsl:for-each>
		<!-- generate machines -->
		<xsl:call-template name="TAB"/>
		<xsl:text>icm_machines_::machine machines_int_[</xsl:text>
		<xsl:number value="count(/icminstr/machines/machine)"/>
		<xsl:text>] = {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/machines/machine">
			<xsl:call-template name="gen_machine">
				<xsl:with-param name="machine" select="."/>
			</xsl:call-template>
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
		<xsl:text>namespace icm_machines_ {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>const carray&lt;icm_machines_::machine&gt; machines(machines_int_);</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>} // namespace icm_machines_</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>} // namespace mlaskal</xsl:text>
		<xsl:call-template name="BR"/>
	</xsl:template>
	<xsl:template name="gen_unit">
		<xsl:param name="unit"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>icm_machines_::unit_map unit_map_</xsl:text>
		<xsl:value-of select="$unit/name"/>
		<xsl:text>[] = {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="$unit/mops/mop">
			<xsl:variable name="mop" select="."/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>{ mops::MOPC_</xsl:text>
			<xsl:value-of select="$mop/mop_name"/>
			<xsl:text>, </xsl:text>
			<xsl:number value="$mop/timing"/>
			<xsl:text> },</xsl:text>
			<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>icm_machines_::unit unit_</xsl:text>
		<xsl:value-of select="$unit/name"/>
		<xsl:text> = {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>"</xsl:text>
		<xsl:value-of select="$unit/name"/>
		<xsl:text>",</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>mops::UT_</xsl:text>
		<xsl:value-of select="$unit/unit_type"/>
		<xsl:text>,</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>carray&lt;icm_machines_::unit_map&gt;(unit_map_</xsl:text>
		<xsl:value-of select="$unit/name"/>
		<xsl:text>)</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
	</xsl:template>
	<xsl:template name="gen_machine_class">
		<xsl:param name="machine"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>ai::IC_OPCODE alin_</xsl:text>
		<xsl:value-of select="$machine/name"/>
		<xsl:text>_int_[] = {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="$machine/allowed_instr_classes/class_name">
			<xsl:variable name="class_name" select="."/>
			<xsl:for-each select="/icminstr/instrs/instr[class_name=$class_name]">
				<xsl:call-template name="TAB"/>
				<xsl:call-template name="TAB"/>
				<xsl:text>ai::ICOP_</xsl:text>
				<xsl:value-of select="./name"/>
				<xsl:text>,</xsl:text>
				<xsl:call-template name="BR"/>
			</xsl:for-each>
		</xsl:for-each>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>const carray&lt;ai::IC_OPCODE&gt; alin_</xsl:text>
		<xsl:value-of select="$machine/name"/>
		<xsl:text>(alin_</xsl:text>
		<xsl:value-of select="$machine/name"/>
		<xsl:text>_int_);</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>class machine_</xsl:text>
		<xsl:value-of select="$machine/name"/>
		<xsl:text> : public icm_machine {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>public:</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>explicit machine_</xsl:text>
		<xsl:value-of select="$machine/name"/>
		<xsl:text>(const icm_machines_::machine *m, ICABSNUM regs, const icm_flat_icblock *inic, ostream *os, const icm_machine_trace_options &amp;to) : icm_machine(m, regs, inic, os, to) { }</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
	</xsl:template>
	<xsl:template name="gen_machine_units">
		<xsl:param name="machine"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>icm_machines_::machine_unit units_</xsl:text>
		<xsl:value-of select="$machine/name"/>
		<xsl:text>[</xsl:text>
		<xsl:number value="count($machine/units/unit)"/>
		<xsl:text>] = {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="$machine/units/unit">
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>{ &amp;unit_</xsl:text>
		<xsl:value-of select="./name"/>
		<xsl:text>, </xsl:text>
		<xsl:number value="./number"/>
		<xsl:text> },</xsl:text>
		<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
	</xsl:template>
	<xsl:template name="gen_machine_creator">
		<xsl:param name="machine"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>icm_machine *creator_</xsl:text>
		<xsl:value-of select="$machine/name"/>
		<xsl:text>(const icm_machines_::machine *m, ICABSNUM regs, const icm_flat_icblock *inic, ostream *os, const icm_machine_trace_options &amp;to) {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>return new machine_</xsl:text>
		<xsl:value-of select="$machine/name"/>
		<xsl:text>(m, regs, inic, os, to);</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>}</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
	</xsl:template>
	<xsl:template name="gen_machine">
		<xsl:param name="machine"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>{ "</xsl:text>
		<xsl:value-of select="$machine/name"/>
		<xsl:text>", carray&lt;icm_machines_::machine_unit&gt;(units_</xsl:text>
		<xsl:value-of select="$machine/name"/>
		<xsl:text>), alin_</xsl:text>
		<xsl:value-of select="$machine/name"/>
		<xsl:text>, creator_</xsl:text>
		<xsl:value-of select="$machine/name"/>
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
