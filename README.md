# Hedens Game of Decay

> Conways Game of Life... with a twist

## Rest In Peace John Horton Conway

I started this particular implementation project with the standard name of "Conway's Game of Life" - and very shortly after I found out that he recently died due to complications of COVID-19. My heart goes out to his family first, but also every other nerd or geek whose life has been altered in some amazing way by this man.

## Compiling

Currently in this very early implementation, no `Makefile` exists. But compiling should be rather easy with a standard tool chain:

```
cd src
gcc game-of-decay.c -o game-of-decay
./game-of-decay
```

## Executing

I think the most important flag at this time to mention is `-R`. When nothing is specified, the range is `0-1` (e.g.: original Conways Game of Life). Specify your range and let 'er rip.

## Original Rules (Conways Game of Life)

An infinite, two-dimensional orthoganal grid of square cells, each of which is one of two possible states, **alive** or **dead**. Every cell interacts with its eight neighbors, which are the cells that are horizontally, vertically, or diagonally adjacent. At each step in time, the following transitions occur:

* Any live cell with fewer than two live neighbors dies
* Any live cell with two or three live neighbors lives on to the next generation
* Any live cell with more than three neighbors dies
* Any dead cell with exactly three live neighbors becomes a live cell

An initial pattern is fed to the system. This is considered the seed. The first generation is created by applying the above rules simultaneously to every cell in the seed; births and deaths occur simultaneously, and the discrete moment at which this happens is sometimes called a *tick*. The rules continue to be applied repeatedly to create further generations.

Most of this was copied or paraphrased from the Wikipedia entry on "Conway's_Game_of_Life" Rules section. If I had the ability to copy and paste it into the VM I'm using, I would've done that instead.

## The New Rules (Hedens Game of Decay)

* Much like real life, there is now a range between **alive** and **dead**. This range is arbitrarily long (currently the absolute minimum is 0, and the maximum is 9 - this is due to the rudimentary display of this early implementation)
    * The range will be given in the format X-Y (where X is **dead**, and Y is **alive**)
    * X must be smaller than Y
    * X and Y are integer values
    * Everything between X and Y is some point of decay
    * A range 0-1 will mimic the original Game of Life perfectly
* In the original there are 3 transformations: birth, remain alive, and death
* In the new - there are now ***4*** transformations: birth, remain alive, decay, and death (although death is by default a slow descent through decay)
    * If we give a range of 0-1 for the original Conway's Game of Life - then it should be sufficient to assume:
        * `< 200%` neighbor value (2 live cells) will kill a live cell
        * `>= 200%` neighbor value (2 live cells) and `<= 300%` (3 live cells) will keep a cell alive
        * `== 300%` (3 live cells) will bring a dead cell to life
        * `> 300%` (3 live cells) will kill a live cell
    * We can adjust these slightly to account for decay:
        * `< 200%` or `> 300%` of a neighbor value will decay a living cell, eventually to death (by decrementing its value by 1)
* A neighbor value is defined as the total value of all of the eight neighbors of any given cell
* We compare the current value of the cell against the percentages for most computations
* When bringing a cell alive, all possible multiples of values in the range are tested before deciding on the new life value
    * 3 values of 4 (a total neighbor value of 12) cannot bring a cell with the value of 5 to life

### A Very Brief Example

Given a range of 0-6, suppose we look at the cell in the center (the 4 ...in the center...):

```
1 1 1
2 4 2
0 0 0
```

Since all of the values of the cells immediately surrounding the 4 are less than 200% of its value (they total 7, 200% would be 8) - this 4 will become a 3 on the next iteration.
