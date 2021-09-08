
../dog/dog.elf:     file format elf64-x86-64


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
    60000022:	e8 cc 0c 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000048:	e8 a6 0c 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600000a1:	e8 4d 0c 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600000ce:	e8 20 0c 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600000fa:	e8 f4 0b 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000127:	e8 c7 0b 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    6000014d:	e8 a1 0b 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000180:	e8 6e 0b 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600001ac:	e8 42 0b 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600001d9:	e8 15 0b 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000205:	e8 e9 0a 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    6000022e:	e8 c0 0a 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000257:	e8 97 0a 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000298:	e8 56 0a 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600002f3:	e8 fb 09 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    6000034e:	e8 a0 09 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000379:	e8 75 09 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    6000039f:	e8 4f 09 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600003c8:	e8 26 09 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600003f6:	e8 f8 08 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    6000041c:	e8 d2 08 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000457:	e8 97 08 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600004aa:	e8 44 08 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600004e5:	e8 09 08 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000538:	e8 b6 07 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    6000057b:	e8 73 07 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600005ac:	e8 42 07 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600005df:	e8 0f 07 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    6000060f:	e8 df 06 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000639:	e8 b5 06 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000663:	e8 8b 06 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000697:	e8 57 06 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600006ca:	e8 24 06 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    600006fe:	e8 f0 05 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000743:	e8 ab 05 00 00       	callq  60000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>
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
    60000897:	e8 8f 00 00 00       	callq  6000092b <main>
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
    600008bc:	48 83 ec 10          	sub    $0x10,%rsp
    600008c0:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    600008c4:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600008c8:	0f b6 00             	movzbl (%rax),%eax
    600008cb:	84 c0                	test   %al,%al
    600008cd:	74 18                	je     600008e7 <print(char const*)+0x2f>
    600008cf:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    600008d3:	0f b6 00             	movzbl (%rax),%eax
    600008d6:	0f b6 c0             	movzbl %al,%eax
    600008d9:	89 c7                	mov    %eax,%edi
    600008db:	e8 f3 fa ff ff       	callq  600003d3 <write(unsigned char)>
    600008e0:	48 83 45 f8 01       	addq   $0x1,-0x8(%rbp)
    600008e5:	eb dd                	jmp    600008c4 <print(char const*)+0xc>
    600008e7:	90                   	nop
    600008e8:	c9                   	leaveq 
    600008e9:	c3                   	retq   

00000000600008ea <isFile(unsigned long)>:
    600008ea:	55                   	push   %rbp
    600008eb:	48 89 e5             	mov    %rsp,%rbp
    600008ee:	48 83 ec 10          	sub    $0x10,%rsp
    600008f2:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
    600008f6:	48 83 7d f8 00       	cmpq   $0x0,-0x8(%rbp)
    600008fb:	74 27                	je     60000924 <isFile(unsigned long)+0x3a>
    600008fd:	48 8b 05 1c 07 00 00 	mov    0x71c(%rip),%rax        # 60001020 <systemDevice>
    60000904:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    60000908:	48 89 d6             	mov    %rdx,%rsi
    6000090b:	48 89 c7             	mov    %rax,%rdi
    6000090e:	e8 a2 fb ff ff       	callq  600004b5 <getFlagsOfINode(unsigned long, unsigned long)>
    60000913:	25 00 80 00 00       	and    $0x8000,%eax
    60000918:	48 85 c0             	test   %rax,%rax
    6000091b:	74 07                	je     60000924 <isFile(unsigned long)+0x3a>
    6000091d:	b8 01 00 00 00       	mov    $0x1,%eax
    60000922:	eb 05                	jmp    60000929 <isFile(unsigned long)+0x3f>
    60000924:	b8 00 00 00 00       	mov    $0x0,%eax
    60000929:	c9                   	leaveq 
    6000092a:	c3                   	retq   

000000006000092b <main>:
    6000092b:	55                   	push   %rbp
    6000092c:	48 89 e5             	mov    %rsp,%rbp
    6000092f:	48 83 ec 60          	sub    $0x60,%rsp
    60000933:	89 7d ac             	mov    %edi,-0x54(%rbp)
    60000936:	48 89 75 a0          	mov    %rsi,-0x60(%rbp)
    6000093a:	48 c7 45 d0 00 20 00 	movq   $0x60002000,-0x30(%rbp)
    60000941:	60 
    60000942:	83 7d ac 01          	cmpl   $0x1,-0x54(%rbp)
    60000946:	7f 3f                	jg     60000987 <main+0x5c>
    60000948:	bf 11 20 00 60       	mov    $0x60002011,%edi
    6000094d:	e8 66 ff ff ff       	callq  600008b8 <print(char const*)>
    60000952:	83 7d ac 00          	cmpl   $0x0,-0x54(%rbp)
    60000956:	7f 0c                	jg     60000964 <main+0x39>
    60000958:	bf 19 20 00 60       	mov    $0x60002019,%edi
    6000095d:	e8 56 ff ff ff       	callq  600008b8 <print(char const*)>
    60000962:	eb 0f                	jmp    60000973 <main+0x48>
    60000964:	48 8b 45 a0          	mov    -0x60(%rbp),%rax
    60000968:	48 8b 00             	mov    (%rax),%rax
    6000096b:	48 89 c7             	mov    %rax,%rdi
    6000096e:	e8 45 ff ff ff       	callq  600008b8 <print(char const*)>
    60000973:	bf 21 20 00 60       	mov    $0x60002021,%edi
    60000978:	e8 3b ff ff ff       	callq  600008b8 <print(char const*)>
    6000097d:	b8 01 00 00 00       	mov    $0x1,%eax
    60000982:	e9 6a 03 00 00       	jmpq   60000cf1 <main+0x3c6>
    60000987:	48 8b 45 a0          	mov    -0x60(%rbp),%rax
    6000098b:	48 8b 40 08          	mov    0x8(%rax),%rax
    6000098f:	48 89 45 c8          	mov    %rax,-0x38(%rbp)
    60000993:	e8 12 fa ff ff       	callq  600003aa <getSystemDevice()>
    60000998:	48 89 05 81 06 00 00 	mov    %rax,0x681(%rip)        # 60001020 <systemDevice>
    6000099f:	48 8b 05 7a 06 00 00 	mov    0x67a(%rip),%rax        # 60001020 <systemDevice>
    600009a6:	48 8b 55 c8          	mov    -0x38(%rbp),%rdx
    600009aa:	48 89 d6             	mov    %rdx,%rsi
    600009ad:	48 89 c7             	mov    %rax,%rdi
    600009b0:	e8 72 fa ff ff       	callq  60000427 <getINodeOfPath(unsigned long, char const*)>
    600009b5:	48 89 45 c0          	mov    %rax,-0x40(%rbp)
    600009b9:	48 83 7d c0 00       	cmpq   $0x0,-0x40(%rbp)
    600009be:	75 14                	jne    600009d4 <main+0xa9>
    600009c0:	bf 2c 20 00 60       	mov    $0x6000202c,%edi
    600009c5:	e8 ee fe ff ff       	callq  600008b8 <print(char const*)>
    600009ca:	b8 01 00 00 00       	mov    $0x1,%eax
    600009cf:	e9 1d 03 00 00       	jmpq   60000cf1 <main+0x3c6>
    600009d4:	48 8b 45 c0          	mov    -0x40(%rbp),%rax
    600009d8:	48 89 c7             	mov    %rax,%rdi
    600009db:	e8 0a ff ff ff       	callq  600008ea <isFile(unsigned long)>
    600009e0:	83 f0 01             	xor    $0x1,%eax
    600009e3:	84 c0                	test   %al,%al
    600009e5:	74 14                	je     600009fb <main+0xd0>
    600009e7:	bf 3c 20 00 60       	mov    $0x6000203c,%edi
    600009ec:	e8 c7 fe ff ff       	callq  600008b8 <print(char const*)>
    600009f1:	b8 01 00 00 00       	mov    $0x1,%eax
    600009f6:	e9 f6 02 00 00       	jmpq   60000cf1 <main+0x3c6>
    600009fb:	48 8b 05 1e 06 00 00 	mov    0x61e(%rip),%rax        # 60001020 <systemDevice>
    60000a02:	48 8b 55 c0          	mov    -0x40(%rbp),%rdx
    60000a06:	48 89 d6             	mov    %rdx,%rsi
    60000a09:	48 89 c7             	mov    %rax,%rdi
    60000a0c:	e8 32 fb ff ff       	callq  60000543 <getFileSize(unsigned long, unsigned long)>
    60000a11:	48 89 45 b8          	mov    %rax,-0x48(%rbp)
    60000a15:	bf 48 20 00 60       	mov    $0x60002048,%edi
    60000a1a:	e8 99 fe ff ff       	callq  600008b8 <print(char const*)>
    60000a1f:	bf 90 20 00 60       	mov    $0x60002090,%edi
    60000a24:	e8 8f fe ff ff       	callq  600008b8 <print(char const*)>
    60000a29:	48 c7 45 f8 00 00 00 	movq   $0x0,-0x8(%rbp)
    60000a30:	00 
    60000a31:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000a35:	48 3b 45 b8          	cmp    -0x48(%rbp),%rax
    60000a39:	0f 83 ad 02 00 00    	jae    60000cec <main+0x3c1>
    60000a3f:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000a43:	48 c1 e8 09          	shr    $0x9,%rax
    60000a47:	48 89 c2             	mov    %rax,%rdx
    60000a4a:	48 8b 05 cf 05 00 00 	mov    0x5cf(%rip),%rax        # 60001020 <systemDevice>
    60000a51:	48 8b 75 c0          	mov    -0x40(%rbp),%rsi
    60000a55:	b9 40 10 00 60       	mov    $0x60001040,%ecx
    60000a5a:	48 89 c7             	mov    %rax,%rdi
    60000a5d:	e8 00 fa ff ff       	callq  60000462 <getSectorOfINode(unsigned long, unsigned long, unsigned long, unsigned char*)>
    60000a62:	83 f0 01             	xor    $0x1,%eax
    60000a65:	84 c0                	test   %al,%al
    60000a67:	74 14                	je     60000a7d <main+0x152>
    60000a69:	bf d7 20 00 60       	mov    $0x600020d7,%edi
    60000a6e:	e8 45 fe ff ff       	callq  600008b8 <print(char const*)>
    60000a73:	b8 01 00 00 00       	mov    $0x1,%eax
    60000a78:	e9 74 02 00 00       	jmpq   60000cf1 <main+0x3c6>
    60000a7d:	48 c7 45 f0 00 00 00 	movq   $0x0,-0x10(%rbp)
    60000a84:	00 
    60000a85:	48 81 7d f0 ff 01 00 	cmpq   $0x1ff,-0x10(%rbp)
    60000a8c:	00 
    60000a8d:	0f 87 a9 01 00 00    	ja     60000c3c <main+0x311>
    60000a93:	48 8b 55 f8          	mov    -0x8(%rbp),%rdx
    60000a97:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000a9b:	48 01 d0             	add    %rdx,%rax
    60000a9e:	48 39 45 b8          	cmp    %rax,-0x48(%rbp)
    60000aa2:	0f 86 94 01 00 00    	jbe    60000c3c <main+0x311>
    60000aa8:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000aac:	83 e0 0f             	and    $0xf,%eax
    60000aaf:	48 85 c0             	test   %rax,%rax
    60000ab2:	0f 85 9a 00 00 00    	jne    60000b52 <main+0x227>
    60000ab8:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
    60000abc:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000ac0:	48 01 d0             	add    %rdx,%rax
    60000ac3:	48 c1 e8 04          	shr    $0x4,%rax
    60000ac7:	66 89 45 b6          	mov    %ax,-0x4a(%rbp)
    60000acb:	0f b7 45 b6          	movzwl -0x4a(%rbp),%eax
    60000acf:	c1 f8 0c             	sar    $0xc,%eax
    60000ad2:	48 98                	cltq   
    60000ad4:	83 e0 0f             	and    $0xf,%eax
    60000ad7:	48 89 c2             	mov    %rax,%rdx
    60000ada:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
    60000ade:	48 01 d0             	add    %rdx,%rax
    60000ae1:	0f b6 00             	movzbl (%rax),%eax
    60000ae4:	0f b6 c0             	movzbl %al,%eax
    60000ae7:	89 c7                	mov    %eax,%edi
    60000ae9:	e8 e5 f8 ff ff       	callq  600003d3 <write(unsigned char)>
    60000aee:	0f b7 45 b6          	movzwl -0x4a(%rbp),%eax
    60000af2:	c1 f8 08             	sar    $0x8,%eax
    60000af5:	48 98                	cltq   
    60000af7:	83 e0 0f             	and    $0xf,%eax
    60000afa:	48 89 c2             	mov    %rax,%rdx
    60000afd:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
    60000b01:	48 01 d0             	add    %rdx,%rax
    60000b04:	0f b6 00             	movzbl (%rax),%eax
    60000b07:	0f b6 c0             	movzbl %al,%eax
    60000b0a:	89 c7                	mov    %eax,%edi
    60000b0c:	e8 c2 f8 ff ff       	callq  600003d3 <write(unsigned char)>
    60000b11:	0f b7 45 b6          	movzwl -0x4a(%rbp),%eax
    60000b15:	c1 f8 04             	sar    $0x4,%eax
    60000b18:	48 98                	cltq   
    60000b1a:	83 e0 0f             	and    $0xf,%eax
    60000b1d:	48 89 c2             	mov    %rax,%rdx
    60000b20:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
    60000b24:	48 01 d0             	add    %rdx,%rax
    60000b27:	0f b6 00             	movzbl (%rax),%eax
    60000b2a:	0f b6 c0             	movzbl %al,%eax
    60000b2d:	89 c7                	mov    %eax,%edi
    60000b2f:	e8 9f f8 ff ff       	callq  600003d3 <write(unsigned char)>
    60000b34:	0f b7 45 b6          	movzwl -0x4a(%rbp),%eax
    60000b38:	83 e0 0f             	and    $0xf,%eax
    60000b3b:	48 89 c2             	mov    %rax,%rdx
    60000b3e:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
    60000b42:	48 01 d0             	add    %rdx,%rax
    60000b45:	0f b6 00             	movzbl (%rax),%eax
    60000b48:	0f b6 c0             	movzbl %al,%eax
    60000b4b:	89 c7                	mov    %eax,%edi
    60000b4d:	e8 81 f8 ff ff       	callq  600003d3 <write(unsigned char)>
    60000b52:	bf 7c 00 00 00       	mov    $0x7c,%edi
    60000b57:	e8 77 f8 ff ff       	callq  600003d3 <write(unsigned char)>
    60000b5c:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000b60:	48 05 40 10 00 60    	add    $0x60001040,%rax
    60000b66:	0f b6 00             	movzbl (%rax),%eax
    60000b69:	0f b6 c0             	movzbl %al,%eax
    60000b6c:	c1 f8 04             	sar    $0x4,%eax
    60000b6f:	48 98                	cltq   
    60000b71:	83 e0 0f             	and    $0xf,%eax
    60000b74:	48 89 c2             	mov    %rax,%rdx
    60000b77:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
    60000b7b:	48 01 d0             	add    %rdx,%rax
    60000b7e:	0f b6 00             	movzbl (%rax),%eax
    60000b81:	0f b6 c0             	movzbl %al,%eax
    60000b84:	89 c7                	mov    %eax,%edi
    60000b86:	e8 48 f8 ff ff       	callq  600003d3 <write(unsigned char)>
    60000b8b:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000b8f:	48 05 40 10 00 60    	add    $0x60001040,%rax
    60000b95:	0f b6 00             	movzbl (%rax),%eax
    60000b98:	0f b6 c0             	movzbl %al,%eax
    60000b9b:	83 e0 0f             	and    $0xf,%eax
    60000b9e:	48 89 c2             	mov    %rax,%rdx
    60000ba1:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
    60000ba5:	48 01 d0             	add    %rdx,%rax
    60000ba8:	0f b6 00             	movzbl (%rax),%eax
    60000bab:	0f b6 c0             	movzbl %al,%eax
    60000bae:	89 c7                	mov    %eax,%edi
    60000bb0:	e8 1e f8 ff ff       	callq  600003d3 <write(unsigned char)>
    60000bb5:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000bb9:	83 e0 0f             	and    $0xf,%eax
    60000bbc:	48 83 f8 0f          	cmp    $0xf,%rax
    60000bc0:	75 70                	jne    60000c32 <main+0x307>
    60000bc2:	bf 7c 00 00 00       	mov    $0x7c,%edi
    60000bc7:	e8 07 f8 ff ff       	callq  600003d3 <write(unsigned char)>
    60000bcc:	48 c7 45 e8 00 00 00 	movq   $0x0,-0x18(%rbp)
    60000bd3:	00 
    60000bd4:	48 83 7d e8 0f       	cmpq   $0xf,-0x18(%rbp)
    60000bd9:	77 4d                	ja     60000c28 <main+0x2fd>
    60000bdb:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
    60000bdf:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000be3:	48 01 d0             	add    %rdx,%rax
    60000be6:	48 83 e0 f0          	and    $0xfffffffffffffff0,%rax
    60000bea:	48 89 c2             	mov    %rax,%rdx
    60000bed:	48 8b 45 e8          	mov    -0x18(%rbp),%rax
    60000bf1:	48 01 d0             	add    %rdx,%rax
    60000bf4:	0f b6 80 40 10 00 60 	movzbl 0x60001040(%rax),%eax
    60000bfb:	88 45 b5             	mov    %al,-0x4b(%rbp)
    60000bfe:	80 7d b5 1f          	cmpb   $0x1f,-0x4b(%rbp)
    60000c02:	76 13                	jbe    60000c17 <main+0x2ec>
    60000c04:	80 7d b5 7e          	cmpb   $0x7e,-0x4b(%rbp)
    60000c08:	77 0d                	ja     60000c17 <main+0x2ec>
    60000c0a:	0f b6 45 b5          	movzbl -0x4b(%rbp),%eax
    60000c0e:	89 c7                	mov    %eax,%edi
    60000c10:	e8 be f7 ff ff       	callq  600003d3 <write(unsigned char)>
    60000c15:	eb 0a                	jmp    60000c21 <main+0x2f6>
    60000c17:	bf 2e 00 00 00       	mov    $0x2e,%edi
    60000c1c:	e8 b2 f7 ff ff       	callq  600003d3 <write(unsigned char)>
    60000c21:	48 83 45 e8 01       	addq   $0x1,-0x18(%rbp)
    60000c26:	eb ac                	jmp    60000bd4 <main+0x2a9>
    60000c28:	bf 0a 00 00 00       	mov    $0xa,%edi
    60000c2d:	e8 a1 f7 ff ff       	callq  600003d3 <write(unsigned char)>
    60000c32:	48 83 45 f0 01       	addq   $0x1,-0x10(%rbp)
    60000c37:	e9 49 fe ff ff       	jmpq   60000a85 <main+0x15a>
    60000c3c:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000c40:	83 e0 0f             	and    $0xf,%eax
    60000c43:	48 85 c0             	test   %rax,%rax
    60000c46:	0f 84 93 00 00 00    	je     60000cdf <main+0x3b4>
    60000c4c:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000c50:	83 e0 0f             	and    $0xf,%eax
    60000c53:	48 89 45 e0          	mov    %rax,-0x20(%rbp)
    60000c57:	48 83 7d e0 0f       	cmpq   $0xf,-0x20(%rbp)
    60000c5c:	77 11                	ja     60000c6f <main+0x344>
    60000c5e:	bf f5 20 00 60       	mov    $0x600020f5,%edi
    60000c63:	e8 50 fc ff ff       	callq  600008b8 <print(char const*)>
    60000c68:	48 83 45 e0 01       	addq   $0x1,-0x20(%rbp)
    60000c6d:	eb e8                	jmp    60000c57 <main+0x32c>
    60000c6f:	bf 7c 00 00 00       	mov    $0x7c,%edi
    60000c74:	e8 5a f7 ff ff       	callq  600003d3 <write(unsigned char)>
    60000c79:	48 c7 45 d8 00 00 00 	movq   $0x0,-0x28(%rbp)
    60000c80:	00 
    60000c81:	48 83 7d d8 0f       	cmpq   $0xf,-0x28(%rbp)
    60000c86:	77 4d                	ja     60000cd5 <main+0x3aa>
    60000c88:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
    60000c8c:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
    60000c90:	48 01 d0             	add    %rdx,%rax
    60000c93:	48 83 e0 f0          	and    $0xfffffffffffffff0,%rax
    60000c97:	48 89 c2             	mov    %rax,%rdx
    60000c9a:	48 8b 45 d8          	mov    -0x28(%rbp),%rax
    60000c9e:	48 01 d0             	add    %rdx,%rax
    60000ca1:	0f b6 80 40 10 00 60 	movzbl 0x60001040(%rax),%eax
    60000ca8:	88 45 b4             	mov    %al,-0x4c(%rbp)
    60000cab:	80 7d b4 1f          	cmpb   $0x1f,-0x4c(%rbp)
    60000caf:	76 13                	jbe    60000cc4 <main+0x399>
    60000cb1:	80 7d b4 7e          	cmpb   $0x7e,-0x4c(%rbp)
    60000cb5:	77 0d                	ja     60000cc4 <main+0x399>
    60000cb7:	0f b6 45 b4          	movzbl -0x4c(%rbp),%eax
    60000cbb:	89 c7                	mov    %eax,%edi
    60000cbd:	e8 11 f7 ff ff       	callq  600003d3 <write(unsigned char)>
    60000cc2:	eb 0a                	jmp    60000cce <main+0x3a3>
    60000cc4:	bf 2e 00 00 00       	mov    $0x2e,%edi
    60000cc9:	e8 05 f7 ff ff       	callq  600003d3 <write(unsigned char)>
    60000cce:	48 83 45 d8 01       	addq   $0x1,-0x28(%rbp)
    60000cd3:	eb ac                	jmp    60000c81 <main+0x356>
    60000cd5:	bf 0a 00 00 00       	mov    $0xa,%edi
    60000cda:	e8 f4 f6 ff ff       	callq  600003d3 <write(unsigned char)>
    60000cdf:	48 81 45 f8 00 02 00 	addq   $0x200,-0x8(%rbp)
    60000ce6:	00 
    60000ce7:	e9 45 fd ff ff       	jmpq   60000a31 <main+0x106>
    60000cec:	b8 00 00 00 00       	mov    $0x0,%eax
    60000cf1:	c9                   	leaveq 
    60000cf2:	c3                   	retq   

Disassembly of section .text._Z7syscallmmmm:

0000000060000cf3 <syscall(unsigned long, unsigned long, unsigned long, unsigned long)>:
    60000cf3:	55                   	push   %rbp
    60000cf4:	48 89 e5             	mov    %rsp,%rbp
    60000cf7:	53                   	push   %rbx
    60000cf8:	48 83 ec 20          	sub    $0x20,%rsp
    60000cfc:	48 89 7d f0          	mov    %rdi,-0x10(%rbp)
    60000d00:	48 89 75 e8          	mov    %rsi,-0x18(%rbp)
    60000d04:	48 89 55 e0          	mov    %rdx,-0x20(%rbp)
    60000d08:	48 89 4d d8          	mov    %rcx,-0x28(%rbp)
    60000d0c:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
    60000d10:	48 8b 5d e8          	mov    -0x18(%rbp),%rbx
    60000d14:	48 8b 4d e0          	mov    -0x20(%rbp),%rcx
    60000d18:	48 8b 55 d8          	mov    -0x28(%rbp),%rdx
    60000d1c:	cd 80                	int    $0x80
    60000d1e:	90                   	nop
    60000d1f:	48 83 c4 20          	add    $0x20,%rsp
    60000d23:	5b                   	pop    %rbx
    60000d24:	5d                   	pop    %rbp
    60000d25:	c3                   	retq   
