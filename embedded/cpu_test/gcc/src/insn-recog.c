/* Generated automatically by the program `genrecog'
from the machine description file `md'.  */

#include "config.h"
#include "rtl.h"
#include "insn-config.h"
#include "recog.h"
#include "real.h"
#include "output.h"
#include "flags.h"

extern rtx gen_split_87 ();
extern rtx gen_split_146 ();
extern rtx gen_split_152 ();
extern rtx gen_split_158 ();
extern rtx gen_split_159 ();
extern rtx gen_split_231 ();
extern rtx gen_split_232 ();
extern rtx gen_split_233 ();
extern rtx gen_split_234 ();
extern rtx gen_split_235 ();
extern rtx gen_split_236 ();
extern rtx gen_split_237 ();
extern rtx gen_split_238 ();
extern rtx gen_split_239 ();
extern rtx gen_split_240 ();
extern rtx gen_split_241 ();
extern rtx gen_split_242 ();
extern rtx gen_split_243 ();
extern rtx gen_split_244 ();

/* `recog' contains a decision tree
   that recognizes whether the rtx X0 is a valid instruction.

   recog returns -1 if the rtx is not valid.
   If the rtx is valid, recog returns a nonnegative number
   which is the insn code number for the pattern that matched.
   This is the same as the order in the machine description of
   the entry that matched.  This number can be used as an index into
   entry that matched.  This number can be used as an index into various
   insn_* tables, such as insn_templates, insn_outfun, and insn_n_operands
   (found in insn-output.c).

   The third argument to recog is an optional pointer to an int.
   If present, recog will accept a pattern if it matches except for
   missing CLOBBER expressions at the end.  In that case, the value
   pointed to by the optional pointer will be set to the number of
   CLOBBERs that need to be added (it should be initialized to zero by
   the caller).  If it is set nonzero, the caller should allocate a
   PARALLEL of the appropriate size, copy the initial entries, and call
   add_clobbers (found in insn-emit.c) to fill in the CLOBBERs.

   The function split_insns returns 0 if the rtl could not
   be split or the split rtl in a SEQUENCE if it can be.*/

rtx recog_operand[MAX_RECOG_OPERANDS];

rtx *recog_operand_loc[MAX_RECOG_OPERANDS];

rtx *recog_dup_loc[MAX_DUP_OPERANDS];

char recog_dup_num[MAX_DUP_OPERANDS];

#define operands recog_operand

int
recog_1 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn;
     int *pnum_clobbers;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1, x2, x3, x4, x5;
  int tem;

  x1 = XEXP (x0, 1);
  switch (GET_MODE (x1))
    {
    case SImode:
      switch (GET_CODE (x1))
	{
	case NE:
	  goto L46;
	case NEG:
	  goto L60;
	case EQ:
	  goto L74;
	case PLUS:
	  goto L104;
	case MINUS:
	  goto L173;
	case LTU:
	  goto L162;
	case GEU:
	  goto L189;
	}
    }
  L254:
  if (noov_compare_op (x1, SImode))
    {
      ro[1] = x1;
      goto L255;
    }
  goto ret0;

  L46:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L47;
    }
  goto L254;

  L47:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 23;
    }
  goto L254;

  L60:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != SImode)
    {
      goto L254;
    }
  switch (GET_CODE (x2))
    {
    case NE:
      goto L61;
    case EQ:
      goto L89;
    case LTU:
      goto L168;
    case PLUS:
      goto L182;
    case GEU:
      goto L195;
    }
  goto L254;

  L61:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L62;
    }
  goto L254;

  L62:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 24;
    }
  goto L254;

  L89:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L90;
    }
  goto L254;

  L90:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 26;
    }
  goto L254;

  L168:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == CCmode && GET_CODE (x3) == REG && XINT (x3, 0) == 0 && 1)
    goto L169;
  goto L254;

  L169:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    return 32;
  goto L254;

  L182:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == LTU && 1)
    goto L183;
  goto L254;

  L183:
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == CCmode && GET_CODE (x4) == REG && XINT (x4, 0) == 0 && 1)
    goto L184;
  goto L254;

  L184:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L185;
  goto L254;

  L185:
  x3 = XEXP (x2, 1);
  if (arith_operand (x3, SImode))
    {
      ro[1] = x3;
      return 34;
    }
  goto L254;

  L195:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == CCmode && GET_CODE (x3) == REG && XINT (x3, 0) == 0 && 1)
    goto L196;
  goto L254;

  L196:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    return 36;
  goto L254;

  L74:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L75;
    }
  goto L254;

  L75:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 25;
    }
  goto L254;

  L104:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != SImode)
    {
      goto L254;
    }
  switch (GET_CODE (x2))
    {
    case NE:
      goto L105;
    case EQ:
      goto L139;
    case LTU:
      goto L201;
    case GEU:
      goto L242;
    }
  goto L254;

  L105:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L106;
    }
  goto L254;

  L106:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L107;
  goto L254;

  L107:
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && register_operand (x2, SImode))
    {
      ro[2] = x2;
      *pnum_clobbers = 1;
      return 27;
    }
  goto L254;

  L139:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L140;
    }
  goto L254;

  L140:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L141;
  goto L254;

  L141:
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && register_operand (x2, SImode))
    {
      ro[2] = x2;
      *pnum_clobbers = 1;
      return 29;
    }
  goto L254;

  L201:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == CCmode && GET_CODE (x3) == REG && XINT (x3, 0) == 0 && 1)
    goto L202;
  goto L254;

  L202:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L203;
  goto L254;

  L203:
  x2 = XEXP (x1, 1);
  if (arith_operand (x2, SImode))
    {
      ro[1] = x2;
      return 37;
    }
  L210:
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L211;
  goto L254;

  L211:
  x3 = XEXP (x2, 0);
  if (arith_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L212;
    }
  goto L254;

  L212:
  x3 = XEXP (x2, 1);
  if (arith_operand (x3, SImode))
    {
      ro[2] = x3;
      return 38;
    }
  goto L254;

  L242:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == CCmode && GET_CODE (x3) == REG && XINT (x3, 0) == 0 && 1)
    goto L243;
  goto L254;

  L243:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L244;
  goto L254;

  L244:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SImode))
    {
      ro[1] = x2;
      return 42;
    }
  goto L254;

  L173:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != SImode)
    {
      goto L254;
    }
  switch (GET_CODE (x2))
    {
    case NEG:
      goto L174;
    case MINUS:
      goto L224;
    case SUBREG:
    case REG:
      if (register_operand (x2, SImode))
	{
	  ro[2] = x2;
	  goto L122;
	}
    }
  L216:
  if (register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L217;
    }
  goto L254;

  L174:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == LTU && 1)
    goto L175;
  goto L254;

  L175:
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == CCmode && GET_CODE (x4) == REG && XINT (x4, 0) == 0 && 1)
    goto L176;
  goto L254;

  L176:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L177;
  goto L254;

  L177:
  x2 = XEXP (x1, 1);
  if (arith_operand (x2, SImode))
    {
      ro[1] = x2;
      return 33;
    }
  goto L254;

  L224:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L225;
    }
  goto L254;

  L225:
  x3 = XEXP (x2, 1);
  if (arith_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L226;
    }
  goto L254;

  L226:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == LTU && 1)
    goto L227;
  goto L254;

  L227:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == CCmode && GET_CODE (x3) == REG && XINT (x3, 0) == 0 && 1)
    goto L228;
  goto L254;

  L228:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    return 40;
  goto L254;

  L122:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != SImode)
    {
      x2 = XEXP (x1, 0);
      goto L216;
    }
  switch (GET_CODE (x2))
    {
    case NE:
      goto L123;
    case EQ:
      goto L157;
    }
  x2 = XEXP (x1, 0);
  goto L216;

  L123:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L124;
    }
  x2 = XEXP (x1, 0);
  goto L216;

  L124:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 28;
    }
  x2 = XEXP (x1, 0);
  goto L216;

  L157:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L158;
    }
  x2 = XEXP (x1, 0);
  goto L216;

  L158:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && pnum_clobbers != 0 && 1)
    {
      *pnum_clobbers = 1;
      return 30;
    }
  x2 = XEXP (x1, 0);
  goto L216;

  L217:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != SImode)
    {
      goto L254;
    }
  switch (GET_CODE (x2))
    {
    case LTU:
      goto L218;
    case PLUS:
      goto L234;
    case GEU:
      goto L250;
    }
  goto L254;

  L218:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == CCmode && GET_CODE (x3) == REG && XINT (x3, 0) == 0 && 1)
    goto L219;
  goto L254;

  L219:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    return 39;
  goto L254;

  L234:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == LTU && 1)
    goto L235;
  goto L254;

  L235:
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == CCmode && GET_CODE (x4) == REG && XINT (x4, 0) == 0 && 1)
    goto L236;
  goto L254;

  L236:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L237;
  goto L254;

  L237:
  x3 = XEXP (x2, 1);
  if (arith_operand (x3, SImode))
    {
      ro[2] = x3;
      return 41;
    }
  goto L254;

  L250:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == CCmode && GET_CODE (x3) == REG && XINT (x3, 0) == 0 && 1)
    goto L251;
  goto L254;

  L251:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    return 43;
  goto L254;

  L162:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    goto L163;
  goto L254;

  L163:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 31;
  goto L254;

  L189:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    goto L190;
  goto L254;

  L190:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 35;
  goto L254;

  L255:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    goto L256;
  goto ret0;

  L256:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 44;
  goto ret0;
 ret0: return -1;
}

int
recog_2 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn;
     int *pnum_clobbers;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1, x2, x3, x4, x5;
  int tem;

  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) != SImode)
    goto ret0;
  switch (GET_CODE (x1))
    {
    case HIGH:
      goto L331;
    case LO_SUM:
      goto L344;
    case ZERO_EXTEND:
      goto L443;
    case SIGN_EXTEND:
      goto L488;
    case FIX:
      goto L544;
    case PLUS:
      goto L572;
    case MINUS:
      goto L610;
    case MULT:
      goto L635;
    case DIV:
      goto L687;
    case UDIV:
      goto L720;
    case AND:
      goto L743;
    case IOR:
      goto L773;
    case XOR:
      goto L803;
    case NOT:
      goto L831;
    case ASHIFT:
      goto L1065;
    case ASHIFTRT:
      goto L1090;
    case LSHIFTRT:
      goto L1095;
    }
  goto ret0;

  L331:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == UNSPEC && XINT (x2, 1) == 0 && XVECLEN (x2, 0) == 1 && 1)
    goto L332;
  L336:
  ro[1] = x2;
  if (check_pic (1))
    return 70;
  goto ret0;

  L332:
  x3 = XVECEXP (x2, 0, 0);
  ro[1] = x3;
  if (check_pic (1))
    return 69;
  goto L336;

  L344:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L345;
    }
  goto ret0;

  L345:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == UNSPEC && XINT (x2, 1) == 0 && XVECLEN (x2, 0) == 1 && 1)
    goto L346;
  if (immediate_operand (x2, SImode))
    {
      ro[2] = x2;
      return 73;
    }
  goto ret0;

  L346:
  x3 = XVECEXP (x2, 0, 0);
  if (immediate_operand (x3, SImode))
    {
      ro[2] = x3;
      return 72;
    }
  goto ret0;

  L443:
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case HImode:
      if (memory_operand (x2, HImode))
	{
	  ro[1] = x2;
	  return 95;
	}
      break;
    case QImode:
      if (sparc_operand (x2, QImode))
	{
	  ro[1] = x2;
	  if (GET_CODE (operands[1]) != CONST_INT)
	    return 99;
	  }
    }
  goto ret0;

  L488:
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case HImode:
      if (memory_operand (x2, HImode))
	{
	  ro[1] = x2;
	  return 105;
	}
      break;
    case QImode:
      if (memory_operand (x2, QImode))
	{
	  ro[1] = x2;
	  return 109;
	}
    }
  goto ret0;

  L544:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) != FIX)
    goto ret0;
  switch (GET_MODE (x2))
    {
    case SFmode:
      goto L545;
    case DFmode:
      goto L550;
    case TFmode:
      goto L555;
    }
  goto ret0;

  L545:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SFmode))
    {
      ro[1] = x3;
      if (TARGET_FPU)
	return 120;
      }
  goto ret0;

  L550:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, DFmode))
    {
      ro[1] = x3;
      if (TARGET_FPU)
	return 121;
      }
  goto ret0;

  L555:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, TFmode))
    {
      ro[1] = x3;
      if (TARGET_FPU)
	return 122;
      }
  goto ret0;

  L572:
  x2 = XEXP (x1, 0);
  if (arith_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L573;
    }
  goto ret0;

  L573:
  x2 = XEXP (x1, 1);
  if (arith_operand (x2, SImode))
    {
      ro[2] = x2;
      return 124;
    }
  goto ret0;

  L610:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L611;
    }
  goto ret0;

  L611:
  x2 = XEXP (x1, 1);
  if (arith_operand (x2, SImode))
    {
      ro[2] = x2;
      return 128;
    }
  goto ret0;

  L635:
  x2 = XEXP (x1, 0);
  if (arith_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L636;
    }
  goto ret0;

  L636:
  x2 = XEXP (x1, 1);
  if (arith_operand (x2, SImode))
    {
      ro[2] = x2;
      if (TARGET_V8 || TARGET_SPARCLITE)
	return 131;
      }
  goto ret0;

  L687:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L688;
    }
  goto ret0;

  L688:
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && arith_operand (x2, SImode))
    {
      ro[2] = x2;
      if (TARGET_V8)
	{
	  *pnum_clobbers = 1;
	  return 139;
	}
      }
  goto ret0;

  L720:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L721;
    }
  goto ret0;

  L721:
  x2 = XEXP (x1, 1);
  if (arith_operand (x2, SImode))
    {
      ro[2] = x2;
      if (TARGET_V8)
	return 141;
      }
  goto ret0;

  L743:
  x2 = XEXP (x1, 0);
  if (arith_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L744;
    }
  L762:
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NOT && 1)
    goto L763;
  goto ret0;

  L744:
  x2 = XEXP (x1, 1);
  if (arith_operand (x2, SImode))
    {
      ro[2] = x2;
      return 145;
    }
  x2 = XEXP (x1, 0);
  goto L762;

  L763:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L764;
    }
  goto ret0;

  L764:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SImode))
    {
      ro[2] = x2;
      return 148;
    }
  goto ret0;

  L773:
  x2 = XEXP (x1, 0);
  if (arith_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L774;
    }
  L792:
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NOT && 1)
    goto L793;
  goto ret0;

  L774:
  x2 = XEXP (x1, 1);
  if (arith_operand (x2, SImode))
    {
      ro[2] = x2;
      return 151;
    }
  x2 = XEXP (x1, 0);
  goto L792;

  L793:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L794;
    }
  goto ret0;

  L794:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SImode))
    {
      ro[2] = x2;
      return 154;
    }
  goto ret0;

  L803:
  x2 = XEXP (x1, 0);
  if (arith_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L804;
    }
  goto ret0;

  L804:
  x2 = XEXP (x1, 1);
  if (arith_operand (x2, SImode))
    {
      ro[2] = x2;
      return 157;
    }
  goto ret0;

  L831:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == XOR && 1)
    goto L832;
  L934:
  if (arith_operand (x2, SImode))
    {
      ro[1] = x2;
      return 174;
    }
  goto ret0;

  L832:
  x3 = XEXP (x2, 0);
  if (reg_or_0_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L833;
    }
  goto L934;

  L833:
  x3 = XEXP (x2, 1);
  if (arith_operand (x3, SImode))
    {
      ro[2] = x3;
      return 161;
    }
  goto L934;

  L1065:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L1066;
    }
  goto ret0;

  L1066:
  x2 = XEXP (x1, 1);
  if (shift_operand (x2, SImode))
    {
      ro[2] = x2;
      return 200;
    }
  goto ret0;

  L1090:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L1091;
    }
  goto ret0;

  L1091:
  x2 = XEXP (x1, 1);
  if (shift_operand (x2, SImode))
    {
      ro[2] = x2;
      return 203;
    }
  goto ret0;

  L1095:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L1096;
    }
  goto ret0;

  L1096:
  x2 = XEXP (x1, 1);
  if (shift_operand (x2, SImode))
    {
      ro[2] = x2;
      return 204;
    }
  goto ret0;
 ret0: return -1;
}

int
recog_3 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn;
     int *pnum_clobbers;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1, x2, x3, x4, x5;
  int tem;

  x1 = XEXP (x0, 0);
  switch (GET_MODE (x1))
    {
    case CCmode:
      switch (GET_CODE (x1))
	{
	case REG:
	  if (XINT (x1, 0) == 0 && 1)
	    goto L2;
	}
      break;
    case CCFPEmode:
      switch (GET_CODE (x1))
	{
	case REG:
	  if (XINT (x1, 0) == 0 && 1)
	    goto L7;
	}
      break;
    case CCFPmode:
      if (GET_CODE (x1) == REG && XINT (x1, 0) == 0 && 1)
	goto L22;
      break;
    case SImode:
      if (register_operand (x1, SImode))
	{
	  ro[0] = x1;
	  goto L45;
	}
    }
  if (GET_CODE (x1) == PC && 1)
    goto L259;
  L276:
  switch (GET_MODE (x1))
    {
    case QImode:
      if (reg_or_nonsymb_mem_operand (x1, QImode))
	{
	  ro[0] = x1;
	  goto L277;
	}
    L279:
      if (register_operand (x1, QImode))
	{
	  ro[0] = x1;
	  goto L280;
	}
      if (GET_CODE (x1) == MEM && 1)
	goto L293;
      break;
    case HImode:
      if (reg_or_nonsymb_mem_operand (x1, HImode))
	{
	  ro[0] = x1;
	  goto L297;
	}
    L299:
      if (register_operand (x1, HImode))
	{
	  ro[0] = x1;
	  goto L300;
	}
      if (GET_CODE (x1) == MEM && 1)
	goto L312;
      break;
    case SImode:
    L315:
      if (reg_or_nonsymb_mem_operand (x1, SImode))
	{
	  ro[0] = x1;
	  goto L316;
	}
    L329:
      if (register_operand (x1, SImode))
	{
	  ro[0] = x1;
	  goto L330;
	}
      break;
    case DImode:
      if (register_operand (x1, DImode))
	{
	  ro[0] = x1;
	  goto L326;
	}
    L364:
      if (reg_or_nonsymb_mem_operand (x1, DImode))
	{
	  ro[0] = x1;
	  goto L365;
	}
    }
  switch (GET_MODE (x1))
    {
    case SImode:
      if (GET_CODE (x1) == MEM && 1)
	goto L361;
    L907:
      if (general_operand (x1, SImode))
	{
	  ro[0] = x1;
	  goto L908;
	}
      break;
    case SFmode:
      if (general_operand (x1, SFmode))
	{
	  ro[0] = x1;
	  goto L373;
	}
    L375:
      if (reg_or_nonsymb_mem_operand (x1, SFmode))
	{
	  ro[0] = x1;
	  goto L376;
	}
    L518:
      if (register_operand (x1, SFmode))
	{
	  ro[0] = x1;
	  goto L519;
	}
      if (GET_CODE (x1) == MEM && 1)
	goto L391;
      break;
    case DFmode:
      if (general_operand (x1, DFmode))
	{
	  ro[0] = x1;
	  goto L395;
	}
    L397:
      if (reg_or_nonsymb_mem_operand (x1, DFmode))
	{
	  ro[0] = x1;
	  goto L398;
	}
    L506:
      if (register_operand (x1, DFmode))
	{
	  ro[0] = x1;
	  goto L507;
	}
      if (GET_CODE (x1) == MEM && 1)
	goto L416;
      break;
    case TFmode:
      if (general_operand (x1, TFmode))
	{
	  ro[0] = x1;
	  goto L420;
	}
    L422:
      if (reg_or_nonsymb_mem_operand (x1, TFmode))
	{
	  ro[0] = x1;
	  goto L423;
	}
    L510:
      if (register_operand (x1, TFmode))
	{
	  ro[0] = x1;
	  goto L511;
	}
      switch (GET_CODE (x1))
	{
	case MEM:
	  goto L438;
	}
      break;
    case CC_NOOVmode:
      switch (GET_CODE (x1))
	{
	case REG:
	  if (XINT (x1, 0) == 0 && 1)
	    goto L576;
	}
    }
  L1098:
  if (GET_CODE (x1) == PC && 1)
    goto L1099;
  L1186:
  ro[0] = x1;
  goto L1187;
  L1244:
  if (GET_CODE (x1) == PC && 1)
    goto L1245;
  if (register_operand (x1, SImode))
    {
      ro[0] = x1;
      goto L1261;
    }
  goto ret0;

  L2:
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CCmode && GET_CODE (x1) == COMPARE && 1)
    goto L455;
  x1 = XEXP (x0, 0);
  goto L1186;

  L455:
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case SImode:
      switch (GET_CODE (x2))
	{
	case ZERO_EXTEND:
	  goto L456;
	case ZERO_EXTRACT:
	  goto L501;
	case SUBREG:
	case REG:
	  if (register_operand (x2, SImode))
	    {
	      ro[0] = x2;
	      goto L4;
	    }
	}
    L837:
      if (cc_arithop (x2, SImode))
	{
	  ro[2] = x2;
	  goto L838;
	}
    L878:
      if (cc_arithopn (x2, SImode))
	{
	  ro[2] = x2;
	  goto L879;
	}
      break;
    case QImode:
      switch (GET_CODE (x2))
	{
	case SUBREG:
	  if (XINT (x2, 1) == 0 && 1)
	    goto L473;
	}
    }
  L855:
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NOT && 1)
    goto L856;
  x1 = XEXP (x0, 0);
  goto L1186;

  L456:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, QImode))
    {
      ro[0] = x3;
      goto L457;
    }
  goto L837;

  L457:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 100;
  x2 = XEXP (x1, 0);
  goto L837;

  L501:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L502;
    }
  goto L837;

  L502:
  x3 = XEXP (x2, 1);
  if (small_int (x3, SImode))
    {
      ro[1] = x3;
      goto L503;
    }
  goto L837;

  L503:
  x3 = XEXP (x2, 2);
  if (small_int (x3, SImode))
    {
      ro[2] = x3;
      goto L504;
    }
  goto L837;

  L504:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    if (INTVAL (operands[2]) > 19)
      return 110;
  x2 = XEXP (x1, 0);
  goto L837;

  L4:
  x2 = XEXP (x1, 1);
  if (arith_operand (x2, SImode))
    {
      ro[1] = x2;
      return 16;
    }
  x2 = XEXP (x1, 0);
  goto L837;

  L838:
  x3 = XEXP (x2, 0);
  if (arith_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L839;
    }
  goto L878;

  L839:
  x3 = XEXP (x2, 1);
  if (arith_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L840;
    }
  goto L878;

  L840:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 162;
  x2 = XEXP (x1, 0);
  goto L878;

  L879:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == NOT && 1)
    goto L880;
  goto L855;

  L880:
  x4 = XEXP (x3, 0);
  if (arith_operand (x4, SImode))
    {
      ro[0] = x4;
      goto L881;
    }
  goto L855;

  L881:
  x3 = XEXP (x2, 1);
  if (reg_or_0_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L882;
    }
  goto L855;

  L882:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 166;
  x2 = XEXP (x1, 0);
  goto L855;

  L473:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L474;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L474:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 102;
  x1 = XEXP (x0, 0);
  goto L1186;

  L856:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == XOR && 1)
    goto L857;
  L939:
  if (arith_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L940;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L857:
  x4 = XEXP (x3, 0);
  if (reg_or_0_operand (x4, SImode))
    {
      ro[0] = x4;
      goto L858;
    }
  goto L939;

  L858:
  x4 = XEXP (x3, 1);
  if (arith_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L859;
    }
  goto L939;

  L859:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 164;
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L939;

  L940:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 175;
  x1 = XEXP (x0, 0);
  goto L1186;

  L7:
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CCFPEmode && GET_CODE (x1) == COMPARE && 1)
    goto L8;
  x1 = XEXP (x0, 0);
  goto L1186;

  L8:
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case DFmode:
      if (register_operand (x2, DFmode))
	{
	  ro[0] = x2;
	  goto L9;
	}
      break;
    case SFmode:
      if (register_operand (x2, SFmode))
	{
	  ro[0] = x2;
	  goto L14;
	}
      break;
    case TFmode:
      if (register_operand (x2, TFmode))
	{
	  ro[0] = x2;
	  goto L19;
	}
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L9:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, DFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 17;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L14:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 18;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L19:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, TFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 19;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L22:
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CCFPmode && GET_CODE (x1) == COMPARE && 1)
    goto L23;
  x1 = XEXP (x0, 0);
  goto L1186;

  L23:
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case DFmode:
      if (register_operand (x2, DFmode))
	{
	  ro[0] = x2;
	  goto L24;
	}
      break;
    case SFmode:
      if (register_operand (x2, SFmode))
	{
	  ro[0] = x2;
	  goto L29;
	}
      break;
    case TFmode:
      if (register_operand (x2, TFmode))
	{
	  ro[0] = x2;
	  goto L34;
	}
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L24:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, DFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 20;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L29:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 21;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L34:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, TFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 22;
      }
  x1 = XEXP (x0, 0);
  goto L1186;
 L45:
  tem = recog_1 (x0, insn, pnum_clobbers);
  if (tem >= 0) return tem;
  x1 = XEXP (x0, 0);
  goto L315;

  L259:
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == IF_THEN_ELSE && 1)
    goto L260;
  x1 = XEXP (x0, 0);
  goto L276;

  L260:
  x2 = XEXP (x1, 0);
  if (noov_compare_op (x2, VOIDmode))
    {
      ro[0] = x2;
      goto L261;
    }
  x1 = XEXP (x0, 0);
  goto L276;

  L261:
  x3 = XEXP (x2, 0);
  if (GET_CODE (x3) == REG && XINT (x3, 0) == 0 && 1)
    goto L262;
  x1 = XEXP (x0, 0);
  goto L276;

  L262:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L263;
  x1 = XEXP (x0, 0);
  goto L276;

  L263:
  x2 = XEXP (x1, 1);
  switch (GET_CODE (x2))
    {
    case LABEL_REF:
      goto L264;
    case PC:
      goto L273;
    }
  x1 = XEXP (x0, 0);
  goto L276;

  L264:
  x3 = XEXP (x2, 0);
  ro[1] = x3;
  goto L265;

  L265:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == PC && 1)
    return 55;
  x1 = XEXP (x0, 0);
  goto L276;

  L273:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L274;
  x1 = XEXP (x0, 0);
  goto L276;

  L274:
  x3 = XEXP (x2, 0);
  ro[1] = x3;
  return 56;

  L277:
  x1 = XEXP (x0, 1);
  if (move_operand (x1, QImode))
    {
      ro[1] = x1;
      if (register_operand (operands[0], QImode)
   || register_operand (operands[1], QImode)
   || operands[1] == const0_rtx)
	return 58;
      }
  x1 = XEXP (x0, 0);
  goto L279;

  L280:
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == QImode && GET_CODE (x1) == SUBREG && XINT (x1, 1) == 0 && 1)
    goto L281;
  x1 = XEXP (x0, 0);
  goto L1186;

  L281:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == LO_SUM && 1)
    goto L282;
  x1 = XEXP (x0, 0);
  goto L1186;

  L282:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, QImode))
    {
      ro[1] = x3;
      goto L283;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L283:
  x3 = XEXP (x2, 1);
  if (immediate_operand (x3, VOIDmode))
    {
      ro[2] = x3;
      return 59;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L293:
  x2 = XEXP (x1, 0);
  if (symbolic_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L294;
    }
  goto L1186;

  L294:
  x1 = XEXP (x0, 1);
  if (pnum_clobbers != 0 && reg_or_0_operand (x1, QImode))
    {
      ro[1] = x1;
      *pnum_clobbers = 1;
      return 60;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L297:
  x1 = XEXP (x0, 1);
  if (move_operand (x1, HImode))
    {
      ro[1] = x1;
      if (register_operand (operands[0], HImode)
   || register_operand (operands[1], HImode)
   || operands[1] == const0_rtx)
	return 62;
      }
  x1 = XEXP (x0, 0);
  goto L299;

  L300:
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) != HImode)
    {
      x1 = XEXP (x0, 0);
      goto L1186;
    }
  switch (GET_CODE (x1))
    {
    case LO_SUM:
      goto L301;
    case HIGH:
      goto L340;
    case ZERO_EXTEND:
      goto L447;
    case SIGN_EXTEND:
      goto L492;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L301:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, HImode))
    {
      ro[1] = x2;
      goto L302;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L302:
  x2 = XEXP (x1, 1);
  if (immediate_operand (x2, VOIDmode))
    {
      ro[2] = x2;
      return 63;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L340:
  x2 = XEXP (x1, 0);
  ro[1] = x2;
  if (check_pic (1))
    return 71;
  x1 = XEXP (x0, 0);
  goto L1186;

  L447:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == QImode && sparc_operand (x2, QImode))
    {
      ro[1] = x2;
      if (GET_CODE (operands[1]) != CONST_INT)
	return 97;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L492:
  x2 = XEXP (x1, 0);
  if (memory_operand (x2, QImode))
    {
      ro[1] = x2;
      return 107;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L312:
  x2 = XEXP (x1, 0);
  if (symbolic_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L313;
    }
  goto L1186;

  L313:
  x1 = XEXP (x0, 1);
  if (pnum_clobbers != 0 && reg_or_0_operand (x1, HImode))
    {
      ro[1] = x1;
      *pnum_clobbers = 1;
      return 64;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L316:
  x1 = XEXP (x0, 1);
  if (move_operand (x1, SImode))
    {
      ro[1] = x1;
      if (register_operand (operands[0], SImode)
   || register_operand (operands[1], SImode)
   || operands[1] == const0_rtx)
	return 66;
      }
  x1 = XEXP (x0, 0);
  goto L329;
 L330:
  tem = recog_2 (x0, insn, pnum_clobbers);
  if (tem >= 0) return tem;
  x1 = XEXP (x0, 0);
  goto L907;

  L326:
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) != DImode)
    {
      x1 = XEXP (x0, 0);
      goto L364;
    }
  switch (GET_CODE (x1))
    {
    case HIGH:
      goto L327;
    case LO_SUM:
      goto L369;
    case PLUS:
      goto L567;
    case MINUS:
      goto L605;
    case MULT:
      goto L653;
    case AND:
      goto L738;
    case IOR:
      goto L768;
    case XOR:
      goto L798;
    case NOT:
      goto L825;
    case NEG:
      goto L905;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L327:
  x2 = XEXP (x1, 0);
  ro[1] = x2;
  if (check_pic (1))
    return 68;
  x1 = XEXP (x0, 0);
  goto L364;

  L369:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, DImode))
    {
      ro[1] = x2;
      goto L370;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L370:
  x2 = XEXP (x1, 1);
  if (immediate_operand (x2, DImode))
    {
      ro[2] = x2;
      return 77;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L567:
  x2 = XEXP (x1, 0);
  if (arith_double_operand (x2, DImode))
    {
      ro[1] = x2;
      goto L568;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L568:
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && arith_double_operand (x2, DImode))
    {
      ro[2] = x2;
      *pnum_clobbers = 1;
      return 123;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L605:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, DImode))
    {
      ro[1] = x2;
      goto L606;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L606:
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && arith_double_operand (x2, DImode))
    {
      ro[2] = x2;
      *pnum_clobbers = 1;
      return 127;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L653:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DImode)
    {
      x1 = XEXP (x0, 0);
      goto L364;
    }
  switch (GET_CODE (x2))
    {
    case SIGN_EXTEND:
      goto L654;
    case ZERO_EXTEND:
      goto L667;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L654:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L655;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L655:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode && GET_CODE (x2) == SIGN_EXTEND && 1)
    goto L656;
  L662:
  if (small_int (x2, SImode))
    {
      ro[2] = x2;
      if (TARGET_V8 || TARGET_SPARCLITE)
	return 135;
      }
  x1 = XEXP (x0, 0);
  goto L364;

  L656:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[2] = x3;
      if (TARGET_V8 || TARGET_SPARCLITE)
	return 134;
      }
  goto L662;

  L667:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L668;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L668:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DImode && GET_CODE (x2) == ZERO_EXTEND && 1)
    goto L669;
  L675:
  if (uns_small_int (x2, SImode))
    {
      ro[2] = x2;
      if (TARGET_V8 || TARGET_SPARCLITE)
	return 138;
      }
  x1 = XEXP (x0, 0);
  goto L364;

  L669:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[2] = x3;
      if (TARGET_V8 || TARGET_SPARCLITE)
	return 137;
      }
  goto L675;

  L738:
  x2 = XEXP (x1, 0);
  if (arith_double_operand (x2, DImode))
    {
      ro[1] = x2;
      goto L739;
    }
  L756:
  if (GET_MODE (x2) == DImode && GET_CODE (x2) == NOT && 1)
    goto L757;
  x1 = XEXP (x0, 0);
  goto L364;

  L739:
  x2 = XEXP (x1, 1);
  if (arith_double_operand (x2, DImode))
    {
      ro[2] = x2;
      return 144;
    }
  x2 = XEXP (x1, 0);
  goto L756;

  L757:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, DImode))
    {
      ro[1] = x3;
      goto L758;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L758:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, DImode))
    {
      ro[2] = x2;
      return 147;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L768:
  x2 = XEXP (x1, 0);
  if (arith_double_operand (x2, DImode))
    {
      ro[1] = x2;
      goto L769;
    }
  L786:
  if (GET_MODE (x2) == DImode && GET_CODE (x2) == NOT && 1)
    goto L787;
  x1 = XEXP (x0, 0);
  goto L364;

  L769:
  x2 = XEXP (x1, 1);
  if (arith_double_operand (x2, DImode))
    {
      ro[2] = x2;
      return 150;
    }
  x2 = XEXP (x1, 0);
  goto L786;

  L787:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, DImode))
    {
      ro[1] = x3;
      goto L788;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L788:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, DImode))
    {
      ro[2] = x2;
      return 153;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L798:
  x2 = XEXP (x1, 0);
  if (arith_double_operand (x2, DImode))
    {
      ro[1] = x2;
      goto L799;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L799:
  x2 = XEXP (x1, 1);
  if (arith_double_operand (x2, DImode))
    {
      ro[2] = x2;
      return 156;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L825:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == DImode && GET_CODE (x2) == XOR && 1)
    goto L826;
  L930:
  if (arith_double_operand (x2, DImode))
    {
      ro[1] = x2;
      return 173;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L826:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, DImode))
    {
      ro[1] = x3;
      goto L827;
    }
  goto L930;

  L827:
  x3 = XEXP (x2, 1);
  if (register_operand (x3, DImode))
    {
      ro[2] = x3;
      return 160;
    }
  goto L930;

  L905:
  x2 = XEXP (x1, 0);
  if (pnum_clobbers != 0 && register_operand (x2, DImode))
    {
      ro[1] = x2;
      *pnum_clobbers = 1;
      return 168;
    }
  x1 = XEXP (x0, 0);
  goto L364;

  L365:
  x1 = XEXP (x0, 1);
  if (general_operand (x1, DImode))
    {
      ro[1] = x1;
      if (register_operand (operands[0], DImode)
   || register_operand (operands[1], DImode)
   || operands[1] == const0_rtx)
	return 76;
      }
  x1 = XEXP (x0, 0);
  goto L1098;

  L361:
  x2 = XEXP (x1, 0);
  if (symbolic_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L362;
    }
  goto L907;

  L362:
  x1 = XEXP (x0, 1);
  if (pnum_clobbers != 0 && reg_or_0_operand (x1, SImode))
    {
      ro[1] = x1;
      *pnum_clobbers = 1;
      return 74;
    }
  x1 = XEXP (x0, 0);
  goto L907;

  L908:
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode && GET_CODE (x1) == NEG && 1)
    goto L909;
  x1 = XEXP (x0, 0);
  goto L1186;

  L909:
  x2 = XEXP (x1, 0);
  if (arith_operand (x2, SImode))
    {
      ro[1] = x2;
      return 169;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L373:
  x1 = XEXP (x0, 1);
  ro[1] = x1;
  if (TARGET_FPU && GET_CODE (operands[1]) == CONST_DOUBLE)
    return 78;
  x1 = XEXP (x0, 0);
  goto L375;

  L376:
  x1 = XEXP (x0, 1);
  if (reg_or_nonsymb_mem_operand (x1, SFmode))
    goto L380;
  x1 = XEXP (x0, 0);
  goto L518;

  L380:
  ro[1] = x1;
  if (TARGET_FPU
   && (register_operand (operands[0], SFmode)
       || register_operand (operands[1], SFmode)))
    return 80;
  L381:
  ro[1] = x1;
  if (! TARGET_FPU
   && (register_operand (operands[0], SFmode)
       || register_operand (operands[1], SFmode)))
    return 81;
  x1 = XEXP (x0, 0);
  goto L518;

  L519:
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) != SFmode)
    {
      x1 = XEXP (x0, 0);
      goto L1186;
    }
  switch (GET_CODE (x1))
    {
    case FLOAT_TRUNCATE:
      goto L520;
    case FLOAT:
      goto L532;
    case PLUS:
      goto L965;
    case MINUS:
      goto L980;
    case MULT:
      goto L995;
    case DIV:
      goto L1024;
    case NEG:
      goto L1037;
    case ABS:
      goto L1049;
    case SQRT:
      goto L1061;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L520:
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case DFmode:
      if (register_operand (x2, DFmode))
	{
	  ro[1] = x2;
	  if (TARGET_FPU)
	    return 114;
	  }
      break;
    case TFmode:
      if (register_operand (x2, TFmode))
	{
	  ro[1] = x2;
	  if (TARGET_FPU)
	    return 115;
	  }
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L532:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 117;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L965:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SFmode))
    {
      ro[1] = x2;
      goto L966;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L966:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SFmode))
    {
      ro[2] = x2;
      if (TARGET_FPU)
	return 179;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L980:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SFmode))
    {
      ro[1] = x2;
      goto L981;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L981:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SFmode))
    {
      ro[2] = x2;
      if (TARGET_FPU)
	return 182;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L995:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SFmode))
    {
      ro[1] = x2;
      goto L996;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L996:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SFmode))
    {
      ro[2] = x2;
      if (TARGET_FPU)
	return 185;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1024:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SFmode))
    {
      ro[1] = x2;
      goto L1025;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1025:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SFmode))
    {
      ro[2] = x2;
      if (TARGET_FPU)
	return 190;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1037:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 193;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1049:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 196;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1061:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 199;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L391:
  x2 = XEXP (x1, 0);
  if (symbolic_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L392;
    }
  goto L1186;

  L392:
  x1 = XEXP (x0, 1);
  if (pnum_clobbers != 0 && reg_or_0_operand (x1, SFmode))
    {
      ro[1] = x1;
      *pnum_clobbers = 1;
      return 82;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L395:
  x1 = XEXP (x0, 1);
  ro[1] = x1;
  if (TARGET_FPU && GET_CODE (operands[1]) == CONST_DOUBLE)
    return 83;
  x1 = XEXP (x0, 0);
  goto L397;

  L398:
  x1 = XEXP (x0, 1);
  if (reg_or_nonsymb_mem_operand (x1, DFmode))
    goto L402;
  x1 = XEXP (x0, 0);
  goto L506;

  L402:
  ro[1] = x1;
  if (TARGET_FPU
   && (register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode)))
    return 85;
  L403:
  ro[1] = x1;
  if (! TARGET_FPU
   && (register_operand (operands[0], DFmode)
       || register_operand (operands[1], DFmode)))
    return 86;
  x1 = XEXP (x0, 0);
  goto L506;

  L507:
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) != DFmode)
    {
      x1 = XEXP (x0, 0);
      goto L1186;
    }
  switch (GET_CODE (x1))
    {
    case FLOAT_EXTEND:
      goto L508;
    case FLOAT_TRUNCATE:
      goto L528;
    case FLOAT:
      goto L536;
    case PLUS:
      goto L960;
    case MINUS:
      goto L975;
    case MULT:
      goto L1000;
    case DIV:
      goto L1019;
    case NEG:
      goto L1033;
    case ABS:
      goto L1045;
    case SQRT:
      goto L1057;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L508:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 111;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L528:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, TFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 116;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L536:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 118;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L960:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, DFmode))
    {
      ro[1] = x2;
      goto L961;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L961:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, DFmode))
    {
      ro[2] = x2;
      if (TARGET_FPU)
	return 178;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L975:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, DFmode))
    {
      ro[1] = x2;
      goto L976;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L976:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, DFmode))
    {
      ro[2] = x2;
      if (TARGET_FPU)
	return 181;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1000:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != DFmode)
    {
      x1 = XEXP (x0, 0);
      goto L1186;
    }
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L1001;
  if (register_operand (x2, DFmode))
    {
      ro[1] = x2;
      goto L991;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1001:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SFmode))
    {
      ro[1] = x3;
      goto L1002;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1002:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == DFmode && GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L1003;
  x1 = XEXP (x0, 0);
  goto L1186;

  L1003:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SFmode))
    {
      ro[2] = x3;
      if (TARGET_V8 && TARGET_FPU)
	return 186;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L991:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, DFmode))
    {
      ro[2] = x2;
      if (TARGET_FPU)
	return 184;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1019:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, DFmode))
    {
      ro[1] = x2;
      goto L1020;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1020:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, DFmode))
    {
      ro[2] = x2;
      if (TARGET_FPU)
	return 189;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1033:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, DFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 192;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1045:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, DFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 195;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1057:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, DFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 198;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L416:
  x2 = XEXP (x1, 0);
  if (symbolic_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L417;
    }
  goto L1186;

  L417:
  x1 = XEXP (x0, 1);
  if (pnum_clobbers != 0 && reg_or_0_operand (x1, DFmode))
    {
      ro[1] = x1;
      *pnum_clobbers = 1;
      return 88;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L420:
  x1 = XEXP (x0, 1);
  ro[1] = x1;
  if (TARGET_FPU && GET_CODE (operands[1]) == CONST_DOUBLE)
    return 89;
  x1 = XEXP (x0, 0);
  goto L422;

  L423:
  x1 = XEXP (x0, 1);
  if (reg_or_nonsymb_mem_operand (x1, TFmode))
    goto L427;
  x1 = XEXP (x0, 0);
  goto L510;

  L427:
  ro[1] = x1;
  if (TARGET_FPU
   && (register_operand (operands[0], TFmode)
       || register_operand (operands[1], TFmode)))
    return 91;
  L428:
  ro[1] = x1;
  if (! TARGET_FPU
   && (register_operand (operands[0], TFmode)
       || register_operand (operands[1], TFmode)))
    return 92;
  x1 = XEXP (x0, 0);
  goto L510;

  L511:
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) != TFmode)
    {
      x1 = XEXP (x0, 0);
      goto L1186;
    }
  switch (GET_CODE (x1))
    {
    case FLOAT_EXTEND:
      goto L512;
    case FLOAT:
      goto L540;
    case PLUS:
      goto L955;
    case MINUS:
      goto L970;
    case MULT:
      goto L1007;
    case DIV:
      goto L1014;
    case NEG:
      goto L1029;
    case ABS:
      goto L1041;
    case SQRT:
      goto L1053;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L512:
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case SFmode:
      if (register_operand (x2, SFmode))
	{
	  ro[1] = x2;
	  if (TARGET_FPU)
	    return 112;
	  }
      break;
    case DFmode:
      if (register_operand (x2, DFmode))
	{
	  ro[1] = x2;
	  if (TARGET_FPU)
	    return 113;
	  }
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L540:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 119;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L955:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, TFmode))
    {
      ro[1] = x2;
      goto L956;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L956:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, TFmode))
    {
      ro[2] = x2;
      if (TARGET_FPU)
	return 177;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L970:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, TFmode))
    {
      ro[1] = x2;
      goto L971;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L971:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, TFmode))
    {
      ro[2] = x2;
      if (TARGET_FPU)
	return 180;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1007:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != TFmode)
    {
      x1 = XEXP (x0, 0);
      goto L1186;
    }
  if (GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L1008;
  if (register_operand (x2, TFmode))
    {
      ro[1] = x2;
      goto L986;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1008:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, DFmode))
    {
      ro[1] = x3;
      goto L1009;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1009:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == TFmode && GET_CODE (x2) == FLOAT_EXTEND && 1)
    goto L1010;
  x1 = XEXP (x0, 0);
  goto L1186;

  L1010:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, DFmode))
    {
      ro[2] = x3;
      if (TARGET_V8 && TARGET_FPU)
	return 187;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L986:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, TFmode))
    {
      ro[2] = x2;
      if (TARGET_FPU)
	return 183;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1014:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, TFmode))
    {
      ro[1] = x2;
      goto L1015;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1015:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, TFmode))
    {
      ro[2] = x2;
      if (TARGET_FPU)
	return 188;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1029:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, TFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 191;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1041:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, TFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 194;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1053:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, TFmode))
    {
      ro[1] = x2;
      if (TARGET_FPU)
	return 197;
      }
  x1 = XEXP (x0, 0);
  goto L1186;

  L438:
  x2 = XEXP (x1, 0);
  if (symbolic_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L439;
    }
  goto L1186;

  L439:
  x1 = XEXP (x0, 1);
  if (pnum_clobbers != 0 && reg_or_0_operand (x1, TFmode))
    {
      ro[1] = x1;
      *pnum_clobbers = 1;
      return 93;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L576:
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == CC_NOOVmode && GET_CODE (x1) == COMPARE && 1)
    goto L577;
  x1 = XEXP (x0, 0);
  goto L1186;

  L577:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) != SImode)
    {
      x1 = XEXP (x0, 0);
      goto L1186;
    }
  switch (GET_CODE (x2))
    {
    case PLUS:
      goto L578;
    case MINUS:
      goto L616;
    case NEG:
      goto L914;
    case ASHIFT:
      goto L1071;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L578:
  x3 = XEXP (x2, 0);
  if (arith_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L579;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L579:
  x3 = XEXP (x2, 1);
  if (arith_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L580;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L580:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 125;
  x1 = XEXP (x0, 0);
  goto L1186;

  L616:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L617;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L617:
  x3 = XEXP (x2, 1);
  if (arith_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L618;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L618:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 129;
  x1 = XEXP (x0, 0);
  goto L1186;

  L914:
  x3 = XEXP (x2, 0);
  if (arith_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L915;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L915:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 170;
  x1 = XEXP (x0, 0);
  goto L1186;

  L1071:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L1072;
    }
  x1 = XEXP (x0, 0);
  goto L1186;

  L1072:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 1 && 1)
    goto L1073;
  x1 = XEXP (x0, 0);
  goto L1186;

  L1073:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    return 201;
  x1 = XEXP (x0, 0);
  goto L1186;

  L1099:
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == LABEL_REF && 1)
    goto L1100;
  x1 = XEXP (x0, 0);
  goto L1186;

  L1100:
  x2 = XEXP (x1, 0);
  ro[0] = x2;
  return 205;

  L1187:
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == CALL && 1)
    goto L1188;
  x1 = XEXP (x0, 0);
  goto L1244;

  L1188:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MEM && 1)
    goto L1189;
  x1 = XEXP (x0, 0);
  goto L1244;

  L1189:
  x3 = XEXP (x2, 0);
  if (address_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1190;
    }
  L1204:
  if (immediate_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1205;
    }
  x1 = XEXP (x0, 0);
  goto L1244;

  L1190:
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && 1)
    {
      ro[2] = x2;
      *pnum_clobbers = 1;
      return 216;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1204;

  L1205:
  x2 = XEXP (x1, 1);
  if (pnum_clobbers != 0 && 1)
    {
      ro[2] = x2;
      *pnum_clobbers = 1;
      return 217;
    }
  x1 = XEXP (x0, 0);
  goto L1244;

  L1245:
  x1 = XEXP (x0, 1);
  if (address_operand (x1, SImode))
    {
      ro[0] = x1;
      return 225;
    }
  goto ret0;

  L1261:
  x1 = XEXP (x0, 1);
  if (GET_MODE (x1) == SImode && GET_CODE (x1) == FFS && 1)
    goto L1262;
  goto ret0;

  L1262:
  x2 = XEXP (x1, 0);
  if (pnum_clobbers != 0 && register_operand (x2, SImode))
    {
      ro[1] = x2;
      if (TARGET_SPARCLITE)
	{
	  *pnum_clobbers = 1;
	  return 230;
	}
      }
  goto ret0;
 ret0: return -1;
}

int
recog_4 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn;
     int *pnum_clobbers;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1, x2, x3, x4, x5;
  int tem;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  switch (GET_MODE (x2))
    {
    case SImode:
      switch (GET_CODE (x2))
	{
	case NE:
	  goto L39;
	case NEG:
	  goto L52;
	case EQ:
	  goto L67;
	case PLUS:
	  goto L95;
	case MINUS:
	  goto L112;
	case MULT:
	  goto L641;
	case DIV:
	  goto L680;
	case UDIV:
	  goto L726;
	}
    }
  L320:
  if (move_pic_label (x2, SImode))
    {
      ro[1] = x2;
      goto L321;
    }
  goto ret0;

  L39:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L40;
    }
  goto L320;

  L40:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L41;
  goto L320;

  L41:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L42;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L42:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return 23;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L52:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    {
      goto L320;
    }
  switch (GET_CODE (x3))
    {
    case NE:
      goto L53;
    case EQ:
      goto L81;
    }
  goto L320;

  L53:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L54;
    }
  goto L320;

  L54:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L55;
  goto L320;

  L55:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L56;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L56:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return 24;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L81:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L82;
    }
  goto L320;

  L82:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L83;
  goto L320;

  L83:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L84;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L84:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return 26;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L67:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L68;
    }
  goto L320;

  L68:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L69;
  goto L320;

  L69:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L70;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L70:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return 25;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L95:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    {
      goto L320;
    }
  switch (GET_CODE (x3))
    {
    case NE:
      goto L96;
    case EQ:
      goto L130;
    }
  goto L320;

  L96:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L97;
    }
  goto L320;

  L97:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L98;
  goto L320;

  L98:
  x3 = XEXP (x2, 1);
  if (register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L99;
    }
  goto L320;

  L99:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L100;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L100:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return 27;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L130:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L131;
    }
  goto L320;

  L131:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L132;
  goto L320;

  L132:
  x3 = XEXP (x2, 1);
  if (register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L133;
    }
  goto L320;

  L133:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L134;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L134:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return 29;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L112:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L113;
    }
  goto L320;

  L113:
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) != SImode)
    {
      goto L320;
    }
  switch (GET_CODE (x3))
    {
    case NE:
      goto L114;
    case EQ:
      goto L148;
    }
  goto L320;

  L114:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L115;
    }
  goto L320;

  L115:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L116;
  goto L320;

  L116:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L117;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L117:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return 28;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L148:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L149;
    }
  goto L320;

  L149:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L150;
  goto L320;

  L150:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L151;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L151:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return 30;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L641:
  x3 = XEXP (x2, 0);
  if (arith_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L642;
    }
  goto L320;

  L642:
  x3 = XEXP (x2, 1);
  if (arith_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L643;
    }
  goto L320;

  L643:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L644;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L644:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CC_NOOVmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    goto L645;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L645:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CC_NOOVmode && GET_CODE (x2) == COMPARE && 1)
    goto L646;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L646:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == MULT && 1)
    goto L647;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L647:
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[1]) && 1)
    goto L648;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L648:
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[2]) && 1)
    goto L649;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L649:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    if (TARGET_V8 || TARGET_SPARCLITE)
      return 132;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L680:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L681;
    }
  goto L320;

  L681:
  x3 = XEXP (x2, 1);
  if (arith_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L682;
    }
  goto L320;

  L682:
  x1 = XVECEXP (x0, 0, 1);
  switch (GET_CODE (x1))
    {
    case CLOBBER:
      goto L683;
    case SET:
      goto L711;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L683:
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[3] = x2;
      if (TARGET_V8)
	return 139;
      }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L711:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    goto L712;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L712:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == COMPARE && 1)
    goto L713;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L713:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == DIV && 1)
    goto L714;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L714:
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[1]) && 1)
    goto L715;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L715:
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[2]) && 1)
    goto L716;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L716:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && pnum_clobbers != 0 && 1)
    if (TARGET_V8)
      {
	*pnum_clobbers = 1;
	return 140;
      }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L726:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L727;
    }
  goto L320;

  L727:
  x3 = XEXP (x2, 1);
  if (arith_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L728;
    }
  goto L320;

  L728:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L729;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L729:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    goto L730;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L730:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == COMPARE && 1)
    goto L731;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L731:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == UDIV && 1)
    goto L732;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L732:
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[1]) && 1)
    goto L733;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L733:
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[2]) && 1)
    goto L734;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L734:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    if (TARGET_V8)
      return 142;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L320;

  L321:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L322;
  goto ret0;

  L322:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 15 && 1)
    goto L323;
  goto ret0;

  L323:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == PC && 1)
    return 67;
  goto ret0;
 ret0: return -1;
}

int
recog_5 (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn;
     int *pnum_clobbers;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1, x2, x3, x4, x5;
  int tem;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  switch (GET_MODE (x2))
    {
    case SImode:
      if (register_operand (x2, SImode))
	{
	  ro[0] = x2;
	  goto L38;
	}
      break;
    case QImode:
      switch (GET_CODE (x2))
	{
	case MEM:
	  goto L287;
	}
      break;
    case HImode:
      switch (GET_CODE (x2))
	{
	case MEM:
	  goto L306;
	}
    }
  switch (GET_MODE (x2))
    {
    case SImode:
      switch (GET_CODE (x2))
	{
	case MEM:
	  goto L355;
	}
      break;
    case SFmode:
      switch (GET_CODE (x2))
	{
	case MEM:
	  goto L385;
	}
      break;
    case DFmode:
      switch (GET_CODE (x2))
	{
	case MEM:
	  goto L410;
	}
      break;
    case TFmode:
      switch (GET_CODE (x2))
	{
	case MEM:
	  goto L432;
	}
      break;
    case CCmode:
      if (GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
	goto L461;
      break;
    case DImode:
      if (register_operand (x2, DImode))
	{
	  ro[0] = x2;
	  goto L559;
	}
      break;
    case CC_NOOVmode:
      switch (GET_CODE (x2))
	{
	case REG:
	  if (XINT (x2, 0) == 0 && 1)
	    goto L584;
	}
    }
  if (GET_CODE (x2) == PC && 1)
    goto L1113;
  L1178:
  ro[0] = x2;
  goto L1179;
  L1254:
  switch (GET_MODE (x2))
    {
    case SImode:
      if (register_operand (x2, SImode))
	{
	  ro[0] = x2;
	  goto L1255;
	}
    L1380:
      if (restore_operand (x2, SImode))
	{
	  ro[0] = x2;
	  goto L1381;
	}
      break;
    case QImode:
      if (restore_operand (x2, QImode))
	{
	  ro[0] = x2;
	  goto L1371;
	}
      break;
    case HImode:
      if (restore_operand (x2, HImode))
	{
	  ro[0] = x2;
	  goto L1376;
	}
      break;
    case SFmode:
      if (restore_operand (x2, SFmode))
	{
	  ro[0] = x2;
	  goto L1386;
	}
    L1397:
      if (GET_CODE (x2) == REG && XINT (x2, 0) == 32 && 1)
	goto L1398;
    }
  goto ret0;
 L38:
  tem = recog_4 (x0, insn, pnum_clobbers);
  if (tem >= 0) return tem;
  x2 = XEXP (x1, 0);
  goto L1178;

  L287:
  x3 = XEXP (x2, 0);
  if (symbolic_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L288;
    }
  goto L1178;

  L288:
  x2 = XEXP (x1, 1);
  if (reg_or_0_operand (x2, QImode))
    {
      ro[1] = x2;
      goto L289;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L289:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L290;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L290:
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[2] = x2;
      return 60;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L306:
  x3 = XEXP (x2, 0);
  if (symbolic_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L307;
    }
  goto L1178;

  L307:
  x2 = XEXP (x1, 1);
  if (reg_or_0_operand (x2, HImode))
    {
      ro[1] = x2;
      goto L308;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L308:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L309;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L309:
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[2] = x2;
      return 64;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L355:
  x3 = XEXP (x2, 0);
  if (symbolic_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L356;
    }
  goto L1178;

  L356:
  x2 = XEXP (x1, 1);
  if (reg_or_0_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L357;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L357:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L358;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L358:
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[2] = x2;
      return 74;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L385:
  x3 = XEXP (x2, 0);
  if (symbolic_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L386;
    }
  goto L1178;

  L386:
  x2 = XEXP (x1, 1);
  if (reg_or_0_operand (x2, SFmode))
    {
      ro[1] = x2;
      goto L387;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L387:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L388;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L388:
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[2] = x2;
      return 82;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L410:
  x3 = XEXP (x2, 0);
  if (symbolic_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L411;
    }
  goto L1178;

  L411:
  x2 = XEXP (x1, 1);
  if (reg_or_0_operand (x2, DFmode))
    {
      ro[1] = x2;
      goto L412;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L412:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L413;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L413:
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[2] = x2;
      return 88;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L432:
  x3 = XEXP (x2, 0);
  if (symbolic_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L433;
    }
  goto L1178;

  L433:
  x2 = XEXP (x1, 1);
  if (reg_or_0_operand (x2, TFmode))
    {
      ro[1] = x2;
      goto L434;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L434:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L435;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L435:
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[2] = x2;
      return 93;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L461:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == COMPARE && 1)
    goto L462;
  x2 = XEXP (x1, 0);
  goto L1178;

  L462:
  x3 = XEXP (x2, 0);
  switch (GET_MODE (x3))
    {
    case SImode:
      switch (GET_CODE (x3))
	{
	case ZERO_EXTEND:
	  goto L463;
	case NOT:
	  goto L865;
	}
    L845:
      if (cc_arithop (x3, SImode))
	{
	  ro[3] = x3;
	  goto L846;
	}
    L887:
      if (cc_arithopn (x3, SImode))
	{
	  ro[3] = x3;
	  goto L888;
	}
      break;
    case QImode:
      if (GET_CODE (x3) == SUBREG && XINT (x3, 1) == 0 && 1)
	goto L480;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L463:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, QImode))
    {
      ro[1] = x4;
      goto L464;
    }
  goto L845;

  L464:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L465;
  x3 = XEXP (x2, 0);
  goto L845;

  L465:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L466;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L845;

  L466:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L467;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L845;

  L467:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == ZERO_EXTEND && 1)
    goto L468;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L845;

  L468:
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    return 101;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L845;

  L865:
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode && GET_CODE (x4) == XOR && 1)
    goto L866;
  L946:
  if (arith_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L947;
    }
  goto L845;

  L866:
  x5 = XEXP (x4, 0);
  if (reg_or_0_operand (x5, SImode))
    {
      ro[1] = x5;
      goto L867;
    }
  goto L946;

  L867:
  x5 = XEXP (x4, 1);
  if (arith_operand (x5, SImode))
    {
      ro[2] = x5;
      goto L868;
    }
  goto L946;

  L868:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L869;
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L946;

  L869:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L870;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L946;

  L870:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L871;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L946;

  L871:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NOT && 1)
    goto L872;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L946;

  L872:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == XOR && 1)
    goto L873;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L946;

  L873:
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[1]) && 1)
    goto L874;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L946;

  L874:
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[2]) && 1)
    return 165;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L946;

  L947:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L948;
  x3 = XEXP (x2, 0);
  goto L845;

  L948:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L949;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L845;

  L949:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L950;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L845;

  L950:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NOT && 1)
    goto L951;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L845;

  L951:
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    return 176;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L845;

  L846:
  x4 = XEXP (x3, 0);
  if (arith_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L847;
    }
  goto L887;

  L847:
  x4 = XEXP (x3, 1);
  if (arith_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L848;
    }
  goto L887;

  L848:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L849;
  x3 = XEXP (x2, 0);
  goto L887;

  L849:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L850;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L887;

  L850:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L851;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L887;

  L851:
  x2 = XEXP (x1, 1);
  if (rtx_equal_p (x2, ro[3]) && 1)
    return 163;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  goto L887;

  L888:
  x4 = XEXP (x3, 0);
  if (GET_MODE (x4) == SImode && GET_CODE (x4) == NOT && 1)
    goto L889;
  x2 = XEXP (x1, 0);
  goto L1178;

  L889:
  x5 = XEXP (x4, 0);
  if (arith_operand (x5, SImode))
    {
      ro[1] = x5;
      goto L890;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L890:
  x4 = XEXP (x3, 1);
  if (reg_or_0_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L891;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L891:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L892;
  x2 = XEXP (x1, 0);
  goto L1178;

  L892:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L893;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L893:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L894;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L894:
  x2 = XEXP (x1, 1);
  if (rtx_equal_p (x2, ro[3]) && 1)
    return 167;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L480:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L481;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L481:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L482;
  x2 = XEXP (x1, 0);
  goto L1178;

  L482:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L483;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L483:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, QImode))
    {
      ro[0] = x2;
      goto L484;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L484:
  x2 = XEXP (x1, 1);
  if (rtx_equal_p (x2, ro[1]) && 1)
    return 103;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L559:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != DImode)
    {
      x2 = XEXP (x1, 0);
      goto L1178;
    }
  switch (GET_CODE (x2))
    {
    case PLUS:
      goto L560;
    case MINUS:
      goto L598;
    case NEG:
      goto L899;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L560:
  x3 = XEXP (x2, 0);
  if (arith_double_operand (x3, DImode))
    {
      ro[1] = x3;
      goto L561;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L561:
  x3 = XEXP (x2, 1);
  if (arith_double_operand (x3, DImode))
    {
      ro[2] = x3;
      goto L562;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L562:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L563;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L563:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return 123;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L598:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, DImode))
    {
      ro[1] = x3;
      goto L599;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L599:
  x3 = XEXP (x2, 1);
  if (arith_double_operand (x3, DImode))
    {
      ro[2] = x3;
      goto L600;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L600:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L601;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L601:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return 127;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L899:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, DImode))
    {
      ro[1] = x3;
      goto L900;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L900:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L901;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L901:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return 168;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L584:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CC_NOOVmode && GET_CODE (x2) == COMPARE && 1)
    goto L585;
  x2 = XEXP (x1, 0);
  goto L1178;

  L585:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    {
      x2 = XEXP (x1, 0);
      goto L1178;
    }
  switch (GET_CODE (x3))
    {
    case PLUS:
      goto L586;
    case MINUS:
      goto L624;
    case NEG:
      goto L921;
    case ASHIFT:
      goto L1079;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L586:
  x4 = XEXP (x3, 0);
  if (arith_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L587;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L587:
  x4 = XEXP (x3, 1);
  if (arith_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L588;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L588:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L589;
  x2 = XEXP (x1, 0);
  goto L1178;

  L589:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L590;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L590:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L591;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L591:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L592;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L592:
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L593;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L593:
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    return 126;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L624:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L625;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L625:
  x4 = XEXP (x3, 1);
  if (arith_operand (x4, SImode))
    {
      ro[2] = x4;
      goto L626;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L626:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L627;
  x2 = XEXP (x1, 0);
  goto L1178;

  L627:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L628;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L628:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L629;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L629:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MINUS && 1)
    goto L630;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L630:
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L631;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L631:
  x3 = XEXP (x2, 1);
  if (rtx_equal_p (x3, ro[2]) && 1)
    return 130;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L921:
  x4 = XEXP (x3, 0);
  if (arith_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L922;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L922:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L923;
  x2 = XEXP (x1, 0);
  goto L1178;

  L923:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L924;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L924:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L925;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L925:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == NEG && 1)
    goto L926;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L926:
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    return 171;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L1079:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1080;
    }
  x2 = XEXP (x1, 0);
  goto L1178;

  L1080:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 1 && 1)
    goto L1081;
  x2 = XEXP (x1, 0);
  goto L1178;

  L1081:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1082;
  x2 = XEXP (x1, 0);
  goto L1178;

  L1082:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L1083;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L1083:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L1084;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L1084:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == ASHIFT && 1)
    goto L1085;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L1085:
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[1]) && 1)
    goto L1086;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L1086:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 1 && 1)
    return 202;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L1113:
  x2 = XEXP (x1, 1);
  if (address_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L1114;
    }
  L1120:
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L1121;
  x2 = XEXP (x1, 0);
  goto L1178;

  L1114:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE && 1)
    goto L1115;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L1120;

  L1115:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L1116;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L1120;

  L1116:
  x3 = XEXP (x2, 0);
  ro[1] = x3;
  return 208;

  L1121:
  x3 = XEXP (x2, 0);
  ro[0] = x3;
  goto L1122;

  L1122:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L1123;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L1123:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 15 && 1)
    goto L1124;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L1124:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L1125;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L1125:
  x3 = XEXP (x2, 0);
  if (rtx_equal_p (x3, ro[0]) && 1)
    return 209;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1178;

  L1179:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CALL && 1)
    goto L1180;
  x2 = XEXP (x1, 0);
  goto L1254;

  L1180:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == MEM && 1)
    goto L1181;
  x2 = XEXP (x1, 0);
  goto L1254;

  L1181:
  x4 = XEXP (x3, 0);
  if (address_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1182;
    }
  L1196:
  if (immediate_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1197;
    }
  x2 = XEXP (x1, 0);
  goto L1254;

  L1182:
  x3 = XEXP (x2, 1);
  ro[2] = x3;
  goto L1183;

  L1183:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1184;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L1196;

  L1184:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 15 && 1)
    return 216;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  x3 = XEXP (x2, 0);
  x4 = XEXP (x3, 0);
  goto L1196;

  L1197:
  x3 = XEXP (x2, 1);
  ro[2] = x3;
  goto L1198;

  L1198:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1199;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1254;

  L1199:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 15 && 1)
    return 217;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1254;

  L1255:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == FFS && 1)
    goto L1256;
  x2 = XEXP (x1, 0);
  goto L1380;

  L1256:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1257;
    }
  x2 = XEXP (x1, 0);
  goto L1380;

  L1257:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1258;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1380;

  L1258:
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[2] = x2;
      if (TARGET_SPARCLITE)
	return 230;
      }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1380;

  L1381:
  x2 = XEXP (x1, 1);
  if (arith_operand (x2, SImode))
    {
      ro[1] = x2;
      goto L1382;
    }
  L1391:
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == PLUS && 1)
    goto L1392;
  goto ret0;

  L1382:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == RETURN && 1)
    if (! TARGET_EPILOGUE)
      return 262;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  goto L1391;

  L1392:
  x3 = XEXP (x2, 0);
  if (arith_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1393;
    }
  goto ret0;

  L1393:
  x3 = XEXP (x2, 1);
  if (arith_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L1394;
    }
  goto ret0;

  L1394:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == RETURN && 1)
    if (! TARGET_EPILOGUE)
      return 264;
  goto ret0;

  L1371:
  x2 = XEXP (x1, 1);
  if (arith_operand (x2, QImode))
    {
      ro[1] = x2;
      goto L1372;
    }
  goto ret0;

  L1372:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == RETURN && 1)
    if (! TARGET_EPILOGUE)
      return 260;
  goto ret0;

  L1376:
  x2 = XEXP (x1, 1);
  if (arith_operand (x2, HImode))
    {
      ro[1] = x2;
      goto L1377;
    }
  goto ret0;

  L1377:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == RETURN && 1)
    if (! TARGET_EPILOGUE)
      return 261;
  goto ret0;

  L1386:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SFmode))
    {
      ro[1] = x2;
      goto L1387;
    }
  x2 = XEXP (x1, 0);
  goto L1397;

  L1387:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == RETURN && 1)
    if (! TARGET_FPU && ! TARGET_EPILOGUE)
      return 263;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  goto L1397;

  L1398:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SFmode))
    {
      ro[0] = x2;
      goto L1399;
    }
  goto ret0;

  L1399:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == RETURN && 1)
    if (! TARGET_EPILOGUE)
      return 265;
  goto ret0;
 ret0: return -1;
}

int
recog (x0, insn, pnum_clobbers)
     register rtx x0;
     rtx insn;
     int *pnum_clobbers;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1, x2, x3, x4, x5;
  int tem;

  L1238:
  switch (GET_CODE (x0))
    {
    case UNSPEC:
      if (GET_MODE (x0) == SImode && XINT (x0, 1) == 0 && XVECLEN (x0, 0) == 2 && 1)
	goto L1239;
      break;
    case SET:
      goto L1;
    case PARALLEL:
      if (XVECLEN (x0, 0) == 2 && 1)
	goto L36;
      if (XVECLEN (x0, 0) == 3 && 1)
	goto L690;
      if (XVECLEN (x0, 0) == 4 && 1)
	goto L1207;
      break;
    case CALL:
      goto L1134;
    case RETURN:
      if (! TARGET_EPILOGUE)
	return 223;
      break;
    case CONST_INT:
      if (XWINT (x0, 0) == 0 && 1)
	return 224;
      break;
    case UNSPEC_VOLATILE:
      if (XINT (x0, 1) == 0 && XVECLEN (x0, 0) == 1 && 1)
	goto L1247;
      if (XINT (x0, 1) == 1 && XVECLEN (x0, 0) == 1 && 1)
	goto L1249;
      if (XINT (x0, 1) == 2 && XVECLEN (x0, 0) == 1 && 1)
	goto L1251;
    }
  goto ret0;

  L1239:
  x1 = XVECEXP (x0, 0, 0);
  if (register_operand (x1, SImode))
    {
      ro[0] = x1;
      goto L1240;
    }
  goto ret0;

  L1240:
  x1 = XVECEXP (x0, 0, 1);
  if (register_operand (x1, SImode))
    {
      ro[1] = x1;
      return 222;
    }
  goto ret0;
 L1:
  return recog_3 (x0, insn, pnum_clobbers);

  L36:
  x1 = XVECEXP (x0, 0, 0);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L37;
    case CALL:
      goto L1128;
    }
  goto ret0;
 L37:
  return recog_5 (x0, insn, pnum_clobbers);

  L1128:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MEM && 1)
    goto L1129;
  goto ret0;

  L1129:
  x3 = XEXP (x2, 0);
  if (address_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L1130;
    }
  L1140:
  if (immediate_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L1141;
    }
  L1159:
  if (address_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L1160;
    }
  L1173:
  if (immediate_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L1174;
    }
  goto ret0;

  L1130:
  x2 = XEXP (x1, 1);
  ro[1] = x2;
  goto L1131;

  L1131:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1132;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1140;

  L1132:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 15 && 1)
    return 211;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1140;

  L1141:
  x2 = XEXP (x1, 1);
  ro[1] = x2;
  goto L1142;

  L1142:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1143;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1159;

  L1143:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 15 && 1)
    return 212;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1159;

  L1160:
  x2 = XEXP (x1, 1);
  ro[1] = x2;
  goto L1161;

  L1161:
  x1 = XVECEXP (x0, 0, 1);
  if (pnum_clobbers != 0 && immediate_operand (x1, VOIDmode))
    {
      ro[2] = x1;
      if (GET_CODE (operands[2]) == CONST_INT && INTVAL (operands[2]) > 0)
	{
	  *pnum_clobbers = 1;
	  return 213;
	}
      }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1173;

  L1174:
  x2 = XEXP (x1, 1);
  ro[1] = x2;
  goto L1175;

  L1175:
  x1 = XVECEXP (x0, 0, 1);
  if (pnum_clobbers != 0 && immediate_operand (x1, VOIDmode))
    {
      ro[2] = x1;
      if (GET_CODE (operands[2]) == CONST_INT && INTVAL (operands[2]) > 0)
	{
	  *pnum_clobbers = 1;
	  return 214;
	}
      }
  goto ret0;

  L690:
  x1 = XVECEXP (x0, 0, 0);
  switch (GET_CODE (x1))
    {
    case SET:
      goto L691;
    case CALL:
      goto L1150;
    }
  goto ret0;

  L691:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L692;
    }
  if (GET_CODE (x2) == PC && 1)
    goto L1104;
  goto ret0;

  L692:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == DIV && 1)
    goto L693;
  goto ret0;

  L693:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L694;
    }
  goto ret0;

  L694:
  x3 = XEXP (x2, 1);
  if (arith_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L695;
    }
  goto ret0;

  L695:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == SET && 1)
    goto L696;
  goto ret0;

  L696:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    goto L697;
  goto ret0;

  L697:
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == COMPARE && 1)
    goto L698;
  goto ret0;

  L698:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == DIV && 1)
    goto L699;
  goto ret0;

  L699:
  x4 = XEXP (x3, 0);
  if (rtx_equal_p (x4, ro[1]) && 1)
    goto L700;
  goto ret0;

  L700:
  x4 = XEXP (x3, 1);
  if (rtx_equal_p (x4, ro[2]) && 1)
    goto L701;
  goto ret0;

  L701:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L702;
  goto ret0;

  L702:
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L703;
  goto ret0;

  L703:
  x2 = XEXP (x1, 0);
  if (scratch_operand (x2, SImode))
    {
      ro[3] = x2;
      if (TARGET_V8)
	return 140;
      }
  goto ret0;

  L1104:
  x2 = XEXP (x1, 1);
  if (register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L1105;
    }
  goto ret0;

  L1105:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE && 1)
    goto L1106;
  goto ret0;

  L1106:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L1107;
  goto ret0;

  L1107:
  x3 = XEXP (x2, 0);
  ro[1] = x3;
  goto L1108;

  L1108:
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == USE && 1)
    goto L1109;
  goto ret0;

  L1109:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 15 && 1)
    return 207;
  goto ret0;

  L1150:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MEM && 1)
    goto L1151;
  goto ret0;

  L1151:
  x3 = XEXP (x2, 0);
  if (address_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L1152;
    }
  L1165:
  if (immediate_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L1166;
    }
  L1218:
  if (address_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L1219;
    }
  L1234:
  if (immediate_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L1235;
    }
  goto ret0;

  L1152:
  x2 = XEXP (x1, 1);
  ro[1] = x2;
  goto L1153;

  L1153:
  x1 = XVECEXP (x0, 0, 1);
  if (immediate_operand (x1, VOIDmode))
    {
      ro[2] = x1;
      goto L1154;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1165;

  L1154:
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1155;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1165;

  L1155:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 15 && 1)
    if (GET_CODE (operands[2]) == CONST_INT && INTVAL (operands[2]) > 0)
      return 213;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1165;

  L1166:
  x2 = XEXP (x1, 1);
  ro[1] = x2;
  goto L1167;

  L1167:
  x1 = XVECEXP (x0, 0, 1);
  if (immediate_operand (x1, VOIDmode))
    {
      ro[2] = x1;
      goto L1168;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1218;

  L1168:
  x1 = XVECEXP (x0, 0, 2);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1169;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1218;

  L1169:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 15 && 1)
    if (GET_CODE (operands[2]) == CONST_INT && INTVAL (operands[2]) > 0)
      return 214;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1218;

  L1219:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    goto L1220;
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1234;

  L1220:
  x1 = XVECEXP (x0, 0, 1);
  if (memory_operand (x1, DImode))
    {
      ro[1] = x1;
      goto L1221;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1234;

  L1221:
  x1 = XVECEXP (x0, 0, 2);
  if (pnum_clobbers != 0 && 1)
    {
      ro[2] = x1;
      *pnum_clobbers = 1;
      return 219;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1234;

  L1235:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    goto L1236;
  goto ret0;

  L1236:
  x1 = XVECEXP (x0, 0, 1);
  if (memory_operand (x1, DImode))
    {
      ro[1] = x1;
      goto L1237;
    }
  goto ret0;

  L1237:
  x1 = XVECEXP (x0, 0, 2);
  if (pnum_clobbers != 0 && 1)
    {
      ro[2] = x1;
      *pnum_clobbers = 1;
      return 220;
    }
  goto ret0;

  L1207:
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == CALL && 1)
    goto L1208;
  goto ret0;

  L1208:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == MEM && 1)
    goto L1209;
  goto ret0;

  L1209:
  x3 = XEXP (x2, 0);
  if (address_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L1210;
    }
  L1225:
  if (immediate_operand (x3, SImode))
    {
      ro[0] = x3;
      goto L1226;
    }
  goto ret0;

  L1210:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    goto L1211;
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1225;

  L1211:
  x1 = XVECEXP (x0, 0, 1);
  if (memory_operand (x1, DImode))
    {
      ro[1] = x1;
      goto L1212;
    }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1225;

  L1212:
  x1 = XVECEXP (x0, 0, 2);
  ro[2] = x1;
  goto L1213;

  L1213:
  x1 = XVECEXP (x0, 0, 3);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1214;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1225;

  L1214:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 15 && 1)
    return 219;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1225;

  L1226:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == CONST_INT && XWINT (x2, 0) == 0 && 1)
    goto L1227;
  goto ret0;

  L1227:
  x1 = XVECEXP (x0, 0, 1);
  if (memory_operand (x1, DImode))
    {
      ro[1] = x1;
      goto L1228;
    }
  goto ret0;

  L1228:
  x1 = XVECEXP (x0, 0, 2);
  ro[2] = x1;
  goto L1229;

  L1229:
  x1 = XVECEXP (x0, 0, 3);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1230;
  goto ret0;

  L1230:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == SImode && GET_CODE (x2) == REG && XINT (x2, 0) == 15 && 1)
    return 220;
  goto ret0;

  L1134:
  x1 = XEXP (x0, 0);
  if (GET_MODE (x1) == SImode && GET_CODE (x1) == MEM && 1)
    goto L1135;
  goto ret0;

  L1135:
  x2 = XEXP (x1, 0);
  if (address_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L1136;
    }
  L1146:
  if (immediate_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L1147;
    }
  goto ret0;

  L1136:
  x1 = XEXP (x0, 1);
  if (pnum_clobbers != 0 && 1)
    {
      ro[1] = x1;
      *pnum_clobbers = 1;
      return 211;
    }
  x1 = XEXP (x0, 0);
  x2 = XEXP (x1, 0);
  goto L1146;

  L1147:
  x1 = XEXP (x0, 1);
  if (pnum_clobbers != 0 && 1)
    {
      ro[1] = x1;
      *pnum_clobbers = 1;
      return 212;
    }
  goto ret0;

  L1247:
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == CONST_INT && XWINT (x1, 0) == 0 && 1)
    return 227;
  goto ret0;

  L1249:
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == CONST_INT && XWINT (x1, 0) == 0 && 1)
    return 228;
  goto ret0;

  L1251:
  x1 = XVECEXP (x0, 0, 0);
  ro[0] = x1;
  return 229;
 ret0: return -1;
}

rtx
split_1 (x0, insn)
     register rtx x0;
     rtx insn;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1, x2, x3, x4, x5;
  rtx tem;

  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 1);
  if (GET_MODE (x2) != SImode)
    goto ret0;
  switch (GET_CODE (x2))
    {
    case AND:
      goto L749;
    case IOR:
      goto L779;
    case XOR:
      goto L809;
    case NOT:
      goto L817;
    case NE:
      goto L1298;
    case NEG:
      goto L1306;
    case EQ:
      goto L1315;
    case PLUS:
      goto L1332;
    case MINUS:
      goto L1342;
    }
  goto ret0;

  L749:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L750;
    }
  goto ret0;

  L750:
  x3 = XEXP (x2, 1);
  ro[2] = x3;
  goto L751;

  L751:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L752;
  goto ret0;

  L752:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[3] = x2;
      if (GET_CODE (operands[2]) == CONST_INT
   && !SMALL_INT (operands[2])
   && (INTVAL (operands[2]) & 0x3ff) == 0x3ff)
	return gen_split_146 (operands);
      }
  goto ret0;

  L779:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L780;
    }
  goto ret0;

  L780:
  x3 = XEXP (x2, 1);
  ro[2] = x3;
  goto L781;

  L781:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L782;
  goto ret0;

  L782:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[3] = x2;
      if (GET_CODE (operands[2]) == CONST_INT
   && !SMALL_INT (operands[2])
   && (INTVAL (operands[2]) & 0x3ff) == 0x3ff)
	return gen_split_152 (operands);
      }
  goto ret0;

  L809:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L810;
    }
  goto ret0;

  L810:
  x3 = XEXP (x2, 1);
  ro[2] = x3;
  goto L811;

  L811:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L812;
  goto ret0;

  L812:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[3] = x2;
      if (GET_CODE (operands[2]) == CONST_INT
   && !SMALL_INT (operands[2])
   && (INTVAL (operands[2]) & 0x3ff) == 0x3ff)
	return gen_split_158 (operands);
      }
  goto ret0;

  L817:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) == SImode && GET_CODE (x3) == XOR && 1)
    goto L818;
  goto ret0;

  L818:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L819;
    }
  goto ret0;

  L819:
  x4 = XEXP (x3, 1);
  ro[2] = x4;
  goto L820;

  L820:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L821;
  goto ret0;

  L821:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[3] = x2;
      if (GET_CODE (operands[2]) == CONST_INT
   && !SMALL_INT (operands[2])
   && (INTVAL (operands[2]) & 0x3ff) == 0x3ff)
	return gen_split_159 (operands);
      }
  goto ret0;

  L1298:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1299;
    }
  goto ret0;

  L1299:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1300;
  goto ret0;

  L1300:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1301;
  goto ret0;

  L1301:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return gen_split_237 (operands);
  goto ret0;

  L1306:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    goto ret0;
  switch (GET_CODE (x3))
    {
    case NE:
      goto L1307;
    case EQ:
      goto L1324;
    }
  goto ret0;

  L1307:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1308;
    }
  goto ret0;

  L1308:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L1309;
  goto ret0;

  L1309:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1310;
  goto ret0;

  L1310:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return gen_split_238 (operands);
  goto ret0;

  L1324:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1325;
    }
  goto ret0;

  L1325:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L1326;
  goto ret0;

  L1326:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1327;
  goto ret0;

  L1327:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return gen_split_240 (operands);
  goto ret0;

  L1315:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1316;
    }
  goto ret0;

  L1316:
  x3 = XEXP (x2, 1);
  if (GET_CODE (x3) == CONST_INT && XWINT (x3, 0) == 0 && 1)
    goto L1317;
  goto ret0;

  L1317:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1318;
  goto ret0;

  L1318:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return gen_split_239 (operands);
  goto ret0;

  L1332:
  x3 = XEXP (x2, 0);
  if (GET_MODE (x3) != SImode)
    goto ret0;
  switch (GET_CODE (x3))
    {
    case NE:
      goto L1333;
    case EQ:
      goto L1353;
    }
  goto ret0;

  L1333:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1334;
    }
  goto ret0;

  L1334:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L1335;
  goto ret0;

  L1335:
  x3 = XEXP (x2, 1);
  if (register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L1336;
    }
  goto ret0;

  L1336:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1337;
  goto ret0;

  L1337:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return gen_split_241 (operands);
  goto ret0;

  L1353:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1354;
    }
  goto ret0;

  L1354:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L1355;
  goto ret0;

  L1355:
  x3 = XEXP (x2, 1);
  if (register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L1356;
    }
  goto ret0;

  L1356:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1357;
  goto ret0;

  L1357:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return gen_split_243 (operands);
  goto ret0;

  L1342:
  x3 = XEXP (x2, 0);
  if (register_operand (x3, SImode))
    {
      ro[2] = x3;
      goto L1343;
    }
  goto ret0;

  L1343:
  x3 = XEXP (x2, 1);
  if (GET_MODE (x3) != SImode)
    goto ret0;
  switch (GET_CODE (x3))
    {
    case NE:
      goto L1344;
    case EQ:
      goto L1364;
    }
  goto ret0;

  L1344:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1345;
    }
  goto ret0;

  L1345:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L1346;
  goto ret0;

  L1346:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1347;
  goto ret0;

  L1347:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return gen_split_242 (operands);
  goto ret0;

  L1364:
  x4 = XEXP (x3, 0);
  if (register_operand (x4, SImode))
    {
      ro[1] = x4;
      goto L1365;
    }
  goto ret0;

  L1365:
  x4 = XEXP (x3, 1);
  if (GET_CODE (x4) == CONST_INT && XWINT (x4, 0) == 0 && 1)
    goto L1366;
  goto ret0;

  L1366:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1367;
  goto ret0;

  L1367:
  x2 = XEXP (x1, 0);
  if (GET_MODE (x2) == CCmode && GET_CODE (x2) == REG && XINT (x2, 0) == 0 && 1)
    return gen_split_244 (operands);
  goto ret0;
 ret0: return 0;
}

rtx
split_insns (x0, insn)
     register rtx x0;
     rtx insn;
{
  register rtx *ro = &recog_operand[0];
  register rtx x1, x2, x3, x4, x5;
  rtx tem;

  L404:
  switch (GET_CODE (x0))
    {
    case SET:
      goto L405;
    case PARALLEL:
      if (XVECLEN (x0, 0) == 2 && 1)
	goto L746;
    }
  goto ret0;

  L405:
  x1 = XEXP (x0, 0);
  if (register_operand (x1, DFmode))
    {
      ro[0] = x1;
      goto L406;
    }
  L1278:
  if (register_operand (x1, VOIDmode))
    {
      ro[0] = x1;
      goto L1279;
    }
  L1287:
  if (register_operand (x1, SImode))
    {
      ro[0] = x1;
      goto L1288;
    }
  goto ret0;

  L406:
  x1 = XEXP (x0, 1);
  if (register_operand (x1, DFmode))
    {
      ro[1] = x1;
      if (reload_completed)
	return gen_split_87 (operands);
      }
  x1 = XEXP (x0, 0);
  goto L1278;

  L1279:
  x1 = XEXP (x0, 1);
  if (memop (x1, VOIDmode))
    {
      ro[1] = x1;
      goto L1280;
    }
  L1283:
  if (extend_op (x1, VOIDmode))
    {
      ro[1] = x1;
      goto L1284;
    }
  x1 = XEXP (x0, 0);
  goto L1287;

  L1280:
  x2 = XEXP (x1, 0);
  if (immediate_operand (x2, SImode))
    {
      ro[2] = x2;
      if (flag_pic)
	return gen_split_233 (operands);
      }
  goto L1283;

  L1284:
  x2 = XEXP (x1, 0);
  if (memop (x2, VOIDmode))
    {
      ro[2] = x2;
      goto L1285;
    }
  x1 = XEXP (x0, 0);
  goto L1287;

  L1285:
  x3 = XEXP (x2, 0);
  if (immediate_operand (x3, SImode))
    {
      ro[3] = x3;
      if (flag_pic)
	return gen_split_234 (operands);
      }
  x1 = XEXP (x0, 0);
  goto L1287;

  L1288:
  x1 = XEXP (x0, 1);
  if (immediate_operand (x1, SImode))
    goto L1292;
  goto ret0;

  L1292:
  ro[1] = x1;
  if (! flag_pic && (GET_CODE (operands[1]) == SYMBOL_REF
		  || GET_CODE (operands[1]) == CONST
		  || GET_CODE (operands[1]) == LABEL_REF))
    return gen_split_235 (operands);
  L1293:
  ro[1] = x1;
  if (flag_pic && (GET_CODE (operands[1]) == SYMBOL_REF
		|| GET_CODE (operands[1]) == CONST))
    return gen_split_236 (operands);
  goto ret0;

  L746:
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET && 1)
    goto L747;
  goto ret0;

  L747:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[0] = x2;
      goto L748;
    }
  L1265:
  if (memop (x2, VOIDmode))
    {
      ro[0] = x2;
      goto L1266;
    }
  goto ret0;
 L748:
  tem = split_1 (x0, insn);
  if (tem != 0) return tem;
  x2 = XEXP (x1, 0);
  goto L1265;

  L1266:
  x3 = XEXP (x2, 0);
  if (symbolic_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1267;
    }
  L1273:
  if (immediate_operand (x3, SImode))
    {
      ro[1] = x3;
      goto L1274;
    }
  goto ret0;

  L1267:
  x2 = XEXP (x1, 1);
  if (reg_or_0_operand (x2, VOIDmode))
    {
      ro[2] = x2;
      goto L1268;
    }
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1273;

  L1268:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1269;
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1273;

  L1269:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[3] = x2;
      if (! flag_pic)
	return gen_split_231 (operands);
      }
  x1 = XVECEXP (x0, 0, 0);
  x2 = XEXP (x1, 0);
  x3 = XEXP (x2, 0);
  goto L1273;

  L1274:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, VOIDmode))
    {
      ro[2] = x2;
      goto L1275;
    }
  goto ret0;

  L1275:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == CLOBBER && 1)
    goto L1276;
  goto ret0;

  L1276:
  x2 = XEXP (x1, 0);
  if (register_operand (x2, SImode))
    {
      ro[3] = x2;
      if (flag_pic)
	return gen_split_232 (operands);
      }
  goto ret0;
 ret0: return 0;
}

