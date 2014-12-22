<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="text" version="1.0" encoding="UTF-8" indent="no"/>
	<xsl:template match="/">
		<xsl:text>/*</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>GEN_MOPS.CPP</xsl:text>
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
		<xsl:text>#include &lt;cassert&gt;</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>#include "mops.hpp"</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>#include "ic_instr.hpp"</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:text>#include "icm_machine.hpp"</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>namespace mlaskal {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>namespace mops {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<!-- generate classes -->
		<xsl:for-each select="/icminstr/mops/mop">
			<xsl:call-template name="gen_mop_class">
				<xsl:with-param name="mop" select="."/>
			</xsl:call-template>
		</xsl:for-each>
		<!-- generate creators -->
		<xsl:for-each select="/icminstr/mops/mop">
			<xsl:call-template name="gen_mop_creator">
				<xsl:with-param name="mop" select="."/>
			</xsl:call-template>
		</xsl:for-each>
		<xsl:text>} // namespace mops</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>mop_factory::mop_creator mop_factory::creators[mops::MOPC_DNOTUSE_MAX_] = {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/mops/mop">
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>mops::create_</xsl:text>
			<xsl:value-of select="./name"/>
			<xsl:text>,</xsl:text>
			<xsl:call-template name="BR"/>
		</xsl:for-each>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<!-- generate mop_info -->
		<xsl:call-template name="TAB"/>
		<xsl:text>mop_info_t mop_info[] = {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:for-each select="/icminstr/mops/mop">
			<xsl:call-template name="gen_mop_info">
				<xsl:with-param name="mop" select="."/>
			</xsl:call-template>
		</xsl:for-each>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<xsl:text>} // namespace mlaskal</xsl:text>
		<xsl:call-template name="BR"/>
	</xsl:template>
	<xsl:template name="gen_mop_class">
		<xsl:param name="mop"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>class </xsl:text>
		<xsl:value-of select="$mop/name"/>
		<xsl:text> : public mop {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>public:</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:value-of select="$mop/name"/>
		<xsl:text>(const ic_instr *instr, icm_machine *mach, icm_machine_unit_pool *up, ICMT_ICIP ip, std::string mop_id, unsigned timing) : mop(instr, mach, up, ip, mop_id, timing) { }</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>private:</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>virtual bool is_ready_int_() const { return </xsl:text>
		<xsl:choose>
			<xsl:when test="$mop/in1">
				<xsl:text>in1_.get()!=0</xsl:text>
				<xsl:if test="$mop/in2">
					<xsl:text> &amp;&amp; in2_.get()!=0</xsl:text>
				</xsl:if>
				<xsl:if test="$mop/in3">
					<xsl:text> &amp;&amp; in3_.get()!=0</xsl:text>
				</xsl:if>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>true</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>; }</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>virtual bool is_ready_addr_int_() const { </xsl:text>
		<xsl:choose>
			<xsl:when test="/icminstr/mop_types/mop_type[name=$mop/mop_type]/unit_type='MEM'">
				<xsl:text>return in1_.get()!=0</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>assert(0); return true</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>; }</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:variable name="mop_type" select="/icminstr/mop_types/mop_type[name=$mop/mop_type]"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>virtual bool is_read_int_() const { return </xsl:text>
		<xsl:choose>
			<xsl:when test="$mop_type/access and $mop_type/access='RD'">
				<xsl:text>true</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>false</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>; }</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>virtual bool is_write_int_() const { return </xsl:text>
		<xsl:choose>
			<xsl:when test="$mop_type/access and $mop_type/access='WR'">
				<xsl:text>true</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>false</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>; }</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>virtual ICREGNUM get_regnum_int_() const { </xsl:text>
		<xsl:choose>
			<xsl:when test="$mop_type/name='RDIN1'">
				<xsl:text>return instr_-&gt;get_in1()</xsl:text>
			</xsl:when>
			<xsl:when test="$mop_type/name='RDIN2'">
				<xsl:text>return instr_-&gt;get_in2()</xsl:text>
			</xsl:when>
			<xsl:when test="$mop_type/name='RDIN3'">
				<xsl:text>return instr_-&gt;get_in3()</xsl:text>
			</xsl:when>
			<xsl:when test="$mop_type/name='RDIN4'">
				<xsl:text>return instr_-&gt;get_in4()</xsl:text>
			</xsl:when>
			<xsl:when test="$mop_type/name='WROUT'">
				<xsl:text>return instr_-&gt;get_out()</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>assert(0); return static_cast&lt;ICREGNUM&gt;(-1)</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>; }</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>virtual void exec_int_();</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>};</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
		<!-- generate execution -->
		<xsl:call-template name="TAB"/>
		<xsl:text>void </xsl:text>
		<xsl:value-of select="$mop/name"/>
		<xsl:text>::exec_int_() {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:if test="$mop/in1 and not($mop/in1_direct or $mop/in1_direct='true')">
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>ICMT_</xsl:text>
			<xsl:call-template name="gen_type">
				<xsl:with-param name="type" select="$mop/in1/type"/>
			</xsl:call-template>
			<xsl:text> in1 = in1_->get</xsl:text>
			<xsl:call-template name="gen_type">
				<xsl:with-param name="type" select="$mop/in1/type"/>
			</xsl:call-template>
			<xsl:text>(ip_);</xsl:text>
			<xsl:call-template name="BR"/>
		</xsl:if>
		<xsl:if test="$mop/in2 and not($mop/in2_direct or $mop/in2_direct='true')">
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>ICMT_</xsl:text>
			<xsl:call-template name="gen_type">
				<xsl:with-param name="type" select="$mop/in2/type"/>
			</xsl:call-template>
			<xsl:text> in2 = in2_->get</xsl:text>
			<xsl:call-template name="gen_type">
				<xsl:with-param name="type" select="$mop/in2/type"/>
			</xsl:call-template>
			<xsl:text>(ip_);</xsl:text>
			<xsl:call-template name="BR"/>
		</xsl:if>
		<xsl:if test="$mop/in3">
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>ICMT_</xsl:text>
			<xsl:call-template name="gen_type">
				<xsl:with-param name="type" select="$mop/in3/type"/>
			</xsl:call-template>
			<xsl:text> in3 = in3_->get</xsl:text>
			<xsl:call-template name="gen_type">
				<xsl:with-param name="type" select="$mop/in3/type"/>
			</xsl:call-template>
			<xsl:text>(ip_);</xsl:text>
			<xsl:call-template name="BR"/>
		</xsl:if>
		<xsl:if test="$mop/out and (not($mop/out_uninit) or $mop/out_uninit='false')">
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>ICMT_</xsl:text>
			<xsl:call-template name="gen_type">
				<xsl:with-param name="type" select="$mop/out/type"/>
			</xsl:call-template>
			<xsl:text> out;</xsl:text>
			<xsl:call-template name="BR"/>
		</xsl:if>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:value-of select="$mop/code"/>
		<xsl:call-template name="BR"/>
		<xsl:if test="$mop/out and not(normalize-space($mop/code) and $mop/out_uninit and not($mop/out_uninit='false'))">
			<xsl:call-template name="TAB"/>
			<xsl:call-template name="TAB"/>
			<xsl:text>out_ = SHRVAL(new icv::</xsl:text>
			<xsl:call-template name="gen_type">
				<xsl:with-param name="type" select="$mop/out/type"/>
			</xsl:call-template>
			<xsl:text>(</xsl:text>
			<xsl:if test="not($mop/out_uninit) or $mop/out_uninit='false'">
				<xsl:text>out</xsl:text>
			</xsl:if>
			<xsl:text>));</xsl:text>
			<xsl:call-template name="BR"/>
		</xsl:if>
		<xsl:call-template name="TAB"/>
		<xsl:text>}</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
	</xsl:template>
	<xsl:template name="gen_mop_creator">
		<xsl:param name="mop"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>mop *create_</xsl:text>
		<xsl:value-of select="$mop/name"/>
		<xsl:text>(const ic_instr *instr, icm_machine *mach, icm_machine_unit_pool *up, ICMT_ICIP ip, std::string mop_id, unsigned timing) {</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>return new </xsl:text>
		<xsl:value-of select="$mop/name"/>
		<xsl:text>(instr, mach, up, ip, mop_id, timing);</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>}</xsl:text>
		<xsl:call-template name="BR"/>
		<xsl:call-template name="BR"/>
	</xsl:template>
	<!-- Generates C++ type -->
	<xsl:template name="gen_type">
		<xsl:param name="type"/>
		<xsl:choose>
			<xsl:when test="$type='B'">
				<xsl:text>BOOL</xsl:text>
			</xsl:when>
			<xsl:when test="$type='I'">
				<xsl:text>INT</xsl:text>
			</xsl:when>
			<xsl:when test="$type='R'">
				<xsl:text>REAL</xsl:text>
			</xsl:when>
			<xsl:when test="$type='S'">
				<xsl:text>STRING</xsl:text>
			</xsl:when>
			<xsl:when test="$type='P'">
				<xsl:text>PTR</xsl:text>
			</xsl:when>
			<xsl:when test="$type='F'">
				<xsl:text>RA</xsl:text>
			</xsl:when>
			<xsl:when test="$type='C'">
				<xsl:text>ICIP</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:message terminate="yes">Unknown type</xsl:message>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="gen_mop_info">
		<xsl:param name="mop"/>
		<xsl:variable name="mop_type" select="/icminstr/mop_types/mop_type[name=$mop/mop_type]"/>
		<xsl:call-template name="TAB"/>
		<xsl:call-template name="TAB"/>
		<xsl:text>{ mops::UT_</xsl:text>
		<xsl:value-of select="$mop_type/unit_type"/>
		<xsl:text>, </xsl:text>
		<xsl:choose>
			<xsl:when test="$mop_type/access and $mop_type/access='RD'">
				<xsl:text>true</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>false</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>, </xsl:text>
		<xsl:choose>
			<xsl:when test="$mop_type/access and $mop_type/access='WR'">
				<xsl:text>true</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>false</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:text>},</xsl:text>
		<xsl:call-template name="TAB"/>
		<xsl:text>// </xsl:text>
		<xsl:value-of select="$mop/name"/>
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
