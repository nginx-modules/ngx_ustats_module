
/**
 * Copyright (c) 2010-2012 Aleksey "0xc0dec" Fedotov
 * http://skbkontur.ru
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


const char HTML[] =
		"<!DOCTYPE HTML PUBLIC \\\"-//W3C//DTD HTML 4.01 Transitional//EN\\\">\n"
		"<html xmlns=\\\"http://www.w3.org/1999/xhtml\\\">\n"
		"    <head>\n"
		"        <style type=\"text/css\">\n"
		"            .statsTable\n"
		"            {\n"
		"                border-collapse: collapse;\n"
		"                border-width: 1px;\n"
		"                border-style: outset;\n"
		"                font-family: 'lucida grande';\n"
		"                font-size: 15px;\n"
		"                width: %s;\n"
		"                height: %s;\n"
		"            }\n"
		"\n"
		"            .cellCommon\n"
		"            {\n"
		"                border-width: 1px;\n"
		"                border-style: solid;\n"
		"                border-color: black;\n"
		"                padding: 4px;\n"
		"                text-align: center;\n"
		"            }\n"
		"\n"
		"            .cellHeader\n"
		"            {\n"
		"                background-color: #12B2C4;\n"
		"            }\n"
		"\n"
		"            .cellBlacklisted\n"
		"            {\n"
		"                background-color: red;\n"
		"            }\n"
		"\n"
		"            .cellDisabled\n"
		"            {\n"
		"                border-color: black;\n"
		"                color: #A8A8A8;\n"
		"            }\n"
		"\n"
		"            .cellNoLastFail\n"
		"            {\n"
		"                color: #EEEEEE;\n"
		"            }\n"
		"\n"
		"            .cellUpstream\n"
		"            {\n"
		"            }\n"
		"\n"
		"			 .cellUpstreamOne\n"
		"			 {\n"
		"			 	 background-color: #3EC7D6;\n"
		"			 }"
		"\n"
		"			 .cellUpstreamTwo\n"
		"			 {\n"
		"			 	 background-color: #12A2B3;\n"
		"			 }"
		"\n"
		"            .cellImplicitUpstream\n"
		"            {\n"
		"                color: #FFFFFF;\n"
		"            }\n"
		"\n"
		"            .cellSortAsc\n"
		"            {\n"
		"            }\n"
		"\n"
		"\n"
		"            .cellSortDesc\n"
		"            {\n"
		"            }\n"
		"\n"
		"            .cellSortTrigger\n"
		"            {\n"
		"                cursor: pointer;\n"
		"            }\n"
		"\n"
		"            button\n"
		"            {\n"
		"                border: 1px outset;\n"
		"                font-family: 'Comic Sans MS', 'sans serif';\n"
		"                cursor: pointer;\n"
		"            }\n"
		"\n"
		"            .down\n"
		"            {\n"
		"                border: 1px inset;\n"
		"                background: buttonhighlight;\n"
		"                color: black;\n"
		"            }\n"
		"\n"
		"            .up\n"
		"            {\n"
		"                border: 1px outset;\n"
		"                color: #AAAAAA;\n"
		"            }\n"
		"        </style>\n"
		"\n"
		"        <script type=\"text/javascript\">\n"
		"            var REFRESH_INTERVAL = %d;\n"
		"            var TABLE_COLUMNS_COUNT = 13;\n"
		"\n"
		"            var updating = 0;\n"
		"            var sortInfo = new Array();\n"
		"\n"
		"            var originalTable = document.createElement(\"table\");\n"
		"\n"
		"            var req = createRequester();\n"
		"            \n"
		"\n"
		"            function createRequester()\n"
		"            {\n"
		"                var result;\n"
		"\n"
		"                try\n"
		"                {\n"
		"                    result = new ActiveXObject(\"Msxml2.XMLHTTP\");\n"
		"                }\n"
		"                catch (e)\n"
		"                {\n"
		"                    try\n"
		"                    {\n"
		"                        result = new ActiveXObject(\"Microsoft.XMLHTTP\");\n"
		"                    }\n"
		"                    catch (e)\n"
		"                    {\n"
		"                        result = false;\n"
		"                    }\n"
		"                    result = false;\n"
		"                }\n"
		"\n"
		"                if (!result && typeof XMLHttpRequest != 'undefined')\n"
		"                    result = new XMLHttpRequest();\n"
		"\n"
		"                return result;\n"
		"            }\n"
		"\n"
		"            /**\n"
		"             * Adjusts sort status text for column with index cellIndex\n"
		"             * in upstream indicated by upstreamCell\n"
		"             */\n"
		"            function adjustSortStatusText(startIndex, endIndex)\n"
		"            {\n"
		"                var table = document.getElementById(\"statsTable\");\n"
		"                var row = table.rows[startIndex];\n"
		"                var cell = row.cells[0];\n"
		"\n"
		"                var sortStatus = undefined;\n"
		"                if (cell.className.indexOf(\"sortInfo$none\") != -1)\n"
		"                    sortStatus = \"none\";\n"
		"                else if (cell.className.indexOf(\"sortInfo$asc\") != -1)\n"
		"                    sortStatus = \"asc\";\n"
		"                else if (cell.className.indexOf(\"sortInfo$desc\") != -1)\n"
		"                    sortStatus = \"desc\";\n"
		"\n"
		"                var r = /sortInfo\\$.*\\$(.*)$/.exec(cell.className);\n"
		"                var ind = parseInt(r[1]);\n"
		"                if (ind == -1)\n"
		"                    return;\n"
		"\n"
		"                do\n"
		"                {\n"
		"                    cell = (row.cells.length == TABLE_COLUMNS_COUNT) ? row.cells[ind + 1] : row.cells[ind];\n"
		"                    var lastSpaceInd = cell.innerHTML.lastIndexOf(\" \");\n"
		"                    switch (sortStatus)\n"
		"                    {\n"
		"                        case \"none\":\n"
		"                            if (lastSpaceInd == -1)\n"
		"                                break;\n"
		"                            cell.innerHTML = cell.innerHTML.substring(0, lastSpaceInd);\n"
		"                            break;\n"
		"\n"
		"                        case \"asc\":\n"
		"                            if (lastSpaceInd != -1 && lastSpaceInd == cell.innerHTML.length - 2) // last fail time cells have spaces\n"
		"                                cell.innerHTML = cell.innerHTML.substring(0, lastSpaceInd);\n"
		"                            cell.innerHTML += \" &darr;\";\n"
		"                            break;\n"
		"\n"
		"                        case \"desc\":\n"
		"                            cell.innerHTML += \" &uarr;\";\n"
		"                            break;\n"
		"                    }\n"
		"\n"
		"                    row = row.nextSibling;\n"
		"                }\n"
		"                while (row && row.cells.length < TABLE_COLUMNS_COUNT);\n"
		"            }\n"
		"\n"
		"\n"
		"            /**\n"
		"             * Adjusts upstream cell meta info to reflect sort status\n"
		"             */\n"
		"            function toggleSortStatus(upstreamCell, cellIndex)\n"
		"            {\n"
		"                // extract current sort info from className attribute\n"
		"                var sortInfo = upstreamCell.className.substring(upstreamCell.className.indexOf(\"sortInfo$\"));\n"
		"                                                   \n"
		"                var newSortInfo = null;\n"
		"                if (sortInfo.indexOf(\"asc\") != -1)\n"
		"                    newSortInfo = \"sortInfo$none$\" + cellIndex;\n"
		"                else if (sortInfo.indexOf(\"desc\") != -1)\n"
		"                    newSortInfo = \"sortInfo$asc$\" + cellIndex;\n"
		"                else\n"
		"                    newSortInfo = \"sortInfo$desc$\" + cellIndex;\n"
		"\n"
		"                upstreamCell.className = upstreamCell.className.replace(sortInfo, newSortInfo);\n"
		"            }\n"
		"\n"
		"\n"
		"            function saveSortInfo()\n"
		"            {\n"
		"                // sortInfo here is empty\n"
		"                var table = document.getElementById(\"statsTable\");\n"
		"                \n"
		"                if (!table)\n"
		"                    return;\n"
		"\n"
		"                for (var i = 0; i < table.rows.length; ++i)\n"
		"                {\n"
		"                    var row = table.rows[i];\n"
		"                    if (row.cells[0].className.indexOf(\"cellUpstream\") != -1) // upstream name cell\n"
		"                        sortInfo[i] = row.cells[0].className;\n"
		"                }\n"
		"            }\n"
		"\n"
		"\n"
		"            function restoreSorting()\n"
		"            {\n"
		"                var table = document.getElementById(\"statsTable\");\n"
		"\n"
		"                for (var i in sortInfo)\n"
		"                {\n"
		"                    table.rows[i].cells[0].className = sortInfo[i];\n"
		"                    var indices = sort(table.rows[i].firstChild);\n"
		"                    if (indices[0] != indices[1])\n"
		"                        adjustSortStatusText(parseInt(indices[0]), parseInt(indices[1]));\n"
		"                }\n"
		"\n"
		"                sortInfo.length = 0;\n"
		"            }\n"
		"\n"
		"\n"
		"            function onSortTriggerClick(cell)\n"
		"            {\n"
		"                // find upstream cell name\n"
		"                var usRow = cell.parentNode;\n"
		"                while (usRow.cells.length != TABLE_COLUMNS_COUNT)\n"
		"                    usRow = usRow.previousSibling;\n"
		"                \n"
		"                // Cell index. For rows with upstream\n"
		"                // name cell, that cell is not included\n"
		"                var cellIndex = cell.cellIndex;\n"
		"                if (cell.parentNode.cells.length == TABLE_COLUMNS_COUNT) // row with upstream name cell\n"
		"                    cellIndex--;\n"
		"\n"
		"                // Remove sorting from previously sorted column\n"
		"                var sortInfo = usRow.cells[0].className.substring(usRow.cells[0].className.indexOf(\"sortInfo$\"));\n"
		"                var r = /sortInfo\\$.*\\$(.*)/.exec(sortInfo);\n"
		"                var oldIndex = parseInt(r[1]);\n"
		"                if (oldIndex != -1 && cellIndex != oldIndex)\n"
		"                {\n"
		"                    usRow.cells[0].className = usRow.cells[0].className.replace(sortInfo, \"sortInfo$none$-1\");\n"
		"                    var indices = sort(usRow.firstChild);\n"
		"                    if (indices[0] != indices[1])\n"
		"                        adjustSortStatusText(parseInt(indices[0]), parseInt(indices[1]));\n"
		"                }\n"
		"\n"
		"                toggleSortStatus(usRow.firstChild, cellIndex);\n"
		"                \n"
		"                var indices = sort(usRow.firstChild);\n"
		"                if (indices[0] != indices[1])\n"
		"                    adjustSortStatusText(parseInt(indices[0]), parseInt(indices[1]));\n"
		"            }\n"
		"\n"
		"\n"
		"            // Sorts backends for the given upstream, using sort info\n"
		"            // saved in upstream name cell class\n"
		"            function sort(usCell)\n"
		"            {\n"
		"                var sortInfo = usCell.className.substring(usCell.className.indexOf(\"sortInfo$\"));\n"
		"\n"
		"                // \"sortInfo$<mode>$<column_index>\"\n"
		"                var rx = /sortInfo\\$(.*)\\$(.*)/;\n"
		"                var result = rx.exec(sortInfo);\n"
		"\n"
		"                var mode = result[1];\n"
		"                var columnIndex = parseInt(result[2]);\n"
		"\n"
		"                var startRow = usCell.parentNode;\n"
		"\n"
		"                var endRow = startRow;\n"
		"                // iterate till the next upstream starting row\n"
		"                while (endRow.nextSibling && endRow.nextSibling.cells.length < TABLE_COLUMNS_COUNT)\n"
		"                    endRow = endRow.nextSibling;\n"
		"\n"
		"                var startIndex = startRow.rowIndex;\n"
		"                var endIndex = endRow.rowIndex;\n"
		"\n"
		"                if (startIndex == endIndex) // upstream with one backend\n"
		"                    return [startIndex, endIndex];\n"
		"\n"
		"                if (mode == \"none\")\n"
		"                {\n"
		"                    // nothing to sort, only restore table\n"
		"                    var table = document.getElementById(\"statsTable\");\n"
		"                    for (var i = startIndex; i <= endIndex; ++i)\n"
		"                    {\n"
		"                        table.rows[i].innerHTML = originalTable.rows[i].innerHTML;\n"
		"                    }\n"
		"                    return [startIndex, endIndex];\n"
		"                }\n"
		"\n"
		"                var table = usCell.parentNode.parentNode;\n"
		"\n"
		"                var tmpArr = new Array();\n"
		"                for (var i = startIndex; i <= endIndex; ++i)\n"
		"                    tmpArr.push(table.rows[i]);\n"
		"\n"
		"                var usStartRow = undefined;\n"
		"                var newUsStartRow = undefined;\n"
		"\n"
		"                tmpArr.sort(function(a, b)\n"
		"                {\n"
		"                    if (a.cells.length == TABLE_COLUMNS_COUNT)\n"
		"                        usStartRow = a;\n"
		"                    else if (b.cells.length == TABLE_COLUMNS_COUNT)\n"
		"                        usStartRow = b;\n"
		"\n"
		"                    var aCell = (a.cells.length == TABLE_COLUMNS_COUNT) ? a.cells[columnIndex + 1] : a.cells[columnIndex];\n"
		"                    var bCell = (b.cells.length == TABLE_COLUMNS_COUNT) ? b.cells[columnIndex + 1] : b.cells[columnIndex];\n"
		"                    \n"
		"                    if (columnIndex != TABLE_COLUMNS_COUNT - 3) // all except last fail\n"
		"                    {\n"
		"                        var result = parseInt(aCell.textContent) - parseInt(bCell.textContent)\n"
		"                        return (mode == \"asc\") ? result : (mode == \"desc\") ? -result : 0;\n"
		"                    }\n"
		"                    else/* if (columnIndex == TABLE_COLUMNS_COUNT - 3) */\n"
		"                    {\n"
		"                        var result = (aCell.textContent > bCell.textContent) ? 1 :\n"
		"                                     (aCell.textContent === bCell.textContent) ? 0 : -1;\n"
		"                        return (mode == \"asc\") ? result : (mode == \"desc\") ? -result : 0;\n"
		"                    }\n"
		"                });\n"
		"\n"
		"                // Remove upstream name cell from old upstream header row\n"
		"                var usCell = usStartRow.cells[0];\n"
		"                usStartRow.removeChild(usCell);\n"
		"                tmpArr[0].insertBefore(usCell, tmpArr[0].cells[0]);\n"
		"\n"
		"                while (tmpArr.length > 0)\n"
		"                    table.insertBefore(tmpArr.pop(), table.rows[startIndex]);\n"
		"\n"
		"                return [startIndex, endIndex];\n"
		"            }\n"
		"\n"
		"\n"
		"            function requestData(url)\n"
		"            {\n"
		"                req.open(\"GET\", url, false);\n"
		"                req.send(null);\n"
		"                return req.responseText;\n"
		"            }\n"
		"\n"
		"\n"
		"            function buildTable(data)\n"
		"            {\n"
		"                data = eval('(' + data + ')');\n"
		"\n"
		"                var headers = [\"Upstream\", \"Backend\", \"Requests\", \"HTTP 499\", \"HTTP 500\", \"HTTP 503\", \"TCP errors\", \"HTTP<br/>read timeouts\",\n"
		"                               \"HTTP<br/>write timeouts\", \"Fail timeouts, sec.\", \"Max fails\", \"Last fail\", \"Total fails\"];\n"
		"\n"
		"                var table = document.createElement(\"table\");\n"
		"\n"
		"                var updateRow = document.createElement(\"tr\");\n"
		"                var updateHeader = document.createElement(\"th\");\n"
		"                updateHeader.id = \"updateStatus\";\n"
		"                updateHeader.className = \"cellCommon cellHeader\";\n"
		"                updateHeader.setAttribute(\"colspan\", headers.length);\n"
		"                updateRow.appendChild(updateHeader);\n"
		"                table.appendChild(updateRow);\n"
		"\n"
		"                var headerRow = document.createElement(\"tr\");\n"
		"                for (var h in headers)\n"
		"                {\n"
		"                    var headerCell = document.createElement(\"th\");\n"
		"                    headerCell.className = \"cellCommon cellHeader\";\n"
		"                    headerCell.innerHTML = headers[h];\n"
		"                    headerRow.appendChild(headerCell);\n"
		"                }\n"
		"                table.appendChild(headerRow);\n"
		"                \n"
		"                // Upstreams\n"
		"				 var usn = 0; // for coloring upstreams\n"
		"                for (var us in data)\n"
		"                {\n"
		"                    var usNameCell = document.createElement(\"th\");\n"
		"                    usNameCell.textContent = us;\n"
		"                    usNameCell.className = \"cellCommon cellHeader\";\n"
		"                    if (data[us][data[us].length - 1] == 1)\n"
		"                        usNameCell.className += \" cellImplicitUpstream\";\n"
		"\n"
		"					 if (usn %% 2 == 0)\n"
		"					 	 usNameCell.className += \" cellUpstreamOne\";\n"
		"					 else\n"
		"					 	 usNameCell.className += \" cellUpstreamTwo\";\n"
		"					 usn++;\n"
		"\n"
		"                    // Upstream cell contents\n"
		"                    usNameCell.setAttribute(\"rowspan\", data[us].length - 1);\n"
		"\n"
		"                    var firstUpsteamBackend = null;\n"
		"\n"
		"                    // Backends\n"
		"                    for (var b in data[us])\n"
		"                    {\n"
		"                        if (b == data[us].length - 1)\n"
		"                            break;\n"
		"\n"
		"                        var backendRow = document.createElement(\"tr\");\n"
		"\n"
		"                        var disabled = data[us][b][1];\n"
		"                        var blacklisted = data[us][b][2];\n"
		"\n"
		"                        // Backend parameters\n"
		"                        for (var param in data[us][b])\n"
		"                        {\n"
		"                            if (param == 1 || param == 2) // disabled or blacklisted\n"
		"                                continue;\n"
		"\n"
		"                            var paramCell = document.createElement(\"td\");\n"
		"                            paramCell.className = \"cellCommon\";\n"
		"\n"
		"                            if (disabled)\n"
		"                                paramCell.className += \" cellDisabled\";\n"
		"\n"
		"                            if (blacklisted)\n"
		"                                paramCell.className += \" cellBlacklisted\";\n"
		"\n"
		"                            // Current parameter is the last file time, and next (total fails) equals to 0\n"
		"                            if (((param == data[us][b].length - 2) && (data[us][b][parseInt(param) + 1] == 0)) ||\n"
		"                                ((param == data[us][b].length - 1) && (data[us][b][param] == 0)))\n"
		"                            {\n"
		"                                paramCell.className += \" cellNoLastFail\";\n"
		"                            }\n"
		"\n"
		"                            if (param == 0) // backend name\n"
		"                            {\n"
		"                                var nameParts = /(.*) \\((.*)\\)/.exec(data[us][b][param]);\n"
		"                                if (nameParts)\n"
		"                                    paramCell.innerHTML = nameParts[1] + \"<br/>(\" + nameParts[2] + \")\";\n"
		"                                else\n"
		"                                    paramCell.textContent = data[us][b][param];\n"
		"                            }\n"
		"                            else\n"
		"                                paramCell.textContent = data[us][b][param];\n"
		"\n"
		"                            backendRow.appendChild(paramCell);\n"
		"\n"
		"                            // Detect what column this parameter resides in and allow/disallow sorting\n"
		"                            if (param != 0 && param != 10 && param != 11) // not name, not fail timeout and not max fails\n"
		"                            {\n"
		"                                paramCell.className += \" cellSortTrigger\";\n"
		"                                paramCell.setAttribute(\"onclick\", \"onSortTriggerClick(this);\");\n"
		"                            }\n"
		"                        }\n"
		"                        \n"
		"                        if (b == 0) // first upstream row\n"
		"                        {\n"
		"                            backendRow.insertBefore(usNameCell, backendRow.firstChild);\n"
		"                            usNameCell.className += \" cellUpstream\";\n"
		"                            usNameCell.className += \" sortInfo$none$-1\";\n"
		"                        }\n"
		"  \n"
		"                        table.appendChild(backendRow);\n"
		"                    }\n"
		"                }\n"
		"\n"
		"                var content = document.getElementById(\"content\");\n"
		"                content.innerHTML = \"<table class=\\\"statsTable\\\" id=\\\"statsTable\\\">\" + table.innerHTML + \"</table>\"; // WTF\n"
		"\n"
		"                originalTable.innerHTML = table.innerHTML;\n"
		"            }\n"
		"\n"
		"\n"
		"            function updateTable(data)\n"
		"            {\n"
		"                if (updating == 0)\n"
		"                {\n"
		"                    updating = 1;\n"
		"\n"
		"                    saveSortInfo();\n"
		"                    buildTable(data);\n"
		"                    restoreSorting();\n"
		"                    updating = 0;\n"
		"\n"
		"                    // Update last update time\n"
		"                    var statusHeader = document.getElementById(\"updateStatus\");\n"
		"                    var now = new Date();\n"
		"                    statusHeader.innerHTML = \"Last update: \" + now.getHours() + \":\" + now.getMinutes() + \":\" + now.getSeconds();\n"
		"                }\n"
		"            }\n"
		"\n"
		"\n"
		"            function onTimer()\n"
		"            {\n"
		"                var data = requestData(\"?json\");\n"
		"                updateTable(data);\n"
		"            }\n"
		"\n"
		"\n"
		"            window.onload = function()\n"
		"            {\n"
		"                onTimer();\n"
		"                setInterval(onTimer, REFRESH_INTERVAL); // TODO request parameters from nginx\n"
		"            }\n"
		"        </script>\n"
		"    </head>\n"
		"\n"
		"    <body>\n"
		"        <table width=\"100%%\" height=\"100%%\">\n"
		"            <tr>\n"
		"                <td align=\"center\" valign=\"middle\">\n"
		"                    <div id=\"content\"></div>\n"
		"                </td>\n"
		"            </tr>\n"
		"        </table>\n"
		"    </body>\n"
		"</html>\n";

/**
 * Shared memory used to store statistics
 */
ngx_shm_zone_t * stats_data = NULL;
static size_t stats_data_size = 0;



typedef struct
{
    /**
     * HTML table width and height.
     * Less or equal 100 - percent value, greater than 100 - pixel value.
     * 70 by default.
     */
    ngx_uint_t html_table_width;
    ngx_uint_t html_table_height;
    /** Page refresh interval, milliseconds. 5000 by default */
    ngx_uint_t refresh_interval;
} ngx_http_ustats_loc_conf_t;



static void * ngx_http_ustats_create_loc_conf(ngx_conf_t *cf);
static char * ngx_http_ustats_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);

static char * ngx_http_ustats(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
//static char * ngx_http_shm_size(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_int_t ngx_http_ustats_handler(ngx_http_request_t *r);

static ngx_buf_t * ngx_http_ustats_create_response_json(ngx_http_request_t * r);
static ngx_buf_t * ngx_http_ustats_create_response_html(ngx_http_request_t * r);



static ngx_command_t  ngx_http_ustats_commands[] =
{
    { 
        ngx_string("ustats"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_http_ustats,
        0,
        0,
        NULL 
    },

    {
        ngx_string("ustats_html_table_width"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_num_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_ustats_loc_conf_t, html_table_width),
        NULL
    },

    {
        ngx_string("ustats_html_table_height"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_num_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_ustats_loc_conf_t, html_table_height),
        NULL
    },

    {
        ngx_string("ustats_refresh_interval"),
        NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
        ngx_conf_set_num_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_ustats_loc_conf_t, refresh_interval),
        NULL
    },

    ngx_null_command
};



static ngx_http_module_t  ngx_http_ustats_module_ctx =
{
    NULL,                                  /* preconfiguration */
    NULL,					               /* postconfiguration */

    NULL,							       /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    ngx_http_ustats_create_loc_conf,       /* create location configuration */
    ngx_http_ustats_merge_loc_conf         /* merge location configuration */
};



ngx_module_t  ngx_http_ustats_module =
{
    NGX_MODULE_V1,
    &ngx_http_ustats_module_ctx,           /* module context */
    ngx_http_ustats_commands,              /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


/*****************************************************************************/
/*****************************************************************************/


static void * ngx_http_ustats_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_ustats_loc_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_ustats_loc_conf_t));
    if (conf == NULL)
        return NGX_CONF_ERROR;

    conf->html_table_width = NGX_CONF_UNSET_UINT;
    conf->html_table_height = NGX_CONF_UNSET_UINT;
    conf->refresh_interval = NGX_CONF_UNSET_UINT;

    return conf;
}


/*****************************************************************************/
/*****************************************************************************/


static char* ngx_http_ustats_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_ustats_loc_conf_t *prev = parent;
    ngx_http_ustats_loc_conf_t *conf = child;

    ngx_conf_merge_uint_value(conf->html_table_width, prev->html_table_width, 70);
    ngx_conf_merge_uint_value(conf->html_table_height, prev->html_table_height, 70);
    ngx_conf_merge_uint_value(conf->refresh_interval, prev->refresh_interval, 5000);

    if (conf->html_table_width < 1)
    {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "html table width must be >= 1");
        return NGX_CONF_ERROR;
    }

    if (conf->html_table_height < 1)
    {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "html table height must be >= 1");
        return NGX_CONF_ERROR;
    }

    if (conf->refresh_interval < 1)
    {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "page refresh interval must be >= 1");
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}


/*****************************************************************************/
/*****************************************************************************/


static ngx_int_t ngx_http_ustats_init_shm(ngx_shm_zone_t * shm_zone, void * data)
{
	if (data)
	{
		shm_zone->data = data;
		return NGX_OK;
	}

	ngx_slab_pool_t *shpool = (ngx_slab_pool_t*)shm_zone->shm.addr;

	void *new_block = ngx_slab_alloc(shpool, stats_data_size);
	memset(new_block, 0, stats_data_size);

	shpool->data = new_block;
	shm_zone->data = new_block;

	return NGX_OK;
}


/*****************************************************************************/
/*****************************************************************************/


static char *ngx_http_ustats(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t  *clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);

    clcf->handler = ngx_http_ustats_handler;

    if (cf->args->nelts == 1)
    {
    	ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "ustats: no shared memory size specified");
    	return NGX_CONF_ERROR;
    }

    ssize_t size = 0;

    ngx_str_t size_arg = ((ngx_str_t*)cf->args->elts)[1];

    if (ngx_strncmp(size_arg.data, "memsize=", 8) == 0)
    {
    	ngx_str_t size_str;

    	size_str.len = size_arg.len - 8;

    	if (size_str.len == 0)
    	{
    		ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "ustats: no shared memory size specified");
    		return NGX_CONF_ERROR;
    	}

    	size_str.data = size_arg.data + 8;

    	size = ngx_parse_size(&size_str);
    	if (size == NGX_ERROR)
		{
			ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "ustats: unable to parse shared memory size");
			return NGX_CONF_ERROR;
		}
    }
    else
    {
    	ngx_conf_log_error(NGX_LOG_EMERG, cf, 0, "ustats: no shared memory size specified");
    	return NGX_CONF_ERROR;
    }

    if (size < (int)ngx_pagesize)
	{
		ngx_conf_log_error(NGX_LOG_WARN, cf, 0, "ustats: shared memory size must be at least %udB", ngx_pagesize);
		size = ngx_pagesize;
	}

	if (stats_data_size && stats_data_size != (size_t)size)
		ngx_conf_log_error(NGX_LOG_WARN, cf, 0, "ustats: cannot change shared memory area without restart, ignoring changes");
	else
		stats_data_size = size;

    ngx_str_t * shm_name = NULL;
	shm_name = ngx_palloc(cf->pool, sizeof(*shm_name));
	shm_name->len = sizeof("stats_data");
	shm_name->data = (unsigned char*)"stats_data";

	if (stats_data_size == 0)
		stats_data_size = ngx_pagesize;

	stats_data = ngx_shared_memory_add(cf, shm_name, stats_data_size + 4 * ngx_pagesize, &ngx_http_ustats_module);

	if (stats_data == NULL)
		return NGX_CONF_ERROR;

	stats_data->init = ngx_http_ustats_init_shm;

    return NGX_CONF_OK;
}


/*****************************************************************************/
/*****************************************************************************/


static ngx_int_t ngx_http_ustats_handler(ngx_http_request_t *r)
{
    ngx_int_t rc;
    ngx_buf_t *b = NULL;
    ngx_chain_t out;

    if (r->method != NGX_HTTP_GET && r->method != NGX_HTTP_HEAD)
        return NGX_HTTP_NOT_ALLOWED;

    rc = ngx_http_discard_request_body(r);

    if (rc != NGX_OK)
        return rc;

    ngx_str_set(&r->headers_out.content_type, "text/plain");

    if (r->method == NGX_HTTP_HEAD)
    {
        r->headers_out.status = NGX_HTTP_OK;

        rc = ngx_http_send_header(r);

        if (rc == NGX_ERROR || rc > NGX_OK || r->header_only)
            return rc;
    }

    // Send HTML or simple JSON
    ngx_uint_t send_json = 0;
    if (r->args.data)
    	send_json = ngx_strncmp(r->args.data, "?json", 5) ? 1 : 0;

    if (send_json)
    {
    	ngx_str_set(&r->headers_out.content_type, "text/plain");
    	b = ngx_http_ustats_create_response_json(r);
    }
    else
    {
    	ngx_str_set(&r->headers_out.content_type, "text/html");
    	b = ngx_http_ustats_create_response_html(r);
    }

    if (b == NULL)
        return NGX_HTTP_INTERNAL_SERVER_ERROR;

    out.buf = b;
    out.next = NULL;

    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = b->last - b->pos;

    b->last_buf = 1;

    rc = ngx_http_send_header(r);

    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only)
        return rc;

    return ngx_http_output_filter(r, &out);
}


/*****************************************************************************/
/*****************************************************************************/



static ngx_buf_t * ngx_http_ustats_create_response_json(ngx_http_request_t * r)
{
	unsigned i, k;
	size_t size = 0;
	ngx_buf_t * b = NULL;
	time_t now = ngx_time();

	ngx_http_upstream_main_conf_t * conf = ngx_http_get_module_main_conf(r, ngx_http_upstream_module);

	/**
	 * Calculate size
	 */
	size += sizeof("{\n");

	for (i = 0; i < conf->upstreams.nelts; ++i)
	{
		ngx_http_upstream_srv_conf_t *uscf = ((ngx_http_upstream_srv_conf_t**)conf->upstreams.elts)[i];
		ngx_http_upstream_rr_peers_t *peers = uscf->peer.data;

		// upstream name
		size += sizeof("	\"\":\n");
		size += (uscf->host.len + 1) * sizeof(u_char);

		// array of backends
		size += sizeof("	[\n");

		for (k = 0; k < peers->number; ++k)
		{
			// Peer name
			size += sizeof("		[\"\", ");
			// <peer_name> (server_conf_name || upstream_name) if there is more than one peer per backend
			if (!peers->peer[k].server || peers->peer[k].server->naddrs > 1)
			{
				size += (peers->peer[k].name.len + 1) * sizeof(u_char);
				size += sizeof(" ()");
				size += (!peers->peer[k].server)
							? (uscf->host.len + 1) * sizeof(u_char) // for implicit backends
							// for the (good of) rest (of us, except the ones who are dead) (c)
							: (peers->peer[k].server->name.len + 1) * sizeof(u_char);
			}
			// one server <-> one peer - write server config name
			else
			{
				size += (peers->peer[k].server->name.len + 1) * sizeof(u_char);
			}

			// disabled and blacklisted
			size += (sizeof(ngx_uint_t) + sizeof(", ")) * 2;

			// numeric parameters
			size += (sizeof(ngx_uint_t) + sizeof(", ")) * 10;

			// failed access time string
			size += sizeof(u_char) * 24 + sizeof("\"\"");

			size += sizeof("],\n");
		}

		// implicit upstream
		size += sizeof("		");
		size += sizeof(ngx_uint_t);

		size += sizeof("	],\n");
	}

	size += sizeof("}");

	b = ngx_create_temp_buf(r->pool, size);

	b->last = ngx_sprintf(b->last, "{\n");

	/**
	 * Fill data
	 */
	for (i = 0; i < conf->upstreams.nelts; ++i)
	{
		ngx_http_upstream_srv_conf_t *uscf = ((ngx_http_upstream_srv_conf_t**)conf->upstreams.elts)[i];
		ngx_http_upstream_rr_peers_t *peers = uscf->peer.data;

		b->last = ngx_sprintf(b->last, "	\"%s\":\n	[\n", uscf->host.data);

		for (k = 0; k < peers->number; ++k)
		{
			b->last = ngx_sprintf(b->last, "		[\"");

			// peer name
			if (!peers->peer[k].server || peers->peer[k].server->naddrs > 1)
			{
				// Dirty workaround. Strange bugs with nginx-created strings when running under Duma
				size_t j;
				// Peer name (resolved)
				for (j = 0; j < peers->peer[k].name.len; ++j)
					b->last = ngx_sprintf(b->last, "%c", peers->peer[k].name.data[j]);
				// Initial config name (unresolved)
				b->last = (!peers->peer[k].server)
								? ngx_sprintf(b->last, " (%s)", uscf->host.data)
								: ngx_sprintf(b->last, " (%s)", (char*)peers->peer[k].server->name.data);
			}
			else
			{
				b->last = ngx_sprintf(b->last, "%s", (peers->peer[k].server->name.data
						? (char*)peers->peer[k].server->name.data
						: (char*)uscf->host.data));
			}

			b->last = ngx_sprintf(b->last, "\", ");

			unsigned disabled = (peers->peer[k].down) ? 1 : 0;
			unsigned blacklisted = (now - *(time_t*)(USTATS_CALC_ADDRESS(peers->peer[k].shm_start_offset, USTATS_LAST_FAIL_TIME_STAT_OFFSET))
					< peers->peer[k].fail_timeout);

			// disabled and blacklisted
			b->last = ngx_sprintf(b->last, "%d, %d, ", disabled, blacklisted);

			// requests
			b->last = ngx_sprintf(b->last, "%d, ", *(ngx_uint_t*)USTATS_CALC_ADDRESS(peers->peer[k].shm_start_offset, USTATS_REQ_STAT_OFFSET));
			// 499s
			b->last = ngx_sprintf(b->last, "%d, ", *(ngx_uint_t*)USTATS_CALC_ADDRESS(peers->peer[k].shm_start_offset, USTATS_HTTP499_STAT_OFFSET));
			// 500s
			b->last = ngx_sprintf(b->last, "%d, ", *(ngx_uint_t*)USTATS_CALC_ADDRESS(peers->peer[k].shm_start_offset, USTATS_HTTP500_STAT_OFFSET));
			// 503s
			b->last = ngx_sprintf(b->last, "%d, ", *(ngx_uint_t*)USTATS_CALC_ADDRESS(peers->peer[k].shm_start_offset, USTATS_HTTP503_STAT_OFFSET));
			// tcp errors
			b->last = ngx_sprintf(b->last, "%d, ", *(ngx_uint_t*)USTATS_CALC_ADDRESS(peers->peer[k].shm_start_offset, USTATS_TCP_ERR_STAT_OFFSET));
			// http read timeouts
			b->last = ngx_sprintf(b->last, "%d, ", *(ngx_uint_t*)USTATS_CALC_ADDRESS(peers->peer[k].shm_start_offset, USTATS_READ_TIMEOUT_STAT_OFFSET));
			// http write timeouts
			b->last = ngx_sprintf(b->last, "%d, ", *(ngx_uint_t*)USTATS_CALC_ADDRESS(peers->peer[k].shm_start_offset, USTATS_WRITE_TIMEOUT_STAT_OFFSET));
			// fail timeout
			b->last = ngx_sprintf(b->last, "%ui, ", peers->peer[k].fail_timeout);
			// max fails
			b->last = ngx_sprintf(b->last, "%ui, ", peers->peer[k].max_fails);
			// last fail
			if (*(time_t*)USTATS_CALC_ADDRESS(peers->peer[k].shm_start_offset, USTATS_LAST_FAIL_TIME_STAT_OFFSET) == 0)
            {
                b->last = ngx_sprintf(b->last, "\"0000-00-00 00:00:00\", ");
            }
			else
			{
				char at_b[30];
				struct tm * t = localtime((time_t*)USTATS_CALC_ADDRESS(peers->peer[k].shm_start_offset, USTATS_LAST_FAIL_TIME_STAT_OFFSET));
				strftime(at_b, sizeof(at_b), "%Y-%m-%d %H:%M:%S", t);
				b->last = ngx_sprintf(b->last, "\"%s\", ", at_b);
			}
			// total fails
			b->last = ngx_sprintf(b->last, "%d", *(ngx_uint_t*)USTATS_CALC_ADDRESS(peers->peer[k].shm_start_offset, USTATS_FAIL_COUNT_STAT_OFFSET));

			// close brackets, insert comma and new line
			b->last = ngx_sprintf(b->last, "],\n");
		}

		// implicit
		unsigned implicit = 0;
		if ((peers->number && (!peers->peer[0].server || !peers->peer[0].server->name.data)))
			implicit = 1;
		b->last = ngx_sprintf(b->last, "		%d\n", implicit);

		b->last = ngx_sprintf(b->last, "	]");
		if (i < conf->upstreams.nelts - 1)
			b->last = ngx_sprintf(b->last, ",");
		b->last = ngx_sprintf(b->last, "\n");
	}

	b->last = ngx_sprintf(b->last, "}\n");

	return b;
}



static ngx_buf_t * ngx_http_ustats_create_response_html(ngx_http_request_t * r)
{
	size_t size = sizeof(HTML) + 3 * sizeof("0000000"); /* table width, height and update timer */
	ngx_buf_t * b = ngx_create_temp_buf(r->pool, size);

	ngx_http_ustats_loc_conf_t * uslc = ngx_http_get_module_loc_conf(r, ngx_http_ustats_module);

	char buf1[8], buf2[8];
	ngx_memset(buf1, 0, 8);
	ngx_memset(buf2, 0, 8);

	ngx_sprintf((u_char*)buf1, "%d%s", uslc->html_table_width,
			uslc->html_table_width <= 100 ? "%" : "");
	ngx_sprintf((u_char*)buf2, "%d%s", uslc->html_table_width,
			uslc->html_table_height <= 100 ? "%" : "");

	b->last = ngx_sprintf(b->last, HTML, buf1, buf2, uslc->refresh_interval);

	return b;
}
