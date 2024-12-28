#pragma once

// store struct packing layouts
#pragma pack(push, 1)



//
// Protocol version
//
// If changes are made to the interface that are compatible with previous versions
// of the interface then the minor version number will be incremented.
// If changes are made to the interface that cause an incompatibility with previous versions
// of the interface then the major version number will be incremented.
//
#define VC5_PROTOCOL_VERSION_MJ     1
#define VC5_PROTOCOL_VERSION_MN     0

//
// VID/PID of devices that support this protocol
//
#define VC5_USB_VID   0x1234    
#define VC5_USB_PID   0x0123    

//
// Device interface GUIDs used on Windows to enumerate the sub-devices
//
#define VC5_WINUSB_INTERFACE_GUID_STR     "{EC03545C-4179-4324-B2B2-24E4A976F2E7}"  
#define VC5_WINUSB_INTERFACE_GUID_INIT    { 0xec03545c, 0x4179, 0x4324, { 0xb2, 0xb2, 0x24, 0xe4, 0xa9, 0x76, 0xf2, 0xe7 } }

//
// Endpoint addresses of each interface
//
#define EPNUM_VENDOR_1_BULK_OUT   0x01
#define EPNUM_VENDOR_1_BULK_IN    0x81

#define EPNUM_VENDOR_2_INT_IN     0x82

//
// get protocol version
//
struct VC5_protocol_version
{
    // major/minor version
    uint8_t protocol_version_mj;
    uint8_t protocol_version_mn;
};


//
// Event message
//
struct VC5_EventMsg
{
    static constexpr uint8_t evt_rotary_turned = 0xe1;
    static constexpr uint8_t evt_rotary_button = 0xe2;

    // operation code
    uint8_t event_code;

    // 0..4
    uint8_t rotary_index;

    // -128 .. +127 for rot_value
    // -1 or 1 for btn_value, 1 == pressed, -1 == released
    int8_t value;

    // padding
    uint8_t reserved;
};


//
// Command and response message header
//
struct VC5_MsgHeader
{
    // max total message length
    static constexpr uint8_t max_cmd_length = 64;
    static constexpr uint8_t max_rsp_length = 64;

    // message code
    static constexpr uint8_t rsp_done = 0xd0;           // no data

    // message status
    static constexpr uint8_t st_success = 0x00;
    static constexpr uint8_t st_error = 0x01;
    static constexpr uint8_t st_invalid_cmd = 0x02;
    static constexpr uint8_t st_invalid_param = 0x03;
    static constexpr uint8_t st_invalid_length = 0x04;

    // total message size in bytes, including the size of this header
    uint32_t message_length;

    // operation code
    uint8_t message_code;

    // unique message identifier for command/response message pairs
    uint8_t unique_id;

    // status code, valid for RESPONSE messages only, otherwise zero
    uint8_t message_status;

    // padding
    uint8_t reserved;
};


// restore packing
#pragma pack(pop)
