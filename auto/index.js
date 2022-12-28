import * as fs from "fs";
import * as path from "path";
import _ from "lodash";
import * as ncp from "copy-paste";

const layers = `
MAC_BASE,
MAC_FN,
WIN_BASE,
LEFT_HANDED_GAMING,
WIN_FN,
PLUS_ONE,
PLUS_TWO,
PLUS_THREE,
PLUS_FOUR,
PLUS_FIVE
`;

const layerArr = text2Arr(layers);

function text2Arr(t) {
    return t.replace(/\s/g, "").split(",");
}

function getCustomKeycodes(input) {
    const custom_keycodes_map = {
        USER00: { full: "KC_MISSION_CONTROL", abbr: "KC_MCTL" },
        USER01: { full: "KC_LAUNCHPAD", abbr: "KC_LPAD" },
        USER02: { full: "KC_LOPTN", abbr: "" },
        USER03: { full: "KC_ROPTN", abbr: "" },
        USER04: { full: "KC_LCMMD", abbr: "" },
        USER05: { full: "KC_RCMMD", abbr: "" },
        USER06: { full: "KC_SIRI", abbr: "" },
        USER07: { full: "KC_TASK_VIEW", abbr: "KC_TASK" },
        USER08: { full: "KC_FILE_EXPLORER", abbr: "KC_FLXP" },
        USER09: { full: "KC_SCREEN_SHOT", abbr: "KC_SNAP" },
        USER10: { full: "KC_CORTANA", abbr: "KC_CRTA" },
    };
    let upperCaseName = input.toUpperCase();
    return (
        custom_keycodes_map[upperCaseName].abbr ||
        custom_keycodes_map[upperCaseName].full
    );
}

const keymapFile = fs.readFileSync(
    path.resolve("//LCO_DS920/Archive/14客制化/客制化键盘专题/Q3/keychron_q3_2022年12月28日.json"),
    "utf-8"
);

const keymapJSON = JSON.parse(keymapFile);

const arr2C = keymapJSON.layers.map((arrKeymapVIA,layerIndex) => {
    const curLayerName = layerArr[layerIndex];

    // const keymapC = `     KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,    KC_F12,   KC_PSCR,  KC_CRTA,  RGB_MOD,
    // KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,    KC_BSPC,  KC_INS,   KC_HOME,  KC_PGUP,
    // KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,   KC_BSLS,  KC_DEL,   KC_END,   KC_PGDN,
    // KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,             KC_ENT,
    // KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,             KC_RSFT,            KC_UP,
    // KC_LCTL,  KC_LCMD,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_RWIN, MO(WIN_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT`;

    // const arrKeymapC = text2Arr(keymapC);

    const evens = _.pullAt(arrKeymapVIA, 13, 65, 76, 83, 84, 85, 87, 88, 89); //删除对应位数的KC_NO
    // console.log("🚀 ~ file: index.js ~ line 24 ~ evens", evens);

    arrKeymapVIA = _.map(arrKeymapVIA, (e) => {
        // console.log(e);
        const reg = /mo\((\d)*\)/i;
        if (e.match(reg)) {
            let number = e.match(reg)[1];
            return `MO(${layerArr[number]})`;
        }
        if (!e.match(/user/i)) {
            return e;
        } else {
            return getCustomKeycodes(e);
        }
    });

    var swap1 = _.pullAt(arrKeymapVIA, 61)[0]; //拿出第61位
    arrKeymapVIA.splice(15, 0, swap1); //移动到第15位
    var swap2 = _.pullAt(arrKeymapVIA, 62)[0]; //拿出第62位
    arrKeymapVIA.splice(32, 0, swap2); //移动到第32位
    var swap3 = _.pullAt(arrKeymapVIA, 61)[0]; //拿出第61位
    arrKeymapVIA.splice(49, 0, swap3); //移动到第49位
    var swap4 = _.pullAt(arrKeymapVIA, 75)[0]; //拿出第75位
    arrKeymapVIA.splice(86, 0, swap4); //移动到第86位

    var text = arrKeymapVIA.join("\n");
    // ncp.copy(text);
    // console.log(text);
    // console.log(arrKeymapC);

    // _.xor(arrKeymapVIA, arrKeymapC);
    // console.log(arrKeymapVIA.length, arrKeymapC.length);

    // console.log(
    //     "🚀 ~ file: index.js ~ line 25 ~ _.difference(arrKeymapVIA, arrKeymapC)",
    //     _.xor(arrKeymapC, arrKeymapVIA)
    // );

    const curLine2C = `[${curLayerName}] = LAYOUT_ansi_87(${arrKeymapVIA.join(
        ","
    )}),`;

    return curLine2C;
});

console.log(arr2C);
ncp.copy(arr2C.join("\n\n"));
