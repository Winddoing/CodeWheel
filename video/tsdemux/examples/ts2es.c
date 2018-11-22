/**
 * This examples demonstrates demuxing a data stream from the Transport-Stream.
 */

#include <stdio.h>
#include <string.h>

#include <tsdemux.h>


void print_pmt(TSDemuxContext *ctx, void *data) {
    TSDPMTData *pmt = (TSDPMTData*)data;
    size_t i;

    for(i=0;i<pmt->program_elements_length; ++i) {
        TSDProgramElement *prog = &pmt->program_elements[i];
        int reg_types = TSD_REG_PES;
        size_t j;
        for(j=0;j<prog->descriptors_length;++j) {
            TSDDescriptor *des = &prog->descriptors[j];

            if(des->tag == 0x97) {
                reg_types |= TSD_REG_ADAPTATION_FIELD;
            }
        }
        // register all the PIDs we come across.
        tsd_register_pid(ctx, prog->elementary_pid, reg_types);
    }
}

FILE *fp;
void event_cb(TSDemuxContext *ctx, uint16_t pid, TSDEventId event_id, void *data)
{
    if(event_id == TSD_EVENT_PMT){
        print_pmt(ctx, data);
    }else if(event_id == TSD_EVENT_PES) {
        TSDPESPacket *pes = (TSDPESPacket*) data;
        printf("PID %d PES Packet, Size: %d, stream_id=%u, pts=%llu, dts=%llu\n", pid, pes->data_bytes_length, pes->stream_id, pes->pts, pes->dts);
		if (!fp)
			fp = fopen("xxxx.es", "w");

		fwrite(pes->data_bytes, pes->data_bytes_length, 1, fp);
    }
}

int main(int argc, char **charv) {
    FILE *file_input = NULL;
    if (argc < 2)
    {
        printf("not enough params\n");
        return -10;
    }

    file_input = fopen(charv[1], "rb");
    if (!file_input)
    {
        printf("failed to open file %s\n", charv[1]);
        return -20;
    }

    // create a demuxing context.
    TSDemuxContext ctx;
    // set default values onto the context.
    tsd_context_init(&ctx);

    // add a callback.
    // the callback is used to determine which PIDs contain the data we want
    // to demux. We also receive PES data for any PIDs that we register later on.
    tsd_set_event_callback(&ctx, event_cb);

    // create a buffer on the stack which we'll use to read the file data into.
    // we'll set the buffer size to exactly 10 TS packets but it could be any
    // size.
    char buffer[1880];

    int count = 0; // number of bytes read from the file.
    size_t parsed = 0; // number of bytes parsed by the demuxer.

    // read the file until we reach the end.
    do {
        // at the end of the do loop, we write back any remainder bytes into
        // the buffer, this is why we write into the buffer at the location
        // &buffer[count - parsed].
        count = fread(&buffer[count - parsed], 1, 1880 - (count - parsed), file_input);

        if(count > 0) {
            // with res, we could report any errors found during demuxing
            TSDCode res;
            parsed = tsd_demux(&ctx, buffer, count, &res);
        }else{
            parsed = 0;
        }
        // during 'demux' our callback may be called, so we can safely discard
        // our buffer.
        // we'll copy any unused bytes back into the start of the buffer.
        // this may happen if we read partial packets from file, or there was
        // some corruption in the TS stream.
        if(parsed < count) {
            memmove(buffer, buffer, count - parsed);
        }
    } while(count > 0);

    // finally end the demux process which will flush any remaining PES data.
    tsd_demux_end(&ctx);

    // destroy context
    tsd_context_destroy(&ctx);

    if (file_input)
        fclose(file_input);

    return 0;
}

