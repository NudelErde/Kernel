
../badsh/badsh.elf:     file format elf64-x86-64


Disassembly of section .text:

0000000060000000 <exit(unsigned long)>:
    60000000:	55                   	push   %rbp
    60000001:	48 89 e5             	mov    %rsp,%rbp
    60000004:	48 83 ec 10          	sub    $0x10,%rsp
    60000008:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    6000000c:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000010:	b9 00 00 00 00       	mov    $0x0,%ecx
    60000015:	48 89 c2             	mov    %rax,%rdx
    60000018:	be 01 00 00 00       	mov    $0x1,%esi
    6000001d:	bf 01 00 00 00       	mov    $0x1,%edi
    60000022:	e8 c2 11 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    60000027:	90                   	nop
    60000028:	c9                   	leaveq 
    60000029:	c3                   	retq   

000000006000002a <getpid()>:
    6000002a:	55                   	push   %rbp
    6000002b:	48 89 e5             	mov    %rsp,%rbp
    6000002e:	48 83 ec 10          	sub    $0x10,%rsp
    60000032:	48 8d 45 f8          	lea    -0x8(%rbp),%rax
    60000036:	b9 00 00 00 00       	mov    $0x0,%ecx
    6000003b:	48 89 c2             	mov    %rax,%rdx
    6000003e:	be 02 00 00 00       	mov    $0x2,%esi
    60000043:	bf 01 00 00 00       	mov    $0x1,%edi
    60000048:	e8 9c 11 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    6000004d:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000051:	c9                   	leaveq 
    60000052:	c3                   	retq   

0000000060000053 <createProcess(unsigned long, char const*, char const*, bool)>:
    60000053:	55                   	push   %rbp
    60000054:	48 89 e5             	mov    %rsp,%rbp
    60000057:	48 83 ec 50          	sub    $0x50,%rsp
    6000005b:	48 89 7d c8          	mov    %rdi,-0x38(%rbp)
    6000005f:	48 89 75 c0          	mov    %rsi,-0x40(%rbp)
    60000063:	48 89 55 b8          	mov    %rdx,-0x48(%rbp)
    60000067:	89 c8                	mov    %ecx,%eax
    60000069:	88 45 b4             	mov    %al,-0x4c(%rbp)
    6000006c:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
    60000070:	48 89 45 d8          	mov    %rax,-0x28(%rbp)
    60000074:	48 8b 45 c0          	mov    -0x40(%rbp),%rax
    60000078:	48 89 45 e0          	mov    %rax,-0x20(%rbp)
    6000007c:	48 8b 45 b8          	mov    -0x48(%rbp),%rax
    60000080:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
    60000084:	0f b6 45 b4          	movzbl -0x4c(%rbp),%eax
    60000088:	88 45 f8             	mov    %al,-0x8(%rbp)
    6000008b:	48 8d 45 d8          	lea    -0x28(%rbp),%rax
    6000008f:	b9 00 00 00 00       	mov    $0x0,%ecx
    60000094:	48 89 c2             	mov    %rax,%rdx
    60000097:	be 03 00 00 00       	mov    $0x3,%esi
    6000009c:	bf 01 00 00 00       	mov    $0x1,%edi
    600000a1:	e8 43 11 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    600000a6:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    600000aa:	c9                   	leaveq 
    600000ab:	c3                   	retq   

00000000600000ac <sleep(unsigned long)>:
    600000ac:	55                   	push   %rbp
    600000ad:	48 89 e5             	mov    %rsp,%rbp
    600000b0:	48 83 ec 10          	sub    $0x10,%rsp
    600000b4:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    600000b8:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600000bc:	b9 00 00 00 00       	mov    $0x0,%ecx
    600000c1:	48 89 c2             	mov    %rax,%rdx
    600000c4:	be 04 00 00 00       	mov    $0x4,%esi
    600000c9:	bf 01 00 00 00       	mov    $0x1,%edi
    600000ce:	e8 16 11 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    600000d3:	90                   	nop
    600000d4:	c9                   	leaveq 
    600000d5:	c3                   	retq   

00000000600000d6 <malloc(unsigned long)>:
    600000d6:	55                   	push   %rbp
    600000d7:	48 89 e5             	mov    %rsp,%rbp
    600000da:	48 83 ec 20          	sub    $0x20,%rsp
    600000de:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    600000e2:	48 8d 55 f8          	lea    -0x8(%rbp),%rdx
    600000e6:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    600000ea:	48 89 d1             	mov    %rdx,%rcx
    600000ed:	48 89 c2             	mov    %rax,%rdx
    600000f0:	be 05 00 00 00       	mov    $0x5,%esi
    600000f5:	bf 01 00 00 00       	mov    $0x1,%edi
    600000fa:	e8 ea 10 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    600000ff:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000103:	c9                   	leaveq 
    60000104:	c3                   	retq   

0000000060000105 <free(void*)>:
    60000105:	55                   	push   %rbp
    60000106:	48 89 e5             	mov    %rsp,%rbp
    60000109:	48 83 ec 10          	sub    $0x10,%rsp
    6000010d:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    60000111:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000115:	b9 00 00 00 00       	mov    $0x0,%ecx
    6000011a:	48 89 c2             	mov    %rax,%rdx
    6000011d:	be 06 00 00 00       	mov    $0x6,%esi
    60000122:	bf 01 00 00 00       	mov    $0x1,%edi
    60000127:	e8 bd 10 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    6000012c:	90                   	nop
    6000012d:	c9                   	leaveq 
    6000012e:	c3                   	retq   

000000006000012f <createSharedPage()>:
    6000012f:	55                   	push   %rbp
    60000130:	48 89 e5             	mov    %rsp,%rbp
    60000133:	48 83 ec 10          	sub    $0x10,%rsp
    60000137:	48 8d 45 f8          	lea    -0x8(%rbp),%rax
    6000013b:	b9 00 00 00 00       	mov    $0x0,%ecx
    60000140:	48 89 c2             	mov    %rax,%rdx
    60000143:	be 07 00 00 00       	mov    $0x7,%esi
    60000148:	bf 01 00 00 00       	mov    $0x1,%edi
    6000014d:	e8 97 10 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    60000152:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000156:	c9                   	leaveq 
    60000157:	c3                   	retq   

0000000060000158 <inviteProcessToSharedPage(unsigned long, unsigned long)>:
    60000158:	55                   	push   %rbp
    60000159:	48 89 e5             	mov    %rsp,%rbp
    6000015c:	48 83 ec 10          	sub    $0x10,%rsp
    60000160:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    60000164:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
    60000168:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
    6000016c:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000170:	48 89 d1             	mov    %rdx,%rcx
    60000173:	48 89 c2             	mov    %rax,%rdx
    60000176:	be 08 00 00 00       	mov    $0x8,%esi
    6000017b:	bf 01 00 00 00       	mov    $0x1,%edi
    60000180:	e8 64 10 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    60000185:	90                   	nop
    60000186:	c9                   	leaveq 
    60000187:	c3                   	retq   

0000000060000188 <getPointerToSharedPage(unsigned long)>:
    60000188:	55                   	push   %rbp
    60000189:	48 89 e5             	mov    %rsp,%rbp
    6000018c:	48 83 ec 20          	sub    $0x20,%rsp
    60000190:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    60000194:	48 8d 55 f8          	lea    -0x8(%rbp),%rdx
    60000198:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    6000019c:	48 89 d1             	mov    %rdx,%rcx
    6000019f:	48 89 c2             	mov    %rax,%rdx
    600001a2:	be 09 00 00 00       	mov    $0x9,%esi
    600001a7:	bf 01 00 00 00       	mov    $0x1,%edi
    600001ac:	e8 38 10 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    600001b1:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600001b5:	c9                   	leaveq 
    600001b6:	c3                   	retq   

00000000600001b7 <freeSharedMemoryPage(unsigned long)>:
    600001b7:	55                   	push   %rbp
    600001b8:	48 89 e5             	mov    %rsp,%rbp
    600001bb:	48 83 ec 10          	sub    $0x10,%rsp
    600001bf:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    600001c3:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600001c7:	b9 00 00 00 00       	mov    $0x0,%ecx
    600001cc:	48 89 c2             	mov    %rax,%rdx
    600001cf:	be 0a 00 00 00       	mov    $0xa,%esi
    600001d4:	bf 01 00 00 00       	mov    $0x1,%edi
    600001d9:	e8 0b 10 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    600001de:	90                   	nop
    600001df:	c9                   	leaveq 
    600001e0:	c3                   	retq   

00000000600001e1 <waitForProcess(unsigned long)>:
    600001e1:	55                   	push   %rbp
    600001e2:	48 89 e5             	mov    %rsp,%rbp
    600001e5:	48 83 ec 20          	sub    $0x20,%rsp
    600001e9:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    600001ed:	48 8d 55 f8          	lea    -0x8(%rbp),%rdx
    600001f1:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    600001f5:	48 89 d1             	mov    %rdx,%rcx
    600001f8:	48 89 c2             	mov    %rax,%rdx
    600001fb:	be 0b 00 00 00       	mov    $0xb,%esi
    60000200:	bf 01 00 00 00       	mov    $0x1,%edi
    60000205:	e8 df 0f 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    6000020a:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    6000020e:	c9                   	leaveq 
    6000020f:	c3                   	retq   

0000000060000210 <getArguments()>:
    60000210:	55                   	push   %rbp
    60000211:	48 89 e5             	mov    %rsp,%rbp
    60000214:	48 83 ec 10          	sub    $0x10,%rsp
    60000218:	48 8d 45 f8          	lea    -0x8(%rbp),%rax
    6000021c:	b9 00 00 00 00       	mov    $0x0,%ecx
    60000221:	48 89 c2             	mov    %rax,%rdx
    60000224:	be 0c 00 00 00       	mov    $0xc,%esi
    60000229:	bf 01 00 00 00       	mov    $0x1,%edi
    6000022e:	e8 b6 0f 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    60000233:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000237:	c9                   	leaveq 
    60000238:	c3                   	retq   

0000000060000239 <getParentPid()>:
    60000239:	55                   	push   %rbp
    6000023a:	48 89 e5             	mov    %rsp,%rbp
    6000023d:	48 83 ec 10          	sub    $0x10,%rsp
    60000241:	48 8d 45 f8          	lea    -0x8(%rbp),%rax
    60000245:	b9 00 00 00 00       	mov    $0x0,%ecx
    6000024a:	48 89 c2             	mov    %rax,%rdx
    6000024d:	be 0d 00 00 00       	mov    $0xd,%esi
    60000252:	bf 01 00 00 00       	mov    $0x1,%edi
    60000257:	e8 8d 0f 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    6000025c:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000260:	c9                   	leaveq 
    60000261:	c3                   	retq   

0000000060000262 <getInterProcessMethodInfo(unsigned long, unsigned char)>:
    60000262:	55                   	push   %rbp
    60000263:	48 89 e5             	mov    %rsp,%rbp
    60000266:	48 83 ec 30          	sub    $0x30,%rsp
    6000026a:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
    6000026e:	89 f0                	mov    %esi,%eax
    60000270:	88 45 d4             	mov    %al,-0x2c(%rbp)
    60000273:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
    60000277:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    6000027b:	0f b6 45 d4          	movzbl -0x2c(%rbp),%eax
    6000027f:	88 45 f8             	mov    %al,-0x8(%rbp)
    60000282:	48 8d 45 f0          	lea    -0x10(%rbp),%rax
    60000286:	b9 00 00 00 00       	mov    $0x0,%ecx
    6000028b:	48 89 c2             	mov    %rax,%rdx
    6000028e:	be 10 00 00 00       	mov    $0x10,%esi
    60000293:	bf 01 00 00 00       	mov    $0x1,%edi
    60000298:	e8 4c 0f 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    6000029d:	0f b6 45 f9          	movzbl -0x7(%rbp),%eax
    600002a1:	88 45 ee             	mov    %al,-0x12(%rbp)
    600002a4:	0f b6 45 fa          	movzbl -0x6(%rbp),%eax
    600002a8:	88 45 ef             	mov    %al,-0x11(%rbp)
    600002ab:	0f b7 45 ee          	movzwl -0x12(%rbp),%eax
    600002af:	c9                   	leaveq 
    600002b0:	c3                   	retq   

00000000600002b1 <callInterProcessMethod(unsigned long, unsigned char, unsigned long*)>:
    600002b1:	55                   	push   %rbp
    600002b2:	48 89 e5             	mov    %rsp,%rbp
    600002b5:	48 83 ec 40          	sub    $0x40,%rsp
    600002b9:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
    600002bd:	89 f0                	mov    %esi,%eax
    600002bf:	48 89 55 c8          	mov    %rdx,-0x38(%rbp)
    600002c3:	88 45 d4             	mov    %al,-0x2c(%rbp)
    600002c6:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
    600002ca:	48 89 45 e0          	mov    %rax,-0x20(%rbp)
    600002ce:	0f b6 45 d4          	movzbl -0x2c(%rbp),%eax
    600002d2:	88 45 f0             	mov    %al,-0x10(%rbp)
    600002d5:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
    600002d9:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
    600002dd:	48 8d 45 e0          	lea    -0x20(%rbp),%rax
    600002e1:	b9 00 00 00 00       	mov    $0x0,%ecx
    600002e6:	48 89 c2             	mov    %rax,%rdx
    600002e9:	be 11 00 00 00       	mov    $0x11,%esi
    600002ee:	bf 01 00 00 00       	mov    $0x1,%edi
    600002f3:	e8 f1 0e 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    600002f8:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600002fc:	c9                   	leaveq 
    600002fd:	c3                   	retq   

00000000600002fe <registerInterProcessMethod(unsigned char, unsigned char, unsigned long, bool)>:
    600002fe:	55                   	push   %rbp
    600002ff:	48 89 e5             	mov    %rsp,%rbp
    60000302:	48 83 ec 40          	sub    $0x40,%rsp
    60000306:	89 f8                	mov    %edi,%eax
    60000308:	48 89 55 d0          	mov    %rdx,-0x30(%rbp)
    6000030c:	89 ca                	mov    %ecx,%edx
    6000030e:	88 45 dc             	mov    %al,-0x24(%rbp)
    60000311:	89 f0                	mov    %esi,%eax
    60000313:	88 45 d8             	mov    %al,-0x28(%rbp)
    60000316:	89 d0                	mov    %edx,%eax
    60000318:	88 45 cc             	mov    %al,-0x34(%rbp)
    6000031b:	0f b6 45 dc          	movzbl -0x24(%rbp),%eax
    6000031f:	88 45 e8             	mov    %al,-0x18(%rbp)
    60000322:	0f b6 45 d8          	movzbl -0x28(%rbp),%eax
    60000326:	88 45 e9             	mov    %al,-0x17(%rbp)
    60000329:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
    6000032d:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    60000331:	0f b6 45 cc          	movzbl -0x34(%rbp),%eax
    60000335:	88 45 f8             	mov    %al,-0x8(%rbp)
    60000338:	48 8d 45 e8          	lea    -0x18(%rbp),%rax
    6000033c:	b9 00 00 00 00       	mov    $0x0,%ecx
    60000341:	48 89 c2             	mov    %rax,%rdx
    60000344:	be 12 00 00 00       	mov    $0x12,%esi
    60000349:	bf 01 00 00 00       	mov    $0x1,%edi
    6000034e:	e8 96 0e 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    60000353:	90                   	nop
    60000354:	c9                   	leaveq 
    60000355:	c3                   	retq   

0000000060000356 <removeInterProcessMethod(unsigned char)>:
    60000356:	55                   	push   %rbp
    60000357:	48 89 e5             	mov    %rsp,%rbp
    6000035a:	48 83 ec 10          	sub    $0x10,%rsp
    6000035e:	89 f8                	mov    %edi,%eax
    60000360:	88 45 fc             	mov    %al,-0x4(%rbp)
    60000363:	0f b6 45 fc          	movzbl -0x4(%rbp),%eax
    60000367:	b9 00 00 00 00       	mov    $0x0,%ecx
    6000036c:	48 89 c2             	mov    %rax,%rdx
    6000036f:	be 13 00 00 00       	mov    $0x13,%esi
    60000374:	bf 01 00 00 00       	mov    $0x1,%edi
    60000379:	e8 6b 0e 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    6000037e:	90                   	nop
    6000037f:	c9                   	leaveq 
    60000380:	c3                   	retq   

0000000060000381 <getDeviceCount()>:
    60000381:	55                   	push   %rbp
    60000382:	48 89 e5             	mov    %rsp,%rbp
    60000385:	48 83 ec 10          	sub    $0x10,%rsp
    60000389:	48 8d 45 f8          	lea    -0x8(%rbp),%rax
    6000038d:	b9 00 00 00 00       	mov    $0x0,%ecx
    60000392:	48 89 c2             	mov    %rax,%rdx
    60000395:	be 01 00 00 00       	mov    $0x1,%esi
    6000039a:	bf 02 00 00 00       	mov    $0x2,%edi
    6000039f:	e8 45 0e 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    600003a4:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600003a8:	c9                   	leaveq 
    600003a9:	c3                   	retq   

00000000600003aa <getSystemDevice()>:
    600003aa:	55                   	push   %rbp
    600003ab:	48 89 e5             	mov    %rsp,%rbp
    600003ae:	48 83 ec 10          	sub    $0x10,%rsp
    600003b2:	48 8d 45 f8          	lea    -0x8(%rbp),%rax
    600003b6:	b9 00 00 00 00       	mov    $0x0,%ecx
    600003bb:	48 89 c2             	mov    %rax,%rdx
    600003be:	be 02 00 00 00       	mov    $0x2,%esi
    600003c3:	bf 02 00 00 00       	mov    $0x2,%edi
    600003c8:	e8 1c 0e 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    600003cd:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600003d1:	c9                   	leaveq 
    600003d2:	c3                   	retq   

00000000600003d3 <write(unsigned char)>:
    600003d3:	55                   	push   %rbp
    600003d4:	48 89 e5             	mov    %rsp,%rbp
    600003d7:	48 83 ec 10          	sub    $0x10,%rsp
    600003db:	89 f8                	mov    %edi,%eax
    600003dd:	88 45 fc             	mov    %al,-0x4(%rbp)
    600003e0:	0f b6 45 fc          	movzbl -0x4(%rbp),%eax
    600003e4:	b9 00 00 00 00       	mov    $0x0,%ecx
    600003e9:	48 89 c2             	mov    %rax,%rdx
    600003ec:	be 01 00 00 00       	mov    $0x1,%esi
    600003f1:	bf 03 00 00 00       	mov    $0x3,%edi
    600003f6:	e8 ee 0d 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    600003fb:	90                   	nop
    600003fc:	c9                   	leaveq 
    600003fd:	c3                   	retq   

00000000600003fe <read()>:
    600003fe:	55                   	push   %rbp
    600003ff:	48 89 e5             	mov    %rsp,%rbp
    60000402:	48 83 ec 10          	sub    $0x10,%rsp
    60000406:	48 8d 45 ff          	lea    -0x1(%rbp),%rax
    6000040a:	b9 00 00 00 00       	mov    $0x0,%ecx
    6000040f:	48 89 c2             	mov    %rax,%rdx
    60000412:	be 02 00 00 00       	mov    $0x2,%esi
    60000417:	bf 03 00 00 00       	mov    $0x3,%edi
    6000041c:	e8 c8 0d 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    60000421:	0f b6 45 ff          	movzbl -0x1(%rbp),%eax
    60000425:	c9                   	leaveq 
    60000426:	c3                   	retq   

0000000060000427 <getINodeOfPath(unsigned long, char const*)>:
    60000427:	55                   	push   %rbp
    60000428:	48 89 e5             	mov    %rsp,%rbp
    6000042b:	48 83 ec 20          	sub    $0x20,%rsp
    6000042f:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    60000433:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
    60000437:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
    6000043b:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    6000043f:	48 8d 55 f0          	lea    -0x10(%rbp),%rdx
    60000443:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    60000447:	48 89 d1             	mov    %rdx,%rcx
    6000044a:	48 89 c2             	mov    %rax,%rdx
    6000044d:	be 01 00 00 00       	mov    $0x1,%esi
    60000452:	bf 04 00 00 00       	mov    $0x4,%edi
    60000457:	e8 8d 0d 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    6000045c:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000460:	c9                   	leaveq 
    60000461:	c3                   	retq   

0000000060000462 <getSectorOfINode(unsigned long, unsigned long, unsigned long, unsigned char*)>:
    60000462:	55                   	push   %rbp
    60000463:	48 89 e5             	mov    %rsp,%rbp
    60000466:	48 83 ec 40          	sub    $0x40,%rsp
    6000046a:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
    6000046e:	48 89 75 d0          	mov    %rsi,-0x30(%rbp)
    60000472:	48 89 55 c8          	mov    %rdx,-0x38(%rbp)
    60000476:	48 89 4d c0          	mov    %rcx,-0x40(%rbp)
    6000047a:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
    6000047e:	48 89 45 e0          	mov    %rax,-0x20(%rbp)
    60000482:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
    60000486:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
    6000048a:	48 8b 45 c0          	mov    -0x40(%rbp),%rax
    6000048e:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    60000492:	48 8d 55 e0          	lea    -0x20(%rbp),%rdx
    60000496:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
    6000049a:	48 89 d1             	mov    %rdx,%rcx
    6000049d:	48 89 c2             	mov    %rax,%rdx
    600004a0:	be 02 00 00 00       	mov    $0x2,%esi
    600004a5:	bf 04 00 00 00       	mov    $0x4,%edi
    600004aa:	e8 3a 0d 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    600004af:	0f b6 45 f8          	movzbl -0x8(%rbp),%eax
    600004b3:	c9                   	leaveq 
    600004b4:	c3                   	retq   

00000000600004b5 <getFlagsOfINode(unsigned long, unsigned long)>:
    600004b5:	55                   	push   %rbp
    600004b6:	48 89 e5             	mov    %rsp,%rbp
    600004b9:	48 83 ec 20          	sub    $0x20,%rsp
    600004bd:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    600004c1:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
    600004c5:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
    600004c9:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    600004cd:	48 8d 55 f0          	lea    -0x10(%rbp),%rdx
    600004d1:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    600004d5:	48 89 d1             	mov    %rdx,%rcx
    600004d8:	48 89 c2             	mov    %rax,%rdx
    600004db:	be 03 00 00 00       	mov    $0x3,%esi
    600004e0:	bf 04 00 00 00       	mov    $0x4,%edi
    600004e5:	e8 ff 0c 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    600004ea:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600004ee:	c9                   	leaveq 
    600004ef:	c3                   	retq   

00000000600004f0 <getDirectoryEntriesOfINode(unsigned long, unsigned long, unsigned long, DirectoryEntry*)>:
    600004f0:	55                   	push   %rbp
    600004f1:	48 89 e5             	mov    %rsp,%rbp
    600004f4:	48 83 ec 40          	sub    $0x40,%rsp
    600004f8:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
    600004fc:	48 89 75 d0          	mov    %rsi,-0x30(%rbp)
    60000500:	48 89 55 c8          	mov    %rdx,-0x38(%rbp)
    60000504:	48 89 4d c0          	mov    %rcx,-0x40(%rbp)
    60000508:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
    6000050c:	48 89 45 e0          	mov    %rax,-0x20(%rbp)
    60000510:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
    60000514:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
    60000518:	48 8b 45 c0          	mov    -0x40(%rbp),%rax
    6000051c:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    60000520:	48 8d 55 e0          	lea    -0x20(%rbp),%rdx
    60000524:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
    60000528:	48 89 d1             	mov    %rdx,%rcx
    6000052b:	48 89 c2             	mov    %rax,%rdx
    6000052e:	be 04 00 00 00       	mov    $0x4,%esi
    60000533:	bf 04 00 00 00       	mov    $0x4,%edi
    60000538:	e8 ac 0c 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    6000053d:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000541:	c9                   	leaveq 
    60000542:	c3                   	retq   

0000000060000543 <getFileSize(unsigned long, unsigned long)>:
    60000543:	55                   	push   %rbp
    60000544:	48 89 e5             	mov    %rsp,%rbp
    60000547:	48 83 ec 20          	sub    $0x20,%rsp
    6000054b:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    6000054f:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
    60000553:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
    60000557:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    6000055b:	48 c7 45 f8 00 00 00 	movq   $0x0,-0x8(%rbp)
    60000562:	00 
    60000563:	48 8d 55 f0          	lea    -0x10(%rbp),%rdx
    60000567:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    6000056b:	48 89 d1             	mov    %rdx,%rcx
    6000056e:	48 89 c2             	mov    %rax,%rdx
    60000571:	be 05 00 00 00       	mov    $0x5,%esi
    60000576:	bf 04 00 00 00       	mov    $0x4,%edi
    6000057b:	e8 69 0c 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    60000580:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000584:	c9                   	leaveq 
    60000585:	c3                   	retq   

0000000060000586 <getPciDeviceCount()>:
    60000586:	55                   	push   %rbp
    60000587:	48 89 e5             	mov    %rsp,%rbp
    6000058a:	48 83 ec 10          	sub    $0x10,%rsp
    6000058e:	48 c7 45 f8 00 00 00 	movq   $0x0,-0x8(%rbp)
    60000595:	00 
    60000596:	48 8d 45 f8          	lea    -0x8(%rbp),%rax
    6000059a:	48 89 c1             	mov    %rax,%rcx
    6000059d:	ba 00 00 00 00       	mov    $0x0,%edx
    600005a2:	be 02 00 00 00       	mov    $0x2,%esi
    600005a7:	bf 05 00 00 00       	mov    $0x5,%edi
    600005ac:	e8 38 0c 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    600005b1:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600005b5:	c9                   	leaveq 
    600005b6:	c3                   	retq   

00000000600005b7 <getPciDeviceList(unsigned long*, unsigned long)>:
    600005b7:	55                   	push   %rbp
    600005b8:	48 89 e5             	mov    %rsp,%rbp
    600005bb:	48 83 ec 10          	sub    $0x10,%rsp
    600005bf:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    600005c3:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
    600005c7:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    600005cb:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    600005cf:	48 89 d1             	mov    %rdx,%rcx
    600005d2:	48 89 c2             	mov    %rax,%rdx
    600005d5:	be 02 00 00 00       	mov    $0x2,%esi
    600005da:	bf 05 00 00 00       	mov    $0x5,%edi
    600005df:	e8 05 0c 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    600005e4:	90                   	nop
    600005e5:	c9                   	leaveq 
    600005e6:	c3                   	retq   

00000000600005e7 <getPciDeviceHeader(unsigned long, unsigned char*)>:
    600005e7:	55                   	push   %rbp
    600005e8:	48 89 e5             	mov    %rsp,%rbp
    600005eb:	48 83 ec 10          	sub    $0x10,%rsp
    600005ef:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    600005f3:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
    600005f7:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
    600005fb:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600005ff:	48 89 d1             	mov    %rdx,%rcx
    60000602:	48 89 c2             	mov    %rax,%rdx
    60000605:	be 03 00 00 00       	mov    $0x3,%esi
    6000060a:	bf 05 00 00 00       	mov    $0x5,%edi
    6000060f:	e8 d5 0b 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    60000614:	90                   	nop
    60000615:	c9                   	leaveq 
    60000616:	c3                   	retq   

0000000060000617 <allocateMemoryPage(unsigned long)>:
    60000617:	55                   	push   %rbp
    60000618:	48 89 e5             	mov    %rsp,%rbp
    6000061b:	48 83 ec 10          	sub    $0x10,%rsp
    6000061f:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    60000623:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000627:	b9 00 00 00 00       	mov    $0x0,%ecx
    6000062c:	48 89 c2             	mov    %rax,%rdx
    6000062f:	be 04 00 00 00       	mov    $0x4,%esi
    60000634:	bf 05 00 00 00       	mov    $0x5,%edi
    60000639:	e8 ab 0b 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    6000063e:	90                   	nop
    6000063f:	c9                   	leaveq 
    60000640:	c3                   	retq   

0000000060000641 <freeMemoryPage(unsigned long)>:
    60000641:	55                   	push   %rbp
    60000642:	48 89 e5             	mov    %rsp,%rbp
    60000645:	48 83 ec 10          	sub    $0x10,%rsp
    60000649:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    6000064d:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000651:	b9 00 00 00 00       	mov    $0x0,%ecx
    60000656:	48 89 c2             	mov    %rax,%rdx
    60000659:	be 05 00 00 00       	mov    $0x5,%esi
    6000065e:	bf 05 00 00 00       	mov    $0x5,%edi
    60000663:	e8 81 0b 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    60000668:	90                   	nop
    60000669:	c9                   	leaveq 
    6000066a:	c3                   	retq   

000000006000066b <virtualToPhysicalAddress(unsigned long)>:
    6000066b:	55                   	push   %rbp
    6000066c:	48 89 e5             	mov    %rsp,%rbp
    6000066f:	48 83 ec 20          	sub    $0x20,%rsp
    60000673:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    60000677:	48 c7 45 f8 00 00 00 	movq   $0x0,-0x8(%rbp)
    6000067e:	00 
    6000067f:	48 8d 55 f8          	lea    -0x8(%rbp),%rdx
    60000683:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    60000687:	48 89 d1             	mov    %rdx,%rcx
    6000068a:	48 89 c2             	mov    %rax,%rdx
    6000068d:	be 06 00 00 00       	mov    $0x6,%esi
    60000692:	bf 05 00 00 00       	mov    $0x5,%edi
    60000697:	e8 4d 0b 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    6000069c:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600006a0:	c9                   	leaveq 
    600006a1:	c3                   	retq   

00000000600006a2 <mapVirtualToPhysicalAddress(unsigned long, unsigned long)>:
    600006a2:	55                   	push   %rbp
    600006a3:	48 89 e5             	mov    %rsp,%rbp
    600006a6:	48 83 ec 10          	sub    $0x10,%rsp
    600006aa:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    600006ae:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
    600006b2:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
    600006b6:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600006ba:	48 89 d1             	mov    %rdx,%rcx
    600006bd:	48 89 c2             	mov    %rax,%rdx
    600006c0:	be 07 00 00 00       	mov    $0x7,%esi
    600006c5:	bf 05 00 00 00       	mov    $0x5,%edi
    600006ca:	e8 1a 0b 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    600006cf:	90                   	nop
    600006d0:	c9                   	leaveq 
    600006d1:	c3                   	retq   

00000000600006d2 <getDriverStatus(unsigned long)>:
    600006d2:	55                   	push   %rbp
    600006d3:	48 89 e5             	mov    %rsp,%rbp
    600006d6:	48 83 ec 20          	sub    $0x20,%rsp
    600006da:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    600006de:	48 c7 45 f8 00 00 00 	movq   $0x0,-0x8(%rbp)
    600006e5:	00 
    600006e6:	48 8d 55 f8          	lea    -0x8(%rbp),%rdx
    600006ea:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    600006ee:	48 89 d1             	mov    %rdx,%rcx
    600006f1:	48 89 c2             	mov    %rax,%rdx
    600006f4:	be 10 00 00 00       	mov    $0x10,%esi
    600006f9:	bf 05 00 00 00       	mov    $0x5,%edi
    600006fe:	e8 e6 0a 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    60000703:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000707:	c9                   	leaveq 
    60000708:	c3                   	retq   

0000000060000709 <driverCall(unsigned long, unsigned char, void*)>:
    60000709:	55                   	push   %rbp
    6000070a:	48 89 e5             	mov    %rsp,%rbp
    6000070d:	48 83 ec 20          	sub    $0x20,%rsp
    60000711:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    60000715:	89 f0                	mov    %esi,%eax
    60000717:	48 89 55 e8          	mov    %rdx,-0x18(%rbp)
    6000071b:	88 45 f4             	mov    %al,-0xc(%rbp)
    6000071e:	80 7d f4 00          	cmpb   $0x0,-0xc(%rbp)
    60000722:	74 26                	je     6000074a <driverCall(unsigned long, unsigned char, void*)+0x41>
    60000724:	80 7d f4 0f          	cmpb   $0xf,-0xc(%rbp)
    60000728:	77 20                	ja     6000074a <driverCall(unsigned long, unsigned char, void*)+0x41>
    6000072a:	48 8b 4d e8          	mov    -0x18(%rbp),%rcx
    6000072e:	0f b6 45 f4          	movzbl -0xc(%rbp),%eax
    60000732:	83 c0 10             	add    $0x10,%eax
    60000735:	48 98                	cltq   
    60000737:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    6000073b:	48 89 c6             	mov    %rax,%rsi
    6000073e:	bf 05 00 00 00       	mov    $0x5,%edi
    60000743:	e8 a1 0a 00 00       	callq  600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
    60000748:	eb 01                	jmp    6000074b <driverCall(unsigned long, unsigned char, void*)+0x42>
    6000074a:	90                   	nop
    6000074b:	c9                   	leaveq 
    6000074c:	c3                   	retq   

000000006000074d <countArgc(char const*)>:
    6000074d:	55                   	push   %rbp
    6000074e:	48 89 e5             	mov    %rsp,%rbp
    60000751:	48 83 ec 18          	sub    $0x18,%rsp
    60000755:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    60000759:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)
    60000760:	48 83 7d e8 00       	cmpq   $0x0,-0x18(%rbp)
    60000765:	74 0b                	je     60000772 <countArgc(char const*)+0x25>
    60000767:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    6000076b:	0f b6 00             	movzbl (%rax),%eax
    6000076e:	84 c0                	test   %al,%al
    60000770:	75 09                	jne    6000077b <countArgc(char const*)+0x2e>
    60000772:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)
    60000779:	eb 31                	jmp    600007ac <countArgc(char const*)+0x5f>
    6000077b:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    6000077f:	0f b6 00             	movzbl (%rax),%eax
    60000782:	84 c0                	test   %al,%al
    60000784:	75 04                	jne    6000078a <countArgc(char const*)+0x3d>
    60000786:	83 45 fc 01          	addl   $0x1,-0x4(%rbp)
    6000078a:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    6000078e:	0f b6 00             	movzbl (%rax),%eax
    60000791:	84 c0                	test   %al,%al
    60000793:	75 0f                	jne    600007a4 <countArgc(char const*)+0x57>
    60000795:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    60000799:	48 83 c0 01          	add    $0x1,%rax
    6000079d:	0f b6 00             	movzbl (%rax),%eax
    600007a0:	84 c0                	test   %al,%al
    600007a2:	74 07                	je     600007ab <countArgc(char const*)+0x5e>
    600007a4:	48 83 45 e8 01       	addq   $0x1,-0x18(%rbp)
    600007a9:	eb d0                	jmp    6000077b <countArgc(char const*)+0x2e>
    600007ab:	90                   	nop
    600007ac:	8b 45 fc             	mov    -0x4(%rbp),%eax
    600007af:	c9                   	leaveq 
    600007b0:	c3                   	retq   

00000000600007b1 <buildArgv(int, char const*)>:
    600007b1:	55                   	push   %rbp
    600007b2:	48 89 e5             	mov    %rsp,%rbp
    600007b5:	48 83 ec 20          	sub    $0x20,%rsp
    600007b9:	89 7d ec             	mov    %edi,-0x14(%rbp)
    600007bc:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
    600007c0:	8b 45 ec             	mov    -0x14(%rbp),%eax
    600007c3:	83 c0 01             	add    $0x1,%eax
    600007c6:	48 98                	cltq   
    600007c8:	48 c1 e0 03          	shl    $0x3,%rax
    600007cc:	48 89 c7             	mov    %rax,%rdi
    600007cf:	e8 02 f9 ff ff       	callq  600000d6 <malloc(unsigned long)>
    600007d4:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    600007d8:	8b 45 ec             	mov    -0x14(%rbp),%eax
    600007db:	48 98                	cltq   
    600007dd:	48 8d 14 c5 00 00 00 	lea    0x0(,%rax,8),%rdx
    600007e4:	00 
    600007e5:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    600007e9:	48 01 d0             	add    %rdx,%rax
    600007ec:	48 c7 00 00 00 00 00 	movq   $0x0,(%rax)
    600007f3:	83 7d ec 00          	cmpl   $0x0,-0x14(%rbp)
    600007f7:	7e 4b                	jle    60000844 <buildArgv(int, char const*)+0x93>
    600007f9:	48 c7 45 f8 00 00 00 	movq   $0x0,-0x8(%rbp)
    60000800:	00 
    60000801:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000805:	48 8d 14 c5 00 00 00 	lea    0x0(,%rax,8),%rdx
    6000080c:	00 
    6000080d:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000811:	48 01 c2             	add    %rax,%rdx
    60000814:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
    60000818:	48 89 02             	mov    %rax,(%rdx)
    6000081b:	48 83 45 f8 01       	addq   $0x1,-0x8(%rbp)
    60000820:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
    60000824:	0f b6 00             	movzbl (%rax),%eax
    60000827:	84 c0                	test   %al,%al
    60000829:	74 07                	je     60000832 <buildArgv(int, char const*)+0x81>
    6000082b:	48 83 45 e0 01       	addq   $0x1,-0x20(%rbp)
    60000830:	eb ee                	jmp    60000820 <buildArgv(int, char const*)+0x6f>
    60000832:	48 83 45 e0 01       	addq   $0x1,-0x20(%rbp)
    60000837:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
    6000083b:	0f b6 00             	movzbl (%rax),%eax
    6000083e:	84 c0                	test   %al,%al
    60000840:	74 02                	je     60000844 <buildArgv(int, char const*)+0x93>
    60000842:	eb bd                	jmp    60000801 <buildArgv(int, char const*)+0x50>
    60000844:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000848:	c9                   	leaveq 
    60000849:	c3                   	retq   

000000006000084a <__start1>:
    6000084a:	55                   	push   %rbp
    6000084b:	48 89 e5             	mov    %rsp,%rbp
    6000084e:	48 83 ec 20          	sub    $0x20,%rsp
    60000852:	e8 d3 f7 ff ff       	callq  6000002a <getpid()>
    60000857:	48 89 05 a2 17 00 00 	mov    %rax,0x17a2(%rip)        # 60002000 <myPid>
    6000085e:	e8 ad f9 ff ff       	callq  60000210 <getArguments()>
    60000863:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    60000867:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    6000086b:	48 89 c7             	mov    %rax,%rdi
    6000086e:	e8 da fe ff ff       	callq  6000074d <countArgc(char const*)>
    60000873:	89 45 f4             	mov    %eax,-0xc(%rbp)
    60000876:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    6000087a:	8b 45 f4             	mov    -0xc(%rbp),%eax
    6000087d:	48 89 d6             	mov    %rdx,%rsi
    60000880:	89 c7                	mov    %eax,%edi
    60000882:	e8 2a ff ff ff       	callq  600007b1 <buildArgv(int, char const*)>
    60000887:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
    6000088b:	48 8b 55 e8          	mov    -0x18(%rbp),%rdx
    6000088f:	8b 45 f4             	mov    -0xc(%rbp),%eax
    60000892:	48 89 d6             	mov    %rdx,%rsi
    60000895:	89 c7                	mov    %eax,%edi
    60000897:	e8 ff 04 00 00       	callq  60000d9b <main>
    6000089c:	89 45 e4             	mov    %eax,-0x1c(%rbp)
    6000089f:	8b 45 e4             	mov    -0x1c(%rbp),%eax
    600008a2:	48 98                	cltq   
    600008a4:	48 89 c7             	mov    %rax,%rdi
    600008a7:	e8 54 f7 ff ff       	callq  60000000 <exit(unsigned long)>
    600008ac:	90                   	nop
    600008ad:	c9                   	leaveq 
    600008ae:	c3                   	retq   

00000000600008af <__start>:
    600008af:	48 89 e5             	mov    %rsp,%rbp
    600008b2:	e8 93 ff ff ff       	callq  6000084a <__start1>
    600008b7:	cc                   	int3   

00000000600008b8 <print(char const*)>:
    600008b8:	55                   	push   %rbp
    600008b9:	48 89 e5             	mov    %rsp,%rbp
    600008bc:	48 83 ec 20          	sub    $0x20,%rsp
    600008c0:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    600008c4:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    600008c8:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
    600008cc:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600008d0:	0f b6 00             	movzbl (%rax),%eax
    600008d3:	84 c0                	test   %al,%al
    600008d5:	74 18                	je     600008ef <print(char const*)+0x37>
    600008d7:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600008db:	0f b6 00             	movzbl (%rax),%eax
    600008de:	0f b6 c0             	movzbl %al,%eax
    600008e1:	89 c7                	mov    %eax,%edi
    600008e3:	e8 eb fa ff ff       	callq  600003d3 <write(unsigned char)>
    600008e8:	48 83 45 f8 01       	addq   $0x1,-0x8(%rbp)
    600008ed:	eb dd                	jmp    600008cc <print(char const*)+0x14>
    600008ef:	90                   	nop
    600008f0:	c9                   	leaveq 
    600008f1:	c3                   	retq   

00000000600008f2 <strequ(char const*, char const*, unsigned long)>:
    600008f2:	55                   	push   %rbp
    600008f3:	48 89 e5             	mov    %rsp,%rbp
    600008f6:	48 83 ec 28          	sub    $0x28,%rsp
    600008fa:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    600008fe:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
    60000902:	48 89 55 d8          	mov    %rdx,-0x28(%rbp)
    60000906:	48 c7 45 f8 00 00 00 	movq   $0x0,-0x8(%rbp)
    6000090d:	00 
    6000090e:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000912:	48 3b 45 d8          	cmp    -0x28(%rbp),%rax
    60000916:	73 2e                	jae    60000946 <strequ(char const*, char const*, unsigned long)+0x54>
    60000918:	48 8b 55 e8          	mov    -0x18(%rbp),%rdx
    6000091c:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000920:	48 01 d0             	add    %rdx,%rax
    60000923:	0f b6 10             	movzbl (%rax),%edx
    60000926:	48 8b 4d e0          	mov    -0x20(%rbp),%rcx
    6000092a:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    6000092e:	48 01 c8             	add    %rcx,%rax
    60000931:	0f b6 00             	movzbl (%rax),%eax
    60000934:	38 c2                	cmp    %al,%dl
    60000936:	74 07                	je     6000093f <strequ(char const*, char const*, unsigned long)+0x4d>
    60000938:	b8 00 00 00 00       	mov    $0x0,%eax
    6000093d:	eb 0c                	jmp    6000094b <strequ(char const*, char const*, unsigned long)+0x59>
    6000093f:	48 83 45 f8 01       	addq   $0x1,-0x8(%rbp)
    60000944:	eb c8                	jmp    6000090e <strequ(char const*, char const*, unsigned long)+0x1c>
    60000946:	b8 01 00 00 00       	mov    $0x1,%eax
    6000094b:	c9                   	leaveq 
    6000094c:	c3                   	retq   

000000006000094d <memcpy(void*, void const*, unsigned long)>:
    6000094d:	55                   	push   %rbp
    6000094e:	48 89 e5             	mov    %rsp,%rbp
    60000951:	48 83 ec 28          	sub    $0x28,%rsp
    60000955:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    60000959:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
    6000095d:	48 89 55 d8          	mov    %rdx,-0x28(%rbp)
    60000961:	48 c7 45 f8 00 00 00 	movq   $0x0,-0x8(%rbp)
    60000968:	00 
    60000969:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    6000096d:	48 3b 45 d8          	cmp    -0x28(%rbp),%rax
    60000971:	73 22                	jae    60000995 <memcpy(void*, void const*, unsigned long)+0x48>
    60000973:	48 8b 55 e0          	mov    -0x20(%rbp),%rdx
    60000977:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    6000097b:	48 01 d0             	add    %rdx,%rax
    6000097e:	48 8b 4d e8          	mov    -0x18(%rbp),%rcx
    60000982:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    60000986:	48 01 ca             	add    %rcx,%rdx
    60000989:	0f b6 00             	movzbl (%rax),%eax
    6000098c:	88 02                	mov    %al,(%rdx)
    6000098e:	48 83 45 f8 01       	addq   $0x1,-0x8(%rbp)
    60000993:	eb d4                	jmp    60000969 <memcpy(void*, void const*, unsigned long)+0x1c>
    60000995:	90                   	nop
    60000996:	c9                   	leaveq 
    60000997:	c3                   	retq   

0000000060000998 <memset(void*, char, unsigned long)>:
    60000998:	55                   	push   %rbp
    60000999:	48 89 e5             	mov    %rsp,%rbp
    6000099c:	48 83 ec 28          	sub    $0x28,%rsp
    600009a0:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    600009a4:	89 f0                	mov    %esi,%eax
    600009a6:	48 89 55 d8          	mov    %rdx,-0x28(%rbp)
    600009aa:	88 45 e4             	mov    %al,-0x1c(%rbp)
    600009ad:	48 c7 45 f8 00 00 00 	movq   $0x0,-0x8(%rbp)
    600009b4:	00 
    600009b5:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600009b9:	48 3b 45 d8          	cmp    -0x28(%rbp),%rax
    600009bd:	73 18                	jae    600009d7 <memset(void*, char, unsigned long)+0x3f>
    600009bf:	48 8b 55 e8          	mov    -0x18(%rbp),%rdx
    600009c3:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600009c7:	48 01 c2             	add    %rax,%rdx
    600009ca:	0f b6 45 e4          	movzbl -0x1c(%rbp),%eax
    600009ce:	88 02                	mov    %al,(%rdx)
    600009d0:	48 83 45 f8 01       	addq   $0x1,-0x8(%rbp)
    600009d5:	eb de                	jmp    600009b5 <memset(void*, char, unsigned long)+0x1d>
    600009d7:	90                   	nop
    600009d8:	c9                   	leaveq 
    600009d9:	c3                   	retq   

00000000600009da <strlen(char const*)>:
    600009da:	55                   	push   %rbp
    600009db:	48 89 e5             	mov    %rsp,%rbp
    600009de:	48 83 ec 18          	sub    $0x18,%rsp
    600009e2:	48 89 7d e8          	mov    %rdi,-0x18(%rbp)
    600009e6:	48 c7 45 f8 00 00 00 	movq   $0x0,-0x8(%rbp)
    600009ed:	00 
    600009ee:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    600009f2:	0f b6 00             	movzbl (%rax),%eax
    600009f5:	84 c0                	test   %al,%al
    600009f7:	74 0c                	je     60000a05 <strlen(char const*)+0x2b>
    600009f9:	48 83 45 f8 01       	addq   $0x1,-0x8(%rbp)
    600009fe:	48 83 45 e8 01       	addq   $0x1,-0x18(%rbp)
    60000a03:	eb e9                	jmp    600009ee <strlen(char const*)+0x14>
    60000a05:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000a09:	c9                   	leaveq 
    60000a0a:	c3                   	retq   

0000000060000a0b <printBase(unsigned long, unsigned long, int)>:
    60000a0b:	55                   	push   %rbp
    60000a0c:	48 89 e5             	mov    %rsp,%rbp
    60000a0f:	48 83 c4 80          	add    $0xffffffffffffff80,%rsp
    60000a13:	48 89 7d 98          	mov    %rdi,-0x68(%rbp)
    60000a17:	48 89 75 90          	mov    %rsi,-0x70(%rbp)
    60000a1b:	89 55 8c             	mov    %edx,-0x74(%rbp)
    60000a1e:	48 c7 45 e8 00 30 00 	movq   $0x60003000,-0x18(%rbp)
    60000a25:	60 
    60000a26:	48 8d 55 a8          	lea    -0x58(%rbp),%rdx
    60000a2a:	b8 00 00 00 00       	mov    $0x0,%eax
    60000a2f:	b9 08 00 00 00       	mov    $0x8,%ecx
    60000a34:	48 89 d7             	mov    %rdx,%rdi
    60000a37:	f3 48 ab             	rep stos %rax,%es:(%rdi)
    60000a3a:	c7 45 fc 01 00 00 00 	movl   $0x1,-0x4(%rbp)
    60000a41:	48 83 7d 98 00       	cmpq   $0x0,-0x68(%rbp)
    60000a46:	74 37                	je     60000a7f <printBase(unsigned long, unsigned long, int)+0x74>
    60000a48:	48 8b 45 98          	mov    -0x68(%rbp),%rax
    60000a4c:	ba 00 00 00 00       	mov    $0x0,%edx
    60000a51:	48 f7 75 90          	divq   -0x70(%rbp)
    60000a55:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    60000a59:	48 01 d0             	add    %rdx,%rax
    60000a5c:	0f b6 10             	movzbl (%rax),%edx
    60000a5f:	8b 45 fc             	mov    -0x4(%rbp),%eax
    60000a62:	48 98                	cltq   
    60000a64:	88 54 05 a8          	mov    %dl,-0x58(%rbp,%rax,1)
    60000a68:	48 8b 45 98          	mov    -0x68(%rbp),%rax
    60000a6c:	ba 00 00 00 00       	mov    $0x0,%edx
    60000a71:	48 f7 75 90          	divq   -0x70(%rbp)
    60000a75:	48 89 45 98          	mov    %rax,-0x68(%rbp)
    60000a79:	83 45 fc 01          	addl   $0x1,-0x4(%rbp)
    60000a7d:	eb c2                	jmp    60000a41 <printBase(unsigned long, unsigned long, int)+0x36>
    60000a7f:	c7 45 f8 00 00 00 00 	movl   $0x0,-0x8(%rbp)
    60000a86:	8b 45 8c             	mov    -0x74(%rbp),%eax
    60000a89:	2b 45 fc             	sub    -0x4(%rbp),%eax
    60000a8c:	39 45 f8             	cmp    %eax,-0x8(%rbp)
    60000a8f:	7f 10                	jg     60000aa1 <printBase(unsigned long, unsigned long, int)+0x96>
    60000a91:	bf 30 00 00 00       	mov    $0x30,%edi
    60000a96:	e8 38 f9 ff ff       	callq  600003d3 <write(unsigned char)>
    60000a9b:	83 45 f8 01          	addl   $0x1,-0x8(%rbp)
    60000a9f:	eb e5                	jmp    60000a86 <printBase(unsigned long, unsigned long, int)+0x7b>
    60000aa1:	c6 45 e7 00          	movb   $0x0,-0x19(%rbp)
    60000aa5:	8b 45 fc             	mov    -0x4(%rbp),%eax
    60000aa8:	48 98                	cltq   
    60000aaa:	48 8d 50 ff          	lea    -0x1(%rax),%rdx
    60000aae:	48 8d 45 a8          	lea    -0x58(%rbp),%rax
    60000ab2:	48 01 d0             	add    %rdx,%rax
    60000ab5:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    60000ab9:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000abd:	0f b6 00             	movzbl (%rax),%eax
    60000ac0:	84 c0                	test   %al,%al
    60000ac2:	74 18                	je     60000adc <printBase(unsigned long, unsigned long, int)+0xd1>
    60000ac4:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000ac8:	0f b6 00             	movzbl (%rax),%eax
    60000acb:	0f b6 c0             	movzbl %al,%eax
    60000ace:	89 c7                	mov    %eax,%edi
    60000ad0:	e8 fe f8 ff ff       	callq  600003d3 <write(unsigned char)>
    60000ad5:	48 83 6d f0 01       	subq   $0x1,-0x10(%rbp)
    60000ada:	eb dd                	jmp    60000ab9 <printBase(unsigned long, unsigned long, int)+0xae>
    60000adc:	90                   	nop
    60000add:	c9                   	leaveq 
    60000ade:	c3                   	retq   

0000000060000adf <simplifyPath(char*, char const*, unsigned long)>:
    60000adf:	55                   	push   %rbp
    60000ae0:	48 89 e5             	mov    %rsp,%rbp
    60000ae3:	48 83 ec 38          	sub    $0x38,%rsp
    60000ae7:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
    60000aeb:	48 89 75 d0          	mov    %rsi,-0x30(%rbp)
    60000aef:	48 89 55 c8          	mov    %rdx,-0x38(%rbp)
    60000af3:	48 c7 45 f8 00 00 00 	movq   $0x0,-0x8(%rbp)
    60000afa:	00 
    60000afb:	48 c7 45 f0 00 00 00 	movq   $0x0,-0x10(%rbp)
    60000b02:	00 
    60000b03:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
    60000b07:	48 89 c7             	mov    %rax,%rdi
    60000b0a:	e8 cb fe ff ff       	callq  600009da <strlen(char const*)>
    60000b0f:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
    60000b13:	48 8b 55 d0          	mov    -0x30(%rbp),%rdx
    60000b17:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000b1b:	48 01 d0             	add    %rdx,%rax
    60000b1e:	0f b6 00             	movzbl (%rax),%eax
    60000b21:	84 c0                	test   %al,%al
    60000b23:	0f 84 51 01 00 00    	je     60000c7a <simplifyPath(char*, char const*, unsigned long)+0x19b>
    60000b29:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000b2d:	48 83 c0 02          	add    $0x2,%rax
    60000b31:	48 39 45 e8          	cmp    %rax,-0x18(%rbp)
    60000b35:	76 49                	jbe    60000b80 <simplifyPath(char*, char const*, unsigned long)+0xa1>
    60000b37:	48 8b 55 d0          	mov    -0x30(%rbp),%rdx
    60000b3b:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000b3f:	48 01 d0             	add    %rdx,%rax
    60000b42:	ba 04 00 00 00       	mov    $0x4,%edx
    60000b47:	be 11 30 00 60       	mov    $0x60003011,%esi
    60000b4c:	48 89 c7             	mov    %rax,%rdi
    60000b4f:	e8 9e fd ff ff       	callq  600008f2 <strequ(char const*, char const*, unsigned long)>
    60000b54:	84 c0                	test   %al,%al
    60000b56:	75 21                	jne    60000b79 <simplifyPath(char*, char const*, unsigned long)+0x9a>
    60000b58:	48 8b 55 d0          	mov    -0x30(%rbp),%rdx
    60000b5c:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000b60:	48 01 d0             	add    %rdx,%rax
    60000b63:	ba 04 00 00 00       	mov    $0x4,%edx
    60000b68:	be 16 30 00 60       	mov    $0x60003016,%esi
    60000b6d:	48 89 c7             	mov    %rax,%rdi
    60000b70:	e8 7d fd ff ff       	callq  600008f2 <strequ(char const*, char const*, unsigned long)>
    60000b75:	84 c0                	test   %al,%al
    60000b77:	74 07                	je     60000b80 <simplifyPath(char*, char const*, unsigned long)+0xa1>
    60000b79:	b8 01 00 00 00       	mov    $0x1,%eax
    60000b7e:	eb 05                	jmp    60000b85 <simplifyPath(char*, char const*, unsigned long)+0xa6>
    60000b80:	b8 00 00 00 00       	mov    $0x0,%eax
    60000b85:	84 c0                	test   %al,%al
    60000b87:	74 31                	je     60000bba <simplifyPath(char*, char const*, unsigned long)+0xdb>
    60000b89:	48 83 45 f0 03       	addq   $0x3,-0x10(%rbp)
    60000b8e:	48 83 7d f8 00       	cmpq   $0x0,-0x8(%rbp)
    60000b93:	0f 84 cc 00 00 00    	je     60000c65 <simplifyPath(char*, char const*, unsigned long)+0x186>
    60000b99:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000b9d:	48 8d 50 ff          	lea    -0x1(%rax),%rdx
    60000ba1:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
    60000ba5:	48 01 d0             	add    %rdx,%rax
    60000ba8:	0f b6 00             	movzbl (%rax),%eax
    60000bab:	3c 2f                	cmp    $0x2f,%al
    60000bad:	0f 84 b1 00 00 00    	je     60000c64 <simplifyPath(char*, char const*, unsigned long)+0x185>
    60000bb3:	48 83 6d f8 01       	subq   $0x1,-0x8(%rbp)
    60000bb8:	eb d4                	jmp    60000b8e <simplifyPath(char*, char const*, unsigned long)+0xaf>
    60000bba:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000bbe:	48 83 c0 02          	add    $0x2,%rax
    60000bc2:	48 39 45 e8          	cmp    %rax,-0x18(%rbp)
    60000bc6:	76 28                	jbe    60000bf0 <simplifyPath(char*, char const*, unsigned long)+0x111>
    60000bc8:	48 8b 55 d0          	mov    -0x30(%rbp),%rdx
    60000bcc:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000bd0:	48 01 d0             	add    %rdx,%rax
    60000bd3:	ba 03 00 00 00       	mov    $0x3,%edx
    60000bd8:	be 1b 30 00 60       	mov    $0x6000301b,%esi
    60000bdd:	48 89 c7             	mov    %rax,%rdi
    60000be0:	e8 0d fd ff ff       	callq  600008f2 <strequ(char const*, char const*, unsigned long)>
    60000be5:	84 c0                	test   %al,%al
    60000be7:	74 07                	je     60000bf0 <simplifyPath(char*, char const*, unsigned long)+0x111>
    60000be9:	b8 01 00 00 00       	mov    $0x1,%eax
    60000bee:	eb 05                	jmp    60000bf5 <simplifyPath(char*, char const*, unsigned long)+0x116>
    60000bf0:	b8 00 00 00 00       	mov    $0x0,%eax
    60000bf5:	84 c0                	test   %al,%al
    60000bf7:	74 07                	je     60000c00 <simplifyPath(char*, char const*, unsigned long)+0x121>
    60000bf9:	48 83 45 f0 01       	addq   $0x1,-0x10(%rbp)
    60000bfe:	eb 65                	jmp    60000c65 <simplifyPath(char*, char const*, unsigned long)+0x186>
    60000c00:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000c04:	48 83 c0 01          	add    $0x1,%rax
    60000c08:	48 39 45 e8          	cmp    %rax,-0x18(%rbp)
    60000c0c:	76 28                	jbe    60000c36 <simplifyPath(char*, char const*, unsigned long)+0x157>
    60000c0e:	48 8b 55 d0          	mov    -0x30(%rbp),%rdx
    60000c12:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000c16:	48 01 d0             	add    %rdx,%rax
    60000c19:	ba 02 00 00 00       	mov    $0x2,%edx
    60000c1e:	be 1f 30 00 60       	mov    $0x6000301f,%esi
    60000c23:	48 89 c7             	mov    %rax,%rdi
    60000c26:	e8 c7 fc ff ff       	callq  600008f2 <strequ(char const*, char const*, unsigned long)>
    60000c2b:	84 c0                	test   %al,%al
    60000c2d:	74 07                	je     60000c36 <simplifyPath(char*, char const*, unsigned long)+0x157>
    60000c2f:	b8 01 00 00 00       	mov    $0x1,%eax
    60000c34:	eb 05                	jmp    60000c3b <simplifyPath(char*, char const*, unsigned long)+0x15c>
    60000c36:	b8 00 00 00 00       	mov    $0x0,%eax
    60000c3b:	84 c0                	test   %al,%al
    60000c3d:	75 26                	jne    60000c65 <simplifyPath(char*, char const*, unsigned long)+0x186>
    60000c3f:	48 8b 55 d0          	mov    -0x30(%rbp),%rdx
    60000c43:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000c47:	48 01 d0             	add    %rdx,%rax
    60000c4a:	0f b6 10             	movzbl (%rax),%edx
    60000c4d:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000c51:	48 8d 48 01          	lea    0x1(%rax),%rcx
    60000c55:	48 89 4d f8          	mov    %rcx,-0x8(%rbp)
    60000c59:	48 8b 4d d8          	mov    -0x28(%rbp),%rcx
    60000c5d:	48 01 c8             	add    %rcx,%rax
    60000c60:	88 10                	mov    %dl,(%rax)
    60000c62:	eb 01                	jmp    60000c65 <simplifyPath(char*, char const*, unsigned long)+0x186>
    60000c64:	90                   	nop
    60000c65:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000c69:	48 3b 45 c8          	cmp    -0x38(%rbp),%rax
    60000c6d:	73 0a                	jae    60000c79 <simplifyPath(char*, char const*, unsigned long)+0x19a>
    60000c6f:	48 83 45 f0 01       	addq   $0x1,-0x10(%rbp)
    60000c74:	e9 9a fe ff ff       	jmpq   60000b13 <simplifyPath(char*, char const*, unsigned long)+0x34>
    60000c79:	90                   	nop
    60000c7a:	48 8b 55 d8          	mov    -0x28(%rbp),%rdx
    60000c7e:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000c82:	48 01 d0             	add    %rdx,%rax
    60000c85:	c6 00 00             	movb   $0x0,(%rax)
    60000c88:	90                   	nop
    60000c89:	c9                   	leaveq 
    60000c8a:	c3                   	retq   

0000000060000c8b <bufferToArgs(char*)>:
    60000c8b:	55                   	push   %rbp
    60000c8c:	48 89 e5             	mov    %rsp,%rbp
    60000c8f:	48 83 ec 28          	sub    $0x28,%rsp
    60000c93:	48 89 7d d8          	mov    %rdi,-0x28(%rbp)
    60000c97:	c6 45 ff 00          	movb   $0x0,-0x1(%rbp)
    60000c9b:	c6 45 fe 00          	movb   $0x0,-0x2(%rbp)
    60000c9f:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
    60000ca3:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    60000ca7:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000cab:	0f b6 00             	movzbl (%rax),%eax
    60000cae:	84 c0                	test   %al,%al
    60000cb0:	0f 84 9e 00 00 00    	je     60000d54 <bufferToArgs(char*)+0xc9>
    60000cb6:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000cba:	0f b6 00             	movzbl (%rax),%eax
    60000cbd:	88 45 ef             	mov    %al,-0x11(%rbp)
    60000cc0:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000cc4:	0f b6 00             	movzbl (%rax),%eax
    60000cc7:	3c 5c                	cmp    $0x5c,%al
    60000cc9:	75 2b                	jne    60000cf6 <bufferToArgs(char*)+0x6b>
    60000ccb:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000ccf:	48 83 c0 01          	add    $0x1,%rax
    60000cd3:	0f b6 00             	movzbl (%rax),%eax
    60000cd6:	3c 22                	cmp    $0x22,%al
    60000cd8:	75 1c                	jne    60000cf6 <bufferToArgs(char*)+0x6b>
    60000cda:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000cde:	0f b6 40 01          	movzbl 0x1(%rax),%eax
    60000ce2:	88 45 ef             	mov    %al,-0x11(%rbp)
    60000ce5:	48 83 45 f0 01       	addq   $0x1,-0x10(%rbp)
    60000cea:	0f b6 45 fe          	movzbl -0x2(%rbp),%eax
    60000cee:	83 e8 01             	sub    $0x1,%eax
    60000cf1:	88 45 fe             	mov    %al,-0x2(%rbp)
    60000cf4:	eb 35                	jmp    60000d2b <bufferToArgs(char*)+0xa0>
    60000cf6:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000cfa:	0f b6 00             	movzbl (%rax),%eax
    60000cfd:	3c 22                	cmp    $0x22,%al
    60000cff:	75 10                	jne    60000d11 <bufferToArgs(char*)+0x86>
    60000d01:	80 75 ff 01          	xorb   $0x1,-0x1(%rbp)
    60000d05:	0f b6 45 fe          	movzbl -0x2(%rbp),%eax
    60000d09:	83 e8 01             	sub    $0x1,%eax
    60000d0c:	88 45 fe             	mov    %al,-0x2(%rbp)
    60000d0f:	eb 39                	jmp    60000d4a <bufferToArgs(char*)+0xbf>
    60000d11:	0f b6 45 ff          	movzbl -0x1(%rbp),%eax
    60000d15:	83 f0 01             	xor    $0x1,%eax
    60000d18:	84 c0                	test   %al,%al
    60000d1a:	74 0f                	je     60000d2b <bufferToArgs(char*)+0xa0>
    60000d1c:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000d20:	0f b6 00             	movzbl (%rax),%eax
    60000d23:	3c 20                	cmp    $0x20,%al
    60000d25:	75 04                	jne    60000d2b <bufferToArgs(char*)+0xa0>
    60000d27:	c6 45 ef 00          	movb   $0x0,-0x11(%rbp)
    60000d2b:	48 0f be 55 fe       	movsbq -0x2(%rbp),%rdx
    60000d30:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000d34:	48 01 c2             	add    %rax,%rdx
    60000d37:	0f b6 45 ef          	movzbl -0x11(%rbp),%eax
    60000d3b:	88 02                	mov    %al,(%rdx)
    60000d3d:	80 7d fe 00          	cmpb   $0x0,-0x2(%rbp)
    60000d41:	74 07                	je     60000d4a <bufferToArgs(char*)+0xbf>
    60000d43:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000d47:	c6 00 00             	movb   $0x0,(%rax)
    60000d4a:	48 83 45 f0 01       	addq   $0x1,-0x10(%rbp)
    60000d4f:	e9 53 ff ff ff       	jmpq   60000ca7 <bufferToArgs(char*)+0x1c>
    60000d54:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
    60000d58:	c9                   	leaveq 
    60000d59:	c3                   	retq   

0000000060000d5a <isFile(unsigned long)>:
    60000d5a:	55                   	push   %rbp
    60000d5b:	48 89 e5             	mov    %rsp,%rbp
    60000d5e:	48 83 ec 10          	sub    $0x10,%rsp
    60000d62:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    60000d66:	48 83 7d f8 00       	cmpq   $0x0,-0x8(%rbp)
    60000d6b:	74 27                	je     60000d94 <isFile(unsigned long)+0x3a>
    60000d6d:	48 8b 05 94 12 00 00 	mov    0x1294(%rip),%rax        # 60002008 <systemDevice>
    60000d74:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    60000d78:	48 89 d6             	mov    %rdx,%rsi
    60000d7b:	48 89 c7             	mov    %rax,%rdi
    60000d7e:	e8 32 f7 ff ff       	callq  600004b5 <getFlagsOfINode(unsigned long, unsigned long)>
    60000d83:	25 00 80 00 00       	and    $0x8000,%eax
    60000d88:	48 85 c0             	test   %rax,%rax
    60000d8b:	74 07                	je     60000d94 <isFile(unsigned long)+0x3a>
    60000d8d:	b8 01 00 00 00       	mov    $0x1,%eax
    60000d92:	eb 05                	jmp    60000d99 <isFile(unsigned long)+0x3f>
    60000d94:	b8 00 00 00 00       	mov    $0x0,%eax
    60000d99:	c9                   	leaveq 
    60000d9a:	c3                   	retq   

0000000060000d9b <main>:
    60000d9b:	55                   	push   %rbp
    60000d9c:	48 89 e5             	mov    %rsp,%rbp
    60000d9f:	48 81 ec 80 03 00 00 	sub    $0x380,%rsp
    60000da6:	89 bd 8c fc ff ff    	mov    %edi,-0x374(%rbp)
    60000dac:	48 89 b5 80 fc ff ff 	mov    %rsi,-0x380(%rbp)
    60000db3:	66 c7 45 fe 00 00    	movw   $0x0,-0x2(%rbp)
    60000db9:	c7 85 a1 fe ff ff 2f 	movl   $0x62696c2f,-0x15f(%rbp)
    60000dc0:	6c 69 62 
    60000dc3:	66 c7 85 a5 fe ff ff 	movw   $0x2f,-0x15b(%rbp)
    60000dca:	2f 00 
    60000dcc:	c7 85 9c fe ff ff 2e 	movl   $0x666c652e,-0x164(%rbp)
    60000dd3:	65 6c 66 
    60000dd6:	c6 85 a0 fe ff ff 00 	movb   $0x0,-0x160(%rbp)
    60000ddd:	bf 22 30 00 60       	mov    $0x60003022,%edi
    60000de2:	e8 d1 fa ff ff       	callq  600008b8 <print(char const*)>
    60000de7:	48 c7 45 f0 02 00 00 	movq   $0x2,-0x10(%rbp)
    60000dee:	00 
    60000def:	e8 b6 f5 ff ff       	callq  600003aa <getSystemDevice()>
    60000df4:	48 89 05 0d 12 00 00 	mov    %rax,0x120d(%rip)        # 60002008 <systemDevice>
    60000dfb:	48 8d 85 9c fd ff ff 	lea    -0x264(%rbp),%rax
    60000e02:	ba 00 01 00 00       	mov    $0x100,%edx
    60000e07:	be 00 00 00 00       	mov    $0x0,%esi
    60000e0c:	48 89 c7             	mov    %rax,%rdi
    60000e0f:	e8 84 fb ff ff       	callq  60000998 <memset(void*, char, unsigned long)>
    60000e14:	48 8d 85 9c fd ff ff 	lea    -0x264(%rbp),%rax
    60000e1b:	ba 02 00 00 00       	mov    $0x2,%edx
    60000e20:	be 2f 30 00 60       	mov    $0x6000302f,%esi
    60000e25:	48 89 c7             	mov    %rax,%rdi
    60000e28:	e8 20 fb ff ff       	callq  6000094d <memcpy(void*, void const*, unsigned long)>
    60000e2d:	48 8d 85 a7 fe ff ff 	lea    -0x159(%rbp),%rax
    60000e34:	ba 01 01 00 00       	mov    $0x101,%edx
    60000e39:	be 00 00 00 00       	mov    $0x0,%esi
    60000e3e:	48 89 c7             	mov    %rax,%rdi
    60000e41:	e8 52 fb ff ff       	callq  60000998 <memset(void*, char, unsigned long)>
    60000e46:	66 c7 45 fe 00 00    	movw   $0x0,-0x2(%rbp)
    60000e4c:	48 8d 85 9c fd ff ff 	lea    -0x264(%rbp),%rax
    60000e53:	48 89 c7             	mov    %rax,%rdi
    60000e56:	e8 5d fa ff ff       	callq  600008b8 <print(char const*)>
    60000e5b:	bf 31 30 00 60       	mov    $0x60003031,%edi
    60000e60:	e8 53 fa ff ff       	callq  600008b8 <print(char const*)>
    60000e65:	c6 45 fd 01          	movb   $0x1,-0x3(%rbp)
    60000e69:	80 7d fd 00          	cmpb   $0x0,-0x3(%rbp)
    60000e6d:	74 57                	je     60000ec6 <main+0x12b>
    60000e6f:	e8 8a f5 ff ff       	callq  600003fe <read()>
    60000e74:	88 45 ef             	mov    %al,-0x11(%rbp)
    60000e77:	0f b6 45 ef          	movzbl -0x11(%rbp),%eax
    60000e7b:	89 c7                	mov    %eax,%edi
    60000e7d:	e8 51 f5 ff ff       	callq  600003d3 <write(unsigned char)>
    60000e82:	80 7d ef 0a          	cmpb   $0xa,-0x11(%rbp)
    60000e86:	74 06                	je     60000e8e <main+0xf3>
    60000e88:	80 7d ef 0d          	cmpb   $0xd,-0x11(%rbp)
    60000e8c:	75 10                	jne    60000e9e <main+0x103>
    60000e8e:	bf 0a 00 00 00       	mov    $0xa,%edi
    60000e93:	e8 3b f5 ff ff       	callq  600003d3 <write(unsigned char)>
    60000e98:	c6 45 fd 00          	movb   $0x0,-0x3(%rbp)
    60000e9c:	eb 26                	jmp    60000ec4 <main+0x129>
    60000e9e:	0f b6 55 ef          	movzbl -0x11(%rbp),%edx
    60000ea2:	0f b7 45 fe          	movzwl -0x2(%rbp),%eax
    60000ea6:	8d 48 01             	lea    0x1(%rax),%ecx
    60000ea9:	66 89 4d fe          	mov    %cx,-0x2(%rbp)
    60000ead:	0f b7 c0             	movzwl %ax,%eax
    60000eb0:	48 98                	cltq   
    60000eb2:	88 94 05 a7 fe ff ff 	mov    %dl,-0x159(%rbp,%rax,1)
    60000eb9:	0f b6 45 ef          	movzbl -0x11(%rbp),%eax
    60000ebd:	89 c7                	mov    %eax,%edi
    60000ebf:	e8 0f f5 ff ff       	callq  600003d3 <write(unsigned char)>
    60000ec4:	eb a3                	jmp    60000e69 <main+0xce>
    60000ec6:	48 8d 85 a7 fe ff ff 	lea    -0x159(%rbp),%rax
    60000ecd:	ba 05 00 00 00       	mov    $0x5,%edx
    60000ed2:	be 34 30 00 60       	mov    $0x60003034,%esi
    60000ed7:	48 89 c7             	mov    %rax,%rdi
    60000eda:	e8 13 fa ff ff       	callq  600008f2 <strequ(char const*, char const*, unsigned long)>
    60000edf:	84 c0                	test   %al,%al
    60000ee1:	74 29                	je     60000f0c <main+0x171>
    60000ee3:	e8 42 f1 ff ff       	callq  6000002a <getpid()>
    60000ee8:	48 83 f8 01          	cmp    $0x1,%rax
    60000eec:	0f 94 c0             	sete   %al
    60000eef:	84 c0                	test   %al,%al
    60000ef1:	74 0f                	je     60000f02 <main+0x167>
    60000ef3:	bf 40 30 00 60       	mov    $0x60003040,%edi
    60000ef8:	e8 bb f9 ff ff       	callq  600008b8 <print(char const*)>
    60000efd:	e9 2b ff ff ff       	jmpq   60000e2d <main+0x92>
    60000f02:	b8 00 00 00 00       	mov    $0x0,%eax
    60000f07:	e9 db 02 00 00       	jmpq   600011e7 <main+0x44c>
    60000f0c:	48 8d 85 a7 fe ff ff 	lea    -0x159(%rbp),%rax
    60000f13:	ba 03 00 00 00       	mov    $0x3,%edx
    60000f18:	be 70 30 00 60       	mov    $0x60003070,%esi
    60000f1d:	48 89 c7             	mov    %rax,%rdi
    60000f20:	e8 cd f9 ff ff       	callq  600008f2 <strequ(char const*, char const*, unsigned long)>
    60000f25:	84 c0                	test   %al,%al
    60000f27:	0f 84 db 00 00 00    	je     60001008 <main+0x26d>
    60000f2d:	48 8d 85 a7 fe ff ff 	lea    -0x159(%rbp),%rax
    60000f34:	48 89 c7             	mov    %rax,%rdi
    60000f37:	e8 9e fa ff ff       	callq  600009da <strlen(char const*)>
    60000f3c:	48 83 f8 03          	cmp    $0x3,%rax
    60000f40:	0f 97 c0             	seta   %al
    60000f43:	84 c0                	test   %al,%al
    60000f45:	0f 84 e2 fe ff ff    	je     60000e2d <main+0x92>
    60000f4b:	48 8d 85 a7 fe ff ff 	lea    -0x159(%rbp),%rax
    60000f52:	48 83 c0 03          	add    $0x3,%rax
    60000f56:	48 8b 15 ab 10 00 00 	mov    0x10ab(%rip),%rdx        # 60002008 <systemDevice>
    60000f5d:	48 89 c6             	mov    %rax,%rsi
    60000f60:	48 89 d7             	mov    %rdx,%rdi
    60000f63:	e8 bf f4 ff ff       	callq  60000427 <getINodeOfPath(unsigned long, char const*)>
    60000f68:	48 89 45 e0          	mov    %rax,-0x20(%rbp)
    60000f6c:	48 83 7d e0 00       	cmpq   $0x0,-0x20(%rbp)
    60000f71:	74 27                	je     60000f9a <main+0x1ff>
    60000f73:	48 8b 05 8e 10 00 00 	mov    0x108e(%rip),%rax        # 60002008 <systemDevice>
    60000f7a:	48 8b 55 e0          	mov    -0x20(%rbp),%rdx
    60000f7e:	48 89 d6             	mov    %rdx,%rsi
    60000f81:	48 89 c7             	mov    %rax,%rdi
    60000f84:	e8 2c f5 ff ff       	callq  600004b5 <getFlagsOfINode(unsigned long, unsigned long)>
    60000f89:	25 00 40 00 00       	and    $0x4000,%eax
    60000f8e:	48 85 c0             	test   %rax,%rax
    60000f91:	74 07                	je     60000f9a <main+0x1ff>
    60000f93:	b8 01 00 00 00       	mov    $0x1,%eax
    60000f98:	eb 05                	jmp    60000f9f <main+0x204>
    60000f9a:	b8 00 00 00 00       	mov    $0x0,%eax
    60000f9f:	84 c0                	test   %al,%al
    60000fa1:	74 56                	je     60000ff9 <main+0x25e>
    60000fa3:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
    60000fa7:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    60000fab:	48 8d 85 9c fd ff ff 	lea    -0x264(%rbp),%rax
    60000fb2:	ba 00 01 00 00       	mov    $0x100,%edx
    60000fb7:	be 00 00 00 00       	mov    $0x0,%esi
    60000fbc:	48 89 c7             	mov    %rax,%rdi
    60000fbf:	e8 d4 f9 ff ff       	callq  60000998 <memset(void*, char, unsigned long)>
    60000fc4:	48 8d 85 a7 fe ff ff 	lea    -0x159(%rbp),%rax
    60000fcb:	48 89 c7             	mov    %rax,%rdi
    60000fce:	e8 07 fa ff ff       	callq  600009da <strlen(char const*)>
    60000fd3:	48 8d 50 fd          	lea    -0x3(%rax),%rdx
    60000fd7:	48 8d 85 a7 fe ff ff 	lea    -0x159(%rbp),%rax
    60000fde:	48 83 c0 03          	add    $0x3,%rax
    60000fe2:	48 8d 8d 9c fd ff ff 	lea    -0x264(%rbp),%rcx
    60000fe9:	48 89 c6             	mov    %rax,%rsi
    60000fec:	48 89 cf             	mov    %rcx,%rdi
    60000fef:	e8 59 f9 ff ff       	callq  6000094d <memcpy(void*, void const*, unsigned long)>
    60000ff4:	e9 34 fe ff ff       	jmpq   60000e2d <main+0x92>
    60000ff9:	bf 74 30 00 60       	mov    $0x60003074,%edi
    60000ffe:	e8 b5 f8 ff ff       	callq  600008b8 <print(char const*)>
    60001003:	e9 25 fe ff ff       	jmpq   60000e2d <main+0x92>
    60001008:	48 8d 85 a7 fe ff ff 	lea    -0x159(%rbp),%rax
    6000100f:	ba 02 00 00 00       	mov    $0x2,%edx
    60001014:	be 89 30 00 60       	mov    $0x60003089,%esi
    60001019:	48 89 c7             	mov    %rax,%rdi
    6000101c:	e8 d1 f8 ff ff       	callq  600008f2 <strequ(char const*, char const*, unsigned long)>
    60001021:	84 c0                	test   %al,%al
    60001023:	0f 84 b4 00 00 00    	je     600010dd <main+0x342>
    60001029:	48 8d 85 a7 fe ff ff 	lea    -0x159(%rbp),%rax
    60001030:	48 89 c7             	mov    %rax,%rdi
    60001033:	e8 a2 f9 ff ff       	callq  600009da <strlen(char const*)>
    60001038:	48 83 f8 02          	cmp    $0x2,%rax
    6000103c:	0f 97 c0             	seta   %al
    6000103f:	84 c0                	test   %al,%al
    60001041:	0f 84 e6 fd ff ff    	je     60000e2d <main+0x92>
    60001047:	48 8d 85 a7 fe ff ff 	lea    -0x159(%rbp),%rax
    6000104e:	48 83 c0 02          	add    $0x2,%rax
    60001052:	48 8b 15 af 0f 00 00 	mov    0xfaf(%rip),%rdx        # 60002008 <systemDevice>
    60001059:	48 89 c6             	mov    %rax,%rsi
    6000105c:	48 89 d7             	mov    %rdx,%rdi
    6000105f:	e8 c3 f3 ff ff       	callq  60000427 <getINodeOfPath(unsigned long, char const*)>
    60001064:	48 89 45 d8          	mov    %rax,-0x28(%rbp)
    60001068:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
    6000106c:	48 89 c7             	mov    %rax,%rdi
    6000106f:	e8 e6 fc ff ff       	callq  60000d5a <isFile(unsigned long)>
    60001074:	84 c0                	test   %al,%al
    60001076:	74 56                	je     600010ce <main+0x333>
    60001078:	48 8d 85 a7 fe ff ff 	lea    -0x159(%rbp),%rax
    6000107f:	48 83 c0 02          	add    $0x2,%rax
    60001083:	48 89 c7             	mov    %rax,%rdi
    60001086:	e8 00 fc ff ff       	callq  60000c8b <bufferToArgs(char*)>
    6000108b:	48 89 45 d0          	mov    %rax,-0x30(%rbp)
    6000108f:	48 8d 85 a7 fe ff ff 	lea    -0x159(%rbp),%rax
    60001096:	48 83 c0 02          	add    $0x2,%rax
    6000109a:	48 8b 3d 67 0f 00 00 	mov    0xf67(%rip),%rdi        # 60002008 <systemDevice>
    600010a1:	48 8b 55 d0          	mov    -0x30(%rbp),%rdx
    600010a5:	b9 00 00 00 00       	mov    $0x0,%ecx
    600010aa:	48 89 c6             	mov    %rax,%rsi
    600010ad:	e8 a1 ef ff ff       	callq  60000053 <createProcess(unsigned long, char const*, char const*, bool)>
    600010b2:	48 89 45 c8          	mov    %rax,-0x38(%rbp)
    600010b6:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
    600010ba:	48 89 c7             	mov    %rax,%rdi
    600010bd:	e8 1f f1 ff ff       	callq  600001e1 <waitForProcess(unsigned long)>
    600010c2:	48 89 05 47 0f 00 00 	mov    %rax,0xf47(%rip)        # 60002010 <returnValue>
    600010c9:	e9 5f fd ff ff       	jmpq   60000e2d <main+0x92>
    600010ce:	bf 8c 30 00 60       	mov    $0x6000308c,%edi
    600010d3:	e8 e0 f7 ff ff       	callq  600008b8 <print(char const*)>
    600010d8:	e9 50 fd ff ff       	jmpq   60000e2d <main+0x92>
    600010dd:	48 8d 85 a7 fe ff ff 	lea    -0x159(%rbp),%rax
    600010e4:	48 89 c7             	mov    %rax,%rdi
    600010e7:	e8 9f fb ff ff       	callq  60000c8b <bufferToArgs(char*)>
    600010ec:	48 89 45 c0          	mov    %rax,-0x40(%rbp)
    600010f0:	c6 45 a7 00          	movb   $0x0,-0x59(%rbp)
    600010f4:	48 8d 85 a7 fe ff ff 	lea    -0x159(%rbp),%rax
    600010fb:	48 89 c7             	mov    %rax,%rdi
    600010fe:	e8 d7 f8 ff ff       	callq  600009da <strlen(char const*)>
    60001103:	48 89 45 b8          	mov    %rax,-0x48(%rbp)
    60001107:	48 8d 8d a1 fe ff ff 	lea    -0x15f(%rbp),%rcx
    6000110e:	48 8d 85 91 fc ff ff 	lea    -0x36f(%rbp),%rax
    60001115:	ba 06 00 00 00       	mov    $0x6,%edx
    6000111a:	48 89 ce             	mov    %rcx,%rsi
    6000111d:	48 89 c7             	mov    %rax,%rdi
    60001120:	e8 28 f8 ff ff       	callq  6000094d <memcpy(void*, void const*, unsigned long)>
    60001125:	48 8d 85 91 fc ff ff 	lea    -0x36f(%rbp),%rax
    6000112c:	48 83 c0 05          	add    $0x5,%rax
    60001130:	48 8b 55 b8          	mov    -0x48(%rbp),%rdx
    60001134:	48 8d 8d a7 fe ff ff 	lea    -0x159(%rbp),%rcx
    6000113b:	48 89 ce             	mov    %rcx,%rsi
    6000113e:	48 89 c7             	mov    %rax,%rdi
    60001141:	e8 07 f8 ff ff       	callq  6000094d <memcpy(void*, void const*, unsigned long)>
    60001146:	48 8b 45 b8          	mov    -0x48(%rbp),%rax
    6000114a:	48 8d 50 05          	lea    0x5(%rax),%rdx
    6000114e:	48 8d 85 91 fc ff ff 	lea    -0x36f(%rbp),%rax
    60001155:	48 8d 0c 10          	lea    (%rax,%rdx,1),%rcx
    60001159:	48 8d 85 9c fe ff ff 	lea    -0x164(%rbp),%rax
    60001160:	ba 05 00 00 00       	mov    $0x5,%edx
    60001165:	48 89 c6             	mov    %rax,%rsi
    60001168:	48 89 cf             	mov    %rcx,%rdi
    6000116b:	e8 dd f7 ff ff       	callq  6000094d <memcpy(void*, void const*, unsigned long)>
    60001170:	48 8b 05 91 0e 00 00 	mov    0xe91(%rip),%rax        # 60002008 <systemDevice>
    60001177:	48 8d 95 91 fc ff ff 	lea    -0x36f(%rbp),%rdx
    6000117e:	48 89 d6             	mov    %rdx,%rsi
    60001181:	48 89 c7             	mov    %rax,%rdi
    60001184:	e8 9e f2 ff ff       	callq  60000427 <getINodeOfPath(unsigned long, char const*)>
    60001189:	48 89 45 b0          	mov    %rax,-0x50(%rbp)
    6000118d:	48 8b 45 b0          	mov    -0x50(%rbp),%rax
    60001191:	48 89 c7             	mov    %rax,%rdi
    60001194:	e8 c1 fb ff ff       	callq  60000d5a <isFile(unsigned long)>
    60001199:	84 c0                	test   %al,%al
    6000119b:	74 3b                	je     600011d8 <main+0x43d>
    6000119d:	48 8b 05 64 0e 00 00 	mov    0xe64(%rip),%rax        # 60002008 <systemDevice>
    600011a4:	48 8b 55 c0          	mov    -0x40(%rbp),%rdx
    600011a8:	48 8d b5 91 fc ff ff 	lea    -0x36f(%rbp),%rsi
    600011af:	b9 00 00 00 00       	mov    $0x0,%ecx
    600011b4:	48 89 c7             	mov    %rax,%rdi
    600011b7:	e8 97 ee ff ff       	callq  60000053 <createProcess(unsigned long, char const*, char const*, bool)>
    600011bc:	48 89 45 a8          	mov    %rax,-0x58(%rbp)
    600011c0:	48 8b 45 a8          	mov    -0x58(%rbp),%rax
    600011c4:	48 89 c7             	mov    %rax,%rdi
    600011c7:	e8 15 f0 ff ff       	callq  600001e1 <waitForProcess(unsigned long)>
    600011cc:	48 89 05 3d 0e 00 00 	mov    %rax,0xe3d(%rip)        # 60002010 <returnValue>
    600011d3:	e9 55 fc ff ff       	jmpq   60000e2d <main+0x92>
    600011d8:	bf 9c 30 00 60       	mov    $0x6000309c,%edi
    600011dd:	e8 d6 f6 ff ff       	callq  600008b8 <print(char const*)>
    600011e2:	e9 46 fc ff ff       	jmpq   60000e2d <main+0x92>
    600011e7:	c9                   	leaveq 
    600011e8:	c3                   	retq   

Disassembly of section .text._Z7syscallmmmm:

00000000600011e9 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>:
    600011e9:	55                   	push   %rbp
    600011ea:	48 89 e5             	mov    %rsp,%rbp
    600011ed:	53                   	push   %rbx
    600011ee:	48 83 ec 20          	sub    $0x20,%rsp
    600011f2:	48 89 7d f0          	mov    %rdi,-0x10(%rbp)
    600011f6:	48 89 75 e8          	mov    %rsi,-0x18(%rbp)
    600011fa:	48 89 55 e0          	mov    %rdx,-0x20(%rbp)
    600011fe:	48 89 4d d8          	mov    %rcx,-0x28(%rbp)
    60001202:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60001206:	48 8b 5d e8          	mov    -0x18(%rbp),%rbx
    6000120a:	48 8b 4d e0          	mov    -0x20(%rbp),%rcx
    6000120e:	48 8b 55 d8          	mov    -0x28(%rbp),%rdx
    60001212:	cd 80                	int    $0x80
    60001214:	90                   	nop
    60001215:	48 83 c4 20          	add    $0x20,%rsp
    60001219:	5b                   	pop    %rbx
    6000121a:	5d                   	pop    %rbp
    6000121b:	c3                   	retq   
