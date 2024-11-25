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
    tud_init(BOARD_DEVICE_RHPORT_NUM);

    create_task();
}


void 
USBDevice::create_task()
{
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
    while(true)
    {
        tud_task_ext(task_usb::c_usb_task_interval_ms, false);

        hid_task();
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
