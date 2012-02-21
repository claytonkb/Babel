code: (
        ("Ctl+C to quit\n") <<
            (( ("Type something\n> ") <<
            >> dup
            ("quit") ==
                (( last ))
                ((  ("You typed: ") <<
                cr << ))
            ? ! ))
        loop
        ("Done.") <<)

