# MonkeyCAM v4 Configuration Guide

MonkeyCAM uses three configuration files, one to describe the
board/ski, one to describe the binding inserts (optional, no inserts
if omitted), and one to describe the details of how to machine the
board. The board definition file describes the shape of the board, and
the thickness and taper of the core. The binding definition file
describes the placement of inserts. The machine and tool definition
file describes machining parameters used by various programs (rapid
and various cutting heights and depths, raw material thicknesses,
etc.) and parameters of each tool used.

See the [MonkeyCAM v4 User’s Guide](Users_Guide.md) for an overview.

## File Format

The configuration files are in [JSON
format](http://en.wikipedia.org/wiki/JSON), which makes them easy to
parse programmatically and also pretty easy for humans to read and
edit.

You don’t really need to understand much JSON to be able to edit these
effectively. Only a subset of JSON is used, and it’s formatted very
simply. The following example shows three parameters and their values:

```
"name": "Sample-snowboard",
"nose length": 18,
"effective edge length": 136,
 "tail length": "140mm",
```

The string before the `:` is the name of the parameter, in this case
`name`, `nose length`, and `effective edge length`. The string or
number after the `:` is the value of the parameter. Thus the name of
this board is `Sample-snowboard`, the length of the nose is `18`cm,
and the length of the effective edge is `136`cm.

The values may be strings or numbers as defined here.

| Datatype | Definition |
|:---:| --- |
| `"string"` | a sequence of zero or more characters, delimted with double-quotation marks. |
| `number` | a signed decimal number that may contain a fractional part. For length parameters, units are inferred based on the section defaults. |

Furthermore, we define a `percentage` as a number from `0.0` to `1.0`.

Avoid changing the structure of the file, i.e., the braces (`{`, `}`)
and names of the parameters. **Only change the values.**

## Board Definition

The board configuration file contains two sections, one to describe
the overall shape of the board and one to describe the thickness
profile.

```
{
    "board":
    {
         parameters
     },
    "profile":
    {
        parameters
     }
}
```

### Board Section

The `board` section describes the complete, final shape of the ski or
snowboard. These are the measurements after it is completed, not of
the core or base material. If you specify that a board has a waist
width of 24.6cm then when you measure the width of the base with edges
it will be 24.6cm wide.

Note that you do not specify the width of the nose and tail, it is
computed from the combination of the waist width, effective edge
length, sidecut radius, and taper.

All length parameters in the board and profile sections accept either
raw `numbers` or `strings` with unit suffixes (`"cm"`, `"mm"`, or `"in"`).
When a unit suffix is omitted, values default to **centimeters**.


#### Name
"name": `string`

Name of the ski or snowboard. This will be used as the prefix of all
generated files.


#### Splitboard
"splitboard": `boolean`

`true` if the snowboard is a splitboard, `false` otherwise. Optional,
and if missing will be assumed to be `false`.


#### Nose Length
"nose length": `number` | `string`

The length (default cm) of the nose from the tip of the board to
the start of the effective edge.


#### Effective Edge Length
"effective edge length": `number` | `string`

The length (default cm) of the effective edge of the board. This
is the length of the board between the nose and tail. The sidecut will
be made along this length.


#### Tail length
"tail length": `number` | `string`

The length (default cm) of the tail from the end of the board to
the end of the effective edge.


#### Sidecut Radius
"sidecut radius": `number` | `string`

The radius (default cm) of a circle which will be used to form the
sidecut along the effective edge.


#### Waist Width
"waist width": `number` | `string`

The width (default cm) of the waist of the board. The waist is
located at the center of the effective edge. Note that because of the
taper, this is not necessarily the narrowest part of the board.


#### Taper
"taper": `number` | `string`

The taper (default cm) of the board from nose to tail. The tail
will be narrower than the nose by this amount.


#### Nose Shape

The nose shape specifies parameters to control the shape of the nose,
specifically the curve from the start of the effective edge to the tip
of the nose.

```
"nose shape":
{
    "type": string,
    "end handle": percentage,
    "transition handle": percentage
}
```

The `type` may be `"Basic Bezier"` or `"Flat"`, other
options will be available in the future. `"Basic Bezier"` specifies a classic
Bezier curve with two control points for one half of the nose shape,
and this curve is mirrored to get the other half. `"Flat"` is similar
but includes an additional flat section of specified width at the tip. The control points
are constrained to ensure that the Bezier curves join properly at the
nose and to the effective edge.

The `end handle` is a percentage of half of the nose width, and
determines how far away from the center of the nose the control point
is, and therefore how much it pulls on the Bezier near the tip. A
smaller percentage will result in a pointy tip, while a lager one will
form a tip which is more blunt.

The `transition handle` is a percentage of the nose length, and
determines how far away from the transition between nose and effective
edge the control point is, and therefore how much it pulls on the
Bezier near this transition point. A small percentage will cause a
sharper transition from nose to effective edge, and a larger one will
smooth it out.

A percentage is used to define the control points because this results
in noses which hold their relative shape fairly well even as the
length and width of the nose changes, so you’re not constantly having
to update the nose definition as you play with other design
parameters.


#### Edge Shape

The edge shape specifies parameters to control the shape of the effective edge.

```
"edge shape":
{
    "type": string
}
```

The `type` may currently be only `"Basic Arc"`, though other options
will be available in the future. This specifies a very simple arc for
the effective edge, and has been sufficient for every board I’ve built
to-date.

There are lots of other options, and I may add them in the
future. However, the practical difference between other options and a
simple arc is stunningly small, which is why I’ve stuck with an arc
for so long.


#### Tail Shape

The tail shape specifies parameters to control the shape of the tail,
specifically the curve from the end of the effective edge to the tip
of the tail.

```
"tail shape":
{
    "type": string,
    "end handle": percentage,
    "transition handle": percentage
}
```

See the specification for the Nose Shape for details; the tail is the
same.

#### Stance Setback
"stance setback": `number` | `string`

*Optional.*

The setback (default cm) which defines how much the reference
stance of a board, or boot center of a ski, is shifted away from the
center of the effective edge towards the tail. A value of `0` keeps the
reference stance centered over the center of the effective edge. A positive
value shifts the reference stance towards the tail. Optional. If omitted a
value of `0` is assumed.

#### Nose and Tail Spacer Width
"nose and tail spacer width": `number` | `string`

Specifies the width (default cm) of the nose and tail spacer. The
core at the nose and tail, starting at the ends of the effective edge,
will be inset by this amount.


#### Nose Edge Extension
"nose edge extension": `number` | `string`

Specifies the length (default cm) of the edge past the end of the
effective edge towards the nose of the board. Optional. If omitted the
edge wraps around the entire nose of the board. If `0.0`, the edge
ends right at the end of the effective edge.


#### Tail Edge Extension
"tail edge extension": `number` | `string`

Specifies the length (default cm) of the edge past the end of the
effective edge towards the tail of the board. Optional. If omitted the
edge wraps around the entire tail of the board. If `0.0`, the edge
ends right at the end of the effective edge.


### Profile Section

The profile specifies the thickness of the core from nose to
tail. Currently, MonkeyCAM generates profiles which are constant
thickness throughout the nose and tail, and are a constant thickness
throughout the bulk of the center of the board. The transition from
the thickest portion at the center to the thinner nose and tail is
defined with Bezier curves for a smooth and highly customizable
transition.


#### Nose Thickness
"nose thickness": `number` | `string`

The thickness (default cm) of the nose. The entire nose right up
to the effective edge will be this thickness.


#### Center Thickness
"center thickness": `number` | `string`

The thickness (default cm) of the center of the board. How much of
the center is this thickness is determined by the nose and tail taper
parameters.


#### Tail Thickness
"tail thickness": `number` | `string`

The thickness (default cm) of the tail. The entire tail right up
to the effective edge will be this thickness.


#### Nose Taper

The nose taper defines how the profile transitions from the larger
center thickness to the thinner nose thickness. A Bezier is used to
generate a smooth transition. To understand these parameters, first
consider the distance from the center of the effective edge to the
start of the nose. This is one half the effective edge length. The
taper is defined using percentages of this distance, starting from the
center of the effective edge.

```
"nose taper":
{
    "taper start": percentage,
    "start handle": percentage,
    "end handle": percentage,
    "taper end": percentage
}
```

`taper start` specifies, as a percentage of half the effective edge
length, the distance from the center of the effective edge towards the
nose where the taper will begin.

`taper end` specifies, as a percentage of half the effective edge
length, the distance from the center of the effective edge towards the
nose where the taper will end.

`start handle` and `end handle` specify the location of two Bezier
control points. These are also percentages of half the effective edge
length. The further a handle is from the start or end, the more it
pulls on the curve and makes the start or end of the transition more
gradual. If the handles are very close to the start and end the
transition will be sharper.


#### Tail Taper

The tail taper is the same as the nose taper, simply directed towards
the tail from the center of the effective edge.

```
"tail taper":
{
    "taper start": percentage,
    "start handle": percentage,
    "end handle": percentage,
    "taper end": percentage
}
```

#### Reference Stance Width
"reference stance width": `number` | `string`

*Optional. Replaced by '--binding-distance' keyword on command line but
retained for backwards compatibility. To be removed in future version.*

The width (default cm) between the center insert group in each
pack. By default, these groups are centered at the waist of the board,
i.e., the center of the effective edge.

#### Nose Insert Pack

*Optional. Moved to Binding definition file ([See below](#nose-insert-pack-1)).
Functionality is currently retained in board definition file for backwards
compatibility. To be removed in future version.*


#### Tail Insert Pack

*Optional. Moved to Binding definition file ([See below](#tail-insert-pack-1)).
Functionality is currently retained in board definition file for backwards
compatibility. To be removed in future version.*


## Binding definition

*Optional. The whole binding definition file is optional.
Omit for skis or boards with no inserts.*

The binding definition configuration file contains a single section
which describes parameters relating to binding insert positioning.

All length parameters in the binding configuration accept either raw
numbers or strings with unit suffixes (`"cm"`, `"mm"`, or `"in"`). When
a unit suffix is omitted, values default to centimeters.

```
{
    “binding”:
    {
        parameters
    }
}
```


#### Binding Name
"binding name": `string`

The name of the binding or insert pattern specification.


### Snowboard insert packs

Inserts "packs" consist of "groups" of four
inserts in a standard 4x4 pattern, with each group staggered from the
other by a given amount. Packs are defined by how many groups they
have, and their spacing. Typically some inserts from one group overlap
some inserts from another group, and that is fine; the minimal number
of cuts will be made. Think of a group as a discrete place at which
you can mount your bindings, and how many such places there are in
various classic packs. The most classic pack of eight inserts has
three groups.

There are two packs, one for the nose and one for the tail.


#### Nose Insert Pack

*Optional. Omit for skis or other boards with no inserts.*

The nose insert pack specifies the number and relative location of the
groups of inserts closest to the nose of the board. The "reference
group" is the only required group of inserts and is implicitly
defined. Additional groups are specified with the other parameters.

```
"nose insert pack":
{
    "count nose": number,
    "count tail": number,
    "offset": number,
    "horizontal spacing": number,
    "vertical spacing": number
}
```

`count nose` specifies the number of additional groups to add from the
reference group towards the nose of the board. Likewise, `count tail`
specifies the number of additional groups to add towards the tail.

`offset` indicates (default cm) how far apart the center of each
group is.

`horizontal spacing` indicates (default cm) how far apart the
inserts within a group are from nose-to-tail, while `vertical spacing`
indicates, again in centimeters, how far apart they are from
side-to-side. (Think about looking at a board from above, with the
nose pointing to the left.)


##### Example: Classic 4x4 Snowboard Pattern

This is a pack of eight inserts, separated nose-to-tail by 4cm each,
and separated side-to-side by 4cm.

```
"nose insert pack":
{
    "count nose": 1,
    "count tail": 1,
    "offset": 4,
    "horizontal spacing": 4,
    "vertical spacing": 4
}
```


##### Example: Typical "Shotgun" Snowboard Pattern

This is a pack of 14 inserts, separated nose-to-tail by 2cm each, and
separated side-to-side by 4cm.

```
"nose insert pack":
{
    "count nose": 2,
    "count tail": 2,
    "offset": 2,
    "horizontal spacing": 4,
    "vertical spacing": 4
}
```


##### Example: Minimal Snowboard Pattern with Powder Option

This is a pack of 6 inserts, separated nose-to-tail by 4cm each, and
separated side-to-side by 4cm. The reference group is accompanied by
one more group towards the tail, which adds just two more inserts.

```
"nose insert pack":
{
    "count nose": 0,
    "count tail": 1,
    "offset": 4,
    "horizontal spacing": 4,
    "vertical spacing": 4
}
```

#### Tail Insert Pack

*Optional. Omit for skis or other boards with no inserts.*

The tail insert pack specifies the number and relative location of the
groups of inserts closest to the tail of the board. The "reference
group" is the only required group of inserts and is implicitly
defined. Additional groups are specified with the other parameters.

```
"tail insert pack":
{
    "count nose": number,
    "count tail": number,
    "offset": number,
    "horizontal spacing": number,
    "vertical spacing": number
}
```

See the Nose Insert Pack; the tail is the same.


### Generic binding inserts

Inserts for skis, snowboards, and splitboards can be specified at
three locations: Toe (Nose), Center or Heel (Tail).

Insert locations are specified using a JSON `array` to specify the
position of the center of each insert. The positions are defined as
offsets from a reference point which differs depending on the type of
board being built:

* For skis, the `toe` and `heel` are relative to the setback and the
  `--bindingdist` parameter, while the `center` is relative to the
  center of the effective edge.

* For snowboards, the `nose` and `tail` are relative to the setback
  and reference stance.

* For splitboards, the `nose` and `tail` are relative to the setback
  and reference stance, while the `center` is relative to the true
  center of the board. This allows the touring insert pack to be
  centered over the balance point of the board independent of the
  other parameters.

All offsets are specified in cm along the board towards the tail (x) or
across the board from left to right (y).

A JSON `array` consists of a series of comma seperated rows, each
enclosed in curly brackets `{row}`, and the whole array enclosed in
square brackets `[.....]`. For all the binding definition arrays each
row contains comma seperated "x" and "y" parameters specifying the
coordinates of an individual insert `"x": number, "y": number`.


#### Toe/Nose

The name can be `toe` or `nose` depending on preference. For clarity,
skis typically use `toe` while snowboards and splitboards typically
use `nose`.

"toe":
\[
  {"x": `number` | `string`,   "y": `number` | `string`},
  {"x": `number` | `string`,   "y": `number` | `string`},
  ...
  {"x": `number` | `string`,   "y": `number` | `string`}
\]

*Optional.*

Toe/nose inserts are specified with position relative to the center of
the boot toe for skis, and relative to the reference stance for
snowboards and splitboards. For tech/pin ski touring bindings toe
inserts are specified relative to the center of the pin
centerline. Note that for frame bindings with a fixed length (i.e. the
distance from the front screw holes to the back screw holes is fixed
as the binding is all one piece) then all of the binding inserts will
be specified relative to the toe (and hence should be part of the
"toe" section of the binding definition file).


#### Center
"center":
\[
  {"x": `number` | `string`,   "y": `number` | `string`},
  ...
\]

*Optional.*

Center inserts are specified relative to the boot/stance center for
skis, and relative to the true board center for splitboards.

For splitboards, the center pack is also duplicated and centered over
each half of the splitboard. This is specifically for the touring
inserts.


#### Heel/Tail

The name can be `heel` or `tail` depending on preference. For clarity,
skis typically use `heel` while snowboards and splitboards typically
use `tail`.

"heel":
\[
  {"x": `number` | `string`,   "y": `number` | `string`},
  ...
\]

*Optional.*

Heel inserts are relative to the center of the boot heel for skis, and
relative to the reference stance for snowboards and splitboards.


#### Example
```
    "toe":
    [
      {"x": -2.5,   "y": 2.0},
      {"x": -2.5,   "y": -2.0},
      {"x": 2.0,   "y": -2.0},
      {"x": 2.0,   "y": 2.0}
    ],
    "heel":
    [
      {"x": -1.5,   "y": 1.8},
      {"x": -1.5,   "y": -1.8},
      {"x": 3.75,   "y": -1.8},
      {"x": 3.75,   "y": 1.8}
    ]
```

## Machine and Tool definition

The machine and tool configuration file contains a single section
which describes parameters to control the machining process. Any
dimension which depends on a cutting tool, material thickness, or is
related to the machining or assembly process is represented here. If
your manufacturing process and materials are fairly consistent then it
will be rare for you to change this file.

All length parameters in the machine configuration and tool definitions
accept either raw numbers or strings with unit suffixes (`"cm"`, `"mm"`,
or `"in"`). When a unit suffix is omitted, values default to inches.

```
{
    “machine”:
    {
        parameters
    }
}
```

### Output Units

#### G-Code Units
"gcode units": `"in"` or `"mm"`

Sets the output units for generated G-code. Use `"in"` for inches
(the default) or `"mm"` for millimeters.
MonkeyCAM converts its internal units to the selected units and
emits `G20` (inches) or `G21` (millimeters) accordingly.

#### XY Rotation
"rotate x/y axes": `"none"` | `"counter-clockwise"` | `"clockwise"`

Rotates the X/Y coordinates when emitting G-code. Use `"none"` for the
default orientation, `"counter-clockwise"` for a -90° rotation, or
`"clockwise"` for a 90° rotation. This is useful when your machine's
long and short axes are configured opposite of the MonkeyCAM defaults.

### Overall speeds and feeds

All feed rates are emitted as `F` codes in the G-Code programs,
without translation. The units are defined by your CNC controller
configuration. Inches per minute (IPM) or millimeters per minute
(mm/min) are common.

#### Rapid Speed
"rapid speed": `number`

The velocity at which the machine will move during rapid movements via
`G00`.

#### Normal Speed
"normal speed": `number`

The velocity at which the machine will move during interpolated
movements via `G01`, `G02`, `G03`.

### Heights and Depths

All heights and depths in the machine configuration are measured
assuming the table or part top is at `Z = 0`, and positive values of
`Z` are **above** the table or part.

#### Bottom Rapid Height

"bottom rapid height": `number` | `string`

The height (default in) at which the cutter will move during all rapid
motions via `G00` when working the bottom of the board. All programs
which work on the bottom of the board use `Z = 0` at the top of the
core blank, thus this is the height above the core blank for all rapid
moves.

#### Top Rapid Height

"top rapid height": `number` | `string`

The height (default in) at which the cutter will move during all rapid
motions via `G00` when working the top of the board. All programs
which work on the top of the board use `Z = 0` at the top of the
table, thus this is the height above the table for all rapid moves and
must be sufficient to clear the core.

### Material Thicknesses

#### Core Blank Thickness

"core blank thickness": `number` | `string`

The thickness (default in) of the core blank. This does not have to be
precise so long as it is **at least** as thick as your thickest core
blank. A value larger than the true thickness of the core blank will
cause some of the G-Code programs to be less efficient than they
otherwise could be, but they will still work.

### Base Cutout Parameters

Each of these effects the [Base Cutout program](G-Code_Program_Guide.md#base-cutout).

#### Base Cutout Tool

"base cutout tool": `tool number`

The tool, defined in the [`tools` section](Configuration_Guide.md#tools), which will be used to cut the base.

#### Base Rapid Height

"base rapid height": `number` | `string`

The height (default in) at which the cutter will move during all rapid
motions via `G00` when working the base and nose/tail spacer
material. All programs which work on the base use `Z = 0` at the top
of the table, thus this is the height above the table for all rapid
moves and must be sufficient to clear the base.

#### Base Cut Thru Height

"base cut thru height": `number` | `string`

The depth (default in) at which the cutter will descend to in order to
cut thru the base material. This should be a negative number slightly
less than `0` in order to ensure the cutter completely penetrates the
base material, i.e., `-0.010` or so.

### Guide Holes Parameters

Each of these effects the [Guide Holes program](G-Code_Program_Guide.md#guide-holes).

#### Guide Hole Tool
"guide hole tool": `tool number`

The tool, defined in the [`tools` section](Configuration_Guide.md#tools), which will be used to cut the guide holes in the core blank.

#### Guide Hole Depth

"guide hole depth": `number` | `string`

The depth (default in) to which the guide holes will be cut thru the
core. Note that this program uses `Z = 0` as the top of the core
blank, so this should be thicker than the core blank and thick enough
to ensure a reasonable hole is left in the table top for alignment
pins.

#### Guide Hole Diameter

"guide hole diameter": `number` | `string`

The diameter (default in) of the guide holes. Make this slightly larger
than the true diameter of your alignment pins, i.e., `0.505` inches
for half inch pins.

#### Guide Hole Offset

"guide hole offset": `number` | `string`

The offset (default in) of the guide holes from the ends of the core.

### Alignment Marks Parameters

Each of these effects the [Alignment Marks program](G-Code_Program_Guide.md#alignment-marks).

#### Alignment Mark Tool

"alignment mark tool": `tool number`

The tool, defined in the [`tools` section](Configuration_Guide.md#tools), which will be used to cut the alignment marks on the bottom of the core.

#### Alignment Mark Offset

"alignment mark offset": `number` | `string`

The offset (default in) of all alignment marks from the edges of the
core.

#### Alignment Mark Depth

"alignment mark depth": `number` | `string`

The depth (default in) of all alignment marks into the core. Typically
quite small, e.g. `-0.025`, but adjust based on personal preference.

#### Alignment Mark Deep Depth

"alignment mark deep depth": `number` | `string`

The depth (default in) of the extra-deep alignment marks near past the
ends of the core that can be used to re-align the machine in the event
of a horrible mistake.

### Edge Groove Parameters

Each of these effects the [Edge Groove program](G-Code_Program_Guide.md#edge-groove).

#### Edge Groove Tool

"edge groove tool": `tool number`

The tool, defined in the [`tools` section](Configuration_Guide.md#tools), which will be used to 
cut the edge groove.

#### Edge Groove Depth

"edge groove depth": `number` | `string`

The depth (default in) of the edge groove around the perimeter of the
core to accommodate the edges.

#### Edge Groove Edge Width

"edge groove edge width": `number` | `string`

The width (default in) of your edge stock including the tines, plus a little extra for some 
play when aligning the core to the base. This parameter is combined with `sidewall overhang` 
to form the total width of the edge groove.

#### Edge Groove Overlap Percentage

"edge groove overlap percentage": `percentage`

The percentage of the selected tool diameter that each successive
cutting pass will overlap with the prior pass.

### Insert Holes Parameters

Each of these effects the [Insert Holes program](G-Code_Program_Guide.md#insert-holes).

#### Insert Tool

"insert tool": `tool number`

The tool, defined in the [`tools` section](Configuration_Guide.md#tools), which will be used to cut the insert holes.

#### Insert Rim Depth

"insert rim depth": `number` | `string`

The depth (default in) of the outer edge of the insert holes.

#### Insert Rim Diameter

"insert rim diameter": `number` | `string`

The diameter (default in) of the rim of the insert holes.

#### Insert Hole Diameter

"insert hole diameter": `number` | `string`

The diameter (default in) of the barrel of the inserts.

### Top Profile Parameters

Each of these effects the [Top Profile program](G-Code_Program_Guide.md#top-profile).

#### Top Profile Tool

"top profile tool": `tool number`

The tool, defined in the [`tools` section](Configuration_Guide.md#tools), which will be used to profile the core.

#### Top Profile Transition Speed

"top profile transition speed": `number`

The feed rate used when transitioning to the next larger profiling
pass. Since the cutter is typically quite large, a slower feed rate
when driving it into the core blank to start the next pass is advised.

#### Top Profile Deep Speed

"top profile deep speed": `number`

The feed rate used at maximum cutting depth during core
profiling. Typically slower than the normal feed rate due to the
likely large diameter of the cutter.

#### Top Profile Overlap Percentage

"top profile overlap percentage": `number`

The percentage of the selected tool diameter that each successive
profiling pass will overlap with the prior pass.

#### Top Profile Lead-in Length

"top profile lead-in length": `number` | `string`

The distance (default in) over which the cutter will ramp into the core
blank before reaching the full cutting depth. A longer lead-in may be
appropriate for thicker core blanks, or larger cutters.

### Top Cutout Parameters

Each of these effects the [Top Cutout program](G-Code_Program_Guide.md#top-cutout).

#### Core Cutout Tool

"core cutout tool": `tool number`

The tool, defined in the [`tools` section](Configuration_Guide.md#tools), which will be used to cut out the core.

#### Sidewall Overhang

"sidewall overhang": `number` | `string`

The amount (default in) of sidewall which will remain past the edges, outside the final board 
shape. Leave a few mm to account for core-to-base alignment error.

This parameter is combined with `edge groove edge width` to form the total width
of the edge groove used in the [Edge Groove program](G-Code_Program_Guide.md#edge-groove).

### Nose Tail Spacers Parameters

Each of these effects the [Nose Tail Spacers program](G-Code_Program_Guide.md#nose-tail-spacers).

#### Spacer End Overhang

"spacer end overhang": `number` | `string`

The minimum amount (default in) of spacer material which will be left
past the ends of the final board shape.

#### Spacer Side Overhang

"spacer side overhang": `number` | `string`

The minimum amount (default in) of spacer material which will be left
past the sides of the final board shape.

### Edge Trench Parameters

Each of these effects the [Edge Trench program](G-Code_Program_Guide.md#edge-trench).

#### Edge Trench Width

"edge trench width": `number` | `string`

The width (default in) of the edge trench. This should be the width of
your sidewall material, and of any additional stringers you want to
lay down perfectly along the effective edge. Recognize that a portion
of this trench will extend past the final board shape as determined by
the sidewall overhang.

#### Edge Trench Extension

"edge trench extension": `number` | `string`

The distance (default in) the edge trench will extend past the ends of
the effective edge. This distance needs to be sufficient to ensure
sidewall and extra stringer material will fill the core a bit past
where it will be shaped by the core cutout program, but not so long
that the trench cuts beyond the boundaries of the core blank.

Currently this measurement is a bit of trial-and-error, though a value
of about `3` inches is typically sufficient. See the [issue](
https://github.com/mikemag/MonkeyCAM/issues/6) about improving this.

### Splitboard Parameters

Each of these effects every program generated, as the core is widended
down the center to allow the board to be cut in half after pressing.

#### Splitboard Center Gap

"splitboard center gap": `number` | `string`

The width (default in) of the gap to leave down the center of the board
for the board to be split after layup. This should be the width of
your bandsaw blade plus a little bit of slop for error to ensure the
blade doesn't bind and dig into the metal center edges. You will
seperate the edged base halves by this amount while preparing for
layup.


#### Splitboard Center Trench Width

"splitboard center trench width": `number` | `string`

The width (default in) of the center trench. This should be the width of
your sidewall material, and of any additional stringers you want to
lay down perfectly along the center of the splitboard.

Note carefully that the `splitboard center gap` does not effect this
trench width, and that the portion of your sidewall material within
this gap will be lost when you split the board after layup. In effect,
this "gap" is much like the sidewall overhang and ensures that
sidewall material can be trimmed back to the center edges during
finishing.



### Tools

The `tools` section has an entry for each tool which will be used by
the generated G-Code programs. Each tool is given an `id` which is
used to reference the tool from other parameters, and is called the
`tool number` in the documentation for those parameters.

```
"tools":
[
    {
        "id": 1,
        "name": "Quarter Inch Upcut Spiral",
        "diameter": 0.247,
        "G-code #": 1
    },
    {
        "id": 2,
        "name": "Quarter Inch Straight",
        "diameter": 0.250,
        "G-code #": 1
    },

]
```

#### ID

"id": `tool number`

A unique numeric id for the tool, so it can be referenced from other
machine configuration parameters.

#### Name

"name": `string`

A name for the tool, which can be whatever you like and will appear in
the header block of the G-Code programs which use this tool.

#### Diameter

"diameter": `number` | `string`

The diameter (default in) of the tool. Be precise and measure carefully
with precision calipers. If you have a quarter-inch cutter which is
really `0.247` inches in diameter, then enter that.

#### G-code #

"G-code #": `number`

The tool number, as defined in your CNC controller, which will be
emitted via `G43`. I.e., `G43 H1 T4` if the number is `4`. This is for
CNC machines with automatic tool changers, or for those who configure
tool height offsets in the CNC controller.
