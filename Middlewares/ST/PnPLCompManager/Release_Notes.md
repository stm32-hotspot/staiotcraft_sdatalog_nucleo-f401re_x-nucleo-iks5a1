---
pagetitle: Release Notes for PnPLCompManager 
lang: en
header-includes: <link rel="icon" type="image/x-icon" href="_htmresc/favicon.png" />
---

::: {.row}
::: {.col-sm-12 .col-lg-4}

<center>
# Release Notes for <mark>PnPLCompManager</mark>
Copyright &copy; 2024  STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com){.logo}
</center>


# Purpose

The **PnPLCompManager** implements the interface used to handle PnP-like commands and properties generated through a Digital Twins Definition Language (DTDL).


:::

::: {.col-sm-12 .col-lg-8}
# Update History

::: {.collapse}
<input type="checkbox" id="collapse-section10" checked aria-hidden="true">
<label for="collapse-section10" aria-hidden="true">__v2.2.2 / 20-Nov-24__</label>
<div>			

## Main Changes

### Patch release

- Inexistent Component error management reshaped for all kind of messages

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section9" checked aria-hidden="true">
<label for="collapse-section9" aria-hidden="true">__v2.2.1 / 20-Sep-24__</label>
<div>			

## Main Changes

### Patch release

- Fixed null pointer exception check
- Added missing "wrong command" management

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section8" checked aria-hidden="true">
<label for="collapse-section8" aria-hidden="true">__v2.2.0 / 19-Jul-24__</label>
<div>			

## Main Changes

### Maintenance release and product update

- Added PnPLCreateLogMessage function to manage log messages (PnPL_Error, PnPL_Warning, PnPL_Info)
- Add multi-threading guard registration method for PnPL: enables users to register custom guard implementations at the application level
- Added comm_interface_id in _PnPLCommand_t struct (useful to route responses to the right communication interface):
	- Added pnpl_ble_responses field in device status json (v2.2.0) and PNPL_BLE_RESPONSES define
	- Removed automatic response for PnPL Commands
	- New generic PnPLSerializeCommandResponse API
- Code cleaning and fixed typo

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section7" aria-hidden="true">
<label for="collapse-section7" aria-hidden="true">__v2.1.2 / 12-Jun-24__</label>
<div>			

## Main Changes

### Patch release

- Add multi-threading guard method for PnPL
- Add usage of pnpl_malloc
- Add PnPLCreateLogMessage function to manage log messages (PnPL_Error, PnPL_Warning, PnPL_Info)


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section6" aria-hidden="true">
<label for="collapse-section6" aria-hidden="true">__v2.1.1 / 12-Apr-24__</label>
<div>			

## Main Changes

### Patch release

- Fixed comp_name array: must be longer (see PnPLParseCommand)
- Renamed PnPL errors defines
- Applyed mcuastyle


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section5" aria-hidden="true">
<label for="collapse-section5" aria-hidden="true">__v2.1.0 / 16-Feb-24__</label>
<div>			

## Main Changes

### Maintenance release

- Bug fixed: return PnPL_Error json in case the component is not found
- Added PnPLSetAllocationFunctions and PnPLFreeSerializedString functions
- Notify with a compiler error message if pnpl_malloc or pnpl_free have already been defined
- Fixed files inclusion chain


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section4" aria-hidden="true">
<label for="collapse-section4" aria-hidden="true">__v2.0.0 / 29-Sep-23__</label>
<div>			

## Main Changes

### Product update

- **First release of PnPLCompManager as ST Middlewares**
- **Added support for SET and COMMAND responses**
- Introduced PNPL_RESPONSES check to manage PnPL Responses: macro to be defined by user in PnPLCompManager_conf.h to enable PnPL responses
- Solved high and medium level MISRA warnings
- Applied mcuAStyle


</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section3" aria-hidden="true">
<label for="collapse-section3" aria-hidden="true">__v1.2.0 / 7-Jul-23__</label>
<div>			

## Main Changes

### Maintenance release

- New PnPLSerializeTelemetry to manage both primitive value and Objects Properties
- Removed old comments

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section2" aria-hidden="true">
<label for="collapse-section2" aria-hidden="true">__v1.1.0 / 24-Mar-23__</label>
<div>			

## Main Changes

### Maintenance release

- Updated get_presentation PnPL Base command: from "system_info" to "get_presentation".

</div>
:::

::: {.collapse}
<input type="checkbox" id="collapse-section1" aria-hidden="true">
<label for="collapse-section1" aria-hidden="true">__v1.0.0 / 2-Dec-22__</label>
<div>			

## Main Changes

### First official release

- First official release

</div>
:::

:::
:::

<footer class="sticky">
::: {.columns}
::: {.column width="95%"}
For complete documentation,
visit: [www.st.com](http://www.st.com/STM32)
:::
::: {.column width="5%"}
<abbr title="Based on template cx566953 version 2.0">Info</abbr>
:::
:::
</footer>