# Dice

Roll dice, or select a random number from an interval.

## Installation

```sh
$ make
$ sudo make install
```

## Usage

```sh
# roll 2 six-sided dice
$ dice 2d6

# select a random number between -5 and 5 (inclusive)
$ dice -5..5

# combine rolls
$ dice 2d6 1d20 -5..5
```
