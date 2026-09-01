# Maximum Cut Depth Design and Workplan

## Status

Design agreed; implementation is in progress.

This document tracks the design and implementation plan for limiting the
depth of cut in MonkeyCAM's deep-cut core programs.

| Slice | Status |
| --- | --- |
| 1. Tool Configuration and Depth Planner | Complete |
| 2. Single-Path Deep-Cut Programs | Complete |
| 3. Layered Top Profile Path Sets | Complete |
| 4. Documentation, Examples, and End-to-End Verification | Not started |

## Goals

- Allow different cutters to have different maximum depths of cut.
- Give every deep-cut program a consistent, optional minimum pass count.
- Apply the limit to the deep-cut programs that opt into this behavior.
- Preserve the canonical sample G-Code by making its historical pass counts
  explicit and omitting tool maximums.
- Preserve every program's final geometry, including the Top Cutout tabs and
  the full 3D Top Profile.
- Keep intermediate cutting behavior predictable and auditable in the
  generated G-Code.

## Scope

The following programs will honor the selected tool's maximum depth of cut:

- Edge Trench
- Splitboard Center Trench
- Core Top Cutout
- Core Top Profile

The following are explicitly not part of this feature:

- Core Edge Groove
- Guide holes, insert holes, and alignment marks
- Base and nose/tail spacer cutouts
- Any other program that does not explicitly opt into depth planning
- The existing experimental Top Profile roughing implementation

The Splitboard Center Trench is included even though it is emitted only for
splitboards.

## Configuration Decision: Tool-Specific, Selectively Applied

The maximum depth of cut will be an optional property of a tool definition:

```json
{
  "id": 4,
  "name": "1.5 Inch Straight",
  "diameter": "1.496in",
  "G-code #": 1,
  "max cut depth": "2mm"
}
```

`max cut depth` is a positive length. As with other machine configuration
lengths, a unitless value defaults to inches, so values such as 2 mm should be
written explicitly as `"2mm"`.

The property belongs to the tool because cutter diameter and geometry are
major factors in a safe depth of cut. The tool entries are already part of a
specific machine/process configuration, which also gives the value the
necessary machine and material context. This lets a large Top Profile cutter
have a shallow limit while a small spiral cutter has a different, potentially
deeper limit.

The property is not enforced automatically by `GCodeWriter`. A generator must
explicitly use depth planning. This selective application is intentional:
many programs already make shallow cuts, drill holes using specialized
behavior, or have other cutting semantics for which this option is not
appropriate.

This is preferred over the alternatives:

- A global value is too coarse for cutters with very different capabilities.
- Program-specific values duplicate the same cutter constraint across every
  program using that cutter and can drift apart accidentally.
- A program-specific override could be added in the future if a real use case
  requires it, but it is not part of this feature.

In the current engine, Edge Trench, Splitboard Center Trench, and Top Cutout
all use `core cutout tool`. Top Profile uses `top profile tool`. Consequently,
the common setup needs at most two independently configured tool limits.

## Program Pass-Count Configuration

Every deep-cut program will have an optional positive integer pass count:

- `edge trench passes` controls both Edge Trench and Splitboard Center Trench.
- `core cutout passes` continues to control Top Cutout.
- `top profile passes` controls Top Profile.

For example, the canonical sample can make its current behavior explicit with:

```json
"edge trench passes": 3,
"top profile passes": 1,
"core cutout passes": 2
```

Edge Trench and Splitboard Center Trench share one setting because they use the
same cutter, start at `core blank thickness`, finish at table-top Z = 0, and
currently share the same hardcoded three-level behavior.

All three settings default to one when omitted. A value of zero or less is
invalid. `core cutout passes` is no longer a required machine parameter, but
existing configurations that specify it retain their current intent.

## Compatibility and Precedence

Each program first establishes its configured minimum pass count, using the
default of one when its option is absent. The selected tool's `max cut depth`
then determines whether more passes are required. The effective pass count is:

```text
max(configured passes or 1, ceil(total removal / maximum cut depth))
```

When the selected tool has no maximum, the effective pass count is simply the
configured pass count or one. In other words:

- Passes only: use the configured number of passes.
- Maximum only: use at least one pass and add as many as the maximum requires.
- Both: treat the configured number as the minimum and add passes if needed.
- Neither: use one pass.

The tool maximum may add passes but never remove passes specified by the
program option.

This regularizes one existing option, one hardcoded value, and one implicit
single pass. It has one deliberate compatibility consequence: an existing
machine configuration that omits the new `edge trench passes` option will use
one pass instead of the historical hardcoded three. The canonical sample
machine configuration will explicitly set `edge trench passes` to 3 so its
Edge Trench and Splitboard Center Trench motion remains unchanged. It may also
set `top profile passes` to 1 for clarity, although omission has the same
behavior.

A configured maximum depth of zero or less, or a configured pass count of zero
or less, is invalid and must produce a clear configuration error before G-Code
is generated.

## Shared Depth-Planning Semantics

For a path or complete path collection:

1. Start at the known top of the uncut material.
2. Find the deepest final Z coordinate in the operation.
3. Compute the total removal as `start Z - deepest final Z`.
4. Read the program's configured pass count, defaulting to one.
5. If the selected tool has a maximum, compute the levels required by it as
   `ceil(total removal / maximum cut depth)`, with at least one level.
6. Use the larger of the configured/default pass count and the result required
   by the maximum. With no maximum, use the configured/default count directly.
7. Distribute the total removal as evenly as fixed-point precision allows
   across that number of levels. When a maximum is configured, no level
   increment may exceed it.
8. For each intermediate level, clamp every final path point upward to the
   current level ceiling:

   ```text
   emitted Z = max(final Z, current level Z)
   ```

9. Emit the original, unclamped Z coordinates on the final level so the final
   geometry is exact.

Level coordinates should be calculated cumulatively from the start and final
depths, rather than by repeatedly subtracting a rounded increment. This keeps
fixed-point rounding from accumulating or making the final increment larger
than the configured maximum.

The limit describes material removed between corresponding depth levels. A
lead-in may contain a larger numerical Z movement from rapid height because
that portion of the movement is through air before it reaches the stock.

### Operation Starting and Final Depths

- Edge Trench and Splitboard Center Trench start at `core blank thickness`
  and finish at table-top Z = 0.
- Top Cutout starts at `core blank thickness`; its deepest final coordinate is
  the cut-through depth below table top. Shallower tab coordinates stop
  descending once their final height is reached.
- Top Profile starts at `core blank thickness`; its deepest final coordinate
  comes from the generated 3D profile paths.

## Top Profile Ordering Decision

Top Profile will be emitted depth level first:

1. Clamp every Top Profile path to the first depth ceiling.
2. Emit the complete existing inside-out path sequence at that level.
3. Retract and use the existing lead-in to begin the next level.
4. Repeat until the final, unclamped path collection is emitted.

It will not take one contour to full depth before starting the next contour.
Doing so would leave a tall wall beside the next contour. The side of the
cutter could then engage that wall over substantially more than the configured
depth even when the cutter's downward increment is limited. Completing the
whole pocket at each level keeps both axial and side engagement close to the
intended limit.

The existing behavior within each level remains unchanged:

- Contours run inside-out.
- Neighboring contours use the Top Profile transition feed.
- The configured lead-in is used when entering an island.
- Disconnected paths retract and rapid as they do now.

## Implementation Slices

### Slice 1: Tool Configuration and Depth Planner

- Add an optional `maxCutDepth` value to `Tool`.
- Parse optional `max cut depth` from each tool definition.
- Validate that a present value is positive.
- Make `core cutout passes` optional with a default of one.
- Add optional `edge trench passes` and `top profile passes`, each with a
  default of one.
- Validate that every configured pass count is a positive integer.
- Add a shared, fixed-point depth-level planner that accepts start Z, deepest
  final Z, optional maximum depth, and the configured/default minimum pass
  count.
- Cover the planner with dependency-free tests for:
  - a total depth smaller than the maximum;
  - exact division;
  - a remainder;
  - passes only, maximum only, both, and neither;
  - a non-binding maximum that preserves the configured pass count and
    schedule;
  - a binding maximum that adds levels;
  - fixed-point rounding near a level boundary;
  - exact final depth;
  - rejection of zero and negative limits.
- Confirm that this slice alone changes no generated G-Code.

### Slice 2: Single-Path Deep-Cut Programs

- Add a max-depth form of the spiral/depth-layer path emitter while retaining
  the existing pass-count form and treating the program's configured/default
  pass count as the minimum.
- Apply it to Edge Trench using `edge trench passes` and the optional maximum
  on `core cutout tool`.
- Apply the same settings to Splitboard Center Trench.
- Apply it to Top Cutout using `core cutout passes` and the optional maximum on
  `core cutout tool`.
- Preserve the original final paths and Top Cutout tab heights exactly.
- Add optional G-Code header comments identifying the configured maximum and
  calculated number of levels so users can audit the result.
- Verify legacy and configured behavior for snowboard, ski, and splitboard
  examples.

### Slice 3: Layered Top Profile Path Sets

- Add path-set depth emission that operates on copies of the final paths and
  leaves the originals unchanged.
- Plan levels from `core blank thickness` to the minimum Z across the complete
  final Top Profile path collection, using `top profile passes` and the
  optional maximum on `top profile tool`.
- For every level, clamp the complete collection and invoke the existing
  inside-out path-set emitter.
- Establish the canonical contour and lead-in X/Y point sequences once, before
  applying per-level Z clamping. Path cleanup and lead-in construction must not
  independently add or remove X/Y points at different levels because their 3D
  segment lengths changed.
- Start each level safely with the existing retract and lead-in behavior.
- Emit the original path collection on the final level.
- Keep the current single-emission behavior when `top profile passes` is one
  and the tool maximum does not require more passes.
- Verify that every point's removal between levels is within the maximum and
  that the final 3D coordinates match the legacy output.

### Slice 4: Documentation, Examples, and End-to-End Verification

- Document `max cut depth` in the tool-definition section of the Configuration
  Guide, including its positive-length and explicit-units requirements.
- Update the G-Code Program Guide for all four programs in scope.
- State explicitly that the property is selectively honored, not globally
  enforced for every use of a tool.
- Add a dedicated machine configuration fixture with different limits on the
  small core cutout tool and large Top Profile tool. Keep maximum-depth values
  out of the canonical sample configuration unless there is a deliberate
  decision to change all checked-in sample output.
- Add `edge trench passes: 3` to the canonical sample machine configuration so
  the new explicit option preserves the historical sample output. Optionally
  add `top profile passes: 1` to make that default visible.
- Add a repeatable comparison script that generates output into temporary
  directories, normalizes expected metadata, and compares both normalized text
  and parsed G-Code motion coordinates.
- Build the engine with the Docker build and run the board, ski, and splitboard
  examples.
- Compare no-option output against the current G-Code, ignoring only expected
  generated metadata such as timestamps.
- For configured output, verify calculated level counts, maximum depth between
  levels, final bounding boxes, final path coordinates, and Top Cutout tabs.

## G-Code Regression and Geometry Verification

The feature will use the same regression technique historically used for
MonkeyCAM changes: run the sample definitions, compare the results with
`Sample-output`, and account only for known generated metadata such as the
timestamp and Git identity. The repeatable comparison must include the sample
snowboard and ski. It must also include the sample splitboard so the
Splitboard Center Trench is exercised.

The comparison should have two layers:

1. A normalized text diff removes only an explicit allow-list of volatile or
   intentionally added comment lines. Any other textual difference is shown.
2. A G-Code motion parser compares the ordered motion commands, coordinates,
   and feeds. This prevents comment normalization from hiding a real toolpath
   change.

X and Y should compare exactly at the precision written to G-Code. Z should
also compare exactly for omitted and non-binding configurations. A tolerance
of at most one final printed unit may be used for a newly calculated legacy-
equivalent depth schedule if fixed-point distribution makes exact textual Z
parity impractical. Final target Z coordinates must always compare exactly.

### Case A: Maximum Omitted, Explicit Sample Pass Counts

- Update the canonical example machine configuration with
  `edge trench passes: 3`, retain its existing `core cutout passes`, and
  optionally make `top profile passes: 1` explicit.
- Leave `max cut depth` absent from every tool.
- Run the sample snowboard, ski, and splitboard.
- Compare every generated `.nc` file with its checked-in counterpart.
- Require identical parsed motion commands, X/Y/Z coordinates, feeds, file
  set, and motion counts.
- Allow differences only in known metadata such as generation time, branch,
  and commit hash.

This is the primary proof that replacing implicit and hardcoded pass counts
with explicit configuration, and adding tool configuration and new emitters,
has not changed the canonical sample output.

### Case B: Maximum Present but Non-Binding

- Create a temporary copy of the sample machine configuration.
- Add `max cut depth` to every tool, including tools used only by programs
  outside this feature.
- Choose each value above the largest depth increment currently emitted by
  every in-scope program using that tool.
- Run the sample snowboard, ski, and splitboard.
- Require the parsed G-Code motion for every file to be identical to Case A.
- Treat newly added audit comments or echoed configuration as non-motion
  differences, but show them in the normalized diff allow-list.

This case verifies both halves of the contract: a non-binding maximum does not
change an existing depth schedule, and programs outside the explicit scope
ignore the property even when their selected tool has one.

### Case C: Pass-Count and Maximum Precedence

Use focused temporary configurations to exercise all four combinations for
each deep-cut program:

- Neither passes nor maximum: require exactly one pass.
- Passes only: require exactly the configured number of passes.
- Maximum only: start with one pass and add the number required by the maximum.
- Both: require the larger of configured passes and the number required by the
  maximum.

Also verify that zero and negative pass counts fail configuration validation.
For Edge Trench and Splitboard Center Trench, verify that the shared
`edge trench passes` value controls both programs.

### Case D: Existing Spiral-Path Parity

Exercise Edge Trench, Splitboard Center Trench, and Top Cutout with maximums
chosen near their existing per-level depth. Separate temporary configurations
may be used when one value on the shared `core cutout tool` cannot be close to
the legacy increments of all programs at once.

For each target program:

- Choose a maximum that retains its configured historical sample pass count.
- Require the ordered X/Y coordinates and motion types to be identical.
- Require feeds and the final Z coordinates to be identical.
- Compare every intermediate Z coordinate and require exact equality where
  possible, otherwise no more than one final printed unit of difference.
- Confirm the Top Cutout tab X/Y positions and final tab Z values are exact.

This demonstrates that max-depth planning agrees with the established
`emitSpiralPath()` behavior at equivalent level counts.

### Case E: Binding Maximum Adds Spiral Levels

- Configure a maximum small enough to add levels to Edge Trench, Splitboard
  Center Trench, and Top Cutout.
- Split the parsed motion into depth passes.
- Require every pass to use the canonical path's exact X/Y sequence.
- Require every level increment to be within the configured maximum.
- Require the last pass to reproduce the legacy final X/Y/Z coordinates
  exactly.

### Case F: Top Profile Coordinate Parity

Run these Top Profile configurations:

- With passes omitted and no maximum, require the complete parsed motion to be
  identical to the legacy single emission.
- With `top profile passes: 1` and a non-binding maximum, require the complete
  parsed motion to be identical to the legacy single emission.
- With `top profile passes` greater than one and no maximum, require exactly
  that many complete depth levels.
- With a binding maximum, split the output into complete depth levels and
  compare each level with the legacy Top Profile path collection.

For every binding-max Top Profile level:

- Require the contour and lead-in X/Y coordinate sequences to be identical to
  the canonical legacy sequence. Additional retract and return moves between
  whole levels are expected and should be compared separately.
- Require unchanged contour order, island order, feeds, and transition moves.
- Verify that Z is the canonical final value clamped to that level's ceiling.
- Verify that corresponding points never descend by more than the configured
  maximum between levels.
- Require the final level's complete X/Y/Z path collection to equal the legacy
  final path collection exactly.

This case is also the acceptance test for preprocessing paths and lead-ins
only once: different depth clamping must never change their X/Y topology.

## Expected Example Behavior

Using the current sample's 0.420-inch (10.668 mm) core blank and a 2 mm
maximum illustrates the expected planning:

- Edge Trench to table top: 6 levels.
- Splitboard Center Trench to table top: 6 levels.
- Top Cutout through the blank and 0.010 inch below table top: 6 levels.
- Top Profile down to a 2 mm minimum core thickness: 5 levels.
- Core Edge Groove remains unchanged because it is outside this feature.

The exact intermediate coordinates may be slightly less than 2 mm apart
because the total removal is distributed evenly. No increment may exceed the
configured maximum, and the final level must land exactly on the original
target geometry.

## Completion Criteria

The feature is complete when:

- Each of the four programs honors the selected tool's optional maximum.
- Different tools can use different values in one machine configuration.
- Programs outside the explicit scope ignore the property.
- Every deep-cut pass option is optional and defaults to one.
- The canonical samples' explicit pass settings with no tool maximum preserve
  current G-Code movements.
- Non-binding values on every sample tool preserve current G-Code movements.
- Every configured depth increment is within the maximum.
- All final paths and cut depths are identical to the existing final geometry.
- The coordinate-level regression cases in this document pass.
- The engine builds and all board, ski, and splitboard verification cases pass.
- User-facing configuration and program documentation is updated.
