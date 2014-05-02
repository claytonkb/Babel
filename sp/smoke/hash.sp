-- hash.sp

((main 
    {(  ("cattle"  "herd"      ) 
        ("eagles"  "aerie"     ) 
        ("fish"    "school"    ) 
        ("geese"   "gaggle"    ) 
        ("gulls"   "screech"   ) 
        ("trout"   "hover"     ))
    mkhash !

    lookup !})

(lookup 
    {{  prompt !
        luha
        2 ith nl <<}
    loop})

(prompt 
    { dup
    "Enter a key> " <<
    >> hash8 })

(remove 
    {{prompt !
        {rmha}
            { "Removed!"}
            { "Nothing removed..."}
        ifte
        nl <<}
    loop})

(exists { { 
        prompt !
        {exha}
            {"Exists!"}
            {"Does not exist"}
        ifte
        nl <<}
    loop})

(mkhash 
    { <- newha ->
    { <- dup ->
        dup 1 ith
        <- 0 ith ->
        inskha }
    each }))


