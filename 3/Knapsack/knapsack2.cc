.global knapsack

.equ ws, 4

knapsack:
	#prolog
	push %ebp #push ebp to the stack; esp points to top of stack; ebp is base pointer
	movl %esp, %ebp #move esp into ebp; ebp point to same thing as esp is pointing to
	subl $1*ws, %esp #subtract $1*ws(or 4) from esp to make room (4 bytes) for variable

	#initialize variables; base values
	.equ weights, 2*ws #(%ebp)
	.equ values, 3*ws #(%ebp)
	.equ num_items, 4*ws #(%ebp)
	.equ capacity, 5*ws #(%ebp)
	.equ cur_value, 6*ws #(%ebp)

	.equ best_value, -1*ws #(%ebp)

	#save live registers
	push %ebx
	push %edi
	push %esi

	movl capacity(%ebp), %eax 	# eax is capacity
	movl weights(%ebp), %ecx 	  # ecx is weights
	movl cur_value(%ebp), %edx 	# edx is cur_value
	movl num_items(%ebp), %esi  # esi is num_items
	movl values(%ebp), %ebx    	# ebx is values

	# edi is i
	movl %edx, best_value(%ebp) #best_value = cur_value;

	movl $0, %edi  # i = 0;
	start_for_loop:
		#reinitialize in for loop
		movl capacity(%ebp), %eax
		movl weights(%ebp), %ecx
		movl cur_value(%ebp), %edx

		# i < num_items
		#i - num_items < 0
		# Negation: i - num_items >= 0
		cmpl %esi, %edi #compare num_items to i
		jae end_for_loop #jump if above or equal to zero (or negation)

		# capacity - weights[i] >= 0
		# Negation: capacity - weights[i] < 0
		cmp (%ecx, %edi, ws), %eax #if(capacity - weights[i] >= 0 )
		jb end_recursion #jump if below zero (or negation)

	recursion:
		# setting up
		movl cur_value(%ebp), %eax         #move cur_value into eax
		addl (%ebx, %edi, ws), %eax        #cur_value + values[i]
		push %eax                          #push eax to the stack; esp points to top of stack

		movl capacity(%ebp), %eax          #move capacity into eax
		subl (%ecx, %edi, ws), %eax        #capacity - weights[i]
		push %eax                          #push eax to the stack; esp points to top of stack

		movl num_items(%ebp), %eax				 #move num_items into eax
		subl %edi, %eax										 # num_items - i
		subl $1, %eax 		                 # num_items - i - 1
		push %eax 												 #push eax to the stack; esp points to top of stack

		movl values(%ebp), %eax            #move values into eax
		leal ws(%eax, %edi, ws), %eax      #values + i + 1; ws+values+(i*ws) provides address
		push %eax 												 #push eax to the stack; esp points to top of stack

		movl weights(%ebp), %eax					 #move weights into eax
		leal ws(%eax, %edi, ws), %eax      #weights + i + 1; ws+weights+(i*ws) provides address
		push %eax 												 #push eax to the stack; esp points to top of stack

		call knapsack											 #take the new values and call knapsack with them //recurse back
		addl $5*ws, %esp									 #add $5*ws(or 20) to esp; add to top of stack

		#max function with cmpl and swap
		cmpl %eax, best_value(%ebp) #compare eax to best_value
		jb swap #jump if below zero; a > b
		jae end_recursion #jump if above or equal to zero; negation of a > b

	swap:
		movl %eax, best_value(%ebp) #move eax into best_value, since eax is bigger
		jmp end_recursion


	end_recursion:
		incl %edi #increment i
		jmp start_for_loop

	end_for_loop:

	#epilogue
	pop %esi #pop num_items
	pop %edi #pop i
	pop %ebx #pop values
	movl best_value(%ebp), %eax #move best_value to eax
	movl %ebp, %esp #move ebp into esp; esp point to same thing as ebp is pointing to
	pop %ebp #pop ebp
	ret
