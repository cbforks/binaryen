	.text
	.file	"/b/build/slave/linux/build/src/src/work/gcc/gcc/testsuite/gcc.c-torture/execute/931004-2.c"
	.section	.text.f,"ax",@progbits
	.hidden	f
	.globl	f
	.type	f,@function
f:                                      # @f
	.param  	i32, i32
	.result 	i32
	.local  	i32, i32, i32
# BB#0:                                 # %entry
	i32.const	$push13=, 0
	i32.const	$push10=, 0
	i32.load	$push11=, __stack_pointer($pop10)
	i32.const	$push12=, 16
	i32.sub 	$push20=, $pop11, $pop12
	tee_local	$push19=, $4=, $pop20
	i32.store	$drop=, __stack_pointer($pop13), $pop19
	i32.store	$drop=, 12($4), $1
	i32.const	$push18=, 4
	i32.add 	$push1=, $1, $pop18
	i32.store	$drop=, 12($4), $pop1
	i32.load	$3=, 0($1)
	block
	block
	block
	i32.const	$push17=, 1
	i32.lt_s	$push2=, $0, $pop17
	br_if   	0, $pop2        # 0: down to label2
# BB#1:                                 # %for.body.preheader
	i32.const	$push3=, 8
	i32.add 	$1=, $1, $pop3
	i32.const	$2=, 0
.LBB0_2:                                # %for.body
                                        # =>This Inner Loop Header: Depth=1
	loop                            # label3:
	i32.const	$push21=, 10
	i32.add 	$push4=, $2, $pop21
	i32.ne  	$push5=, $pop4, $3
	br_if   	3, $pop5        # 3: down to label1
# BB#3:                                 # %for.cond
                                        #   in Loop: Header=BB0_2 Depth=1
	i32.store	$drop=, 12($4), $1
	i32.const	$push26=, -4
	i32.add 	$push7=, $1, $pop26
	i32.load	$3=, 0($pop7)
	i32.const	$push25=, 4
	i32.add 	$push0=, $1, $pop25
	copy_local	$1=, $pop0
	i32.const	$push24=, 1
	i32.add 	$push23=, $2, $pop24
	tee_local	$push22=, $2=, $pop23
	i32.lt_s	$push6=, $pop22, $0
	br_if   	0, $pop6        # 0: up to label3
.LBB0_4:                                # %for.end
	end_loop                        # label4:
	end_block                       # label2:
	i32.const	$push8=, 123
	i32.ne  	$push9=, $3, $pop8
	br_if   	1, $pop9        # 1: down to label0
# BB#5:                                 # %if.end8
	i32.const	$push16=, 0
	i32.const	$push14=, 16
	i32.add 	$push15=, $4, $pop14
	i32.store	$drop=, __stack_pointer($pop16), $pop15
	return  	$1
.LBB0_6:                                # %if.then
	end_block                       # label1:
	call    	abort@FUNCTION
	unreachable
.LBB0_7:                                # %if.then7
	end_block                       # label0:
	call    	abort@FUNCTION
	unreachable
	.endfunc
.Lfunc_end0:
	.size	f, .Lfunc_end0-f

	.section	.text.main,"ax",@progbits
	.hidden	main
	.globl	main
	.type	main,@function
main:                                   # @main
	.result 	i32
	.local  	i32
# BB#0:                                 # %entry
	i32.const	$push7=, 0
	i32.const	$push4=, 0
	i32.load	$push5=, __stack_pointer($pop4)
	i32.const	$push6=, 16
	i32.sub 	$push9=, $pop5, $pop6
	tee_local	$push8=, $0=, $pop9
	i32.store	$drop=, __stack_pointer($pop7), $pop8
	i64.const	$push0=, 528280977420
	i64.store	$drop=, 8($0), $pop0
	i64.const	$push1=, 47244640266
	i64.store	$drop=, 0($0), $pop1
	i32.const	$push2=, 3
	i32.call	$drop=, f@FUNCTION, $pop2, $0
	i32.const	$push3=, 0
	call    	exit@FUNCTION, $pop3
	unreachable
	.endfunc
.Lfunc_end1:
	.size	main, .Lfunc_end1-main


	.ident	"clang version 4.0.0 "
	.functype	abort, void
	.functype	exit, void, i32
