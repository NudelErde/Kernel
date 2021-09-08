
../boot/boot.elf:     file format elf64-x86-64


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
    60000022:	e8 98 0c 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000048:	e8 72 0c 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600000a1:	e8 19 0c 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600000ce:	e8 ec 0b 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600000fa:	e8 c0 0b 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000127:	e8 93 0b 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    6000014d:	e8 6d 0b 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000180:	e8 3a 0b 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600001ac:	e8 0e 0b 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600001d9:	e8 e1 0a 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000205:	e8 b5 0a 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    6000022e:	e8 8c 0a 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000257:	e8 63 0a 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000298:	e8 22 0a 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600002f3:	e8 c7 09 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    6000034e:	e8 6c 09 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000379:	e8 41 09 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    6000039f:	e8 1b 09 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600003c8:	e8 f2 08 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600003f6:	e8 c4 08 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    6000041c:	e8 9e 08 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000457:	e8 63 08 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600004aa:	e8 10 08 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600004e5:	e8 d5 07 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000538:	e8 82 07 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    6000057b:	e8 3f 07 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600005ac:	e8 0e 07 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600005df:	e8 db 06 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    6000060f:	e8 ab 06 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000639:	e8 81 06 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000663:	e8 57 06 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000697:	e8 23 06 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600006ca:	e8 f0 05 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600006fe:	e8 bc 05 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000743:	e8 77 05 00 00       	callq  60000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000857:	48 89 05 a2 07 00 00 	mov    %rax,0x7a2(%rip)        # 60001000 <myPid>
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
    60000897:	e8 5e 02 00 00       	callq  60000afa <main>
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

00000000600008b8 <getCPL()>:
    600008b8:	55                   	push   %rbp
    600008b9:	48 89 e5             	mov    %rsp,%rbp
    600008bc:	48 83 ec 10          	sub    $0x10,%rsp
    600008c0:	8c c8                	mov    %cs,%eax
    600008c2:	89 45 fc             	mov    %eax,-0x4(%rbp)
    600008c5:	8b 45 fc             	mov    -0x4(%rbp),%eax
    600008c8:	83 e0 03             	and    $0x3,%eax
    600008cb:	c9                   	leaveq 
    600008cc:	c3                   	retq   

00000000600008cd <printBase(unsigned long, unsigned long, int)>:
    600008cd:	55                   	push   %rbp
    600008ce:	48 89 e5             	mov    %rsp,%rbp
    600008d1:	48 83 c4 80          	add    $0xffffffffffffff80,%rsp
    600008d5:	48 89 7d 98          	mov    %rdi,-0x68(%rbp)
    600008d9:	48 89 75 90          	mov    %rsi,-0x70(%rbp)
    600008dd:	89 55 8c             	mov    %edx,-0x74(%rbp)
    600008e0:	48 c7 45 e8 00 20 00 	movq   $0x60002000,-0x18(%rbp)
    600008e7:	60 
    600008e8:	48 8d 55 a8          	lea    -0x58(%rbp),%rdx
    600008ec:	b8 00 00 00 00       	mov    $0x0,%eax
    600008f1:	b9 08 00 00 00       	mov    $0x8,%ecx
    600008f6:	48 89 d7             	mov    %rdx,%rdi
    600008f9:	f3 48 ab             	rep stos %rax,%es:(%rdi)
    600008fc:	c7 45 fc 01 00 00 00 	movl   $0x1,-0x4(%rbp)
    60000903:	48 83 7d 98 00       	cmpq   $0x0,-0x68(%rbp)
    60000908:	74 37                	je     60000941 <printBase(unsigned long, unsigned long, int)+0x74>
    6000090a:	48 8b 45 98          	mov    -0x68(%rbp),%rax
    6000090e:	ba 00 00 00 00       	mov    $0x0,%edx
    60000913:	48 f7 75 90          	divq   -0x70(%rbp)
    60000917:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    6000091b:	48 01 d0             	add    %rdx,%rax
    6000091e:	0f b6 10             	movzbl (%rax),%edx
    60000921:	8b 45 fc             	mov    -0x4(%rbp),%eax
    60000924:	48 98                	cltq   
    60000926:	88 54 05 a8          	mov    %dl,-0x58(%rbp,%rax,1)
    6000092a:	48 8b 45 98          	mov    -0x68(%rbp),%rax
    6000092e:	ba 00 00 00 00       	mov    $0x0,%edx
    60000933:	48 f7 75 90          	divq   -0x70(%rbp)
    60000937:	48 89 45 98          	mov    %rax,-0x68(%rbp)
    6000093b:	83 45 fc 01          	addl   $0x1,-0x4(%rbp)
    6000093f:	eb c2                	jmp    60000903 <printBase(unsigned long, unsigned long, int)+0x36>
    60000941:	c7 45 f8 00 00 00 00 	movl   $0x0,-0x8(%rbp)
    60000948:	8b 45 8c             	mov    -0x74(%rbp),%eax
    6000094b:	2b 45 fc             	sub    -0x4(%rbp),%eax
    6000094e:	39 45 f8             	cmp    %eax,-0x8(%rbp)
    60000951:	7f 10                	jg     60000963 <printBase(unsigned long, unsigned long, int)+0x96>
    60000953:	bf 30 00 00 00       	mov    $0x30,%edi
    60000958:	e8 76 fa ff ff       	callq  600003d3 <write(unsigned char)>
    6000095d:	83 45 f8 01          	addl   $0x1,-0x8(%rbp)
    60000961:	eb e5                	jmp    60000948 <printBase(unsigned long, unsigned long, int)+0x7b>
    60000963:	c6 45 e7 00          	movb   $0x0,-0x19(%rbp)
    60000967:	8b 45 fc             	mov    -0x4(%rbp),%eax
    6000096a:	48 98                	cltq   
    6000096c:	48 8d 50 ff          	lea    -0x1(%rax),%rdx
    60000970:	48 8d 45 a8          	lea    -0x58(%rbp),%rax
    60000974:	48 01 d0             	add    %rdx,%rax
    60000977:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    6000097b:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    6000097f:	0f b6 00             	movzbl (%rax),%eax
    60000982:	84 c0                	test   %al,%al
    60000984:	74 18                	je     6000099e <printBase(unsigned long, unsigned long, int)+0xd1>
    60000986:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    6000098a:	0f b6 00             	movzbl (%rax),%eax
    6000098d:	0f b6 c0             	movzbl %al,%eax
    60000990:	89 c7                	mov    %eax,%edi
    60000992:	e8 3c fa ff ff       	callq  600003d3 <write(unsigned char)>
    60000997:	48 83 6d f0 01       	subq   $0x1,-0x10(%rbp)
    6000099c:	eb dd                	jmp    6000097b <printBase(unsigned long, unsigned long, int)+0xae>
    6000099e:	90                   	nop
    6000099f:	c9                   	leaveq 
    600009a0:	c3                   	retq   

00000000600009a1 <printKernelOut(char const*)>:
    600009a1:	55                   	push   %rbp
    600009a2:	48 89 e5             	mov    %rsp,%rbp
    600009a5:	48 83 ec 10          	sub    $0x10,%rsp
    600009a9:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    600009ad:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600009b1:	0f b6 00             	movzbl (%rax),%eax
    600009b4:	84 c0                	test   %al,%al
    600009b6:	74 18                	je     600009d0 <printKernelOut(char const*)+0x2f>
    600009b8:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600009bc:	0f b6 00             	movzbl (%rax),%eax
    600009bf:	0f b6 c0             	movzbl %al,%eax
    600009c2:	89 c7                	mov    %eax,%edi
    600009c4:	e8 0a fa ff ff       	callq  600003d3 <write(unsigned char)>
    600009c9:	48 83 45 f8 01       	addq   $0x1,-0x8(%rbp)
    600009ce:	eb dd                	jmp    600009ad <printKernelOut(char const*)+0xc>
    600009d0:	90                   	nop
    600009d1:	c9                   	leaveq 
    600009d2:	c3                   	retq   

00000000600009d3 <onWrite(char)>:
    600009d3:	55                   	push   %rbp
    600009d4:	48 89 e5             	mov    %rsp,%rbp
    600009d7:	48 83 ec 08          	sub    $0x8,%rsp
    600009db:	89 f8                	mov    %edi,%eax
    600009dd:	88 45 fc             	mov    %al,-0x4(%rbp)
    600009e0:	90                   	nop
    600009e1:	c9                   	leaveq 
    600009e2:	c3                   	retq   

00000000600009e3 <startByDirEntry(DirectoryEntry*, char const*)>:
    600009e3:	55                   	push   %rbp
    600009e4:	48 89 e5             	mov    %rsp,%rbp
    600009e7:	48 81 ec 30 02 00 00 	sub    $0x230,%rsp
    600009ee:	48 89 bd d8 fd ff ff 	mov    %rdi,-0x228(%rbp)
    600009f5:	48 89 b5 d0 fd ff ff 	mov    %rsi,-0x230(%rbp)
    600009fc:	48 8b 85 d8 fd ff ff 	mov    -0x228(%rbp),%rax
    60000a03:	48 8b 40 10          	mov    0x10(%rax),%rax
    60000a07:	0f b6 00             	movzbl (%rax),%eax
    60000a0a:	3c 2e                	cmp    $0x2e,%al
    60000a0c:	0f 84 e5 00 00 00    	je     60000af7 <startByDirEntry(DirectoryEntry*, char const*)+0x114>
    60000a12:	48 c7 45 f8 00 00 00 	movq   $0x0,-0x8(%rbp)
    60000a19:	00 
    60000a1a:	48 8b 85 d0 fd ff ff 	mov    -0x230(%rbp),%rax
    60000a21:	48 89 45 f0          	mov    %rax,-0x10(%rbp)
    60000a25:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000a29:	0f b6 00             	movzbl (%rax),%eax
    60000a2c:	84 c0                	test   %al,%al
    60000a2e:	74 21                	je     60000a51 <startByDirEntry(DirectoryEntry*, char const*)+0x6e>
    60000a30:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000a34:	0f b6 10             	movzbl (%rax),%edx
    60000a37:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000a3b:	48 8d 48 01          	lea    0x1(%rax),%rcx
    60000a3f:	48 89 4d f8          	mov    %rcx,-0x8(%rbp)
    60000a43:	88 94 05 e8 fd ff ff 	mov    %dl,-0x218(%rbp,%rax,1)
    60000a4a:	48 83 45 f0 01       	addq   $0x1,-0x10(%rbp)
    60000a4f:	eb d4                	jmp    60000a25 <startByDirEntry(DirectoryEntry*, char const*)+0x42>
    60000a51:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000a55:	48 8d 50 01          	lea    0x1(%rax),%rdx
    60000a59:	48 89 55 f8          	mov    %rdx,-0x8(%rbp)
    60000a5d:	c6 84 05 e8 fd ff ff 	movb   $0x2f,-0x218(%rbp,%rax,1)
    60000a64:	2f 
    60000a65:	48 8b 85 d8 fd ff ff 	mov    -0x228(%rbp),%rax
    60000a6c:	48 8b 40 10          	mov    0x10(%rax),%rax
    60000a70:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
    60000a74:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    60000a78:	0f b6 00             	movzbl (%rax),%eax
    60000a7b:	84 c0                	test   %al,%al
    60000a7d:	74 21                	je     60000aa0 <startByDirEntry(DirectoryEntry*, char const*)+0xbd>
    60000a7f:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    60000a83:	0f b6 10             	movzbl (%rax),%edx
    60000a86:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000a8a:	48 8d 48 01          	lea    0x1(%rax),%rcx
    60000a8e:	48 89 4d f8          	mov    %rcx,-0x8(%rbp)
    60000a92:	88 94 05 e8 fd ff ff 	mov    %dl,-0x218(%rbp,%rax,1)
    60000a99:	48 83 45 e8 01       	addq   $0x1,-0x18(%rbp)
    60000a9e:	eb d4                	jmp    60000a74 <startByDirEntry(DirectoryEntry*, char const*)+0x91>
    60000aa0:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000aa4:	48 8d 50 01          	lea    0x1(%rax),%rdx
    60000aa8:	48 89 55 f8          	mov    %rdx,-0x8(%rbp)
    60000aac:	c6 84 05 e8 fd ff ff 	movb   $0x0,-0x218(%rbp,%rax,1)
    60000ab3:	00 
    60000ab4:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000ab8:	48 8d 50 01          	lea    0x1(%rax),%rdx
    60000abc:	48 89 55 f8          	mov    %rdx,-0x8(%rbp)
    60000ac0:	c6 84 05 e8 fd ff ff 	movb   $0x0,-0x218(%rbp,%rax,1)
    60000ac7:	00 
    60000ac8:	bf 11 20 00 60       	mov    $0x60002011,%edi
    60000acd:	e8 cf fe ff ff       	callq  600009a1 <printKernelOut(char const*)>
    60000ad2:	bf 2b 20 00 60       	mov    $0x6000202b,%edi
    60000ad7:	e8 c5 fe ff ff       	callq  600009a1 <printKernelOut(char const*)>
    60000adc:	48 8d 85 e8 fd ff ff 	lea    -0x218(%rbp),%rax
    60000ae3:	48 89 c7             	mov    %rax,%rdi
    60000ae6:	e8 b6 fe ff ff       	callq  600009a1 <printKernelOut(char const*)>
    60000aeb:	bf 43 20 00 60       	mov    $0x60002043,%edi
    60000af0:	e8 ac fe ff ff       	callq  600009a1 <printKernelOut(char const*)>
    60000af5:	eb 01                	jmp    60000af8 <startByDirEntry(DirectoryEntry*, char const*)+0x115>
    60000af7:	90                   	nop
    60000af8:	c9                   	leaveq 
    60000af9:	c3                   	retq   

0000000060000afa <main>:
    60000afa:	55                   	push   %rbp
    60000afb:	48 89 e5             	mov    %rsp,%rbp
    60000afe:	48 81 ec 00 01 00 00 	sub    $0x100,%rsp
    60000b05:	e8 a0 f8 ff ff       	callq  600003aa <getSystemDevice()>
    60000b0a:	48 89 05 f7 04 00 00 	mov    %rax,0x4f7(%rip)        # 60001008 <systemDevice>
    60000b11:	bf 46 20 00 60       	mov    $0x60002046,%edi
    60000b16:	e8 86 fe ff ff       	callq  600009a1 <printKernelOut(char const*)>
    60000b1b:	e8 98 fd ff ff       	callq  600008b8 <getCPL()>
    60000b20:	0f b6 c0             	movzbl %al,%eax
    60000b23:	ba 01 00 00 00       	mov    $0x1,%edx
    60000b28:	be 0a 00 00 00       	mov    $0xa,%esi
    60000b2d:	48 89 c7             	mov    %rax,%rdi
    60000b30:	e8 98 fd ff ff       	callq  600008cd <printBase(unsigned long, unsigned long, int)>
    60000b35:	bf 54 20 00 60       	mov    $0x60002054,%edi
    60000b3a:	e8 62 fe ff ff       	callq  600009a1 <printKernelOut(char const*)>
    60000b3f:	48 8b 05 c2 04 00 00 	mov    0x4c2(%rip),%rax        # 60001008 <systemDevice>
    60000b46:	b9 00 00 00 00       	mov    $0x0,%ecx
    60000b4b:	ba 56 20 00 60       	mov    $0x60002056,%edx
    60000b50:	be 66 20 00 60       	mov    $0x60002066,%esi
    60000b55:	48 89 c7             	mov    %rax,%rdi
    60000b58:	e8 f6 f4 ff ff       	callq  60000053 <createProcess(unsigned long, char const*, char const*, bool)>
    60000b5d:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
    60000b61:	48 8b 05 a0 04 00 00 	mov    0x4a0(%rip),%rax        # 60001008 <systemDevice>
    60000b68:	be 75 20 00 60       	mov    $0x60002075,%esi
    60000b6d:	48 89 c7             	mov    %rax,%rdi
    60000b70:	e8 b2 f8 ff ff       	callq  60000427 <getINodeOfPath(unsigned long, char const*)>
    60000b75:	48 89 45 e0          	mov    %rax,-0x20(%rbp)
    60000b79:	48 c7 45 d8 08 00 00 	movq   $0x8,-0x28(%rbp)
    60000b80:	00 
    60000b81:	48 8b 05 80 04 00 00 	mov    0x480(%rip),%rax        # 60001008 <systemDevice>
    60000b88:	48 8d 95 08 ff ff ff 	lea    -0xf8(%rbp),%rdx
    60000b8f:	48 8b 75 e0          	mov    -0x20(%rbp),%rsi
    60000b93:	48 89 d1             	mov    %rdx,%rcx
    60000b96:	ba 08 00 00 00       	mov    $0x8,%edx
    60000b9b:	48 89 c7             	mov    %rax,%rdi
    60000b9e:	e8 4d f9 ff ff       	callq  600004f0 <getDirectoryEntriesOfINode(unsigned long, unsigned long, unsigned long, DirectoryEntry*)>
    60000ba3:	48 89 85 00 ff ff ff 	mov    %rax,-0x100(%rbp)
    60000baa:	48 8b 85 00 ff ff ff 	mov    -0x100(%rbp),%rax
    60000bb1:	48 83 f8 07          	cmp    $0x7,%rax
    60000bb5:	77 0c                	ja     60000bc3 <main+0xc9>
    60000bb7:	48 8d 85 00 ff ff ff 	lea    -0x100(%rbp),%rax
    60000bbe:	48 8b 00             	mov    (%rax),%rax
    60000bc1:	eb 05                	jmp    60000bc8 <main+0xce>
    60000bc3:	b8 08 00 00 00       	mov    $0x8,%eax
    60000bc8:	48 89 45 d0          	mov    %rax,-0x30(%rbp)
    60000bcc:	48 c7 45 f8 00 00 00 	movq   $0x0,-0x8(%rbp)
    60000bd3:	00 
    60000bd4:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000bd8:	48 3b 45 d0          	cmp    -0x30(%rbp),%rax
    60000bdc:	73 2f                	jae    60000c0d <main+0x113>
    60000bde:	48 8d 8d 08 ff ff ff 	lea    -0xf8(%rbp),%rcx
    60000be5:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    60000be9:	48 89 d0             	mov    %rdx,%rax
    60000bec:	48 01 c0             	add    %rax,%rax
    60000bef:	48 01 d0             	add    %rdx,%rax
    60000bf2:	48 c1 e0 03          	shl    $0x3,%rax
    60000bf6:	48 01 c8             	add    %rcx,%rax
    60000bf9:	be 75 20 00 60       	mov    $0x60002075,%esi
    60000bfe:	48 89 c7             	mov    %rax,%rdi
    60000c01:	e8 dd fd ff ff       	callq  600009e3 <startByDirEntry(DirectoryEntry*, char const*)>
    60000c06:	48 83 45 f8 01       	addq   $0x1,-0x8(%rbp)
    60000c0b:	eb c7                	jmp    60000bd4 <main+0xda>
    60000c0d:	48 8b 85 00 ff ff ff 	mov    -0x100(%rbp),%rax
    60000c14:	48 83 f8 08          	cmp    $0x8,%rax
    60000c18:	0f 86 95 00 00 00    	jbe    60000cb3 <main+0x1b9>
    60000c1e:	48 8b 95 00 ff ff ff 	mov    -0x100(%rbp),%rdx
    60000c25:	48 89 d0             	mov    %rdx,%rax
    60000c28:	48 01 c0             	add    %rax,%rax
    60000c2b:	48 01 d0             	add    %rdx,%rax
    60000c2e:	48 c1 e0 03          	shl    $0x3,%rax
    60000c32:	48 89 c7             	mov    %rax,%rdi
    60000c35:	e8 9c f4 ff ff       	callq  600000d6 <malloc(unsigned long)>
    60000c3a:	48 89 45 c8          	mov    %rax,-0x38(%rbp)
    60000c3e:	48 8b 95 00 ff ff ff 	mov    -0x100(%rbp),%rdx
    60000c45:	48 8b 05 bc 03 00 00 	mov    0x3bc(%rip),%rax        # 60001008 <systemDevice>
    60000c4c:	48 8b 4d c8          	mov    -0x38(%rbp),%rcx
    60000c50:	48 8b 75 e0          	mov    -0x20(%rbp),%rsi
    60000c54:	48 89 c7             	mov    %rax,%rdi
    60000c57:	e8 94 f8 ff ff       	callq  600004f0 <getDirectoryEntriesOfINode(unsigned long, unsigned long, unsigned long, DirectoryEntry*)>
    60000c5c:	48 89 85 00 ff ff ff 	mov    %rax,-0x100(%rbp)
    60000c63:	48 c7 45 f0 08 00 00 	movq   $0x8,-0x10(%rbp)
    60000c6a:	00 
    60000c6b:	48 8b 85 00 ff ff ff 	mov    -0x100(%rbp),%rax
    60000c72:	48 39 45 f0          	cmp    %rax,-0x10(%rbp)
    60000c76:	73 2f                	jae    60000ca7 <main+0x1ad>
    60000c78:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
    60000c7c:	48 89 d0             	mov    %rdx,%rax
    60000c7f:	48 01 c0             	add    %rax,%rax
    60000c82:	48 01 d0             	add    %rdx,%rax
    60000c85:	48 c1 e0 03          	shl    $0x3,%rax
    60000c89:	48 89 c2             	mov    %rax,%rdx
    60000c8c:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
    60000c90:	48 01 d0             	add    %rdx,%rax
    60000c93:	be 75 20 00 60       	mov    $0x60002075,%esi
    60000c98:	48 89 c7             	mov    %rax,%rdi
    60000c9b:	e8 43 fd ff ff       	callq  600009e3 <startByDirEntry(DirectoryEntry*, char const*)>
    60000ca0:	48 83 45 f0 01       	addq   $0x1,-0x10(%rbp)
    60000ca5:	eb c4                	jmp    60000c6b <main+0x171>
    60000ca7:	48 8b 45 c8          	mov    -0x38(%rbp),%rax
    60000cab:	48 89 c7             	mov    %rax,%rdi
    60000cae:	e8 52 f4 ff ff       	callq  60000105 <free(void*)>
    60000cb3:	bf 10 27 00 00       	mov    $0x2710,%edi
    60000cb8:	e8 ef f3 ff ff       	callq  600000ac <sleep(unsigned long)>
    60000cbd:	eb f4                	jmp    60000cb3 <main+0x1b9>

Disassembly of section .text._Z7syscallmmmm:

0000000060000cbf <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>:
    60000cbf:	55                   	push   %rbp
    60000cc0:	48 89 e5             	mov    %rsp,%rbp
    60000cc3:	53                   	push   %rbx
    60000cc4:	48 83 ec 20          	sub    $0x20,%rsp
    60000cc8:	48 89 7d f0          	mov    %rdi,-0x10(%rbp)
    60000ccc:	48 89 75 e8          	mov    %rsi,-0x18(%rbp)
    60000cd0:	48 89 55 e0          	mov    %rdx,-0x20(%rbp)
    60000cd4:	48 89 4d d8          	mov    %rcx,-0x28(%rbp)
    60000cd8:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000cdc:	48 8b 5d e8          	mov    -0x18(%rbp),%rbx
    60000ce0:	48 8b 4d e0          	mov    -0x20(%rbp),%rcx
    60000ce4:	48 8b 55 d8          	mov    -0x28(%rbp),%rdx
    60000ce8:	cd 80                	int    $0x80
    60000cea:	90                   	nop
    60000ceb:	48 83 c4 20          	add    $0x20,%rsp
    60000cef:	5b                   	pop    %rbx
    60000cf0:	5d                   	pop    %rbp
    60000cf1:	c3                   	retq   
