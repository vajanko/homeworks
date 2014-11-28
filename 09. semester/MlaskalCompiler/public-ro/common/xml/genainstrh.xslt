<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text" version="1.0" encoding="UTF-8" indent="no"/>
	<xsl:template match="/">
		<xsl:text>/*</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GEN_AINSTR.HPP</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GENERATED, DO NOT MODIFY!!!!</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>*/</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>#ifndef GEN_AINSTR_HPP_</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>#define GEN_AINSTR_HPP_</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<!-- instruction classes forward declaration -->
		<xsl:text>namespace ai {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
			<xsl:call-template name="TAB"/>
			<xsl:text>class </xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>;</xsl:text>
			<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<!-- basic visitor -->
		<xsl:call-template name="TAB"/>
		<xsl:text>class ai_visitor {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>public:</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>virtual ~ai_visitor() { }</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>virtual void visit_</xsl:text>
		<xsl:value-of select="./name"/>
		<xsl:text>(ai::</xsl:text>
		<xsl:value-of select="./name"/>
		<xsl:text> *instr)=0;</xsl:text>
		<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<!-- basic const visitor -->
		<xsl:call-template name="TAB"/>
		<xsl:text>class ai_const_visitor {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>public:</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>virtual ~ai_const_visitor() { }</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>virtual void visit_</xsl:text>
		<xsl:value-of select="./name"/>
		<xsl:text>(const ai::</xsl:text>
		<xsl:value-of select="./name"/>
		<xsl:text> *instr)=0;</xsl:text>
		<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<!-- templated visitor -->
		<xsl:call-template name="TAB"/>
		<xsl:text>template &lt;typename P&gt;</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>class ai_templated_visitor : public ai_visitor, public P {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>public:</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>ai_templated_visitor( const P &amp; r) : P( r) {}</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>private:</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>virtual void visit_</xsl:text>
		<xsl:value-of select="./name"/>
		<xsl:text>(ai::</xsl:text>
		<xsl:value-of select="./name"/>
		<xsl:text> *instr) { this->template visit&lt;ai::</xsl:text>
		<xsl:value-of select="./name"/>
		<xsl:text>&gt;(instr); }</xsl:text>
		<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<!-- templated const visitor -->
		<xsl:call-template name="TAB"/>
		<xsl:text>template &lt;typename P&gt;</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>class ai_templated_const_visitor : public ai_const_visitor, public P {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>public:</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>ai_templated_const_visitor( const P &amp; r) : P( r) {}</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>private:</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>virtual void visit_</xsl:text>
		<xsl:value-of select="./name"/>
		<xsl:text>(const ai::</xsl:text>
		<xsl:value-of select="./name"/>
		<xsl:text> *instr) { this->template visit&lt;ai::</xsl:text>
		<xsl:value-of select="./name"/>
		<xsl:text>&gt;(instr); }</xsl:text>
		<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<!-- instruction implementations -->
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/instrs/instr">
			<xsl:call-template name="TAB"/>
			<xsl:text>class </xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text> : public mlaskal::abstract_instr {</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>public:</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:value-of select="./name"/>
			<xsl:text>(</xsl:text>
			<xsl:if test="./reg_out">
				<xsl:text>mlaskal::ICREGNUM out</xsl:text>
			</xsl:if>
			<xsl:if test="./operand">
				<xsl:if test="./reg_out">
					<xsl:text>, </xsl:text>
				</xsl:if>
			</xsl:if>
			<xsl:choose>
				<xsl:when test="./operand='LITINT'">
					<xsl:text>mlaskal::aic_literal_storage_int_type::const_pointer litint</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='LITREAL'">
					<xsl:text>mlaskal::aic_literal_storage_real_type::const_pointer litreal</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='LITSTR'">
					<xsl:text>mlaskal::aic_literal_storage_string_type::const_pointer litstr</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='ABSNUM'">
					<xsl:text>mlaskal::ICABSNUM absnum</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='GSTKOFFS' or ./operand='LSTKOFFS' or ./operand='SSTKOFFS'">
					<xsl:text>mlaskal::ICRELNUM stkoffs</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='RELJUMP'">
					<xsl:text>mlaskal::plain_icblock&lt;mlaskal::abstract_instr&gt;::const_iterator tgt</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='ABSJUMP'">
					<xsl:text>mlaskal::abstract_functions::const_iterator tgt</xsl:text>
				</xsl:when>
			</xsl:choose>
			<xsl:if test="./reg_in1">
				<xsl:if test="./operand or ./reg_out">
					<xsl:text>, </xsl:text>
				</xsl:if>
				<xsl:text>mlaskal::ICREGNUM in1</xsl:text>
			</xsl:if>
			<xsl:if test="./reg_in2">
				<xsl:text>, mlaskal::ICREGNUM in2</xsl:text>
			</xsl:if>
			<xsl:if test="./reg_in3">
				<xsl:text>, mlaskal::ICREGNUM in3</xsl:text>
			</xsl:if>
			<xsl:if test="./reg_in4">
				<xsl:text>, mlaskal::ICREGNUM in4</xsl:text>
			</xsl:if>
			<xsl:text>) : mlaskal::abstract_instr(ai::ICOP_</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:choose>
				<xsl:when test="./reg_out">
					<xsl:text>, out</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>, 0</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:choose>
				<xsl:when test="./operand='LITINT'">
					<xsl:text>, litint</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='LITREAL'">
					<xsl:text>, litreal</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='LITSTR'">
					<xsl:text>, litstr</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='ABSNUM'">
					<xsl:text>, absnum</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='GSTKOFFS' or ./operand='LSTKOFFS' or ./operand='SSTKOFFS'">
					<xsl:text>, stkoffs</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='RELJUMP'">
					<xsl:text>, tgt</xsl:text>
				</xsl:when>
				<xsl:when test="./operand='ABSJUMP'">
					<xsl:text>, tgt</xsl:text>
				</xsl:when>
			</xsl:choose>
			<xsl:choose>
				<xsl:when test="./reg_in1">
					<xsl:text>, in1</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>, 0</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:choose>
				<xsl:when test="./reg_in2">
					<xsl:text>, in2</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>, 0</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:choose>
				<xsl:when test="./reg_in3">
					<xsl:text>, in3</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>, 0</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:choose>
				<xsl:when test="./reg_in4">
					<xsl:text>, in4</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>, 0</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text>) { }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:value-of select="./name"/>
			<xsl:text>(mlaskal::ICREGNUM out, mlaskal::ICREGNUM in1, mlaskal::ICREGNUM in2, mlaskal::ICREGNUM in3, mlaskal::ICREGNUM in4) : mlaskal::abstract_instr(ai::ICOP_</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>, out, in1, in2, in3, in4) { }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>private:</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>virtual void patch_in1_int_(mlaskal::ICREGNUM</xsl:text>
			<xsl:choose>
				<xsl:when test="./reg_in1">
					<xsl:text> reg) { in1_=reg;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>) { throw std::logic_error("Instruction has no IN1");</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text> }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>virtual void patch_in2_int_(mlaskal::ICREGNUM</xsl:text>
			<xsl:choose>
				<xsl:when test="./reg_in2">
					<xsl:text> reg) { in2_=reg;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>) { throw std::logic_error("Instruction has no IN2");</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text> }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>virtual void patch_in3_int_(mlaskal::ICREGNUM</xsl:text>
			<xsl:choose>
				<xsl:when test="./reg_in3">
					<xsl:text> reg) { in3_=reg;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>) { throw std::logic_error("Instruction has no IN3");</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text> }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>virtual void patch_in4_int_(mlaskal::ICREGNUM</xsl:text>
			<xsl:choose>
				<xsl:when test="./reg_in4">
					<xsl:text> reg) { in4_=reg;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>) { throw std::logic_error("Instruction has no IN4");</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text> }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>virtual void patch_out_int_(mlaskal::ICREGNUM</xsl:text>
			<xsl:choose>
				<xsl:when test="./reg_out">
					<xsl:text> reg) { out_=reg;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>) { throw std::logic_error("Instruction has no OUT/INOUT");</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text> }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>virtual void patch_litint_int_(mlaskal::aic_literal_storage_int_type::const_pointer</xsl:text>
			<xsl:choose>
				<xsl:when test="./operand='LITINT'">
					<xsl:text> litint) { litint_=litint;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>) { throw std::logic_error("Instruction has no LITINT");</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text> }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>virtual void patch_litreal_int_(mlaskal::aic_literal_storage_real_type::const_pointer</xsl:text>
			<xsl:choose>
				<xsl:when test="./operand='LITREAL'">
					<xsl:text> litreal) { litreal_=litreal;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>) { throw std::logic_error("Instruction has no LITREAL");</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text> }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>virtual void patch_litstr_int_(mlaskal::aic_literal_storage_string_type::const_pointer</xsl:text>
			<xsl:choose>
				<xsl:when test="./operand='LITSTR'">
					<xsl:text> litstr) { litstr_=litstr;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>) { throw std::logic_error("Instruction has no LITSTR");</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text> }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>virtual void patch_absnum_int_(mlaskal::ICABSNUM</xsl:text>
			<xsl:choose>
				<xsl:when test="./operand='ABSNUM'">
					<xsl:text> absnum) { absnum_=absnum;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>) { throw std::logic_error("Instruction has no ABSNUM");</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text> }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>virtual void patch_stkoffs_int_(mlaskal::ICRELNUM</xsl:text>
			<xsl:choose>
				<xsl:when test="./operand='GSTKOFFS' or ./operand='LSTKOFFS' or ./operand='SSTKOFFS'">
					<xsl:text> stkoffs) { stkoffs_=stkoffs;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>) { throw std::logic_error("Instruction has no STKOFFS");</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text> }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>virtual void patch_reljump_int_(mlaskal::plain_icblock&lt;mlaskal::abstract_instr&gt;::const_iterator</xsl:text>
			<xsl:choose>
				<xsl:when test="./operand='RELJUMP'">
					<xsl:text> tgt) { reljump_=tgt;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>) { throw std::logic_error("Instruction has no RELJUMP");</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text> }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>virtual void patch_absjump_int_(mlaskal::abstract_functions::const_iterator</xsl:text>
			<xsl:choose>
				<xsl:when test="./operand='ABSJUMP'">
					<xsl:text> tgt) { absjump_=tgt;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>) { throw std::logic_error("Instruction has no ABSJUMP");</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:text> }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>virtual void accept_int_(ai_visitor *visitor) { visitor->visit_</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>(this); }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>virtual void accept_int_(ai_const_visitor *visitor) const { visitor->visit_</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>(this); }</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>};</xsl:text>
			<xsl:call-template name="BR"/>
			<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:text>}</xsl:text>
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
	</xsl:template></xsl:stylesheet>
