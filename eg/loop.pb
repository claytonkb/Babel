code: (
        ("Ctl+C to quit\n") <<
            (( ("Type something\n> ") <<
            >> dup
            ("quit") ==
                (( break ))
                ((  ("You typed: ") <<
                cr << ))
            ? ! ))
        loop   
        ("Done.") << die)

