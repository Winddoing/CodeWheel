#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <drm.h>
#include <xf86drm.h>
#include <xf86drmMode.h>

//#include "util/kms.h"
//#include "drm_fourcc.h"
//#include "buffers.h"
//#include "libdrm_macros.h"

static void debug_save_file(void *ptr, int size, char *path)
{
    FILE *debug_file;
    int ret = 0;

    debug_file = fopen(path, "wb");
    if (!debug_file) {
        printf("DEBUG: open debug file %s failed!\n", path);
        return;
    }

    ret = fwrite(ptr, 1, size, debug_file);
    if (ret != size)
        printf("DEBUG: save debug file %s failed\n", path);
    else
        printf("DEBUG: save debug file as %s\n", path);

    fclose(debug_file);
}

/* Fill black and white vertical stripe data, like:
 * White | Black | White | Black
 */
static void fill_frame_rgb24_date(unsigned char *ptr, int width, int height, int linesize)
{
    unsigned char *line_ptr = NULL;
    unsigned char d_r = 0, d_g = 0, d_b = 0;

    if (!ptr || (linesize < width)) {
        printf("ERROR: %s invalid args!\n", __func__);
        return;
    }

    for (int i = 0; i < height; i++) {
        line_ptr = ptr + i * linesize;
        for (int j = 0; j < width; j++) {
            if (j < (width / 4)) {
                d_r = 0; d_g = 0; d_b = 0;
            } else if (j < (width / 2)) {
                d_r = 255; d_g = 255; d_b = 255;
            } else if (j < (width * 3 / 4)) {
                d_r = 0; d_g = 0; d_b = 0;
            } else {
                d_r = 255; d_g = 255; d_b = 255;
            }

            *line_ptr = d_r;
            line_ptr++;
            *line_ptr = d_g;
            line_ptr++;
            *line_ptr = d_b;
            line_ptr++;
        }
    }
}

int main(int argc, char *argv[])
{
    int frame_cnt = 30;
    int width, height;
    int ret = 0;
    int drm_fd = 0;
    int connector_id, crtc_id, plane_id;

    drmModeRes *res = NULL;
    drmModeConnector *conn = NULL;
    drmModeCrtc *crtc = NULL;
    drmModePlaneRes *pres = NULL;
    unsigned int fb_id;

    if (argc == 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
        printf("### Input Resolution:%sx%s --> (%d, %d)\n", argv[1], argv[2], width, height);
    } else {
        width = 720;
        height = 1280;
        printf("### Default Resolution: 720x1280\n");
    }

    printf("### Init drm ...\n");
    drm_fd = util_open(NULL, NULL);
    if (drm_fd < 0) {
        printf("ERROR: drm_open failed!\n");
        return -1;
    }

    /***********************************************************
     * Get screen infos
     ***********************************************************/
    if (drmSetClientCap (drm_fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1)) {
        printf("ERROR: drmModeGetCrtc failed!\n");
        goto OUT;
    }

    res = drmModeGetResources (drm_fd);
    if (!res) {
        printf("ERROR: drmModeGetResources failed!\n");
        drmClose(drm_fd);
        return -1;
    }

    printf("INFO: count_fbs:%d, count_crtcs:%d, count_encoders:%d, count_connectors:%d \n",
            res->count_fbs, res->count_crtcs, res->count_encoders, res->count_connectors);

    /* This test for only 1 connector and only 1 crtc */
    connector_id = res->connectors[0];
    crtc_id = res->crtcs[0];

    /* Just fort print connector only */
    conn = drmModeGetConnector (drm_fd, connector_id);
    if (!conn) {
        printf("ERROR: drmModeGetConnector failed!\n");
        goto OUT;
    }

    /* Get plane infos */
    pres = drmModeGetPlaneResources (drm_fd);
    if (!pres) {
        printf("ERROR: drmModeGetPlaneResources failed!\n");
        goto OUT;
    }
    /* This test for only 1 plane */
    plane_id = pres->planes[0];

    printf("INFO: connector->name:%s-%u\n", util_lookup_connector_type_name(conn->connector_type),
            conn->connector_type_id);
    printf("INFO: connector id = %d / crtc id = %d / plane id = %d\n",
            connector_id, crtc_id, plane_id);

    /***********************************************************
     * Start Mode Setting
     ***********************************************************/
    struct bo *bo;
    drmModeModeInfo *mode;
    /* Max planes:4
     * handles: dumb buffer handle, create by libdrm api
     * pitches: real linesize, by Bytes.
     * offset: every plane offset.
     */
    unsigned int handles[4] = {0};
    unsigned int pitches[4] = {0};
    unsigned int offsets[4] = {0};
    uint64_t cap = 0;

    /* Support dumb buffer? */
    ret = drmGetCap(drm_fd, DRM_CAP_DUMB_BUFFER, &cap);
    if (ret || cap == 0) {
        fprintf(stderr, "ERROR: driver doesn't support the dumb buffer API\n");
        goto OUT;
    }

    /* Create dumb buffer and mmap it */
    bo = bo_create(drm_fd, DRM_FORMAT_RGB888, width, height, handles, pitches, offsets, UTIL_PATTERN_SMPTE);
    if (bo == NULL) {
        printf("ERROR: bo_create failed!\n");
        goto OUT;
    }

    for (int i = 0; i < 4; i++)
        printf("INFO: #%d handles:%d, pitches:%d, offsets:%d\n", i, handles[i], pitches[i], offsets[i]);

    /* Fill date: full screen is white */
    memset(bo->ptr, 255, height * pitches[0]);

    /* Get framebuff id, this id is used by drm api to find the buffer.
     * Format the display memory, then the drm api knows what format to
     * display the memory of the drm application.
     */
    ret = drmModeAddFB2(drm_fd, width, height, DRM_FORMAT_RGB888, handles, pitches, offsets, &fb_id, 0);
    if (ret) {
        printf("ERROR: drmModeAddFB2 failed!\n");
        goto OUT;
    }

    /* Get the crtc display mode. For this test, only 1 mode support. */
    mode = &conn->modes[0];
    printf("INFO: mode->mame:%s, %dx%d, Conn_id:%d, crtc_id:%d\n",
            mode->name, mode->hdisplay, mode->vdisplay, connector_id, crtc_id);

    ret = drmModeSetCrtc(drm_fd, crtc_id, fb_id, 0, 0, (uint32_t *)&connector_id, 1, mode);
    if (ret) {
        printf("ERROR: drmModeSetCrtc failed!\n");
        goto OUT;
    }

    /* XXX: Actually check if this is needed */
    drmModeDirtyFB(drm_fd, fb_id, NULL, 0);

    /* show modeseting effect: full screen white */
    sleep(1);

    /***********************************************************
     * Start Set plane: show frame to plane
     ***********************************************************/
    while (frame_cnt--) {
        fill_frame_rgb24_date(bo->ptr, width, height, pitches[0]);
        //debug_save_file(bo->ptr, pitches[0]*height);

        /* note src coords (last 4 args) are in Q16 format */
        if (drmModeSetPlane(drm_fd, plane_id, crtc_id, fb_id,
                    0, 0, 0, width, height, 0, 0, width << 16, height << 16)) {
            printf("ERROR: drmModeSetPlane failed! plane_id:%d, fb_id:%d\n", plane_id, fb_id);
            return -1;
        }

        sleep(1);
    }

OUT:
    if (bo) {
        if (bo->ptr)
            drm_munmap(bo->ptr, bo->size);
        bo->ptr = NULL;
        bo_destroy(bo);
    }

    if (pres)
        drmModeFreePlaneResources (pres);
    if (conn)
        drmModeFreeConnector (conn);
    if (res)
        drmModeFreeResources (res);
    if (drm_fd)
        drmClose(drm_fd);

    return 0;
}
