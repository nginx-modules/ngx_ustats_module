# Basic statistics for NGINX backend
## Nginx Module: ustats

### Features

* Number of requests
* Http 499/500/503 errors count
* Tcp errors
* Http read/write timeouts
* Fail timeout
* Max fails count
* Last failed access time
* Total fails count
* Blacklisted backend highlighting
* Down backends highlighting

The module's **web interface** provides a good visual representation of what's going on with your backends. Values in some columns of the table can be sorted within each upstream row.

Gathered data can also be retrieved in **JSON format**. To do so, append `?json=1` to the end of location on which the module was set to work on (see configuration instructions below).

![Screenshot](https://cloud.githubusercontent.com/assets/3759759/19833620/29290e0e-9e51-11e6-8400-7c26c1543237.png)

#### Installation

Tested with nginx: 1.2.2

* Copy ustats folder into your nginx/src/http/modules folder
* Copy nginx.patch file into nginx root folder,
  `cd` into nginx `root folder` and apply the patch

        patch -p1 -i nginx-1.7.2.patch

    Run `./configure` with all parameters you normally use, appending option

        --add-module=src/http/modules/ngx_ustats_module
        make && make install

#### Configuration

*Example*

```nginx
location /ustats {
    ustats memsize=3m;
    ustats_refresh_interval 6000;
    ustats_html_table_width 95;
    ustats_html_table_height 95;
}
```

### Configuration directives - base

#### ustats
* **syntax**: `ustats memsize=size`
* **default**: `n/a`
* **context**: `location`

Enables module handler for this location and sets the size of the shared memory that will be used to store statistics data across all worker processes.
Example: `ustats memsize=2m;`

#### ustats_html_table_width
* **syntax**: `ustats_html_table_width number`
* **default**: `70`
* **context**: `location`

Specifies web interface table width. Values less or equal to 100 are interpreted as percents, otherwise as pixels.

#### ustats_html_table_height
* **syntax**: `ustats_html_table_height number`
* **default**: `70`
* **context**: `location`

See _ustats_html_table_width_.

#### ustats_refresh_interval
* **syntax**: `ustats_refresh_interval number`
* **default**: `5000`
* **context**: `location`

Specifies page refresh interval in milliseconds.
