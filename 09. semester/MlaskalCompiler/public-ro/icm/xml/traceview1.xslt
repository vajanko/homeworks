<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method="html" version="1.0" encoding="UTF-8" indent="yes"/>
	<xsl:variable name="trace" select="/icm_trace"/>
	<xsl:param name="icminstr_file" select="'../../common/xml/icminstr.xml'"/>
	<xsl:variable name="icminstr" select="document( $icminstr_file)/icminstr"/>
	<xsl:key name="k_instr" match="/icm_trace/code/instr" use="IP"/>
	<xsl:key name="k_icminstr_instr" match="/icminstr/instrs/instr" use="name"/>
	<xsl:template match="/icm_trace">
		<html>
			<head>
				<title>
					<xsl:text>Trace file </xsl:text>
					<xsl:value-of select="trace_info/input_file"/>
				</title>
			</head>
			<body>
				<h4>
					<xsl:text>Trace file </xsl:text>
					<xsl:value-of select="trace_info/input_file"/>
				</h4>
				<h4>
					<xsl:text>ICMINSTR file </xsl:text>
					<xsl:value-of select="$icminstr_file"/>
				</h4>
				<h4>
					<xsl:text>ICMINSTR machines: </xsl:text>
					<xsl:value-of select="$icminstr/machines/machine/name"/>
				</h4>
				<table frame="box" border="3" rules="cols" cellspacing="0">
					<tr>
						<th>Tick</th>
						<th>Fetch</th>
						<xsl:for-each select="$trace/stats/unit_pools/unit_pool_type">
							<th>
								<xsl:value-of select="name"/>
							</th>
						</xsl:for-each>
					</tr>
					<xsl:for-each select="$trace/ticks/tick">
						<xsl:variable name="tick" select="."/>
						<xsl:variable name="executes" select="$tick/actions/execute | $tick/actions/fetch"/>
						<xsl:if test="not( $executes)">
							<tr>
								<th>
									<xsl:attribute name="bgcolor"><xsl:call-template name="color2"><xsl:with-param name="index" select="$tick/tick_number"/></xsl:call-template></xsl:attribute>
									<xsl:value-of select="$tick/tick_number"/>
								</th>
								<td>
									<xsl:attribute name="bgcolor"><xsl:call-template name="color2"><xsl:with-param name="index" select="$tick/tick_number"/></xsl:call-template></xsl:attribute>
								</td>
								<xsl:for-each select="$trace/stats/unit_pools/unit_pool_type">
									<xsl:variable name="unit_pool_type" select="."/>
									<td>
										<xsl:attribute name="bgcolor"><xsl:call-template name="color2"><xsl:with-param name="index" select="$tick/tick_number"/></xsl:call-template></xsl:attribute>
									</td>
								</xsl:for-each>
							</tr>
						</xsl:if>
						<xsl:for-each select="$executes[ position() = 1]">
							<xsl:variable name="execute" select="."/>
							<tr>
								<th>
									<xsl:attribute name="bgcolor"><xsl:call-template name="color2"><xsl:with-param name="index" select="$tick/tick_number"/></xsl:call-template></xsl:attribute>
									<xsl:attribute name="rowspan"><xsl:value-of select="count( $executes)"/></xsl:attribute>
									<xsl:value-of select="$tick/tick_number"/>
								</th>
								<xsl:call-template name="show_execute">
									<xsl:with-param name="execute" select="$execute"/>
									<xsl:with-param name="tick" select="$tick"/>
								</xsl:call-template>
							</tr>
						</xsl:for-each>
						<xsl:for-each select="$executes[ position() > 1]">
							<xsl:variable name="execute" select="."/>
							<tr>
								<xsl:call-template name="show_execute">
									<xsl:with-param name="execute" select="$execute"/>
									<xsl:with-param name="tick" select="$tick"/>
								</xsl:call-template>
							</tr>
						</xsl:for-each>
					</xsl:for-each>
				</table>
			</body>
		</html>
	</xsl:template>
	<xsl:template name="show_execute">
		<xsl:param name="execute"/>
		<xsl:param name="tick"/>
		<xsl:choose>
			<xsl:when test="$execute/self::fetch">
				<xsl:variable name="instr" select="key( 'k_instr', $execute/IP)"/>
				<td>
					<xsl:attribute name="bgcolor"><xsl:call-template name="color1"><xsl:with-param name="index" select="$execute/IP"/></xsl:call-template></xsl:attribute>
					<xsl:value-of select="$execute/IP"/>
					<xsl:text>:</xsl:text>
					<xsl:value-of select="$instr/opcode"/>
					<xsl:text> </xsl:text>
					<xsl:value-of select="$instr/operands"/>
				</td>
			</xsl:when>
			<xsl:otherwise>
				<td>
					<xsl:attribute name="bgcolor"><xsl:call-template name="color2"><xsl:with-param name="index" select="$tick/tick_number"/></xsl:call-template></xsl:attribute>
				</td>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:for-each select="$trace/stats/unit_pools/unit_pool_type">
			<xsl:variable name="unit_pool_type" select="."/>
			<xsl:choose>
				<xsl:when test="$execute/self::execute and $execute/unit/name = $unit_pool_type/name">
					<xsl:variable name="instr" select="key( 'k_instr', $execute/mop/IP)"/>
					<td>
						<xsl:attribute name="bgcolor"><xsl:call-template name="color1"><xsl:with-param name="index" select="$execute/mop/IP"/></xsl:call-template></xsl:attribute>
						<xsl:value-of select="$execute/mop/IP"/>
						<xsl:text>/</xsl:text>
						<xsl:value-of select="$execute/mop/mod_id"/>
						<xsl:for-each select="$icminstr">
							<xsl:variable name="instr_mop" select="key( 'k_icminstr_instr', $instr/opcode)/mop_descr/mop[ id = $execute/mop/mod_id]"/>
							<xsl:text>:</xsl:text>
							<xsl:value-of select="$instr_mop/mop_name"/>
<!--							
							<xsl:text>:</xsl:text>
							<xsl:value-of select="$icminstr/units/unit[ name = $unit_pool_type/name]/mops/mop[ mop_name = $instr_mop/mop_name]/timing"/>
-->
						</xsl:for-each>
					</td>
				</xsl:when>
				<xsl:otherwise>
					<td>
						<xsl:attribute name="bgcolor"><xsl:call-template name="color2"><xsl:with-param name="index" select="$tick/tick_number"/></xsl:call-template></xsl:attribute>
					</td>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:for-each>
	</xsl:template>
	<xsl:template name="color1">
		<xsl:param name="index"/>
		<xsl:variable name="indexmod" select="$index mod 10"/>
		<xsl:choose>
			<xsl:when test="$indexmod = 0">
				<xsl:text>#C0C0FF</xsl:text>
			</xsl:when>
			<xsl:when test="$indexmod = 1">
				<xsl:text>#C0FFC0</xsl:text>
			</xsl:when>
			<xsl:when test="$indexmod = 2">
				<xsl:text>#C0FFFF</xsl:text>
			</xsl:when>
			<xsl:when test="$indexmod = 3">
				<xsl:text>#FFC0C0</xsl:text>
			</xsl:when>
			<xsl:when test="$indexmod = 4">
				<xsl:text>#FFC0FF</xsl:text>
			</xsl:when>
			<xsl:when test="$indexmod = 5">
				<xsl:text>#FFFFC0</xsl:text>
			</xsl:when>
			<xsl:when test="$indexmod = 6">
				<xsl:text>#C0C0E0</xsl:text>
			</xsl:when>
			<xsl:when test="$indexmod = 7">
				<xsl:text>#C0E0C0</xsl:text>
			</xsl:when>
			<xsl:when test="$indexmod = 8">
				<xsl:text>#C0E0E0</xsl:text>
			</xsl:when>
			<xsl:when test="$indexmod = 9">
				<xsl:text>#E0C0C0</xsl:text>
			</xsl:when>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="color2">
		<xsl:param name="index"/>
		<xsl:variable name="indexmod" select="$index mod 2"/>
		<xsl:choose>
			<xsl:when test="$indexmod = 0">
				<xsl:text>#A0A0A0</xsl:text>
			</xsl:when>
			<xsl:when test="$indexmod = 1">
				<xsl:text>#C0C0C0</xsl:text>
			</xsl:when>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>
