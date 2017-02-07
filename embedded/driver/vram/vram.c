
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/major.h>
#include <linux/vmalloc.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/blkdev.h>
#include <linux/bitops.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/hdreg.h>
#include <linux/dma-mapping.h>

#define DEVICE_NAME "vram"
#define VRAM_MINORS		16
#define SECTOR_SIZE    512

struct vram_device{
	struct mutex mutex_lock;
	spinlock_t lock;
	unsigned int phys_addr;
	unsigned int size;
	void *virt_addr;
	struct gendisk *gendisk;
	struct request_queue *queue;
	struct resource *mem_region;
};
static int vram_major;
static atomic_t vram_index;
void dump_tlb_all();
#define SECTOR_SHIFT		9
void flush_tlb_all(void);
static void vram_do_bvec(struct vram_device *vram, struct page *page,
					   unsigned int len, unsigned int off, int readmode,
					   sector_t sector)
{
	void *mem;
	void* addr = vram->virt_addr + (sector << SECTOR_SHIFT);
	mem = kmap_atomic(page);
	if (readmode) {
		memcpy(mem + off, addr, len);
		local_flush_icache_range((unsigned long)mem + off,(unsigned long)mem + off + len);
	} else {
		memcpy(addr, mem + off, len);
	}
	kunmap_atomic(mem);
}

static void do_vram_request(struct request_queue *q)
{
	struct request *req;
	struct vram_device *vram = q->queuedata;
	req = blk_fetch_request(q);
	while (req) {
		unsigned long start = blk_rq_pos(req) << 9;
		unsigned long len  = blk_rq_cur_bytes(req);
		int err = 0;

		if (start + len > vram->size) {
			pr_err(DEVICE_NAME ": bad access: block=%llu, "
			       "count=%u\n",
			       (unsigned long long)blk_rq_pos(req),
			       blk_rq_cur_sectors(req));
			err = -EIO;
			goto done;
		}
		{
			int readmode;
			struct bio_vec *bvec;
			sector_t sector;
			int i;
			readmode = (rq_data_dir(req) == READ);
			sector = req->bio->bi_sector;
			bio_for_each_segment(bvec,req->bio, i) {
				unsigned int len = bvec->bv_len;
				vram_do_bvec(vram, bvec->bv_page, len,
								  bvec->bv_offset, readmode, sector);
				sector += len >> SECTOR_SHIFT;
			}
		}


		if(0){
			void* addr = vram->virt_addr + start;
			unsigned long size = len;




			if (rq_data_dir(req) == READ){
				printk("r s:%p t:%p start = %ld size = %ld\n",addr,req->buffer,start,size);
				if(req->buffer == 0)
					flush_tlb_all();
				memcpy(req->buffer, addr, size);
				if(req->buffer == 0){
					dump_tlb_all();
					dump_stack();
				}
				dma_cache_sync(NULL,req->buffer,size,DMA_TO_DEVICE);
				flush_icache_range((unsigned long)req->buffer,(unsigned long)req->buffer + size);
			}else{
				memcpy(addr, req->buffer, size);
				printk("w s:%p t:%p size=%ld\n",req->buffer,addr,size);
			}
		}
	done:
		if (!__blk_end_request_cur(req, err))
			req = blk_fetch_request(q);
	}

}

static int vram_getgeo(struct block_device *bd, struct hd_geometry *geo)
{
	geo->start = 0;
	geo->cylinders = (unsigned short)(get_capacity(bd->bd_disk) >> 11);
	geo->heads = (unsigned char)(1 << 6);
	geo->sectors = (unsigned char)(1 << 5);
	return 0;
}
static int pmem_alloc(struct vram_device *vram)
{
	char s[20];
	sprintf(s,"%s-%d",DEVICE_NAME,atomic_read(&vram_index));
	vram->mem_region = request_mem_region(vram->phys_addr, vram->size, s);
	if (!vram->mem_region) {
			pr_err("could not reserve region [0x%x:0x%x]\n",
					 vram->phys_addr, vram->size);
			return -1;
	}
	vram->virt_addr = ioremap(vram->phys_addr, vram->size);
	if(0){
		unsigned int *v = vram->virt_addr;
		int i;
		printk("vram check mem!----- \n");
		for(i = 0;i < vram->size / 4;i++){
			if((i & (1* 1024 * 1024 - 1)) == 0){
				printk("write addr %p\n",(v+i));
			}
			*(v+i) = i;
		}
		for(i = 0;i < vram->size / 4;i++){
			unsigned int val;
			if((i & (1* 1024 * 1024 - 1)) == 0){
				printk("read check addr %p\n",(v+i));
			}
			val = *(v + i);
			if(val != i){
				printk("check error,addr %p, expect:0x%08x real:0x%08x\n",v+i,i,val);
			}
		}
	}
	return vram->virt_addr? 0:-1;
}
static void pmem_free(struct vram_device *vram)
{
	if(vram->virt_addr)
		iounmap(vram->virt_addr);
	if(vram->mem_region)
		release_mem_region(vram->phys_addr, vram->size);
}
static const struct block_device_operations vram_fops = {
	.owner = THIS_MODULE,
	.getgeo = vram_getgeo,
};

static struct vram_device *vram_new_devices(struct device *dev, struct resource *res)
{
	struct vram_device *vram;
	int err;
	vram = kmalloc(sizeof(struct vram_device),GFP_KERNEL);
	if(IS_ERR_OR_NULL(vram)) {
		pr_err("%s %d vram malloc failed!\n",__FILE__,__LINE__);
		return NULL;
	}
	memset(vram,0,sizeof(struct vram_device));

	vram->phys_addr = res->start;
	vram->size = resource_size(res);
	if(pmem_alloc(vram)){
		goto out_disk;
	}

	mutex_init(&vram->mutex_lock);
	spin_lock_init(&vram->lock);
	vram->gendisk = alloc_disk(1);
    if (!vram->gendisk)
		goto out_disk;

    vram->queue = blk_init_queue(do_vram_request, &vram->lock);
    if (!vram->queue)
		goto out_queue;
	vram->queue->queuedata = vram;
	err = elevator_change(vram->queue, "noop");
	if (err) {
		printk(KERN_ERR "%s:%d (elevator_init) fail\n",
			   __func__, __LINE__);
		goto change_err;
	}

	blk_queue_max_hw_sectors(vram->queue, 1024);
	blk_queue_bounce_limit(vram->queue, BLK_BOUNCE_ANY);

    vram->gendisk->major = vram_major;
    vram->gendisk->first_minor = atomic_inc_return(&vram_index) - 1;
    vram->gendisk->fops = &vram_fops;
    sprintf(vram->gendisk->disk_name, "vram");

    vram->gendisk->queue = vram->queue;
	vram->gendisk->private_data = vram;
	set_capacity(vram->gendisk,vram->size / SECTOR_SIZE);
    add_disk(vram->gendisk);

    return vram;
change_err:
	blk_cleanup_queue(vram->queue);
	pr_err("%s %d vram failed!\n",__FILE__,__LINE__);
out_queue:
    put_disk(vram->gendisk);
	pr_err("%s %d vram failed!\n",__FILE__,__LINE__);
out_disk:
	kfree(vram);
	pr_err("%s %d vram failed!\n",__FILE__,__LINE__);
    return 0;
}

static void vram_delete_device(struct vram_device *vram)
{
    del_gendisk(vram->gendisk);
    put_disk(vram->gendisk);
    blk_cleanup_queue(vram->queue);
	pmem_free(vram);
	kfree(vram);
}
static int vram_probe(struct platform_device *pdev)
{
	struct vram_device *vram;
	struct resource *res;
	if (WARN_ON(pdev->num_resources > 1))
		return -ENXIO;
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENXIO;
	vram = vram_new_devices(&pdev->dev, res);
	if (IS_ERR(vram))
		return PTR_ERR(vram);
	platform_set_drvdata(pdev, vram);
	return 0;
}

static int vram_remove(struct platform_device *pdev)
{
	struct vram_device *vd = platform_get_drvdata(pdev);
	vram_delete_device(vd);
	return 0;
}

static struct platform_driver vram_driver = {
	.probe		= vram_probe,
	.remove		= vram_remove,
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= DEVICE_NAME,
	},
};


static struct resource jz_vram_resources[] = {
	[0] = {
		.flags = IORESOURCE_MEM,
		.start	= 0x20000000 + 512*1024*1024,
		.end	= 0x20000000 + 512*1024*1024  + 512 * 1024 * 1024 - 1,
	},
};

struct platform_device jz_vram_device = {
	.name	= "vram",
	.num_resources	= ARRAY_SIZE(jz_vram_resources),
	.resource	= jz_vram_resources,
};

static int __init vram_init(void)
{
	int error;
	vram_major = register_blkdev(0, DEVICE_NAME);
	if (vram_major < 0)
		return vram_major;
	error = platform_driver_register(&vram_driver);
	if (error)
		unregister_blkdev(vram_major, DEVICE_NAME);
	platform_device_register(&jz_vram_device);
	if(error == 0)
		printk("vram init ok.\n");
	return error;
}
static void __exit vram_exit(void)
{
	platform_driver_unregister(&vram_driver);
	unregister_blkdev(vram_major, DEVICE_NAME);
}

module_init(vram_init);
module_exit(vram_exit);
MODULE_LICENSE("GPL");
