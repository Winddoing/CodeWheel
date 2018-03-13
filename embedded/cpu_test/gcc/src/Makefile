###############################################################################
# SPECtools2000 modifie for 95
#   Version 0.3
#   This makefile should NOT be changed other than to customize for a
#   particular benchmark.  Users may overide variables by placing them
#   in the Makefile.spec file.  Or by using the "runspec" program to
#   build the Makefile.spec file for them.
###############################################################################
#	BENCHMARK	-- standard definitions for this benchmark

NUMBER    = 120
NAME      = gcc
EXEBASE   = ../bin/gcc
BENCHLANG = C
MYOBJS = c-parse.o c-lang.o c-lex.o c-pragma.o c-decl.o c-typeck.o c-convert.o c-aux-info.o c-common.o c-iterate.o bc-emit.o bc-optab.o version.o tree.o print-tree.o stor-layout.o fold-const.o function.o stmt.o expr.o calls.o expmed.o explow.o optabs.o varasm.o rtl.o print-rtl.o rtlanal.o emit-rtl.o real.o dbxout.o sdbout.o dwarfout.o xcoffout.o integrate.o jump.o cse.o loop.o unroll.o flow.o stupid.o combine.o regclass.o local-alloc.o global.o reload.o reload1.o caller-save.o insn-peep.o reorg.o sched.o final.o recog.o reg-stack.o insn-opinit.o insn-recog.o insn-extract.o insn-output.o insn-emit.o insn-attrtab.o aux-output.o getpwd.o convert.o ucbqsort.o obstack.o getsrc.o t_gcc.o

SOURCES   = $(addsuffix $(OBJ),$(basename $(MYOBJS)))
NEED_MATH = yes


# Include standard template for this suite
-include ../../../../Makefile.defaults
-include ../../../Makefile.defaults
-include ../../Makefile.defaults
-include ../Makefile.defaults

nomakefiles:
	@echo "*** Whoops!  I didn't find a Makefile.defaults file."
	@echo "*** Your distribution is probably corrupt."
	@echo
