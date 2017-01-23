# Command List
This page lists and describes available commands.<BR><BR>


<H2> Command / Respose Format </H2>

<TABLE>
<TR align="Left" valign="center"><td colspan="3"><B>Command</B></td></TR>
<TR><TH> Command (ASCII String) </TH><TH> Parameter (ASCII String) </TH><TH> End Of Line </TH></TR>
<TR align="center" valign="center"><TD> N Bytes </TD><TD> 0 ~ N Bytes </TD><TD> 0x0D (CR) </TD></TR>
<TR></TR>
<TR align="Left" valign="center"><td colspan="3">Ex.</td></TR>
<TR align="center" valign="center"><TD> LoraPower </TD><TD> 17 </TD><TD> ↵ </TD></TR>
<TR align="center" valign="center"><TD> UartEchoOn </TD><TD>  </TD><TD> ↵ </TD></TR>
</TABLE>
<BR>

<TABLE>
<TR align="Left" valign="center"><td colspan="2"><B>Response for Command syntax error or Execute result</B></td></TR>
<TR><TH> Response (ASCII String) </TH><TH> End Of Line </TH></TR>
<TR align="Left" valign="center"><td colspan="2">Ex.</td></TR>
<TR align="center" valign="center"><TD> UNKNOW </TD><TD> (CR)(LF) </TD></TR>
<TR align="center" valign="center"><TD> OK </TD><TD> (CR)(LF) </TD></TR>
<TR align="center" valign="center"><TD> ERROR </TD><TD> (CR)(LF) </TD></TR>
</TABLE>
<BR>

<TABLE>
<TR align="Left" valign="center"><td colspan="2"><B>Response for Query Result</B></td></TR>
<TR>
  <TD><B>Response Item 1</B><I>(space) Response Item 2 ……. (space) Response Item N (ASCII String)</I> </TD>
  <TD><B> End Of Line </B></TD>
</TR>
<TR align="Left" valign="center"><td colspan="2">Ex.</td></TR>
<TR align="center" valign="center"><TD> TxPacketTimeout=2183 </TD><TD> (CR)(LF) </TD></TR>
<TR align="center" valign="center"><TD> RFFrequency=915000000  BW=6  SF=12 ErrorCoding=1 </TD><TD> (CR)(LF) </TD></TR>
</TABLE>
<BR>

<BR>
