/* Generated automatically by the program `genpeep'
from the machine description file `md'.  */

#include "config.h"
#include "rtl.h"
#include "regs.h"
#include "output.h"
#include "real.h"

extern rtx peep_operand[];

#define operands peep_operand

rtx
peephole (ins1)
     rtx ins1;
{
  rtx insn, x, pat;
  int i;

  if (NEXT_INSN (ins1)
      && GET_CODE (NEXT_INSN (ins1)) == BARRIER)
    return 0;

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L245;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! register_operand (x, SImode)) goto L245;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! memory_operand (x, SImode)) goto L245;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L245; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L245;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L245;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! register_operand (x, SImode)) goto L245;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! memory_operand (x, SImode)) goto L245;
  if (! (registers_ok_for_ldd_peep (operands[0], operands[2]) 
   && ! MEM_VOLATILE_P (operands[1]) && ! MEM_VOLATILE_P (operands[3])
   && addrs_ok_for_ldd_peep (XEXP (operands[1], 0), XEXP (operands[3], 0)))) goto L245;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (4, operands));
  INSN_CODE (ins1) = 245;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L245:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L246;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! memory_operand (x, SImode)) goto L246;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! register_operand (x, SImode)) goto L246;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L246; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L246;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L246;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! memory_operand (x, SImode)) goto L246;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! register_operand (x, SImode)) goto L246;
  if (! (registers_ok_for_ldd_peep (operands[1], operands[3]) 
   && ! MEM_VOLATILE_P (operands[0]) && ! MEM_VOLATILE_P (operands[2])
   && addrs_ok_for_ldd_peep (XEXP (operands[0], 0), XEXP (operands[2], 0)))) goto L246;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (4, operands));
  INSN_CODE (ins1) = 246;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L246:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L247;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! register_operand (x, SFmode)) goto L247;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! memory_operand (x, SFmode)) goto L247;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L247; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L247;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L247;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! register_operand (x, SFmode)) goto L247;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! memory_operand (x, SFmode)) goto L247;
  if (! (registers_ok_for_ldd_peep (operands[0], operands[2]) 
   && ! MEM_VOLATILE_P (operands[1]) && ! MEM_VOLATILE_P (operands[3])
   && addrs_ok_for_ldd_peep (XEXP (operands[1], 0), XEXP (operands[3], 0)))) goto L247;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (4, operands));
  INSN_CODE (ins1) = 247;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L247:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L248;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! memory_operand (x, SFmode)) goto L248;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! register_operand (x, SFmode)) goto L248;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L248; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L248;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L248;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! memory_operand (x, SFmode)) goto L248;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! register_operand (x, SFmode)) goto L248;
  if (! (registers_ok_for_ldd_peep (operands[1], operands[3]) 
   && ! MEM_VOLATILE_P (operands[0]) && ! MEM_VOLATILE_P (operands[2])
   && addrs_ok_for_ldd_peep (XEXP (operands[0], 0), XEXP (operands[2], 0)))) goto L248;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (4, operands));
  INSN_CODE (ins1) = 248;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L248:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L249;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! register_operand (x, SImode)) goto L249;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! memory_operand (x, SImode)) goto L249;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L249; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L249;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L249;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! register_operand (x, SImode)) goto L249;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! memory_operand (x, SImode)) goto L249;
  if (! (registers_ok_for_ldd_peep (operands[2], operands[0]) 
   && ! MEM_VOLATILE_P (operands[3]) && ! MEM_VOLATILE_P (operands[1])
   && addrs_ok_for_ldd_peep (XEXP (operands[3], 0), XEXP (operands[1], 0)))) goto L249;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (4, operands));
  INSN_CODE (ins1) = 249;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L249:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L250;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! memory_operand (x, SImode)) goto L250;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! register_operand (x, SImode)) goto L250;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L250; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L250;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L250;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! memory_operand (x, SImode)) goto L250;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! register_operand (x, SImode)) goto L250;
  if (! (registers_ok_for_ldd_peep (operands[3], operands[1]) 
   && ! MEM_VOLATILE_P (operands[2]) && ! MEM_VOLATILE_P (operands[0])
   && addrs_ok_for_ldd_peep (XEXP (operands[2], 0), XEXP (operands[0], 0)))) goto L250;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (4, operands));
  INSN_CODE (ins1) = 250;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L250:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L251;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! register_operand (x, SFmode)) goto L251;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! memory_operand (x, SFmode)) goto L251;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L251; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L251;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L251;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! register_operand (x, SFmode)) goto L251;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! memory_operand (x, SFmode)) goto L251;
  if (! (registers_ok_for_ldd_peep (operands[2], operands[0]) 
   && ! MEM_VOLATILE_P (operands[3]) && ! MEM_VOLATILE_P (operands[1])
   && addrs_ok_for_ldd_peep (XEXP (operands[3], 0), XEXP (operands[1], 0)))) goto L251;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (4, operands));
  INSN_CODE (ins1) = 251;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L251:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L252;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! memory_operand (x, SFmode)) goto L252;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! register_operand (x, SFmode)) goto L252;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L252; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L252;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L252;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! memory_operand (x, SFmode)) goto L252;
  x = XEXP (pat, 1);
  operands[3] = x;
  if (! register_operand (x, SFmode)) goto L252;
  if (! (registers_ok_for_ldd_peep (operands[3], operands[1]) 
   && ! MEM_VOLATILE_P (operands[2]) && ! MEM_VOLATILE_P (operands[0])
   && addrs_ok_for_ldd_peep (XEXP (operands[2], 0), XEXP (operands[0], 0)))) goto L252;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (4, operands));
  INSN_CODE (ins1) = 252;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L252:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L253;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! register_operand (x, SImode)) goto L253;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! register_operand (x, SImode)) goto L253;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L253; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L253;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L253;
  x = XEXP (pat, 0);
  if (GET_CODE (x) != REG) goto L253;
  if (GET_MODE (x) != CCmode) goto L253;
  if (XINT (x, 0) != 0) goto L253;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != COMPARE) goto L253;
  if (GET_MODE (x) != CCmode) goto L253;
  x = XEXP (XEXP (pat, 1), 0);
  operands[2] = x;
  if (! register_operand (x, SImode)) goto L253;
  x = XEXP (XEXP (pat, 1), 1);
  if (GET_CODE (x) != CONST_INT) goto L253;
  if (XWINT (x, 0) != 0) goto L253;
  if (! ((rtx_equal_p (operands[2], operands[0])
    || rtx_equal_p (operands[2], operands[1]))
   && ! FP_REG_P (operands[0]) && ! FP_REG_P (operands[1]))) goto L253;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 253;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L253:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L254;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! register_operand (x, HImode)) goto L254;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! memory_operand (x, HImode)) goto L254;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L254; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L254;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L254;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! register_operand (x, SImode)) goto L254;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != SIGN_EXTEND) goto L254;
  if (GET_MODE (x) != SImode) goto L254;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[0], x)) goto L254;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L254; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L254;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L254;
  x = XEXP (pat, 0);
  if (GET_CODE (x) != REG) goto L254;
  if (GET_MODE (x) != CCmode) goto L254;
  if (XINT (x, 0) != 0) goto L254;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != COMPARE) goto L254;
  if (GET_MODE (x) != CCmode) goto L254;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[2], x)) goto L254;
  x = XEXP (XEXP (pat, 1), 1);
  if (GET_CODE (x) != CONST_INT) goto L254;
  if (XWINT (x, 0) != 0) goto L254;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 254;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L254:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L255;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! register_operand (x, QImode)) goto L255;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! memory_operand (x, QImode)) goto L255;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L255; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L255;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L255;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! register_operand (x, SImode)) goto L255;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != SIGN_EXTEND) goto L255;
  if (GET_MODE (x) != SImode) goto L255;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[0], x)) goto L255;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L255; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L255;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L255;
  x = XEXP (pat, 0);
  if (GET_CODE (x) != REG) goto L255;
  if (GET_MODE (x) != CCmode) goto L255;
  if (XINT (x, 0) != 0) goto L255;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != COMPARE) goto L255;
  if (GET_MODE (x) != CCmode) goto L255;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[2], x)) goto L255;
  x = XEXP (XEXP (pat, 1), 1);
  if (GET_CODE (x) != CONST_INT) goto L255;
  if (XWINT (x, 0) != 0) goto L255;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 255;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L255:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L256;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! register_operand (x, HImode)) goto L256;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! memory_operand (x, HImode)) goto L256;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L256; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L256;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L256;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! register_operand (x, SImode)) goto L256;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != SIGN_EXTEND) goto L256;
  if (GET_MODE (x) != SImode) goto L256;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[0], x)) goto L256;
  if (! (dead_or_set_p (insn, operands[0]))) goto L256;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 256;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L256:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L257;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! register_operand (x, QImode)) goto L257;
  x = XEXP (pat, 1);
  operands[1] = x;
  if (! memory_operand (x, QImode)) goto L257;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L257; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L257;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L257;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! register_operand (x, SImode)) goto L257;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != SIGN_EXTEND) goto L257;
  if (GET_MODE (x) != SImode) goto L257;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[0], x)) goto L257;
  if (! (dead_or_set_p (insn, operands[0]))) goto L257;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 257;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L257:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L258;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! register_operand (x, SImode)) goto L258;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != LO_SUM) goto L258;
  if (GET_MODE (x) != SImode) goto L258;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[0], x)) goto L258;
  x = XEXP (XEXP (pat, 1), 1);
  operands[1] = x;
  if (! immediate_operand (x, SImode)) goto L258;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L258; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L258;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L258;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! register_operand (x, DFmode)) goto L258;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != MEM) goto L258;
  if (GET_MODE (x) != DFmode) goto L258;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[0], x)) goto L258;
  if (! (RTX_UNCHANGING_P (operands[1]) && reg_unused_after (operands[0], insn))) goto L258;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 258;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L258:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L259;
  x = XEXP (pat, 0);
  operands[0] = x;
  if (! register_operand (x, SImode)) goto L259;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != LO_SUM) goto L259;
  if (GET_MODE (x) != SImode) goto L259;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[0], x)) goto L259;
  x = XEXP (XEXP (pat, 1), 1);
  operands[1] = x;
  if (! immediate_operand (x, SImode)) goto L259;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L259; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L259;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L259;
  x = XEXP (pat, 0);
  operands[2] = x;
  if (! register_operand (x, SFmode)) goto L259;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != MEM) goto L259;
  if (GET_MODE (x) != SFmode) goto L259;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[0], x)) goto L259;
  if (! (RTX_UNCHANGING_P (operands[1]) && reg_unused_after (operands[0], insn))) goto L259;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 259;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L259:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != PARALLEL) goto L266;
  if (XVECLEN (x, 0) != 2) goto L266;
  x = XVECEXP (pat, 0, 0);
  if (GET_CODE (x) != SET) goto L266;
  x = XEXP (XVECEXP (pat, 0, 0), 0);
  operands[0] = x;
  x = XEXP (XVECEXP (pat, 0, 0), 1);
  if (GET_CODE (x) != CALL) goto L266;
  x = XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0);
  if (GET_CODE (x) != MEM) goto L266;
  if (GET_MODE (x) != SImode) goto L266;
  x = XEXP (XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0), 0);
  operands[1] = x;
  if (! call_operand_address (x, SImode)) goto L266;
  x = XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1);
  operands[2] = x;
  x = XVECEXP (pat, 0, 1);
  if (GET_CODE (x) != CLOBBER) goto L266;
  x = XEXP (XVECEXP (pat, 0, 1), 0);
  if (GET_CODE (x) != REG) goto L266;
  if (GET_MODE (x) != SImode) goto L266;
  if (XINT (x, 0) != 15) goto L266;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L266; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L266;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L266;
  x = XEXP (pat, 0);
  if (GET_CODE (x) != PC) goto L266;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != LABEL_REF) goto L266;
  x = XEXP (XEXP (pat, 1), 0);
  operands[3] = x;
  if (! (short_branch (INSN_UID (insn), INSN_UID (operands[3])))) goto L266;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (4, operands));
  INSN_CODE (ins1) = 266;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L266:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != PARALLEL) goto L267;
  if (XVECLEN (x, 0) != 2) goto L267;
  x = XVECEXP (pat, 0, 0);
  if (GET_CODE (x) != CALL) goto L267;
  x = XEXP (XVECEXP (pat, 0, 0), 0);
  if (GET_CODE (x) != MEM) goto L267;
  if (GET_MODE (x) != SImode) goto L267;
  x = XEXP (XEXP (XVECEXP (pat, 0, 0), 0), 0);
  operands[0] = x;
  if (! call_operand_address (x, SImode)) goto L267;
  x = XEXP (XVECEXP (pat, 0, 0), 1);
  operands[1] = x;
  x = XVECEXP (pat, 0, 1);
  if (GET_CODE (x) != CLOBBER) goto L267;
  x = XEXP (XVECEXP (pat, 0, 1), 0);
  if (GET_CODE (x) != REG) goto L267;
  if (GET_MODE (x) != SImode) goto L267;
  if (XINT (x, 0) != 15) goto L267;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L267; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L267;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L267;
  x = XEXP (pat, 0);
  if (GET_CODE (x) != PC) goto L267;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != LABEL_REF) goto L267;
  x = XEXP (XEXP (pat, 1), 0);
  operands[2] = x;
  if (! (short_branch (INSN_UID (insn), INSN_UID (operands[2])))) goto L267;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (3, operands));
  INSN_CODE (ins1) = 267;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L267:

  insn = ins1;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != PARALLEL) goto L268;
  if (XVECLEN (x, 0) != 2) goto L268;
  x = XVECEXP (pat, 0, 0);
  if (GET_CODE (x) != SET) goto L268;
  x = XEXP (XVECEXP (pat, 0, 0), 0);
  operands[0] = x;
  if (! register_operand (x, SImode)) goto L268;
  x = XEXP (XVECEXP (pat, 0, 0), 1);
  if (GET_CODE (x) != MINUS) goto L268;
  if (GET_MODE (x) != SImode) goto L268;
  x = XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 0);
  operands[1] = x;
  if (! reg_or_0_operand (x, SImode)) goto L268;
  x = XEXP (XEXP (XVECEXP (pat, 0, 0), 1), 1);
  if (GET_CODE (x) != REG) goto L268;
  if (GET_MODE (x) != SImode) goto L268;
  if (XINT (x, 0) != 0) goto L268;
  x = XVECEXP (pat, 0, 1);
  if (GET_CODE (x) != CLOBBER) goto L268;
  x = XEXP (XVECEXP (pat, 0, 1), 0);
  if (GET_CODE (x) != REG) goto L268;
  if (GET_MODE (x) != CCmode) goto L268;
  if (XINT (x, 0) != 0) goto L268;
  do { insn = NEXT_INSN (insn);
       if (insn == 0) goto L268; }
  while (GET_CODE (insn) == NOTE
	 || (GET_CODE (insn) == INSN
	     && (GET_CODE (PATTERN (insn)) == USE
		 || GET_CODE (PATTERN (insn)) == CLOBBER)));
  if (GET_CODE (insn) == CODE_LABEL
      || GET_CODE (insn) == BARRIER)
    goto L268;
  pat = PATTERN (insn);
  x = pat;
  if (GET_CODE (x) != SET) goto L268;
  x = XEXP (pat, 0);
  if (GET_CODE (x) != REG) goto L268;
  if (GET_MODE (x) != CCmode) goto L268;
  if (XINT (x, 0) != 0) goto L268;
  x = XEXP (pat, 1);
  if (GET_CODE (x) != COMPARE) goto L268;
  x = XEXP (XEXP (pat, 1), 0);
  if (!rtx_equal_p (operands[0], x)) goto L268;
  x = XEXP (XEXP (pat, 1), 1);
  if (GET_CODE (x) != CONST_INT) goto L268;
  if (XWINT (x, 0) != 0) goto L268;
  PATTERN (ins1) = gen_rtx (PARALLEL, VOIDmode, gen_rtvec_v (2, operands));
  INSN_CODE (ins1) = 268;
  delete_for_peephole (NEXT_INSN (ins1), insn);
  return NEXT_INSN (insn);
 L268:

  return 0;
}

rtx peep_operand[4];
