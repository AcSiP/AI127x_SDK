<H2> 6. Payload Control </H2>

<BR>


- <B>LoraPayloadLength</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> Payload length for transmission in byte. </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Configure LoRa packet payload length in byte.<BR>
		It must be within 1 to 255.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD><I>LoraPayloadLength  40</I></TD></TR>
</TABLE>
<BR>

- <B>LoraGetPayloadLength</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Report the payload length of LoRa last transmit / received packet in byte.</TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraGetPayloadLength</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>PayloadLength=59</I>
	</TD>
</TR>
</TABLE>
<BR>


- <B>LoraMaxPayloadLength</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> Maximum payload length for receive in byte. </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Configure maximum receive payload length in byte for LoRa packet.<BR>
		It must be within 1 to 255.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD><I>LoraMaxPayloadLength  255</I></TD></TR>
</TABLE>
<BR>

- <B>LoraGetRxPacketTimeout</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Report current timeout interval for receive in mS.<BR>
		It would be re-calculated and affected by maximum payload length.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraGetRxPacketTimeout</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>RxPacketTimeout=2183</I>
	</TD>
</TR>
</TABLE>
<BR>



- <B>LoraGetPara</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Report complete RF and Payload parameters.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraGetPara</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>RFFrequency=915000000  BW=7  SF=10  ErrorCoding=1  RFPower=20 FreqHopOn=false  HeaderStatus=Explicit  CRC=ON  PreambleLength=12  PayloadLength=255  MaxPayloadLength=255  TxPacketTimeout=3221  RxPacketTimeout=3221</I>
	</TD>
</TR>
</TABLE>
<BR>

<BR>
<BR>
[Back to Index](CLI__Command_List__Index.md)
