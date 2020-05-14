/*
Shreya Venkatesan
Faustine Yiu
ECS 50 FQ 2018
*/

#local variables: i, j, k, dotProduct, resultMatrix (5 locals)

.global matMult
.equ ws, 4

.text

matMult:

	prologue:
		push %ebp
		movl %esp, %ebp
		subl $5*ws, %esp #make space for 5 locals
		#save registers

		#STACK after prologue:
		#num_cols_b
		#num_rows_b
		#b
		#num_cols_a
		#num_rows_a
		#a
		#ret address
		#ebp: old ebp
		#i
		#j
		#k
		#resultMatrix
		#esp: dotProduct

		#ARGS
		.equ a, (2*ws) 							#%ebp
		.equ num_rows_a, (3*ws) 		        #%ebp
		.equ num_cols_a, (4*ws) 		        #%ebp
		.equ b, (5*ws) 					   		#%ebp
		.equ num_rows_b, (6*ws) 		        #%ebp
		.equ num_cols_b, (7*ws) 		        #%ebp

		#LOCALS
		.equ i, (-1*ws) 						#%ebp
		.equ j, (-2*ws) 						#%ebp
		.equ k, (-3*ws) 						#%ebp
		.equ resultMatrix, (-4*ws) 	            #%ebp
		.equ dotProduct, (-5*ws) 		        #%ebp

		push %ebx
		push %edi

		#int** resultMatrix = (int**) malloc(num_rows_a * sizeof(int*));
		movl num_rows_a(%ebp), %eax #eax = num_rows_a
		shll $2, %eax  #eax = num_rows_a * sizeof(int*))
		push %eax #place malloc's arguement onto the stack
		call malloc
		addl $1*ws, %esp #clear malloc's arguement
		#eax = (int**) malloc(num_rows_a * sizeof(int*));
		movl %eax, resultMatrix(%ebp)


		#for(int i = 0; i < num_rows_a; ++i){
		movl $0, %edi #int i = 0
		#i < num_rows_a
		#i - num_rows_a < 0
		#i - num_rows_a >= 0 (NEGATION)

		first_for_start:
				cmpl num_rows_a(%ebp), %edi
				jge first_for_end

				movl %edi, i(%ebp) #store i

				#resultMatrix[i] = (int*) malloc(num_cols_b*sizeof(int*));
				movl num_cols_b(%ebp), %eax #eax = num_cols_b
				shll $2, %eax #eax = num_cols_b * sizeof(int)
				push %eax #set arguement for malloc
				call malloc
				addl $1*ws, %esp #clear arguement for malloc

				#eax = (int*)malloc(num_cols_b * sizeof(int));
				movl resultMatrix(%ebp), %edx #edx = resultMatrix
				movl i(%ebp), %edi
				movl %eax, (%edx, %edi, ws) #(int*)malloc( num_cols_b * sizeof(int));
				movl %edx, %eax #resultMatrix[i] = eax

				#for(int j = 0; j < num_cols_b; ++j){
				movl $0, %ebx #int j = 0
				#j < num_cols_b
				#j - num_cols_b < 0
				#j - num_cols_b >= 0 (NEGATION)

				second_for_start:
					cmpl num_cols_b(%ebp), %ebx
					jge second_for_end
					#int dotProduct = 0;
					movl $0, %edx #edx = dotProduct = 0
					movl %ebx, j(%ebp) #store index j in ebp

      			#for(int k = 0; k < num_cols_a; ++k){
          			movl $0, %ecx
          			    #k < num_cols_a
						#k - num_cols_a < 0
						#k - num_cols_a >= 0 (NEGATION)
						third_for_start:
							cmpl num_cols_a(%ebp), %ecx
							jge third_for_end
							pushl %ebx

							pushl %eax
							#int rowA = a[i][k];
							movl a(%ebp), %eax
							movl (%eax, %edi, ws), %eax
							movl (%eax, %ecx, ws), %eax

							push %edx
							#int colB = b[k][j];
							movl j(%ebp), %edx
							movl b(%ebp), %ebx
							movl (%ebx, %ecx, ws), %ebx
							movl (%ebx, %edx, ws), %ebx

							#a[i][k] * b[k][j]
							mull %ebx #ebx = ebx * eax
							pop %edx
							addl %eax, %edx #dotProduct += a[i][k] * b[k][j]
							pop %eax #eax has the result
							pop %ebx

							incl %ecx #k++
							jmp third_for_start
						third_for_end:
							push %eax
							#resultMatrix[i][j] = dotProduct
							movl (%eax, %edi, ws), %eax
							movl %edx, (%eax, %ebx, ws)
							pop %eax #eax has the result

					incl %ebx #j++
					jmp second_for_start
				second_for_end:

			incl %edi #i++
			jmp first_for_start
		first_for_end:

		movl resultMatrix(%ebp), %eax #eax has the return value

	epilogue:
		#restore registers via pop
		pop %edi
		pop %ebx
		movl %ebp, %esp
		pop %ebp
		ret
