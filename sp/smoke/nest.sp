-- nest.sp

((main { 
    "The sum is: "
    (1 2 3 4 5) 
    sum ! 
    <-> <<
    %d nl <<})

(sum 
    {{give
        depth 1 -
        <- {+} ->
        times} 
    nest}))

