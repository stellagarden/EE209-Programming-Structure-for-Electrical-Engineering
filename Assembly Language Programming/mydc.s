### --------------------------------------------------------------------
### mydc.s
### 20190533
### Lee Hyemin
### Assignment 4
### Desk Calculator (dc)
### --------------------------------------------------------------------

	.equ   ARRAYSIZE, 20
	.equ   EOF, -1
	.equ   RAND_MAX, 1024
	.equ   IBASE, 12
	.equ   IEXP, 8
	.equ   IPOWER, -4
	.equ   IINDEX, -8
	
.section ".rodata"

scanfFormat:
	.asciz "%s"
printfFormat:
	.asciz "%d\n"
stackEmptyMsg:
	.asciz "dc: stack empty\n"
undefinedMsg:
  .asciz "dc: Undefined command\n"
### --------------------------------------------------------------------

        .section ".data"

### --------------------------------------------------------------------

        .section ".bss"
buffer:
        .skip  ARRAYSIZE
index:
		.skip 4

### --------------------------------------------------------------------

	.section ".text"	

	## -------------------------------------------------------------
	## int main(void)
	## Runs desk calculator program.  Returns 0.
	## -------------------------------------------------------------
	## int powerfunc(int iExp, int iBase)
	## Calculate iBase ^ iExp. Returns result.
	## -------------------------------------------------------------

	.globl  main
	.type   main,@function
	.globl powerfunc
	.type powerfunc,@function

main:
	## dc number stack initialized. %esp = %ebp
	pushl   %ebp
	movl    %esp, %ebp

	## srand(time(NULL))
	pushl $0
	call time
	addl $4, %esp
	pushl %eax
	call srand
	addl $4, %esp

## /* whileloop */
input:
	## scanf("%s", buffer)
	pushl	$buffer
	pushl	$scanfFormat
	call    scanf
	addl    $8, %esp

	## if user input is EOF, quit
	cmp	$EOF, %eax
	je	quit
	
	## if (!isdigit(buffer[0]) except '_') goto command_classifier
	movzx buffer, %eax
	cmp $'_', %eax
	je neg_num
	cmp $'0', %eax
	jl command_classifier
	cmp $'9', %eax
	jg command_classifier
	
	jmp num

neg_num:
	## if (buffer[0] == '_') substitude '_' to '-' /* so that the input number can be actually negative */
	movb $'-', buffer

num:
	## /* the intput is a digit */
	## 	int no = atoi(buffer)
	## 	stack.push(no)
	movl $0, %eax
	pushl $buffer
	call atoi
	addl $4, %esp
	pushl %eax
	jmp input

command_classifier:
	## if (buffer[0] == 'p') goto print
	cmp $'p', %eax
	je print

	## else if (buffer[0] == 'q') goto quit
	cmp $'q', %eax
	je quit

	## else if (buffer[0] == 'f') goto fprint
	cmp $'f', %eax
	je fprint

	## else if (buffer[0] == 'c') goto clear
	cmp $'c', %eax
	je clear

	## else if (buffer[0] == 'd') goto dup
	cmp $'d', %eax
	je dup

	## else if (buffer[0] == 'r') goto rev
	cmp $'r', %eax
	je rev

	## else if (buffer[0] == 'x') goto dc_rand
	cmp $'x', %eax
	je dc_rand

	## else if (buffer[0] == 'y') goto prime
	cmp $'y', %eax
	je prime

	## else if (buffer[0] == '+') goto dc_add
	cmp $'+', %eax
	je dc_add

	## else if (buffer[0] == '-') goto dc_sub
	cmp $'-', %eax
	je dc_sub

	## else if (buffer[0] == '*') goto dc_mul
	cmp $'*', %eax
	je dc_mul

	## else if (buffer[0] == '/') goto dc_div
	cmp $'/', %eax
	je dc_div

	## else if (buffer[0] == '%') goto dc_remain
	cmp $'%', %eax
	je dc_remain

	## else if (buffer[0] == '^') goto dc_pow
	cmp $'^', %eax
	je dc_pow

	## else printf("dc: Undefined command\n")
	pushl $undefinedMsg
	call printf
	addl $4, %esp
	jmp input

print:
	## if (stack.peek() == NULL) goto print_stack_empty
	cmp %esp, %ebp
	je print_stack_empty

	## else printf("%d\n", (int)stack.top()); /* value is already pushed in the stack */
	pushl (%esp)
	pushl $printfFormat
	call printf
	addl $8, %esp
	jmp input

print_stack_empty:
	## printf("dc: stack empty\n");
	## continue;
	pushl $stackEmptyMsg
	call printf
	addl $4, %esp
	jmp input

print_double_stack_empty:
	## stack.push(b);
	## printf("dc: stack empty\n");
	## continue;
	pushl %ebx
	pushl $stackEmptyMsg
	call printf
	addl $4, %esp
	jmp input

dc_add:
	## if (stack.peek() == NULL) goto print_stack_empty
	cmp %esp, %ebp
	je print_stack_empty

	## int b = (int)stack.pop();
	popl %ebx

	## if (stack.peek() == NULL) goto print_double_stack_empty
	cmp %esp, %ebp
	je print_double_stack_empty

	## int a = (int)stack.pop();
	popl %eax

	## int res = a + b;
	## stack.push(res);
	addl %eax, %ebx
	pushl %ebx
	jmp input


dc_sub:
	## if (stack.peek() == NULL) goto print_stack_empty
	cmp %esp, %ebp
	je print_stack_empty

	## int b = (int)stack.pop();
	popl %ebx

	## if (stack.peek() == NULL) goto print_double_stack_empty
	cmp %esp, %ebp
	je print_double_stack_empty

	## int a = (int)stack.pop();
	popl %eax

	## int res = a - b;
	## stack.push(res);
	subl %ebx, %eax
	pushl %eax
	jmp input

dc_mul:
	## if (stack.peek() == NULL) goto print_stack_empty
	cmp %esp, %ebp
	je print_stack_empty

	## int b = (int)stack.pop();
	popl %ebx

	## if (stack.peek() == NULL) goto print_double_stack_empty
	cmp %esp, %ebp
	je print_double_stack_empty

	## int a = (int)stack.pop();
	popl %eax

	## int res = a * b;
	## stack.push(res);
	imull %ebx, %eax
	pushl %eax
	jmp input

dc_div:
	## if (stack.peek() == NULL) goto print_stack_empty
	cmp %esp, %ebp
	je print_stack_empty

	## int b = (int)stack.pop();
	popl %ebx

	## if (stack.peek() == NULL) goto print_double_stack_empty
	cmp %esp, %ebp
	je print_double_stack_empty

	## int a = (int)stack.pop();
	popl %eax

	## int res = a / b;
	## stack.push(res);
	cdq
	idiv %ebx
	pushl %eax
	jmp input

dc_remain:
	## if (stack.peek() == NULL) goto print_stack_empty
	cmp %esp, %ebp
	je print_stack_empty

	## int b = (int)stack.pop();
	popl %ebx

	## if (stack.peek() == NULL) goto print_double_stack_empty
	cmp %esp, %ebp
	je print_double_stack_empty

	## int a = (int)stack.pop();
	popl %eax

	## int res = a % b;
	## stack.push(res);
	cdq
	idiv %ebx
	pushl %edx
	jmp input

dc_pow:
	## if (stack.peek() == NULL) goto print_stack_empty
	cmp %esp, %ebp
	je print_stack_empty

	## int b = (int)stack.pop();
	popl %ebx

	## if (stack.peek() == NULL) goto print_double_stack_empty
	cmp %esp, %ebp
	je print_double_stack_empty
	pushl %ebx

	## int res = a ^ b;
	## stack.push(res);
	call powerfunc
	add $8, %esp
	pushl %eax
	jmp input

quit:	
	## return 0
	movl    $0, %eax
	movl    %ebp, %esp
	popl    %ebp
	ret

powerfunc:
	pushl %ebp
	movl %esp, %ebp

	## for(int iPower = 1, iIndex; iIndex <= iExp; iIndex++){
	##		iPower *= iBase;
	## }
	## return iPower
	pushl   $1
	subl    $4, %esp
	movl    $1, IINDEX(%ebp)
	power_for_loop:
		movl    IINDEX(%ebp), %eax
		cmpl    IEXP(%ebp), %eax
		jg      power_for_loop_end

		movl    IPOWER(%ebp), %eax
		imull   IBASE(%ebp), %eax
		movl    %eax, IPOWER(%ebp)

		incl    IINDEX(%ebp)
		jmp     power_for_loop
	power_for_loop_end:
		movl    IPOWER(%ebp), %eax
		movl    %ebp, %esp
		popl    %ebp
		ret	

fprint:
	## index = 0
	## for(int *now = &(stack.peek()); now!=NULL;){
	## 		printf("%d\n", *now);
	##		index += 4;
	##		*now = &(stack.peek()) + index;
	## }
	movl $0, index
	movl %esp, %eax
	fprint_for_loop:
		cmp %eax, %ebp
		je input

		pushl (%eax)
		pushl $printfFormat
		call printf
		addl $8, %esp

		addl $4, index
		movl %esp, %eax
		addl index, %eax
		jmp fprint_for_loop

clear:
	## while(stack.peek()!=NULL) stack.pop()
	cmp %esp, %ebp
	je input
	popl %eax
	jmp clear

dup:
	## if (stack.peek() == NULL) goto print_stack_empty
	cmp %esp, %ebp
	je print_stack_empty

	## int a = stack.pop());
	popl %eax

	## stack.push(a);
	## stack.push(a);
	pushl %eax
	pushl %eax
	jmp input

rev:
	## if (stack.peek() == NULL) goto print_stack_empty
	cmp %esp, %ebp
	je print_stack_empty

	## int b = (int)stack.pop();
	popl %ebx

	## if (stack.peek() == NULL) goto print_double_stack_empty
	cmp %esp, %ebp
	je print_double_stack_empty

	## int a = (int)stack.pop();
	popl %eax

	## stack.push(b);
	## stack.push(a);
	pushl %ebx
	pushl %eax
	jmp input

dc_rand:
  ## int random_number = rand() % RAND_MAX;
  call rand

  movl $RAND_MAX, %ebx
  cdq
  idivl %ebx
  
  ## stack.push(random_number)
  pushl %edx
  jmp input

prime:
  ## int max_prime = 0;
  movl $0, %ebx
  ## 
  ## for(int now=stack.peek();now>1;now--){
  ##	for(int i=2; i<now; i++){
  ##  		if(now%i==0) goto prime_for_sub;
  ##	}
  ##	stack.push(now);
  ##	goto input;
  ## }
  ## goto input;

  ## for(int now=stack.peek();now>1;now--){
  movl (%esp), %edi
  prime_for_loop:
	cmp $1, %edi
	jle prime_for_end

	## for(int i=2; i<now; i++){
	movl $2, %ecx
	prime_inner_for_loop:
		cmp %ecx, %edi
		jle prime_inner_for_end

		## if(now%i==0) goto prime_for_sub
		cdq
		movl %edi, %eax
		idiv %ecx
		cmp $0, %edx
		je prime_for_sub
		
		addl $1, %ecx
		jmp prime_inner_for_loop
  	prime_inner_for_end:
	    ## stack.push(now);
		## goto input;
	  	pushl %edi
		jmp input
		
  prime_for_sub:
	subl $1, %edi
	jmp prime_for_loop

  prime_for_end:
	jmp input
