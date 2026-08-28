    /* Gate C PoC: standard HID Digitizer/Touch Pad top-level collection with
     * two parallel Finger logical collections. */
    HID_RI_USAGE_PAGE(8, 0x0D),            // Digitizers
    HID_RI_USAGE(8, 0x04),                 // Touch Screen (AppleMultitouchHIDService probe)
    HID_RI_COLLECTION(8, 0x01),            // Application
#    ifdef DIGITIZER_SHARED_EP
        HID_RI_REPORT_ID(8, REPORT_ID_DIGITIZER),
#    endif
        // Finger 1: Confidence + Tip + 2-bit Contact ID + X/Y.
        HID_RI_USAGE(8, 0x22),             // Finger
        HID_RI_COLLECTION(8, 0x02),        // Logical
            HID_RI_LOGICAL_MINIMUM(8, 0x00),
            HID_RI_LOGICAL_MAXIMUM(8, 0x01),
            HID_RI_USAGE(8, 0x47),         // Confidence
            HID_RI_USAGE(8, 0x42),         // Tip Switch
            HID_RI_REPORT_COUNT(8, 0x02),
            HID_RI_REPORT_SIZE(8, 0x01),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
            HID_RI_LOGICAL_MAXIMUM(8, 0x03),
            HID_RI_USAGE(8, 0x51),         // Contact Identifier
            HID_RI_REPORT_COUNT(8, 0x01),
            HID_RI_REPORT_SIZE(8, 0x02),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
            HID_RI_REPORT_COUNT(8, 0x01),
            HID_RI_REPORT_SIZE(8, 0x04),
            HID_RI_INPUT(8, HID_IOF_CONSTANT),
            HID_RI_USAGE_PAGE(8, 0x01),    // Generic Desktop
            HID_RI_LOGICAL_MINIMUM(8, 0x00),
            HID_RI_LOGICAL_MAXIMUM(16, 0x0FFF),
            HID_RI_PHYSICAL_MINIMUM(8, 0x00),
            HID_RI_PHYSICAL_MAXIMUM(16, 0x0190), // 4.00 inch
            HID_RI_UNIT(8, 0x13),          // Inch, English Linear
            HID_RI_UNIT_EXPONENT(8, 0x0E), // -2
            HID_RI_USAGE(8, 0x30),         // X
            HID_RI_REPORT_COUNT(8, 0x01),
            HID_RI_REPORT_SIZE(8, 0x10),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
            HID_RI_PHYSICAL_MAXIMUM(16, 0x0113), // 2.75 inch
            HID_RI_USAGE(8, 0x31),         // Y
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
        HID_RI_END_COLLECTION(0),

        // Finger 2, same layout and coordinate range.
        HID_RI_USAGE_PAGE(8, 0x0D),        // Digitizers
        HID_RI_USAGE(8, 0x22),             // Finger
        HID_RI_COLLECTION(8, 0x02),        // Logical
            HID_RI_LOGICAL_MINIMUM(8, 0x00),
            HID_RI_LOGICAL_MAXIMUM(8, 0x01),
            HID_RI_USAGE(8, 0x47),         // Confidence
            HID_RI_USAGE(8, 0x42),         // Tip Switch
            HID_RI_REPORT_COUNT(8, 0x02),
            HID_RI_REPORT_SIZE(8, 0x01),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
            HID_RI_LOGICAL_MAXIMUM(8, 0x03),
            HID_RI_USAGE(8, 0x51),         // Contact Identifier
            HID_RI_REPORT_COUNT(8, 0x01),
            HID_RI_REPORT_SIZE(8, 0x02),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
            HID_RI_REPORT_COUNT(8, 0x01),
            HID_RI_REPORT_SIZE(8, 0x04),
            HID_RI_INPUT(8, HID_IOF_CONSTANT),
            HID_RI_USAGE_PAGE(8, 0x01),    // Generic Desktop
            HID_RI_LOGICAL_MINIMUM(8, 0x00),
            HID_RI_LOGICAL_MAXIMUM(16, 0x0FFF),
            HID_RI_PHYSICAL_MINIMUM(8, 0x00),
            HID_RI_PHYSICAL_MAXIMUM(16, 0x0190),
            HID_RI_UNIT(8, 0x13),
            HID_RI_UNIT_EXPONENT(8, 0x0E),
            HID_RI_USAGE(8, 0x30),         // X
            HID_RI_REPORT_COUNT(8, 0x01),
            HID_RI_REPORT_SIZE(8, 0x10),
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
            HID_RI_PHYSICAL_MAXIMUM(16, 0x0113),
            HID_RI_USAGE(8, 0x31),         // Y
            HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
        HID_RI_END_COLLECTION(0),

        HID_RI_USAGE_PAGE(8, 0x0D),        // Digitizers
        HID_RI_LOGICAL_MINIMUM(8, 0x00),
        HID_RI_LOGICAL_MAXIMUM(8, 0x02),
        HID_RI_USAGE(8, 0x54),             // Contact Count
        HID_RI_REPORT_COUNT(8, 0x01),
        HID_RI_REPORT_SIZE(8, 0x08),
        HID_RI_INPUT(8, HID_IOF_DATA | HID_IOF_VARIABLE | HID_IOF_ABSOLUTE),
    HID_RI_END_COLLECTION(0),
