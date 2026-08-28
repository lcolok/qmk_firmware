    /* Gate C v4a: enumeration-only Magic Trackpad 2 canary.
     *
     * This is an independently expressed HID layout matching the public
     * Magic-Trackpad-2 simulator contract used by VoodooInput: a normal mouse
     * report (ID 0x02), a Touch Pad/vendor input report (ID 0x3F, 16 bytes),
     * and the large vendor input report (ID 0x44, 1387 bytes) that stock macOS
     * uses to select its private multitouch path.  The canary intentionally
     * sends none of those multitouch input reports; unknown GET_REPORT requests
     * are stalled in usb_main.c.  This lets us test driver matching without
     * feeding guessed vendor-frame payloads to AppleHIDStack. */

    /* Relative mouse collection, report ID 0x02. */
    HID_RI_USAGE_PAGE(8, 0x01),
    HID_RI_USAGE(8, 0x02),
    HID_RI_COLLECTION(8, 0x01),
        HID_RI_USAGE(8, 0x01),
        HID_RI_COLLECTION(8, 0x00),
            HID_RI_USAGE_PAGE(8, 0x09),
            HID_RI_USAGE_MINIMUM(8, 0x01),
            HID_RI_USAGE_MAXIMUM(8, 0x03),
            HID_RI_LOGICAL_MINIMUM(8, 0x00),
            HID_RI_LOGICAL_MAXIMUM(8, 0x01),
            HID_RI_REPORT_ID(8, 0x02),
            HID_RI_REPORT_COUNT(8, 0x03),
            HID_RI_REPORT_SIZE(8, 0x01),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
            HID_RI_REPORT_COUNT(8, 0x01),
            HID_RI_REPORT_SIZE(8, 0x05),
            HID_RI_INPUT(8, HID_IOF_CONSTANT),
            HID_RI_USAGE_PAGE(8, 0x01),
            HID_RI_USAGE(8, 0x30),
            HID_RI_USAGE(8, 0x31),
            HID_RI_LOGICAL_MINIMUM(8, 0x81),
            HID_RI_LOGICAL_MAXIMUM(8, 0x7F),
            HID_RI_REPORT_SIZE(8, 0x08),
            HID_RI_REPORT_COUNT(8, 0x02),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_RELATIVE),
            HID_RI_REPORT_COUNT(8, 0x04),
            HID_RI_REPORT_SIZE(8, 0x08),
            HID_RI_INPUT(8, HID_IOF_CONSTANT),
        HID_RI_END_COLLECTION(0),
    HID_RI_END_COLLECTION(0),

    /* Touch Pad collection.  The 16-byte vendor payload is report 0x3F. */
    HID_RI_USAGE_PAGE(8, 0x0D),
    HID_RI_USAGE(8, 0x05),
    HID_RI_COLLECTION(8, 0x01),
        HID_RI_USAGE_PAGE(16, 0xFF00),
        HID_RI_USAGE(8, 0x0C),
        HID_RI_LOGICAL_MINIMUM(8, 0x00),
        HID_RI_LOGICAL_MAXIMUM(16, 0x00FF),
        HID_RI_REPORT_SIZE(8, 0x08),
        HID_RI_REPORT_COUNT(8, 0x10),
        HID_RI_REPORT_ID(8, 0x3F),
        HID_RI_INPUT(8, 0x22),
    HID_RI_END_COLLECTION(0),

    /* Apple vendor multitouch collection.  Declared for driver matching only;
     * v4a never transmits report 0x44. */
    HID_RI_USAGE_PAGE(16, 0xFF00),
    HID_RI_USAGE(8, 0x0C),
    HID_RI_COLLECTION(8, 0x01),
        HID_RI_USAGE_PAGE(16, 0xFF00),
        HID_RI_USAGE(8, 0x0C),
        HID_RI_LOGICAL_MINIMUM(8, 0x00),
        HID_RI_LOGICAL_MAXIMUM(16, 0x00FF),
        HID_RI_REPORT_ID(8, 0x44),
        HID_RI_REPORT_SIZE(8, 0x08),
        HID_RI_REPORT_COUNT(16, 0x056B),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_ARRAY | HID_IOF_ABSOLUTE),
    HID_RI_END_COLLECTION(0),
