code:
   (`"car:\n"       <<
    `(1 2 3)        car         disp !

    `"cdr:\n"       <<
    `(1 2 3)        cdr         disp !

    `"cons:\n"      <<
    ``1 `(2 3)      cons dup    disp !

    `"uncons:\n"    <<
                    uncons      disp !   
                                disp !

    `"uncons:\n"    <<
    `(`1 2 3)       uncons      disp ! 
                                disp !

    `"ar2ls:\n"     <<
    `[1 2 3]        ar2ls dup   disp !

    `"bons:\n"      <<
                    bons        disp !

    `"push:\n"      <<
    `(1 2)  ``3     push dup    disp !

    `"pop:\n"       <<
                    pop         disp !
                                disp !

    `"unshift:\n"   <<
    `(2 3)  ``1     unshift dup disp !

    `"shift:\n"     <<
                    shift       disp !
                                disp !)

disp: (($ cr <<))

