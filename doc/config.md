# mcmap json config

if provided via parameter or found in the current working directory as `mcmap.json`,
mcmap will obey to json configuration file of the following format:

    {
      "bounds": 
      [
        -10,
        -10,
        10,
        10
      ],
      "blockSize": 16,
      "outputDir": "./output/",
      "renderEnd": false,
      "renderNether": false,
      "renderOverworld": true,
      "renderOverworldNight": false,
      "saveMapStatistics": true,
      "tiledOutput": true,
      "tileSize": 128,
      "world": "~/.minecraft/saves/worldsave",
      "zoomLevels": [1,4,12]
    }

## `world`

The world save to process. mcmap can process raw saves and the following compressed formats:

- `world.tar`
- `world.tar.gz`

## `bounds`

The chunk range boundaries to map. Expected order is { NE, NW, SE, SW }.
If any of the values is zero, all values are assumed zero.

A value of zero leads to the mapping of the whole world.

## `blockSize`

How many pixels (square) shall a mapped block have? (has to be a multiple of 16)

## `zoomLevels`

The zoom levels to render.

Zoom levels calculate themselves by y-distance from the rendered block * 16, e.g.
level 1 is rendered from 16 blocks above, 16 from 256 (current build height limit). 

## `tiledOutput`

Shall the output be tiled (broken up into smaller pieces)?

## `tileSize`

Size of each tile, defaults to 256px * 256px.

## `outputDir`

Output directory of generated map.

## `saveMapStatistics`

If true, mcmap saves some statistics of the map into $output/map_statistics.json
and $output/region_statistics.json
