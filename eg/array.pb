-- array.pb
-- This program illustrates Babel's array operators

my_leaf: {0 1 2 3 4 5 6 7 8 9}
-- my_interior: ((0)(1)(2)(3)(4)(5)(6)(7)(8)(9))
my_interior: [0 1 2 3 4 5 6 7 8 9]

main:
   (`"Converting an array to a list:\n" <<
    `my_interior disp !
    `"... \n" <<
    `my_interior ar2ls disp !

    `"\nSlicing a leaf array from 3 to 7:\n" <<
    `my_leaf disp !
    `"... \n" <<
    `my_leaf `3 `7 slice
    disp !

    `"\nSlicing an interior array from 3 to 7:\n" <<
    `my_interior disp !
    `"... \n" <<
    `my_interior `3 `7 slice
    disp !

    `"\nTruncating a leaf array to 7:\n" <<
    `my_leaf `7 trunc
    disp !

    `"\nTruncating an interior array to 7:\n" <<
    `my_interior `7 trunc
    disp !

    `"\nLeaf-array length:\n" <<
    `my_leaf size disp !

    `"\nInterior-array length:\n" <<
    `my_interior size disp !

    `"\nCutting a leaf-array:\n" <<
    `my_leaf `3 cut
    disp ! disp !

    `"\nCatting cut leaf-arrays:\n" <<
    `my_leaf `3 cut cat
    disp !

    `"\narray-8 length:\n" <<
    `"Hello, world: " <<
    `"Hello, world" disp !
    `"array-8 length: " <<
    `"Hello, world" size8 disp !

    `my_leaf dup dup
    `"\nleaf-array s-field: " <<
    s disp !
    `"leaf-array islf: " <<
    islf disp !
    `"leaf-array isin: " <<
    isin disp !

    ``0 dup dup
    `"\ninterior-array s-field: " <<
    s disp !
    `"interior-array islf: " <<
    islf disp !
    `"interior-array isin: " <<
    isin disp !

    `"\nAllocating a new leaf-array:\n" <<
    `3 newlf disp !

    `"\nAllocating a new interior-array:\n" <<
    `3 newin disp !

    `"\nComparing two leaf-arrays:\n") <<
    `"a: " << (a) disp !
    `"b: " << (b) disp !
    `"a b ==: ") << (a) (b) == disp !
    `"b a ==: ") << (b) (a) == disp !
    `"a a ==: ") << (a) (a) == disp !

)

a: {1 3 5}
b: {2 4 6}

disp: (($ cr <<))

