# N64 Raytracer sample on modern toolchain

Tested with GCC 9.8.0 + libstdc++ + newlib.

GLM needs to be patched for this to compile
`define GLM_HAS_EXTENDED_INTEGER_TYPE' 1 => 0
at glm/glm/detail/setup.hpp:173

Writes text one character at the time to COP0 register 25. You can hack cen64
to dump the characters as they are written to the register.

## How to build in three easy steps:

1. Compile yourself a toolchain

   (This may involve ritualistic sacrifice and/or supernatural pacts. Read the fine print.)

2. Patch the mentioned digit in the GLM code
3. Run make
4. Be confused because I forgot to mention the ROM header blob that I won't distribute, and the hardcoded path to a tool from n64chain.
5. Fix both of these issues
6. Do step 3 again
7. Get a linker error on -lfmt

   While following these instructions to make sure it builds, I found out that I forgot that I had built libfmt into a static lib somehow. I'm sure you can figure it out yourself if you've gotten this far.

## I got lost in the first step; just show me a picture
![It's a sphere](https://github.com/einhov/n64-rtsample/raw/master/test.png "It's a sphere")
