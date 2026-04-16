module.exports = [
  {
    "type": "heading",
    "defaultValue": "Appearance Settings"
  },
  {
    "type": "text",
    "defaultValue": "Adjust Colors, Sizes, and Visibility."
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
        "label": "Hour Numeral",
        "allowGray": "true"
      },
      {
        "type": "color",
        "messageKey": "MinuteColor",
        "defaultValue": "0x000000",
        "label": "Minute Shadow"
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Sizes"
      },
      {
        "type": "slider",
        "messageKey": "HourLength",
        "defaultValue": 7,
        "label": "Hour Hand Length",
        "min": 1,
        "max": 15,
        "step": 1
      },
      {
        "type": "slider",
        "messageKey": "HourWidth",
        "defaultValue": 6,
        "label": "Hour Hand Width",
        "min": 1,
        "max": 15,
        "step": 1
      },
      {
        "type": "slider",
        "messageKey": "MinuteLength",
        "defaultValue": 13,
        "label": "Minute Hand Length",
        "min": 1,
        "max": 15,
        "step": 1
      },
      {
        "type": "slider",
        "messageKey": "MinuteWidth",
        "defaultValue": 4,
        "label": "Minute Hand Width",
        "min": 1,
        "max": 15,
        "step": 1
      },
      {
        "type": "slider",
        "messageKey": "DateSize",
        "defaultValue": 2,
        "label": "Date Size",
        "min": 1,
        "max": 5,
        "step": 1
      },
      {
        "type": "slider",
        "messageKey": "DateThickness",
        "defaultValue": 2,
        "label": "Date Thickness",
        "min": 1,
        "max": 5,
        "step": 1
      },
      {
        "type": "slider",
        "messageKey": "BatteryLength",
        "defaultValue": 4,
        "label": "Battery Bar Length",
        "min": 1,
        "max": 10,
        "step": 1
      },
      {
        "type": "slider",
        "messageKey": "BatteryWidth",
        "defaultValue": 3,
        "label": "Battery Bar Width",
        "min": 2,
        "max": 6,
        "step": 1
      }
    ]
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Visibility"
      },
      {
        "type": "toggle",
        "messageKey": "ShowDate",
        "label": "Show Date",
        "defaultValue": true
      },
      {
        "type": "toggle",
        "messageKey": "DateFront",
        "label": "Date in Front",
        "defaultValue": true
      },
      {
        "type": "toggle",
        "messageKey": "ShowBattery",
        "label": "Show Battery",
        "defaultValue": true
      },
      {
        "type": "toggle",
        "messageKey": "BatteryFront",
        "label": "Battery in Front",
        "defaultValue": false
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];
