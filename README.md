# book-analyzer
Book-Analyzer demo

## Contents

* [Overview](#overview)
* [Design](#design)
* [Quick Start](#quick-start)
* [Run](#run)

# Overview
Book-Analyzer is a tool to inpect Bid-Orders and Ask-Orders from input logs. The tool determines what is the best action to call at a particular timestamp,

# Design

# Scope
- Able to parse from STDIN
- Calculate highest bidders to sell to.
- Calculate lowest seller to buy from.
- Log parsing errors and skip transactions

## Performance
The data is structured in the following queries:
- Handle to hashmaps which are separated by 'side'. This is not easily extendible because logic and handling are couple to support Sell|Buy only. I did not know or see any other type of order so far. 
- Each order hashmap is modeled as `id -> order (id, timestamp, side, price, shares)`. Redundancy in side and id are a tradeof to be able to use the model in the business logic - all in one POJO.
- Pricing index key is price -> { id1, id2, ... idN} sorted by price. This uses a binary tree so O(logN) is expected in the insertions.
- Calculating the action takes O(K) where K is the number of orders that satisfy TARGET_SIZE. Pricing index already avoids to sort all pricing index again.
The algorithm to handle data has the follow asymptotical analysis:

AddOrder: 
- Insert order into database: O(logN)
    - Insert order into order hashmap O(1)
    - Insert in sorted price -> ids O(logN)

ReduceOrder:
- Lookup Order into database: O(1)

Calculate Action:
- Iterate from sorted prices: O(k)
    - foreach K:
        - Lookup id: O(1)


All in all, scaling K will endup with the following Big O : ` O(N * O(logN) * O(K)) `

## Future Extensions
This tool was written in C++ as this is my strongest language and I consider among its feature to handle performance issues very well.
For this scope, the tool may no have the best performance but there are some extensions/features that can be done in the future:

[ ] Extend data to be stored in a external database (redis, filesystem, database server)
[ ] Improve pagination. Currently is written in a way where chunks can be read from a file or stream
[ ] Local Data uses binary trees (set STL) to sort by pricing. Other techniques can be used as quick_sort to simplify the data structure.

# Quick Start

The source code needs to be compiled first. Also the project needs CMake and GoogleTest in order to work.
In the following titles you can build the core program and the unit-tests.

## Install Dependencies

Not all OS may have CMake and Autotools by default. Autotools is needed to build and CMake is needed for GoogleTest package

**Linux**

Check if dependencies are already installed or else install them.

```
which cmake
sudo apt install cmake
```

**MacOS**

Check if dependencies are already installed or else install them.

```
which cmake autoconf automake libtool
brew install cmake autoconf automake libtool
```

## Build project

Project builds with autotools.

```
cd bccp/
./autogen.sh
./configure
make
```

# Run 
After building project, you can run the binary with the following params:

```
./book-analyzer 200 < input.txt
```

Note: the package provide `input.txt` as an example

## Input

This CLI program can handle the following Orders. Please attach to the format as CLI may no handle some parsing errors because that was out of the current scope.
More parsing handling can done in the future contributions.

**AddOrder**

This order type will insert a new order in the database.
Book-Analyzer will calculate action depending on how many Orders are in the database.

```
$timestamp A $id $side $price $shares
```

timestamp: given timestamp of the day.
A : characte 'A' which represents the AddOrder type
id : order id
side : either S for Sell, B for buy
price : price per share
shares : qty of shares in the order (order size)

**ReduceOrder**
This order type will remove shares from previous inserted orders in the database.

```
$timestamp R $id $shares
```

timestamp: given timestamp of the day.
R : characte 'R' which represents the ReduceOrder type
id : order id
shares : qty of shares to remove

## Output

Book-Analyzer will calculate the best actions and throw them via stdout.
The tool will only throw a new action when the database has change on the following:
- When Orders are insufficient (< TARGET_SIZE shares) after have reported.
- Total Expense|Income changed over the TARGET_SIZE threshold

```
$timestamp $action $total
```
timestamp: time of the day when action is possible
action: Sell|Buy 
total: total cost|income if the action is performed

## Errors

The errors do not terminate the program, instead skip the troublesome order.
A warning may be shown.
