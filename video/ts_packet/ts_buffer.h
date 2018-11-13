/*
 * ts_buffer.h
 *
 *  Created on: 2015-1-18
 *      Author: shenwei
 */

#ifndef TS_BUFFER_H_
#define TS_BUFFER_H_

#define ts_free free
#define ts_malloc malloc

struct ts_buffer{
	char *data;
	char  *curr;
	int32_t length;
};


static int32_t ts_buffer_resize(struct ts_buffer *b, int32_t size){
	if(size <= 0){
		ts_warn("size need bigger than zero\n");
		return 0;
	}
	int32_t length = b->length;
	if(length == 0 ){
		length = size;
	}
	while(length < size){
		length = length * 2;
	}

	char *data = ts_malloc(length);
	if(!data){
		ts_warn("malloc failed\n");
		return 0;
	}

	int32_t curr = b->curr - b->data;
	memcpy(data, b->data, curr);
	ts_free(b->data);
	b->data = data;
	b->curr = b->data + curr;
	b->length = length;

	return size;
}

struct ts_buffer *ts_buffer_new(){

	struct ts_buffer *b;
	b = ts_malloc(sizeof(*b));
	if(!b){
		ts_warn("malloc failed\n");
		return NULL;
	}
	memset(b, 0, sizeof(*b));
	if(!ts_buffer_resize(b, 1024)){
		ts_free(b);
		return NULL;
	}
	return b;
}


void ts_buffer_free(struct ts_buffer *b){
	assert(b);
	if(b->data){
		ts_free(b->data);
	}
	ts_free(b);

}

void ts_buffer_init(struct ts_buffer *b){
	assert(b);
	b->curr = b->data;
}

int32_t ts_buffer_size(struct ts_buffer *b){
	assert(b);
	return (b->curr - b->data);
}

int32_t ts_buffer_write(struct ts_buffer *b, char *data, int32_t size){
	assert(b && data && size > 0);
	if(b->curr - b->data + size > b->length){
		if(!ts_buffer_resize(b, b->curr - b->data + size)){
			ts_warn("buffer resize form:%d to:%d failed!\n", ts_buffer_size(b), ts_buffer_size(b) + size);
			return 0;
		}
	}
	memcpy(b->curr, data, size);
	b->curr = b->curr + size;
	return size;
}

int32_t ts_buffer_read(struct ts_buffer *b, char *data, int32_t size){
	assert(b && data && size > 0);
	if( b->curr - b->data < size){
		ts_warn("require data size:%d > buffer data size:%d\n", size, ts_buffer_size(b));
		return 0;
	}
	memcpy(data, b->data, size);
	return size;
}

void ts_buffer_show(struct ts_buffer *b, int32_t size){
	int32_t i;
	int32_t bsize = ts_buffer_size(b);
	for(i = 0; i < bsize && i < size ;i++){
		if(i%32 == 0){
			printf("\n");
		}
		printf(" %02x", b->data[i]&0xff);
	}
	printf("\n");
}

#endif /* TS_BUFFER_H_ */
