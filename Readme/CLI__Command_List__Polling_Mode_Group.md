<H2> 8. Polling Mode Group Management</H2>

<BR>

- <B>LoraGetMyAddr</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Report the device address of AcSiP's LoRa protocol.</TD></TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraGetMyAddr</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>LoraMyAddr=6b0052</I>
	</TD>
</TR>
</TABLE>
<BR>


- <B>LoraGetGateWayAddr</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Report the master device address of AcSiP's polling protocol.<BR>
		<BR>
		This command is used for slave-end device.<BR>
		If this device is not join any group/network yet, it shows 'NULL'.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraGetGateWayAddr</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>LoraGateWayAddr=NULL</I>
	</TD>
</TR>
</TABLE>
<BR>


- <B>LoraJoinNode</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> Slave device address of AcSiP's polling protocol. </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Ask master device polls the slave device to join the group/network.<BR>
		<BR>
		This command is used for master-end device.<BR>
		<BR>
		The master will poll target slave device with join request.<BR>
		The slave will acknowledge the request to master device.<BR>
		The master response 'JoinOK' <B>while receiving the acknowledge</B>.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraJoinNode  2c0054</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>Node=2c0054 EVT=JoinOK</I>
	</TD>
</TR>
</TABLE>
<BR>


- <B>LoraLeaveNode</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> Slave device address of AcSiP's polling protocol. </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Ask master device polls the slave device to leave the group/network.<BR>
		<BR>
		This command is used for master-end device.<BR>
		<BR>
		The master will poll target slave device with leave request.<BR>
		The slave will acknowledge the request to master device.<BR>
		The master response 'LeaveOK' <B>while receiving the acknowledge</B>.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraLeaveNode  2c0054</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>Node=2c0054 EVT=LeaveOK</I>
	</TD>
</TR>
</TABLE>
<BR>

- <B>LoraForceJoinNode</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> Slave device address of AcSiP's polling protocol. </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Configure master device that the slave device is exist inside the group/network.<BR>
		<BR>
		This command is used for master-end device.<BR>
		<BR>
		The master will <B>NOT </B> poll target slave device with join request.<BR>
		The master adds the target device into group/network list in force.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD><I>LoraForceJoinNode  2c0054</I></TD></TR>
</TABLE>
<BR>

- <B>LoraForceLeaveNode</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> Slave device address of AcSiP's polling protocol. </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Configure master device that remove the target device from the group/network.<BR>
		<BR>
		This command is used for master-end device.<BR>
		<BR>
		The master will <B>NOT </B> poll target slave device with leave request.<BR>
		The master remove the target device from group/network list in force.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>		<TD><I>LoraForceLeaveNode  2c0054</I></TD></TR>
</TABLE>
<BR>


- <B>LoraSetNodePara</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>
	<TD>
		Slave device address of AcSiP's polling protocol.<BR>
		<BR>
		The duration (in second) of slave device sleeping.<BR>
		<BR>
		The alias in Base64 encoding of slave device.<BR>
		The quotation marks <B>MUST</B> be used for encoded Base64 string.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Ask master device polls the slave device to setup parameter for slave.<BR>
		<BR>
		This command is used for master-end device.<BR>
		<BR>
		The master will poll target slave device with configure request(Master_AcsipProtocol_Interval).<BR>
		The slave will acknowledge the request to master device.<BR>
		The master response 'setting SUCCESSED' <B>while receiving the acknowledge</B>.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraSetNodePara 1e002c 10 “abcd”</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>Node=1e002c setting SUCCESSED!</I>
	</TD>
</TR>
</TABLE>
<BR>


- <B>LoraGetNodePara</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> Slave device address of AcSiP's polling protocol. </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>		<TD> Report the parameter of target device.</TD></TR>
<TR align="Left" valign="center"><TD><B>Example 1 (with alias)</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraGetNodePara</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>Node=110011 EVT=GetNode <B>0 abcd<B></I>
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example 2 (without alias)</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraGetNodePara</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>Node=110011 EVT=GetNode <B>0<B></I>
	</TD>
</TR>
</TABLE>
<BR>


- <B>LoraListNodes</B>
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>		<TD> N/A </TD></TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Report the composition of the group/network.<BR>
		<BR>
		The first result is the number of slave nodes.<BR>
		The rest is the list of each slave device address of the group/network.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraListNodes</I><BR>
		Response:<BR>
		&nbsp; &nbsp;	<I>Nodes=<B>3</B> 1e002c 220022 330033</I>
	</TD>
</TR>
</TABLE>
<BR>

- <B>LoraNodeData</B>  (for Master)
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>
	<TD>
		Slave device address of AcSiP's polling protocol.<BR>
		<BR>
		The private payload in Base64 encoding of slave device.<BR>
		The quotation marks is <B>NOT<B> used for encoded Base64 string.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Ask master device polls the slave device to fetch private data from slave.<BR>
		<BR>
		This method is used for master-end device.<BR>
		<BR>
		The master will poll target slave device with data exchange request(Master_AcsipProtocol_Data).<BR>
		The slave will send the private data <B>while requesting</B>.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraNodeData 1e002c Qwer1ad8</I><BR>
		Response:<BR>
		(On the Slave-End)
		&nbsp; &nbsp;	<I>LoraGateWayAddr=6b0052  DataLength=8  Data=Qwer1ad8</I>
	</TD>
</TR>
</TABLE>
<BR>


- <B>LoraNodeData</B>  (for Slave)
<TABLE>
<TR align="Left" valign="center"><TD><B>Parameter(s)</B></TD>
	<TD>
		The private payload in Base64 encoding of slave device.<BR>
		The quotation marks is <B>NOT<B> used for encoded Base64 string.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Description</B></TD>
	<TD>
		Prepare private data and wait for polling by master.<BR>
		<BR>
		This method is used for slave-end device.<BR>
		<BR>
		The slave buffers the private data <B>until requesting</B>.<BR>
		If the buffer is not empty, this command returns failure.
	</TD>
</TR>
<TR align="Left" valign="center"><TD><B>Example</B></TD>
	<TD>
		Command:<BR>
		&nbsp; &nbsp;	<I>LoraNodeData Abcd12345678</I><BR>
		Response:<BR>
		(On the Master-End)
		&nbsp; &nbsp;	<I>Node=1e002c  DataLength=12  Data=Abcd12345678</I>
	</TD>
</TR>
</TABLE>
<BR>


<BR>
<BR>
[Back to Index](CLI__Command_List__Index.md)
