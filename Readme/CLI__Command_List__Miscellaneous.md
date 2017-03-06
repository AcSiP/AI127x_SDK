<H2> 3. Miscellaneous </H2>

<BR>

- <B>SystemReset</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Soft Reboot the system. </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD><I>SystemReset</I></TD></TR>
</TABLE>
<BR>


- <B>LoraSystemMode</B>
<TABLE>
<TR align="Left" valign="center">
	<TD><B>Parameter(s)</B></TD>
	<TD>
		<B>inNormal</B>       => For user normal application(defult)<BR>
		<B>inTD</B>           => Demo Ping/Pong test<BR>
		<B>inPV</B>           => Reserved for production test<BR>
		<B>inIdle</B>         => Reserved for RD use<BR>
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Setup system operation mode. </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD><I>LoraSystemMode  inNormal</I></TD></TR>
</TABLE>
<BR>

- <B>LoraGetSystemMode</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Query system operation mode. </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraGetSystemMode</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>SystemOperationMode=InNormal</I>
	</TD>
</TR>
</TABLE>
<BR>


- <B>LoraMode</B>
<TABLE>
<TR align="Left" valign="center">
	<TD><B>Parameter(s)</B></TD>
	<TD>
		<B>MASTER</B><BR>
		or<BR>
		<B>SLAVE</B> (default)<BR>
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Configure the role of device. </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD><I>LoraMode  MASTER</I></TD></TR>
</TABLE>
<BR>


- <B>LoraGetMode</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Query the role of device. </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraGetMode</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>LoraMode=MASTER</I>
	</TD>
</TR>
</TABLE>
<BR>


- <B>GetFWVersion</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Query the version of application firmware. </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>GetFWVersion</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>FirmwareVersion=<B>1.4.2017.0110</B> for <B>SX1276</B></I>
	</TD>
</TR>
</TABLE>
<BR>


- <B>GetUUID</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Query the (MCU) unique device ID by STM. </TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>GetUUID</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>UUID=3736393834335109006b0052</I>
	</TD>
</TR>
</TABLE>
<BR>


- <B>SleepStop</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>
	<TD>
		Desired period of sleep time in seconds.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Make device sleep a while.<BR>
		It will wake up after the specific sleep time.<BR>
		The maximun time period is 86400 seconds (24 Hours)
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD><I>SleepStop  300</I></TD></TR>

</TABLE>
<BR>

<BR>
<BR>
[Back to Index](CLI__Command_List__Index.md)
