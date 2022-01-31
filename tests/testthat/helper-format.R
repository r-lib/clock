test_all_formats <- function(zone = TRUE) {
  out <- c(
    "C: %C",
    "y: %y",
    "Y: %Y",
    "b: %b",
    "h: %h",
    "B: %B",
    "m: %m",
    "d: %d",
    "a: %a",
    "A: %A",
    "w: %w",
    "g: %g",
    "G: %G",
    "V: %V",
    "u: %u",
    "U: %U",
    "W: %W",
    "j: %j",
    "D: %D",
    "x: %x",
    "F: %F",
    "H: %H",
    "I: %I",
    "M: %M",
    "S: %S",
    "p: %p",
    "R: %R",
    "T: %T",
    "X: %X",
    "r: %r",
    "c: %c",
    "%: %%"
  )

  if (!zone) {
    return(out)
  }

  c(
    out,
    "z: %z",
    "Ez: %Ez",
    "Z: %Z"
  )
}
