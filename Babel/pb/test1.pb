-- Comments

test:
    ( {1}
    ( {2}
    ( {3} 
    )))

namespace:
    { 1 2 3 4 }
-- Ends with undent
-- namespaces don't nest

main1:
    ( { 13 } )

main: -- obj construction begins at "main" entrypoint
    (
        ( {1} {2} *my_label ) -- OK

        ( my_label foo bar baz ) -- OK

        ( {1} {2} { 3 4 } ) -- OK

        ( {1 2} {3} {4} ) -- OK
        ( {1} {2} {3 4} ) -- OK

    )

this_is_a_label: 
{
    "This is the first element of a list..."
    "2nd element"
    0x13
    "etc."
}

this_is_a_definition: 
    {23} -- decimal 23
string_definition: 
    {"this is a C-style string"}

hex_definition: 
    {0x42}

-- Also, need a switch to generate .c file output...

