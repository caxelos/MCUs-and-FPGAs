######################  Axelos Christos  ###############################################################################
# - This program compresses a data stream if it is uncompressed and decompresses it if it is already compressed.
# - I use three character arrays: (1) for the original data, (2) for the final output, and (3) when processing/outputting the "0x1B" marker.
# - Below, I explain how the compression and decompression work.
 
##################################################################################################################################

.data
	.align 0
	init_str: #.space 1001
		  .byte
	'a', 'b', 'c', 'c', 'c', 'd', 'd', 'd', 'd', 'd', 'd', 'b', 'b', 'c', 'c', 'c', 'c', 'a', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'e', 'f', 0x1b
	#'a', 'b', 'c', 'c', 'c',  0x1b, 'd', '6', 'b', 'b', 0x1b, 'e', '1', '2', 'f', 0x1b, 0x1b
	
	final_str: .space 1000
	esc_str: .asciiz "0x1b"
	
.text
.globl main
################# VARIABLES ##############
# $s0 - Base address of the original string
# $s1 - Pointer to the original string  	 
main:
	la $s0, init_str	# load str address
	la $s1, final_str
	
	###### FIND IF COMPRESSED OR NOT ######
	# if the 2nd last byte is 0xb1, then it is in compressed form!
	
	lbu $t1, -2($s1)
	beq $t1, 0x1b, Compressed
	
	######## UN-COMPRESSED DATA AND COMPRESS IT ###############
	
	move $a0, $s0
	move $a1, $s1
	
	jal compress_it
	j EXIT_PROGRAMM
	
	######## COMPRESSED DATA AND UN-COMPRESS IT ############## 
	Compressed:
	
	move $a0, $s0
	move $a1, $s1
	
	jal uncompress_it
	########### exit programm ###########
	EXIT_PROGRAMM:
	
	li $v0, 10
	move $a0, $v0
	syscall
	
################################ PROCESS "compress_it" ###############################################################
# - HOW IS THE COMPRESSION PERFORMED?
#   - (1) Read each character and examine the next three characters as well.
#   - (2) If at least one of the next three characters differs, store the character
#         normally for each consecutive occurrence.
#   - (3) If the next three characters are identical to the current character,
#         write the marker 0x1B, followed by the character and its repetition count
#         (all encoded in ASCII format).
#   - If the current character is 0xB1, exit the loop and terminate the program.	

compress_it:
	move $s0, $a0
	move $s1, $a1
	
	li $s7, 0x1b
	
	#move $t0, $a0
	#move $t1, $a1
	
	#li $s2, 0	# deikths ston INIT peinaka
	#li $s3, 0	# deikths ston FINAL pinaka
	
	li $v0, 11
	
	MAIN_COMPRESSION_LOOP:
		#lbu $t0, init_str($s2)	#load N byte
		lbu  $t0, 0($s0)
		beq $t0, 0x1b, EXIT
		
		li $v0, 11
		
		li $t4, ','
		move $a0, $t4
		syscall
		#move $a0, $t0
		#syscall
		
		#lb $t1, init_str + 1($s2)	#load N+1 byte(if )
		lb  $t1, 1($s0)
		bne $t0, $t1, ADD_1 
		
		#lb $t2, init_str + 2($s2)	#load N+2 byte(if)
		lb $t2, 2($s0)
		bne $t0, $t2, ADD_2_IN_ROW
		
		#lb $t3, init_str + 3($s2)  #load N+3 byte (if)
		lb  $t3, 3($s0)
		bne $t0, $t3, ADD_3_IN_ROW
		
		############## THIS CODE EXECUTES FOR ZIP PROCESS ###############
		# PRINT "Oxb1"
		li $v0, 4
		la $a0, esc_str
	
		#sb $s7, final_str($s3)
		sb $s7, 0($s1)
		addi $s1, $s1, 1
		syscall
		
		li $v0, 11
		move $a0, $t4
		syscall
		
		# PRINT LETTER
		#sb $t0, final_str($s3)
		sb  $t0, 0($s1)
		addi $s1, $s1, 1
		move $a0, $t0
		syscall
		
		move $a0, $t4
		syscall
		
		
		li $t7, 4	#times already found the character
		addi $s0, $s0, 4
		
		Loop2:
			#lb $t4, init_str($s2)
			lb  $t4, 0($s0)
			bne $t0, $t4, Exit_Loop2
			
			addi $t7, $t7, 1
			addi $s0, $s0, 1
			j Loop2
			
		Exit_Loop2:
		
		li $v0, 1
		move $a0, $t7
		syscall
		
		addi $t7, $t7, '0'
		#sb $t7, final_str($s3)
		sb  $t7, 0($s1)
		addi $s1, $s1, 1
		
		li $v0, 4 
		j MAIN_COMPRESSION_LOOP
	
	ADD_1:
		#sb $t0, final_str($s3)
		sb $t0, 0($s1)
		addi $s1, $s1, 1
		
		addi $s0, $s0, 1
		move $a0, $t0
		syscall
		
		j MAIN_COMPRESSION_LOOP
	
	ADD_2_IN_ROW:
		move $a0, $t0
	
		#sb $t0, final_str($s3)
		sb  $t0,  ($s1)
		addi $s1, $s1, 1
		syscall
		
		move $a0, $t4
		syscall
		
		move $a0, $t0 
		#sb $t0, final_str($s3)
		sb  $t0, 0($s1)
		addi $s1, $s1, 1
		syscall
		
		addi $s0, $s0, 2
		j MAIN_COMPRESSION_LOOP
		
	ADD_3_IN_ROW:
		move $a0, $t0
	
		#sb $t0, final_str($s3)
		sb  $t0, 0($s1)
		addi $s1, $s1, 1
		syscall
		
		move $a0, $t4
		syscall
		
		move $a0, $t0
		#sb $t0, final_str($s3)
		sb  $t0,  0($s1)
		addi $s1, $s1, 1
		syscall
		
		move $a0, $t4
		syscall
		
		move $a0, $t0
		#sb $t0, final_str($s3)
		sb  $t0,  0($s1)
		addi $s1, $s1, 1
		syscall
		
		addi $s0, $s0, 3
		j MAIN_COMPRESSION_LOOP
	EXIT:
	
	li $t1, ','
	li $v0, 11
	move $a0, $t1
	syscall
	
	li $t0, 0x1b
	#sb $t0, final_str($s3)
	sb  $t0, 0($s1)
	addi $s1, $s1, 1
	#sb $t0, final_str($s3)
	sb  $t0, 0($s1) 	
	li $v0, 4
	la $a0, esc_str
	syscall
	
	li $v0, 11
	move $a0, $t1
	syscall
	
	li $v0, 4
	la $a0, esc_str
	syscall
	
	##### exit process #####
	jr $ra
	
################################# Process uncompress_it: #################################
# - HOW IS THE DECOMPRESSION PERFORMED?
#   1) Read the input characters one by one and copy them to memory.
#   2) When the marker 0x1B is encountered:
#      - Read the next character.
#      - If it is another 0x1B, terminate the program.
#      - Otherwise, treat it as the character to be expanded and read the
#        repetition count that follows.
#   3) Write the character to memory as many times as indicated by the count.
 
uncompress_it:
	#la $s0, init_str	# load str address
	#la $s1, final_str
	move $s0, $a0
	move $s1, $a1
	
	#li $s0, 0
	#li $s1, 0
	
	li $s7, ','
	li $v0, 11

	UNCOMPRESSION_MAIN_LOOP:
	
	#lbu $t0, init_str($s0)
	lbu  $t0, 0($s0)
	
	beq $t0, 0x1b, UNZIP
	#sb $t0, final_str($s1)
	sb $t0, 0($s1)
	
	move $a0, $t0
	syscall
	
	move $a0, $s7
	syscall
	
	addi $s0, $s0, 1
	addi $s1, $s1, 1
		
	j UNCOMPRESSION_MAIN_LOOP
	### sto shmeio afto ftanoume otan synantame toulaxiston 4 synexomenous xarakthres h' telos twn dedomenwn ##
	UNZIP:
	
	addi $s0, $s0, 1	#load next char after 0xb1
	#lbu $t0, init_str($s0)	#(0xb1, char, times)
	lbu  $t0, 0($s0)
	beq $t0, 0x1b, quit_main_loop  # gia 2o sinexomeno 0x1b, vges
	addi $s0, $s0, 1
	
	#### THIS LOOP MAKES THE "ATOI" FUNCTION #####
	li $t2, 0	# loop counter
	li $t3, 0
	li $t5, 0	# final number
	li $t7, 10
	
	#### Edw ypologizw POSES fores synanthsa ton xarakthra sthn seira #####
	TIMES_IN_ROW_LOOP:
		#lb $t1, init_str($s0)
		lb $t1,  0($s0)
	
		blt $t1, '0', TIMES_FOUND	# if it isn't number, exit
		bgt $t1, '9', TIMES_FOUND # The same...
		
		beq $t5, 0, Continue
		li $t2, 0
		
		mult_loop:
			add $t3, $t3, $t5
			addi $t2, $t2, 1
	
			beq $t2, 10, Exit_mult
			j mult_loop
		
		Exit_mult:
		move $t5, $t3
		
		Continue:  
		sub  $t4, $t1, '0'	#the real digit, not ascii
		add $t5, $t5, $t4
		
		addi $s0, $s0, 1

		
		j TIMES_IN_ROW_LOOP
			
	TIMES_FOUND:
	######## Write the character to memory as many times as it was encountered #####
	li $t4, 0	#loop counter
	uncompressAndSaveLOOP:
		beq $t4, $t5, Exit_Loop3
	
		#sb $t0, final_str($s1)
		sb $t0,  0($s1)
		move $a0, $t0
		syscall
		
		move $a0, $s7
		syscall
		
		addi $s1, $s1, 1
		addi $t4, $t4, 1
		
		j uncompressAndSaveLOOP
	
	Exit_Loop3:
	j UNCOMPRESSION_MAIN_LOOP
	
	quit_main_loop:
	li $t5, 0x1b
	#sb $t5, final_str($s1)
	sb  $t5,  0($s1)
	
	li $v0, 4
	la $a0, esc_str
	syscall
	##### exit process #####
	jr $ra
	
		
