# mcmap json config

if provided via parameter or found in the current working directory as `mcmap.json`,
mcmap will obey to json configuration file of the following format:

    {
      "assetDir": "~/.minecraft/extracted_jar/assets"
      "bounds": 
      [
        -10,
        -10,
        10,
        10
      ],

      "blockSize": 16,
      "outputDir": "./output/",
      "renderDimensions": ["Overworld"],
      "renderOrientations": ["N"],
      "renderModes": ["top"]
      "saveMapStatistics": true,
      "statisticsOnly": false,
      "tiledOutput": true,
      "tileSize": 128,
      "world": "~/.minecraft/saves/worldsave",
      "zoomLevels": [1,4,12]
    }

## Rendering Options

- `renderDimensions` expects an array of the to-be-processed dimensions.
  Dimensions can either be given as integer (-1, 0, 1) or with their
  *official* name (Nether, Overworld, End).

- `renderOrientations` expects an array of the output orientations.
  These should be given in a compass-direction fashion, e.g. N => North, SE => South East
  (Please be aware that not all directions apply to all render modes. S.b. for details.)

- `renderModes` also expects an array for the output modes.
  This can be **top** or **isometric**. If isometric rendering is chosen, all
  pure orientations (e.g. N, W) are not going to be considered. Vice versa,
  for top rendering, all the mixed orientations are not going to be considered.

## `statisticsOnly`

If set to true, no rendering will be done.

## `assetDir`

The asset dir from the extracted minecraft client.

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
