# Pseudo-Hilbert Curve

CPSC 453 Homework 1. A discrete (pseudo) Hilbert curve in a **1024×1024** SFML 3 window. Construction and naming follow Grant Sanderson (3Blue1Brown), [Hilbert's Curve: Is infinite math useful?](https://www.youtube.com/watch?v=3s7h2MHQtxc) (2017).

## What the video is saying

A **true** Hilbert curve is the infinite limit: a continuous path that fills a square. Any finite order is only a **pseudo-Hilbert curve**. Raising the order does not jump to a new shape; a point already on the path just moves a little closer to its limiting location in the square.

That is why Hilbert curves keep **locality**. Points that are close along the 1D path stay close in 2D. That is useful when you store 2D data in a 1D list (for example mapping a 256×256 image to a line, then a 1080p image, without scrambling neighbours).

None of the finite curves we draw is space-filling. Zoom into a pixel and the polyline only cuts a slice of it. The infinite limit is the object the video is really defining.

## How the curve is built (from notes)

1. Divide the square into a grid (order 1 is 2×2).
2. Trace the quadrant centres in a U: bottom-left → top-left → top-right → bottom-right.
3. To go from order *n*−1 to *n*, subdivide again (2×2 → 4×4, and so on).
4. Place a copy of the smaller curve in each quadrant. Grid width of each copy is *L* = 2^{n−1}.
5. Flip the lower-left (+90° / transpose) and lower-right (−90°) copies so the four pieces connect tip-to-tail.

Order *n* lives on a 2^n × 2^n grid and visits 4^n points. This window is 1024×1024, and 2^{10} = 1024, so the highest useful order is **n = 10**. Beyond that, extra points land on the same pixels.

## What the program does

- Starts at **order 3**.
- **↑ / +** increase order, **↓ / −** decrease it, clamped to 1…10.
- Points go into an SFML `VertexArray` (`LineStrip`): each vertex is a position plus a colour. SFML cannot draw raw `(x, y)` lists.
- `sf::View` maps the grid into the window. Negative view height flips Y so (0, 0) is the bottom-left, matching the notes.
- Margin is **10% on each side at every order**, so the curve keeps a constant size in the 1024×1024 window. Only the amount of detail changes.

## Build

```powershell
cmake -B build
cmake --build build
./build/bin/main
```
