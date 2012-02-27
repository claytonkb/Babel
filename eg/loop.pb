code:  (("Ctl+C to quit\n")             <<
            (( ("Type something\n> ")   <<
            >> dup
            ("quit") ==
                (( continue ))
                ((  ("You typed: ")     <<
                cr                      << ))
            ? ! ("Aha\n") << ))
        loop   
        ("Done.")                       <<)

