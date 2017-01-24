<H2> 3. Miscellaneous </H2>

<BR>

- <B>SystemReset</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Response on Success</B></TD>	<TD> OK </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Soft Reboot the system. </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD> SystemReset </TD></TR>
</TABLE>
<BR>

- <B>LoraSystemMode</B>
<TABLE>
<TR align="Left" valign="center">
	<TD><B>Parameter(s)</B></TD>
	<TD>
		inNormal   =>   For user normal application(defult)<BR>
		inTD       =>   Demo Ping/Pong test<BR>
		inPV       =>   Reserved for production test<BR>
		inIdle     =>   Reserved for RD use<BR>
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Response on Success</B></TD>	<TD> OK </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Setup system operation mode. </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD> LoraSystemMode  inNormal </TD></TR>
</TABLE>
<BR>


- <B>LoraMode</B>
<TABLE>
<TR align="Left" valign="center">
	<TD><B>Parameter(s)</B></TD>
	<TD>
		<B>MASTER</B> or <B>SLAVE</B> (default)<BR>
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Response on Success</B></TD>	<TD> OK </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Configure the role of LoRa device. </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD> LoraMode  MASTER </TD></TR>
</TABLE>
<BR>

<BR>
<BR>
[Back to Index](CLI__Command_List__Index.md)
