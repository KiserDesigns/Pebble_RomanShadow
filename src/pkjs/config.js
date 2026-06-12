module.exports = [
  {
    "type": "heading",
    "defaultValue": "Rombra (Roman Umbra) Settings"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Visuals"
      },
      {
        "type": "slider",
        "messageKey": "HourSize",
        "defaultValue": 2,
        "label": "Hour Numeral Size",
        "min": 1,
        "max": 4,
        "step": 0.1
      },
      {
        "type": "color",
        "messageKey": "BackgroundColor",
        "defaultValue": "0xAAAAAA",
        "label": "Background",
        "allowGray": "true"
      },
      {
        "type": "color",
        "messageKey": "HourColor",
        "defaultValue": "0xFFFFFF",
        "label": "Hour Numeral"
      },
      {
        "type": "color",
        "messageKey": "MinuteColor",
        "defaultValue": "0x000000",
        "label": "Minute Shadow",
        "allowGray": "true"
      },
      {
        "type": "color",
        "messageKey": "BacklightColor",
        "defaultValue": "0xFFFFFF",
        "label": "Backlight Color",
        "capabilities": ["PLATFORM_EMERY"]
      },
      {
        "type": "slider",
        "messageKey": "TickSize",
        "label": "5-min Tick Size (0=None)",
        "defaultValue": 3,
        "min": 0,
        "max": 15,
        "step": 1
      },
      {
        "type": "toggle",
        "messageKey": "HourMode",
        "label": "24hr Mode (0-XXIII)",
        "defaultValue": false
      },,
      {
        "type": "toggle",
        "messageKey": "HourBorder",
        "label": "Draw Border around Hour",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "NumeralType",
        "label": "Arabic Numerals",
        "defaultValue": false
      }
      
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Interactions"
      },
      {
        "type": "toggle",
        "messageKey": "HourPulse",
        "label": "Double Pulse each Hour",
        "defaultValue": true
      },
      {
        "type": "slider",
        "messageKey": "VibeStart",
        "label": "AM Hour of First Hour Pulse",
        "defaultValue": 8,
        "min": 0,
        "max": 12,
        "step": 1
      },
      {
        "type": "slider",
        "messageKey": "VibeEnd",
        "label": "PM Hour of Last Hour Pulse",
        "defaultValue": 20,
        "min": 12,
        "max": 23,
        "step": 1
      },
      {
        "type": "slider",
        "messageKey": "VibeOffset",
        "label": "Offset Hour Pulse by X Minutes",
        "defaultValue": 0,
        "min": -5,
        "max": 5,
        "step": 1
      },
      {
        "type": "toggle",
        "messageKey": "BTPulse",
        "label": "Pulse on BT Disconnect",
        "defaultValue": true
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Schedule"
      },
      {
        "type": "text",
        "defaultValue": "Set colors to Black to use Visuals colors."
      },
      {
        "type": "text",
        "defaultValue": "Enable | HH:MM | BG | Hr | Min | Backlight",
        "capabilities": ["PLATFORM_EMERY"]
      },
      {
        "type": "text",
        "defaultValue": "Enable | HH:MM | Background | Hour | Min ",
        "capabilities": ["NOT_PLATFORM_EMERY"]
      },
      {
        "type": "custom-toggle",
        "messageKey": "EN[0]"
      },
      {
        "type": "custom-hour",
        "messageKey": "HR[0]"
      },
      {
        "type": "custom-minute",
        "messageKey": "MIN[0]"
      },
      {
        "type": "custom-color",
        "messageKey": "BG[0]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "NUM[0]"
      },
      {
        "type": "custom-color",
        "messageKey": "UMB[0]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "BL[0]",
        "defaultValue": "0xFFFFFF",
        "capabilities": ["PLATFORM_EMERY"]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "custom-toggle",
        "messageKey": "EN[1]"
      },
      {
        "type": "custom-hour",
        "messageKey": "HR[1]"
      },
      {
        "type": "custom-minute",
        "messageKey": "MIN[1]"
      },
      {
        "type": "custom-color",
        "messageKey": "BG[1]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "NUM[1]"
      },
      {
        "type": "custom-color",
        "messageKey": "UMB[1]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "BL[1]",
        "defaultValue": "0xFFFFFF",
        "capabilities": ["PLATFORM_EMERY"]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "custom-toggle",
        "messageKey": "EN[2]"
      },
      {
        "type": "custom-hour",
        "messageKey": "HR[2]"
      },
      {
        "type": "custom-minute",
        "messageKey": "MIN[2]"
      },
      {
        "type": "custom-color",
        "messageKey": "BG[2]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "NUM[2]"
      },
      {
        "type": "custom-color",
        "messageKey": "UMB[2]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "BL[2]",
        "defaultValue": "0xFFFFFF",
        "capabilities": ["PLATFORM_EMERY"]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "custom-toggle",
        "messageKey": "EN[3]"
      },
      {
        "type": "custom-hour",
        "messageKey": "HR[3]"
      },
      {
        "type": "custom-minute",
        "messageKey": "MIN[3]"
      },
      {
        "type": "custom-color",
        "messageKey": "BG[3]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "NUM[3]"
      },
      {
        "type": "custom-color",
        "messageKey": "UMB[3]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "BL[3]",
        "defaultValue": "0xFFFFFF",
        "capabilities": ["PLATFORM_EMERY"]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "custom-toggle",
        "messageKey": "EN[4]"
      },
      {
        "type": "custom-hour",
        "messageKey": "HR[4]"
      },
      {
        "type": "custom-minute",
        "messageKey": "MIN[4]"
      },
      {
        "type": "custom-color",
        "messageKey": "BG[4]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "NUM[4]"
      },
      {
        "type": "custom-color",
        "messageKey": "UMB[4]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "BL[4]",
        "defaultValue": "0xFFFFFF",
        "capabilities": ["PLATFORM_EMERY"]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "custom-toggle",
        "messageKey": "EN[5]"
      },
      {
        "type": "custom-hour",
        "messageKey": "HR[5]"
      },
      {
        "type": "custom-minute",
        "messageKey": "MIN[5]"
      },
      {
        "type": "custom-color",
        "messageKey": "BG[5]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "NUM[5]"
      },
      {
        "type": "custom-color",
        "messageKey": "UMB[5]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "BL[5]",
        "defaultValue": "0xFFFFFF",
        "capabilities": ["PLATFORM_EMERY"]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "custom-toggle",
        "messageKey": "EN[6]"
      },
      {
        "type": "custom-hour",
        "messageKey": "HR[6]"
      },
      {
        "type": "custom-minute",
        "messageKey": "MIN[6]"
      },
      {
        "type": "custom-color",
        "messageKey": "BG[6]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "NUM[6]"
      },
      {
        "type": "custom-color",
        "messageKey": "UMB[6]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "BL[6]",
        "defaultValue": "0xFFFFFF",
        "capabilities": ["PLATFORM_EMERY"]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "custom-toggle",
        "messageKey": "EN[7]"
      },
      {
        "type": "custom-hour",
        "messageKey": "HR[7]"
      },
      {
        "type": "custom-minute",
        "messageKey": "MIN[7]"
      },
      {
        "type": "custom-color",
        "messageKey": "BG[7]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "NUM[7]"
      },
      {
        "type": "custom-color",
        "messageKey": "UMB[7]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "BL[7]",
        "defaultValue": "0xFFFFFF",
        "capabilities": ["PLATFORM_EMERY"]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "custom-toggle",
        "messageKey": "EN[8]"
      },
      {
        "type": "custom-hour",
        "messageKey": "HR[8]"
      },
      {
        "type": "custom-minute",
        "messageKey": "MIN[8]"
      },
      {
        "type": "custom-color",
        "messageKey": "BG[8]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "NUM[8]"
      },
      {
        "type": "custom-color",
        "messageKey": "UMB[8]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "BL[8]",
        "defaultValue": "0xFFFFFF",
        "capabilities": ["PLATFORM_EMERY"]
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "custom-toggle",
        "messageKey": "EN[9]"
      },
      {
        "type": "custom-hour",
        "messageKey": "HR[9]"
      },
      {
        "type": "custom-minute",
        "messageKey": "MIN[9]"
      },
      {
        "type": "custom-color",
        "messageKey": "BG[9]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "NUM[9]"
      },
      {
        "type": "custom-color",
        "messageKey": "UMB[9]",
        "allowGray": "true"
      },
      {
        "type": "custom-color",
        "messageKey": "BL[9]",
        "defaultValue": "0xFFFFFF",
        "capabilities": ["PLATFORM_EMERY"]
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
