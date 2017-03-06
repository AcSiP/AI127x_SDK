<H2> 5. RF Parameters </H2>

<BR>

- <B>LoraFreq</B>
<TABLE>
<TR align="Left" valign="center">
	<TD><B>Parameter(s)</B></TD>
	<TD>
		Carrier frequency in Hz.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Configure RF carrier frequency in Hz.<BR>
		For SX1276, Range is 847 MHz ~ 930 MHz<BR>
		For SX1278, Range is 430 MHz ~ 510 MHz
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD><I>LoraFreq  915000000</I></TD></TR>
</TABLE>
<BR>

- <B>LoraGetRFFrequency</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Report current RF carrier frequency in Hz. </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraGetRFFrequency</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>RFFrequency=915000000</I>
	</TD>
</TR>
</TABLE>
<BR>


- <B>LoraPower</B>
<TABLE>
<TR align="Left" valign="center">
	<TD><B>Parameter(s)</B></TD>
	<TD>
		Carrier frequency in Hz.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Configure RF output power level in dBm.<BR>
		It must be within 5 to 20.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD><I>LoraPower  17</I></TD></TR>
</TABLE>
<BR>

- <B>LoraGetRFPower</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Report output power setup for transmission in dBm. </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraGetRFPower</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>RFPower=20</I>
	</TD>
</TR>
</TABLE>
<BR>


- <B>LoraSF</B>
<TABLE>
<TR align="Left" valign="center">
	<TD><B>Parameter(s)</B></TD>
	<TD>
		Spreading factor ( 6 to 12 ).<BR>
		Default value is 10.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Configure LoRa modulation spreading factor.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD><I>LoraSF  8</I></TD></TR>
</TABLE>
<BR>

- <B>LoraGetSF</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Report the LoRa modulation spreading factor ( 6 to 12 ). </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraGetSF</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>SF=10</I>
	</TD>
</TR>
</TABLE>
<BR>

- <B>LoraBW</B>
<TABLE>
<TR align="Left" valign="center">
	<TD><B>Parameter(s)</B></TD>
	<TD>
		Bandwidth factor.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Configure LoRa modulation bandwidth factor.<BR>
		6 => 62.5 kHz<BR>
		7 => 125 kHz (default)<BR>
		8 => 250 kHz<BR>
		9 => 500 kHz
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD><I>LoraBW  7</I></TD></TR>
</TABLE>
<BR>

- <B>LoraGetBW</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Report the LoRa modulation bandwidth factor. </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraGetBW</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>BW=7</I>
	</TD>
</TR>
</TABLE>
<BR>


- <B>LoraPreambleLength</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> Preamble length in symbol. </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Configure LoRa packet preamble length.<BR>
		It must be within 6 to 65535.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD><I>LoraPreambleLength  12</I></TD></TR>
</TABLE>
<BR>

- <B>LoraGetPreambleLength</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Report the preamble length of LoRa packet while transmission in symbol. </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraGetPreambleLength</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>PreambleLength=14</I>
	</TD>
</TR>
</TABLE>
<BR>


- <B>LoraErrorCoding</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> FEC coding rate factor. </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Configure LoRa packet FEC coding rate factor.<BR>
		1 => 1.25 (default)<BR>
		2 => 1.5<BR>
		3 => 1.75<BR>
		4 => 2.0
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD><I>LoraErrorCoding  1</I></TD></TR>
</TABLE>
<BR>

- <B>LoraGetErrorCoding</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Report the LoRa packet FEC coding rate factor. </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraGetErrorCoding</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>ErrorCoding=1</I>
	</TD>
</TR>
</TABLE>
<BR>

<BR>
<BR>
[Back to Index](CLI__Command_List__Index.md)
