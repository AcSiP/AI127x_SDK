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
		For SX1276, Range is 847 MHz ~ 930 MHz
		For SX1278, Range is 420 MHz ~ 480 MHz
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



<BR>
<BR>
[Back to Index](CLI__Command_List__Index.md)
