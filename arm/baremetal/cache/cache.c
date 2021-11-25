#include "stdio.h"
#include "stdint.h"
#include "utils_def.h"

static unsigned long tlb_size = 0UL;
static unsigned long tlb_fillptr = 0UL;
static unsigned long tlb_addr = 0UL;
static unsigned long tlb_emerg = 0UL;

extern void *memset(void *dst, int val, size_t count);
extern void hang(void);

//#define DEBUG 1

#ifdef DEBUG
#define debug mprintf
#else
#define debug(...)
#endif

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

typedef uint32_t u32;
typedef int32_t s32;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint64_t u64;
typedef int64_t s64;
typedef char s8;
typedef unsigned char u8;

/*
 * SCTLR_EL1/SCTLR_EL2/SCTLR_EL3 bits definitions
 */
#define CR_M		(1 << 0)	/* MMU enable			*/
#define CR_A		(1 << 1)	/* Alignment abort enable	*/
#define CR_C		(1 << 2)	/* Dcache enable		*/
#define CR_SA		(1 << 3)	/* Stack Alignment Check Enable	*/
#define CR_I		(1 << 12)	/* Icache enable		*/
#define CR_WXN		(1 << 19)	/* Write Permision Imply XN	*/
#define CR_EE		(1 << 25)	/* Exception (Big) Endian	*/

/* PAGE_SHIFT determines the page size */
#undef  PAGE_SIZE
#define PAGE_SHIFT		12
#define PAGE_SIZE		(1 << PAGE_SHIFT)
#define PAGE_MASK		(~(PAGE_SIZE - 1))

/***************************************************************/

/*
 * Memory types
 */
#define MT_DEVICE_NGNRNE	0
#define MT_DEVICE_NGNRE		1
#define MT_DEVICE_GRE		2
#define MT_NORMAL_NC		3
#define MT_NORMAL		4

#define MEMORY_ATTRIBUTES	((0x00 << (MT_DEVICE_NGNRNE * 8)) |	\
				(0x04 << (MT_DEVICE_NGNRE * 8))   |	\
				(0x0c << (MT_DEVICE_GRE * 8))     |	\
				(0x44 << (MT_NORMAL_NC * 8))      |	\
				(UL(0xff) << (MT_NORMAL * 8)))

/*
 * Hardware page table definitions.
 *
 */

#define PTE_TYPE_MASK		(3 << 0)
#define PTE_TYPE_FAULT		(0 << 0)
#define PTE_TYPE_TABLE		(3 << 0)
#define PTE_TYPE_PAGE		(3 << 0)
#define PTE_TYPE_BLOCK		(1 << 0)
#define PTE_TYPE_VALID		(1 << 0)

#define PTE_TABLE_PXN		(1UL << 59)
#define PTE_TABLE_XN		(1UL << 60)
#define PTE_TABLE_AP		(1UL << 61)
#define PTE_TABLE_NS		(1UL << 63)

/*
 * Block
 */
#define PTE_BLOCK_MEMTYPE(x)	((x) << 2)
#define PTE_BLOCK_NS            (1 << 5)
#define PTE_BLOCK_NON_SHARE	(0 << 8)
#define PTE_BLOCK_OUTER_SHARE	(2 << 8)
#define PTE_BLOCK_INNER_SHARE	(3 << 8)
#define PTE_BLOCK_AF		(1 << 10)
#define PTE_BLOCK_NG		(1 << 11)
#define PTE_BLOCK_PXN		(UL(1) << 53)
#define PTE_BLOCK_UXN		(UL(1) << 54)

/*
 * AttrIndx[2:0]
 */
#define PMD_ATTRINDX(t)		((t) << 2)
#define PMD_ATTRINDX_MASK	(7 << 2)
#define PMD_ATTRMASK		(PTE_BLOCK_PXN		| \
				 PTE_BLOCK_UXN		| \
				 PMD_ATTRINDX_MASK	| \
				 PTE_TYPE_VALID)

/*
 * TCR flags.
 */
#define TCR_T0SZ(x)		((64 - (x)) << 0)
#define TCR_IRGN_NC		(0 << 8)
#define TCR_IRGN_WBWA		(1 << 8)
#define TCR_IRGN_WT		(2 << 8)
#define TCR_IRGN_WBNWA		(3 << 8)
#define TCR_IRGN_MASK		(3 << 8)
#define TCR_ORGN_NC		(0 << 10)
#define TCR_ORGN_WBWA		(1 << 10)
#define TCR_ORGN_WT		(2 << 10)
#define TCR_ORGN_WBNWA		(3 << 10)
#define TCR_ORGN_MASK		(3 << 10)
#define TCR_SHARED_NON		(0 << 12)
#define TCR_SHARED_OUTER	(2 << 12)
#define TCR_SHARED_INNER	(3 << 12)
#define TCR_TG0_4K		(0 << 14)
#define TCR_TG0_64K		(1 << 14)
#define TCR_TG0_16K		(2 << 14)
#define TCR_EPD1_DISABLE	(1 << 23)

#define TCR_EL1_RSVD		(1 << 31)
#define TCR_EL2_RSVD		(1 << 31 | 1 << 23)
#define TCR_EL3_RSVD		(1 << 31 | 1 << 23)

struct mm_region {
	u64 virt;
	u64 phys;
	u64 size;
	u64 attrs;
};

static struct mm_region mem_map[] = {
	{
		.virt = 0x0UL,
		.phys = 0x0UL,
		.size = 0xFC000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0xFC000000UL,
		.phys = 0xFC000000UL,
		.size = 0x03000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		.virt = 0xFF780000UL,
		.phys = 0xFF780000UL,
		.size = 0x1000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		.virt = 0xFF800000UL,
		.phys = 0xFF800000UL,
		.size = 0x00800000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	}, {
		/* List terminator */
		0,
	}
};

static inline unsigned int current_el(void)
{
	unsigned long el;

	asm volatile("mrs %0, CurrentEL" : "=r" (el) : : "cc");
	return 3 & (el >> 2);
}

static inline unsigned int get_sctlr(void)
{
	unsigned int el;
	unsigned long val;

	el = current_el();
	if (el == 1)
		asm volatile("mrs %0, sctlr_el1" : "=r" (val) : : "cc");
	else if (el == 2)
		asm volatile("mrs %0, sctlr_el2" : "=r" (val) : : "cc");
	else
		asm volatile("mrs %0, sctlr_el3" : "=r" (val) : : "cc");

	return val;
}

static inline void set_sctlr(unsigned long val)
{
	unsigned int el;

	el = current_el();
	if (el == 1)
		asm volatile("msr sctlr_el1, %0" : : "r" (val) : "cc");
	else if (el == 2)
		asm volatile("msr sctlr_el2, %0" : : "r" (val) : "cc");
	else
		asm volatile("msr sctlr_el3, %0" : : "r" (val) : "cc");

	asm volatile("isb");
}

void icache_disable(void)
{
	set_sctlr(get_sctlr() & ~CR_I);
}

extern void __asm_invalidate_icache_all(void);

void invalidate_icache_all(void)
{
	__asm_invalidate_icache_all();
}

int icache_status(void)
{
	return (get_sctlr() & CR_I) != 0;
}

void icache_enable(void)
{
	invalidate_icache_all();
	set_sctlr(get_sctlr() | CR_I);
}

extern void __asm_flush_dcache_all(void);

inline void flush_dcache_all(void)
{
	__asm_flush_dcache_all();
}

extern void __asm_invalidate_tlb_all(void);

void dcache_disable(void)
{
	unsigned int sctlr;

	sctlr = get_sctlr();

	/* if cache isn't enabled no need to disable */
	if (!(sctlr & CR_C))
		return;

	set_sctlr(sctlr & ~(CR_C|CR_M));

	flush_dcache_all();
	__asm_invalidate_tlb_all();
}

int dcache_status(void)
{
	return (get_sctlr() & CR_C) != 0;
}

extern void __asm_invalidate_dcache_all(void);

void invalidate_dcache_all(void)
{
	__asm_invalidate_dcache_all();
}

#define MAX_PTE_ENTRIES 512

static u64 *create_table(void)
{
	u64 *new_table = (u64*)tlb_fillptr;
	u64 pt_len = MAX_PTE_ENTRIES * sizeof(u64);

	debug("%s() enter\n", __func__);
	/* Allocate MAX_PTE_ENTRIES pte entries */
	tlb_fillptr += pt_len;

	if (tlb_fillptr - tlb_addr > tlb_size)
		mprintf("Insufficient RAM for page table: 0x%x > 0x%x. "
		      "Please increase the size in get_page_table_size()",
			tlb_fillptr - tlb_addr,
			tlb_size);

	/* Mark all entries as invalid */
	memset(new_table, 0, pt_len);

	debug("%s() leave\n", __func__);
	return new_table;
}

/*
 *  With 4k page granule, a virtual address is split into 4 lookup parts
 *  spanning 9 bits each:
 *
 *    _______________________________________________
 *   |       |       |       |       |       |       |
 *   |   0   |  Lv0  |  Lv1  |  Lv2  |  Lv3  |  off  |
 *   |_______|_______|_______|_______|_______|_______|
 *     63-48   47-39   38-30   29-21   20-12   11-00
 *
 *             mask        page size
 *
 *    Lv0: FF8000000000       --
 *    Lv1:   7FC0000000       1G
 *    Lv2:     3FE00000       2M
 *    Lv3:       1FF000       4K
 *    off:          FFF
 */

u64 get_tcr(int el, u64 *pips, u64 *pva_bits)
{
	u64 max_addr = 0;
	u64 ips, va_bits;
	u64 tcr;
	int i;

	/* Find the largest address we need to support */
	for (i = 0; mem_map[i].size || mem_map[i].attrs; i++)
		max_addr = max(max_addr, mem_map[i].virt + mem_map[i].size);

	/* Calculate the maximum physical (and thus virtual) address */
	if (max_addr > (1ULL << 44)) {
		ips = 5;
		va_bits = 48;
	} else  if (max_addr > (1ULL << 42)) {
		ips = 4;
		va_bits = 44;
	} else  if (max_addr > (1ULL << 40)) {
		ips = 3;
		va_bits = 42;
	} else  if (max_addr > (1ULL << 36)) {
		ips = 2;
		va_bits = 40;
	} else  if (max_addr > (1ULL << 32)) {
		ips = 1;
		va_bits = 36;
	} else {
		ips = 0;
		va_bits = 32;
	}

	if (el == 1) {
		tcr = TCR_EL1_RSVD | (ips << 32) | TCR_EPD1_DISABLE;
	} else if (el == 2) {
		tcr = TCR_EL2_RSVD | (ips << 16);
	} else {
		tcr = TCR_EL3_RSVD | (ips << 16);
	}

	/* PTWs cacheable, inner/outer WBWA and inner shareable */
	tcr |= TCR_TG0_4K | TCR_SHARED_INNER | TCR_ORGN_WBWA | TCR_IRGN_WBWA;
	tcr |= TCR_T0SZ(va_bits);

	if (pips)
		*pips = ips;
	if (pva_bits)
		*pva_bits = va_bits;

	return tcr;
}

static int pte_type(u64 *pte)
{
	return *pte & PTE_TYPE_MASK;
}

/* Returns the LSB number for a PTE on level <level> */
static int level2shift(int level)
{
	/* Page is 12 bits wide, every level translates 9 bits */
	return (12 + 9 * (3 - level));
}

static u64 *find_pte(u64 addr, int level)
{
	int start_level = 0;
	u64 *pte;
	u64 idx;
	u64 va_bits;
	int i;

	debug("addr=%x level=%d\n", addr, level);

	get_tcr(0, NULL, &va_bits);
	if (va_bits < 39)
		start_level = 1;

	if (level < start_level)
		return NULL;

	/* Walk through all page table levels to find our PTE */
	pte = (u64*)tlb_addr;
	for (i = start_level; i < 4; i++) {
		idx = (addr >> level2shift(i)) & 0x1FF;
		pte += idx;
		debug("idx=%x PTE %p at level %d: %x\n", idx, pte, i, *pte);

		/* Found it */
		if (i == level)
			return pte;
		/* PTE is no table (either invalid or block), can't traverse */
		if (pte_type(pte) != PTE_TYPE_TABLE)
			return NULL;
		/* Off to the next level */
		pte = (u64*)(*pte & 0x0000fffffffff000ULL);
	}

	/* Should never reach here */
	return NULL;
}

static void set_pte_table(u64 *pte, u64 *table)
{
	/* Point *pte to the new table */
	debug("Setting %p to addr=%p\n", pte, table);
	*pte = PTE_TYPE_TABLE | (unsigned long)table;
}

/* Splits a block PTE into table with subpages spanning the old block */
static void split_block(u64 *pte, int level)
{
	u64 old_pte = *pte;
	u64 *new_table;
	u64 i = 0;
	/* level describes the parent level, we need the child ones */
	int levelshift = level2shift(level + 1);

	if (pte_type(pte) != PTE_TYPE_BLOCK)
		debug("PTE %p (%x) is not a block. Some driver code wants to "
		      "modify dcache settings for an range not covered in "
		      "mem_map.", pte, old_pte);

	new_table = create_table();
	debug("Splitting pte %p (%x) into %p\n", pte, old_pte, new_table);

	for (i = 0; i < MAX_PTE_ENTRIES; i++) {
		new_table[i] = old_pte | (i << levelshift);

		/* Level 3 block PTEs have the table type */
		if ((level + 1) == 3)
			new_table[i] |= PTE_TYPE_TABLE;

		debug("Setting new_table[%lld] = %x\n", i, new_table[i]);
	}

	/* Set the new table into effect */
	set_pte_table(pte, new_table);
}

static void add_map(struct mm_region *map)
{
	u64 *pte;
	u64 virt = map->virt;
	u64 phys = map->phys;
	u64 size = map->size;
	u64 attrs = map->attrs | PTE_TYPE_BLOCK | PTE_BLOCK_AF;
	u64 blocksize;
	int level;
	u64 *new_table;

	debug("%s() enter\n", __func__);
	while (size) {
		pte = find_pte(virt, 0);
		if (pte && (pte_type(pte) == PTE_TYPE_FAULT)) {
			debug("Creating table for virt 0x%x\n", virt);
			new_table = create_table();
			set_pte_table(pte, new_table);
		}

		for (level = 1; level < 4; level++) {
			pte = find_pte(virt, level);
			if (!pte)
				debug("pte not found\n");

			blocksize = 1ULL << level2shift(level);
			debug("Checking if pte fits for virt=%x size=%x blocksize=%x\n",
			      virt, size, blocksize);
			if (size >= blocksize && !(virt & (blocksize - 1))) {
				/* Page fits, create block PTE */
				debug("Setting PTE %p to block virt=%x\n",
				      pte, virt);
				if (level == 3)
					*pte = phys | attrs | PTE_TYPE_PAGE;
				else
					*pte = phys | attrs;
				virt += blocksize;
				phys += blocksize;
				size -= blocksize;
				break;
			} else if (pte_type(pte) == PTE_TYPE_FAULT) {
				/* Page doesn't fit, create subpages */
				debug("Creating subtable for virt 0x%x blksize=%x\n",
				      virt, blocksize);
				new_table = create_table();
				set_pte_table(pte, new_table);
			} else if (pte_type(pte) == PTE_TYPE_BLOCK) {
				debug("Split block into subtable for virt 0x%x blksize=0x%x\n",
				      virt, blocksize);
				split_block(pte, level);
			}
		}
	}
	debug("%s() leave\n", __func__);
}

enum pte_type {
	PTE_INVAL,
	PTE_BLOCK,
	PTE_LEVEL,
};

/*
 * This is a recursively called function to count the number of
 * page tables we need to cover a particular PTE range. If you
 * call this with level = -1 you basically get the full 48 bit
 * coverage.
 */
static int count_required_pts(u64 addr, int level, u64 maxaddr)
{
	int levelshift = level2shift(level);
	u64 levelsize = 1ULL << levelshift;
	u64 levelmask = levelsize - 1;
	u64 levelend = addr + levelsize;
	int r = 0;
	int i;
	enum pte_type pte_type = PTE_INVAL;

	for (i = 0; mem_map[i].size || mem_map[i].attrs; i++) {
		struct mm_region *map = &mem_map[i];
		u64 start = map->virt;
		u64 end = start + map->size;

		/* Check if the PTE would overlap with the map */
		if (max(addr, start) <= min(levelend, end)) {
			start = max(addr, start);
			end = min(levelend, end);

			/* We need a sub-pt for this level */
			if ((start & levelmask) || (end & levelmask)) {
				pte_type = PTE_LEVEL;
				break;
			}

			/* Lv0 can not do block PTEs, so do levels here too */
			if (level <= 0) {
				pte_type = PTE_LEVEL;
				break;
			}

			/* PTE is active, but fits into a block */
			pte_type = PTE_BLOCK;
		}
	}

	/*
	 * Block PTEs at this level are already covered by the parent page
	 * table, so we only need to count sub page tables.
	 */
	if (pte_type == PTE_LEVEL) {
		int sublevel = level + 1;
		u64 sublevelsize = 1ULL << level2shift(sublevel);

		/* Account for the new sub page table ... */
		r = 1;

		/* ... and for all child page tables that one might have */
		for (i = 0; i < MAX_PTE_ENTRIES; i++) {
			r += count_required_pts(addr, sublevel, maxaddr);
			addr += sublevelsize;

			if (addr >= maxaddr) {
				/*
				 * We reached the end of address space, no need
				 * to look any further.
				 */
				break;
			}
		}
	}

	return r;
}

/* Returns the estimated required size of all page tables */
u64 get_page_table_size(void)
{
	u64 one_pt = MAX_PTE_ENTRIES * sizeof(u64);
	u64 size = 0;
	u64 va_bits;
	int start_level = 0;

	get_tcr(0, NULL, &va_bits);
	if (va_bits < 39)
		start_level = 1;

	/* Account for all page tables we would need to cover our memory map */
	size = one_pt * count_required_pts(0, start_level - 1, 1ULL << va_bits);

	/*
	 * We need to duplicate our page table once to have an emergency pt to
	 * resort to when splitting page tables later on
	 */
	size *= 2;

	/*
	 * We may need to split page tables later on if dcache settings change,
	 * so reserve up to 4 (random pick) page tables for that.
	 */
	size += one_pt * 4;

	return size;
}

void setup_pgtables(void)
{
	int i;

	debug("%s() enter\n", __func__);
	if (!tlb_fillptr || !tlb_addr)
		mprintf("Page table pointer not setup.");

	/*
	 * Allocate the first level we're on with invalidate entries.
	 * If the starting level is 0 (va_bits >= 39), then this is our
	 * Lv0 page table, otherwise it's the entry Lv1 page table.
	 */
	create_table();

	/* Now add all MMU table entries one after another to the table */
	for (i = 0; mem_map[i].size || mem_map[i].attrs; i++)
		add_map(&mem_map[i]);
	debug("%s() leave\n", __func__);
}

static void setup_all_pgtables(void)
{
	u64 addr = tlb_addr;
	u64 size = tlb_size;

	debug("%s() enter\n", __func__);
	/* Reset the fill ptr */
	tlb_fillptr = addr;

	/* Create normal system page tables */
	setup_pgtables();

	/* todo Create emergency page tables */
	tlb_size -= (uintptr_t)tlb_fillptr - tlb_addr;
	tlb_addr = tlb_fillptr;
	setup_pgtables();
	tlb_emerg = tlb_addr;
	tlb_addr = addr;
	tlb_size = size;
	debug("%s() leave\n", __func__);
}

static inline void set_ttbr_tcr_mair(int el, u64 table, u64 tcr, u64 attr)
{
	asm volatile("dsb sy");
	if (el == 1) {
		asm volatile("msr ttbr0_el1, %0" : : "r" (table) : "memory");
		asm volatile("msr tcr_el1, %0" : : "r" (tcr) : "memory");
		asm volatile("msr mair_el1, %0" : : "r" (attr) : "memory");
	} else if (el == 2) {
		asm volatile("msr ttbr0_el2, %0" : : "r" (table) : "memory");
		asm volatile("msr tcr_el2, %0" : : "r" (tcr) : "memory");
		asm volatile("msr mair_el2, %0" : : "r" (attr) : "memory");
	} else if (el == 3) {
		asm volatile("msr ttbr0_el3, %0" : : "r" (table) : "memory");
		asm volatile("msr tcr_el3, %0" : : "r" (tcr) : "memory");
		asm volatile("msr mair_el3, %0" : : "r" (attr) : "memory");
	} else {
		hang();
	}
	asm volatile("isb");
}

void mmu_setup(void)
{
	int el;

	tlb_emerg = 0;
	tlb_size = get_page_table_size();
	tlb_addr = 0x200000;
	debug("tbl_size = %x\n", tlb_size);
	debug("tbl_addr = %x\n", tlb_addr);
	/* Set up page tables only once */
	debug("setup all page talbes\n");
	setup_all_pgtables();

	el = current_el();
	set_ttbr_tcr_mair(el, tlb_addr, get_tcr(el, NULL, NULL),
			  MEMORY_ATTRIBUTES);

	/* enable the mmu */
	set_sctlr(get_sctlr() | CR_M);
	mprintf("mmu setup complete.\n");
}

void dcache_enable(void)
{
	/* The data cache is not active unless the mmu is enabled */
	if (!(get_sctlr() & CR_M)) {
		invalidate_dcache_all();
		__asm_invalidate_tlb_all();
		mmu_setup();
	}

	set_sctlr(get_sctlr() | CR_C);
}

void enable_caches(void)
{
	if (!icache_status()) {
		//mprintf("enable I-cache\n");
		icache_enable();
	}
	if (!dcache_status()) {
		//mprintf("enable D-cache\n");
		dcache_enable();
	}
}

