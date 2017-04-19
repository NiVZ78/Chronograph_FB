'use strict';
/* eslint-disable quotes */
module.exports = [
  {
    "type": "heading",
    "id": "main-heading",
    "defaultValue": "Chronograph-FB Configuration",
    "size": 1
  },
  {
    "type": "text",
    "defaultValue": "Use this configuration page to choose the settings you would like applied to this app."
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Settings"
      },
      {
        "type": "select",
        "messageKey": "DATE_FORMAT",
        "defaultValue": "0",
        "label": "Date Format",
        "description": " ",
        "options": [
          { 
            "label": "DD/MM",
            "value": "0" 
          },
          { 
            "label": "MM/DD",
            "value": "1" 
          }
        ],
      },
      {
        "type": "color",
        "capabilities": ["COLOR"],
        "messageKey": "SECOND_COLOR",
        "defaultValue": "55AAFF",
        "label": "Second Color",
        "sunlight": false
      },
      {
        "type": "color",
        "capabilities": ["COLOR"],
        "messageKey": "MINUTE_COLOR",
        "defaultValue": "FFFF00",
        "label": "Minute Color",
        "sunlight": false
      },
      {
        "type": "color",
        "capabilities": ["COLOR"],
        "messageKey": "HOUR_COLOR",
        "defaultValue": "FF0000",
        "label": "Hour Color",
        "sunlight": false
      },
      {
        "type": "color",
        "capabilities": ["COLOR"],
        "messageKey": "SECOND_TEXT_COLOR",
        "defaultValue": "555555",
        "label": "Second Text Color",
        "sunlight": false
      },
      {
        "type": "color",
        "capabilities": ["COLOR"],
        "messageKey": "DATE_TEXT_COLOR",
        "defaultValue": "FFFFFF",
        "label": "Date Text Color",
        "sunlight": false
      },
      {
        "type": "text",
        "defaultValue": "<h6>The settings above will only change on the watch after pressing the SAVE button below.</h6>",
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save"
  }
];