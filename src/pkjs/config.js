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
        "max": 3,
        "step": 1
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
        "type": "slider",
        "messageKey": "TickSize",
        "label": "5-min Tick Size (0=None)",
        "defaultValue": 3,
        "min": 0,
        "max": 23,
        "step": 1
      },
      {
        "type": "toggle",
        "messageKey": "HourMode",
        "label": "24hr Mode (0-XXIII)",
        "defaultValue": false
      },
      {
        "type": "color",
        "messageKey": "BacklightColor",
        "defaultValue": "0xFFFFFF",
        "label": "Backlight Color",
        "capabilities": ["PLATFORM_EMERY"]
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
  }
];
