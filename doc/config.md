# mcmap json config

if provided via parameter or found in the current working directory as `mcmap.json`,
mcmap will obey to json configuration file of the following format:

    {
      "world": "~/.minecraft/saves/worldsave",
      "bounds": 
      [
        -10,
        -10,
        10,
        10
      ],
      "blockSize": 16,
      "zoomLevels": [1,4,12],
      "tiledOutput": true,
      "tileSize": 128,
      "outputDir": "./output/"
    }

## `world`

The world save to process. mcmap can process raw saves and the following compressed formats:

- `world.tar`
- `world.tar.gz`

## `bounds`

The chunk range boundaries to map. Expected order is { NE, NW, SE, SW }.

## `blockSize`

How many pixels (square) shall a mapped block have? (should be a multiple of 16)

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
