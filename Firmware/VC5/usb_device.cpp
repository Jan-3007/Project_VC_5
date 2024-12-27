#include "VC5_global.h"



ReportState g_report;
USBDevice g_usb_device;


void
init_usb()
{
    g_usb_device.init();
}


// ctor
USBDevice::USBDevice()
{
}


void
USBDevice::init()
{
    // create USB task
    task_handle_ = xTaskCreateStatic(                                                    
        task_entry,                                 // TaskFunction_t       pxTaskCode,
        task_usb::name,                             // const char * const   pcName,
        task_usb::c_stack_size_words,               // const uint32_t       ulStackDepth,
        this,                                       // void * const         pvParameters,
        task_usb::c_prio,                           // UBaseType_t          uxPriority,
        task_stack_,                                // StackType_t * const  puxStackBuffer,
        &task_TCB_                                  // StaticTask_t * const pxTaskBuffer
    );
}


void 
USBDevice::task_entry(void* param)
{
    USBDevice* This = static_cast<USBDevice*>(param);

    This->task();
}


void 
USBDevice::task()
{
    tusb_init();
//    tud_init(BOARD_DEVICE_RHPORT_NUM);

    while(true)
    {
        tud_task_ext(task_usb::c_usb_task_interval_ms, false);

//        hid_task();

        // task for vendor 2
        vendor_2_task();
    }
}


void 
USBDevice::hid_task()
{
    if(!tud_hid_ready())
    {
        return;
    }

    if(g_report.key_sent)
    {
        // send key release event
        uint16_t empty_key = 0;
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &empty_key, 2);

        g_report.key_sent = false;
    }
    else
    {
        // check all rotaries for a value change
        for(uint rot_index = 0; rot_index < c_num_rotaries; rot_index++)
        {
            // get value of the rotary
            int rot_value = capture_rotary_value(rot_index);

            if(rot_value > 0)
            {
                // send increment
                uint16_t volume_up = HID_USAGE_CONSUMER_VOLUME_INCREMENT;
                tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_up, 2);

                g_report.key_sent = true;
            }
            else if(rot_value < 0)
            {
                // send decrement
                uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
                tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);

                g_report.key_sent = true;
            }
            else
            {
                // rotary value has not changed
            }

            
            // get value of rotary button
            int btn_value = capture_button_value(rot_index);

            // on button released
            if(btn_value < 0)
            {
                // send mute
                uint16_t mute = HID_USAGE_CONSUMER_MUTE;
                tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &mute, 2);

                g_report.key_sent = true;
            }
            else
            {
                // button value has not changed
            }
        }
    }
}


void USBDevice::vendor_2_task()
{
    // check if host has sent SET CONFIGURATION
    if(!tud_vendor_n_mounted_in(ITF_INDEX_VENDOR_2))
    {
        return;
    }

    // tud_vendor_n_write_available actually requires an index (0 based)
    if(CFG_TUD_VENDOR_TX_BUFSIZE == tud_vendor_n_write_available(ITF_INDEX_VENDOR_2))
    {
        // TX FiFo is empty
        // check all rotaries
        for(uint i = 0; i < 5; i++)
        {
            // get value of rotary
            int rot_value = capture_rotary_value(i);
            if(rot_value != 0)
            {
                // create message to send
                Volctrl_EventMsg msg;
                msg.event_code = Volctrl_EventMsg::evt_rotary_turned;
                msg.rotary_index = i;
                msg.value = rot_value;
                msg.reserved = 0;

                // send message
                tud_vendor_n_write(ITF_INDEX_VENDOR_2, &msg, sizeof(msg));
                // for sending packages smaller than CFG_TUD_VENDOR_EPSIZE
                tud_vendor_n_flush(ITF_INDEX_VENDOR_2);
                break;
            }

            // get value of button
            int btn_value = capture_button_value(i);
            if(btn_value != 0)
            {
                // create message to send
                Volctrl_EventMsg msg;
                msg.event_code = Volctrl_EventMsg::evt_rotary_button;
                msg.rotary_index = i;
                msg.value = btn_value;
                msg.reserved = 0;

                // send message
                tud_vendor_n_write(ITF_INDEX_VENDOR_2, &msg, sizeof(msg));
                // for sending packages smaller than CFG_TUD_VENDOR_EPSIZE
                tud_vendor_n_flush(ITF_INDEX_VENDOR_2);
                break;
            }
        }
    }
}


// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
extern "C"
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) bufsize;
}


// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
extern "C"
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

//    usb_isr_callback();

    return 0;
}


// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
extern "C"
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
    (void) instance;
    (void) report;
    (void) len;
}



//--------------------------------------------------------------------+
// for MS OS 2.0 desc
//--------------------------------------------------------------------+

// Invoked when a control transfer occurred
// Driver response accordingly to the request and the transfer stage (setup/data/ack)
// return false to stall control endpoint (e.g unsupported request)
extern "C"
bool tud_vendor_control_xfer_cb(uint8_t rhport, uint8_t stage, tusb_control_request_t const * request)
{
    // nothing to with DATA & ACK stage
    if (stage != CONTROL_STAGE_SETUP) return true;

    switch (request->bmRequestType_bit.type)
    {
        case TUSB_REQ_TYPE_VENDOR:
            switch (request->bRequest)
            {
                case VENDOR_REQUEST_MICROSOFT:
                    if ( request->wIndex == 7 )       // 0x07 for MS_OS_20_DESCIPTOR_INDEX
                    {
                        // Get Microsoft OS 2.0 compatible descriptor
                        uint16_t total_len;
                        memcpy(&total_len, desc_ms_os_20 + 8, 2);

                        return tud_control_xfer(rhport, request, const_cast<uint8_t*>(desc_ms_os_20), total_len);
                    }
                    break;

                default: 
                    break;
            }
            break;

        default: 
            break;
    }

  // stall unknown request
  return false;
}
