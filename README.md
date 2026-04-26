# Radical Framework

This is the Radical game development framework, which is a *batteries included* framework implemented in C, intentionally targeting older, legacy hardware. For fun, of course. This is nothing serious, I'm just curious to see what *modern* coding practices and legacy targeting will get us. This is free, open-source software; licensed under the [BSD 3 Clause license.](./LICENSE)

## Why?

Purely for fun! If you want to contribute, you're welcome to. If you've got a weird target you want to try and port this to... try it out! If it works, contribute it! Maybe you want to use this for a commercial project? You can. Under BSD 3, you absolutely can use this commercially. Remember, *no warranty!*

## Building?

If you want to build from source, right now the only option is through CMake, since it *just plain works* and is completely fine for the project. Though as targets get included, these instructions may diverge. For now though, just run these:

```sh
user@pc radical $ cmake -S . -B Build [optional flags]
user@pc radical $ cmake --build build
```

And in a few moments, you'll have the entire framework built!
