
/*
	int rec_strcmp(char* str1, char* str2){	
		if(*str1 != *str2){ //first letters are different
			return *str1 - *str2;
		}
		else if(*str1 == '\0'){ //both strings are empty
			return 0;
		}
		else{
			return rec_strcmp(str1 + 1, str2 +1);
		}	
	}
*/

.global rec_strcmp

.equ ws, 4
.text
rec_strcmp:
	prologue:
		push %ebp
		movl %esp, %ebp
		#make space for locals
		#save regs
		.equ str1, (2*ws) #(%ebp)
		.equ str2, (3*ws) #(%ebp)
	
	if: #if(*str1 != *str2)
		movl str1(%ebp), %eax #eax = str1
		movl str2(%ebp), %ecx #ecx = str2
		movb (%eax), %al #al = *str1
	
		cmpb %al, (%ecx) 
		jz else_if
		#return *str1 - *str2;
		subb (%ecx), %al
		movsx %al, %eax #double check instruction
		jmp epilogue
	
	else_if: #*str1 == '\0'
		cmpb $0, %al
		jnz else
		movl $0, %eax
		jmp epilogue
	else:
		#return rec_strcmp(str1 + 1, str2 +1)
		leal 1(%ecx), %ecx #ecx = str2 + 1
		push %ecx #place str2 + 1 on the stack
		movl str1(%ebp), %eax 
		leal 1(%eax), %eax
		push %eax #place str1 + 1 on stack
		call rec_strcmp
		addl $2*ws, %esp #clear args

	epilogue:
		movl %ebp, %esp #remove locals
		pop %ebp
		ret
	
	
	
	
	
