00000000  E9110B0003        jmp dword 0x3000b16
00000005  0000              add [eax],al
00000007  0000              add [eax],al
00000009  0000              add [eax],al
0000000B  0100              add [eax],eax
0000000D  0100              add [eax],eax
0000000F  1000              adc [eax],al
00000011  0000              add [eax],al
00000013  0000              add [eax],al
00000015  0000              add [eax],al
00000017  0000              add [eax],al
00000019  0000              add [eax],al
0000001B  0000              add [eax],al
0000001D  0000              add [eax],al
0000001F  0000              add [eax],al
00000021  2000              and [eax],al
00000023  0000              add [eax],al
00000025  0000              add [eax],al
00000027  0000              add [eax],al
00000029  0000              add [eax],al
0000002B  0000              add [eax],al
0000002D  0000              add [eax],al
0000002F  00FF              add bh,bh
00000031  FF00              inc dword [eax]
00000033  0000              add [eax],al
00000035  98                cwde
00000036  CF                iretd
00000037  00FF              add bh,bh
00000039  FF00              inc dword [eax]
0000003B  0000              add [eax],al
0000003D  92                xchg eax,edx
0000003E  CF                iretd
0000003F  00FF              add bh,bh
00000041  FF00              inc dword [eax]
00000043  800BF2            or byte [ebx],0xf2
00000046  0000              add [eax],al
00000048  6800000000        push dword 0x0
0000004D  E900002700        jmp dword 0x270052
00000052  0000              add [eax],al
00000054  0000              add [eax],al
00000056  0000              add [eax],al
00000058  0800              or [eax],al
0000005A  008E00000000      add [esi+0x0],cl
00000060  0800              or [eax],al
00000062  008E00000000      add [esi+0x0],cl
00000068  0800              or [eax],al
0000006A  008E00000000      add [esi+0x0],cl
00000070  0800              or [eax],al
00000072  008E00000000      add [esi+0x0],cl
00000078  0800              or [eax],al
0000007A  008E00000000      add [esi+0x0],cl
00000080  0800              or [eax],al
00000082  008E00000000      add [esi+0x0],cl
00000088  0800              or [eax],al
0000008A  008E00000000      add [esi+0x0],cl
00000090  0800              or [eax],al
00000092  008E00000000      add [esi+0x0],cl
00000098  0800              or [eax],al
0000009A  008E00000000      add [esi+0x0],cl
000000A0  0800              or [eax],al
000000A2  008E00000000      add [esi+0x0],cl
000000A8  0800              or [eax],al
000000AA  008E00000000      add [esi+0x0],cl
000000B0  0800              or [eax],al
000000B2  008E00000000      add [esi+0x0],cl
000000B8  0800              or [eax],al
000000BA  008E00000000      add [esi+0x0],cl
000000C0  0800              or [eax],al
000000C2  008E00000000      add [esi+0x0],cl
000000C8  0800              or [eax],al
000000CA  008E00000000      add [esi+0x0],cl
000000D0  0800              or [eax],al
000000D2  008E00000000      add [esi+0x0],cl
000000D8  0800              or [eax],al
000000DA  008E00000000      add [esi+0x0],cl
000000E0  0800              or [eax],al
000000E2  008E00000000      add [esi+0x0],cl
000000E8  0800              or [eax],al
000000EA  008E00000000      add [esi+0x0],cl
000000F0  0800              or [eax],al
000000F2  008E00000000      add [esi+0x0],cl
000000F8  0800              or [eax],al
000000FA  008E00000000      add [esi+0x0],cl
00000100  0800              or [eax],al
00000102  008E00000000      add [esi+0x0],cl
00000108  0800              or [eax],al
0000010A  008E00000000      add [esi+0x0],cl
00000110  0800              or [eax],al
00000112  008E00000000      add [esi+0x0],cl
00000118  0800              or [eax],al
0000011A  008E00000000      add [esi+0x0],cl
00000120  0800              or [eax],al
00000122  008E00000000      add [esi+0x0],cl
00000128  0800              or [eax],al
0000012A  008E00000000      add [esi+0x0],cl
00000130  0800              or [eax],al
00000132  008E00000000      add [esi+0x0],cl
00000138  0800              or [eax],al
0000013A  008E00000000      add [esi+0x0],cl
00000140  0800              or [eax],al
00000142  008E00000000      add [esi+0x0],cl
00000148  0800              or [eax],al
0000014A  008E00000000      add [esi+0x0],cl
00000150  0800              or [eax],al
00000152  008E00000000      add [esi+0x0],cl
00000158  0800              or [eax],al
0000015A  008E00000000      add [esi+0x0],cl
00000160  0800              or [eax],al
00000162  008E00000000      add [esi+0x0],cl
00000168  0800              or [eax],al
0000016A  008E00000000      add [esi+0x0],cl
00000170  0800              or [eax],al
00000172  008E00000000      add [esi+0x0],cl
00000178  0800              or [eax],al
0000017A  008E00000000      add [esi+0x0],cl
00000180  0800              or [eax],al
00000182  008E00000000      add [esi+0x0],cl
00000188  0800              or [eax],al
0000018A  008E00000000      add [esi+0x0],cl
00000190  0800              or [eax],al
00000192  008E00000000      add [esi+0x0],cl
00000198  0800              or [eax],al
0000019A  008E00000000      add [esi+0x0],cl
000001A0  0800              or [eax],al
000001A2  008E00000000      add [esi+0x0],cl
000001A8  0800              or [eax],al
000001AA  008E00000000      add [esi+0x0],cl
000001B0  0800              or [eax],al
000001B2  008E00000000      add [esi+0x0],cl
000001B8  0800              or [eax],al
000001BA  008E00000000      add [esi+0x0],cl
000001C0  0800              or [eax],al
000001C2  008E00000000      add [esi+0x0],cl
000001C8  0800              or [eax],al
000001CA  008E00000000      add [esi+0x0],cl
000001D0  0800              or [eax],al
000001D2  008E00000000      add [esi+0x0],cl
000001D8  0800              or [eax],al
000001DA  008E00000000      add [esi+0x0],cl
000001E0  0800              or [eax],al
000001E2  008E00000000      add [esi+0x0],cl
000001E8  0800              or [eax],al
000001EA  008E00000000      add [esi+0x0],cl
000001F0  0800              or [eax],al
000001F2  008E00000000      add [esi+0x0],cl
000001F8  0800              or [eax],al
000001FA  008E00000000      add [esi+0x0],cl
00000200  0800              or [eax],al
00000202  008E00000000      add [esi+0x0],cl
00000208  0800              or [eax],al
0000020A  008E00000000      add [esi+0x0],cl
00000210  0800              or [eax],al
00000212  008E00000000      add [esi+0x0],cl
00000218  0800              or [eax],al
0000021A  008E00000000      add [esi+0x0],cl
00000220  0800              or [eax],al
00000222  008E00000000      add [esi+0x0],cl
00000228  0800              or [eax],al
0000022A  008E00000000      add [esi+0x0],cl
00000230  0800              or [eax],al
00000232  008E00000000      add [esi+0x0],cl
00000238  0800              or [eax],al
0000023A  008E00000000      add [esi+0x0],cl
00000240  0800              or [eax],al
00000242  008E00000000      add [esi+0x0],cl
00000248  0800              or [eax],al
0000024A  008E00000000      add [esi+0x0],cl
00000250  0800              or [eax],al
00000252  008E00000000      add [esi+0x0],cl
00000258  0800              or [eax],al
0000025A  008E00000000      add [esi+0x0],cl
00000260  0800              or [eax],al
00000262  008E00000000      add [esi+0x0],cl
00000268  0800              or [eax],al
0000026A  008E00000000      add [esi+0x0],cl
00000270  0800              or [eax],al
00000272  008E00000000      add [esi+0x0],cl
00000278  0800              or [eax],al
0000027A  008E00000000      add [esi+0x0],cl
00000280  0800              or [eax],al
00000282  008E00000000      add [esi+0x0],cl
00000288  0800              or [eax],al
0000028A  008E00000000      add [esi+0x0],cl
00000290  0800              or [eax],al
00000292  008E00000000      add [esi+0x0],cl
00000298  0800              or [eax],al
0000029A  008E00000000      add [esi+0x0],cl
000002A0  0800              or [eax],al
000002A2  008E00000000      add [esi+0x0],cl
000002A8  0800              or [eax],al
000002AA  008E00000000      add [esi+0x0],cl
000002B0  0800              or [eax],al
000002B2  008E00000000      add [esi+0x0],cl
000002B8  0800              or [eax],al
000002BA  008E00000000      add [esi+0x0],cl
000002C0  0800              or [eax],al
000002C2  008E00000000      add [esi+0x0],cl
000002C8  0800              or [eax],al
000002CA  008E00000000      add [esi+0x0],cl
000002D0  0800              or [eax],al
000002D2  008E00000000      add [esi+0x0],cl
000002D8  0800              or [eax],al
000002DA  008E00000000      add [esi+0x0],cl
000002E0  0800              or [eax],al
000002E2  008E00000000      add [esi+0x0],cl
000002E8  0800              or [eax],al
000002EA  008E00000000      add [esi+0x0],cl
000002F0  0800              or [eax],al
000002F2  008E00000000      add [esi+0x0],cl
000002F8  0800              or [eax],al
000002FA  008E00000000      add [esi+0x0],cl
00000300  0800              or [eax],al
00000302  008E00000000      add [esi+0x0],cl
00000308  0800              or [eax],al
0000030A  008E00000000      add [esi+0x0],cl
00000310  0800              or [eax],al
00000312  008E00000000      add [esi+0x0],cl
00000318  0800              or [eax],al
0000031A  008E00000000      add [esi+0x0],cl
00000320  0800              or [eax],al
00000322  008E00000000      add [esi+0x0],cl
00000328  0800              or [eax],al
0000032A  008E00000000      add [esi+0x0],cl
00000330  0800              or [eax],al
00000332  008E00000000      add [esi+0x0],cl
00000338  0800              or [eax],al
0000033A  008E00000000      add [esi+0x0],cl
00000340  0800              or [eax],al
00000342  008E00000000      add [esi+0x0],cl
00000348  0800              or [eax],al
0000034A  008E00000000      add [esi+0x0],cl
00000350  0800              or [eax],al
00000352  008E00000000      add [esi+0x0],cl
00000358  0800              or [eax],al
0000035A  008E00000000      add [esi+0x0],cl
00000360  0800              or [eax],al
00000362  008E00000000      add [esi+0x0],cl
00000368  0800              or [eax],al
0000036A  008E00000000      add [esi+0x0],cl
00000370  0800              or [eax],al
00000372  008E00000000      add [esi+0x0],cl
00000378  0800              or [eax],al
0000037A  008E00000000      add [esi+0x0],cl
00000380  0800              or [eax],al
00000382  008E00000000      add [esi+0x0],cl
00000388  0800              or [eax],al
0000038A  008E00000000      add [esi+0x0],cl
00000390  0800              or [eax],al
00000392  008E00000000      add [esi+0x0],cl
00000398  0800              or [eax],al
0000039A  008E00000000      add [esi+0x0],cl
000003A0  0800              or [eax],al
000003A2  008E00000000      add [esi+0x0],cl
000003A8  0800              or [eax],al
000003AA  008E00000000      add [esi+0x0],cl
000003B0  0800              or [eax],al
000003B2  008E00000000      add [esi+0x0],cl
000003B8  0800              or [eax],al
000003BA  008E00000000      add [esi+0x0],cl
000003C0  0800              or [eax],al
000003C2  008E00000000      add [esi+0x0],cl
000003C8  0800              or [eax],al
000003CA  008E00000000      add [esi+0x0],cl
000003D0  0800              or [eax],al
000003D2  008E00000000      add [esi+0x0],cl
000003D8  0800              or [eax],al
000003DA  008E00000000      add [esi+0x0],cl
000003E0  0800              or [eax],al
000003E2  008E00000000      add [esi+0x0],cl
000003E8  0800              or [eax],al
000003EA  008E00000000      add [esi+0x0],cl
000003F0  0800              or [eax],al
000003F2  008E00000000      add [esi+0x0],cl
000003F8  0800              or [eax],al
000003FA  008E00000000      add [esi+0x0],cl
00000400  0800              or [eax],al
00000402  008E00000000      add [esi+0x0],cl
00000408  0800              or [eax],al
0000040A  008E00000000      add [esi+0x0],cl
00000410  0800              or [eax],al
00000412  008E00000000      add [esi+0x0],cl
00000418  0800              or [eax],al
0000041A  008E00000000      add [esi+0x0],cl
00000420  0800              or [eax],al
00000422  008E00000000      add [esi+0x0],cl
00000428  0800              or [eax],al
0000042A  008E00000000      add [esi+0x0],cl
00000430  0800              or [eax],al
00000432  008E00000000      add [esi+0x0],cl
00000438  0800              or [eax],al
0000043A  008E00000000      add [esi+0x0],cl
00000440  0800              or [eax],al
00000442  008E00000000      add [esi+0x0],cl
00000448  0800              or [eax],al
0000044A  008E00000000      add [esi+0x0],cl
00000450  0800              or [eax],al
00000452  008E00000000      add [esi+0x0],cl
00000458  0800              or [eax],al
0000045A  008E00000000      add [esi+0x0],cl
00000460  0800              or [eax],al
00000462  008E00000000      add [esi+0x0],cl
00000468  0800              or [eax],al
0000046A  008E00000000      add [esi+0x0],cl
00000470  0800              or [eax],al
00000472  008E00000000      add [esi+0x0],cl
00000478  0800              or [eax],al
0000047A  008E00000000      add [esi+0x0],cl
00000480  0800              or [eax],al
00000482  008E00000000      add [esi+0x0],cl
00000488  0800              or [eax],al
0000048A  008E00000000      add [esi+0x0],cl
00000490  0800              or [eax],al
00000492  008E00000000      add [esi+0x0],cl
00000498  0800              or [eax],al
0000049A  008E00000000      add [esi+0x0],cl
000004A0  0800              or [eax],al
000004A2  008E00000000      add [esi+0x0],cl
000004A8  0800              or [eax],al
000004AA  008E00000000      add [esi+0x0],cl
000004B0  0800              or [eax],al
000004B2  008E00000000      add [esi+0x0],cl
000004B8  0800              or [eax],al
000004BA  008E00000000      add [esi+0x0],cl
000004C0  0800              or [eax],al
000004C2  008E00000000      add [esi+0x0],cl
000004C8  0800              or [eax],al
000004CA  008E00000000      add [esi+0x0],cl
000004D0  0800              or [eax],al
000004D2  008E00000000      add [esi+0x0],cl
000004D8  0800              or [eax],al
000004DA  008E00000000      add [esi+0x0],cl
000004E0  0800              or [eax],al
000004E2  008E00000000      add [esi+0x0],cl
000004E8  0800              or [eax],al
000004EA  008E00000000      add [esi+0x0],cl
000004F0  0800              or [eax],al
000004F2  008E00000000      add [esi+0x0],cl
000004F8  0800              or [eax],al
000004FA  008E00000000      add [esi+0x0],cl
00000500  0800              or [eax],al
00000502  008E00000000      add [esi+0x0],cl
00000508  0800              or [eax],al
0000050A  008E00000000      add [esi+0x0],cl
00000510  0800              or [eax],al
00000512  008E00000000      add [esi+0x0],cl
00000518  0800              or [eax],al
0000051A  008E00000000      add [esi+0x0],cl
00000520  0800              or [eax],al
00000522  008E00000000      add [esi+0x0],cl
00000528  0800              or [eax],al
0000052A  008E00000000      add [esi+0x0],cl
00000530  0800              or [eax],al
00000532  008E00000000      add [esi+0x0],cl
00000538  0800              or [eax],al
0000053A  008E00000000      add [esi+0x0],cl
00000540  0800              or [eax],al
00000542  008E00000000      add [esi+0x0],cl
00000548  0800              or [eax],al
0000054A  008E00000000      add [esi+0x0],cl
00000550  0800              or [eax],al
00000552  008E00000000      add [esi+0x0],cl
00000558  0800              or [eax],al
0000055A  008E00000000      add [esi+0x0],cl
00000560  0800              or [eax],al
00000562  008E00000000      add [esi+0x0],cl
00000568  0800              or [eax],al
0000056A  008E00000000      add [esi+0x0],cl
00000570  0800              or [eax],al
00000572  008E00000000      add [esi+0x0],cl
00000578  0800              or [eax],al
0000057A  008E00000000      add [esi+0x0],cl
00000580  0800              or [eax],al
00000582  008E00000000      add [esi+0x0],cl
00000588  0800              or [eax],al
0000058A  008E00000000      add [esi+0x0],cl
00000590  0800              or [eax],al
00000592  008E00000000      add [esi+0x0],cl
00000598  0800              or [eax],al
0000059A  008E00000000      add [esi+0x0],cl
000005A0  0800              or [eax],al
000005A2  008E00000000      add [esi+0x0],cl
000005A8  0800              or [eax],al
000005AA  008E00000000      add [esi+0x0],cl
000005B0  0800              or [eax],al
000005B2  008E00000000      add [esi+0x0],cl
000005B8  0800              or [eax],al
000005BA  008E00000000      add [esi+0x0],cl
000005C0  0800              or [eax],al
000005C2  008E00000000      add [esi+0x0],cl
000005C8  0800              or [eax],al
000005CA  008E00000000      add [esi+0x0],cl
000005D0  0800              or [eax],al
000005D2  008E00000000      add [esi+0x0],cl
000005D8  0800              or [eax],al
000005DA  008E00000000      add [esi+0x0],cl
000005E0  0800              or [eax],al
000005E2  008E00000000      add [esi+0x0],cl
000005E8  0800              or [eax],al
000005EA  008E00000000      add [esi+0x0],cl
000005F0  0800              or [eax],al
000005F2  008E00000000      add [esi+0x0],cl
000005F8  0800              or [eax],al
000005FA  008E00000000      add [esi+0x0],cl
00000600  0800              or [eax],al
00000602  008E00000000      add [esi+0x0],cl
00000608  0800              or [eax],al
0000060A  008E00000000      add [esi+0x0],cl
00000610  0800              or [eax],al
00000612  008E00000000      add [esi+0x0],cl
00000618  0800              or [eax],al
0000061A  008E00000000      add [esi+0x0],cl
00000620  0800              or [eax],al
00000622  008E00000000      add [esi+0x0],cl
00000628  0800              or [eax],al
0000062A  008E00000000      add [esi+0x0],cl
00000630  0800              or [eax],al
00000632  008E00000000      add [esi+0x0],cl
00000638  0800              or [eax],al
0000063A  008E00000000      add [esi+0x0],cl
00000640  0800              or [eax],al
00000642  008E00000000      add [esi+0x0],cl
00000648  0800              or [eax],al
0000064A  008E00000000      add [esi+0x0],cl
00000650  0800              or [eax],al
00000652  008E00000000      add [esi+0x0],cl
00000658  0800              or [eax],al
0000065A  008E00000000      add [esi+0x0],cl
00000660  0800              or [eax],al
00000662  008E00000000      add [esi+0x0],cl
00000668  0800              or [eax],al
0000066A  008E00000000      add [esi+0x0],cl
00000670  0800              or [eax],al
00000672  008E00000000      add [esi+0x0],cl
00000678  0800              or [eax],al
0000067A  008E00000000      add [esi+0x0],cl
00000680  0800              or [eax],al
00000682  008E00000000      add [esi+0x0],cl
00000688  0800              or [eax],al
0000068A  008E00000000      add [esi+0x0],cl
00000690  0800              or [eax],al
00000692  008E00000000      add [esi+0x0],cl
00000698  0800              or [eax],al
0000069A  008E00000000      add [esi+0x0],cl
000006A0  0800              or [eax],al
000006A2  008E00000000      add [esi+0x0],cl
000006A8  0800              or [eax],al
000006AA  008E00000000      add [esi+0x0],cl
000006B0  0800              or [eax],al
000006B2  008E00000000      add [esi+0x0],cl
000006B8  0800              or [eax],al
000006BA  008E00000000      add [esi+0x0],cl
000006C0  0800              or [eax],al
000006C2  008E00000000      add [esi+0x0],cl
000006C8  0800              or [eax],al
000006CA  008E00000000      add [esi+0x0],cl
000006D0  0800              or [eax],al
000006D2  008E00000000      add [esi+0x0],cl
000006D8  0800              or [eax],al
000006DA  008E00000000      add [esi+0x0],cl
000006E0  0800              or [eax],al
000006E2  008E00000000      add [esi+0x0],cl
000006E8  0800              or [eax],al
000006EA  008E00000000      add [esi+0x0],cl
000006F0  0800              or [eax],al
000006F2  008E00000000      add [esi+0x0],cl
000006F8  0800              or [eax],al
000006FA  008E00000000      add [esi+0x0],cl
00000700  0800              or [eax],al
00000702  008E00000000      add [esi+0x0],cl
00000708  0800              or [eax],al
0000070A  008E00000000      add [esi+0x0],cl
00000710  0800              or [eax],al
00000712  008E00000000      add [esi+0x0],cl
00000718  0800              or [eax],al
0000071A  008E00000000      add [esi+0x0],cl
00000720  0800              or [eax],al
00000722  008E00000000      add [esi+0x0],cl
00000728  0800              or [eax],al
0000072A  008E00000000      add [esi+0x0],cl
00000730  0800              or [eax],al
00000732  008E00000000      add [esi+0x0],cl
00000738  0800              or [eax],al
0000073A  008E00000000      add [esi+0x0],cl
00000740  0800              or [eax],al
00000742  008E00000000      add [esi+0x0],cl
00000748  0800              or [eax],al
0000074A  008E00000000      add [esi+0x0],cl
00000750  0800              or [eax],al
00000752  008E00000000      add [esi+0x0],cl
00000758  0800              or [eax],al
0000075A  008E00000000      add [esi+0x0],cl
00000760  0800              or [eax],al
00000762  008E00000000      add [esi+0x0],cl
00000768  0800              or [eax],al
0000076A  008E00000000      add [esi+0x0],cl
00000770  0800              or [eax],al
00000772  008E00000000      add [esi+0x0],cl
00000778  0800              or [eax],al
0000077A  008E00000000      add [esi+0x0],cl
00000780  0800              or [eax],al
00000782  008E00000000      add [esi+0x0],cl
00000788  0800              or [eax],al
0000078A  008E00000000      add [esi+0x0],cl
00000790  0800              or [eax],al
00000792  008E00000000      add [esi+0x0],cl
00000798  0800              or [eax],al
0000079A  008E00000000      add [esi+0x0],cl
000007A0  0800              or [eax],al
000007A2  008E00000000      add [esi+0x0],cl
000007A8  0800              or [eax],al
000007AA  008E00000000      add [esi+0x0],cl
000007B0  0800              or [eax],al
000007B2  008E00000000      add [esi+0x0],cl
000007B8  0800              or [eax],al
000007BA  008E00000000      add [esi+0x0],cl
000007C0  0800              or [eax],al
000007C2  008E00000000      add [esi+0x0],cl
000007C8  0800              or [eax],al
000007CA  008E00000000      add [esi+0x0],cl
000007D0  0800              or [eax],al
000007D2  008E00000000      add [esi+0x0],cl
000007D8  0800              or [eax],al
000007DA  008E00000000      add [esi+0x0],cl
000007E0  0800              or [eax],al
000007E2  008E00000000      add [esi+0x0],cl
000007E8  0800              or [eax],al
000007EA  008E00000000      add [esi+0x0],cl
000007F0  0800              or [eax],al
000007F2  008E00000000      add [esi+0x0],cl
000007F8  0800              or [eax],al
000007FA  008E00000000      add [esi+0x0],cl
00000800  0800              or [eax],al
00000802  008E00000000      add [esi+0x0],cl
00000808  0800              or [eax],al
0000080A  008E00000000      add [esi+0x0],cl
00000810  0800              or [eax],al
00000812  008E00000000      add [esi+0x0],cl
00000818  0800              or [eax],al
0000081A  008E00000000      add [esi+0x0],cl
00000820  0800              or [eax],al
00000822  008E00000000      add [esi+0x0],cl
00000828  0800              or [eax],al
0000082A  008E00000000      add [esi+0x0],cl
00000830  0800              or [eax],al
00000832  008E00000000      add [esi+0x0],cl
00000838  0800              or [eax],al
0000083A  008E00000000      add [esi+0x0],cl
00000840  0800              or [eax],al
00000842  008E00000000      add [esi+0x0],cl
00000848  0800              or [eax],al
0000084A  008E00000000      add [esi+0x0],cl
00000850  0800              or [eax],al
00000852  008E0000FF07      add [esi+0x7ff0000],cl
00000858  0000              add [eax],al
0000085A  0000              add [eax],al
0000085C  4C                dec esp
0000085D  6F                outsd
0000085E  61                popad
0000085F  64696E6720202020  imul ebp,[fs:esi+0x67],dword 0x20202020
00000867  007265            add [edx+0x65],dh
0000086A  61                popad
0000086B  64792E            fs jns 0x89c
0000086E  2E2E2E2E2E004E4F  add [cs:esi+0x4f],cl
00000876  2020              and [eax],ah
00000878  204B45            and [ebx+0x45],cl
0000087B  52                push edx
0000087C  4E                dec esi
0000087D  45                inc ebp
0000087E  4C                dec esp
0000087F  0000              add [eax],al
00000881  0000              add [eax],al
00000883  00696D            add [ecx+0x6d],ch
00000886  61                popad
00000887  67650012          add [gs:bp+si],dl
0000088B  2400              and al,0x0
0000088D  0000              add [eax],al
0000088F  0000              add [eax],al
00000891  0000              add [eax],al
00000893  0000              add [eax],al
00000895  0000              add [eax],al
00000897  0000              add [eax],al
00000899  0000              add [eax],al
0000089B  0000              add [eax],al
0000089D  0000              add [eax],al
0000089F  0000              add [eax],al
000008A1  0000              add [eax],al
000008A3  0000              add [eax],al
000008A5  0000              add [eax],al
000008A7  0000              add [eax],al
000008A9  0000              add [eax],al
000008AB  0000              add [eax],al
000008AD  0000              add [eax],al
000008AF  0000              add [eax],al
000008B1  0000              add [eax],al
000008B3  0000              add [eax],al
000008B5  0000              add [eax],al
000008B7  0000              add [eax],al
000008B9  0000              add [eax],al
000008BB  0000              add [eax],al
000008BD  0000              add [eax],al
000008BF  0000              add [eax],al
000008C1  0000              add [eax],al
000008C3  0000              add [eax],al
000008C5  0000              add [eax],al
000008C7  0000              add [eax],al
000008C9  0000              add [eax],al
000008CB  0000              add [eax],al
000008CD  0000              add [eax],al
000008CF  0000              add [eax],al
000008D1  0000              add [eax],al
000008D3  0000              add [eax],al
000008D5  0000              add [eax],al
000008D7  0000              add [eax],al
000008D9  0000              add [eax],al
000008DB  0000              add [eax],al
000008DD  0000              add [eax],al
000008DF  0000              add [eax],al
000008E1  0000              add [eax],al
000008E3  0000              add [eax],al
000008E5  0000              add [eax],al
000008E7  0000              add [eax],al
000008E9  0000              add [eax],al
000008EB  0000              add [eax],al
000008ED  0000              add [eax],al
000008EF  0000              add [eax],al
000008F1  0000              add [eax],al
000008F3  0000              add [eax],al
000008F5  00FF              add bh,bh
000008F7  0000              add [eax],al
000008F9  0000              add [eax],al
000008FB  0000              add [eax],al
000008FD  0000              add [eax],al
000008FF  0000              add [eax],al
00000901  0000              add [eax],al
00000903  0000              add [eax],al
00000905  0000              add [eax],al
00000907  0000              add [eax],al
00000909  0000              add [eax],al
0000090B  0000              add [eax],al
0000090D  0000              add [eax],al
0000090F  0000              add [eax],al
00000911  0000              add [eax],al
00000913  0000              add [eax],al
00000915  0000              add [eax],al
00000917  0000              add [eax],al
00000919  0000              add [eax],al
0000091B  0000              add [eax],al
0000091D  0000              add [eax],al
0000091F  0000              add [eax],al
00000921  0000              add [eax],al
00000923  0000              add [eax],al
00000925  0000              add [eax],al
00000927  0000              add [eax],al
00000929  0000              add [eax],al
0000092B  0000              add [eax],al
0000092D  0000              add [eax],al
0000092F  0000              add [eax],al
00000931  0000              add [eax],al
00000933  0000              add [eax],al
00000935  0000              add [eax],al
00000937  0000              add [eax],al
00000939  0000              add [eax],al
0000093B  0000              add [eax],al
0000093D  0000              add [eax],al
0000093F  0000              add [eax],al
00000941  0000              add [eax],al
00000943  0000              add [eax],al
00000945  0000              add [eax],al
00000947  0000              add [eax],al
00000949  0000              add [eax],al
0000094B  0000              add [eax],al
0000094D  0000              add [eax],al
0000094F  0000              add [eax],al
00000951  0000              add [eax],al
00000953  0000              add [eax],al
00000955  0000              add [eax],al
00000957  0000              add [eax],al
00000959  0000              add [eax],al
0000095B  0000              add [eax],al
0000095D  0000              add [eax],al
0000095F  0000              add [eax],al
00000961  0000              add [eax],al
00000963  0000              add [eax],al
00000965  0000              add [eax],al
00000967  0000              add [eax],al
00000969  0000              add [eax],al
0000096B  0000              add [eax],al
0000096D  0000              add [eax],al
0000096F  0000              add [eax],al
00000971  0000              add [eax],al
00000973  0000              add [eax],al
00000975  0000              add [eax],al
00000977  0000              add [eax],al
00000979  0000              add [eax],al
0000097B  0000              add [eax],al
0000097D  0000              add [eax],al
0000097F  0000              add [eax],al
00000981  0000              add [eax],al
00000983  0000              add [eax],al
00000985  0000              add [eax],al
00000987  0000              add [eax],al
00000989  0000              add [eax],al
0000098B  0000              add [eax],al
0000098D  0000              add [eax],al
0000098F  0000              add [eax],al
00000991  0000              add [eax],al
00000993  0000              add [eax],al
00000995  0000              add [eax],al
00000997  0000              add [eax],al
00000999  0000              add [eax],al
0000099B  0000              add [eax],al
0000099D  0000              add [eax],al
0000099F  0000              add [eax],al
000009A1  0000              add [eax],al
000009A3  0000              add [eax],al
000009A5  0000              add [eax],al
000009A7  0000              add [eax],al
000009A9  0000              add [eax],al
000009AB  0000              add [eax],al
000009AD  0000              add [eax],al
000009AF  0000              add [eax],al
000009B1  0000              add [eax],al
000009B3  0000              add [eax],al
000009B5  0000              add [eax],al
000009B7  0000              add [eax],al
000009B9  0000              add [eax],al
000009BB  0000              add [eax],al
000009BD  0000              add [eax],al
000009BF  0000              add [eax],al
000009C1  0000              add [eax],al
000009C3  0000              add [eax],al
000009C5  0000              add [eax],al
000009C7  0000              add [eax],al
000009C9  0000              add [eax],al
000009CB  0000              add [eax],al
000009CD  0000              add [eax],al
000009CF  0000              add [eax],al
000009D1  0000              add [eax],al
000009D3  0000              add [eax],al
000009D5  0000              add [eax],al
000009D7  0000              add [eax],al
000009D9  0000              add [eax],al
000009DB  0000              add [eax],al
000009DD  0000              add [eax],al
000009DF  0000              add [eax],al
000009E1  0000              add [eax],al
000009E3  0000              add [eax],al
000009E5  0000              add [eax],al
000009E7  0000              add [eax],al
000009E9  0000              add [eax],al
000009EB  0000              add [eax],al
000009ED  0000              add [eax],al
000009EF  0000              add [eax],al
000009F1  0000              add [eax],al
000009F3  0000              add [eax],al
000009F5  0000              add [eax],al
000009F7  0000              add [eax],al
000009F9  0000              add [eax],al
000009FB  0000              add [eax],al
000009FD  0000              add [eax],al
000009FF  0000              add [eax],al
00000A01  0000              add [eax],al
00000A03  0000              add [eax],al
00000A05  0000              add [eax],al
00000A07  0000              add [eax],al
00000A09  0000              add [eax],al
00000A0B  0000              add [eax],al
00000A0D  0000              add [eax],al
00000A0F  0000              add [eax],al
00000A11  0000              add [eax],al
00000A13  0000              add [eax],al
00000A15  0000              add [eax],al
00000A17  0000              add [eax],al
00000A19  0000              add [eax],al
00000A1B  0000              add [eax],al
00000A1D  0000              add [eax],al
00000A1F  0000              add [eax],al
00000A21  0000              add [eax],al
00000A23  0000              add [eax],al
00000A25  0000              add [eax],al
00000A27  0000              add [eax],al
00000A29  0000              add [eax],al
00000A2B  0000              add [eax],al
00000A2D  0000              add [eax],al
00000A2F  0000              add [eax],al
00000A31  0000              add [eax],al
00000A33  0000              add [eax],al
00000A35  0000              add [eax],al
00000A37  0000              add [eax],al
00000A39  0000              add [eax],al
00000A3B  0000              add [eax],al
00000A3D  0000              add [eax],al
00000A3F  0000              add [eax],al
00000A41  0000              add [eax],al
00000A43  0000              add [eax],al
00000A45  0000              add [eax],al
00000A47  0000              add [eax],al
00000A49  0000              add [eax],al
00000A4B  0000              add [eax],al
00000A4D  0000              add [eax],al
00000A4F  0000              add [eax],al
00000A51  0000              add [eax],al
00000A53  0000              add [eax],al
00000A55  0000              add [eax],al
00000A57  0000              add [eax],al
00000A59  0000              add [eax],al
00000A5B  0000              add [eax],al
00000A5D  0000              add [eax],al
00000A5F  0000              add [eax],al
00000A61  0000              add [eax],al
00000A63  0000              add [eax],al
00000A65  0000              add [eax],al
00000A67  0000              add [eax],al
00000A69  0000              add [eax],al
00000A6B  0000              add [eax],al
00000A6D  0000              add [eax],al
00000A6F  0000              add [eax],al
00000A71  0000              add [eax],al
00000A73  0000              add [eax],al
00000A75  0000              add [eax],al
00000A77  0000              add [eax],al
00000A79  0000              add [eax],al
00000A7B  0000              add [eax],al
00000A7D  0000              add [eax],al
00000A7F  0000              add [eax],al
00000A81  0000              add [eax],al
00000A83  0000              add [eax],al
00000A85  0000              add [eax],al
00000A87  0000              add [eax],al
00000A89  0000              add [eax],al
00000A8B  0000              add [eax],al
00000A8D  0000              add [eax],al
00000A8F  0000              add [eax],al
00000A91  0000              add [eax],al
00000A93  0000              add [eax],al
00000A95  0000              add [eax],al
00000A97  0000              add [eax],al
00000A99  0000              add [eax],al
00000A9B  0000              add [eax],al
00000A9D  0000              add [eax],al
00000A9F  0000              add [eax],al
00000AA1  0000              add [eax],al
00000AA3  0000              add [eax],al
00000AA5  0000              add [eax],al
00000AA7  0000              add [eax],al
00000AA9  0000              add [eax],al
00000AAB  0000              add [eax],al
00000AAD  0000              add [eax],al
00000AAF  0000              add [eax],al
00000AB1  0000              add [eax],al
00000AB3  0000              add [eax],al
00000AB5  0000              add [eax],al
00000AB7  0000              add [eax],al
00000AB9  0000              add [eax],al
00000ABB  0000              add [eax],al
00000ABD  0000              add [eax],al
00000ABF  0000              add [eax],al
00000AC1  0000              add [eax],al
00000AC3  0000              add [eax],al
00000AC5  0000              add [eax],al
00000AC7  0000              add [eax],al
00000AC9  0000              add [eax],al
00000ACB  0000              add [eax],al
00000ACD  0000              add [eax],al
00000ACF  0000              add [eax],al
00000AD1  0000              add [eax],al
00000AD3  0000              add [eax],al
00000AD5  0000              add [eax],al
00000AD7  0000              add [eax],al
00000AD9  0000              add [eax],al
00000ADB  0000              add [eax],al
00000ADD  0000              add [eax],al
00000ADF  0000              add [eax],al
00000AE1  0000              add [eax],al
00000AE3  0000              add [eax],al
00000AE5  0000              add [eax],al
00000AE7  0000              add [eax],al
00000AE9  0000              add [eax],al
00000AEB  0000              add [eax],al
00000AED  0000              add [eax],al
00000AEF  0000              add [eax],al
00000AF1  0000              add [eax],al
00000AF3  0000              add [eax],al
00000AF5  0000              add [eax],al
00000AF7  0000              add [eax],al
00000AF9  0000              add [eax],al
00000AFB  0000              add [eax],al
00000AFD  0000              add [eax],al
00000AFF  0000              add [eax],al
00000B01  0000              add [eax],al
00000B03  0000              add [eax],al
00000B05  0000              add [eax],al
00000B07  0000              add [eax],al
00000B09  0000              add [eax],al
00000B0B  0000              add [eax],al
00000B0D  0000              add [eax],al
00000B0F  0000              add [eax],al
00000B11  0002              add [edx],al
00000B13  008CC88ED88ED0    add [eax+ecx*8-0x2f712772],cl
00000B1A  BC0002B800        mov esp,0xb80200
00000B1F  B88EE8BB00        mov eax,0xbbe88e
00000B24  0065C6            add [ebp-0x3a],ah
00000B27  07                pop es
00000B28  2083C30281FB      and [ebx-0x47efd3d],al
00000B2E  A00F75F3B4        mov al,[0xb4f3750f]
00000B33  00B003CD1031      add [eax+0x3110cd03],dh
00000B39  C0                db 0xc0
00000B3A  31DB              xor ebx,ebx
00000B3C  B40B              mov ah,0xb
00000B3E  B300              mov bl,0x0
00000B40  CD10              int 0x10
00000B42  B600              mov dh,0x0
00000B44  E85F036A02        call dword 0x26a0ea8
00000B49  6800D06A01        push dword 0x16ad000
00000B4E  6A00              push byte +0x0
00000B50  6A04              push byte +0x4
00000B52  E858026683        call dword 0x83660daf
00000B57  C40A              les ecx,[edx]
00000B59  6A02              push byte +0x2
00000B5B  6800C06A01        push dword 0x16ac000
00000B60  6A00              push byte +0x0
00000B62  6A10              push byte +0x10
00000B64  E846026683        call dword 0x83660daf
00000B69  C40A              les ecx,[edx]
00000B6B  B8002C8EC0        mov eax,0xc08e2c00
00000B70  31F6              xor esi,esi
00000B72  3E66C706FB0C      mov word [ds:esi],0xcfb
00000B78  0000              add [eax],al
00000B7A  0000              add [eax],al
00000B7C  3E66833EFB        cmp word [ds:esi],byte -0x5
00000B81  0C40              or al,0x40
00000B83  7724              ja 0xba9
00000B85  BF840A83C6        mov edi,0xc6830a84
00000B8A  043E              add al,0x3e
00000B8C  8A053C00741E      mov al,[dword 0x1e74003c]
00000B92  263A0475044647EB  cmp al,[es:esi*2-0x14b8b9fc]
00000B9A  F083E6C0          lock and esi,byte -0x40
00000B9E  83C640            add esi,byte +0x40
00000BA1  3E66FF06          inc word [ds:esi]
00000BA5  FB                sti
00000BA6  0CEB              or al,0xeb
00000BA8  D3                db 0xd3
00000BA9  B602              mov dh,0x2
00000BAB  E8F802EBFE        call dword 0xfeeb0ea8
00000BB0  B601              mov dh,0x1
00000BB2  E8F10283E6        call dword 0xe6830ea8
00000BB7  C02666            shl byte [esi],0x66
00000BBA  8B043E            mov eax,[esi+edi]
00000BBD  66A3090DB800      mov [0xb80d09],ax
00000BC3  2D8EC031F6        sub eax,0xf631c08e
00000BC8  3E66FF0E          dec word [ds:esi]
00000BCC  090D3E66833E      or [dword 0x3e83663e],ecx
00000BD2  090D00740583      or [dword 0x83057400],ecx
00000BD8  C640EBEC          mov byte [eax-0x15],0xec
00000BDC  83C608            add esi,byte +0x8
00000BDF  26668B0483        mov ax,[es:ebx+eax*4]
00000BE4  EE                out dx,al
00000BE5  086605            or [esi+0x5],ah
00000BE8  FF0F              dec dword [edi]
00000BEA  0000              add [eax],al
00000BEC  66C1E80C          shr ax,0xc
00000BF0  3E66A30D0D663D    mov [ds:0x3d660d0d],ax
00000BF7  0008              add [eax],cl
00000BF9  0000              add [eax],al
00000BFB  0F8F1F016683      jg dword 0x83660d20
00000C01  F8                clc
00000C02  07                pop es
00000C03  0F87830083C6      ja dword 0xc6830c8c
00000C09  183E              sbb [esi],bh
00000C0B  8936              mov [esi],esi
00000C0D  030D6A036A00      add ecx,[dword 0x6a036a]
00000C13  6A01              push byte +0x1
00000C15  26FF740226        push dword [es:edx+eax+0x26]
00000C1A  FF34E8            push dword [eax+ebp*8]
00000C1D  8E01              mov es,[ecx]
00000C1F  6683C40A          add sp,byte +0xa
00000C23  3E8B1E            mov ebx,[ds:esi]
00000C26  110D83C30883      adc [dword 0x8308c383],ecx
00000C2C  C3                ret
00000C2D  2083C3043E66      and [ebx+0x663e04c3],al
00000C33  FF0E              dec dword [esi]
00000C35  0D0DB80030        or eax,0x3000b80d
00000C3A  8ED8              mov ds,ax
00000C3C  83C604            add esi,byte +0x4
00000C3F  3E66833E0D        cmp word [ds:esi],byte +0xd
00000C44  0D000F84D6        or eax,0xd6840f00
00000C49  00B800108ED8      add [eax-0x2771f000],bh
00000C4F  3E8B36            mov esi,[ds:esi]
00000C52  030DB800308E      add ecx,[dword 0x8e3000b8]
00000C58  C026FF            shl byte [esi],0xff
00000C5B  7702              ja 0xc5f
00000C5D  26FF37            push dword [es:edi]
00000C60  6A01              push byte +0x1
00000C62  3EFF74023E        push dword [ds:edx+eax+0x3e]
00000C67  FF34E8            push dword [eax+ebp*8]
00000C6A  41                inc ecx
00000C6B  0183C40AB800      add [ebx+0xb80ac4],eax
00000C71  108ED83E66FF      adc [esi-0x99c128],cl
00000C77  0E                push cs
00000C78  0D0D3E8306        or eax,0x6833e0d
00000C7D  030D04B8002D      add ecx,[dword 0x2d00b804]
00000C83  8EC0              mov es,ax
00000C85  83C304            add ebx,byte +0x4
00000C88  EBB5              jmp short 0xc3f
00000C8A  83C618            add esi,byte +0x18
00000C8D  83C61C            add esi,byte +0x1c
00000C90  6A02              push byte +0x2
00000C92  6800F06A01        push dword 0x16af000
00000C97  26FF740226        push dword [es:edx+eax+0x26]
00000C9C  FF34E8            push dword [eax+ebp*8]
00000C9F  0C01              or al,0x1
00000CA1  6683C40A          add sp,byte +0xa
00000CA5  B8002F8EC0        mov eax,0xc08e2f00
00000CAA  31F6              xor esi,esi
00000CAC  6A03              push byte +0x3
00000CAE  6A00              push byte +0x0
00000CB0  6A01              push byte +0x1
00000CB2  26FF740226        push dword [es:edx+eax+0x26]
00000CB7  FF34E8            push dword [eax+ebp*8]
00000CBA  F1                int1
00000CBB  006683            add [esi-0x7d],ah
00000CBE  C40A              les ecx,[edx]
00000CC0  B800108ED8        mov eax,0xd88e1000
00000CC5  3E8B1E            mov ebx,[ds:esi]
00000CC8  110D83C30883      adc [dword 0x8308c383],ecx
00000CCE  C3                ret
00000CCF  2083C30483C6      and [ebx-0x397cfb3d],al
00000CD5  043E              add al,0x3e
00000CD7  66FF0E            dec word [esi]
00000CDA  0D0D3E891E        or eax,0x1e893e0d
00000CDF  030D3E66833E      add ecx,[dword 0x3e83663e]
00000CE5  0D0D007436        or eax,0x3674000d
00000CEA  B8002F8ED8        mov eax,0xd88e2f00
00000CEF  B800308EC0        mov eax,0xc08e3000
00000CF4  26FF7702          push dword [es:edi+0x2]
00000CF8  26FF37            push dword [es:edi]
00000CFB  6A01              push byte +0x1
00000CFD  3EFF74023E        push dword [ds:edx+eax+0x3e]
00000D02  FF34E8            push dword [eax+ebp*8]
00000D05  A6                cmpsb
00000D06  006683            add [esi-0x7d],ah
00000D09  C40A              les ecx,[edx]
00000D0B  B800108ED8        mov eax,0xd88e1000
00000D10  3E66FF0E          dec word [ds:esi]
00000D14  0D0D83C304        or eax,0x4c3830d
00000D19  83C604            add esi,byte +0x4
00000D1C  EBC3              jmp short 0xce1
00000D1E  EBFE              jmp short 0xd1e
00000D20  E8810065C6        call dword 0xc6650da6
00000D25  06                push es
00000D26  9E                sahf
00000D27  0F426631          cmovc esp,[esi+0x31]
00000D2B  C08CC866C1E00466  ror byte [eax+ecx*8+0x4e0c166],0x66
00000D33  0528020000        add eax,0x228
00000D38  66A352026631      mov [0x31660252],ax
00000D3E  C08CC866C1E00466  ror byte [eax+ecx*8+0x4e0c166],0x66
00000D46  058E0A0000        add eax,0xa8e
00000D4B  A34A0266C1        mov [0xc166024a],eax
00000D50  E810A24C02        call dword 0x24caf65
00000D55  8826              mov [esi],ah
00000D57  4F                dec edi
00000D58  026631            add ah,[esi+0x31]
00000D5B  C08CC866C1E00466  ror byte [eax+ecx*8+0x4e0c166],0x66
00000D63  0556020000        add eax,0x256
00000D68  66A3580A6631      mov [0x31660a58],ax
00000D6E  C066B83D          shl byte [esi-0x48],0x3d
00000D72  1101              adc [ecx],eax
00000D74  00A3560266C1      add [ebx-0x3e99fdaa],ah
00000D7A  E810A35C02        call dword 0x25cb08f
00000D7F  FA                cli
00000D80  0F0116            lgdt [esi]
00000D83  50                push eax
00000D84  020F              add cl,[edi]
00000D86  011E              add [esi],ebx
00000D88  56                push esi
00000D89  0AE4              or ah,ah
00000D8B  92                xchg eax,edx
00000D8C  0C02              or al,0x2
00000D8E  E692              out 0x92,al
00000D90  0F20C0            mov eax,cr0
00000D93  6683C801          or ax,byte +0x1
00000D97  0F22C0            mov cr0,eax
00000D9A  66EAE0100100      jmp word 0x1:0x10e0
00000DA0  0800              or [eax],al
00000DA2  EBFE              jmp short 0xda2
00000DA4  52                push edx
00000DA5  BAF203B000        mov edx,0xb003f2
00000DAA  EE                out dx,al
00000DAB  5A                pop edx
00000DAC  C3                ret
00000DAD  55                push ebp
00000DAE  89E5              mov ebp,esp
00000DB0  50                push eax
00000DB1  53                push ebx
00000DB2  51                push ecx
00000DB3  52                push edx
00000DB4  6631C0            xor ax,ax
00000DB7  36668B460A        mov ax,[ss:esi+0xa]
00000DBC  6625FFFF          and ax,0xffff
00000DC0  0F0066C1          verr [esi-0x3f]
00000DC4  E8048EC0BB        call dword 0xbbc09bcd
00000DC9  0010              add [eax],dl
00000DCB  8EDB              mov ds,bx
00000DCD  6631DB            xor bx,bx
00000DD0  368B5E06          mov ebx,[ss:esi+0x6]
00000DD4  66C1E310          shl bx,0x10
00000DD8  368B5E04          mov ebx,[ss:esi+0x4]
00000DDC  66C1E303          shl bx,0x3
00000DE0  6689DA            mov dx,bx
00000DE3  6653              push bx
00000DE5  368A4608          mov al,[ss:esi+0x8]
00000DE9  C0E003            shl al,0x3
00000DEC  36884608          mov [ss:esi+0x8],al
00000DF0  36884609          mov [ss:esi+0x9],al
00000DF4  36837E0800        cmp dword [ss:esi+0x8],byte +0x0
00000DF9  0F849F008CC3      jz dword 0xc38c0e9e
00000DFF  89DA              mov edx,ebx
00000E01  81C2400281E2      add edx,0xe2810240
00000E07  00F0              add al,dh
00000E09  81E300F039DA      and ebx,0xda39f000
00000E0F  7411              jz 0xe22
00000E11  8CC3              mov ebx,es
00000E13  29DA              sub edx,ebx
00000E15  C1EA05            shr edx,0x5
00000E18  363A5608          cmp dl,[ss:esi+0x8]
00000E1C  7704              ja 0xe22
00000E1E  36885608          mov [ss:esi+0x8],dl
00000E22  31DB              xor ebx,ebx
00000E24  368B56F6          mov edx,[ss:esi-0xa]
00000E28  368B46F4          mov eax,[ss:esi-0xc]
00000E2C  3EF736            div dword [ds:esi]
00000E2F  8B0A              mov ecx,[edx]
00000E31  92                xchg eax,edx
00000E32  3EF636            div byte [ds:esi]
00000E35  8A0A              mov cl,[edx]
00000E37  88F1              mov cl,dh
00000E39  C0E106            shl cl,0x6
00000E3C  08E1              or cl,ah
00000E3E  FEC1              inc cl
00000E40  88D5              mov ch,dl
00000E42  88C6              mov dh,al
00000E44  3E8A16            mov dl,[ds:esi]
00000E47  8D0A              lea ecx,[edx]
00000E49  3EA08A0A28E0      mov al,[ds:0xe0280a8a]
00000E4F  363A4608          cmp al,[ss:esi+0x8]
00000E53  7604              jna 0xe59
00000E55  368A4608          mov al,[ss:esi+0x8]
00000E59  30E4              xor ah,ah
00000E5B  CD13              int 0x13
00000E5D  B402              mov ah,0x2
00000E5F  CD13              int 0x13
00000E61  80FC00            cmp ah,0x0
00000E64  758E              jnz 0xdf4
00000E66  30E4              xor ah,ah
00000E68  36284608          sub [ss:esi+0x8],al
00000E6C  36284609          sub [ss:esi+0x9],al
00000E70  360146F4          add [ss:esi-0xc],eax
00000E74  C1E005            shl eax,0x5
00000E77  8CC3              mov ebx,es
00000E79  01C3              add ebx,eax
00000E7B  8EC3              mov es,bx
00000E7D  36807E0800        cmp byte [ss:esi+0x8],0x0
00000E82  0F856EFF36C6      jnz dword 0xc6370df6
00000E88  46                inc esi
00000E89  0812              or [edx],dl
00000E8B  368A4609          mov al,[ss:esi+0x9]
00000E8F  3C12              cmp al,0x12
00000E91  0F875FFF3688      ja dword 0x88370df6
00000E97  46                inc esi
00000E98  08E9              or cl,ch
00000E9A  58                pop eax
00000E9B  FF665B            jmp dword near [esi+0x5b]
00000E9E  5A                pop edx
00000E9F  59                pop ecx
00000EA0  5B                pop ebx
00000EA1  58                pop eax
00000EA2  89EC              mov esp,ebp
00000EA4  5D                pop ebp
00000EA5  C3                ret
00000EA6  56                push esi
00000EA7  50                push eax
00000EA8  53                push ebx
00000EA9  51                push ecx
00000EAA  52                push edx
00000EAB  BE5C0AB0A0        mov esi,0xa0b00a5c
00000EB0  F6E6              mul dh
00000EB2  89C7              mov edi,eax
00000EB4  31C0              xor eax,eax
00000EB6  B00C              mov al,0xc
00000EB8  F6E6              mul dh
00000EBA  01C6              add esi,eax
00000EBC  803C0074          cmp byte [eax+eax],0x74
00000EC0  0B8A04658805      or ecx,[edx+0x5886504]
00000EC6  46                inc esi
00000EC7  83C702            add edi,byte +0x2
00000ECA  EBF0              jmp short 0xebc
00000ECC  5A                pop edx
00000ECD  59                pop ecx
00000ECE  5B                pop ebx
00000ECF  58                pop eax
00000ED0  5E                pop esi
00000ED1  C3                ret
00000ED2  0000              add [eax],al
00000ED4  0000              add [eax],al
00000ED6  0000              add [eax],al
00000ED8  0000              add [eax],al
00000EDA  0000              add [eax],al
00000EDC  0000              add [eax],al
00000EDE  0000              add [eax],al
00000EE0  66B81000          mov ax,0x10
00000EE4  8EC0              mov es,ax
00000EE6  8ED8              mov ds,ax
00000EE8  8ED0              mov ss,ax
00000EEA  8EE0              mov fs,ax
00000EEC  66B81B00          mov ax,0x1b
00000EF0  8EE8              mov gs,ax
00000EF2  65C6055802000050  mov byte [dword gs:0x258],0x50
00000EFA  65C6055A0200004D  mov byte [dword gs:0x25a],0x4d
00000F02  BCF70C0100        mov esp,0x10cf7
00000F07  FA                cli
00000F08  EA000203000800    jmp dword 0x8:0x30200
00000F0F  EBFE              jmp short 0xf0f
00000F11  55                push ebp
00000F12  89E5              mov ebp,esp
00000F14  56                push esi
00000F15  57                push edi
00000F16  50                push eax
00000F17  53                push ebx
00000F18  51                push ecx
00000F19  52                push edx
00000F1A  8B4D10            mov ecx,[ebp+0x10]
00000F1D  8B750C            mov esi,[ebp+0xc]
00000F20  8B7D08            mov edi,[ebp+0x8]
00000F23  83F900            cmp ecx,byte +0x0
00000F26  740B              jz 0xf33
00000F28  268A1E            mov bl,[es:esi]
00000F2B  3E881F            mov [ds:edi],bl
00000F2E  46                inc esi
00000F2F  47                inc edi
00000F30  49                dec ecx
00000F31  EBF0              jmp short 0xf23
00000F33  5A                pop edx
00000F34  59                pop ecx
00000F35  5B                pop ebx
00000F36  58                pop eax
00000F37  5F                pop edi
00000F38  5E                pop esi
00000F39  89EC              mov esp,ebp
00000F3B  5D                pop ebp
00000F3C  C3                ret
00000F3D  50                push eax
00000F3E  53                push ebx
00000F3F  66BB1000          mov bx,0x10
00000F43  8EDB              mov ds,bx
00000F45  3E8B1D800A0100    mov ebx,[dword ds:0x10a80]
00000F4C  D1E3              shl ebx,1
00000F4E  65C6034E          mov byte [gs:ebx],0x4e
00000F52  D1EB              shr ebx,1
00000F54  43                inc ebx
00000F55  3E891D800A0100    mov [dword ds:0x10a80],ebx
00000F5C  B020              mov al,0x20
00000F5E  E620              out 0x20,al
00000F60  5B                pop ebx
00000F61  58                pop eax
00000F62  CF                iretd
00000F63  90                nop
00000F64  90                nop
00000F65  90                nop
00000F66  C3                ret
00000F67  55                push ebp
00000F68  89E5              mov ebp,esp
00000F6A  60                pushad
00000F6B  8B4D08            mov ecx,[ebp+0x8]
00000F6E  8B7D0C            mov edi,[ebp+0xc]
00000F71  83F900            cmp ecx,byte +0x0
00000F74  7408              jz 0xf7e
00000F76  3EC60700          mov byte [ds:edi],0x0
00000F7A  47                inc edi
00000F7B  49                dec ecx
00000F7C  EBF3              jmp short 0xf71
00000F7E  61                popad
00000F7F  89EC              mov esp,ebp
00000F81  5D                pop ebp
00000F82  C3                ret
