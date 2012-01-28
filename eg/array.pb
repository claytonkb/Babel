-- array.pb
-- This program illustrates Babel's array operators

my_leaf: {0 1 2 3 4 5 6 7 8 9}
-- my_interior: [[0][1][2][3][4][5][6][7][8][9]]
my_interior: [0 1 2 3 4 5 6 7 8 9]

code:
   (["Converting an array to a list:\n"] stdout
    [my_interior] [disp] call
    ["... \n"] stdout
    [my_interior] ar2ls [disp] call

    ["\nSlicing a leaf array from 3 to 7:\n"] stdout
    [my_leaf] [disp] call
    ["... \n"] stdout
    [my_leaf] [3] [7] slice
    [disp] call    

    ["\nSlicing an interior array from 3 to 7:\n"] stdout
    [my_interior] [disp] call
    ["... \n"] stdout
    [my_interior] [3] [7] slice
    [disp] call    

    ["\nTruncating a leaf array to 7:\n"] stdout
    [my_leaf] [7] trunc
    [disp] call

    ["\nTruncating an interior array to 7:\n"] stdout
    [my_interior] [7] trunc
    [disp] call

    ["\nLeaf-array length:\n"] stdout
    [my_leaf] size [disp] call

    ["\nInterior-array length:\n"] stdout
    [my_interior] size [disp] call

    ["\nCutting a leaf-array:\n"] stdout
    [my_leaf] [3] cut
    [disp] call [disp] call

    ["\nCatting cut leaf-arrays:\n"] stdout
    [my_leaf] [3] cut cat
    [disp] call

    ["\narray-8 length:\n"] stdout
    ["Hello, world: "] stdout
    ["Hello, world"] [disp] call
    ["array-8 length: "] stdout
    ["Hello, world"] size8 [disp] call

    [my_leaf] dup dup
    ["\nleaf-array s-field: "] stdout
    s [disp] call
    ["leaf-array islf: "] stdout
    islf [disp] call
    ["leaf-array isin: "] stdout
    isin [disp] call

    [[0]] dup dup
    ["\ninterior-array s-field: "] stdout
    s [disp] call
    ["interior-array islf: "] stdout
    islf [disp] call
    ["interior-array isin: "] stdout
    isin [disp] call

    ["\nAllocating a new leaf-array:\n"] stdout
    [3] newlf [disp] call

--    bbl2str doesn't handle interior arrays correctly...
--    ["\nAllocating a new interior-array:\n"] stdout
--    [3] newin [disp] call

    ["\nComparing two leaf-arrays:\n"] stdout
    ["a: "] stdout [a] [disp] call
    ["b: "] stdout [b] [disp] call
    ["a b ~: "] stdout [a] [b] cmp [disp] call
    ["b a ~: "] stdout [b] [a] cmp [disp] call
    ["a a ~: "] stdout [a] [a] cmp [disp] call

)

a: {1 3 5}
b: {2 4 6}

disp: (bbl2str stdout ["\n"] stdout ret)

dump: (bbl2gv ["dump.dot"] spit8 ret)

