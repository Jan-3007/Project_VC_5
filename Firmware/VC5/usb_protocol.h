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
#define VOLCTRL_PROTOCOL_VERSION_MJ     1
#define VOLCTRL_PROTOCOL_VERSION_MN     0

//
// VID/PID of devices that support this protocol
//
#define VOLCTRL_VID   0x152A    // Thesycon
#define VOLCTRL_PID   0x0301    // Volume Controller

//
// Device interface GUIDs used on Windows to enumerate the sub-devices
//
#define VOLCTRL_WINUSB_INTERFACE_GUID_STR     "{EC03545C-4179-4324-B2B2-24E4A976F2E7}"  
#define VOLCTRL_WINUSB_INTERFACE_GUID_INIT    { 0xec03545c, 0x4179, 0x4324, { 0xb2, 0xb2, 0x24, 0xe4, 0xa9, 0x76, 0xf2, 0xe7 } }

//
// Endpoint addresses of each interface
//
#define EPNUM_HID_INT_IN        0x81

#define EPNUM_VENDOR_1_BULK_OUT   0x02
#define EPNUM_VENDOR_1_BULK_IN    0x82

#define EPNUM_VENDOR_2_INT_IN     0x83

//
// get protocol version
//
struct VolctrlProtocolVersion
{
    // major/minor version
    uint8_t protocolVersionMj;
    uint8_t protocolVersionMn;
};

//
// Event message
//
struct Volctrl_EventMsg
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



// restore packing
#pragma pack(pop)
