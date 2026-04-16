module.exports = [
  {
    "type": "heading",
    "defaultValue": "Roman Shadow Settings"
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
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Colors"
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
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Vibrations"
      },
      {
        "type": "toggle",
        "messageKey": "HourPulse",
        "label": "Double Pulse each Hour",
        "defaultValue": true
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
