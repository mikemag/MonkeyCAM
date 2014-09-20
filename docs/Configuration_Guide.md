# MonkeyCAM v4.0 Configuration Guide

MonkeyCAM uses two configuration files, one to describe the board and
one to describe the details of how to machine the board. The board
definition file describes the shape of the board, the thickness and
taper of the core, placement of inserts, etc. The machine and tool
definition file describes machining parameters used by various
programs (rapid and various cutting heights and depths, raw material
thicknesses, etc.) and parameters of each tool used.

See the [MonkeyCAM v4.0 User’s Guide](
https://github.com/mikemag/MonkeyCAM/blob/master/docs/Users_Guide.md)
for an overview.

## File Format

The configuration files are in JSON format, which makes them easy to
parse programmatically and also pretty easy for humans to read and
edit.

You don’t really need to understand much JSON to be able to edit these
effectively. Only a subset of JSON is used, and it’s formatted very
simply. The following example shows three parameters and their values:

```
"name": "Sample-snowboard",
"nose length": 18,
"effective edge length": 136,
```

The string before the `:` is the name of the parameter, in this case
`name`, `nose length`, and `effective edge length`. The string or
number after the `:` is the value of the parameter. Thus the name of
this board is `Sample-snowboard`, the length of the nose is `18`cm,
and the length of the effective edge is `136`cm.

Avoid changing the structure of the file, i.e., the braces (`{`, `}`)
and names of the parameters. **Only change the values.**

## Board Definition

The board configuration file contains two sections, one to describe
the overall shape of the board and one to describe the thickness
profile.

```
{
    “board”:
    {
         parameters
     },
    “profile”:
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

#### Name
"name": "Sample-snowboard",

#### Nose Length
”nose length”: `length in cm`

The length of the nose from the tip of the board to the start of the
effective edge.

#### Effective Edge Length
"effective edge length": 136

#### Tail length
"tail length": 14

#### Sidecut Radius
"sidecut radius": 1100

#### Waist Width
"waist width": 24.6

#### Taper
"taper": 0.4

#### Nose Shape
"nose shape":
{
"type": "Basic Bezier",
"end handle": 0.50,
"transition handle": 0.75
},

#### Edge Shape
"edge shape":
{
"type": "Basic Arc"
},
#### Tail Shape
"tail shape":
{
"type": "Basic Bezier",
"end handle": 0.50,
"transition handle": 0.75
},

#### Reference Stance Width
"reference stance width": 52

#### Stance Setback
"stance setback": 2

#### Nose Insert Pack
"nose insert pack":
{
"count nose": 1,
"count tail": 1,
"offset": 4,
"horizontal spacing": 4,
"vertical spacing": 4
},

#### Tail Insert Pack
"tail insert pack":
{
"count nose": 1,
"count tail": 1,
"offset": 4,
"horizontal spacing": 4,
"vertical spacing": 4
},

#### Nose and Tail Spacer Width
"nose and tail spacer width": 2

### Profile Section

#### Nose Thickness
"nose thickness": 0.2

#### Center Thickness
"center thickness": 0.79

#### Tail Thickness
"tail thickness": 0.2

#### Nose Taper
"nose taper":
{
"taper start": 0.45,
"start handle": 0.70,
"end handle": 0.80,
"taper end": 1.00
},

#### Tail Taper
"tail taper":
{
"taper start": 0.45,
"start handle": 0.70,
"end handle": 0.80,
"taper end": 1.00
}

## Machine and Tool definition
