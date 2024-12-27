#pragma once


class USBDevice
{
protected:
    TaskHandle_t task_handle_;
    StaticTask_t task_TCB_;
    StackType_t task_stack_[task_usb::c_stack_size_words];


public:
    // ctor
    USBDevice();

    void
    init();

    void 
    notify_task()
    {
        vTaskNotifyGiveFromISR(task_handle_, NULL);
    }

protected:
    static void 
    task_entry(void* param);

    void 
    task();

    void
    vendor_2_task();
};



void
init_usb();
