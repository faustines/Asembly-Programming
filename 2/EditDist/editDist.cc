/*
int main(int argc, char** argv){
  if(argc < 3){
    printf("Usage: %s word1 word 2\n", argv[0]);
    exit(1);
  }
  printf("The distance between %s and %s is %d.\n", argv[1], argv[2], editDist(argv[1], argv[2]));

  return 0;
}
*/


#string1, string2 (char[i])
#eax register is final output

.global _start
.equ ws, 4

.data

string1:
	.rept 100
		.byte 0
	.endr

string2:
	.rept 100
		.byte 0
	.endr

oldDist:
	.rept 100 #allocating 100 bytes
			.int 0
	.endr

curDist:
	.rept 100 #allocating 100 bytes
			.int 0
	.endr

word1_len: #string1 length
	.int 0

word2_len: #string2 length
	.int 0

.text

/*int min(int a, int b){
  return a < b ? a:b;
}*/

/*
int min(int a, int b){
	if(a < b){
	return a;
	}
	else{
	return b;
	}
}
*/

min:

	#EAX will be A
	#EBX will be B
	#EAX will have the return value

	#if(a < b)
   #if(a - b < 0)
   #NEG: if(a - b >= 0)
	min_if:
		cmpl %ebx, %eax
		jge min_else
		movl %eax, %eax
		ret #return statement
	min_else:
		movl %ebx, %eax
		ret #return statement

strlen:
	/*
	int str_len(char* str){
		int len;
		for(len = 0, str[len]!='\0'; ++len);

		return len;
	}
*/
	#str will be ESI
	#EAX is len, which is the return value

	#for(len = 0, str[len]!='\0'; ++len);
	movl $0, %eax; #len = 0

	strlen_for_start:
		#str[len]!='\0'
		#'\0' == 0
		#str[len]!=0
		#neg str[len] == \0

		cmpb $0, (%esi, %eax)
		jz strlen_for_end

		incl %eax # ++len
		jmp strlen_for_start
	strlen_for_end:
	ret #return to where you were called from
end_strlen:

_start:

	#ESI = String

	#int word1_len = strlen(word1);
	movl $string1, %esi
	call strlen
	movl %eax, word1_len

	#int word2_len = strlen(word2);
	movl $string2, %esi
	call strlen
	movl %eax, word2_len

	/*
	for(i = 0; i < word2_len + 1; i++){
    oldDist[i] = i;
    # *(oldDist + i) = i
    curDist[i] = i;
    # *(curDist + i) = i
  }
	*/

	#ECX = i
	#i < word2_len + 1
	#i <= word2_len
	#i - word2_len <= 0
	#i - word2_len > 0 (NEGATION)


	movl $0, %ecx #ecx = i = 0

	start_for_loop:
			cmpl word2_len, %ecx #i - word2_len
			jg end_for_loop ##i - word2_len > 0 (NEGATION)
				#Advanced indexing mode; oldDist and curDist are registers.
				# *(oldDist + i) = i
				movl %ecx, oldDist(, %ecx, ws)
				# *(curDist + i) = i
				movl %ecx, curDist(, %ecx, ws)
			incl %ecx #i++
			jmp start_for_loop
	end_for_loop:


	#ECX = i
	#EDX = j

	movl $1, %ecx #ecx = i = 1


	#for(i = 1; i < word1_len + 1; i++)
	#for(i = 1; i <= word1_len; i++)

	outer_for_loop:
		cmpl word1_len, %ecx #i - word1_len
		jg end_outer #ecx - word1_len > 0 (NEGATION)
			#curDist[0] = i;
			#*(curDist + 0) = i
			movl %ecx, curDist

			#for(j = 1; j < word2_len + 1; j++)
			#for(j = 1; j <= word2_len; j++)

			movl $1, %edx #edx = j = 1

			inner_for_loop:
				cmpl word2_len, %edx #j - word2_len
				jg end_inner #j - word2_len > 0 (NEGATION)
				#word1 = string1
				#word2 = string2

				#if(word1[i-1] == word2[j-1])
				#if(word1[i-1] - word2[j-1] == 0)
				#NEG: if(word1[i-1] - word2[j-1] != 0)
				#word1[i-1] = *(word1 + i - 1)
				#word1 - 1*ws(, %ecx, ws)
				#word2[j-1] = *(word2 + j - 1)
				#word2 - 1*ws(, %edx, ws)

				if:
					movb string1 - 1(, %ecx, 1), %bl
					movb string2 - 1(, %edx, 1), %al
					cmpb %al, %bl
					jnz else # != 0
					#curDist[j] = oldDist[j - 1];
					#*(curDist + j) = *(oldDist + j - 1)

					movl oldDist -1*ws(, %edx, ws), %edi
					movl %edi, curDist(, %edx, ws)
					jmp end_else

				else:
				#curDist[j] = min(min(oldDist[j], curDist[j-1]), oldDist[j-1]) + 1;
					#EAX = oldDist[j]
					#EBX = curDist[j-1]
					#call min
					movl $0, %eax
					movl $0, %ebx

					movl oldDist(, %edx, ws), %eax
					movl curDist -1*ws(, %edx, ws), %ebx
					call min

					#EAX will have the min
					#EBX = oldDist[j-1]
					#call min

          movl oldDist - 1*ws(, %edx, ws), %ebx
					call min

					#EAX has min
					#EAX = EAX + 1 (addl)
					addl $1, %eax

					#curDist[j] = EAX (movl)
					movl %eax, curDist(, %edx, ws)
				end_else:


				incl %edx
				movl $0, %eax
				movl $0, %ebx
				jmp inner_for_loop
			end_inner:

		#swap(&oldDist, &curDist);
		#swap(oldDistPtr, curDistPtr)

     #EDI = k
     movl $0, %edi
     /*
     for(int k = 0; k <= word2_len; ++k){
       oldDist[k] = curDist[k]
     }
     */
     start_copy_loop:
       cmpl word2_len, %edi
       jg end_copy
       #oldDist]k] = curDist[k]
       #*(oldDist + k) = *(curDist + k)
       movl curDist(, %edi, ws), %ebx
       movl %ebx, oldDist(, %edi, ws)
       incl %edi
       jmp start_copy_loop
     end_copy:
       #display ((int*)&oldDist)[0]@#

		incl %ecx
		jmp outer_for_loop
	end_outer:

	#dist = oldDist[word2_len]
	#eax = dist (move dist into eax)
  movl word2_len, %ebx
  movl oldDist(,%ebx, ws), %eax


done:
	nop #do nothing
