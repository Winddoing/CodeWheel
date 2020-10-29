
``` C
static int vtest_block_write(int fd, void *buf, int size)
{
   char *ptr = buf;
   int left;
   int ret;
   left = size;

   do {
      ret = write(fd, ptr, left);
      if (ret < 0) {
         return -errno;
      }

      left -= ret;
      ptr += ret;
   } while (left);

   return size;
}

static int vtest_block_read(struct vtest_input *input, void *buf, int size)
{
   int fd = input->data.fd;
   char *ptr = buf;
   int left;
   int ret;
   static int savefd = -1;

   left = size;
   do {
      ret = read(fd, ptr, left);
      if (ret <= 0) {
         return ret == -1 ? -errno : 0;
      }

      left -= ret;
      ptr += ret;
   } while (left);

   return size;
}
```
> https://gitlab.freedesktop.org/virgl/virglrenderer
