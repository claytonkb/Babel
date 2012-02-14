--each.pb

code:  ([(car pr ["\n"] pr)] 
            [[("Bob") ("John")  ("Brad") ("Gary") 
              ("Joe") ("Clark") ("Bill") ("Jerry")]] 
        eachar

        ["\n"] pr

          [(down
                [0x12345678] 
                [0x87654321] 
            up
            eval
            show ["\n"] cat8 pr)]
          [((cand)      (cor)       (cxor)
            (cxnor)     (cnor)      (cnand)
            (cnot)      ([4] cushl) ([4] cushr)
            ([4] curol) ([4] curor) ([4] cashr))]
        each)

