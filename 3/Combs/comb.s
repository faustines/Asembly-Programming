.global get_combs

.equ ws, 4

get_combs:
	#prologue
	push %ebp              #push ebp to the stack; esp points to top of stack; ebp is base pointer
	movl %esp, %ebp        #move esp into ebp; ebp point to same thing as esp is pointing to
	subl $4*ws, %esp 			 #esp #subtract $4*ws(or 16) from esp to make room (16 bytes) for variables

	#initialize variables; base values
	.equ items,(2*ws) 			#pointer
	.equ k,(3*ws) 					#(%ebp)
	.equ len,(4*ws) 				#(%ebp)

	# locals
	.equ count,(-1*ws) 			#(%ebp)
	.equ cur_comb,(-2*ws) 	#(%ebp)
	.equ nc,(-3*ws) 				#(%ebp)
	.equ combs,(-4*ws) 			#(%ebp)

	movl $0, count(%ebp)		# int count = 0;

	# int nc = num_combs(len, k);
	push k(%ebp)        #push k to the stack; esp points to top of stack
	push len(%ebp)      #push len to the stack; esp points to top of stack
	call num_combs      #call num_combs from main.c with k and len
	addl $2*ws, %esp    # clear stack
	movl %eax, nc(%ebp) # save result into nc

	# int* cur_comb = (int*)malloc(k*sizeof(int));
	movl k(%ebp), %edi 					#edi = k
	shll $2, %edi								#edi = k*4
	push %edi
	call malloc
	addl $1*ws, %esp						#clear argument
	movl %eax, cur_comb(%ebp)		#save result into cur_comb

	# int** combs = (int**)malloc(nc*sizeof(int*));
	movl nc(%ebp), %eax 				#eax = num_combs
	shll $2, %eax 								#eax = num_combs*4
	push %eax
	call malloc
	addl $1*ws, %esp 						#clear argument
	movl %eax, combs(%ebp)			#save result into combs

	movl $0, %edi # i = 0

	#for (i = 0; i < nc; i++)
	movl combs(%ebp), %ebx
	start_get_combs_for_loop:
		movl k(%ebp), %eax					#eax = k
		shll $2, %eax								#eax = k(%ebp)*4
		push %eax										#push k to the stack; esp points to top of stack
		call malloc
		movl k(%ebp), %esi					#esi = k
		movl %eax, (%ebx, %edi, ws) #combs[i] = (int*)malloc(k * sizeof(int));
		incl %edi										#i++
		cmp nc(%ebp), %edi					#compare nc to edi
	jl start_get_combs_for_loop		#jump if less than; if i < nc

	#push combs_helper arguments: items, combs, cur_comb, 0, len, 0, k, count to call combs_helper
	push items(%ebp)
	movl combs(%ebp), %eax
	push %eax
	push cur_comb(%ebp)
	leal count(%ebp) , %eax
	push %eax
	push k(%ebp)
	push len(%ebp)
	push $0
	push $0
	call combs_helper
	addl $8*ws, %esp  						#clear arguments

	#epilogue
	movl combs(%ebp), %eax 				#return saved value
	movl %ebp, %esp								#move ebp into esp; esp point to same thing as ebp is pointing to
	pop %ebp											#pop ebp
	ret														#return eax aka combs

combs_helper:
	#prologue
	push %ebp
	movl %esp, %ebp
	subl $1*ws, %esp

	#arguments
	.equ start,(2*ws) 	#(%ebp)
	.equ n,(4*ws) 			#(%ebp)
	.equ k,(3*ws) 		#(%ebp)
	.equ maxk,(5*ws) 			#(%ebp)
	.equ count,(6*ws) 	#(%ebp)
	.equ cur_comb,(7*ws)#(%ebp)
	.equ combs,(8*ws) 	#(%ebp)
	.equ items,(9*ws) 	#(%ebp)
	.equ i,(-1*ws) 			#(%ebp)

	#if (k == maxk)
	movl maxk(%ebp), %edi
	cmpl k(%ebp), %edi
	jnz before_second_for_loop #jump if not zero; negation

	movl $0, i(%ebp) # i = 0
	start_first_for_loop:
		#for (i = 0; i < maxk; i++)
		movl i(%ebp), %edi
		cmpl maxk(%ebp), %edi
		jge end_first_for_loop

		#combs[*count][i] = cur_comb[i]
		#cur_comb[i]
		movl cur_comb(%ebp), %ecx
		movl (%ecx,%edi,ws), %ecx #ecx = cur_comb[i]

		#combs[*count]
		movl count(%ebp), %esi #esi = &count
		movl (%esi), %esi #dereference position @ count
		movl combs(%ebp), %eax # eax = **combs
		movl (%eax, %esi, ws), %eax #combs[*count]
		movl %ecx, (%eax, %edi, ws)  #combs[count][i] = cur_comb[i]

		incl i(%ebp) #i++
		jmp start_first_for_loop
	end_first_for_loop:

	#(*count)++;
	movl count(%ebp), %esi 	 #esi = &count
	movl (%esi), %esi	 	 		 #dereference position @ count
	incl %esi 	  		 			 #count++
	movl count(%ebp), %eax
	movl %esi, (%eax)
	jmp end_combs_helper

	#for (i = start; i < n; i++)
	before_second_for_loop:
		movl start(%ebp), %edi
		movl %edi, i(%ebp) #i = start;
	start_second_for_loop:
		movl i(%ebp), %edi
		cmpl n(%ebp), %edi
		jge end_second_for_loop

		#cur_comb[k] = items[i];
		movl items(%ebp), %ebx
		movl (%ebx, %edi,ws), %ecx #ecx = items[i]
		movl cur_comb(%ebp), %eax
		movl k(%ebp), %esi
		movl %ecx, (%eax,%esi,ws)

		#count = combs_helper(i + 1, k + 1, n, maxk, count, cur_comb, combs, items);
		#*items
		push items(%ebp)
		movl combs(%ebp), %eax
		push %eax
		push cur_comb(%ebp)
		push count(%ebp)
		push maxk(%ebp)
		push n(%ebp)
		movl k(%ebp), %eax
		incl %eax
		push %eax
		movl i(%ebp), %edi
		incl %edi
		push %edi
		call combs_helper
		addl $8*ws, %esp #clear arguments

		incl i(%ebp) #i++
		jmp start_second_for_loop
	end_second_for_loop:

	end_combs_helper:
		movl combs(%ebp), %eax 				#return saved value
		movl %ebp, %esp								#move ebp into esp; esp point to same thing as ebp is pointing to
		pop %ebp											#pop ebp
		ret														#return eax aka combs
