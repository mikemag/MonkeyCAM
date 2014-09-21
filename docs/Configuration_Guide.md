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
| `number` | a signed decimal number that may contain a fractional part. May denote inches or centimeters, depending on the parameter. |

Furthermore, will define a `percentage` as a number from 0.0 to 1.0.

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


#### Name
"name": `string`

Name of the ski or snowboard. This will be used as the prefix of all
generated files.


#### Nose Length
"nose length": `number`

The length, in centimeters, of the nose from the tip of the board to
the start of the effective edge.


#### Effective Edge Length
"effective edge length": `number`

The length, in centimeters, of the effective edge of the board. This
is the length of the board between the nose and tail. The sidecut will
be made along this length.


#### Tail length
"tail length": `number`

The length, in centimeters, of the tail from the end of the board to
the end of the effective edge.


#### Sidecut Radius
"sidecut radius": `number`

The radius, in centimeters, of a circle which will be used to form the
sidecut along the effective edge.


#### Waist Width
"waist width": `number`

The width, in centimeters, of the waist of the board. The waist is
located at the center of the effective edge. Note that because of the
taper, this is not necessarily the narrowest part of the board.


#### Taper
"taper": `number`

The taper, in centimeters, of the board from nose to tail. The tail
will be narrower than the nose by this amount.


#### Nose Shape

The nose shape specifies parameters to control the shape of the nose,
specifically the curve from the start of the effective edge to the tip
of the nose.

```
"nose shape":
{
    "type": `string`,
    "end handle": `percentage`,
    "transition handle": `percentage`
}
```

The `type` may currently be only `"Basic Bezier"`, though other
options will be available in the future. This specifies a classic
Bezier curve with two control points for one half of the nose shape,
and this curve is mirrored to get the other half. The control points
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
    "type": `string`
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
    "type": `string`,
    "end handle": `percentage`,
    "transition handle": `percentage`
}
```

See the specification for the Nose Shape for details; the tail is the
same.


### Inserts

Inserts are collected into "packs" which consist of "groups" of four
inserts in a standard 4x4 pattern, with each group staggered from the
other by a given amount. Packs are defined by how many groups they
have, and their spacing. Typically some inserts from one group overlap
some inserts from another group, and that is fine; the minimal number
of cuts will be made. Think of a group as a discrete place at which
you can mount your bindings, and how many such places there are in
various classic packs. The most classic pack of eight inserts has
three groups.

There are two packs, one for the nose and one for the tail.

Currently the only way to place inserts is for snowboards. In the
future I'll add other ways, specifically for splitboards and various
standard ski binding patterns.


#### Reference Stance Width
"reference stance width": `number`

The width, in centimeters, between the center insert group in each
pack. By default, these groups are centered at the waist of the board.


#### Stance Setback
"stance setback": `number`

The setback, in centimeters, which defines how much the reference
stance is shifted away from the waist towards the tail. A value of `0`
keeps the reference stance centered over the waist. A positive value
shifts the reference stance towards the tail.


#### Nose Insert Pack

The nose insert pack specifies the number and relative location of the
groups of inserts closest to the nose of the board. The "reference
group" is the only required group of inserts and is implicitly
defined. Additional groups are specified with the other parameters.

```
"nose insert pack":
{
    "count nose": `number`,
    "count tail": `number`,
    "offset": `number`,
    "horizontal spacing": `number`,
    "vertical spacing": `number`
}
```

`count nose` specifies the number of additional groups to add from the
reference group towards the nose of the board. Likewise, `count tail`
specifies the number of additional groups to add towards the tail.

`offset` indicates, in centimeters, how far apart the center of each
group is.

`horizontal spacing` indicates, in centimeters, how far apart the
inserts within a group are from nose-to-tail, while `vertical spacing`
indicates, again in centimeters, how far apart they are from
side-to-side. (Think about looking at a board from above, with the
nose pointing to the left.)


##### Classic 4x4 Snowboard Pattern

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


##### Typical "Shotgun" Snowboard Pattern

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


##### Minimal Snowboard Pattern with Powder Option

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

The tail insert pack specifies the number and relative location of the
groups of inserts closest to the tail of the board. The "reference
group" is the only required group of inserts and is implicitly
defined. Additional groups are specified with the other parameters.

```
"tail insert pack":
{
    "count nose": `number`,
    "count tail": `number`,
    "offset": `number`,
    "horizontal spacing": `number`,
    "vertical spacing": `number`
}
```

See the Nose Insert Pack; the tail is the same.


#### Nose and Tail Spacer Width
"nose and tail spacer width": `number`

Specifies the width, in centimeters, of the nose and tail spacer. The
core at the nose and tail, starting at the ends of the effective edge,
will be inset by this amount.


### Profile Section

The profile specifies the thickness of the core from nose to
tail. Currently, MonkeyCAM generates profiles which are constant
thickness throughout the nose and tail, and are a constant thickness
throughout the bulk of the center of the board. The transition from
the thickest portion at the center to the thinner nose and tail is
defined with Bezier curves for a smooth and highly customizable
transition.


#### Nose Thickness
"nose thickness": `number`

The thickness, in centimeters, of the nose. The entire nose right up
to the effective edge will be this thickness.


#### Center Thickness
"center thickness": `number`

The thickness, in centimeters, of the center of the board. How much of
the center is this thickness is determined by the nose and tail taper
parameters.


#### Tail Thickness
"tail thickness": `number`

The thickness, in centimeters, of the tail. The entire tail right up
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
    "taper start": `percentage`,
    "start handle": `percentage`,
    "end handle": `percentage`,
    "taper end": `percentage`
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
    "taper start": `percentage`,
    "start handle": `percentage`,
    "end handle": `percentage`,
    "taper end": `percentage`
}
```


## Machine and Tool definition

Coming soon... only 40 more parameters to document ;)
