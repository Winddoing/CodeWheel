#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <epoxy/gl.h>

#define check_glerror(func) \
    do {    \
        GLenum err = glGetError();  \
        if (err == GL_INVALID_VALUE)    \
            printf("\t GL Invalid Value (GL_INVALID_VALUE)\n");  \
        if (err != GL_NO_ERROR) {    \
            printf("%s %d: %s error 0x%x(%d)\n", __func__, __LINE__, func, err, err); \
        }   \
    } while(0)

static void copy_tex_to_cache(uint32_t src_id, uint32_t dst_id, int tex_w, int tex_h)
{
    static GLuint fbo = 0;;

    MLOGD("src_id=%d, dst_id=%d, tex_w=%d, tex_h=%d\n", src_id, dst_id, tex_w, tex_h);
    /* clear GL error */
    while( glGetError() != GL_NO_ERROR) continue;

    if (fbo == 0) {
        glGenFramebuffers(1, &fbo);
        check_glerror("glGenFramebuffers_fbo");
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    check_glerror("glBindFramebuffer_fbo");
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, src_id, 0);
    check_glerror("glFramebufferTexture2D_fbo");

    glBindTexture(GL_TEXTURE_2D, dst_id);
    check_glerror("glBindTexture_fbo");

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_w, tex_w, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    check_glerror("glTexImage2D");

    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, tex_w, tex_h);
    check_glerror("glCopyTexSubImage2D_fbo");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    check_glerror("glBindFramebuffer_fbo");

    glBindTexture(GL_TEXTURE_2D, 0);
    check_glerror("glBindTexture_fbo");
}

static void copy_tex_to_encode(uint32_t src_id, uint32_t dst_id, int tex_w, int tex_h, int enc_w, int enc_h)
{
    static GLuint rfboid = 0;
    static GLuint wfboid = 0;

    MLOGD("src_id=%d, dst_id=%d, tex_w=%d, tex_h=%d\n", src_id, dst_id, tex_w, tex_h);
    /* clear GL error */
    while( glGetError() != GL_NO_ERROR) continue;

    /* init once */
    if(rfboid == 0) {
        glGenFramebuffers(1, &rfboid);
        check_glerror("glGenFramebuffers_rfboid");
    }
    if(wfboid == 0) {
        glGenFramebuffers(1, &wfboid);
        check_glerror("glGenFramebuffers_wfboid");
    }

    /* prepare copy texture to encoder */
    glBindFramebuffer(GL_READ_FRAMEBUFFER, rfboid);
    check_glerror("glBindFramebuffer_rfboid");
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, wfboid);
    check_glerror("glBindFramebuffer_wfboid");

    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, src_id, 0);
    check_glerror("glFramebufferTexture2D_rfboid");
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst_id, 0);
    check_glerror("glFramebufferTexture2D_wfboid");

    /* copy. scale if width and height not match */
    glBlitFramebuffer(0, 0, tex_w, tex_h, 0, 0, enc_w, enc_h, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    check_glerror("glBlitFramebuffer()");

    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

int init_cache_texture(struct sdg_s* sdg)
{
    printf("Generate cache texture ...\n");

    glGenTextures(CACHE_TEXTURE_SZ, sdg->enc.cache_texture);
    check_glerror("glGenTextures");

    int i = 0;
    for (i = 0; i < CACHE_TEXTURE_SZ; i++) {
        sdg->enc.cache_info[i].status = ENCODED;
        sdg->enc.cache_info[i].is_enc = ENC_WAIT;
        sdg->enc.cache_info[i].w = 0;
        sdg->enc.cache_info[i].h = 0;

        printf("cache_texture id: %d\n", sdg->enc.cache_texture[i]);
    }

    return 0;
}

int dele_cache_texture(struct sdg_s* sdg)
{
    printf("Delete cache texture ...\n");

    glDeleteTextures(CACHE_TEXTURE_SZ, sdg->enc.cache_texture);
    check_glerror("glDeleteTextures");

    return 0;
}

int push_cache_texture(struct sdg_s* sdg, uint32_t render_tex_id, int width, int height)
{
    int pos = 0;
    int wait_cnt = 0;

    pos = sdg->enc.render_pos % CACHE_TEXTURE_SZ;
    while (sdg->enc.cache_info[pos].status == RELOCK) {
        usleep(100);
        wait_cnt++;
    }
    if (wait_cnt)
        printf("Wait for available rendering cache pos=%d, render texture wait cnt=%d...\n", pos, wait_cnt);

    sdg->enc.cache_info[pos].status = RELOCK;
    sdg->enc.cache_info[pos].w = width;
    sdg->enc.cache_info[pos].h = height;
    copy_tex_to_cache(render_tex_id,  sdg->enc.cache_texture[pos], width, height);
    sdg->enc.cache_info[pos].is_enc = ENC_RUN;
    sdg->enc.cache_info[pos].status = RENDERED;
    pos++;
    sdg->enc.render_pos = pos;

    return 0;
}

int pop_cache_texture(struct sdg_s* sdg, uint32_t encode_tex_id, int width, int height)
{
    int pos = 0, w = 0, h = 0;
    int wait_cnt = 0;


    sdg->enc.encode_pos = sdg->enc.render_pos;
    pos = sdg->enc.encode_pos % CACHE_TEXTURE_SZ;
    if (sdg->enc.cache_info[pos].is_enc == ENC_WAIT) {
        return 1;
    }

    while(sdg->enc.cache_info[pos].status == RELOCK || sdg->enc.cache_info[pos].status == ENCODED) {
        usleep(100);
        wait_cnt++;
    }
    if (wait_cnt)
        printf("Wait for available encoding cache, pos=%d, encode texture wait cnt=%d...\n", pos, wait_cnt);

    sdg->enc.cache_info[pos].status = RELOCK;
    w = sdg->enc.cache_info[pos].w;
    h = sdg->enc.cache_info[pos].h;

    copy_tex_to_encode(sdg->enc.cache_texture[pos], encode_tex_id, w, h, width, height);
    sdg->enc.cache_info[pos].is_enc = ENC_WAIT;
    sdg->enc.cache_info[pos].status = ENCODED;
    pos++;
    sdg->enc.encode_pos = pos;

    return 0;
}

#define CAPTURE_TARGET_WIDTH 1920
#define CAPTURE_TARGET_HEIGHT 1080

void enc_pixel_data_from_gpu(GLbyte* data, GLenum format, int texid, int width, int height, int enc_w, int enc_h)
{
    static GLuint rFbo = 0;
    static GLuint wFbo = 0;
    static GLuint wTex = 0;
    static int frame_count = 0;

    int encodeWidth  = enc_w;
    int encodeHeight = enc_h;

    GLenum err;
    GLuint readTex = 0;

    glGetError();
    if(rFbo == 0) {
		glGenFramebuffers(1, &rFbo);
        check_glerror("glGenFramebuffers_rFbo");
	}
    if(wFbo == 0) {
		glGenFramebuffers(1, &wFbo);
        check_glerror("glGenFramebuffers_wFbo");
	}
    if(wTex == 0) {
		glGenTextures(1, &wTex);
        check_glerror("glGenTextures_wTex");
        glBindTexture(GL_TEXTURE_2D, wTex);
        check_glerror("glBindTexture_wTex");
        glTexImage2D(GL_TEXTURE_2D, 0, format, encodeWidth, encodeHeight, 0, format, GL_UNSIGNED_BYTE, NULL);
        check_glerror("glTexImage2D_wTex");
	}

    // scale
    if(width != encodeWidth || height != encodeHeight) {
        printf("src %dx%d, dst %dx%d, not match, scale !!!\n", width, height, encodeWidth, encodeHeight);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, rFbo);
        check_glerror("glBindFramebuffer_1");
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texid, 0);
        check_glerror("glFramebufferTexture2D_texid");
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, wFbo);
        check_glerror("glBindFramebuffer_2");
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, wTex, 0);
        check_glerror("glFramebufferTexture2D_wTex");
        glBlitFramebuffer(0, 0, width, height, 0, 0, encodeWidth, encodeHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        check_glerror("glBlitFramebuffer_texid_wTex");

        readTex = wTex;
    } else {
        readTex = texid;
    }

    glBindFramebuffer(GL_READ_FRAMEBUFFER, rFbo);
    check_glerror("glBindFramebuffer_3");
    glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, readTex, 0);
    check_glerror("glFramebufferTexture2D_readTex");
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    check_glerror("glReadBuffer");

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glPixelStorei(GL_PACK_SKIP_ROWS, 0);
    glPixelStorei(GL_PACK_SKIP_PIXELS, 0);

    glReadPixels(0, 0, encodeWidth, encodeHeight, format, GL_UNSIGNED_BYTE, data);
    err = glGetError();
    if(err != GL_NO_ERROR) {
        printf("ERROR: glReadPixels error 0x%x\n", err);
    }
}

void debug_read_img_from_gpu(uint32_t tex_id, int width, int height, int ignore_frames, int capture_frames)
{
    static int is_start              = 0;
    static int curr_ignore_frames    = 0;
    static int curr_capture_frames   = 0;
    static int wanted_ignore_frames  = 0;
    static int wanted_capture_frames = 0;
    static char file_name[256]       = {0};
    static GLenum img_format         = GL_RGBA;
    static int target_width          = 0;
    static int target_height         = 0;
    static FILE *file                = NULL;
    static GLbyte *pbits             = NULL;  /* CPU memory to save image */
    static size_t frame_size         = 0;
	static GLuint fbo                = 0;
    GLenum err;

    /* run only once */
    if(is_start == 0) {
        is_start              = 1;
        wanted_ignore_frames  = ignore_frames;
        wanted_capture_frames = capture_frames;
        target_width          = width;
        target_height         = height;
        frame_size            = target_width * target_height * 4;
        sprintf(file_name, "image_RGBA_%dx%d_%d_frames.dat", target_width, target_height, wanted_capture_frames);
        file = fopen(file_name, "w+");
        if(!file) {
            printf("Open %s failed\n", file_name);
            return;
        }

        pbits = (GLbyte *)malloc(frame_size);
        if(!pbits) {
            printf("malloc %ld bytes failed\n", frame_size);
            return;
        }
    }

    /* file will be closed if capture finished */
    if(!file || !target_width || !target_height || !wanted_capture_frames) return;

    if(CAPTURE_TARGET_WIDTH != target_width || CAPTURE_TARGET_HEIGHT != target_height) {
        printf("ERROR: width*height not match, target %d*%d, but get %d*%d\n",
                CAPTURE_TARGET_WIDTH, CAPTURE_TARGET_HEIGHT, width, height);
        return;
    }

    if(wanted_ignore_frames && (curr_ignore_frames < wanted_ignore_frames)) {
        curr_ignore_frames++;
        printf("Left ignore frames %d\n", (wanted_ignore_frames - curr_ignore_frames));
        return;
    }

    if(wanted_capture_frames && (curr_capture_frames < wanted_capture_frames)) {
        enc_pixel_data_from_gpu(pbits, img_format, tex_id, target_width, target_height,
                CAPTURE_TARGET_WIDTH, CAPTURE_TARGET_HEIGHT);

        size_t ret = fwrite(pbits, 1, frame_size, file);
        if(ret != frame_size) {
            printf("Write fail: wanted %ld, but only write %ld\n", frame_size, ret);
            return;
        }

        curr_capture_frames++;
        printf("capture %d/%d\n", curr_capture_frames, wanted_capture_frames);
    }

    if(curr_capture_frames >= wanted_capture_frames && file) {
        printf("Capture finish, total %d frames in %s\n", curr_capture_frames, file_name);
        fclose(file);
        file = NULL;
    }
}
