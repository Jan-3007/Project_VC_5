#include "VC5_global.h"


USBCmdProcessor g_USB_cmd_processor;


void
USBCmdProcessor::task()
{
    // check if host has sent SET CONFIGURATION
    if(!tud_vendor_n_mounted_IN(ITF_INDEX_VENDOR_2))
    {
        return;
    }

    uint32_t len = tud_vendor_n_read(ITF_INDEX_VENDOR_1, cmd_buffer_.data(), cmd_buffer_.size());
    if(len == 0)
    {
        return;
    }

    if(len >= sizeof(VC5_MsgHeader))
    {
        process_cmd(len);
    }
    else
    {
        printf("%s: vendor 1, unexpected length, len = %u\n", __func__, len);
    }
}


void 
USBCmdProcessor::process_cmd(uint32_t len)
{
    const VC5_MsgHeader* header = reinterpret_cast<VC5_MsgHeader*>(cmd_buffer_.data());
    if(len != header->message_length)
    {
        // error
        printf("%s: invalid message_length = %u, len = %u\n", __func__, header->message_length, len);
        send_rsp_done(header->unique_id, VC5_MsgHeader::st_invalid_length);
        return;
    }

    // process command
    uint8_t status = VC5_MsgHeader::st_error;
    switch(header->message_code)
    {
        default:
            status = VC5_MsgHeader::st_invalid_cmd;
            break;
    }

    send_rsp_done(header->unique_id, status);
}


void
USBCmdProcessor::send_rsp_done(uint8_t unique_id, uint8_t status)
{
    VC5_MsgHeader* header = reinterpret_cast<VC5_MsgHeader*>(rsp_buffer_.data());
    header->message_length = sizeof(VC5_MsgHeader);
    header->message_code = VC5_MsgHeader::rsp_done;
    header->unique_id = unique_id;
    header->message_status = status;
    header->reserved = 0;

    static_assert(sizeof(*header) <= VENDOR_BULK_EP_SIZE, "wrong msg size");
    // send message
    tud_vendor_n_write(ITF_INDEX_VENDOR_1, header, sizeof(VC5_MsgHeader));
    // for sending packages smaller than CFG_TUD_VENDOR_EPSIZE
    tud_vendor_n_flush(ITF_INDEX_VENDOR_1);
}


void
vendor_1_task()
{
    g_USB_cmd_processor.task();
}